//
// Tournament.cpp
// Engine Layer - Tournament simulation orchestrator
//

#include "engine/Tournament.h"
#include <iostream>
#include <cmath>
#include <cassert>

using namespace std;

// ============================================================
//  CONSTRUCTION
// ============================================================

Tournament::Tournament(const string& name, TournamentFormat format,
                       const vector<const Team*>& teams,
                       int prizePool, const string& location,
                       const string& date, const string& tier)
        : name_(name), format_(format), teams_(teams),
          prizePool_(prizePool), location_(location),
          date_(date), tier_(tier) {

    // Validate team count based on format
    if (format == TournamentFormat::Groups) {
        if (teams.size() != 16) {
            cerr << "Error: Groups format requires exactly 16 teams. Got " << teams.size() << endl;
        }
    } else if (format == TournamentFormat::Bracket) {
        // Bracket needs power of 2
        int n = (int)teams.size();
        if (n < 2 || (n & (n - 1)) != 0) {
            cerr << "Error: Bracket format requires power-of-2 teams. Got " << n << endl;
        }
    }

    seedTeams();
}

// ============================================================
//  SEEDING
// ============================================================

void Tournament::seedTeams() {
    seededTeams_ = teams_;
    sort(seededTeams_.begin(), seededTeams_.end(), [](const Team* a, const Team* b) {
        return a->getRanking() < b->getRanking(); // Lower ranking number = higher seed
    });
}

// ============================================================
//  CORE SIMULATION
// ============================================================

void Tournament::simulate() {
    if (simulated_) {
        cerr << "Tournament already simulated." << endl;
        return;
    }

    if (format_ == TournamentFormat::Bracket) {
        simulateBracket();
    } else if (format_ == TournamentFormat::Groups) {
        simulateGroupsAndPlayoffs();
    }

    simulated_ = true;
}

// Run a single match, track stats, store in allMatches_
MatchResult Tournament::runMatch(const Team* team1, const Team* team2) {
    MatchResult result = MatchSimulator::simulate(*team1, *team2);

    // Track player aggregate stats
    for (const auto& entry : result.playerStats) {
        playerStats_[entry.first].addMatch(entry.second);
    }

    allMatches_.push_back(result);
    return result;
}

// ============================================================
//  BRACKET FORMAT (Single Elimination)
// ============================================================

void Tournament::simulateBracket() {
    // Seeded bracket: 1v16, 8v9, 5v12, 4v13, 3v14, 6v11, 7v10, 2v15
    vector<const Team*> currentRound = seededTeams_;

    // Track losers per round for placement ordering
    vector<vector<const Team*>> roundLosers;

    while (currentRound.size() > 1) {
        vector<const Team*> nextRound;
        vector<const Team*> thisRoundLosers;
        size_t numMatches = currentRound.size() / 2;

        // Determine round name
        string roundName;
        if (currentRound.size() == 2) roundName = "FINAL";
        else if (currentRound.size() == 4) roundName = "SF";
        else if (currentRound.size() == 8) roundName = "QF";
        else roundName = "RO" + to_string(currentRound.size());

        for (size_t i = 0; i < numMatches; i++) {
            const Team* team1 = currentRound[i];
            const Team* team2 = currentRound[currentRound.size() - 1 - i];

            string label = roundName + " " + to_string(i + 1);
            MatchResult result = runMatch(team1, team2);

            const Team* winner = (result.winnerName == team1->getName()) ? team1 : team2;
            const Team* loser = (winner == team1) ? team2 : team1;

            PlayoffMatch pm;
            pm.roundName = label;
            pm.result = result;
            pm.winner = winner;
            pm.loser = loser;
            playoffMatches_.push_back(pm);

            nextRound.push_back(winner);
            thisRoundLosers.push_back(loser);
        }

        roundLosers.push_back(thisRoundLosers);
        currentRound = nextRound;
    }

    // Champion
    champion_ = currentRound.front()->getName();

    // Build placements: champion first, then losers from later rounds first
    placements_.push_back(currentRound.front());
    for (int r = (int)roundLosers.size() - 1; r >= 0; r--) {
        for (const Team* t : roundLosers[r]) {
            placements_.push_back(t);
        }
    }
}

// ============================================================
//  GROUPS + PLAYOFFS FORMAT
// ============================================================

void Tournament::simulateGroupsAndPlayoffs() {
    // Step 1: Assign teams to 4 groups via snake draft
    vector<vector<const Team*>> groups = assignGroups();

    // Step 2: Run group stage (round-robin within each group)
    string groupNames[] = {"Group A", "Group B", "Group C", "Group D"};
    for (int i = 0; i < 4; i++) {
        GroupResult gr = runGroupStage(groupNames[i], groups[i]);
        groupResults_.push_back(gr);
    }

    // Step 3: Collect qualified teams (top 2 from each group)
    // qualifiedTeams_ order: A1, A2, B1, B2, C1, C2, D1, D2
    for (const auto& gr : groupResults_) {
        qualifiedTeams_.push_back(gr.standings[0].team);  // 1st place
        qualifiedTeams_.push_back(gr.standings[1].team);  // 2nd place
    }

    // Step 4: Build playoff bracket
    // Matchups: A1 vs B2, C1 vs D2, B1 vs A2, D1 vs C2
    vector<const Team*> playoffSeeding = {
            qualifiedTeams_[0], qualifiedTeams_[3],  // A1 vs B2
            qualifiedTeams_[4], qualifiedTeams_[7],  // C1 vs D2
            qualifiedTeams_[2], qualifiedTeams_[1],  // B1 vs A2
            qualifiedTeams_[6], qualifiedTeams_[5],  // D1 vs C2
    };

    runPlayoffBracket(playoffSeeding);

    // Step 5: Build full placements
    // Champion and runner-up from playoffs, then 3rd-4th (SF losers),
    // then 5th-8th (QF losers), then group stage eliminated teams by group standing
    // Playoff placements are already handled in runPlayoffBracket

    // Add eliminated group stage teams (3rd and 4th place from each group)
    for (const auto& gr : groupResults_) {
        for (size_t i = 2; i < gr.standings.size(); i++) {
            placements_.push_back(gr.standings[i].team);
        }
    }
}

// Snake draft seeding into 4 groups
// Seeds 1-16 distributed as:
//   Row 1: A B C D  (seeds 1,2,3,4)
//   Row 2: D C B A  (seeds 5,6,7,8)
//   Row 3: A B C D  (seeds 9,10,11,12)
//   Row 4: D C B A  (seeds 13,14,15,16)
vector<vector<const Team*>> Tournament::assignGroups() {
    vector<vector<const Team*>> groups(4);

    // Snake pattern indices for each group
    // Group A: seeds 0, 7, 8, 15
    // Group B: seeds 1, 6, 9, 14
    // Group C: seeds 2, 5, 10, 13
    // Group D: seeds 3, 4, 11, 12
    int pattern[4][4] = {
            {0, 7, 8,  15},   // Group A
            {1, 6, 9,  14},   // Group B
            {2, 5, 10, 13},   // Group C
            {3, 4, 11, 12},   // Group D
    };

    for (int g = 0; g < 4; g++) {
        for (int s = 0; s < 4; s++) {
            groups[g].push_back(seededTeams_[pattern[g][s]]);
        }
    }

    return groups;
}

// Run round-robin group stage
GroupResult Tournament::runGroupStage(const string& groupName, const vector<const Team*>& group) {
    GroupResult result;
    result.groupName = groupName;

    // Initialize standings
    vector<GroupStanding> standings(group.size());
    for (size_t i = 0; i < group.size(); i++) {
        standings[i].team = group[i];
    }

    // Round-robin: every team plays every other team
    for (size_t i = 0; i < group.size(); i++) {
        for (size_t j = i + 1; j < group.size(); j++) {
            MatchResult match = runMatch(group[i], group[j]);
            result.matches.push_back(match);

            // Update standings
            for (auto& s : standings) {
                if (s.team->getName() == group[i]->getName()) {
                    s.roundsWon += match.team1Score;
                    s.roundsLost += match.team2Score;
                    if (match.winnerName == group[i]->getName()) s.wins++;
                    else s.losses++;
                }
                if (s.team->getName() == group[j]->getName()) {
                    s.roundsWon += match.team2Score;
                    s.roundsLost += match.team1Score;
                    if (match.winnerName == group[j]->getName()) s.wins++;
                    else s.losses++;
                }
            }
        }
    }

    sortGroupStandings(standings);
    result.standings = standings;
    return result;
}

void Tournament::sortGroupStandings(vector<GroupStanding>& standings) {
    sort(standings.begin(), standings.end(), [](const GroupStanding& a, const GroupStanding& b) {
        if (a.wins != b.wins) return a.wins > b.wins;
        return a.roundDiff() > b.roundDiff();
    });
}

// Playoff bracket: takes 8 teams in matchup order, runs QF → SF → Final
void Tournament::runPlayoffBracket(const vector<const Team*>& playoffTeams) {
    // QF: 4 matches from paired teams
    vector<const Team*> sfTeams;
    for (size_t i = 0; i < playoffTeams.size(); i += 2) {
        string label = "QF " + to_string(i / 2 + 1);
        MatchResult result = runMatch(playoffTeams[i], playoffTeams[i + 1]);

        const Team* winner = (result.winnerName == playoffTeams[i]->getName()) ? playoffTeams[i] : playoffTeams[i + 1];
        const Team* loser = (winner == playoffTeams[i]) ? playoffTeams[i + 1] : playoffTeams[i];

        PlayoffMatch pm;
        pm.roundName = label;
        pm.result = result;
        pm.winner = winner;
        pm.loser = loser;
        playoffMatches_.push_back(pm);

        sfTeams.push_back(winner);
    }

    // 5th-8th: QF losers (added to placements later, after champion/runner-up/SF losers)
    vector<const Team*> qfLosers;
    for (const auto& pm : playoffMatches_) {
        if (pm.roundName.find("QF") != string::npos) {
            qfLosers.push_back(pm.loser);
        }
    }

    // SF: 2 matches
    vector<const Team*> finalTeams;
    vector<const Team*> sfLosers;
    for (size_t i = 0; i < sfTeams.size(); i += 2) {
        string label = "SF " + to_string(i / 2 + 1);
        MatchResult result = runMatch(sfTeams[i], sfTeams[i + 1]);

        const Team* winner = (result.winnerName == sfTeams[i]->getName()) ? sfTeams[i] : sfTeams[i + 1];
        const Team* loser = (winner == sfTeams[i]) ? sfTeams[i + 1] : sfTeams[i];

        PlayoffMatch pm;
        pm.roundName = label;
        pm.result = result;
        pm.winner = winner;
        pm.loser = loser;
        playoffMatches_.push_back(pm);

        finalTeams.push_back(winner);
        sfLosers.push_back(loser);
    }

    // Grand Final
    {
        string label = "FINAL";
        MatchResult result = runMatch(finalTeams[0], finalTeams[1]);

        const Team* winner = (result.winnerName == finalTeams[0]->getName()) ? finalTeams[0] : finalTeams[1];
        const Team* loser = (winner == finalTeams[0]) ? finalTeams[1] : finalTeams[0];

        PlayoffMatch pm;
        pm.roundName = label;
        pm.result = result;
        pm.winner = winner;
        pm.loser = loser;
        playoffMatches_.push_back(pm);

        champion_ = winner->getName();

        // Placements: 1st, 2nd, 3rd-4th, 5th-8th
        placements_.push_back(winner);       // 1st
        placements_.push_back(loser);        // 2nd
        for (const Team* t : sfLosers)
            placements_.push_back(t);        // 3rd-4th
        for (const Team* t : qfLosers)
            placements_.push_back(t);        // 5th-8th
    }
}

// ============================================================
//  RESULTS GETTERS
// ============================================================

string Tournament::getChampion() const {
    return champion_;
}

const vector<const Team*>& Tournament::getPlacements() const {
    return placements_;
}

const vector<GroupResult>& Tournament::getGroupResults() const {
    return groupResults_;
}

const vector<const Team*>& Tournament::getQualifiedTeams() const {
    return qualifiedTeams_;
}

const vector<PlayoffMatch>& Tournament::getPlayoffMatches() const {
    return playoffMatches_;
}

const map<string, PlayerAggregateStats>& Tournament::getAllPlayerStats() const {
    return playerStats_;
}

const vector<MatchResult>& Tournament::getAllMatches() const {
    return allMatches_;
}

// ============================================================
//  PLAYER RANKINGS
// ============================================================

vector<pair<string, PlayerAggregateStats>> Tournament::getPlayerRankings(int minMaps, bool ascending) const {
    vector<pair<string, PlayerAggregateStats>> rankings;

    for (const auto& entry : playerStats_) {
        if (entry.second.matchesPlayed >= minMaps) {
            rankings.push_back({entry.first, entry.second});
        }
    }

    sort(rankings.begin(), rankings.end(), [ascending](const auto& a, const auto& b) {
        return ascending ? a.second.avgRating() < b.second.avgRating()
                         : a.second.avgRating() > b.second.avgRating();
    });

    return rankings;
}

// ============================================================
//  MVP CALCULATION
// ============================================================

int Tournament::getTeamPlacement(const string& playerName) const {
    for (size_t i = 0; i < placements_.size(); i++) {
        for (const Player& p : placements_[i]->getRoster()) {
            if (p.getName() == playerName) {
                return (int)i + 1;  // 1-indexed
            }
        }
    }
    return (int)placements_.size(); // Not found = worst placement
}

MVPResult Tournament::calculateMVP() const {
    MVPResult mvp;
    double bestScore = -1.0;

    // Find max matches played (for normalization)
    int maxMatches = 0;
    for (const auto& entry : playerStats_) {
        maxMatches = max(maxMatches, entry.second.matchesPlayed);
    }
    if (maxMatches == 0) return mvp;

    int totalTeams = (int)placements_.size();

    for (const auto& entry : playerStats_) {
        const string& playerName = entry.first;
        const PlayerAggregateStats& stats = entry.second;
        if (stats.matchesPlayed == 0) continue;

        // Average HLTV rating
        double avgRating = stats.avgRating();

        // Match participation weight (more matches = more opportunity to prove)
        double matchWeight = (double)stats.matchesPlayed / maxMatches;

        // Team placement weight (1st place = 1.0, last = 0.0)
        int placement = getTeamPlacement(playerName);
        double placementWeight = 1.0 - ((double)(placement - 1) / totalTeams);

        // Final MVP score: 60% rating, 20% matches played, 20% team placement
        double score = (avgRating * 0.6) + (matchWeight * 0.2) + (placementWeight * 0.2);

        if (score > bestScore) {
            bestScore = score;
            mvp.playerName = playerName;
            mvp.score = score;
            mvp.avgRating = avgRating;
            mvp.matchesPlayed = stats.matchesPlayed;
            mvp.placement = placement;
            mvp.teamName = "";  // Reset before searching

            // Find team name
            for (const auto& team : placements_) {
                for (const Player& p : team->getRoster()) {
                    if (p.getName() == playerName) {
                        mvp.teamName = team->getName();
                        break;
                    }
                }
                if (!mvp.teamName.empty()) break;
            }
        }
    }

    return mvp;
}

MVPResult Tournament::getMVP() const {
    return calculateMVP();
}
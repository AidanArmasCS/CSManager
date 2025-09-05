//
// Created by Aidan Armas on 7/3/25.
//

#include "Tournament.h"
#include "core/Match.h"
#include "core/Team.h"
#include <cmath>
#include <iomanip>


// Default Constructor setting all the base variables to the passed arguments
Tournament::Tournament(std::string tFormat, std::string tLocation, std::string tName, std::string tTier, int tPrizePool,
                       vector<Team*> teamList, std::string tLength, std::string tDate) {
    format = tFormat;
    location = tLocation;
    name = tName;
    tier = tTier;
    prizePool = tPrizePool;
    teams = teamList;
    length = tLength;
    date = tDate;
    seededTeams = teams;
}

Tournament::~Tournament() {}


// Adds team to already constructed Tournament Object
void Tournament::addTeam(Team* team) {
    teams.push_back(team);
    seededTeams.push_back(team);
}


// Removes Team From already constructed Tournament Object
void Tournament::removeTeam(Team* team) {
    teams.erase(remove(teams.begin(), teams.end(), team), teams.end());
    seededTeams.erase(remove(seededTeams.begin(), seededTeams.end(), team), seededTeams.end());
}

void Tournament::simulateTournament() {
    // Sort Team List by Ranking for Seeding
    sort(seededTeams.begin(), seededTeams.end(), [](Team* a, Team* b) {
        return a->getTeamRanking() < b->getTeamRanking(); // Lover = Higher Seed
    });

    if (format == "bracket") {
        bracketFormatSim();
    }
    else if (format == "groups") {
        groupFormatSim();
    }
    else {
        cerr << "Improper Format Type Try Again";
        exit(1);
    }
}

// Function To Handle Individual Match Simulation and Stat Tracking
void Tournament::simulateMatchAndStats(Team* team1, Team* team2) {
    Match match(team1, team2);
    match.simulateMatch();
    match.simulateMatchStats();

    for (Team* team: {team1, team2}) {
        for (Player& player : team->getRoster()) {
            string namePlayer = player.getName();
            double rating = match.simulateMatchRatings(&player);
            double adr = player.getMatchADR();

            auto& agg = playerStatsMap[namePlayer];

            agg.totalKills += player.getMatchKills();
            agg.totalDeaths += player.getMatchDeaths();
            agg.totalAssists += player.getMatchAssists();
            agg.totalHLTV += rating;
            agg.totalADR += adr;
            agg.matchesPlayed += 1;
        }
    }
    setWinner(match.getWinner());
}

void Tournament::bracketFormatSim() {
    vector<Team*> currentRound = seededTeams;
    vector<Team*> thisRoundPlacements;

    while (currentRound.size() > 1) {
        vector<Team*> nextRound;
        size_t numMatches = currentRound.size() / 2;

        for (size_t i = 0; i < numMatches; i++) {
            Team* team1 = currentRound[i];
            Team* team2 = currentRound[currentRound.size() - 1 - i];


            simulateMatchAndStats(team1, team2);

            Team* winnerTeam = (team1->getName() == winner) ? team1 : team2;
            Team* loserTeam = (winnerTeam == team1) ? team2 : team1;

            nextRound.push_back(winnerTeam);
            thisRoundPlacements.push_back(loserTeam); // losers of the round
        }
        currentRound = nextRound;
    }

    // Final Winner when current Round has 1 team left
    winner = currentRound.front()->getName();
    placements.push_back(currentRound.front());

    // Place rest of teams in order of placing
    placements.insert(placements.end(), thisRoundPlacements.begin(), thisRoundPlacements.end());
}

void Tournament::groupFormatSim() {
    cout << "PLACEHOLDER FOR IMPLEMENTATION" << endl;
}

void Tournament::calculateMVP() {
    string mvp;
    double bestScore = -1.0;
    double mvpRating;

    // Get Max Matches Player
    int maxMatches = 0;
    for (const auto& entry : playerStatsMap) {
        maxMatches = max(maxMatches, entry.second.matchesPlayed);
    }

    for (const auto& entry : playerStatsMap) {
        const string& playerName = entry.first;
        const PlayerStats& stats = entry.second;
        if (stats.matchesPlayed == 0) continue;

        // Average Rating
        double avgRating = stats.totalHLTV / stats.matchesPlayed;

        // Normalize Matches Played
        double matchWeight = static_cast<double>(stats.matchesPlayed) / maxMatches;

        // Get team placement
        int placement = -1;
        for (int i = 0; i < placements.size(); i++) {
            for (Player& p : placements[i]->getRoster()) {
                if (p.getName() == playerName) {
                    placement = i;
                    break;
                }
            }
            if (placement != -1) break;
        }

        // Placement weight: higher = better team placement
        double placementWeight = 1.0 - (static_cast<double>(placement) / placements.size());

        // Final MVP Score
        double score = (avgRating * 0.6) + (matchWeight * 0.2) + (placementWeight * 0.2);

        if (score > bestScore) {
            bestScore = score;
            mvp = playerName;
        }
    }
    cout << "MVP Of " << name << ": " << mvp << " (Score: " << fixed << setprecision(2) << bestScore << ")\n";
}


void Tournament::printPlacements() {
    cout << "Final Placements for " << name << endl;
    for (size_t  i = 0; i < placements.size(); i++) {
        cout << (i + 1) << ". " << placements[i]->getName() << endl;
    }
}

void Tournament::printAllPlayerRatings(bool descending) {
    vector<pair<string, double>> sortedRatings;

    for (auto& entry : playerStatsMap) {
        const PlayerStats& stats = entry.second;
        if (stats.matchesPlayed == 0) continue;
        double avg = stats.totalHLTV / stats.matchesPlayed;
        sortedRatings.push_back({entry.first, avg});
    }

    sort(sortedRatings.begin(), sortedRatings.end(), [&](auto& a, auto& b) {
        return descending ? a.second > b.second : a.second < b.second;
    });

    cout << "\nPlayer Ratings\n";
    for (auto& p : sortedRatings) {
        cout << fixed << setprecision(2) << p.first << " - " << p.second << endl;
    }
}

void Tournament::distributePrizeMoney() {
    cout << "Placeholder for implementation later" << endl;
}

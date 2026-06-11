//
// Tournament.h
// Engine Layer - Tournament simulation orchestrator
//
// Owns: format config, seeding, group stage, bracket playoffs,
//       aggregate stat tracking, placements, MVP calculation
//
// Does NOT own: match simulation (delegates to MatchSimulator),
//               rating calculation (delegates to RatingCalculator),
//               display/printing (returns data, caller decides how to print)
//

#ifndef TOURNAMENT_H
#define TOURNAMENT_H

#include "core/Team.h"
#include "core/Player.h"
#include "core/MatchResult.h"
#include "engine/MatchSimulator.h"
#include "engine/RatingCalculator.h"
#include <vector>
#include <map>
#include <string>
#include <algorithm>

using namespace std;

// ============================================================
//  DATA CONTAINERS (returned by Tournament, displayed by caller)
// ============================================================

// Format types
enum class TournamentFormat {
    Bracket,        // Single-elimination bracket (seeded: 1v16, 2v15, etc.)
    Groups,         // 4 groups of 4, round-robin, top 2 advance → single-elim playoffs
    // Swiss        // Future: Swiss system
};

// A team's record within a group
struct GroupStanding {
    const Team* team = nullptr;
    int wins = 0;
    int losses = 0;
    int roundsWon = 0;
    int roundsLost = 0;

    int roundDiff() const { return roundsWon - roundsLost; }
};

// A complete group's results
struct GroupResult {
    string groupName;
    vector<GroupStanding> standings;   // Sorted: wins desc, round diff desc
    vector<MatchResult> matches;      // All matches played in this group
};

// A single playoff matchup result
struct PlayoffMatch {
    string roundName;                 // e.g. "QF 1", "SF 2", "FINAL"
    MatchResult result;
    const Team* winner = nullptr;
    const Team* loser = nullptr;
};

// MVP result
struct MVPResult {
    string playerName;
    string teamName;
    double score = 0.0;
    double avgRating = 0.0;
    int matchesPlayed = 0;
    int placement = 0;               // Team's final placement (1 = champion)
};

// ============================================================
//  TOURNAMENT CLASS
// ============================================================

class Tournament {
public:

    // --- Construction ---
    Tournament(const string& name, TournamentFormat format,
               const vector<const Team*>& teams,
               int prizePool = 0, const string& location = "",
               const string& date = "", const string& tier = "");

    // --- Core Simulation ---
    // Runs the entire tournament. Call once, then query results.
    void simulate();

    // --- Results (query after simulate()) ---

    // Overall
    string getChampion() const;
    const vector<const Team*>& getPlacements() const;
    string getName() const { return name_; }
    TournamentFormat getFormat() const { return format_; }
    int getTeamCount() const { return (int)teams_.size(); }

    // Group stage (only valid for Groups format)
    const vector<GroupResult>& getGroupResults() const;
    const vector<const Team*>& getQualifiedTeams() const;

    // Playoffs
    const vector<PlayoffMatch>& getPlayoffMatches() const;

    // Player stats
    const map<string, PlayerAggregateStats>& getAllPlayerStats() const;

    // Sorted player rankings (descending by avg rating)
    // minMaps: minimum matches played to qualify
    vector<pair<string, PlayerAggregateStats>> getPlayerRankings(int minMaps = 1, bool ascending = false) const;

    // MVP calculation
    MVPResult getMVP() const;

    // All match results in order played
    const vector<MatchResult>& getAllMatches() const;


private:

    // --- Config ---
    string name_;
    TournamentFormat format_;
    vector<const Team*> teams_;         // Original team list (unmodified)
    vector<const Team*> seededTeams_;   // Sorted by ranking for seeding
    int prizePool_;
    string location_;
    string date_;
    string tier_;

    // --- State (populated during simulate()) ---
    bool simulated_ = false;
    string champion_;
    vector<const Team*> placements_;

    // Group stage
    vector<GroupResult> groupResults_;
    vector<const Team*> qualifiedTeams_;  // 8 teams that advance from groups

    // Playoffs
    vector<PlayoffMatch> playoffMatches_;

    // Stats
    map<string, PlayerAggregateStats> playerStats_;
    vector<MatchResult> allMatches_;

    // --- Internal Simulation Methods ---

    // Seed teams by ranking (lower ranking number = higher seed)
    void seedTeams();

    // Run a single match, record stats, store result
    MatchResult runMatch(const Team* team1, const Team* team2);

    // --- Bracket Format ---
    void simulateBracket();

    // --- Groups + Playoffs Format ---
    void simulateGroupsAndPlayoffs();

    // Group stage: snake-draft seeding into 4 groups, round-robin
    vector<vector<const Team*>> assignGroups();
    GroupResult runGroupStage(const string& groupName, const vector<const Team*>& group);
    void sortGroupStandings(vector<GroupStanding>& standings);

    // Playoffs: QF → SF → Final
    void runPlayoffBracket(const vector<const Team*>& playoffTeams);

    // --- MVP ---
    MVPResult calculateMVP() const;

    // Helper: find which team a player belongs to
    int getTeamPlacement(const string& playerName) const;
};

#endif // TOURNAMENT_H
//
// MatchResult.h
// Layer 1 - Data container for match outcomes
//
// This replaces:
//   - matchKills/matchDeaths/matchAssists/matchADR that were on Player
//   - winner/team1score/team2score that were on Match
//   - The PlayerStats struct that was duplicated in main.cpp and Tournament.h
//

#ifndef MATCHRESULT_H
#define MATCHRESULT_H

#include <string>
#include <vector>
#include <map>

using namespace std;

// Stats for a single player in a single match
struct PlayerMatchStats {
    string playerName;
    string teamName;
    int kills = 0;
    int deaths = 0;
    int assists = 0;
    double adr = 0.0;
    double hltvRating = 0.0;
};

// Complete result of one match
struct MatchResult {
    string team1Name;
    string team2Name;
    int team1Score = 0;
    int team2Score = 0;
    string winnerName;

    // Per-player stats for this match, keyed by player name
    map<string, PlayerMatchStats> playerStats;

    // Helper methods
    bool isFinished() const { return !winnerName.empty(); }
    string getLoserName() const {
        if (winnerName == team1Name) return team2Name;
        if (winnerName == team2Name) return team1Name;
        return "";
    }
    int getWinnerScore() const {
        return (winnerName == team1Name) ? team1Score : team2Score;
    }
    int getLoserScore() const {
        return (winnerName == team1Name) ? team2Score : team1Score;
    }
};

// Cumulative stats across multiple matches (for StatTracker / Tournament use)
struct PlayerAggregateStats {
    int totalKills = 0;
    int totalDeaths = 0;
    int totalAssists = 0;
    double totalHLTV = 0.0;
    double totalADR = 0.0;
    int matchesPlayed = 0;

    double avgKills() const { return matchesPlayed > 0 ? (double)totalKills / matchesPlayed : 0; }
    double avgDeaths() const { return matchesPlayed > 0 ? (double)totalDeaths / matchesPlayed : 0; }
    double avgAssists() const { return matchesPlayed > 0 ? (double)totalAssists / matchesPlayed : 0; }
    double avgHLTV() const { return matchesPlayed > 0 ? totalHLTV / matchesPlayed : 0; }
    double avgRating() const { return avgHLTV(); }  // alias
    double avgADR() const { return matchesPlayed > 0 ? totalADR / matchesPlayed : 0; }
    double kdRatio() const { return totalDeaths > 0 ? (double)totalKills / totalDeaths : 0; }

    // Add a single match's stats to the aggregate
    void addMatch(const PlayerMatchStats& match) {
        totalKills += match.kills;
        totalDeaths += match.deaths;
        totalAssists += match.assists;
        totalHLTV += match.hltvRating;
        totalADR += match.adr;
        matchesPlayed++;
    }
};

#endif // MATCHRESULT_H
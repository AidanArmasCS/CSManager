//
// MatchSimulator.h
// Layer 2 - Stateless match simulation engine
//
// Takes two const Team references, simulates a full match,
// returns a MatchResult with all scores and player stats.
// Never mutates Team or Player objects.
//

#ifndef MATCHSIMULATOR_H
#define MATCHSIMULATOR_H

#include "core/Team.h"
#include "core/MatchResult.h"
#include "engine/RatingCalculator.h"
#include "engine/HLTVCalculator.h"
#include <vector>
#include <string>

using namespace std;

class MatchSimulator {
public:

    // Simulate a full match between two teams
    // Returns a complete MatchResult with scores, winner, and all player stats
    static MatchResult simulate(const Team& team1, const Team& team2);

private:

    // Step 1: Determine winner and scores based on team ratings + randomness
    static void determineWinnerAndScores(int team1Rating, int team2Rating,
                                         const string& team1Name, const string& team2Name,
                                         MatchResult& result);

    // Step 2: Calculate total team kills/deaths based on round scores
    static void calculateTeamTotals(int winnerScore, int loserScore,
                                    int& winnerKills, int& winnerDeaths,
                                    int& loserKills, int& loserDeaths);

    // Step 3: Calculate role-based weights for stat distribution
    struct StatWeights {
        vector<double> killWeights;
        vector<double> deathWeights;
        vector<double> assistWeights;
    };

    static StatWeights calculatePlayerWeights(const vector<Player>& roster);

    // Step 4: Distribute kills/deaths/assists to individual players using weights
    static void distributeStats(const vector<Player>& roster,
                                const StatWeights& weights,
                                int totalKills, int totalDeaths, int totalAssists,
                                int totalRounds,
                                const string& teamName,
                                MatchResult& result);
};

#endif // MATCHSIMULATOR_H
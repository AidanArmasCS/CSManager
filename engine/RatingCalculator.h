//
// RatingCalculator.h
// Layer 2 - Stateless calculator for player ratings, team ratings, and chemistry
//
// All methods are static. No state is stored. Takes const references,
// returns computed values. Never mutates Player or Team.
//

#ifndef RATINGCALCULATOR_H
#define RATINGCALCULATOR_H

#include "core/player.h"
#include "core/Team.h"
#include <unordered_map>
#include <string>

class RatingCalculator {
public:

    // --- Player Ratings ---

    // Base overall rating using role-weighted formula (raw stats only)
    static int getPlayerOverallRating(const Player& player);

    // Adjusted rating: applies trait stat boosts WITHOUT mutating the player
    // Creates temporary adjusted stats, computes rating, returns result
    static int getAdjustedPlayerRating(const Player& player);

    // --- Chemistry ---

    // Individual player chemistry within a team context
    // Base 65 + style matching with teammates + trait effects
    static double getPlayerChemistry(const Player& player, const vector<Player>& teammates);

    // Team-wide chemistry: weighted average of all player chemistries
    static int getTeamChemistry(const Team& team);

    // --- Team Rating ---

    // Full team rating: weighted player ratings scaled by chemistry
    static int getTeamOverallRating(const Team& team);

private:

    // Internal helper: computes the role-weighted formula given a set of stats
    // Used by both getPlayerOverallRating and getAdjustedPlayerRating
    static double computeRoleRating(const string& role,
                                    int aim, int movement, int gameSense, int teamwork,
                                    int entrying, int awp, int clutch, int consistency);

    // Maps trait names to which stat they affect
    // Returns the stat name as a string key: "aim", "teamwork", "gameSense", "clutch"
    static string getTraitTargetStat(const string& traitName);
};

#endif // RATINGCALCULATOR_H
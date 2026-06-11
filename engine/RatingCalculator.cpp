//
// RatingCalculator.cpp
// Layer 2 - Stateless calculator for player ratings, team ratings, and chemistry
//

#include "RatingCalculator.h"
#include <algorithm>
#include <vector>
#include <cmath>
#include <unordered_map>

using namespace std;

// ============================================================
//  PRIVATE HELPERS
// ============================================================

// The core role-weighted rating formula
// Extracted so both raw and adjusted ratings use the exact same math
double RatingCalculator::computeRoleRating(const string& role,
                                           int aim, int movement, int gameSense, int teamwork,
                                           int entrying, int awp, int clutch, int consistency) {
    double rating = 0.0;

    if (role == "IGL") {
        rating = ((aim * 0.9) + (movement * 0.65) + (gameSense * 1.9) + (teamwork * 2.0)
                  + (entrying * 0.5) + (awp * 0.5) + (clutch * 0.6) + (consistency * 1.1)) / 9.0;
    }
    else if (role == "AWP") {
        rating = ((aim * 1.1) + (movement * 1.1) + (gameSense * 1.05) + (teamwork * 1.05)
                  + (entrying * 0.8) + (awp * 1.5) + (clutch * 0.4) + (consistency * 1.1)) / 9.0;
    }
    else if (role == "Entry") {
        rating = ((aim * 1.25) + (movement * 1.1) + (gameSense * 1.0) + (teamwork * 1.0)
                  + (entrying * 1.9) + (awp * 0.25) + (clutch * 0.25) + (consistency * 1.01)) / 9.0;
    }
    else if (role == "Rifler") {
        rating = ((aim * 1.2) + (movement * 1.1) + (gameSense * 1.15) + (teamwork * 1.15)
                  + (entrying * 0.9) + (awp * 0.25) + (clutch * 1.05) + (consistency * 1.18)) / 9.0;
    }
    else if (role == "Lurker") {
        rating = ((aim * 1.1) + (movement * 1.0) + (gameSense * 1.1) + (teamwork * 1.6)
                  + (entrying * 0.6) + (awp * 0.33) + (clutch * 1.05) + (consistency * 1.1)) / 9.0;
    }
    else if (role == "Support") {
        rating = ((aim * 1.1) + (movement * 1.0) + (gameSense * 1.1) + (teamwork * 1.6)
                  + (entrying * 0.6) + (awp * 0.33) + (clutch * 1.05) + (consistency * 1.1)) / 9.0;
    }

    rating += 14.0;

    // Cap at 99
    if (rating >= 99.0) {
        rating = 99.0;
    }

    return rating;
}

// Maps each trait name to the stat it modifies
string RatingCalculator::getTraitTargetStat(const string& traitName) {
    static const unordered_map<string, string> traitTargetMap = {
            // Negative traits
            {"Selfish",             "teamwork"},
            {"Toxic",               "teamwork"},
            {"Inconsistent",        "teamwork"},
            {"Choker",              "aim"},
            {"Tilter",              "teamwork"},
            {"Bad Communicator",    "teamwork"},
            {"Baiter",              "teamwork"},
            {"Stubborn",            "teamwork"},
            {"Large Ego",           "teamwork"},
            {"Lazy",                "teamwork"},

            // Positive traits
            {"Leader",              "teamwork"},
            {"Team Player",         "teamwork"},
            {"Clutch Master",       "clutch"},
            {"Supportive",          "teamwork"},
            {"Aim Demon",           "aim"},
            {"Tactical Genius",     "gameSense"},
            {"Disciplined",         "teamwork"},
            {"Versatile",           "teamwork"},
            {"Quick Learner",       "gameSense"},
            {"Master Communicator", "teamwork"},
            {"None",                "teamwork"}
    };

    auto it = traitTargetMap.find(traitName);
    if (it != traitTargetMap.end()) {
        return it->second;
    }
    return "";
}


// ============================================================
//  PLAYER RATINGS
// ============================================================

int RatingCalculator::getPlayerOverallRating(const Player& player) {
    double rating = computeRoleRating(
            player.getRole(),
            player.getAim(), player.getMovement(), player.getGameSense(), player.getTeamwork(),
            player.getEntrying(), player.getAwping(), player.getClutch(), player.getConsistency()
    );
    return static_cast<int>(rating);
}

int RatingCalculator::getAdjustedPlayerRating(const Player& player) {
    // Start with a copy of the raw stats
    int aim         = player.getAim();
    int movement    = player.getMovement();
    int gameSense   = player.getGameSense();
    int teamwork    = player.getTeamwork();
    int entrying    = player.getEntrying();
    int awp         = player.getAwping();
    int clutch      = player.getClutch();
    int consistency = player.getConsistency();

    // Map stat names to local variable pointers (local copies, NOT the player's fields)
    unordered_map<string, int*> statMap = {
            {"aim",       &aim},
            {"movement",  &movement},
            {"gameSense", &gameSense},
            {"teamwork",  &teamwork},
            {"entrying",  &entrying},
            {"awp",       &awp},
            {"clutch",    &clutch},
            {"consistency", &consistency}
    };

    // Apply trait boosts to the LOCAL copies
    for (const Trait& trait : player.getTraits()) {
        string targetStat = getTraitTargetStat(trait.name);
        auto it = statMap.find(targetStat);
        if (it != statMap.end()) {
            *(it->second) += trait.statBoost;
        }
    }

    // Compute rating using the adjusted LOCAL copies
    double rating = computeRoleRating(
            player.getRole(),
            aim, movement, gameSense, teamwork,
            entrying, awp, clutch, consistency
    );

    return static_cast<int>(rating);
}


// ============================================================
//  CHEMISTRY
// ============================================================

double RatingCalculator::getPlayerChemistry(const Player& player, const vector<Player>& teammates) {
    double chemistry = 65.0; // Base chemistry

    // Style matching with teammates
    for (const Player& teammate : teammates) {
        if (teammate.getName() != player.getName()) {
            if (teammate.getStyle() != player.getStyle()) {
                chemistry -= 2.0;
            } else {
                chemistry += 2.0;
            }
        }
    }

    // Trait effects on chemistry
    for (const Trait& trait : player.getTraits()) {
        chemistry += trait.effect;
    }

    // Clamp between 1 and 99
    chemistry = max(1.0, min(99.0, chemistry));

    return chemistry;
}

int RatingCalculator::getTeamChemistry(const Team& team) {
    const vector<Player>& roster = team.getRoster();

    if (roster.empty()) {
        return 50;
    }

    vector<double> chemistryValues;

    // Calculate each player's chemistry
    for (size_t i = 0; i < roster.size(); i++) {
        // Build teammates list (everyone except current player)
        vector<Player> teammates;
        for (size_t j = 0; j < roster.size(); j++) {
            if (i != j) {
                teammates.push_back(roster[j]);
            }
        }

        double playerChem = getPlayerChemistry(roster[i], teammates);
        chemistryValues.push_back(playerChem);
    }

    // Sort highest to lowest
    sort(chemistryValues.rbegin(), chemistryValues.rend());

    // Weighted average: top 3 get 1.1x, bottom 2 get 0.9x
    double totalChemistry = 0.0;
    for (size_t i = 0; i < chemistryValues.size(); i++) {
        if (i < 3) {
            totalChemistry += chemistryValues[i] * 1.1;
        } else {
            totalChemistry += chemistryValues[i] * 0.9;
        }
    }

    totalChemistry /= static_cast<double>(roster.size());
    return static_cast<int>(totalChemistry);
}


// ============================================================
//  TEAM RATING
// ============================================================

int RatingCalculator::getTeamOverallRating(const Team& team) {
    const vector<Player>& roster = team.getRoster();

    if (roster.empty()) {
        return 0;
    }

    // Collect adjusted ratings for each player
    vector<int> playerRatings;
    playerRatings.reserve(roster.size());

    for (const Player& p : roster) {
        playerRatings.push_back(getAdjustedPlayerRating(p));
    }

    // Sort highest to lowest
    sort(playerRatings.rbegin(), playerRatings.rend());

    // Weighted total: top player 1.2x, 2nd/3rd 1.1x, bottom 2 0.9x
    double totalRating = 0.0;
    for (size_t i = 0; i < playerRatings.size(); i++) {
        if (i == 0) {
            totalRating += playerRatings[i] * 1.2;
        } else if (i == 1 || i == 2) {
            totalRating += playerRatings[i] * 1.1;
        } else {
            totalRating += playerRatings[i] * 0.9;
        }
    }

    double baseTeamRating = totalRating / static_cast<double>(playerRatings.size());

    // Apply chemistry scaling
    double teamChemistry = getTeamChemistry(team);
    double chemistryEffect = (teamChemistry - 65) / 70.0;
    double scaledChemistryImpact = 1.0 + (chemistryEffect * 0.15);
    double finalRating = baseTeamRating * scaledChemistryImpact;

    return static_cast<int>(finalRating);
}
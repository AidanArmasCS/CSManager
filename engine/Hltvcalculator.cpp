//
// HLTVCalculator.cpp
// Layer 2 - HLTV 2.0 rating and ADR calculation
//

#include "HLTVCalculator.h"
#include <cstdlib>
#include <algorithm>

using namespace std;

// ============================================================
//  ADR SIMULATION
// ============================================================

double HLTVCalculator::simulateADR(int kills, int assists, const string& role, int totalRounds) {
    if (totalRounds == 0) return 0.0;

    double totalDamage = 0.0;
    bool isAWP = (role == "AWP");

    // Kill damage
    for (int i = 0; i < kills; i++) {
        double killDamage;

        if (isAWP) {
            // AWP shots: 85-100 damage, 80% chance of 95-100
            killDamage = 85 + (rand() % 16);
            if ((rand() % 100) < 80) {
                killDamage = 95 + (rand() % 6);
            }
        } else {
            // Standard kill damage: 50-100, 75% bias toward 75-95
            killDamage = 50 + (rand() % 51);
            if ((rand() % 100) < 75) {
                killDamage = 75 + (rand() % 21);
            }
        }
        totalDamage += killDamage;
    }

    // Assist damage
    for (int i = 0; i < assists; i++) {
        double assistDamage;

        if (isAWP) {
            // AWP assist damage: 20-50 (lower than rifles)
            assistDamage = 20 + (rand() % 31);
        } else {
            // Rifle assist damage: 40-70, 70% bias for 50-65
            assistDamage = 40 + (rand() % 31);
            if ((rand() % 100) < 70) {
                assistDamage = 50 + (rand() % 16);
            }
        }
        totalDamage += assistDamage;
    }

    // Baseline chip damage (tagging rounds with no kills/assists)
    int chipRounds = max(0, totalRounds - (kills + assists));
    if (isAWP) {
        totalDamage += chipRounds * (15 + (rand() % 11));  // 15-25 per round
    } else {
        totalDamage += chipRounds * (25 + (rand() % 16));  // 25-40 per round
    }

    return totalDamage / totalRounds;
}


// ============================================================
//  HLTV 2.0 RATING
// ============================================================

double HLTVCalculator::calculateRatingWithADR(int kills, int deaths, int assists,
                                              const string& role, int totalRounds, double adr) {
    if (totalRounds == 0) return 0.0;

    double fpr = static_cast<double>(kills) / totalRounds;
    double dpr = static_cast<double>(deaths) / totalRounds;
    double apr = static_cast<double>(assists) / totalRounds;

    // K/D difference per round: the PRIMARY driver of rating
    double kdDiff = fpr - dpr;

    // ADR deviation from average (~72)
    // Increased divisor from 400→250 so high/low ADR matters more
    double adrBonus = (adr - 72.0) / 250.0;

    // Assist bonus
    double assistBonus = apr * 0.10;

    double hltvRating;

    if (role == "AWP") {
        hltvRating = 1.0
                     + (kdDiff * 0.72)
                     + (adrBonus)
                     + (assistBonus)
                     + 0.02;
    }
    else if (role == "IGL") {
        // IGL: K/D still matters significantly, assists help but don't rescue
        hltvRating = 1.0
                     + (kdDiff * 0.68)
                     + (adrBonus)
                     + (assistBonus * 1.5);
    }
    else {
        // Rifler, Entry, Support, Lurker
        hltvRating = 1.0
                     + (kdDiff * 0.75)
                     + (adrBonus)
                     + (assistBonus);
    }

    return hltvRating;
}

HLTVResult HLTVCalculator::calculate(int kills, int deaths, int assists,
                                     const string& role, int totalRounds) {
    HLTVResult result;

    if (totalRounds == 0) return result;

    result.adr = simulateADR(kills, assists, role, totalRounds);
    result.fpr = static_cast<double>(kills) / totalRounds;
    result.dpr = static_cast<double>(deaths) / totalRounds;
    double apr = static_cast<double>(assists) / totalRounds;
    result.impact = (2.13 * result.fpr) + (0.42 * apr) - 0.41;
    result.hltvRating = calculateRatingWithADR(kills, deaths, assists, role, totalRounds, result.adr);

    return result;
}
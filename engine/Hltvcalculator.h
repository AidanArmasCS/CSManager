//
// HLTVCalculator.h
// Layer 2 - Stateless calculator for HLTV 2.0 ratings and ADR
//
// Takes match stats (kills, deaths, assists, role, rounds),
// simulates damage for ADR, and computes the HLTV 2.0 rating.
// No state stored. Never mutates any objects.
//

#ifndef HLTVCALCULATOR_H
#define HLTVCALCULATOR_H

#include "core/MatchResult.h"
#include <string>

using namespace std;

struct HLTVResult {
    double hltvRating = 0.0;
    double adr = 0.0;
    double fpr = 0.0;       // Frags per round
    double dpr = 0.0;       // Deaths per round
    double impact = 0.0;    // Impact rating
};

class HLTVCalculator {
public:

    // Full calculation: simulates ADR and computes HLTV rating
    // Takes the player's match stats, their role, and total rounds played
    static HLTVResult calculate(int kills, int deaths, int assists,
                                const string& role, int totalRounds);

    // If you already have ADR (e.g. from a replay), skip damage simulation
    static double calculateRatingWithADR(int kills, int deaths, int assists,
                                         const string& role, int totalRounds, double adr);

private:

    // Simulates total damage dealt based on kills, assists, role, and rounds
    static double simulateADR(int kills, int assists, const string& role, int totalRounds);
};

#endif // HLTVCALCULATOR_H
//
// MatchSimulator.cpp
// Layer 2 - Stateless match simulation engine
//

#include "MatchSimulator.h"
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <iostream>

using namespace std;


// ============================================================
//  PUBLIC: Main simulate method
// ============================================================

MatchResult MatchSimulator::simulate(const Team& team1, const Team& team2) {
    MatchResult result;
    result.team1Name = team1.getName();
    result.team2Name = team2.getName();

    // Get team ratings through RatingCalculator (no mutation)
    int team1Rating = RatingCalculator::getTeamOverallRating(team1);
    int team2Rating = RatingCalculator::getTeamOverallRating(team2);

    // Step 1: Determine winner and scores
    determineWinnerAndScores(team1Rating, team2Rating,
                             team1.getName(), team2.getName(), result);

    int totalRounds = result.team1Score + result.team2Score;

    // Figure out which team won
    const Team& winningTeam = (result.winnerName == team1.getName()) ? team1 : team2;
    const Team& losingTeam = (result.winnerName == team1.getName()) ? team2 : team1;
    int winnerScore = result.getWinnerScore();
    int loserScore = result.getLoserScore();

    // Step 2: Calculate team-level kill/death totals
    int winnerKills, winnerDeaths, loserKills, loserDeaths;
    calculateTeamTotals(winnerScore, loserScore,
                        winnerKills, winnerDeaths, loserKills, loserDeaths);

    // Step 3 & 4: Calculate weights and distribute stats for each team
    const vector<Player>& winnerRoster = winningTeam.getRoster();
    const vector<Player>& loserRoster = losingTeam.getRoster();

    if (!winnerRoster.empty()) {
        StatWeights winnerWeights = calculatePlayerWeights(winnerRoster);
        int winnerAssists = static_cast<int>(winnerKills * 0.10);
        distributeStats(winnerRoster, winnerWeights,
                        winnerKills, winnerDeaths, winnerAssists,
                        totalRounds, winningTeam.getName(), result);
    }

    if (!loserRoster.empty()) {
        StatWeights loserWeights = calculatePlayerWeights(loserRoster);
        int loserAssists = static_cast<int>(loserKills * 0.10);
        distributeStats(loserRoster, loserWeights,
                        loserKills, loserDeaths, loserAssists,
                        totalRounds, losingTeam.getName(), result);
    }

    return result;
}


// ============================================================
//  STEP 1: Determine Winner and Scores
// ============================================================

void MatchSimulator::determineWinnerAndScores(int team1Rating, int team2Rating,
                                              const string& team1Name, const string& team2Name,
                                              MatchResult& result) {
    // Random variance to allow upsets (-7 to +7)
    int randomFactor = rand() % 17 - 7;
    team1Rating += randomFactor;
    team2Rating -= randomFactor;

    bool team1Wins = team1Rating > team2Rating;
    int ratingDiff = abs(team1Rating - team2Rating);

    // Base losing score: closer teams = closer games
    int baseLosingScore = max(1, min(11, 13 - ((ratingDiff / 4) + rand() % 3)));

    // 15% chance of a close game regardless of rating difference
    if (rand() % 100 < 15) {
        baseLosingScore = 9 + (rand() % 2);
    }

    if (team1Wins) {
        result.team1Score = 13;
        result.team2Score = baseLosingScore;
        result.winnerName = team1Name;
    } else {
        result.team2Score = 13;
        result.team1Score = baseLosingScore;
        result.winnerName = team2Name;
    }
}


// ============================================================
//  STEP 2: Calculate Team Kill/Death Totals
// ============================================================

void MatchSimulator::calculateTeamTotals(int winnerScore, int loserScore,
                                         int& winnerKills, int& winnerDeaths,
                                         int& loserKills, int& loserDeaths) {
    // Base: winning team gets 5 kills per round won, losing team gets 5 kills per round won
    winnerKills = winnerScore * 5;
    winnerDeaths = loserScore * 5;

    // Extra deaths for winning team in rounds they won (not always flawless)
    int deathsInWins = 0;
    for (int i = 0; i < winnerScore; i++) {
        deathsInWins += rand() % 5;
    }

    // Extra kills for winning team in rounds they lost (they still get some kills)
    int killsInLosses = 0;
    for (int i = 0; i < loserScore; i++) {
        killsInLosses += rand() % 5;
    }

    winnerKills += killsInLosses;
    winnerDeaths += deathsInWins;

    // Losing team's stats are the mirror
    loserKills = winnerDeaths;
    loserDeaths = winnerKills;
}


// ============================================================
//  STEP 3: Calculate Role-Based Weights
// ============================================================

MatchSimulator::StatWeights MatchSimulator::calculatePlayerWeights(const vector<Player>& roster) {
    StatWeights weights;
    size_t n = roster.size();
    weights.killWeights.resize(n, 0.0);
    weights.deathWeights.resize(n, 0.0);
    weights.assistWeights.resize(n, 0.0);

    double totalKillWeight = 0.0, totalDeathWeight = 0.0, totalAssistWeight = 0.0;

    for (size_t i = 0; i < n; i++) {
        const Player& player = roster[i];
        string role = player.getRole();

        if (role == "AWP") {
            weights.killWeights[i] = (player.getAwping() * 4.25 + player.getGameSense() * 2.2 + player.getAim() * 2.0);
            weights.deathWeights[i] = (64.0 - player.getGameSense() * 0.6 + player.getTeamwork() * 0.6);
            weights.assistWeights[i] = (player.getTeamwork() * 0.8);
        }
        else if (role == "Entry") {
            weights.killWeights[i] = (player.getAim() * 4.0 + player.getEntrying() * 4.2 + player.getTeamwork() * 1.5);
            weights.deathWeights[i] = (85.0 - player.getGameSense() * 0.4 + player.getTeamwork() * 0.3);
            weights.assistWeights[i] = (player.getTeamwork() * 1.0 + player.getEntrying() * 0.5);
        }
        else if (role == "Lurker") {
            weights.killWeights[i] = (player.getAim() * 2.925 + player.getGameSense() * 3.8 + player.getTeamwork() * 2.0);
            weights.deathWeights[i] = (76.0 - player.getGameSense() * 1.0 + player.getTeamwork() * 1.0);
            weights.assistWeights[i] = (player.getTeamwork() * 1.0);
        }
        else if (role == "Support") {
            weights.killWeights[i] = (player.getAim() * 3.0 + player.getGameSense() * 2.8 + player.getTeamwork() * 3.5);
            weights.deathWeights[i] = (85.0 - player.getGameSense() * 1.0 + player.getTeamwork() * 0.8);
            weights.assistWeights[i] = (player.getTeamwork() * 1.8 + player.getGameSense() * 0.6);
        }
        else if (role == "IGL") {
            weights.killWeights[i] = (player.getAim() * 2.2 + player.getTeamwork() * 3.5 + player.getGameSense() * 2.0);
            weights.deathWeights[i] = (82.0 - player.getGameSense() * 1.4 + player.getTeamwork() * 1.3);
            weights.assistWeights[i] = (player.getTeamwork() * 1.6 + player.getGameSense() * 0.8);
        }
        else { // Rifler (default)
            weights.killWeights[i] = (player.getAim() * 4.0 + player.getMovement() * 2.5 + player.getGameSense() * 2.5);
            weights.deathWeights[i] = (82.0 - player.getGameSense() * 1.4 + player.getTeamwork() * 1.3);
            weights.assistWeights[i] = (player.getTeamwork() * 1.0);
        }

        totalKillWeight += weights.killWeights[i];
        totalDeathWeight += weights.deathWeights[i];
        totalAssistWeight += weights.assistWeights[i];
    }

    // Prevent division by zero
    if (totalKillWeight == 0) totalKillWeight = 1;
    if (totalDeathWeight == 0) totalDeathWeight = 1;
    if (totalAssistWeight == 0) totalAssistWeight = 1;

    // Normalize to sum to 1.0
    for (size_t i = 0; i < n; i++) {
        weights.killWeights[i] /= totalKillWeight;
        weights.deathWeights[i] /= totalDeathWeight;
        weights.assistWeights[i] /= totalAssistWeight;
    }

    return weights;
}


// ============================================================
//  STEP 4: Distribute Stats to Individual Players
// ============================================================

// Helper: distribute a pool of stats using weighted random selection
static void distributePool(int total, const vector<double>& weights, vector<int>& out, int maxPerPlayer = -1) {
    int remaining = total;
    size_t n = weights.size();

    while (remaining > 0) {
        double randValue = (rand() % 100) / 100.0;
        double cumulative = 0.0;
        bool assigned = false;

        for (size_t i = 0; i < n; i++) {
            cumulative += weights[i];
            if (randValue <= cumulative) {
                if (maxPerPlayer < 0 || out[i] < maxPerPlayer) {
                    out[i]++;
                    remaining--;
                    assigned = true;
                }
                break;
            }
        }

        if (!assigned) {
            out[rand() % n]++;
            remaining--;
        }
    }
}

void MatchSimulator::distributeStats(const vector<Player>& roster,
                                     const StatWeights& weights,
                                     int totalKills, int totalDeaths, int totalAssists,
                                     int totalRounds,
                                     const string& teamName,
                                     MatchResult& result) {
    size_t n = roster.size();

    vector<int> kills(n, 0);
    vector<int> deaths(n, 0);
    vector<int> assists(n, 0);

    // Distribute using weighted random
    distributePool(totalKills, weights.killWeights, kills);
    distributePool(totalDeaths, weights.deathWeights, deaths, totalRounds); // cap deaths at total rounds
    distributePool(totalAssists, weights.assistWeights, assists);

    // Build PlayerMatchStats for each player and compute HLTV rating
    for (size_t i = 0; i < n; i++) {
        PlayerMatchStats stats;
        stats.playerName = roster[i].getName();
        stats.teamName = teamName;
        stats.kills = kills[i];
        stats.deaths = deaths[i];
        stats.assists = assists[i];

        // Use HLTVCalculator to get rating and ADR
        HLTVResult hltv = HLTVCalculator::calculate(
                kills[i], deaths[i], assists[i],
                roster[i].getRole(), totalRounds
        );

        stats.hltvRating = hltv.hltvRating;
        stats.adr = hltv.adr;

        // Store in result, keyed by player name
        result.playerStats[stats.playerName] = stats;
    }
}
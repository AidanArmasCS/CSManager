//
// Created by Main on 1/29/2025.
//

#include "Match.h"
#include <random>
#include <vector>
#include <string>
#include <iostream>
#include <Team.h>
#include <algorithm>

using namespace std;

Match::Match(Team* team1Name, Team* team2Name) {
    team1 = team1Name;
    team2 = team2Name;

}

string Match::getWinner() const {
    return winner;
}

void Match::displayResult() const {
    cout << "Match Result: " << endl;
    cout << team1->getName() << " [" << team1score << "] - ["
         << team2score << "] " << team2->getName() << endl;

    if (!winner.empty()) {
        cout << "Winner: " << winner << endl;
    }
    else {
        cout << "Match has not been simulated yet" << endl;
    }
}

void Match::simulateMatch() {
    if (!winner.empty()) {
        cout << "This match has already been played. Winner: " << winner << endl;
        return;
    }

    if (team1 == nullptr || team2 == nullptr) {
        cout << "Error: One or both teams are missing!" << endl;
        return;
    }

    // Get team ratings
    int team1Rating = team1->getTeamOverallRating();
    int team2Rating = team2->getTeamOverallRating();

    // Random variance to allow upsets
    int randomFactor = rand() % 17 - 7;  // Adds randomness between -7 and +7
    team1Rating += randomFactor;
    team2Rating -= randomFactor;

    bool team1Wins = team1Rating > team2Rating;
    int ratingDiff = abs(team1Rating - team2Rating);

    //BASE LOSING CALCULATION (if big gap in rating lower rounds for loser (usually) )
    int baseLosingScore = max(1, min(11, 13 - ((ratingDiff / 4) + rand() % 3)));

    if (rand() % 100 < 15) { // 15% chance of close score match despite difference
        baseLosingScore = 9 + (rand() % 2); // CREATES 13-9 score line aka close game
    }

    if (team1Wins) {
        team1score = 13;
        team2score = baseLosingScore;
        team1->recordWin();
        team2->recordLoss();
        winner = team1->getName();
    }
    else {
        team2score = 13;
        team1score = baseLosingScore;
        team1->recordLoss();
        team2->recordWin();
        winner = team2->getName();
    }

    cout << "Match simulated: " << team1->getName() << " [" << team1score
         << "] - [" << team2score << "] " << team2->getName() << endl;
    cout << "Winner: " << winner << endl;


    simulateMatchStats();

}

void Match::resetMatch() {
    winner = "";
    team1score = 0;
    team2score = 0;

    cout << "Match has been reset. Ready for a new simulation." << endl;
}


void Match::simulateMatchStats() {
    cout << "\n===== Player Stats =====\n";

    int winningTeamScore, losingTeamScore;
    Team* winningTeam;
    Team* losingTeam;

    if (team1score > team2score) {
        winningTeamScore = team1score;
        losingTeamScore = team2score;
        winningTeam = team1;
        losingTeam = team2;
    } else {
        winningTeamScore = team2score;
        losingTeamScore = team1score;
        winningTeam = team2;
        losingTeam = team1;
    }

    int winningTeamDeaths = losingTeamScore * 5;
    int winningTeamKills = winningTeamScore * 5;

    int winningTeamDeathsInRoundWins = 0;
    for (int i = 0; i < winningTeamScore; i++) {
        winningTeamDeathsInRoundWins += rand() % 5;
    }

    int winningTeamKillsInRoundsLost = 0;
    for (int i = 0; i < losingTeamScore; i++) {
        winningTeamKillsInRoundsLost += rand() % 5;
    }

    winningTeamKills += winningTeamKillsInRoundsLost;
    winningTeamDeaths += winningTeamDeathsInRoundWins;

    int losingTeamKills = winningTeamDeaths;
    int losingTeamDeaths = winningTeamKills;

    // **Display Baseline Totals**
    cout << winningTeam->getName() << " Baseline: " << winningTeamKills << " Kills | " << winningTeamDeaths << " Deaths\n";
    cout << losingTeam->getName() << " Baseline: " << losingTeamKills << " Kills | " << losingTeamDeaths << " Deaths\n";

    // **Step 2: Distribute Stats Per Player**
    for (Team* team : {winningTeam, losingTeam}) {
        cout << "\n" << team->getName() << " Match Stats:\n";

        vector<Player>& players = team->getRoster();
        if (players.empty()) {
            cout << "Error: No players found for " << team->getName() << endl;
            continue;
        }

        vector<int> kills(players.size(), 0);
        vector<int> deaths(players.size(), 0);
        vector<int> assists(players.size(), 0);

        vector<double> killWeights(players.size(), 0);
        vector<double> deathWeights(players.size(), 0);
        vector<double> assistWeights(players.size(), 0);

        double totalKillWeight = 0.0, totalDeathWeight = 0.0, totalAssistWeight = 0.0;

        for (size_t i = 0; i < players.size(); i++) {

            Player& player = players[i];

            if (player.getRole() == "AWP") { // AWP CASE
                killWeights[i] = (player.getAwping() * 4.25 + player.getGameSense() * 2.2 + player.getAim() * 2.0);
                totalKillWeight += killWeights[i];

                // **Death Weighting
                deathWeights[i] = (64.0 - player.getGameSense() * 0.6 + player.getTeamwork() * 0.6);
                totalDeathWeight += deathWeights[i];

                // **Assist Weighting (Higher for Supports & IGLs)**
                assistWeights[i] = (player.getTeamwork() * 1.0);
                totalAssistWeight += assistWeights[i];
            }
            else if (player.getRole() == "Entry") { // ENTRY CASE
                // **Kill Weighting (Higher for Star Players)**
                killWeights[i] = (player.getAim() * 4.0 + player.getEntrying() * 4.2 + player.getTeamwork() * 1.5);
                totalKillWeight += killWeights[i];

                // **Death Weighting (Higher for Supports & IGLs)**
                deathWeights[i] = (85.0 - player.getGameSense() * 0.4 + player.getTeamwork() * 0.3);
                totalDeathWeight += deathWeights[i];

                // **Assist Weighting (Higher for Supports & IGLs)**
                assistWeights[i] = (player.getTeamwork() * 1.3 + player.getEntrying() * 1.1);
                totalAssistWeight += assistWeights[i];
            }
            else if (player.getRole() == "Lurker") { // LURKER CASE
                // **Kill Weighting (Higher for Star Players)**
                killWeights[i] = (player.getAim() * 2.925 + player.getGameSense() * 3.8 + player.getTeamwork() * 2.0);
                totalKillWeight += killWeights[i];

                // **Death Weighting (Higher for Supports & IGLs)**
                deathWeights[i] = (76.0 -  player.getGameSense() * 1.0 + player.getTeamwork() * 1.0);
                totalDeathWeight += deathWeights[i];

                // **Assist Weighting (Higher for Supports & IGLs)**
                assistWeights[i] = (player.getTeamwork() * 1.4);
                totalAssistWeight += assistWeights[i];
            }
            else if (player.getRole() == "Support") { // SUPPORT CASE
                // **Kill Weighting (Higher for Star Players)**
                killWeights[i] = (player.getAim() * 3.0 + player.getGameSense() * 2.8 + player.getTeamwork() * 3.5);
                totalKillWeight += killWeights[i];

                // **Death Weighting (Higher for Supports & IGLs)**
                deathWeights[i] = (85.0 - player.getGameSense() * 1.0 + player.getTeamwork() * 0.8);
                totalDeathWeight += deathWeights[i];

                // **Assist Weighting (Higher for Supports & IGLs)**
                assistWeights[i] = (player.getTeamwork() * 3.0 + player.getGameSense() * 1.5);
                totalAssistWeight += assistWeights[i];
            }
            else if (player.getRole() == "IGL") { // IGL CASE
                // **Kill Weighting (Higher for Star Players)**
                killWeights[i] = (player.getAim() * 2.2 + player.getTeamwork() * 3.5 + player.getGameSense() * 2.0);
                totalKillWeight += killWeights[i];

                // **Death Weighting (Higher for Supports & IGLs)**
                deathWeights[i] = (82.0 - player.getGameSense() * 1.4 + player.getTeamwork() * 1.3);
                totalDeathWeight += deathWeights[i];

                // **Assist Weighting (Higher for Supports & IGLs)**
                assistWeights[i] = (player.getTeamwork() * 2.8 + player.getGameSense() * 2.0);
                totalAssistWeight += assistWeights[i];
            }
            else { // RIFLER CASE
                // **Kill Weighting (Higher for Star Players)**
                killWeights[i] = (player.getAim() * 4.0 + player.getMovement() * 2.5 + player.getGameSense() * 2.5);
                totalKillWeight += killWeights[i];

                // **Death Weighting (Higher for Supports & IGLs)**
                deathWeights[i] = (82.0 - player.getGameSense() * 1.4 + player.getTeamwork() * 1.3);
                totalDeathWeight += deathWeights[i];

                // **Assist Weighting (Higher for Supports & IGLs)**
                assistWeights[i] = (player.getTeamwork() * 1.5);
                totalAssistWeight += assistWeights[i];
            }
        }
        // **Prevent division by zero**
        if (totalKillWeight == 0) totalKillWeight = 1;
        if (totalDeathWeight == 0) totalDeathWeight = 1;
        if (totalAssistWeight == 0) totalAssistWeight = 1;

        // **Normalize weights**
        for (size_t i = 0; i < players.size(); i++) {
            killWeights[i] /= totalKillWeight;
            deathWeights[i] /= totalDeathWeight;
            assistWeights[i] /= totalAssistWeight;
        }

        int totalTeamKills = (team == winningTeam) ? winningTeamKills : losingTeamKills;
        int totalTeamDeaths = (team == winningTeam) ? winningTeamDeaths : losingTeamDeaths;
        int totalTeamAssists = totalTeamKills * 0.25;

        // **Distribute Kills**
        int remainingKills = totalTeamKills;
        while (remainingKills > 0) {
            double randValue = (rand() % 100) / 100.0;
            double cumulative = 0.0;
            bool assigned = false;

            for (size_t i = 0; i < players.size(); i++) {
                cumulative += killWeights[i];
                if (randValue <= cumulative) {
                    kills[i]++;
                    remainingKills--;
                    assigned = true;
                    break;
                }
            }

            if (!assigned) {
                kills[rand() % players.size()]++;
                remainingKills--;
            }
        }

        // **Distribute Deaths**
        int remainingDeaths = totalTeamDeaths;
        while (remainingDeaths > 0) {
            double randValue = (rand() % 100) / 100.0;
            double cumulative = 0.0;
            bool assigned = false;

            for (size_t i = 0; i < players.size(); i++) {
                cumulative += deathWeights[i];
                if (randValue <= cumulative && deaths[i] < (team1score + team2score)) {
                    deaths[i]++;
                    remainingDeaths--;
                    assigned = true;
                    break;
                }
            }

            if (!assigned) {
                deaths[rand() % players.size()]++;
                remainingDeaths--;
            }
        }

        // **Distribute Assists**
        int remainingAssists = totalTeamAssists;
        while (remainingAssists > 0) {
            double randValue = (rand() % 100) / 100.0;
            double cumulative = 0.0;
            bool assigned = false;

            for (size_t i = 0; i < players.size(); i++) {
                cumulative += assistWeights[i];
                if (randValue <= cumulative) {
                    assists[i]++;
                    remainingAssists--;
                    assigned = true;
                    break;
                }
            }

            if (!assigned) {
                assists[rand() % players.size()]++;
                remainingAssists--;
            }
        }

        // **Apply Match Stats to Each Player**
        for (size_t i = 0; i < players.size(); i++) {
            players[i].addMatchStats(kills[i], deaths[i], assists[i]);
        }

        // **Print Stats**
        for (size_t i = 0; i < players.size(); i++) {
            cout << players[i].getName() << " - "
                 << kills[i] << " Kills | "
                 << deaths[i] << " Deaths | "
                 << assists[i] << " Assists | "
                 << simulateMatchRatings(&players[i]) << "\n";
        }
    }
}

void Match::simulateRounds() {
    cout << "\n===== Round-by-Round Breakdown =====" << endl;

    int roundsWonByWinningTeam = max(team1score, team2score);
    int roundsWonByLosingTeam = min(team1score, team2score);
    int totalRounds = roundsWonByLosingTeam + roundsWonByWinningTeam;

    Team *winningTeam = (team1score > team2score) ? team1 : team2;
    Team *losingTeam = (winningTeam == team1) ? team2 : team1;


    vector<int> roundWinners;
    for (int i = 0; i < roundsWonByWinningTeam - 1; i++) roundWinners.push_back(1); // STORE ROUND COUNTS IN ROUND WINNER VECTOR
    for (int i = 0; i < roundsWonByLosingTeam; i++) roundWinners.push_back(0);

    shuffle(roundWinners.begin(), roundWinners.end(), std::mt19937(std::random_device()())); // RANDOMIZE ROUNDS
    roundWinners.push_back(1);

    for (int round = 1; round <= totalRounds; round++) {
        Team *roundWinner = (roundWinners[round - 1] == 1) ? winningTeam : losingTeam;

        Player& keyPlayer = roundWinner->getRoster()[rand() % roundWinner->getRoster().size()];

        string roundType;
        int eventChance = rand() % 100;

        if (eventChance < 10) {
            roundType = "Flawless Round, all 5 stayed alive.";
        }
        else if (eventChance < 25) {
            roundType = "won in a 1v1 clutch by " + keyPlayer.getName() + ".";
        }
        else if (eventChance < 45) {
            roundType = "secured by " + keyPlayer.getName() + " with a multi-kill.";
        }
        else {
            roundType = "A typical round that was back and forth.";
        }
        // Print round result
        cout << "Round " << round << " won by " << roundWinner->getName() << " - " << roundType << endl;
    }
}

double Match::simulateMatchRatings(Player* playerName) {
    double totalRounds = team1score + team2score;

    if (totalRounds == 0) return 0;

    double kills = playerName->getMatchKills();
    double deaths = playerName->getMatchDeaths();
    double assists = playerName->getMatchAssists();

    double fpr, dpr, adr, impact;

    fpr = kills / totalRounds;
    dpr = deaths / totalRounds;

    //ROUNDS SURVIVED CALC
    double roundsSurvived = totalRounds - deaths;

    // **Total Damage for ADR Calculation**
    double totalDamage = 0.0;

    for (int i = 0; i < kills; i++) {
        double killDamage;

        if (playerName->getRole() == "AWP") {
            // **AWP Shots Should Do 85-100 Damage (Most are 100)**
            killDamage = 85 + (rand() % 16);  // 85-100
            if ((rand() % 100) < 80) { // 80% chance to deal 95-100 damage
                killDamage = 95 + (rand() % 6);
            }
        } else {
            // **Standard Kill Damage**
            killDamage = 50 + (rand() % 51);  // 50-100
            if ((rand() % 100) < 75) { // 75% bias toward 75-95
                killDamage = 75 + (rand() % 21);
            }
        }
        totalDamage += killDamage;
    }

// **Assist Damage (AWPers Should Have a Lower Assist Damage Range)**
    for (int i = 0; i < assists; i++) {
        double assistDamage;

        if (playerName->getRole() == "AWP") {
            assistDamage = 20 + (rand() % 31);  // AWP Assist Damage: 20-50 (Lower than rifles)
        } else {
            assistDamage = 40 + (rand() % 31);  // Rifle Assist Damage: 40 - 70 dmg per assist
            if ((rand() % 100) < 70) { // 70% chance bias for 50 - 65
                assistDamage = 50 + (rand() % 16);
            }
        }
        totalDamage += assistDamage;
    }

// **Baseline Chip Damage (General Tagging Damage)**
    if (playerName->getRole() == "AWP") {
        totalDamage += ((totalRounds - (kills + assists)) * (15 + (rand() % 11))); // AWPers deal 15-25 chip damage
    } else {
        totalDamage += ((totalRounds - (kills + assists)) * (25 + (rand() % 16))); // Riflers deal 10-20 chip damage
    }

// **Final ADR Calculation**
    adr = totalDamage / totalRounds;

    playerName->setMatchADR(adr);


    impact  = (2.13 * fpr) + (0.42 * (assists / totalRounds)) - 0.41;

    // **HLTV 2.0 Rating Calculation**
    double hltvRating;


    if (playerName->getRole() == "AWP" ) {
        hltvRating = (0.42 * fpr) + (-0.33 * dpr) + (0.33 * impact) + (0.0065 * adr) + 0.2;
    }
    else if (playerName->getRole() == "IGL" ) {
        hltvRating = (0.5 * fpr) + (-0.4 * dpr) + (0.33 * impact) + (0.0065 * adr) + 0.13;
    }
    else {
        hltvRating = (0.5 * fpr) + (-0.4 * dpr) + (0.33 * impact) + (0.0065 * adr) + 0.1;
    }

    // **Debugging Output**
  /*  cout << playerName->getName() << " FPR: " << fpr << endl;
    cout << playerName->getName() << " ADR: " << adr << endl;
    cout << playerName->getName() << " Impact: " << impact << endl; */

    return hltvRating;
}

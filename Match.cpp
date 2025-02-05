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
    int randomFactor = rand() % 17 - 8;  // Adds randomness between -8 and +8
    team1Rating += randomFactor;
    team2Rating -= randomFactor;

    bool team1Wins = team1Rating > team2Rating;
    int ratingDiff = abs(team1Rating - team2Rating);

    //BASE LOSING CALCULATION (if big gap in rating lower rounds for loser (usually) )
    int baseLosingScore = max(1, min(11, 13 - ((ratingDiff / 8) + rand() % 3)));

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

    simulateRounds();

}

void Match::resetMatch() {
    winner = "";
    team1score = 0;
    team2score = 0;

    cout << "Match has been reset. Ready for a new simulation." << endl;
}

void Match::simulateMatchStats() {

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


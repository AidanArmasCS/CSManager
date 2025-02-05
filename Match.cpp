//
// Created by Main on 1/29/2025.
//

#include "Match.h"
#include <vector>
#include <string>
#include <iostream>
#include <Team.h>

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
        baseLosingScore = 9 + (rand() % 2); // CREATES 13-10 socre line aka close game
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

}

void Match::resetMatch() {
    winner = "";
    team1score = 0;
    team2score = 0;

    cout << "Match has been reset. Ready for a new simulation." << endl;
}

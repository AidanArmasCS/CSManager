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

    result = rand() % 2;

    if (result == 0) { // TEAM 1 WINS
        team1score = 13;
        team2score = 1 + (rand() % 11);

        team1->recordWin();
        team2->recordLoss();
        winner = team1->getName();
    }
    else {
        team2score = 13;
        team1score = 1 + (rand() % 11);

        team2->recordWin();
        team1->recordLoss();
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

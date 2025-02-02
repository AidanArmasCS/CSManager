//
// Created by Main on 1/29/2025.
//

#include "Team.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;

Team::Team(const string &teamName, const string &usernationality) {
    name = teamName;
    wins = 0;
    losses = 0;
    nationality = usernationality;
}

Team::~Team() {}


// TEAM MANAGEMENT METHODS

string Team::getName() const { // METHOD FOR RETURNING TEAM NAME
    return name;
}
string Team::getNationality() const {
    return nationality;
}


void Team::addPlayer(const Player &newPlayer) {
    if (roster.size() < 5) {
        roster.push_back(newPlayer);
    } else {
        cout << "Failed Adding Player. " << name << " already has the 5 players" << endl;
    }
}

bool Team::removePlayer(const string &playerName) {
        if (roster.empty()) {
            cout << "Failed Removing Player. " << name << " already has 0 players" << endl;
            return false;
        }

            for (auto it = roster.begin(); it != roster.end(); ++it) {
                if (it->getName() == playerName) {
                    roster.erase(it);
                    cout << playerName << " has been removed from " << name << "\n";
                    return true;
        }
    }
    cout << "Player " << playerName << " not found in " << name << endl;
    return false;
}

void Team::displayRoster() const {
    if (roster.empty()) {
        cout << "Roster for " << name << " is empty" << endl;
        return;
    }

    cout << name << " Team Roster: " << endl;
    for (const auto &player : roster)  {
        player.displayPlayer();
    }
}

void Team::recordWin() {
    wins++;
}

void Team::recordLoss() {
    losses++;
}

void Team::displayRecord() {
    cout << "Team: " << name << " | Wins: " << wins << " | Losses: " << losses << endl;
}






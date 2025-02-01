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


void Team::addPlayer(const Player &newPlayer) {
    if (roster.size() < 5) {
        roster.push_back(newPlayer);
}
    else {
        cout << "Failed Adding Player. " << name << " already has the 5 players" << endl;
    }

}




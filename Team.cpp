//
// Created by Main on 1/29/2025.
//

#include "Team.h"
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <cmath>

class Player;

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
    cout << "Team: " << name << " (Overall Rating: " << getTeamOverallRating() << ")\n";

    if (roster.empty()) {
        cout << "No players in the team.\n";
        return;
    }

    for (Player p : roster) {
        p.displayPlayer();
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

int Team::getTeamOverallRating() const {
    if (roster.empty()) { // ensure roster isnt 0 for divison
        return 0;
    }
    vector<int> playerRatings;
    playerRatings.reserve(roster.size());

    for (Player p: roster) { // collect ratings from roster
        playerRatings.push_back(p.getAdjustedOverallRating());
    }

//sort from highest to lowest
    sort(playerRatings.rbegin(), playerRatings.rend());

    double totalRating = 0;

    for (size_t i = 0; i < playerRatings.size(); i++) {
        if (i < 3)  // Top 3 players get a 1.1x boost
            totalRating += playerRatings[i] * 1.1;
        else  // Bottom 2 players get a 0.9x reduction
            totalRating += playerRatings[i] * 0.9;
    }

    double totalChemistry = getTeamChemistry();
    double baseTeamRating = totalRating / static_cast<double>(playerRatings.size());

    //APPLY CHEMISTRY EFFECTS
    double chemistryEffect = (totalChemistry - 65) / 70.0; // NORMALIZE CHEM IMPACT
    double scaledChemistryImpact = 1.0 + (chemistryEffect * 0.15);
    double finalRating = baseTeamRating * scaledChemistryImpact;

    return static_cast<int>(finalRating);
}


int Team::getTeamChemistry() const {
    if (roster.empty()) {
        return 50.0; // DEFAULT FOR EMPTY ROSTER
    }

    double totalChemistry = 0;
    vector<double> chemistryValues;

    //CALC EACH PLAYER CHEM
    for (Player player : roster) {
        //temp vector to exclude current so not compared to self
        vector<Player> teammates;
        for (const Player &teammate : roster) {
            if (&teammate != &player) {
                teammates.push_back(teammate);
            }
        }

        //CALL CHEM CALC
        player.calculateChemistry(teammates);

        //STORE FOR AVG
        chemistryValues.push_back(player.getChemistry());
    }
    //SORT HIGHEST TO LOWEST
    sort(chemistryValues.rbegin(), chemistryValues.rend());

    // Compute weighted chemistry
    for (size_t i = 0; i < chemistryValues.size(); i++) {
        if (i < 3) { // Top 3 players get a 1.1x weight
            totalChemistry += chemistryValues[i] * 1.1;
        } else { // Bottom 2 players get a 0.9x weight
            totalChemistry += chemistryValues[i] * 0.9;
        }
    }
    //NORMALIZE
    totalChemistry = totalChemistry / static_cast<double>(roster.size());
    return static_cast<int>(totalChemistry);
}





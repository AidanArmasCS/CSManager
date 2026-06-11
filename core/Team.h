//
// Created by Main on 1/29/2025.
// Refactored: Layer 1 - Pure data model
//

#ifndef TEAM_H
#define TEAM_H

#include <vector>
#include <string>
#include "Player.h"

using namespace std;

class Team {
private:
    string name;
    string nationality;
    int ranking;
    vector<Player> roster;

public:
    Team(const string& teamName, const string& teamNationality, int teamRanking);
    ~Team();

    // --- Identity ---
    string getName() const;
    string getNationality() const;
    int getRanking() const;
    void setRanking(int newRanking);

    // --- Roster Management ---
    bool addPlayer(const Player& newPlayer);
    bool removePlayer(const string& playerName);
    const vector<Player>& getRoster() const;
    int getRosterSize() const;
    bool isRosterFull() const;

    // --- Display ---
    void displayRoster() const;
};

#endif // TEAM_H
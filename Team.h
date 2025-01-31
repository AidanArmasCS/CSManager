//
// Created by Main on 1/29/2025.
//

#ifndef TEAM_H
#define TEAM_H

#include <vector>
#include <string>
#include "player.h"

using namespace std;



class team {
private:
    string name;
    vector<Player> roster;
    int wins;
    int losses;


public:
    //Constructor
    team(const string &teamName);
    //Destructor
    ~team();

    // TEAM MANAGEMENT
    void addPlayer(const Player &newPlayer);
    bool removePlayer(const string &playerName);
    void displayRoster() const;

    void addTeam(const string &teamName, const string &nationality);
    bool removeTeam(const string &teamName);
    void displayTeams() const;

    //WIN LOSS RECORD
    void recordWin();
    void recordLoss();
    void displayRecord();
};



#endif //TEAM_H

//
// Created by Main on 1/29/2025.
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
    vector<Player> roster;
    int wins;
    int losses;
    string nationality;


public:
    //Constructor
    Team(const string &teamName, const string &usernationality);
    //Destructor
    ~Team();

    // TEAM MANAGEMENT
    void addPlayer(const Player &newPlayer);
    bool removePlayer(const string &playerName);
    string getName() const;
    string getNationality() const;
    int getTeamChemistry() const;


    //TEAM VIEWING
    int getTeamOverallRating() const;
    void displayRoster() const;
    vector<Player>& getRoster() { return roster; }



    //WIN LOSS RECORD
    void recordWin();
    void recordLoss();
    void displayRecord();

};



#endif //TEAM_H
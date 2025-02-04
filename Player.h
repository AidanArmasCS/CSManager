//
// Created by Main on 1/29/2025.
//

#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <string>

using namespace std;



class Player {
private:
    string name;
    string nationality;
    string role;

    int aim;
    int movement;
    int gameSense;
    int teamwork;
    int entrying;
    int awp;
    int clutch;
    int consistency;



public:

    Player(const string &playerName, const string &playerRole, const string &playerNationality,
           int aim, int movement, int gameSense, int teamwork,
           int entrying, int awp, int clutch, int consistency);
    ~Player();

    string getName() const;
    string getRole() const;
    string getNationality() const;
    int getOverallRating() const;

    void displayPlayer() const;




};



#endif //PLAYER_H

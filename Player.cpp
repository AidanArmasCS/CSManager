//
// Created by Main on 1/29/2025.
//

#include "Player.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;

Player::Player(const string &playerName, const string &playerRole, const string &playerNationality,
               int aim, int movement, int gameSense, int teamwork,
               int entrying, int awp, int clutch, int consistency) {
    this->name = playerName;
    this->role = playerRole;
    this->nationality = playerNationality;
    this->aim = aim;
    this->movement = movement;
    this->gameSense = gameSense;
    this->teamwork = teamwork;
    this->entrying = entrying;
    this->awp = awp;
    this->clutch = clutch;
    this->consistency = consistency;
}

Player::~Player() {}

//GETTERS
string Player::getName() const { return name; }
string Player::getRole() const { return role; }
string Player::getNationality() const { return nationality; }


int Player::getOverallRating() const {
    int overallRating;
    overallRating = (aim + movement + gameSense + teamwork + entrying + awp + clutch + consistency) / 9;
    overallRating += 14;
    if (overallRating >= 99) {
        overallRating = 99;
        return overallRating;
    }
    return overallRating;
}



//PRINT INFO OF PLAYER
void Player::displayPlayer() const {
        cout << "Player: " << name << " | Role: " << role << " | Nationality: " << nationality << endl;
        cout << "Skills:\n";
        cout << "Aim: " << aim << ", Movement: " << movement << ", Game Sense: " << gameSense << ", Teamwork: " << teamwork << endl;
        cout << "Entrying: " << entrying << ", AWP: " << awp << ", Clutch: " << clutch << ", Consistency: " << consistency << endl;
        cout << "Overall Rating: " << getOverallRating() << endl;
}

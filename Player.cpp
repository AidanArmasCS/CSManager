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
    double overallRating = 0;
    string playerRole = getRole();

    if (playerRole == "IGL") {
        overallRating = ( ( (aim * 0.85) + (movement * 0.85) + (gameSense * 1.5) + (teamwork * 2.5) + (entrying * 0.75) + (awp * 0.5) + (clutch * 0.66) + (consistency * 1.2) ) / 9 );
        overallRating += 14;
    }
    else if (playerRole == "AWP") {
        overallRating = (( (aim * 1.1) + (movement * 1.1) + (gameSense * 1.05) + (teamwork * 1.05) + (entrying * 0.8) + (awp * 1.5) + (clutch * 0.4) + (consistency * 1.1) ) / 9 );
        overallRating += 14;
    }
    else if (playerRole == "Entry") {
        overallRating = ( ( (aim * 1.2) + (movement * 1.0) + (gameSense * 0.65) + (teamwork * 0.70) + (entrying * 1.7) + (awp * 0.25) + (clutch * 0.25) + (consistency * 1.05) ) / 9 );
        overallRating += 14;
    }
    else if (playerRole == "Rifler") {
        overallRating = ( ( (aim * 1.2) + (movement * 1.05) + (gameSense * 1.05) + (teamwork * 1.05) + (entrying * 0.9) + (awp * 0.33) + (clutch * 1.05) + (consistency * 1.05) ) / 9 );
        overallRating += 14;
    }
    else if (playerRole == "Lurker") {
        overallRating = ( ( (aim * 1.2) + (movement * 1.1) + (gameSense * 1.3) + (teamwork * 1.25) + (entrying * 0.33) + (awp * 0.25) + (clutch * 1.2) + (consistency * 1.18) ) / 9 );
        overallRating += 14;
    }


    if (overallRating >= 99.0) {
        overallRating = 99.0;
        return static_cast<int>(overallRating);
    }
    return static_cast<int>(overallRating);
}



//PRINT INFO OF PLAYER
void Player::displayPlayer() const {
        cout << "Player: " << name << " | Role: " << role << " | Nationality: " << nationality << endl;
        cout << "Skills:\n";
        cout << "Aim: " << aim << ", Movement: " << movement << ", Game Sense: " << gameSense << ", Teamwork: " << teamwork << endl;
        cout << "Entrying: " << entrying << ", AWP: " << awp << ", Clutch: " << clutch << ", Consistency: " << consistency << endl;
        cout << "Overall Rating: " << getOverallRating() << endl;
}

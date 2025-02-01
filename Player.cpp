//
// Created by Main on 1/29/2025.
//

#include "Player.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;

Player::Player(const string &playerName, const string &playerRole, const string &playerNationality) {
    name = playerName;
    role = playerRole;
    nationality = playerNationality;
}

Player::~Player() {}

// PLAYER MANAGEMENT METHODS

string Player::getName() const {
    return name;
}

string Player::getRole() const {
    return role;
}

string Player::getNationality() const {
    return nationality;
}

void Player::displayPlayer() const {
    cout << "Player: " << name << " | Role: " << role << " | Nationality: " << nationality << endl;
}

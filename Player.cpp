//
// Created by Main on 1/29/2025.
//

#include "Player.h"
#include <vector>
#include <string>
#include <iostream>
#include <Trait.h>
#include <unordered_map>

using namespace std;

Player::Player(const string &playerName, const string &playerRole, const string &playerNationality,
               int aim, int movement, int gameSense, int teamwork,
               int entrying, int awp, int clutch, int consistency,
               PlayerStyle s, const vector<string>& assignedTraits ) {
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
    this->style = s;

    // Convert trait names to Trait objects
    for (const string& traitName : assignedTraits) {
        if (predefinedTraits.find(traitName) != predefinedTraits.end()) {
            traits.push_back(predefinedTraits[traitName]); // Store the Trait object
        } else {
            cout << "Warning: Trait '" << traitName << "' not found in predefinedTraits!" << endl;
        }
    }

}

void Player::addTrait(const Trait& trait) {
    traits.push_back(trait);
    chemistry += trait.effect; // GIVE BOOST OR NEGATIVE TO BASE CHEM
}
void Player::calculateChemistry(vector<Player>& teammates) { // THIS METHOD FINDS THE PLAYER'S INDIVIDUAL CHEMISTRY
    // Adjust chemistry based on teammate styles
    for (auto &teammate: teammates) {
        if (teammate.getName() != this->getName()) {
            if (teammate.style != this->style) {
                chemistry -= 2.0; // Penalty for mismatched styles
            } else {
                chemistry += 2.0; // Reward for same style
            }
        }
    }

    // Apply trait effects to chemistry
    for (const Trait& trait : traits) {
        chemistry += trait.effect;  // Directly add trait effect
    }

    // Ensure chemistry stays between 1 and 99
    chemistry = max(1.0, min(99.0, chemistry));
}




Player::~Player() {}

//GETTERS
string Player::getName() const { return name; }
string Player::getRole() const { return role; }
string Player::getNationality() const { return nationality; }
int Player::getAim() const { return aim; }
int Player::getMovement() const { return movement; }
int Player::getEntrying() const { return entrying; }
int Player::getGameSense() const { return gameSense; }
int Player::getTeamwork() const { return teamwork; }



int Player::getAdjustedOverallRating() { // ADJUSTS THE PLAYERS RATINGS BASED OFF OF THE PLAYERS TRAITS
    double adjustedRating = 0.0;


    //MAP TRAITS
    unordered_map<string, int*> attributeMap = {
            {"Selfish",          &teamwork},
            {"Toxic",            &teamwork},
            {"Inconsistent",     &teamwork},
            {"Choker",           &aim},
            {"Tilter",           &teamwork},
            {"Bad Communicator", &teamwork},
            {"Baiter",           &teamwork},
            {"Stubborn",         &teamwork},
            {"Large Ego",        &teamwork},
            {"Lazy",             &teamwork},

            {"Leader",           &teamwork},
            {"Team Player",      &teamwork},
            {"Clutch Master",    &clutch},
            {"Supportive",       &teamwork},
            {"Aim Demon",        &aim},
            {"Tactical Genius",  &gameSense},
            {"Disciplined",      &teamwork},
            {"Versatile",        &teamwork},
            {"Quick Learner",    &gameSense},
            {"Master Communicator", &teamwork},
            {"None", &teamwork}
    };


    //CHANGE THE ATTRIBUTES
    for (const Trait& trait : traits) {
        auto it = attributeMap.find(trait.name);
        if (it != attributeMap.end()) {
            *(it->second) += trait.statBoost;
        }
    }

    // RETURN ADJUSTED RATING FOR STATS
    adjustedRating = getOverallRating();
    return static_cast<int>(adjustedRating);
}


//PRINT INFO OF PLAYER
int Player::getOverallRating() const {
    double overallRating = 0.0;
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
    else if (playerRole == "Support") {
        overallRating = ( ( (aim * 1.0) + (movement * 1.0) + (gameSense * 1.2) + (teamwork * 1.8) + (entrying * 0.6) + (awp * 0.33) + (clutch * 1.05) + (consistency * 1.2) ) / 9 );
        overallRating += 14;
    }


    if (overallRating >= 99.0) {
        overallRating = 99.0;
        return static_cast<int>(overallRating);
    }
    return static_cast<int>(overallRating);
}
void Player::displayPlayer() {
    cout << "Player: " << name << " | Role: " << role << " | Nationality: " << nationality << endl;
    cout << "Skills:\n";
    cout << "Aim: " << aim << ", Movement: " << movement << ", Game Sense: " << gameSense << ", Teamwork: " << teamwork << endl;
    cout << "Entrying: " << entrying << ", AWP: " << awp << ", Clutch: " << clutch << ", Consistency: " << consistency << endl;
    cout << "Overall Rating: " << getOverallRating() << endl;
}
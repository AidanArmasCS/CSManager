// Refactored: Layer 1 - Pure data model
//

#include "player.h"
#include <iostream>

using namespace std;

Player::Player(const string& playerName, const string& playerRole, const string& playerNationality,
               int aim, int movement, int gameSense, int teamwork,
               int entrying, int awp, int clutch, int consistency,
               PlayerStyle s, const vector<string>& assignedTraits)
        : name(playerName), role(playerRole), nationality(playerNationality),
          aim(aim), movement(movement), gameSense(gameSense), teamwork(teamwork),
          entrying(entrying), awp(awp), clutch(clutch), consistency(consistency),
          style(s)
{
    for (const string& traitName : assignedTraits) {
        if (predefinedTraits.find(traitName) != predefinedTraits.end()) {
            traits.push_back(predefinedTraits[traitName]);
        } else {
            cout << "Warning: Trait '" << traitName << "' not found in predefinedTraits!" << endl;
        }
    }
}

Player::~Player() {}

// --- Identity ---
string Player::getName() const { return name; }
string Player::getRole() const { return role; }
string Player::getNationality() const { return nationality; }
PlayerStyle Player::getStyle() const { return style; }
const vector<Trait>& Player::getTraits() const { return traits; }

// --- Raw Stats ---
int Player::getAim() const { return aim; }
int Player::getMovement() const { return movement; }
int Player::getGameSense() const { return gameSense; }
int Player::getTeamwork() const { return teamwork; }
int Player::getEntrying() const { return entrying; }
int Player::getAwping() const { return awp; }
int Player::getClutch() const { return clutch; }
int Player::getConsistency() const { return consistency; }

// --- Display ---
void Player::displayPlayer() const {
    cout << "Player: " << name << " | Role: " << role << " | Nationality: " << nationality << endl;
    cout << "  Aim: " << aim << ", Movement: " << movement
         << ", Game Sense: " << gameSense << ", Teamwork: " << teamwork << endl;
    cout << "  Entrying: " << entrying << ", AWP: " << awp
         << ", Clutch: " << clutch << ", Consistency: " << consistency << endl;

    if (!traits.empty()) {
        cout << "  Traits: ";
        for (size_t i = 0; i < traits.size(); i++) {
            cout << traits[i].name;
            if (i < traits.size() - 1) cout << ", ";
        }
        cout << endl;
    }
}
// Refactored: Layer 1 - Pure data model
//

#include "Team.h"
#include <iostream>

using namespace std;

Team::Team(const string& teamName, const string& teamNationality, int teamRanking)
        : name(teamName), nationality(teamNationality), ranking(teamRanking)
{}

Team::~Team() {}

// --- Identity ---
string Team::getName() const { return name; }
string Team::getNationality() const { return nationality; }
int Team::getRanking() const { return ranking; }
void Team::setRanking(int newRanking) { ranking = newRanking; }

// --- Roster Management ---
bool Team::addPlayer(const Player& newPlayer) {
    if (roster.size() >= 5) {
        cout << "Failed: " << name << " already has 5 players." << endl;
        return false;
    }

    // Prevent duplicate player names
    for (const Player& p : roster) {
        if (p.getName() == newPlayer.getName()) {
            cout << "Failed: " << newPlayer.getName() << " is already on " << name << "." << endl;
            return false;
        }
    }

    roster.push_back(newPlayer);
    return true;
}

bool Team::removePlayer(const string& playerName) {
    for (auto it = roster.begin(); it != roster.end(); ++it) {
        if (it->getName() == playerName) {
            roster.erase(it);
            return true;
        }
    }
    cout << "Player " << playerName << " not found in " << name << "." << endl;
    return false;
}

const vector<Player>& Team::getRoster() const { return roster; }

int Team::getRosterSize() const { return static_cast<int>(roster.size()); }

bool Team::isRosterFull() const { return roster.size() >= 5; }

// --- Display ---
void Team::displayRoster() const {
    cout << "Team: " << name << " | " << nationality << " | Ranking: " << ranking << endl;

    if (roster.empty()) {
        cout << "  No players on roster." << endl;
        return;
    }

    cout << "  Roster (" << roster.size() << "/5):" << endl;
    for (const Player& p : roster) {
        p.displayPlayer();
    }
}
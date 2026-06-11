// Refactored: Layer 1 - Pure data model
//

#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <string>
#include "Trait.h"
#include "PlayerStyle.h"

using namespace std;

class Player {
private:
    string name;
    string role;
    string nationality;

    // Raw stats - these NEVER change after construction
    int aim;
    int movement;
    int gameSense;
    int teamwork;
    int entrying;
    int awp;
    int clutch;
    int consistency;

    PlayerStyle style;
    vector<Trait> traits;

public:
    Player(const string& playerName, const string& playerRole, const string& playerNationality,
           int aim, int movement, int gameSense, int teamwork,
           int entrying, int awp, int clutch, int consistency,
           PlayerStyle s, const vector<string>& assignedTraits);
    ~Player();

    // --- Identity ---
    string getName() const;
    string getRole() const;
    string getNationality() const;
    PlayerStyle getStyle() const;
    const vector<Trait>& getTraits() const;

    // --- Raw Stats (immutable after construction) ---
    int getAim() const;
    int getMovement() const;
    int getGameSense() const;
    int getTeamwork() const;
    int getEntrying() const;
    int getAwping() const;
    int getClutch() const;
    int getConsistency() const;

    // --- Display ---
    void displayPlayer() const;
};

#endif // PLAYER_H
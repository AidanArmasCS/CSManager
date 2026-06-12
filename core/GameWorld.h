//
// GameWorld.h
// Layer 1/Engine - The single source of truth for a save/career.
//
// Owns the calendar (clock) and every team in the world, and tracks
// which team belongs to the player. The main loop drives this object
// forward in time; financials and the event schedule will be added as
// members here later. The engine NEVER prints — it only exposes data
// that a UI layer (text menu now, graphical later) reads and renders.
//

#ifndef GAMEWORLD_H
#define GAMEWORLD_H

#include "Calendar.h"
#include "Team.h"
#include <vector>
#include <string>

using namespace std;

class GameWorld {
private:
    Calendar calendar_;
    vector<Team> teams_;     // owns all teams by value
    int myTeamIndex_ = -1;   // index into teams_, or -1 if unset

public:
    GameWorld() = default;

    // --- Setup ---
    void addTeam(const Team& team);
    // Picks the player's club by name. Returns false if not found.
    bool setMyTeam(const string& teamName);

    // --- Time ---
    const Calendar& getCalendar() const { return calendar_; }
    void advanceDay();
    void advanceDays(int n);

    // --- Teams ---
    const vector<Team>& getTeams() const { return teams_; }
    int getTeamCount() const { return (int)teams_.size(); }

    // The player's club (nullptr if none chosen yet).
    const Team* getMyTeam() const;

    // Borrowed pointers to all teams, for engine calls that expect them
    // (e.g. Tournament). Valid until teams_ is modified.
    vector<const Team*> getTeamPointers() const;
};

#endif // GAMEWORLD_H

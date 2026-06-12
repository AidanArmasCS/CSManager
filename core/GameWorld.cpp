//
// GameWorld.cpp
// Layer 1/Engine - The single source of truth for a save/career.
//

#include "GameWorld.h"

void GameWorld::addTeam(const Team& team) {
    teams_.push_back(team);
}

bool GameWorld::setMyTeam(const string& teamName) {
    for (size_t i = 0; i < teams_.size(); i++) {
        if (teams_[i].getName() == teamName) {
            myTeamIndex_ = (int)i;
            return true;
        }
    }
    return false;
}

void GameWorld::advanceDay() {
    calendar_.advanceDay();
    // Later: fire any scheduled events landing on this date
    // (paydays, match days, tournament starts).
}

void GameWorld::advanceDays(int n) {
    for (int i = 0; i < n; i++) advanceDay();
}

const Team* GameWorld::getMyTeam() const {
    if (myTeamIndex_ < 0 || myTeamIndex_ >= (int)teams_.size()) return nullptr;
    return &teams_[myTeamIndex_];
}

vector<const Team*> GameWorld::getTeamPointers() const {
    vector<const Team*> ptrs;
    ptrs.reserve(teams_.size());
    for (const Team& t : teams_) ptrs.push_back(&t);
    return ptrs;
}

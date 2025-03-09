//
// Created by Main on 1/29/2025.
//

#ifndef PLAYER_H
#define PLAYER_H

#include <utility>
#include <vector>
#include <string>
#include <Trait.h>

using namespace std;

enum class PlayerStyle { // PLAYER STYLES STORAGE
    Aggressive,
    Passive,
    Balanced
};



class Player {
private:
    string name, nationality, role;

    int aim, movement, gameSense, teamwork, entrying, awp, clutch, consistency;

    int matchKills = 0;
    int matchDeaths = 0;
    int matchAssists = 0;
    double matchADR = 0.0;

    PlayerStyle style;
    vector<Trait> traits;
    double chemistry = 65.0; // BASE PLAYER CHEMISTRY


public:

    Player(const string &playerName, const string &playerRole, const string &playerNationality,
           int aim, int movement, int gameSense, int teamwork,
           int entrying, int awp, int clutch, int consistency,
           PlayerStyle s, const vector<string>& assignedTraits);
    ~Player();


    // PLAYER INFO METHODS
    string getName() const;
    string getRole() const;
    string getNationality() const;
    int getOverallRating() const;
    int getAim() const;
    int getMovement() const;
    int getEntrying() const;
    int getGameSense() const;
    int getTeamwork() const;
    int getAwping() const { return awp; }
    void displayPlayer();



    //MATCH METHODS
    void setMatchADR(double adr) {
        matchADR = adr;
    }
    double getMatchADR() const { return matchADR; }
    int getMatchKills() const { return matchKills; }
    int getMatchDeaths() const { return matchDeaths; }
    int getMatchAssists() const { return matchAssists; }
    void addMatchStats(int kills, int deaths, int assists) {
        matchKills += kills;
        matchDeaths += deaths;
        matchAssists += assists;
    }
    void resetMatchStats() {
        matchKills = 0;
        matchDeaths = 0;
        matchAssists = 0;
    }


    //CHEMISTRY METHODS
    void addTrait (const Trait& trait);
    void calculateChemistry(vector<Player>& teammates);
    double getChemistry() const { return chemistry; }
    int getAdjustedOverallRating();



};



#endif //PLAYER_H
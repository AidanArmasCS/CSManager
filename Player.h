//
// Created by Main on 1/29/2025.
//

#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <string>

using namespace std;



class Player {
private:
    string name;
    string nationality;
    string role;

    int aim;
    int movement;
    int gameSense;
    int teamwork;
    int entrying;
    int awp;
    int clutch;
    int consistency;

    int matchKills = 0;
    int matchDeaths = 0;
    int matchAssists = 0;



public:

    Player(const string &playerName, const string &playerRole, const string &playerNationality,
           int aim, int movement, int gameSense, int teamwork,
           int entrying, int awp, int clutch, int consistency);
    ~Player();

    string getName() const;
    string getRole() const;
    string getNationality() const;
    int getOverallRating() const;
    int getAim() const;
    int getMovement() const;
    int getEntrying() const;
    int getGameSense() const;
    int getTeamwork() const;



    void displayPlayer() const;

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



};



#endif //PLAYER_H

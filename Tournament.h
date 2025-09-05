//
// Created by Aidan Armas on 7/3/25.
//

#ifndef CSMANAGER_TOURNAMENT_H
#define CSMANAGER_TOURNAMENT_H

#include "Team.h"
#include "Match.h"
#include "Player.h"
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <algorithm>
#include <cmath>

using namespace std;

class Tournament {
private:
    string format;
    string location;
    string name;
    string tier;
    int prizePool;
    vector<Team*> teams;
    string winner;
    string length;
    string date;
    vector<Team*> placements;
    vector<Team*> seededTeams;
    map<string, PlayerStats> playerStatsMap;


    void setWinner(const string& winningTeam) { winner = winningTeam; }
    void calculateMVP();
    void simulateMatchAndStats(Team* team1, Team* team2);

public:

    // Default Tournament
    Tournament(string tFormat, string tLocation, string tName, string tTier, int tPrizePool, vector<Team*> teamList, string tLength, string tDate);
    ~Tournament();

    void addTeam(Team* team);
    void removeTeam(Team* team);


    void simulateTournament();
    void bracketFormatSim();
    void groupFormatSim();

    void getMVP() { calculateMVP(); }
    void getWinner() { cout << "Tournament Winner: " << winner << endl; }

    void distributePrizeMoney();
    void printPlacements();
    void printAllPlayerRatings(bool descending = true);

    vector<Player*> getAllStarTeam(int count = 5);


};

#endif //CSMANAGER_TOURNAMENT_H

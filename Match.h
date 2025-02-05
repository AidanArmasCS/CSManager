//
// Created by Main on 1/29/2025.
//

#ifndef MATCH_H
#define MATCH_H

#include <vector>
#include <string>
#include <iostream>
#include <Team.h>

using namespace std;



class Match {
private:
    Team* team1;
    Team* team2;
    int team1score;
    int team2score;
    string winner;
    int result;

public:
    Match(Team* team1Name, Team* team2Name);
    void simulateMatch();
    void displayResult() const;
    string getWinner() const;
    void resetMatch();

    void simulateMatchStats();
    void simulateRounds();

};



#endif //MATCH_H

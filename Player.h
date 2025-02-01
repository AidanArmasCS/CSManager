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


public:

    Player(const string &playerName, const string &playerRole, const string &playerNationality);
    ~Player();

    string getName() const;
    string getRole() const;
    string getNationality() const;

    void displayPlayer() const;


};



#endif //PLAYER_H

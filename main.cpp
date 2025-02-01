#include <iostream>
#include "Team.h"

using namespace std;

int main() {
    Team team1("Uptime", "USA");
    Player player1("JJboyson","Entry","Poland");
    Player player2("Godrod15","Support","Poland");
    Player player3("Cav","IGL","Poland");
    Player player4("Ragin","Support","Poland");
    Player player5("Kappa","AWP","Poland");

    team1.addPlayer(player1);
    team1.addPlayer(player2);
    team1.addPlayer(player3);
    team1.addPlayer(player4);
    team1.addPlayer(player5);


    team1.displayRoster();
    cout << endl;

    team1.recordWin();
    team1.recordWin();
    team1.recordLoss();
    team1.recordLoss();
    team1.recordLoss();
    team1.recordLoss();

    team1.displayRecord();




    return 0;
}
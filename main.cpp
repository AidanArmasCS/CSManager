#include <iostream>
#include "Team.h"

using namespace std;

int main() {
    Team team1("Uptime", "USA");
    Player player1("JJboyson","Entry","Poland");
    Player player2("JJboyson","Entry","Poland");
    Player player3("JJboyson","Entry","Poland");
    Player player4("JJboyson","Entry","Poland");
    Player player5("JJboyson","Entry","Poland");
    Player player6("JJboyson","Entry","Poland");

    player1.displayPlayer();
    team1.addPlayer(player1);
    team1.addPlayer(player2);
    team1.addPlayer(player3);
    team1.addPlayer(player4);
    team1.addPlayer(player5);
    team1.addPlayer(player6);



    return 0;
}
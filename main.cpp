#include <iostream>
#include <vector>
#include <map>
#include <ctime>
#include "Match.h"
#include "Team.h"
#include "Player.h"

using namespace std;

void addTeam(vector<Team> &teams) { // METHOD FOR ADDING TEAMS
    string teamName;
    string teamNationality;
    cout << "Enter the Team Name: ";
    cin.ignore();
    getline(cin, teamName);

    cout << "Enter team Nationality: ";
    getline(cin,teamNationality);

    if (teamName.empty() || teamNationality.empty()) { // EDGE CASE FOR BLANK ENTER
        cout << "Team name and nationality cannot be empty.\n";
        return;
    }

    // Edge case: Prevent duplicate team names
    for (const auto &team : teams) {
        if (team.getName() == teamName) {
            cout << "Error: A team with this name already exists.\n";
            return;
        }
    }

    teams.push_back(Team(teamName, teamNationality));
    cout << "Team " << teamName << " added successfully!\n";
}

void removeTeam(vector<Team> &teams) { //METHOD FOR REMOVING TEAMS
    string teamName;
    cout << "Enter the Team Name: ";
    cin.ignore();
    getline(cin, teamName);

    for (auto it = teams.begin(); it != teams.end(); it++) {
        if (it->getName() == teamName) {
            teams.erase(it);
            cout << "Team " << teamName << " removed successfully.\n";
            return;
            }
        }
    cout << "Error: Team '" << teamName << "' not found.\n";
}

void displayTeams(vector<Team> &teams) {
    cout << "List of Teams" << endl;
    if (teams.empty()) {
        cout << "There are no teams currently added" << endl;
        return;
    }
    cout << "List of Current Teams: " << endl;
    for (auto & team : teams) {
        cout << team.getName() << " | ";
        cout << team.getNationality() << endl;
    }
}

void addPlayertoTeam(vector<Team> &teams) {
    if (teams.empty()) {
        cout << "No teams available. Create a team first.\n";
        return;
    }

    cout << "Enter the name of the team to add a player to: ";
    cin.ignore();
    string teamName;
    getline(cin, teamName);

    for (auto &team: teams) {
        if (team.getName() == teamName) {
            string playerName;
            string role;
            string nationality;
            int aim, movement, gameSense, teamwork, entrying, awp, clutch, consistency;

            cout << "Enter player name: ";
            getline(cin, playerName);

            cout << "Enter the Player Role" << endl;
            cout << "IGL, Entry, Support, AWP: ";
            getline(cin, role);

            cout << "Enter Player Nationality: ";
            getline(cin, nationality);

            //RATINGS
            cout << "Enter Aim (0-100): ";
            while (!(cin >> aim) || aim < 0 || aim > 100) {
                cout << "Invalid input. Enter a number between 0-100: ";
                cin.clear();
                cin.ignore(10000, '\n');
            }

            cout << "Enter Movement (0-100): ";
            while (!(cin >> movement) || movement < 0 || movement > 100) {
                cout << "Invalid input. Enter a number between 0-100: ";
                cin.clear();
                cin.ignore(10000, '\n');
            }

            cout << "Enter Game Sense (0-100): ";
            while (!(cin >> gameSense) || gameSense < 0 || gameSense > 100) {
                cout << "Invalid input. Enter a number between 0-100: ";
                cin.clear();
                cin.ignore(10000, '\n');
            }

            cout << "Enter Teamwork (0-100): ";
            while (!(cin >> teamwork) || teamwork < 0 || teamwork > 100) {
                cout << "Invalid input. Enter a number between 0-100: ";
                cin.clear();
                cin.ignore(10000, '\n');
            }

            cout << "Enter Entrying (0-100): ";
            while (!(cin >> entrying) || entrying < 0 || entrying > 100) {
                cout << "Invalid input. Enter a number between 0-100: ";
                cin.clear();
                cin.ignore(10000, '\n');
            }

            cout << "Enter AWP Skill (0-100): ";
            while (!(cin >> awp) || awp < 0 || awp > 100) {
                cout << "Invalid input. Enter a number between 0-100: ";
                cin.clear();
                cin.ignore(10000, '\n');
            }

            cout << "Enter Clutch Ability (0-100): ";
            while (!(cin >> clutch) || clutch < 0 || clutch > 100) {
                cout << "Invalid input. Enter a number between 0-100: ";
                cin.clear();
                cin.ignore(10000, '\n');
            }

            cout << "Enter Consistency (0-100): ";
            while (!(cin >> consistency) || consistency < 0 || consistency > 100) {
                cout << "Invalid input. Enter a number between 0-100: ";
                cin.clear();
                cin.ignore(10000, '\n');
            }

            // Clear leftover newline after last cin input
            cin.ignore();

            if (playerName.empty() || role.empty() || nationality.empty()) {
                cout << "Error: Player details cannot be empty.\n";
                return;
            }

            Player newPlayer(playerName, role, nationality,
                             aim, movement, gameSense, teamwork,
                             entrying, awp, clutch, consistency);
            team.addPlayer(newPlayer);
            cout << playerName << " added successfully to " << teamName << "!\n";
            return;
        }
    }
    cout << "Error: Team '" << teamName << "' not found.\n";
}

void removePlayerTeam(vector<Team> &teams) {
    if (teams.empty()) {
        cout << "No teams available. Create a team first.\n";
        return;
    }

    cout << "Enter the name of the team to remove a player to: ";
    cin.ignore();
    string teamName;
    getline(cin, teamName);

    cout << "Enter the player you would like to remove: ";
    string playerName;
    getline(cin, playerName);
    cout << endl;

    for (auto &team: teams) {
        if (team.getName() == teamName) {
            team.removePlayer(playerName);
            return;
        }
    }
    cout << "Team has not been found, try again." << endl;
}

void displayTeamRoster(vector<Team> &teams) {
    if (teams.empty()) {
        cout << "No teams available. Create a team first.\n";
        return;
    }

    cout << "Enter the name of the team's roster you would like to view: ";
    cin.ignore();
    string teamName;
    getline(cin, teamName);
    cout << endl;

    for (auto &team: teams) {
        if (team.getName() == teamName) {
            team.displayRoster();
            return;
        }
    }
    cout << "Team Entered Does not Exist Please try again.\n";
}

void simulateMatch(vector<Team> &teams) {
    if (teams.empty()) {
        cout << "No teams available. Create a team first.\n";
        return;
    }

    cout << "Enter the name of the teams you want to play: ";
    cin.ignore();
    string userName1;
    string userName2;
    Team *teamName1;
    Team *teamName2;
    cin >> userName1;
    cin >> userName2;
    cout << endl;
    int one = 0;
    int two = 0;

    for (auto &team: teams) {
        if (team.getName() == userName1) {
            teamName1 = &team;
            one++;
        }
        else if (team.getName() == userName2) {
            teamName2 = &team;
            two++;
        }
    }

    if (one != 1 || two != 1) {
       cout << "One of the two teams isn't an active team";
        return;
    }

    Match sim(teamName1, teamName2);
    sim.simulateMatch();
}

void displayTeamRecord(vector<Team> &teams) {
    if (teams.empty()) {
        cout << "No teams available. Create a team first.\n";
        return;
    }

    cout << "Enter the name of the team's record you would like to view: ";
    cin.ignore();
    string teamName;
    getline(cin, teamName);
    cout << endl;

    for (auto &team: teams) {
        if (team.getName() == teamName) {
            team.displayRecord();
            return;
        }
    }
}




int main() {
    srand(time(0)); // RANDOM EVERY TIME

    vector<Team> teams; // STORE ALL TEAMS
    vector<Match> matches; // STORE MATCH RESULTS

    Team team1("FaZe", "International");

    team1.addPlayer(Player("Frozen", "Rifler", "Slovakia", 93, 87, 91, 87, 90, 60, 80, 90));
    team1.addPlayer(Player("ropz", "Lurker", "Estonia", 96, 88, 95, 90, 75, 50, 77, 85));
    team1.addPlayer(Player("broky", "AWP", "Latvia", 90, 84, 85, 75, 65, 95, 75, 85));
    team1.addPlayer(Player("rain", "Rifler", "Norway", 87, 85, 80, 82, 98, 50, 60, 75));
    team1.addPlayer(Player("karrigan", "IGL", "Denmark", 73, 78, 95, 99, 50, 30, 40, 75));

    Team team2("Navi", "CIS");

    team2.addPlayer(Player("b1t", "Rifler", "Ukraine", 95, 88, 90, 85, 92, 50, 80, 88));
    team2.addPlayer(Player("jL", "Rifler", "Lithuania", 90, 90, 90, 90, 95, 50, 90, 90));
    team2.addPlayer(Player("w0nderful", "AWP", "Ukraine", 85, 82, 79, 80, 64, 87, 70, 80));
    team2.addPlayer(Player("iM", "Rifler", "Romania", 86, 85, 80, 78, 95, 50, 70, 75));
    team2.addPlayer(Player("AleksiB", "IGL", "Finland", 75, 80, 83, 90, 50, 40, 45, 70));


    teams.push_back(team1);
    teams.push_back(team2);

    Match match(&team1, &team2);

    cout << "Faze Overall: " << team1.getTeamOverallRating() << endl;
    cout << "Navi Overall: " << team2.getTeamOverallRating() << endl;

    match.simulateMatch();
    return 0;
}

   /* while (true) {
        cout << "\n===== CS2 Manager =====\n";
        cout << "1. Add Team\n";
        cout << "2. Remove Team\n";
        cout << "3. Display Teams\n";
        cout << "4. Add Player to Team\n";
        cout << "5. Remove Player from Team\n";
        cout << "6. Display Team Roster\n";
        cout << "7. Simulate Match\n";
        cout << "8. Display Team Record\n";
        cout << "0. Exit\n";
        cout << "Enter your choice:  ";

        int choice;
        cin >> choice;

        switch (choice) {
            case 1:
                addTeam(teams);
                break;
            case 2:
                removeTeam(teams);
                break;
            case 3:
                displayTeams(teams);
                break;
            case 4:
                addPlayertoTeam(teams);
                break;
            case 5:
                removePlayerTeam(teams);
                break;
            case 6:
                displayTeamRoster(teams);
                break;
            case 7:
                simulateMatch(teams);
                break;
            case 8:
                displayTeamRecord(teams);
                break;
            case 0:
                return 0;
        }
    }
} */
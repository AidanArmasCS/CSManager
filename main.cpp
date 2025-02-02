#include <iostream>
#include <vector>
#include "Team.h"
#include "Player.h"
#include "Match.h"

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

            cout << "Enter player name: ";
            getline(cin, playerName);

            cout << "Enter the Player Role" << endl;
            cout << "IGL, Entry, Support, AWP: ";
            getline(cin, role);

            cout << "Enter Player Nationality: ";
            getline(cin, nationality);

            if (playerName.empty() || role.empty() || nationality.empty()) {
                cout << "Error: Player details cannot be empty.\n";
                return;
            }

            Player newPlayer(playerName, role, nationality);
            team.addPlayer(newPlayer);
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

    while (true) {
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
        cout << "Enter your choice: ";

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
}
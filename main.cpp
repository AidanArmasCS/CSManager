#include <iostream>
#include <vector>
#include <map>
#include <ctime>
#include <cmath>
#include "Match.h"
#include "Team.h"
#include "Player.h"
#include "Tournament.h"

using namespace std;

void addTeam(vector<Team> &teams) { // METHOD FOR ADDING TEAMS
    string teamName;
    string teamNationality;
    int ranking = 0;
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

    teams.emplace_back(teamName, teamNationality, ranking++);
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
            PlayerStyle s;
            vector<string> assignedTraits;

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
                             entrying, awp, clutch, consistency,
                             s, assignedTraits );
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

  /*  const int NUM_SIMULATIONS = 10; // Number of simulations

    int naviWins = 0;
    int spiritWins = 0;
    int spiritLosses = 0;
    int naviLosses = 0;

    // **Cumulative player stats**
    // **Cumulative player stats including HLTV Rating & ADR**
    struct PlayerStats {
        int totalKills = 0;
        int totalDeaths = 0;
        int totalAssists = 0;
        double totalHLTV = 0.0;
        double totalADR = 0.0;
    };

    map<string, PlayerStats> playerStatsMap; // Store cumulative stats for each player

    for (int sim = 0; sim < NUM_SIMULATIONS; sim++) {
        // **Recreate teams every simulation to reset data**
        Team team1("FaZe", "International");
        team1.addPlayer(Player("Frozen", "Rifler", "Slovakia", 93, 87, 91, 87, 90, 60, 80, 90,PlayerStyle::Balanced,{"Leader", "Selfish"}));
        team1.addPlayer(Player("ropz", "Lurker", "Estonia", 96, 88, 95, 90, 75, 50, 77, 85,PlayerStyle::Passive,{"None"}));
        team1.addPlayer(Player("broky", "AWP", "Latvia", 90, 84, 85, 75, 65, 95, 75, 85,PlayerStyle::Passive,{"None"}));
        team1.addPlayer(Player("rain", "Support", "Norway", 87, 85, 80, 82, 98, 50, 60, 75,PlayerStyle::Passive, {"None"}));
        team1.addPlayer(Player("karrigan", "IGL", "Denmark", 73, 78, 95, 99, 50, 30, 40, 75,PlayerStyle::Passive, {"None"}));

        Team team2("Navi", "CIS");
        team2.addPlayer(Player("b1t", "Rifler", "Ukraine", 95, 88, 90, 85, 92, 50, 80, 88, PlayerStyle::Passive, {"Leader", "Selfish"}));
        team2.addPlayer(Player("jL", "Rifler", "Lithuania", 92, 90, 90, 90, 95, 50, 90, 90,PlayerStyle::Passive, {"Leader"}));
        team2.addPlayer(Player("w0nderful", "AWP", "Ukraine", 85, 82, 79, 80, 64, 87, 70, 80,PlayerStyle::Passive, {"Leader"}));
        team2.addPlayer(Player("iM", "Support", "Romania", 86, 85, 80, 78, 95, 50, 70, 75,PlayerStyle::Passive, {"Leader"}));
        team2.addPlayer(Player("AleksiB", "IGL", "Finland", 75, 80, 83, 90, 50, 40, 45, 70,PlayerStyle::Balanced, {"Leader"}));

        Team team3("Spirit Academy", "CIS");
        team3.addPlayer(Player("Donk", "Rifler", "Russia", 80, 75, 78, 74, 82, 40, 65, 70,PlayerStyle::Aggressive, {"None"}));
        team3.addPlayer(Player("Latt1kk", "IGL", "Russia", 78, 74, 76, 70, 79, 45, 60, 72,PlayerStyle::Aggressive, {"None"}));
        team3.addPlayer(Player("Zont1x", "Support", "Russia", 77, 72, 75, 72, 81, 38, 55, 71,PlayerStyle::Aggressive, {"None"}));
        team3.addPlayer(Player("S1ren", "Entry", "Russia", 76, 70, 74, 71, 85, 30, 50, 68,PlayerStyle::Aggressive, {"None"}));
        team3.addPlayer(Player("ArtFr0st", "AWP", "Russia", 75, 69, 73, 68, 70, 85, 60, 65,PlayerStyle::Aggressive, {"None"}));

        Match match(&team2, &team3);
        match.simulateMatch(); // Run match simulation

        cout << "Navi Overall: " << team2.getTeamOverallRating() << " Navi Chemistry: " << team2.getTeamChemistry() << endl;
        cout << "Spirit Overall: " << team3.getTeamOverallRating() << " Spirit Chemistry: " << team3.getTeamChemistry() << endl;

        if (match.getWinner() == "Navi") {
            naviWins++;
            spiritLosses++;
        } else {
            naviLosses++;
            spiritWins++;
        }

        // **Track match stats per player including HLTV Rating & ADR**
        for (Team *team: {&team3, &team2}) {
            for (Player &player: team->getRoster()) {
                string playerName = player.getName();
                double rating = match.simulateMatchRatings(&player);
                double adr = player.getMatchADR(); // Assuming ADR is stored per player

                playerStatsMap[playerName].totalKills += player.getMatchKills();
                playerStatsMap[playerName].totalDeaths += player.getMatchDeaths();
                playerStatsMap[playerName].totalAssists += player.getMatchAssists();
                playerStatsMap[playerName].totalHLTV += rating;
                playerStatsMap[playerName].totalADR += adr;
            }
        }
    }

    // **Display Final Stats After 1000 Simulations**
    cout << "\n===== Player Performance Over " << NUM_SIMULATIONS << " Simulations =====\n";
    for (const auto &entry: playerStatsMap) {
        const string &playerName = entry.first;
        const PlayerStats &stats = entry.second;

        // Compute averages
        double avgKills = (double) stats.totalKills / NUM_SIMULATIONS;
        double avgDeaths = (double) stats.totalDeaths / NUM_SIMULATIONS;
        double avgAssists = (double) stats.totalAssists / NUM_SIMULATIONS;
        double avgHLTV = stats.totalHLTV / NUM_SIMULATIONS;
        double avgADR = stats.totalADR / NUM_SIMULATIONS;
        double kdRatio = avgKills / avgDeaths;
        double roundedKD = round(kdRatio * 100.0) / 100.0;
        double roundHLTV = round(avgHLTV * 100.0) / 100.0;
        double roundedADR = round(avgADR * 100.0) / 100.0;

        cout << fixed << setprecision(2);
        cout << playerName << " - Avg Kills: " << avgKills
             << " | Avg Deaths: " << avgDeaths
             << " | Avg Assists: " << avgAssists
             << " | K/D: " << roundedKD
             << " | Avg HLTV: " << roundHLTV
             << " | Avg ADR: " << roundedADR << endl;

    }

    // Display final win-loss records
    cout << "\n===== Final Win/Loss Records After " << NUM_SIMULATIONS << " Simulations =====" << endl;
    cout << "Navi Record: " << naviWins << " Wins | " << naviLosses << " Losses | Win %: "
         << (static_cast<double>(naviWins) / NUM_SIMULATIONS) * 100.0 << "%" << endl;
    cout << "Spirit Record: " << spiritWins << " Wins | " << spiritLosses << " Losses | Win %: "
         << (static_cast<double>(spiritWins) / NUM_SIMULATIONS) * 100.0 << "%" << endl; */

  /*  Player player1("mopoz", "Rifler", "Spain", 60, 61, 60, 65, 35, 30, 40, 60, PlayerStyle::Aggressive,{});
    Player player2("alex", "IGL", "Spain", 50, 55, 65, 73, 60, 30, 44, 57, PlayerStyle::Passive,{});
    Player player3("sausol", "Rifler", "Spain", 58, 62, 61, 62, 33, 30, 44, 62, PlayerStyle::Balanced, {});
    Player player4("stadodo", "AWP", "Portugal", 44, 57, 60, 60, 30, 65, 50, 56, PlayerStyle::Passive, {});
    Player player5("dav1g", "Entry", "Spain", 47, 53, 58, 59, 62, 30, 30, 55, PlayerStyle::Aggressive,{});


    cout << player1.getName() << " Overall: " <<player1.getAdjustedOverallRating() << endl;
    cout << player2.getName() << " Overall: " <<player2.getAdjustedOverallRating() << endl;
    cout << player3.getName() << " Overall: " <<player3.getAdjustedOverallRating() << endl;
    cout << player4.getName() << " Overall: " <<player4.getAdjustedOverallRating() << endl;
    cout << player5.getName() << " Overall: " <<player5.getAdjustedOverallRating(); */


    Player donk("donk", "Rifler", "Russia", 99, 99, 97, 92, 96, 50, 96, 99, PlayerStyle::Aggressive, {"Aim Demon", "Quick Learner"});
    Player sh1ro("sh1ro", "AWP", "Russia", 90, 86, 88, 87, 80, 95, 87, 88, PlayerStyle::Passive, {"Team Player"});
    Player chopper("chopper", "IGL", "Russia", 77, 72, 86, 93, 68, 50, 65, 73, PlayerStyle::Balanced, {"Inconsistent"});
    Player zont1x("zont1x", "Support", "Russia", 84, 80, 78, 81, 70, 50, 70, 80, PlayerStyle::Balanced, {"Team Player"});
    Player magixx("magixx", "Entry", "Russia", 80, 73, 74, 80, 85, 50, 75, 80, PlayerStyle::Aggressive, {"Inconsistent"});
    Team spirit("Spirit", "Russia", 1);
    spirit.addPlayer(donk);
    spirit.addPlayer(sh1ro);
    spirit.addPlayer(chopper);
    spirit.addPlayer(zont1x);
    spirit.addPlayer(magixx);

    Player zywoo("Zyw0o", "AWP", "France", 99, 99, 96, 87, 88, 97, 92, 99, PlayerStyle::Balanced, {"Aim Demon", "Clutch Master"});
    Player flameZ("flameZ", "Entry", "Israel", 93, 89, 88, 87, 96, 50, 87, 88, PlayerStyle::Aggressive, {"Disciplined"});
    Player ropz("ropz", "Lurker", "Estonia", 90, 85, 90, 85, 60, 50, 85, 85, PlayerStyle::Passive, {"Master Communicator", "Clutch Master", "Team Player"});
    Player apEX("apEX", "IGL", "France", 76, 70, 92, 95, 75, 50, 58, 76, PlayerStyle::Aggressive, {"Leader", "Tilter", "Choker"});
    Player mezii("mezii", "Support", "United Kingdom", 82, 75, 76, 78, 70, 50, 70, 80, PlayerStyle::Aggressive, {"Versatile"});
    Team vitality("Vitality", "International", 2);
    vitality.addPlayer(zywoo);
    vitality.addPlayer(flameZ);
    vitality.addPlayer(ropz);
    vitality.addPlayer(apEX);
    vitality.addPlayer(mezii);

    Player jL("jL", "Rifler", "Lithuania", 92, 92, 91, 93, 95, 50, 90, 90, PlayerStyle::Aggressive, {"Tilter", "Team Player"});
    Player b1t("b1t", "Rifler", "Ukraine", 98, 86, 90, 85, 83, 50, 80, 90, PlayerStyle::Aggressive,{"Aim Demon", "Disciplined"});
    Player aleksib("Aleksib", "IGL", "Finland", 73, 80, 93, 99, 60, 50, 50, 75, PlayerStyle::Aggressive,{"Leader", "Tactical Genius"});
    Player w0nderful("w0nderful", "AWP", "Ukraine", 85, 85, 81, 80, 64, 90, 70, 80, PlayerStyle::Balanced,{"Inconsistent", "Choker", "Versatile"});
    Player iM("iM", "Support", "Romania", 86, 85, 82, 82, 95, 50, 70, 80, PlayerStyle::Passive,{"Inconsistent", "Supportive", "Team Player"});
    Team navi("Navi", "International", 3);
    navi.addPlayer(jL);
    navi.addPlayer(b1t);
    navi.addPlayer(aleksib);
    navi.addPlayer(w0nderful);
    navi.addPlayer(iM);

    Player xantares("XANTARES", "Rifler", "Turkey", 94, 86, 85, 84, 90, 50, 72, 88, PlayerStyle::Aggressive,{"Aim Demon"});
    Player woxic("woxic", "AWP", "Turkey", 83, 77, 75, 80, 66, 85, 74, 80, PlayerStyle::Aggressive,{"Tilter"});
    Player maj3r("MAJ3R", "IGL", "Turkey", 68, 69, 81, 86, 65, 50, 72, 68, PlayerStyle::Aggressive,{"Master Communicator"});
    Player wicadia("Wicadia", "Rifler", "Turkey", 85, 80, 77, 76, 70, 50, 66, 78, PlayerStyle::Aggressive,{"Quick Learner"});
    Player jottAAA("jottAAA", "Entry", "Turkey", 80, 75, 72, 75, 80, 50, 73, 74, PlayerStyle::Balanced,{"Supportive", "Inconsistent"});
    Team EternalFire("Eternal Fire", "Turkey", 4);
    EternalFire.addPlayer(xantares);
    EternalFire.addPlayer(woxic);
    EternalFire.addPlayer(maj3r);
    EternalFire.addPlayer(wicadia);
    EternalFire.addPlayer(jottAAA);

    Player m0NESY("m0NESY", "AWP", "Russia", 99, 97, 95, 86, 89, 99, 95, 99, PlayerStyle::Balanced,{"Aim Demon"});
    Player malbsMd("malbsMd", "Entry", "Argentina", 93, 91, 86, 83, 93, 50, 63, 87, PlayerStyle::Aggressive,{"Quick Learner", "Team Player"});
    Player snax("Snax", "IGL", "Poland", 60, 65, 90, 90, 64, 50, 72, 79, PlayerStyle::Balanced,{"Leader", "Clutch Master", "Inconsistent", "Choker"});
    Player heavygod("HeavyGod", "Lurker", "Israel", 86, 80, 82, 82, 70, 50, 84, 80, PlayerStyle::Balanced,{"Disciplined"});
    Player hunter("huNter-", "Rifler", "Bosnia", 80, 78, 80, 80, 73, 50, 68, 76, PlayerStyle::Aggressive,{"Inconsistent"});
    Team g2("G2", "International", 5);
    g2.addPlayer(m0NESY);
    g2.addPlayer(malbsMd);
    g2.addPlayer(snax);
    g2.addPlayer(heavygod);
    g2.addPlayer(hunter);

    Player broky("broky", "AWP", "Latvia", 91, 87, 85, 83, 65, 95, 82, 85, PlayerStyle::Passive,{"Clutch Master"});
    Player frozen("frozen", "Rifler", "Slovakia", 93, 87, 91, 87, 90, 50, 80, 90, PlayerStyle::Balanced,{"Team Player"});
    Player karrigan("karrigan", "IGL", "Denmark", 71, 75, 99, 99, 65, 50, 60, 78, PlayerStyle::Aggressive,{"Tactical Genius", "Leader", "Master Communicator"});
    Player elige("EliGE", "Rifler", "USA", 92, 85, 86, 82, 75, 50, 60, 85, PlayerStyle::Aggressive,{"Quick Learner", "Toxic", "Stubborn"});
    Player rain("rain", "Support", "Norway", 87, 85, 84, 80, 74, 50, 60, 83, PlayerStyle::Passive,{"Team Player", "Supportive"});
    Team faze("FaZe", "International", 6);
    faze.addPlayer(broky);
    faze.addPlayer(frozen);
    faze.addPlayer(karrigan);
    faze.addPlayer(elige);
    faze.addPlayer(rain);

    Player senzu("Seznu", "Rifler", "Mongolia", 92, 84, 83, 85, 72, 50, 68, 86, PlayerStyle::Aggressive, {"Aim Demon"});
    Player nineten("910", "AWP", "Mongolia", 85, 80, 73, 83, 63, 88, 70, 83, PlayerStyle::Balanced, {"None"});
    Player bLitz("bLitz", "IGL", "Mongolia", 80, 73, 80, 86, 66, 50, 65, 80, PlayerStyle::Balanced, {"Leader"});
    Player mzhino("mzhino", "Rifler", "Mongolia", 81, 82, 75, 84, 78, 50, 70, 78, PlayerStyle::Aggressive,{"Quick Learner", "Team Player"});
    Player techno4k("Techno4k", "Lurker", "Mongolia", 81, 80, 81, 82, 65, 50, 75, 78, PlayerStyle::Passive,{"Disciplined"});
    Team mongolz("The MongolZ", "Mongolia", 7);
    mongolz.addPlayer(senzu);
    mongolz.addPlayer(nineten);
    mongolz.addPlayer(bLitz);
    mongolz.addPlayer(mzhino);
    mongolz.addPlayer(techno4k);

    Player spinx("Spinx", "Rifler", "Israel", 96, 89, 87, 82, 85, 50, 83, 89, PlayerStyle::Aggressive,{"Aim Demon", "Versatile"});
    Player xertioN("xertioN", "Entry", "Israel", 89, 86, 83, 81, 93, 50, 73, 88, PlayerStyle::Aggressive,{"Supportive"});
    Player jimpphat("Jimpphat", "Rifler", "Finland", 87, 82, 81, 80, 76, 50, 85, 85, PlayerStyle::Aggressive,{"Supportive"});
    Player torzsi("torzsi", "AWP", "Hungary", 82, 75, 78, 80, 70, 90, 80, 84, PlayerStyle::Balanced,{"Clutch Master", "Disciplined"});
    Player brollan("Brollan", "IGL", "Sweden", 77, 70, 81, 84, 75, 50, 60, 76, PlayerStyle::Balanced,{"Versatile"});
    Team mouz("MOUZ", "International", 8);
    mouz.addPlayer(spinx);
    mouz.addPlayer(xertioN);
    mouz.addPlayer(torzsi);
    mouz.addPlayer(jimpphat);
    mouz.addPlayer(brollan);

    Player stavn("stavn", "Rifler", "Denmark", 85, 80, 81, 80, 76, 50, 72, 85, PlayerStyle::Aggressive,{"Supportive"});
    Player device("device", "AWP", "Denmark", 82, 81, 84, 76, 62, 84, 70, 78, PlayerStyle::Passive,{"Lazy", "Disciplined"});
    Player staehr("Staehr", "Support", "Denmark", 79, 83, 86, 80, 72, 50, 69, 83, PlayerStyle::Passive, {"Versatile"});
    Player jabbi("jabbi", "Rifler", "Denmark", 84, 79, 82, 80, 72, 50, 65, 82, PlayerStyle::Balanced, {});
    Player cadian("cadiaN", "IGL", "Denmark", 74, 75, 78, 82, 65, 65, 78, 76, PlayerStyle::Aggressive,{"Toxic", "Tilter"});
    Team astralis("Astralis", "Denmark", 9);
    astralis.addPlayer(stavn);
    astralis.addPlayer(device);
    astralis.addPlayer(staehr);
    astralis.addPlayer(jabbi);
    astralis.addPlayer(cadian);

    Player niko("Niko", "Rifler", "Bosnia", 97, 95, 96, 97, 96, 50, 90, 99, PlayerStyle::Aggressive,{"Aim Demon", "Leader"});
    Player degster("degster", "AWP", "Russia", 82, 80, 84, 83, 64, 84, 60, 82, PlayerStyle::Balanced, {"Selfish"});
    Player teses("TeSeS", "Entry", "Denmark", 86, 80, 76, 80, 85, 50, 71, 86, PlayerStyle::Passive,{"Supportive", "Team Player"});
    Player magisk("Magisk", "Support", "Denmark", 80, 82, 86, 85, 60, 50, 71, 84, PlayerStyle::Passive, {"Disciplined", "Team Player"});
    Player kyxsan("kyxsan", "IGL", "Macedonia", 60, 63, 80, 84, 76, 50, 68, 82, PlayerStyle::Aggressive,{"Supportive", "Team Player"});
    Team falcons("Falcons", "International", 10);
    falcons.addPlayer(niko);
    falcons.addPlayer(degster);
    falcons.addPlayer(teses);
    falcons.addPlayer(magisk);
    falcons.addPlayer(kyxsan);

    Player electroNic("electroNic", "IGL", "Russia", 83, 76, 86, 87, 76, 50, 60, 85, PlayerStyle::Balanced, {"Leader"});
    Player icy("ICY", "AWP", "Kazkhstan", 81, 82, 80, 81, 62, 84, 83, 82, PlayerStyle::Passive,{"Clutch Master"});
    Player fl1t("FL1T", "Lurker", "Russia", 85, 78, 84, 79, 70, 50, 72, 83, PlayerStyle::Aggressive,{"Versatile"});
    Player fl4mus("FL4MUS", "Rifler", "Russia", 85, 80, 80, 78, 70, 50, 67, 83, PlayerStyle::Aggressive,{"Versatile"});
    Player fame("fame", "Rifler", "Russia", 81, 75, 80, 84, 75, 50, 70, 76, PlayerStyle::Passive, {"Supportive", "Team Player"});
    Team virtuspro("Virtus Pro", "Russia", 11);
    virtuspro.addPlayer(electroNic);
    virtuspro.addPlayer(icy);
    virtuspro.addPlayer(fl1t);
    virtuspro.addPlayer(fl4mus);
    virtuspro.addPlayer(fame);

    Player nertz("NertZ", "Rifler", "Israel", 91, 83, 82, 81, 68, 50, 68, 86, PlayerStyle::Aggressive,{"Team Player", "Quick Learner"});
    Player twistzz("Twistzz", "IGL", "Canada", 85, 76, 81, 84, 70, 50, 60, 81, PlayerStyle::Passive,{"Large Ego"});
    Player naf("NAF", "Rifler", "Canada", 82, 85, 86, 82, 67, 50, 65, 83, PlayerStyle::Balanced, {"Versatile"});
    Player ultimate("ultimate", "AWP", "Poland", 76, 78, 78, 78, 68, 83, 70, 80, PlayerStyle::Passive, {"Clutch Master"});
    Player jks("jks", "Support", "Australia", 78, 75, 85, 83, 69, 50, 67, 82, PlayerStyle::Aggressive,{"Disciplined", "Team Player"});
    Team liquid("Liquid", "International", 12);
    liquid.addPlayer(nertz);
    liquid.addPlayer(twistzz);
    liquid.addPlayer(naf);
    liquid.addPlayer(ultimate);
    liquid.addPlayer(jks);

    Player biguzera("biguzera", "IGL", "Brazil", 80, 74, 80, 87, 65, 50, 65, 81, PlayerStyle::Aggressive,{"Leader"});
    Player nqz("nqz", "AWP", "Brazil", 74, 80, 78, 84, 59, 86, 68, 83, PlayerStyle::Balanced,{"Clutch Master"});
    Player dav1deuS("dav1deuS", "Rifler", "Chile", 84, 76, 78, 80, 67, 50, 64, 80, PlayerStyle::Passive, {"Aim Demon", "Inconsistent"});
    Player kauez("kauez", "Lurker", "Brazil", 76, 80, 83, 82, 65, 50, 66, 79, PlayerStyle::Passive, {"Supportive"});
    Player snow("snow", "Support", "Brazil", 78, 75, 77, 84, 64, 50, 72, 80, PlayerStyle::Aggressive,{"Disciplined"});
    Team paiN("paiN", "Brazil", 13);
    paiN.addPlayer(biguzera);
    paiN.addPlayer(nqz);
    paiN.addPlayer(dav1deuS);
    paiN.addPlayer(kauez);
    paiN.addPlayer(snow);

    Player rez("REZ", "Rifler", "Sweden", 86, 84, 84, 82, 86, 50, 66, 85, PlayerStyle::Aggressive,{"Leader"});
    Player pr("PR", "Rifler", "Czech Republic", 90, 84, 80, 78, 66, 50, 65, 83, PlayerStyle::Balanced,{"Clutch Master"});
    Player ztr("ztr", "IGL", "Sweden", 75, 72, 82, 83, 70, 50, 70, 77, PlayerStyle::Passive, {"Aim Demon", "Inconsistent"});
    Player sl3nd("sl3nd", "AWP", "Hungary", 72, 76, 80, 82, 60, 82, 75, 83, PlayerStyle::Passive, {"Supportive"});
    Player tauson("Tauson", "Support", "Denmark", 76, 75, 78, 82, 80, 50, 58, 79, PlayerStyle::Aggressive,{"Disciplined"});
    Team gamerlegion("GamerLegion", "International", 14);
    gamerlegion.addPlayer(rez);
    gamerlegion.addPlayer(pr);
    gamerlegion.addPlayer(ztr);
    gamerlegion.addPlayer(sl3nd);
    gamerlegion.addPlayer(tauson);

    Player kscerato("KSCERATO", "Rifler", "Brazil", 94, 87, 86, 84, 50, 50, 90, 87, PlayerStyle::Aggressive,{"Clutch Master"});
    Player fallen("Fallen", "IGL", "Brazil", 65, 69, 84, 92, 63, 80, 65, 80, PlayerStyle::Balanced,{"Leader", "Master Communicator"});
    Player yuurih("yuurih", "Rifler", "Brazil", 87, 80, 82, 84, 58, 50, 68, 83, PlayerStyle::Passive, {"Versatile"});
    Player skullz("skullz", "Support", "Brazil", 74, 77, 78, 83, 65, 50, 66, 80, PlayerStyle::Passive, {"Disciplined"});
    Player chelo("chelo", "Entry", "Brazil", 72, 74, 73, 77, 83, 50, 60, 75, PlayerStyle::Aggressive,{"Team Player"});
    Team furia("Furia", "Brazil", 15);
    furia.addPlayer(kscerato);
    furia.addPlayer(fallen);
    furia.addPlayer(yuurih);
    furia.addPlayer(skullz);
    furia.addPlayer(chelo);

    Player Maka("Maka", "IGL", "France", 78, 78, 78, 80, 67, 80, 70, 76, PlayerStyle::Passive, {"Leader"});
    Player lucky("Lucky", "Entry", "France", 82, 80, 80, 80, 80, 50, 72, 83, PlayerStyle::Aggressive,{"Versatile"});
    Player bodyy("bodyy", "Rifler", "France", 83, 78, 83, 82, 84, 50, 50, 81, PlayerStyle::Balanced,{"Supportive"});
    Player ex3rcice("Ex3rcice", "Support", "France", 79, 75, 78, 78, 69, 50, 68, 78, PlayerStyle::Passive, {"Team Player"});
    Player graviti("Graviti", "Entry", "France", 76, 74, 75, 79, 81, 50, 60, 75, PlayerStyle::Aggressive,{"Supportive"});
    Team threeDMAX("3DMAX", "France", 16);
    threeDMAX.addPlayer(Maka);
    threeDMAX.addPlayer(lucky);
    threeDMAX.addPlayer(bodyy);
    threeDMAX.addPlayer(ex3rcice);
    threeDMAX.addPlayer(graviti);


    vector<Team*> allTeams = {
            &spirit, &vitality, &navi, &EternalFire, &g2,
            &faze, &mongolz, &mouz, &astralis, &falcons,
            &virtuspro, &liquid, &paiN, &gamerlegion, &furia, &threeDMAX
    };

    Tournament major("bracket", "Cologne", "IEM Cologne 2025", "S-Tier", 1000000, allTeams, "7 days", "July 10, 2025");
    major.simulateTournament();
    major.getMVP();








    return 0;
}
// Tournament Test v4 — Using Tournament Engine Class
// Display logic in main, simulation logic in Tournament
// Tests both Groups and Bracket formats

#include "core/Player.h"
#include "core/Team.h"
#include "core/MatchResult.h"
#include "engine/Tournament.h"
#include "engine/RatingCalculator.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <ctime>

using namespace std;

// ============================================================
//  DISPLAY HELPERS (presentation layer — NOT in Tournament)
// ============================================================

void printMatchScoreboard(const MatchResult& result, const string& label) {
    cout << "\n" << label << endl;
    cout << "  " << result.team1Name << " [" << result.team1Score
         << "] - [" << result.team2Score << "] " << result.team2Name << endl;
    cout << "  Winner: " << result.winnerName << endl;
    cout << string(78, '-') << endl;
    cout << "  " << left << setw(16) << "Player"
         << right << setw(6) << "Kills"
         << setw(8) << "Deaths"
         << setw(9) << "Assists"
         << setw(8) << "ADR"
         << setw(10) << "Rating"
         << setw(8) << "K/D"
         << "  " << "Team" << endl;
    cout << string(78, '-') << endl;

    vector<const PlayerMatchStats*> sorted;
    for (const auto& entry : result.playerStats)
        sorted.push_back(&entry.second);
    sort(sorted.begin(), sorted.end(), [](const PlayerMatchStats* a, const PlayerMatchStats* b) {
        return a->hltvRating > b->hltvRating;
    });

    for (const PlayerMatchStats* s : sorted) {
        double kd = (s->deaths > 0) ? (double)s->kills / s->deaths : (double)s->kills;
        cout << "  " << left << setw(16) << s->playerName
             << right << setw(6) << s->kills
             << setw(8) << s->deaths
             << setw(9) << s->assists
             << setw(8) << fixed << setprecision(1) << s->adr
             << setw(10) << fixed << setprecision(2) << s->hltvRating
             << setw(8) << fixed << setprecision(2) << kd
             << "  " << s->teamName << endl;
    }
}

void printTeamRating(const Team& team) {
    int rating = RatingCalculator::getTeamOverallRating(team);
    int chem = RatingCalculator::getTeamChemistry(team);
    cout << "  " << left << setw(22) << team.getName()
         << "OVR: " << setw(4) << rating
         << "CHEM: " << setw(4) << chem
         << "(Rank #" << team.getRanking() << ")" << endl;
}

void printGroupStandings(const GroupResult& gr) {
    cout << "\n  " << gr.groupName << " Standings:" << endl;
    cout << "  " << string(60, '-') << endl;
    cout << "  " << left << setw(22) << "Team"
         << right << setw(5) << "W"
         << setw(5) << "L"
         << setw(8) << "RW"
         << setw(8) << "RL"
         << setw(8) << "Diff" << endl;
    cout << "  " << string(60, '-') << endl;

    for (size_t i = 0; i < gr.standings.size(); i++) {
        const auto& s = gr.standings[i];
        string marker = (i < 2) ? " [Q]" : "    ";
        cout << "  " << left << setw(22) << (s.team->getName() + marker)
             << right << setw(5) << s.wins
             << setw(5) << s.losses
             << setw(8) << s.roundsWon
             << setw(8) << s.roundsLost
             << setw(8) << s.roundDiff() << endl;
    }
}

void printPlayerRankings(const string& title,
                         const vector<pair<string, PlayerAggregateStats>>& rankings,
                         int limit = 20) {
    cout << "\n" << title << endl;
    cout << "  " << left << setw(16) << "Player"
         << right << setw(5) << "Maps"
         << setw(9) << "Avg K"
         << setw(9) << "Avg D"
         << setw(9) << "Avg A"
         << setw(7) << "K/D"
         << setw(9) << "Rating" << endl;
    cout << "  " << string(64, '-') << endl;

    int count = 0;
    for (const auto& p : rankings) {
        if (count >= limit) break;
        const PlayerAggregateStats& s = p.second;
        cout << "  " << left << setw(16) << p.first
             << right << setw(5) << s.matchesPlayed
             << setw(9) << fixed << setprecision(1) << s.avgKills()
             << setw(9) << s.avgDeaths()
             << setw(9) << s.avgAssists()
             << setw(7) << fixed << setprecision(2) << s.kdRatio()
             << setw(9) << s.avgRating() << endl;
        count++;
    }
}

// ============================================================
//  TEAM BUILDERS (same data as before)
// ============================================================

Team makeSpirit() {
    Team t("Spirit", "Russia", 1);
    t.addPlayer(Player("donk", "Rifler", "Russia", 99, 99, 97, 92, 96, 50, 96, 99, PlayerStyle::Aggressive, {"Aim Demon", "Quick Learner"}));
    t.addPlayer(Player("sh1ro", "AWP", "Russia", 90, 86, 88, 87, 80, 95, 87, 88, PlayerStyle::Passive, {"Team Player"}));
    t.addPlayer(Player("chopper", "IGL", "Russia", 77, 72, 86, 93, 68, 50, 65, 73, PlayerStyle::Balanced, {"Inconsistent"}));
    t.addPlayer(Player("zont1x", "Support", "Russia", 84, 80, 78, 81, 70, 50, 70, 80, PlayerStyle::Balanced, {"Team Player"}));
    t.addPlayer(Player("magixx", "Entry", "Russia", 80, 73, 74, 80, 85, 50, 75, 80, PlayerStyle::Aggressive, {"Inconsistent"}));
    return t;
}

Team makeVitality() {
    Team t("Vitality", "International", 2);
    t.addPlayer(Player("Zyw0o", "AWP", "France", 99, 99, 96, 87, 88, 97, 92, 99, PlayerStyle::Balanced, {"Aim Demon", "Clutch Master"}));
    t.addPlayer(Player("flameZ", "Entry", "Israel", 93, 89, 88, 87, 96, 50, 87, 88, PlayerStyle::Aggressive, {"Disciplined"}));
    t.addPlayer(Player("ropz", "Lurker", "Estonia", 90, 85, 90, 85, 60, 50, 85, 85, PlayerStyle::Passive, {"Master Communicator", "Clutch Master", "Team Player"}));
    t.addPlayer(Player("apEX", "IGL", "France", 76, 70, 92, 95, 75, 50, 58, 76, PlayerStyle::Aggressive, {"Leader", "Tilter", "Choker"}));
    t.addPlayer(Player("mezii", "Support", "United Kingdom", 82, 75, 76, 78, 70, 50, 70, 80, PlayerStyle::Aggressive, {"Versatile"}));
    return t;
}

Team makeFaZe() {
    Team t("FaZe", "International", 6);
    t.addPlayer(Player("broky", "AWP", "Latvia", 91, 87, 85, 83, 65, 95, 82, 85, PlayerStyle::Passive, {"Clutch Master"}));
    t.addPlayer(Player("frozen", "Rifler", "Slovakia", 93, 87, 91, 87, 90, 50, 80, 90, PlayerStyle::Balanced, {"Team Player"}));
    t.addPlayer(Player("karrigan", "IGL", "Denmark", 71, 75, 99, 99, 65, 50, 60, 78, PlayerStyle::Aggressive, {"Tactical Genius", "Leader", "Master Communicator"}));
    t.addPlayer(Player("EliGE", "Rifler", "USA", 92, 85, 86, 82, 75, 50, 60, 85, PlayerStyle::Aggressive, {"Quick Learner", "Toxic", "Stubborn"}));
    t.addPlayer(Player("rain", "Support", "Norway", 87, 85, 84, 80, 74, 50, 60, 83, PlayerStyle::Passive, {"Team Player", "Supportive"}));
    return t;
}

Team makeVirtusProf() {
    Team t("Virtus.pro", "Russia", 11);
    t.addPlayer(Player("electroNic", "IGL", "Russia", 83, 76, 86, 87, 76, 50, 60, 85, PlayerStyle::Balanced, {"Leader"}));
    t.addPlayer(Player("ICY", "AWP", "Kazakhstan", 81, 82, 80, 81, 62, 84, 83, 82, PlayerStyle::Passive, {"Clutch Master"}));
    t.addPlayer(Player("FL1T", "Lurker", "Russia", 85, 78, 84, 79, 70, 50, 72, 83, PlayerStyle::Aggressive, {"Versatile"}));
    t.addPlayer(Player("FL4MUS", "Rifler", "Russia", 85, 80, 80, 78, 70, 50, 67, 83, PlayerStyle::Aggressive, {"Versatile"}));
    t.addPlayer(Player("fame", "Support", "Russia", 81, 75, 80, 84, 75, 50, 70, 76, PlayerStyle::Passive, {"Supportive", "Team Player"}));
    return t;
}

Team makePaiN() {
    Team t("paiN", "Brazil", 13);
    t.addPlayer(Player("biguzera", "IGL", "Brazil", 80, 74, 80, 87, 65, 50, 65, 81, PlayerStyle::Aggressive, {"Leader"}));
    t.addPlayer(Player("nqz", "AWP", "Brazil", 74, 80, 78, 84, 59, 86, 68, 83, PlayerStyle::Balanced, {"Clutch Master"}));
    t.addPlayer(Player("dav1deuS", "Rifler", "Chile", 84, 76, 78, 80, 67, 50, 64, 80, PlayerStyle::Passive, {"Aim Demon", "Inconsistent"}));
    t.addPlayer(Player("kauez", "Lurker", "Brazil", 76, 80, 83, 82, 65, 50, 66, 79, PlayerStyle::Passive, {"Supportive"}));
    t.addPlayer(Player("snow", "Support", "Brazil", 78, 75, 77, 84, 64, 50, 72, 80, PlayerStyle::Aggressive, {"Disciplined"}));
    return t;
}

Team makeBetBoom() {
    Team t("BetBoom", "Russia", 20);
    t.addPlayer(Player("Ax1Le", "Rifler", "Russia", 92, 84, 82, 81, 66, 50, 60, 86, PlayerStyle::Balanced, {"Versatile"}));
    t.addPlayer(Player("Boombl4", "IGL", "Russia", 70, 75, 81, 85, 85, 50, 62, 83, PlayerStyle::Balanced, {"Leader", "Lazy"}));
    t.addPlayer(Player("Magnojez", "Support", "Russia", 85, 79, 80, 82, 63, 50, 61, 78, PlayerStyle::Aggressive, {"Inconsistent"}));
    t.addPlayer(Player("zorte", "AWP", "Russia", 75, 72, 80, 74, 55, 80, 70, 78, PlayerStyle::Aggressive, {}));
    t.addPlayer(Player("S1ren", "Rifler", "Russia", 78, 76, 78, 80, 64, 50, 63, 80, PlayerStyle::Passive, {"Team Player"}));
    return t;
}

Team makeB8() {
    Team t("B8", "Ukraine", 25);
    t.addPlayer(Player("headtr1ck", "AWP", "Ukraine", 82, 75, 76, 77, 62, 77, 79, 75, PlayerStyle::Balanced, {}));
    t.addPlayer(Player("npl", "IGL", "Ukraine", 78, 72, 72, 78, 70, 50, 66, 72, PlayerStyle::Balanced, {"Supportive"}));
    t.addPlayer(Player("alex666", "Rifler", "Ukraine", 81, 72, 70, 76, 68, 50, 60, 73, PlayerStyle::Aggressive, {}));
    t.addPlayer(Player("kensizor", "Rifler", "Ukraine", 80, 75, 74, 72, 69, 50, 57, 74, PlayerStyle::Aggressive, {"Quick Learner"}));
    t.addPlayer(Player("esenthial", "Entry", "Ukraine", 67, 68, 72, 73, 83, 50, 60, 69, PlayerStyle::Balanced, {"Team Player"}));
    return t;
}

Team makeENCE() {
    Team t("ENCE", "International", 37);
    t.addPlayer(Player("gla1ve", "IGL", "Denmark", 62, 66, 83, 88, 60, 50, 62, 77, PlayerStyle::Balanced, {"Leader", "Tactical Genius"}));
    t.addPlayer(Player("sdy", "Rifler", "Ukraine", 82, 77, 82, 84, 50, 50, 57, 75, PlayerStyle::Balanced, {"Team Player", "Versatile"}));
    t.addPlayer(Player("xKacpersky", "Rifler", "Poland", 85, 74, 70, 71, 68, 50, 64, 74, PlayerStyle::Aggressive, {}));
    t.addPlayer(Player("podi", "AWP", "Finland", 68, 65, 67, 68, 45, 75, 70, 68, PlayerStyle::Balanced, {"Inconsistent"}));
    t.addPlayer(Player("Neityu", "Support", "France", 71, 66, 70, 70, 55, 50, 60, 66, PlayerStyle::Balanced, {}));
    return t;
}

Team makeFnatic() {
    Team t("fnatic", "International", 38);
    t.addPlayer(Player("blameF", "Lurker", "Denmark", 87, 83, 85, 64, 50, 50, 57, 80, PlayerStyle::Passive, {"Baiter", "Selfish"}));
    t.addPlayer(Player("MATYS", "Rifler", "Slovakia", 80, 75, 74, 76, 70, 50, 50, 74, PlayerStyle::Balanced, {}));
    t.addPlayer(Player("KRIMZ", "Support", "Sweden", 70, 63, 83, 80, 57, 50, 59, 77, PlayerStyle::Balanced, {"Team Player"}));
    t.addPlayer(Player("fear", "IGL", "Ukraine", 57, 60, 69, 75, 90, 50, 50, 65, PlayerStyle::Balanced, {}));
    t.addPlayer(Player("Burmylov", "AWP", "Ukraine", 60, 63, 65, 67, 66, 72, 68, 63, PlayerStyle::Balanced, {}));
    return t;
}

Team make9Pandas() {
    Team t("9 Pandas", "Russia", 56);
    t.addPlayer(Player("r3salt", "Rifler", "Russia", 84, 80, 70, 69, 50, 40, 54, 80, PlayerStyle::Passive, {}));
    t.addPlayer(Player("d1Ledez", "Rifler", "Russia", 80, 73, 68, 70, 64, 40, 54, 78, PlayerStyle::Balanced, {}));
    t.addPlayer(Player("shalfey", "IGL", "Russia", 67, 60, 71, 71, 45, 40, 45, 68, PlayerStyle::Aggressive, {}));
    t.addPlayer(Player("Alv", "AWP", "Russia", 62, 65, 67, 68, 40, 68, 43, 65, PlayerStyle::Passive, {}));
    t.addPlayer(Player("Krad", "Lurker", "Russia", 65, 63, 70, 70, 40, 40, 42, 60, PlayerStyle::Balanced, {}));
    return t;
}

Team makeODDIK() {
    Team t("ODDIK", "Brazil", 70);
    t.addPlayer(Player("WOOD7", "IGL", "Brazil", 47, 50, 70, 71, 40, 63, 42, 60, PlayerStyle::Balanced, {}));
    t.addPlayer(Player("togs", "Support", "Brazil", 67, 64, 63, 64, 35, 40, 70, 65, PlayerStyle::Balanced, {}));
    t.addPlayer(Player("naitte", "Rifler", "Brazil", 64, 65, 60, 67, 50, 40, 40, 65, PlayerStyle::Aggressive, {}));
    t.addPlayer(Player("matios", "Lurker", "Brazil", 60, 62, 57, 64, 42, 40, 60, 63, PlayerStyle::Passive, {}));
    t.addPlayer(Player("Ceruttera", "Rifler", "Brazil", 50, 55, 56, 54, 42, 40, 35, 50, PlayerStyle::Balanced, {}));
    return t;
}

Team makeChinggis() {
    Team t("Chinggis Warriors", "Mongolia", 78);
    t.addPlayer(Player("controlez", "AWP", "Mongolia", 60, 60, 62, 60, 35, 60, 65, 60, PlayerStyle::Aggressive, {}));
    t.addPlayer(Player("ROUX", "Entry", "Mongolia", 64, 56, 57, 56, 70, 35, 40, 54, PlayerStyle::Aggressive, {}));
    t.addPlayer(Player("Efire", "IGL", "Mongolia", 48, 52, 58, 60, 40, 35, 45, 53, PlayerStyle::Balanced, {}));
    t.addPlayer(Player("NEUZ", "Rifler", "Mongolia", 67, 63, 55, 57, 35, 35, 35, 56, PlayerStyle::Balanced, {}));
    t.addPlayer(Player("ariucle", "Rifler", "Mongolia", 62, 54, 56, 54, 50, 35, 35, 53, PlayerStyle::Passive, {}));
    return t;
}

Team makeExceritus() {
    Team t("Exceritus", "Peru", 95);
    t.addPlayer(Player("Antuanette", "IGL", "Peru", 50, 53, 55, 58, 33, 30, 35, 53, PlayerStyle::Passive, {}));
    t.addPlayer(Player("SJR", "Rifler", "Peru", 52, 60, 56, 57, 45, 30, 32, 55, PlayerStyle::Aggressive, {}));
    t.addPlayer(Player("zockie", "Support", "Peru", 55, 52, 51, 56, 35, 30, 30, 54, PlayerStyle::Balanced, {}));
    t.addPlayer(Player("Slayerhz", "AWP", "Peru", 40, 50, 50, 48, 30, 50, 50, 49, PlayerStyle::Aggressive, {}));
    t.addPlayer(Player("BabyRage", "Rifler", "Colombia", 43, 56, 52, 53, 30, 30, 40, 52, PlayerStyle::Balanced, {}));
    return t;
}

Team makeNomadS() {
    Team t("NomadS", "Mongolia", 101);
    t.addPlayer(Player("hoolig4n", "AWP", "Mongolia", 38, 47, 48, 47, 30, 52, 30, 48, PlayerStyle::Aggressive, {}));
    t.addPlayer(Player("shinobi", "Rifler", "Mongolia", 45, 50, 49, 52, 30, 30, 30, 52, PlayerStyle::Passive, {}));
    t.addPlayer(Player("HenX", "IGL", "Mongolia", 42, 48, 49, 46, 30, 30, 30, 50, PlayerStyle::Balanced, {}));
    t.addPlayer(Player("KRST", "Support", "Mongolia", 34, 40, 40, 37, 30, 30, 50, 43, PlayerStyle::Balanced, {}));
    t.addPlayer(Player("CRYTICAL", "Rifler", "Mongolia", 32, 43, 45, 34, 40, 30, 33, 45, PlayerStyle::Aggressive, {}));
    return t;
}

Team makeGodsReign() {
    Team t("Gods Reign", "India", 117);
    t.addPlayer(Player("Ph1NNN", "Rifler", "India", 65, 63, 60, 52, 20, 20, 45, 64, PlayerStyle::Balanced, {}));
    t.addPlayer(Player("R2B2", "Entry", "India", 50, 55, 59, 50, 40, 20, 45, 60, PlayerStyle::Balanced, {}));
    t.addPlayer(Player("f1redup", "AWP", "India", 45, 52, 55, 47, 20, 40, 32, 57, PlayerStyle::Aggressive, {}));
    t.addPlayer(Player("Rossi", "Support", "India", 48, 42, 48, 49, 45, 20, 50, 51, PlayerStyle::Passive, {}));
    t.addPlayer(Player("Bhavi", "IGL", "India", 35, 30, 50, 53, 55, 20, 24, 50, PlayerStyle::Passive, {}));
    return t;
}

// ============================================================
//  MAIN
// ============================================================

int main() {
    srand(time(0));

    // Build 16 teams across all tiers
    Team spirit    = makeSpirit();
    Team vitality  = makeVitality();
    Team faze      = makeFaZe();
    Team vp        = makeVirtusProf();
    Team pain      = makePaiN();
    Team betboom   = makeBetBoom();
    Team b8        = makeB8();
    Team ence      = makeENCE();
    Team fnatic    = makeFnatic();
    Team pandas    = make9Pandas();
    Team oddik     = makeODDIK();
    Team chinggis  = makeChinggis();
    Team exceritus = makeExceritus();
    Team nomads    = makeNomadS();
    Team godsreign = makeGodsReign();

    // Need 16 — add one more mid-tier
    Team envy("Envy", "International", 49);
    envy.addPlayer(Player("siuhy", "IGL", "Poland", 68, 68, 87, 90, 65, 50, 53, 80, PlayerStyle::Balanced, {"Tactical Genius"}));
    envy.addPlayer(Player("Perfecto", "Support", "Russia", 78, 75, 80, 84, 60, 50, 82, 84, PlayerStyle::Passive, {"Team Player"}));
    envy.addPlayer(Player("k0nfig", "Rifler", "Denmark", 80, 72, 74, 77, 67, 50, 60, 80, PlayerStyle::Balanced, {}));
    envy.addPlayer(Player("dupreeh", "Rifler", "Denmark", 76, 70, 77, 80, 64, 50, 60, 75, PlayerStyle::Balanced, {}));
    envy.addPlayer(Player("GuardiaN", "AWP", "Slovakia", 57, 58, 70, 70, 50, 73, 54, 65, PlayerStyle::Passive, {}));

    vector<const Team*> allTeams = {
            &spirit, &vitality, &faze, &vp,
            &pain, &betboom, &b8, &ence,
            &fnatic, &envy, &pandas, &oddik,
            &chinggis, &exceritus, &nomads, &godsreign
    };

    // ============================================================
    //  CREATE AND RUN TOURNAMENT
    // ============================================================

    Tournament tournament("BLAST World Final 2025", TournamentFormat::Groups,
                          allTeams, 2000000, "Copenhagen", "Nov 15, 2025", "S-Tier");

    cout << "================================================================" << endl;
    cout << "       " << tournament.getName() << endl;
    cout << "       " << tournament.getTeamCount() << " Teams | Groups + Playoffs | Bo1" << endl;
    cout << "       Rankings: #1 to #117 | Prize Pool: $2,000,000" << endl;
    cout << "================================================================" << endl;

    // Print all team ratings before simulation
    cout << "\n--- All Teams (by Ranking) ---" << endl;
    // Sort for display
    vector<const Team*> sortedTeams = allTeams;
    sort(sortedTeams.begin(), sortedTeams.end(), [](const Team* a, const Team* b) {
        return a->getRanking() < b->getRanking();
    });
    for (const Team* t : sortedTeams) printTeamRating(*t);

    // *** RUN THE TOURNAMENT ***
    tournament.simulate();

    // ============================================================
    //  DISPLAY GROUP STAGE RESULTS
    // ============================================================
    cout << "\n================================================================" << endl;
    cout << "                       GROUP STAGE" << endl;
    cout << "================================================================" << endl;

    const auto& groupResults = tournament.getGroupResults();
    for (const auto& gr : groupResults) {
        cout << "\n  --- " << gr.groupName << " ---" << endl;

        // Print group composition
        cout << "  Teams: ";
        for (size_t i = 0; i < gr.standings.size(); i++) {
            cout << gr.standings[i].team->getName();
            if (i < gr.standings.size() - 1) cout << ", ";
        }
        cout << endl;

        // Print each match scoreboard
        int matchNum = 1;
        for (const auto& match : gr.matches) {
            int r1 = RatingCalculator::getTeamOverallRating(*sortedTeams[0]); // not needed for label
            string label = "  " + gr.groupName + " M" + to_string(matchNum++) + ": " +
                           match.team1Name + " vs " + match.team2Name;
            printMatchScoreboard(match, label);
        }

        // Print standings
        printGroupStandings(gr);
    }

    // ============================================================
    //  DISPLAY QUALIFIED TEAMS
    // ============================================================
    cout << "\n================================================================" << endl;
    cout << "                    QUALIFIED FOR PLAYOFFS" << endl;
    cout << "================================================================" << endl;
    for (size_t i = 0; i < groupResults.size(); i++) {
        const auto& gr = groupResults[i];
        cout << "  " << gr.groupName << ": "
             << gr.standings[0].team->getName() << " (1st), "
             << gr.standings[1].team->getName() << " (2nd)" << endl;
    }

    // ============================================================
    //  DISPLAY PLAYOFF MATCHES
    // ============================================================
    const auto& playoffs = tournament.getPlayoffMatches();

    // QFs
    cout << "\n================================================================" << endl;
    cout << "                     QUARTER-FINALS" << endl;
    cout << "================================================================" << endl;
    for (const auto& pm : playoffs) {
        if (pm.roundName.find("QF") != string::npos) {
            string label = "  " + pm.roundName + ": " + pm.result.team1Name + " vs " + pm.result.team2Name;
            printMatchScoreboard(pm.result, label);
        }
    }

    // SFs
    cout << "\n================================================================" << endl;
    cout << "                      SEMI-FINALS" << endl;
    cout << "================================================================" << endl;
    for (const auto& pm : playoffs) {
        if (pm.roundName.find("SF") != string::npos) {
            string label = "  " + pm.roundName + ": " + pm.result.team1Name + " vs " + pm.result.team2Name;
            printMatchScoreboard(pm.result, label);
        }
    }

    // Final
    cout << "\n================================================================" << endl;
    cout << "                      GRAND FINAL" << endl;
    cout << "================================================================" << endl;
    for (const auto& pm : playoffs) {
        if (pm.roundName == "FINAL") {
            string label = "  FINAL: " + pm.result.team1Name + " vs " + pm.result.team2Name;
            printMatchScoreboard(pm.result, label);
        }
    }

    // ============================================================
    //  CHAMPION
    // ============================================================
    cout << "\n================================================================" << endl;
    cout << "                   TOURNAMENT CHAMPION" << endl;
    cout << "================================================================" << endl;
    cout << "\n  >>> " << tournament.getChampion() << " wins " << tournament.getName() << "! <<<" << endl;

    // ============================================================
    //  PLACEMENTS
    // ============================================================
    cout << "\n--- Final Placements ---" << endl;
    const auto& placements = tournament.getPlacements();
    for (size_t i = 0; i < placements.size(); i++) {
        cout << "  " << (i + 1) << ". " << placements[i]->getName() << endl;
    }

    // ============================================================
    //  MVP
    // ============================================================
    MVPResult mvp = tournament.getMVP();
    cout << "\n--- Tournament MVP ---" << endl;
    cout << "  " << mvp.playerName << " (" << mvp.teamName << ")"
         << " | Rating: " << fixed << setprecision(2) << mvp.avgRating
         << " | Maps: " << mvp.matchesPlayed
         << " | Score: " << mvp.score
         << " | Team Placed: " << mvp.placement << endl;

    // ============================================================
    //  TOP 20 PLAYER RATINGS
    // ============================================================
    auto top20 = tournament.getPlayerRankings(2);
    printPlayerRankings("  TOP 20 PLAYER RATINGS (min 2 maps)", top20, 20);

    // ============================================================
    //  BOTTOM 10 PLAYER RATINGS
    // ============================================================
    auto bottom10 = tournament.getPlayerRankings(1, true);  // ascending
    printPlayerRankings("  BOTTOM 10 PLAYER RATINGS", bottom10, 10);

    // ============================================================
    //  INTEGRITY CHECK
    // ============================================================
    cout << "\n--- Integrity Check ---" << endl;
    cout << "  donk aim: " << spirit.getRoster()[0].getAim() << " (99)" << endl;
    cout << "  Zyw0o aim: " << vitality.getRoster()[0].getAim() << " (99)" << endl;
    cout << "  CRYTICAL aim: " << nomads.getRoster()[4].getAim() << " (32)" << endl;
    cout << "  Total matches played: " << tournament.getAllMatches().size() << endl;
    cout << "  Placements count: " << tournament.getPlacements().size() << endl;

    return 0;
}
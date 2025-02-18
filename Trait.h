//
// Created by Aidan Armas on 2/9/25.
//

#ifndef TRAIT_H
#define TRAIT_H

#include <map>
#include <string>
#include <utility>


using namespace std;

struct Trait {
    string name;
    double effect; // CHEMISTRY IMPACT
    int statBoost; // boost or penalty for each attributes

    Trait() : name(""), effect(0), statBoost(0) {} // DEFAULT CONSTRUCTOR

    Trait(string n, double e, int s) : name(n), effect(e), statBoost(s) {}
};

//PREDEFINED TRAITS
static map<string, Trait> predefinedTraits = {

        //NEGATIVE TRAITS
        {"Selfish", Trait("Selfish", -5, -2)},
        {"Toxic", Trait("Toxic", -10, +0)},
        {"Inconsistent", Trait("Inconsistent", -3, +0)},
        {"Choker", Trait("Choker", -1, -2)},
        {"Tilter", Trait("Tilter", -1, -1)},
        {"Bad Communicator", Trait("Bad Communicator", -4, -1)},
        {"Baiter", Trait("Baiter", -5, +0)},
        {"Stubborn", Trait("Stubborn", -2, -1)},
        {"Large Ego", Trait("Large Ego", -4, -1)},
        {"Lazy", Trait("Lazy", -2, -2)},


        //POSITIVE TRAITS
        {"Leader", Trait("Leader", +7, +4)},
        {"Team Player", Trait("Team Player", +3, +2)},
        {"Clutch Master", Trait("Clutch Master", +2, +1)},
        {"Supportive", Trait("Supportive", +2, +1)},
        {"Aim Demon", Trait("Aim Demon", +0, +3)},
        {"Tactical Genius", Trait("Tactical Genius", +2,+3)},
        {"Disciplined", Trait("Disciplined", +0, +2)},
        {"Versatile", Trait("Versatile", +0, +5)},
        {"Quick Learner", Trait("Quick Learner", +0, +2)},
        {"Master Communicator", Trait("Master Communicator", +4, +1)},
         //NONE
         {"None", Trait("None", +0, +0)}
};

#endif //TRAIT_H

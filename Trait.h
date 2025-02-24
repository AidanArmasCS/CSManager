#ifndef TRAIT_H
#define TRAIT_H

#include <map>
#include <string>
#include <vector>

using namespace std;

struct Trait {
    string name;
    double effect;  // Chemistry impact
    int statBoost;  // Attribute boost

    Trait() : name("None"), effect(0), statBoost(0) {}  // Default constructor

    Trait(string n, double e, int s) : name(std::move(n)), effect(e), statBoost(s) {}
};

// Declare predefinedTraits globally (but define it in Trait.cpp)
extern map<string, Trait> predefinedTraits;

#endif // TRAIT_H
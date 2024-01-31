/* Class Functions
 * ----------
 * COMP.CS.110 FALL 2023
 * ----------
 * Class describing the functions.
 *
 * This class is retrieved from template folder. However, map DS has
 * been added (starting line 29).
 *
 *
 * Program author
 * Name: Abdullah Sagir
 * Student number: 151945467
 * UserID: nxabsa
 * E-Mail: abdulla.sagir@tuni.fi
 *
 * */

#ifndef FUNCTIONS_HH
#define FUNCTIONS_HH

#include <map>
#include <string>
#include <vector>

using namespace std;

enum Result{NOTHING, PAIR, TWO_PAIRS, THREE_OF_A_KIND, STRAIGHT, FULL_HOUSE,
              FOUR_OF_A_KIND, YATZY};

const map<Result, string> RESULT_TEXTS =
    { { NOTHING,         "nothing"},
     { PAIR,            "pair"},
     { TWO_PAIRS,       "two pairs"},
     { THREE_OF_A_KIND, "three of a kind"},
     { STRAIGHT,        "straight"},
     { FULL_HOUSE,      "full house"},
     { FOUR_OF_A_KIND,  "four of a kind"},
     { YATZY,           "yatzy"} };

// Rolls a dice, i.e. returns a random number from the interval [1..6].
int roll_dice();

// The purpose of this function is to form and return textual construction
// for the values in point_values vector given as a parameter.
// Format could be e.g.:
//   5 5 5 5 5  -> "yatzy (5)"
//   3 2 3 3 3  -> "four of a kind (3), in addition 2"
//   5 1 1 5 5  -> "full house (5, 1)"
//   4 2 6 5 3  -> "straight (6)"
//   4 6 4 4 2  -> "three of a kind (4), in addition 6 ja 2",
//   5 1 1 2 5  -> "two pairs (5, 1), in addition 2"
//   3 1 6 4 3  -> "pair (3), in addition 6, 4, 1"
//   2 6 1 4 5  -> "nothing (6), in addition 5, 4, 2, 1"
// or something similar.
//
// The textual construction is passed in the reference parameter result_text.
// The return value is the rank of the result as follows:
// NOTHING = 0, PAIR = 1, TWO_PAIRS = 2, THREE_OF_A_KIND = 3, STRAIGHT = 4,
// FULL_HOUSE = 5, FOUR_OF_A_KIND = 6, YATZY = 7.
int construe_result(const vector<int>& point_values, string& result_text);

// Decides the winner based on the given point values of several players.
// Each element of the outer vector is a point value vector of a player.
Result decide_winner(const vector<vector<int>>& point_values, vector<int> &winners);
#endif // FUNCTIONS_HH

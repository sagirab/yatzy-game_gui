/* Class GameEngine
 * ----------
 * COMP.CS.110 FALL 2023
 * ----------
 * Class describing the game engine.
 *
 * This class is retrieved from template folder. However, few changes made.
 *
 * Player struct is now have string name. Simply because we can change
 * the names for players.
 *
 * Another //Obvious constant is added for storing max dice value - 6.
 *
 * Changed function descriptions are: roll ad report winner.
 *
 * Program author
 * Name: Abdullah Sagir
 * Student number: 151945467
 * UserID: nxabsa
 * E-Mail: abdulla.sagir@tuni.fi
 *
 * */
#ifndef GAMEENGINE_HH
#define GAMEENGINE_HH

#include "functions.hh"
#include <string>
#include <vector>

// Obvious constants
const int INITIAL_NUMBER_OF_ROLLS = 3;
const int NUMBER_OF_DICES = 5;
const int MAX_DICE_VALUE = 6;

// Data of each player
struct Player
{
    unsigned int id_;
    unsigned int rolls_left_;
    std::vector<int> latest_point_values_;
    std::vector<int> best_point_values_;
    std::string name;
};

class GameEngine
{
public:

    enum class Status;
    // Constructor
    GameEngine();

    // Destructor
    ~GameEngine();

    // Adds a new player
    void add_player(const Player player);

    // Prints guide text, telling which player is in turn and how many trials
    // they have left.
    void update_guide() const;

    // Rolls all dices, i.e. draws a new series of face numbers for the player
    // currently in turn. Moreover, reports the winner, if after the draw, all
    // players have used all their turns.
    Status roll(std::vector<int> &diceValues, const std::vector<int> &lockStatus,  std::pair<std::vector<int>, Result> &winnerReport);

    // Gives turn for the next player having turns left, i.e. for the next
    // element in the players_ vector. After the last one, turn is given for
    // the second one (since the first one is NOBODY).
    void give_turn();

    // Reports a winner based on the current situation and sets the game_over_
    // attribute as true.
    Result report_winner(std::vector<int> &winners);

    // Tells if the game is over, i.e. if all players have used all their
    // turns.
    bool is_game_over() const;

    // give total player count
    int playerCount() const;

    // give current player index
    int currentPlayerIdx() const;
    Player currentPlayer() const;
    Player playerInfo(int idx) const;


    // reset the games
    void reset();

private:
    // Reports the status of the player currently in turn
    void report_player_status() const;

    // Updates best and latest points of the player in turn:
    // latest_point_values_ will always be new_points,
    // best_point_values_ will be new_points, if the last_mentioned is better.
    void update_points(const std::vector<int>& new_points);

    // Returns true if all turns of all players have been used,
    // otherwise returns false.
    bool all_turns_used() const;

    // Vector of all players
    std::vector<Player> players_;

    // Tells the player currently in turn (index of players_ vector)
    unsigned int game_turn_;

    // Tells if the game is over
    bool game_over_;
};

enum class GameEngine::Status
{
    PROGRESSING = 100,
    NO_MORE_ROLLS_LEFT,
    DONE,
    FAIL
};

#endif // GAMEENGINE_HH

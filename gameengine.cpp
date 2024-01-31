/* Class CPP GameEngine
 * ----------
 * COMP.CS.110 FALL 2023
 * ----------
 * Class functions describing the game engine.
 *
 * Program author
 * Name: Abdullah Sagir
 * Student number: 151945467
 * UserID: nxabsa
 * E-Mail: abdulla.sagir@tuni.fi
 *
 * */
#include "gameengine.hh"
#include "functions.hh"
#include <iostream>
#include <sstream>


GameEngine::GameEngine():
    game_turn_(0), game_over_(false)
{
}

GameEngine::~GameEngine()
{
}

void GameEngine::add_player(const Player player)
{
    players_.push_back(player);
}

void GameEngine::update_guide() const
{
    if(players_.size() <= game_turn_)
    {
        std::cout << "Internal error: update_guide" << std::endl;
        return;
    }
    ostringstream outputstream{""};
    outputstream << "Player " << game_turn_ + 1 << " in turn, "
                 << players_.at(game_turn_).rolls_left_ << " trials left!";
    std::cout << outputstream.str() << std::endl;
}

GameEngine::Status GameEngine::roll(std::vector<int> &diceValues, const std::vector<int> &lockStatus,  std::pair<std::vector<int>, Result> &winnerReport)
{
    if(players_.size() <= game_turn_)
    {
        std::cout << "Internal error: roll" << std::endl;
        return Status::FAIL;
    }

    if(players_.at(game_turn_).rolls_left_ == 0)
    {
        std::cout << "No more rolls left" << std::endl;
        return Status::NO_MORE_ROLLS_LEFT;
    }

    ostringstream outputstream{""};
    vector<int> new_points = diceValues;
    unsigned int dice = 0;
    while ( dice < NUMBER_OF_DICES )
    {
        if (lockStatus.at(dice)){
            std::cout << new_points.at(dice) << " ";
            ++dice;
            continue;
        }

        int point_value = roll_dice();
        std::cout << point_value << " ";
        new_points.at(dice) = point_value;
        ++dice;
    }
    diceValues = new_points;

    update_points(new_points);
    report_player_status();

    // Decreasing rolls left
    --players_.at(game_turn_).rolls_left_;

    // Checking if the player in turn has rolls left
    if ( players_.at(game_turn_).rolls_left_ == 0 )
    {
        outputstream << "Turn of " << players_.at(game_turn_).id_
                     << " has ended!";
        std::cout << outputstream.str() << std::endl;
    }

    // Checking if any player has turns left
    if ( all_turns_used() )
    {
        winnerReport.second = report_winner(winnerReport.first);
        return Status::DONE;
    }
    return Status::PROGRESSING;
}

void GameEngine::give_turn()
{
    // Searching for the next player among those, whose id_ is greater than
    // that of the current player
    for ( unsigned int i = game_turn_ + 1; i < players_.size(); ++i )
    {
        if ( players_.at(i).rolls_left_ > 0 )
        {
            game_turn_ = i;
            return;
        }
    }

    // A suitable next player couldn't be found in the previous search, so
    // searching for the next player among those, whose id_ is less than
    // or equal to that of the current player
    // (perhaps the current player is the only one having turns left)
    for(unsigned int i = 0; i <= game_turn_; ++i)
    {
        if(players_.at(i).rolls_left_ > 0)
        {
            game_turn_ = i;
            return;
        }
    }
}

Result GameEngine::report_winner(vector<int> &winners)
{
    vector<vector<int>> all_point_values;
    for ( auto player : players_ )
    {
        all_point_values.push_back(player.best_point_values_);
    }
    game_over_ = true;

    return decide_winner(all_point_values, winners);
}

bool GameEngine::is_game_over() const
{
    return game_over_;
}

int GameEngine::playerCount() const
{
    return players_.size();
}

int GameEngine::currentPlayerIdx() const
{
    return game_turn_;
}

Player GameEngine::currentPlayer() const
{
    return players_.at(game_turn_);
}

Player GameEngine::playerInfo(int idx) const
{
    if(static_cast<std::vector<Player>::size_type>(idx) > players_.size())
        return {};

    return players_.at(idx);
}

void GameEngine::reset()
{
    game_turn_ = 0;
    game_over_ = false;
    players_.clear();
}

void GameEngine::report_player_status() const
{
    if ( players_.size() <= game_turn_ )
    {
        std::cout << "Internal error: report_player_status" << std::endl;
        return;
    }
    string textual_description = "";
    construe_result(players_.at(game_turn_).latest_point_values_,
                    textual_description);
    std::cout << textual_description << std::endl;
}

void GameEngine::update_points(const std::vector<int>& new_points)
{
    if ( players_.size() <= game_turn_ )
    {
        std::cout << "Internal error: update_points" << std::endl;
        return;
    }
    string dummy = "";
    int new_result = construe_result(new_points, dummy);
    int best_result_so_far
            = construe_result(players_.at(game_turn_).best_point_values_,
                              dummy);
    if ( new_result > best_result_so_far )
    {
        players_.at(game_turn_).best_point_values_ = new_points;
    }
    players_.at(game_turn_).latest_point_values_ = new_points;
}

bool GameEngine::all_turns_used() const
{
    for ( auto player : players_ )
    {
        if ( player.rolls_left_ > 0 )
        {
            return false;
        }
    }
    return true;
}

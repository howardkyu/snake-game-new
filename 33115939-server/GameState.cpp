#include "GameState.h"

const std::string GameState::FOOD_COLOR = "blue";
const std::string GameState::PLAYER_ONE_COLOR = "red";
const std::string GameState::PLAYER_TWO_COLOR = "green";
const std::string GameState::PLAYER_ONE_DIRECTION = "R";
const std::string GameState::PLAYER_TWO_DIRECTION = "L";

GameState::GameState() {
    inSession = false;
}

/******************/

/**
 * Adds a new player to the player map
 */
void GameState::addPlayer(int clientID, std::string playerID) {
    playerMap.insert({clientID, playerID});
}

/**
 * Removes a player from the player map if the player exists
 */
void GameState::removePlayer(int clientID) {
    if (playerMap.find(clientID) != playerMap.end())
        playerMap.erase(clientID);
}

/**
 * Gets the player ID from the client ID
 */
std::string GameState::getPlayerID(int clientID) {
    return playerMap[clientID];
}

/**
 * Returns the number of players currently connected
 */
int GameState::playerCount() {
    return playerMap.size();
}

/**
 * Returns if game is currently waiting for
 * another user
 */
int GameState::waiting() {
    return playerMap.size() <= 2;
}

/******************/

void GameState::setup() {
    bool first = true;
    for (auto it : playerMap) {
        if (first) {
            player1 = Player(it.first, it.second, PLAYER_ONE_COLOR, PLAYER_ONE_DIRECTION);
            first = false;
        } else {
            player2 = Player(it.first, it.second, PLAYER_TWO_COLOR, PLAYER_TWO_DIRECTION);
        }
    }
}

void GameState::start() {
    inSession = true;
}

void GameState::reset() {
    inSession = false;
}

std::string GameState::generateSetupMessage() {
    std::string setupMessage = "SETUP:" + 
        std::to_string(GameState::COL) + ":" + std::to_string(GameState::ROW) +
        ":" + GameState::FOOD_COLOR + ":" +
        player1.getPlayerID() + "," + player1.getColor() + "," + player1.getDirection() + ":" +
        player2.getPlayerID() + "," + player2.getColor() + "," + player2.getDirection();

    return setupMessage;  
}
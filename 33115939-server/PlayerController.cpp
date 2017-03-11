#include "PlayerController.h"

PlayerController::PlayerController() {
}

/**
 * Adds a new player to the player map
 */
void PlayerController::addPlayer(int clientID, std::string playerID) {
    playerMap.insert({clientID, playerID});
}

/**
 * Removes a player from the player map if the player exists
 */
void PlayerController::removePlayer(int clientID) {
    if (playerMap.find(clientID) != playerMap.end())
        playerMap.erase(clientID);
}

/**
 * Returns the number of players currently connected
 */
int PlayerController::size() {
    return playerMap.size();
}

/**
 * Returns if player controller is currently waiting for
 * another user
 */
int PlayerController::waiting() {
    return playerMap.size() <= 2;
}

/**
 * Returns whether the map is empty
 */
int PlayerController::empty() {
    return playerMap.empty();
}


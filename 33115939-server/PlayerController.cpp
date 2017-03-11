PlayerController::PlayerController() {
}

/**
 * Adds a new player to the player map
 */
void PlayerController::addPlayer(int clientID, std::string playerID) {
    playerMap.insert({clientID, playerID});
}

/**
 * Removes a player from the player map
 */
void PlayerController::removePlayer(int clientID) {
    playerMap.erase(clientID);
}

/**
 * Returns the number of players currently connected
 */
int PlayerController::size() {
    return playerMap.size();
}

/**
 * Returns whether the map is empty
 */
int PlayerController::size() {
    return playerMap.empty();
}


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
    cidToPidMap.insert({clientID, playerID});
}

/**
 * Removes a player from the player map if the player exists
 */
void GameState::removePlayer(int clientID) {
    if (cidToPidMap.find(clientID) != cidToPidMap.end())
        cidToPidMap.erase(clientID);
}

/**
 * Gets the player ID from the client ID
 */
std::string GameState::getPlayerID(int clientID) {
    return cidToPidMap[clientID];
}

/**
 * Returns the number of players currently connected
 */
int GameState::playerCount() {
    return cidToPidMap.size();
}

/******************/

/**
 * Adds the message to the appropriate player's message queue
 */
int GameState::addMessage(int clientID, std::string message) {
    if(player1.getClientID() == clientID) {
        player1.addMessage(message);
    } else {
        player2.addMessage(message);
    }
}

std::string GameState::generateNextState() {
    // Get player1 and player2 next messages if any exists
    std::string player1Message = getNextMessage(player1);
    std::string player2Message = getNextMessage(player2);

    // If either have not receive a message, postpone
    if (player1Message == "NULL" || player2Message == "NULL") 
        return "NULL";

    // Add a coordinate to player1's snake and erase the tail
    Coord player1Head = getHeadCoord(player1Message);
    player1.addHead(player1Head);
    player1.removeTail();

    // Add a coordinate to player2's snake and erase the tail
    Coord player2Head = getHeadCoord(player2Message);
    player2.addHead(player2Head);
    player2.removeTail();

    // Check for collision
    bool collided = checkCollision(player1Head, player2Head);

    if(collided) {
        return "COLLIDED";
    } else {
        // Construct message to send to both clients
        // Syntax is: UPDATE:pid1;ADD,x,y;ERASE,x,y;SCORE,xxx:pid2;ADD,x,y;ERASE,x,y;SCORE,yyy
        return "UPDATE:" + player1Message + ";SCORE," + std::to_string(player1.getScore()) + ":" + player2Message + "SCORE," + std::to_string(player2.getScore());
    }
}

/******************/

void GameState::setup() {
    bool first = true;
    for (auto it : cidToPidMap) {
        if (first) {
            player1 = Player(it.first, it.second, PLAYER_ONE_COLOR, PLAYER_ONE_DIRECTION);
            player1.addHead(Coord{GameState::PLAYER_ONE_START_X, GameState::START_Y});
            first = false;
        } else {
            player2 = Player(it.first, it.second, PLAYER_TWO_COLOR, PLAYER_TWO_DIRECTION);
            player2.addHead(Coord{GameState::PLAYER_TWO_START_X, GameState::START_Y});
        }
    }
}

void GameState::start() {
    inSession = true;
}

void GameState::reset() {
    inSession = false;
    cidToPidMap.clear();
}

/**
 * Returns if game is currently waiting for
 * another user
 */
int GameState::waiting() {
    return cidToPidMap.size() <= 2;
}

/**
 * Generates the setup string
 * SETUP:COL:ROW:FOOD_COLOR:pid1,color,R,x,y:pid2,color,L,x,y
 */
std::string GameState::generateSetupMessage() {
    std::string setupMessage = "SETUP:" + 
        std::to_string(GameState::COL) + ":" + std::to_string(GameState::ROW) +
        ":" + GameState::FOOD_COLOR + ":" +
        player1.getPlayerID() + "," + player1.getColor() + "," + player1.getDirection() +
        std::to_string(player1.getHeadCoord().x) + "," + std::to_string(player1.getHeadCoord().y) + ":" +
        player2.getPlayerID() + "," + player2.getColor() + "," + player2.getDirection() +
        std::to_string(player2.getHeadCoord().x) + "," + std::to_string(player2.getHeadCoord().y);

    return setupMessage;  
}

/******************/

/**
 * Returns a message from the player's queue
 * If none exists, return "NULL"
 */
std::string GameState::getNextMessage(Player player) {
    if(player.getMessageCount() > 0) {
        return player.getNextMessage();
    } else {
        return "NULL";
    }
}

bool GameState::checkCollision(Coord player1Coord, Coord player2Coord) {
    if(player1.hasCoord(player2Coord))
        return true;
    if (player2.hasCoord(player1Coord))
        return true;
    return false;
}

Coord GameState::getHeadCoord(std::string message) {
    // Initial message is pid;ADD,x,y;ERASE,x,y
    // Split message so that result vector should now have {"pid", ADD,x,y", "ERASE,x,y"}
    std::vector<std::string> commandVector = Parse::split(message,';');
    // Split messages so that addCoords vector has "ADD,x,y"
    std::vector<std::string> addCoords = Parse::split(commandVector[1], ',');

    return Coord{std::stoi(addCoords[1]), std::stoi(addCoords[2])};
}
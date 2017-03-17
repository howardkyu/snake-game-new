// Kuan Hao Yu     33115939
// Kimberly Nguy   86242309
// Pafoua Fang     84351619
// Kuanping Change 71018021

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

/**
 * Generates the string for the next authoritative game state
 */
std::string GameState::generateNextState() {
    // Return "NULL" if player1 or player2 has no messages
    if (player1.getMessageCount() == 0 || player2.getMessageCount() == 0) {
        return "NULL";
    }
    
    // Get player1 and player2 next messages if both exists
    std::string player1Message = player1.getNextMessage();
    std::string player2Message = player2.getNextMessage();

    // Add a coordinate to player1's snake and erase the tail
    Coord player1Head = getHeadCoord(player1Message);
    if (!ateFood(&player1, player1Head)) {
        player1.removeTail();
    }

    // Add a coordinate to player2's snake and erase the tail
    Coord player2Head = getHeadCoord(player2Message);
    if (!ateFood(&player2, player2Head)) {
        player2.removeTail();
    }
 
    // Check for collision
    bool collided = checkCollision(player1Head, player2Head);

    player2.addHead(player2Head);
    player1.addHead(player1Head);

    if (collided) {
        inSession = false;

        // COLLIDED:pid1;SCORE,xxx:pid2;SCORE,xxx
        return "COLLIDED:" + player1.getPlayerID() + ";SCORE," + std::to_string(player1.getScore()) +
            ":" + player2.getPlayerID() + ";SCORE," + std::to_string(player2.getScore());
    } else {
        // Construct message to send to both clients
        // Syntax: STATE:pid1;ADD,x,y;ERASE,x,y;SCORE,xxx:pid2;ADD,x,y;ERASE,x,y;SCORE,yyy:FOOD,x,y
        return "STATE:" + player1Message + ";SCORE," + std::to_string(player1.getScore()) + ":" +
            player2Message + ";SCORE," + std::to_string(player2.getScore()) +
            ":FOOD," + std::to_string(foodCoord.x) + "," + std::to_string(foodCoord.y);
    }
}

/******************/

/**
 * Sets up the food and the player in the beginning of the game
 */
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
    generateFood();
}

/**
 * Trigger inSession boolean to true
 */
void GameState::start() {
    inSession = true;
}

/**
 * Reset the game state to initial values
 */
void GameState::reset() {
    inSession = false;
    cidToPidMap.clear();
}

/**
 * Returns if game is currently waiting for
 * another user
 */
int GameState::waiting() {
    return cidToPidMap.size() < 2;
}

/**
 * Returns whether a game is in session
 */
bool GameState::gameInSession() {
    return inSession;
}

/**
 * Generates the setup string
 * SETUP:COL:ROW:FOOD_COLOR:pid1,color,R,x,y:pid2,color,L,x,y
 */
std::string GameState::generateSetupMessage() {
    std::string setupMessage = "SETUP:" + 
        std::to_string(GameState::COL) + ":" + std::to_string(GameState::ROW) +
        ":" + GameState::FOOD_COLOR + ":" + std::to_string(foodCoord.x) + ":" + std::to_string(foodCoord.y) + ":" +
        player1.getPlayerID() + "," + player1.getColor() + "," + player1.getDirection() + "," +
        std::to_string(player1.getHeadCoord().x) + "," + std::to_string(player1.getHeadCoord().y) + ":" +
        player2.getPlayerID() + "," + player2.getColor() + "," + player2.getDirection() + "," +
        std::to_string(player2.getHeadCoord().x) + "," + std::to_string(player2.getHeadCoord().y);

    return setupMessage;  
}

/******************/

/**
 * Checks whether two players' heads have collided
 */
bool GameState::checkCollision(Coord player1Head, Coord player2Head) {
    if (player1Head.x == player2Head.x && player1Head.y == player2Head.y)
        return true;
    if(player1.hasCoord(player1Head, player2Head))
        return true;
    if (player2.hasCoord(player1Head, player2Head))
        return true;
    return false;
}

/**
 * Check whether either player consumes a food
 */
bool GameState::ateFood(Player* player, Coord playerHead) {
    if (playerHead.x == foodCoord.x && playerHead.y == foodCoord.y) {
        player->incScore();
        generateFood();
        return true;
    }
    return false;
}

/**
 * Parses the new head coordinates from a player's message
 */
Coord GameState::getHeadCoord(std::string message) {
    // Initial message is pid;ADD,x,y;ERASE,x,y
    // Split message so that result vector should now have {"pid", ADD,x,y", "ERASE,x,y"}
    std::vector<std::string> commandVector = Parse::split(message,';');
    // Split messages so that addCoords vector has "ADD,x,y"
    std::vector<std::string> addCoords = Parse::split(commandVector[1], ',');

    return Coord{std::stoi(addCoords[1]), std::stoi(addCoords[2])};
}

/**
 * Generate a random coordinate for the food within the game board
 */
Coord GameState::generateFood() {
    foodCoord.x = rand() % COL;
    foodCoord.y = rand() % ROW;
}
// Kuan Hao Yu     33115939
// Kimberly Nguy   86242309
// Pafoua Fang     84351619
// Kuanping Change 71018021

#include "Player.h"

Player::Player() {}

Player::Player(int clientID, std::string playerID, std::string color, std::string direction) {
    this->clientID = clientID;
    this->playerID = playerID;
    this->color = color;
    this->score = 0;
    this->direction = direction;
    this->messageCount = 0;
}

/**
 * Add a message to the player's queue
 */
void Player::addMessage(std::string message) {
    messageQueue.push(message);
    ++messageCount;
}

/**
 * Return a message from the player's queue
 */
std::string Player::getNextMessage() {
    std::string message = messageQueue.front();
    messageQueue.pop();
    --messageCount;
    return message;
}

/**
 * Adds a new head to the player's snake
 */
void Player::addHead(Coord coord) {
    snake.push_front(coord);
}

/**
 * Removes the tail from a player's snake
 */
void Player::removeTail() {
    snake.pop_back();
}

/**
 * Retreive the head coordinate from the player's snake
 */
Coord Player::getHeadCoord() {
    return snake.front();
}

/**
 * Return whether a coord is inside the player's snake
 */
bool Player::hasCoord(Coord coord1, Coord coord2) {
    // std::cout << "Snake " << clientID << ":";
    for (int i = 0; i < snake.size(); i++) {
        // std::cout << snake[i].x <<"," << snake[i].y;
        if (coord1.x == snake[i].x && coord1.y == snake[i].y) {
            return true;
        }
        if (coord2.x == snake[i].x && coord2.y == snake[i].y) {
            return true;
        }
    }
    // std::cout << std::endl;
}

/**
 * Increments a player's score
 */
void Player::incScore() {
    this->score++;
}

/**
 * Return the player's client ID
 */
int Player::getClientID() {
    return clientID;
}

/**
 * Return the player's player ID
 */
std::string Player::getPlayerID() {
    return playerID;
}

/**
 * Return the player's snake color
 */
std::string Player::getColor() {
    return color;
}

/**
 * Return the player's score
 */
int Player::getScore() {
    return score;
}

/**
 * Return the player's snake
 */
std::deque<Coord> Player::getSnake() {
    return snake;
}

/**
 * Return the player's snake direction
 */
std::string Player::getDirection() {
    return direction;
}

/**
 * Return the player's message count
 */
int Player::getMessageCount() {
    return messageCount;
}
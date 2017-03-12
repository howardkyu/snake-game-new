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

void Player::addHead(Coord coord) {
    snake.push_front(coord);
}

void Player::removeTail() {
    snake.pop_back();
}

Coord Player::getHeadCoord() {
    return snake.front();
}

/**
 * Return whether a coord is inside the player's snake
 */
bool Player::hasCoord(Coord coord) {
    for(int i = 0; i < snake.size(); i++) {
        if(coord.x == snake[i].x && coord.y == snake[i].y) {
            return true;
        }
    }
}

int Player::getClientID() {
    return clientID;
}

std::string Player::getPlayerID() {
    return playerID;
}

std::string Player::getColor() {
    return color;
}

int Player::getScore() {
    return score;
}

std::deque<Coord> Player::getSnake() {
    return snake;
}

std::string Player::getDirection() {
    return direction;
}

int Player::getMessageCount() {
    return messageCount;
}
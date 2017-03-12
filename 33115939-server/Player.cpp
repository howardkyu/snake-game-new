#include "Player.h"

Player::Player() {}

Player::Player(int clientID, std::string playerID, std::string color, std::string direction) {
    this->clientID = clientID;
    this->playerID = playerID;
    this->color = color;
    this->score = 0;
    this->direction = direction;
}

void Player::addCoord(Coord coord) {
    snake.push_back(coord);
}

Coord Player::getHeadCoord() {
    return snake.front();
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

std::vector<Coord> Player::getSnake() {
    return snake;
}

std::string Player::getDirection() {
    return direction;
}
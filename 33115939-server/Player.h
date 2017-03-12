#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include "Coord.h"

class Player {
    int clientID;
    std::string playerID;
    std::string color;
    int score;
    std::vector<Coord> snake;
    std::string direction;

    public:
        // Constructor
        Player();
        Player(int clientID, std::string playerID, std::string color, std::string direction);

        void addCoord(Coord coord);
        Coord getHeadCoord();

        int getClientID();
        std::string getPlayerID();
        std::string getColor();
        int getScore();
        std::vector<Coord> getSnake();
        std::string getDirection();
};

#endif /* PLAYER_H */
#pragma once
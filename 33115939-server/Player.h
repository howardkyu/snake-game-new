#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include <deque>
#include <queue>
#include <iostream>
#include "Coord.h"

class Player {
    int clientID;
    std::string playerID;
    std::string color;
    int score;
    std::deque<Coord> snake;
    std::string direction;
    std::queue<std::string> messageQueue;
    int messageCount;

    public:
        // Constructor
        Player();
        Player(int clientID, std::string playerID, std::string color, std::string direction);

        // Player message queue methods
        void addMessage(std::string message);
        std::string getNextMessage();

        // Snake modifier methods
        void addHead(Coord coord);
        void removeTail();
        Coord getHeadCoord();
        bool hasCoord(Coord coord1, Coord coord2);

        // Player modifier methods
        void incScore();
        
        // Accessor methods
        int getClientID();
        std::string getPlayerID();
        std::string getColor();
        int getScore();
        std::deque<Coord> getSnake();
        std::string getDirection();
        int getMessageCount();
        
        
};

#endif /* PLAYER_H */
#pragma once
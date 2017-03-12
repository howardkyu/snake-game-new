#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <string>
#include <unordered_map>
#include <vector>
#include <deque>
#include <queue>
#include "Player.h"
#include "Coord.h"
#include "Parse.h"

class GameState {

    // Map containing clientIDs and their playerIDs
    std::unordered_map<int, std::string> cidToPidMap;

    // Boolean for whether game is in session
    bool inSession;
    Player player1;
    Player player2;

    public:
        // Constructor
        GameState();

        // Methods for controlling players
        void addPlayer(int clientID, std::string);
        void removePlayer(int clientID);
        std::string getPlayerID(int clientID);
        int playerCount();
        int addMessage(int clientID, std::string message);
        std::string generateNextState();

        // Methods for controlling the game
        void setup();
        void start();
        void reset();
        int waiting();
        std::string generateSetupMessage();
       
    private:
        // Static constants
        static constexpr int COL = 60;
        static constexpr int ROW = 60;
        static constexpr int PLAYER_ONE_START_X = COL / 4;
        static constexpr int PLAYER_TWO_START_X = COL - (COL / 4);
        static constexpr int START_Y = ROW / 2;
        static const std::string FOOD_COLOR;
        static const std::string PLAYER_ONE_COLOR;
        static const std::string PLAYER_TWO_COLOR;
        static const std::string PLAYER_ONE_DIRECTION;
        static const std::string PLAYER_TWO_DIRECTION;

        std::string getNextMessage(Player player);
        bool checkCollision(Coord player1Coord, Coord player2Coord);
        Coord getHeadCoord(std::string message);
};

#endif /* GAMESTATE_H */
#pragma once
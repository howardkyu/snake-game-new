#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <string>
#include <unordered_map>
#include <vector>
#include "Player.h"

class GameState {
    struct Coord {
        int x;
        int y;
    };

    // Map containing clientIDs and their playerIDs
    std::unordered_map<int, std::string> playerMap;

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

        // Methods for controlling the game
        void setup();
        void start();
        void reset();

        std::string generateSetupMessage();
        int waiting();

        // Static constants
        static constexpr int COL = 60;
        static constexpr int ROW = 60;
        static const std::string FOOD_COLOR;
        static const std::string PLAYER_ONE_COLOR;
        static const std::string PLAYER_TWO_COLOR;
        static const std::string PLAYER_ONE_DIRECTION;
        static const std::string PLAYER_TWO_DIRECTION;
};

#endif /* GAMESTATE_H */
#pragma once
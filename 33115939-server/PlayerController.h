#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include <string>
#include <unordered_map>

class PlayerController {
    // Map containing clientIDs and their playerIDs
    std::unordered_map<int, std::string> playerMap;
    public:
        // Constructor
        PlayerController();
        void addPlayer(int clientID, std::string);
        void removePlayer(int clientID);
        int size();
        int empty();
}
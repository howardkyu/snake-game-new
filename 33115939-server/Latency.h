#ifndef LATENCY_H
#define LATENCY_H

#include <queue>
#include <string>
#include <chrono>
#include <random>
#include <iostream>

struct Message {
    int clientID;
    std::string message;
    long long timeToProcess;
};

class Latency {
    public:
        Latency();
        void delayReceive(int clientID, std::string message, long long time);
        void delaySend(int clientID, std::string message);
        void receiveNextMessages(long long currentTime, void (*receiveHandler)(int clientID, std::string message));
        void sendNextMessages(long long currentTime, void (*sendHandler)(int clientID, std::string message));
        void sendNTP(int clientID);
        void reset();

        bool empty();      
    private:
        std::queue<Message> receiveQueue;
        std::queue<Message> sendQueue;
        std::default_random_engine generator;
        
        // Static constants
        static std::uniform_int_distribution<int> distribution;
        static constexpr int MAX_DELAY = 150;
        static constexpr int MIN_DELAY = MAX_DELAY/2;
        static constexpr int MAX_BUFFER_SIZE = 100;  
};

#endif
#pragma once
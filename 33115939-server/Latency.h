#ifndef LATENCY_H
#define LATENCY_H

#include <vector>
#include <string>

class Latency {
    std::vector<std::string> inQueue;
    std::vector<std::string> outQueue;

    public:
        Latency();
        void delayReceive();
        void delaySend();
        std::string receiveNextMessage();
        std::string sendNextMessage();

        
};
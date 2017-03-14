#include "Latency.h"

using namespace std;
using namespace chrono;

uniform_int_distribution<int> Latency::distribution(75, 150);

Latency::Latency() {
    generator = default_random_engine();
    sendQueue = queue<Message>();
    receiveQueue = queue<Message>();
}

void Latency::delayReceive(int clientID, string message, long long time) {
    // Generate a distributed random number from MIN_DELAY to MAX_DELAY
    int delay = Latency::distribution(generator);
    
    if (receiveQueue.size() <= Latency::MAX_BUFFER_SIZE) {
        long long receiveTimeStamp = time + delay;
        receiveQueue.push(Message{clientID, message, receiveTimeStamp});
    }
}

void Latency::delaySend(int clientID, string message) {
    // Convert to get the long long type time for the duration
    // time_since_epoch returns a the amount of time between this time and clock's epoch
    long long currentTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

    // Generate a distributed random number from MIN_DELAY to MAX_DELAY
    int delay = Latency::distribution(generator);   
        
    // If send queue is smaller than max buffer size
    // Set the time stamp and push it to the back
    if (sendQueue.size() <= Latency::MAX_BUFFER_SIZE) {
        // Add the current time and delay 
        long long sendTimeStamp = currentTime + delay;
        sendQueue.push(Message{clientID, message + ":" + to_string(sendTimeStamp), sendTimeStamp});
    }
}

void Latency::receiveNextMessages(long long currentTime, void (*receiveHandler)(int clientID, string message)) {    
    if (receiveQueue.empty())
        return;
        
    while (!receiveQueue.empty()) {
        Message message = receiveQueue.front();
        if (currentTime >= message.timeToProcess) {
            (*receiveHandler)(message.clientID, message.message);
            receiveQueue.pop();
        } else {
            break;
        }
    }
}

void Latency::sendNextMessages(long long currentTime, void (*sendHandler)(int clientID, string message)) {
    if (sendQueue.empty())
        return;
    
    while (!sendQueue.empty()) {
        Message message = sendQueue.front();
        if (currentTime >= sendQueue.front().timeToProcess) {
            (*sendHandler)(message.clientID, message.message);
            sendQueue.pop();
        } else {
            break;
        }
    }
}

void Latency::sendNTP(int clientID) {
    long long currentTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    delaySend(clientID, "NTP:" + to_string(currentTime));
}

void Latency::reset() {
    generator = default_random_engine();
    sendQueue = queue<Message>();
    receiveQueue = queue<Message>();
}

bool Latency::empty() {
    return sendQueue.empty() && receiveQueue.empty();
}
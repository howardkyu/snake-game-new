#include "Latency.h"

using namespace std;
using namespace chrono;

Latency::Latency() {
    this->generator = default_random_engine();
    uniform_int_distribution<int> distribution(
        Latency::MIN_DELAY, Latency::MAX_DELAY);
    sendQueue = queue<Message>();
    receiveQueue = queue<Message>();
}

void Latency::delayReceive(int clientID, string message, long long time) {
    // Generate a distributed random number from MIN_DELAY to MAX_DELAY
    int delay = distribution(generator);

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
    int delay = distribution(generator);   
        
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
    
    while (true) {
        Message message = receiveQueue.front();
        if (currentTime >= receiveQueue.front().timeToProcess) {
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
    
    while (true) {
        Message message = sendQueue.front();
        if (currentTime >= sendQueue.front().timeToProcess) {
            (*sendHandler)(message.clientID, message.message);
            sendQueue.pop();
        } else {
            break;
        }
    }
}

void Latency::reset() {
    this->generator = default_random_engine();
    uniform_int_distribution<int> distribution(
        Latency::MIN_DELAY, Latency::MAX_DELAY);
    sendQueue = queue<Message>();
    receiveQueue = queue<Message>();
}
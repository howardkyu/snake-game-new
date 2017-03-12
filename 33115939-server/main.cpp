#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <sstream>
#include <random>
#include <chrono>
#include <vector>
#include "websocket.h"
#include "GameState.h"
#include "Latency.h"
#include "Parse.h"

using namespace std;
using namespace chrono;

webSocket server;
GameState gamestate;
Latency latency;

void resetGame() {
    gamestate.reset();
    latency.reset();
}

/**
 * Handler function for initial setup for the game.
 */
void handleInit(int clientID, string playerID) {
    if (gamestate.waiting())
        gamestate.addPlayer(clientID, playerID);
    if (!gamestate.waiting()) {
        // Setup game state
        gamestate.setup();

        // Start the game
        gamestate.start();

        // Generate setup message for clients and send without delay
        string setupMessage = gamestate.generateSetupMessage();

        vector<int> clientIDs = server.getClientIDs();
        for (unsigned int i = 0; i < clientIDs.size(); i++) {
            server.wsSend(clientIDs[i], setupMessage);
        }
    }
}

/**
 * Handler function for delayed receiving. Called by latency.
 */
void handleMessage(int clientID, string message) {
    std::vector<std::string> messageVector = Parse::split(message, ':');

    if (messageVector[0] == "UPDATE") {     /* UPDATE:pid;ADD,x,y;ERASE,x,y */
        // Add message to the player's message queue
        gamestate.addMessage(clientID, messageVector[1]);
    } else if (messageVector[0] == "NTP") {
        // Send a delayed NTP to the player
        latency.sendNTP(clientID);
    }   
}

/**
 * Handler function for delayed sending. Called by latency.
 */
void handleSend(int clientID, string message) {
    server.wsSend(clientID, message);
}

/**
 * Called when the client connects
 * When the client tries to establish a connection, check if two players are
 * already connected. If not, send an ACCEPTED message. If so, send a REJECTED
 * message.
 */
void openHandler(int clientID) {
    if (gamestate.waiting()) {
        cout << "Welcome: " << clientID << endl;
        server.wsSend(clientID, "ACCEPTED");
    } else {
        cout << "Reject: " << clientID << endl;
        server.wsSend(clientID, "REJECTED");
        server.wsClose(clientID);
    }
}

/**
 * Called when the client closes the connection
 */
void closeHandler(int clientID) {
    cout << "Disconnect: " << clientID << endl;
    gamestate.removePlayer(clientID);

    if (gamestate.waiting()) {
        gamestate.reset();
        latency.reset();

        vector<int> clientIDs = server.getClientIDs();
        for (int i = 0; i < clientIDs.size(); i++) {
            server.wsClose(clientIDs[i]);
        }
    }
}

/**
 * Called when a message is sent through the socket
 */
void messageHandler(int clientID, string message) {
    cout << "D_RAWRECEIVE: " << message << endl;
    vector<string> messageVector = Parse::split(message, ':');

    if (messageVector[0] == "INIT") {
        handleInit(clientID, messageVector[0]);
    } else {
        string time = messageVector[messageVector.size() - 1];
        latency.delayReceive(clientID, message, stoll(time));
    }
}

void periodicHandler() {
    long long currentTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count(); 

    // Receive any delayed messages if exists
    latency.receiveNextMessages(currentTime, handleMessage);

    // Send any delayed messages if exists
    latency.sendNextMessages(currentTime, handleSend);

    // Generate the next state to send to clients
    std::string message = gamestate.generateNextState();
    while (message != "NULL") {
        // Send the clients the message 
        vector<int> clientIDs = server.getClientIDs();
        for(int i = 0; i < clientIDs.size(); i++) {
            latency.delaySend(clientIDs[i], message);
        }

        // If next state is collided, then reset the game
        if(message == "COLLIDED") {
            gamestate.reset();
            latency.reset();
        }

        // Generate next gamestate
        message = gamestate.generateNextState();
    } 
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    
    // Set port to run the server
    int port;
    cout << "Please set server port: ";
    cin >> port;

    // Set event handlers
    server.setOpenHandler(openHandler);         /* Called upon opening connection */
    server.setCloseHandler(closeHandler);       /* Called upon handling connection */
    server.setMessageHandler(messageHandler);   /* Called upon receiving message */
    server.setPeriodicHandler(periodicHandler); /* Called every once a while */

    // Start server to listen to specified port
    server.startServer(port);

    gamestate = GameState();
    latency = Latency();

    return 1;
}
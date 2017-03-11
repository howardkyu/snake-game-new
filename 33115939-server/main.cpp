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

using namespace std;
using namespace chrono;

webSocket server;
GameState gamestate;
Latency latency;

//
default_random_engine engine;

vector<string> split(string message, char delimiter) {
	vector<string> result = vector<string>();
	stringstream stream(message);
	string token;
	while (getline(stream, token, delimiter)) {
		result.push_back(token);
	}
	return result;
}

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
        // TODO SEND ENDGAME TO CLIENTS
    }
}

void messageHandler(int clientID, string message) {
    cout << "D_RAWRECEIVE: " << message << endl;
    vector<string> messageVector = split(message, ':');

    if (messageVector[0] == "INIT") {
        handleInit(clientID, messageVector[0]);
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

    // Start server to listen to specified port
    server.startServer(port);

    engine = default_random_engine();
    gamestate = GameState();
    latency = Latency();

    return 1;
}
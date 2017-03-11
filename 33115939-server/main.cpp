#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <sstream>
#include <random>
#include <chrono>
#include "websocket.h"
#include "PlayerController.h"

using namespace std;
using namespace chrono;

webSocket server;
PlayerController playerController;

//
default_random_engine engine;

/**
 * Called when the client connects
 * When the client tries to establish a connection, check if two players are
 * already connected. If not, send an ACCEPTED message. If so, send a REJECTED
 * message.
 */
void openHandler(int clientID) {
    if (playerController.size() <= 2) {
        cout << "Welcome: " << clientID << std::endl;
        server.wsSend(clientID, "ACCEPTED");
    } else {
        cout << "Reject: " << clientID << std::endl;
        server.wsSend(clientID, "REJECTED");
        server.wsClose(clientID);
    }
}

/**
 * Called when the client closes the connection
 * Remove the player from the PlayerController
 */
void closeHandler(int clientID) {
    cout << "Disconnect: " << clientID << std::endl;
    playerController.removePlayer(clientID);
    // TODO End game if 
    if (playerController.waiting()) {
        // End game
    }
}

void messageHandler(int clientID, string message) {

}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    engine = default_random_engine();
    playerController = PlayerController();

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

    return 1;
}
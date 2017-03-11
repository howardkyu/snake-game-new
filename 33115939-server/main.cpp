#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <sstream>
#include <random>
#include "websocket.h"
#include "PlayerController.h"

using namespace std;
using namespace chrono;

webSocket server;
PlayerController playerController;

//
default_random_engine engine;

void openHandler(int clientID) {

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
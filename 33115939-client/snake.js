// Kuan Hao Yu     33115939
// Kimberly Nguy   86242309
// Pafoua Fang     84351619
// Kuanping Change 71018021

var CELL_PX = 10;
var MS_PER_FRAME = 50;
var COL = 0;
var ROW = 0;

// Colors for canvas
var BG_COLOR = "white";
var SNAKE_COLOR;
var OPPONENT_SNAKE_COLOR;
var FOOD_COLOR;

var Server;
var gameCanvas;
var scoreCanvas;
var gameCtx;
var scoreCtx;
var connected;

var foodPosition;

var playerID;
var playerDirection;
var playerNumber;
var playerScore = 0;
var playerSnake = [];

var opponentID;
var opponentScore = 0;

var ntpQueue = [];
var ping;

var gameLoop;
var ntpLoop;

var inSession = false;

function receive(message) {
    console.log("D_RECEIVING: " + message);
    var messageList = message.split(":");

    if (messageList[0] === "ACCEPTED") {
        
        sendWithTime("INIT:" + playerID);

    } 
    else if (messageList[0] === "SETUP") {

        // Syntax for the setup message that they player sent:  
        // SETUP:COL:ROW:FOOD_COLOR:pid1,color,R,x,y:pid2,color,L,x,y

        // Parse the message and initialize the variables
        gameSetup(messageList);
    
        // Set the input listener for user keys
        setInputListener();

        runGame();
        gameLoop = setInterval(runGame, 100);

        pollNTP();
        setInterval(pollNTP, 3000); // poll NTP every 3 seconds

    } else if (messageList[0] === "STATE" && inSession) {
        // Syntax for the message that server sends to client to update state:
        // STATE:pid1;ADD,x,y;ERASE,x,y;SCORE,xxx:pid2;ADD,x,y;ERASE,x,y;SCORE,yyy:FOOD,x,y

        var playerOneInfo = messageList[1].split(";");
        var playerTwoInfo = messageList[2].split(";");
        var foodInfo = messageList[3].split(",");

        if (playerOneInfo[0] == playerID) {
            // Parse and update player's info
            parsePlayerInfo(playerOneInfo);
            // Parse opponent's info
            parseOpponentInfo(playerTwoInfo);
        }  
        else {
            parsePlayerInfo(playerTwoInfo);
            parseOpponentInfo(playerOneInfo);
         }

         // Update the food if needed
         var newFoodX = parseInt(foodInfo[1]);
         var newFoodY = parseInt(foodInfo[2]);
         if (foodPosition.x != newFoodX || foodPosition.y != newFoodY) {
            //  erase(foodPosition.x, foodPosition.y);
             foodPosition = {x: newFoodX, y: newFoodY};
             draw(foodPosition.x, foodPosition.y, FOOD_COLOR);
         }

         // Update the score board
         updateScoreBoard();

    } else if (messageList[0] === "COLLIDED") {
        inSession = false;

        // Syntax for collision message is:
        // COLLIDED:pid1;SCORE,xxx:pid2;SCORE,xxx
        clearInterval(gameLoop);
        clearInterval(ntpLoop);

        var playerOneInfo = messageList[1].split(";");
        var playerTwoInfo = messageList[2].split(";");

        if(playerOneInfo[0] == playerID) {
            var playerScore = playerOneInfo[1].split(",")[1];
            var opponentScore = playerTwoInfo[1].split(",")[1];
        }
        else {
            var playerScore = playerTwoInfo[1].split(",")[1];
            var opponentScore = playerOneInfo[1].split(",")[1];
        }

        // Update the score board
        updateScoreBoard();
        displayWinner();

    } else if (messageList[0] === "NTP") {
        console.log(message);

        calculatePing(messageList[1], messageList[2]);
        updatePingText();
    }
}

// Parses the message from the server concerning the player
// and updates the player's score
function parsePlayerInfo(playerInfo) {
    var scoreInfo = playerInfo[playerInfo.length-1].split(",");
    playerScore = scoreInfo[1]; 
}

// Parses the message from the server concerning the opponent
// and updates the opponent's snake and score
function parseOpponentInfo(opponentInfo) {
    // Update the opponent's info
    for(var i = 1; i < opponentInfo.length; i++) {
        var info = opponentInfo[i].split(",");

        switch(info[0]) {
            case("ADD"):
                draw(info[1], info[2], OPPONENT_SNAKE_COLOR);
                break;
            case("ERASE"):
                erase(info[1], info[2]);
                break;
            case("SCORE"):
                opponentScore = info[1];
                break;             
        }
    }
}

// Compares the players' scores to see who is the winner
function comparePlayersScore() {
    if(playerScore > opponentScore) {
        return playerID.toUpperCase() + " WINS!";
    }
    else if (playerScore < opponentScore) {
        return opponentID.toUpperCase() + " WINS!";
    }
    else {
        return "IT'S A TIE!";
    }
}

// Parses the server's message and initializes the variables for the game
function gameSetup(messageList) {
        inSession = true;

        gameCtx = gameCanvas.getContext("2d");
        scoreCtx = scoreCanvas.getContext("2d");

        // Syntax for the setup message that they player sent:  
        // SETUP:COL:ROW:FOOD_COLOR:pid1,color,R,x,y:pid2,color,L,x,y

        // Obtain the row and col for the gameboard
        COL = parseInt(messageList[1]);
        ROW = parseInt(messageList[2]);
        setupCanvas(parseInt(messageList[1]), parseInt(messageList[2]));

        FOOD_COLOR = messageList[3];

        foodPosition = {x: parseInt(messageList[4]), y: parseInt(messageList[5])}

        var playerOneInfo = messageList[6].split(",");
        var playerTwoInfo = messageList[7].split(",");

        // Obtain the player's snake color, direction and the snake's starting coords
        // Obtain the opponent's playerID and starting snake coord
        if(playerOneInfo[0] == playerID) {
            SNAKE_COLOR = playerOneInfo[1];
            playerDirection = playerOneInfo[2];
            playerSnake.push({x:parseInt(playerOneInfo[3]), y:parseInt(playerOneInfo[4])}); 

            opponentID = playerTwoInfo[0];
            OPPONENT_SNAKE_COLOR = playerTwoInfo[1];
            
            // Draw initial opponent snake
            draw(parseInt(playerTwoInfo[3]), parseInt(playerTwoInfo[4]), OPPONENT_SNAKE_COLOR);
        }
        else {
            SNAKE_COLOR = playerTwoInfo[1];
            playerDirection = playerTwoInfo[2];
            playerSnake.push({x: parseInt(playerTwoInfo[3]), y:parseInt(playerTwoInfo[4])});

            opponentID = playerOneInfo[0];
            OPPONENT_SNAKE_COLOR = playerOneInfo[1];

            // Draw initial opponent snake
            draw(parseInt(playerOneInfo[3]), parseInt(playerOneInfo[4]), OPPONENT_SNAKE_COLOR);
        }
        // Draw initial snake
        draw(playerSnake[0].x, playerSnake[0].y, SNAKE_COLOR);

        // Draw initial food
        draw(foodPosition.x, foodPosition.y, FOOD_COLOR);
        
        // Update the score board
        updateScoreBoard();
}

// Sets up an event listener to listen for player's keyboard inputs
// Also creates the scoreboard for the game
function setInputListener() {
    document.addEventListener('keydown', function(event) {
        // Arrow keys
        if (event.keyCode == 38 && playerDirection != 'D') playerDirection = 'U';
        else if (event.keyCode == 37 && playerDirection != 'R') playerDirection = 'L';
        else if (event.keyCode == 40 && playerDirection != 'U') playerDirection = 'D';
        else if (event.keyCode == 39 && playerDirection != 'L') playerDirection = 'R';
    });
}

// Main game loop. Updates the snake position and checks if food is eaten
function runGame() {
    var newX = playerSnake[0].x;
    var newY = playerSnake[0].y;

    if (playerDirection == 'R') newX++;
    else if (playerDirection == 'L') newX--;
    else if (playerDirection == 'U') newY--;
    else if (playerDirection == 'D') newY++;

    if (newX >= COL) newX = 0;
    if (newX < 0) newX = COL-1;
    if (newY >= ROW) newY = 0;
    if (newY < 0) newY = ROW-1;

    // Construct message to send to the server
    var messageToSend = "UPDATE:" + playerID + ";ADD," + newX + "," + newY;

    var head;
    // If food is not eaten, erase the tail
    if (newX != foodPosition.x || newY != foodPosition.y) {     /* if food not eaten */
        var tail = playerSnake.pop()
        erase(tail.x, tail.y);
        messageToSend += ";ERASE," + tail.x + "," + tail.y;
    }
    playerSnake.unshift({x: newX, y: newY});

    // Draw the head 
    draw(newX, newY, SNAKE_COLOR); 

    // Send the move updates to the server
    sendWithTime(messageToSend);    
}

// Connects to the server
function connect() {
    Server = new FancyWebSocket('ws://' + document.getElementById('ip').value + ':' + document.getElementById('port').value);
    
    Server.bind('open', function() {
        // document.getElementById('cnt-btn').disabled = true;

        gameCanvas = document.getElementById("canvas-game");
        scoreCanvas = document.getElementById("canvas-score");

        playerID = document.getElementById('player-id').value;

    });

    Server.bind('close', function(data) {
        document.getElementById('cnt-btn').disabled = false;
        clearInterval(gameLoop);
        clearInterval(ntpLoop);
    });

    Server.bind('message', receive);

    Server.connect();
}

// Draws
function draw(x, y, color) {
   gameCtx.fillStyle = color;
   gameCtx.fillRect(x * CELL_PX, y * CELL_PX, CELL_PX, CELL_PX); 
}


// Draws the cell white to erase the color
function erase(x, y) {
   draw(x,y, BG_COLOR);
}

// Determines which score to update when server sends a message that contains "SCORE1"
function updateScore1(state) {
    if (playerType == "PLAYER1") {
        playerScore = state;
    }
    else {
        opponentScore= state;
    }     
}


// Determines which score to update when server sends a message that contains "SCORE2"
function updateScore2(state) {
    if (playerType == "PLAYER2") {
        playerScore = state;
    }
    else {
        opponentScore = state;
    }     
}

// Updates the canvas size
function setupCanvas(col, row) {
    gameCanvas.height = col * CELL_PX;
    gameCanvas.width = row * CELL_PX;

    scoreCanvas.width = col / 4 * CELL_PX;
    scoreCanvas.width = row * CELL_PX;
}

function updateScoreBoard() {
    // Clear canvas or else fillText will overlap text over and over again
    scoreCtx.clearRect(0,0,scoreCanvas.width,scoreCanvas.height);
    
    scoreCtx.font = "30px Ariel";
    scoreCtx.textAlign = "center";
    scoreCtx.fillText(playerID + "'s Score: " + playerScore, scoreCanvas.width*2/8, scoreCanvas.height/2);
    scoreCtx.fillText(opponentID + "'s Score: " + opponentScore, scoreCanvas.width*6/8, scoreCanvas.height/2);   
}


// Compares the players' scores to see who is the winner
function comparePlayersScore() {
    if(playerScore > opponentScore) {
        return playerID.toUpperCase() + " WINS!";
    }
    else if (playerScore < opponentScore) {
        return opponentID.toUpperCase() + " WINS!";
    }
    else {
        return "IT'S A TIE!";
    }
}

// Displays a text stating who won the game
function displayWinner() {
    gameCtx.clearRect(0,0,gameCanvas.width,gameCanvas.height);
    gameCtx.font = "40px Ariel";
    gameCtx.fillStyle = "red";
    gameCtx.textAlign = "center";
    var winner = comparePlayersScore();
    gameCtx.fillText("GAME OVER: " + winner, gameCanvas.width/2, gameCanvas.height/2);
}

function pollNTP() {
    var time = new Date().getTime();
    Server.send("message", "NTP:" + time);
    ntpQueue.push(time);
}

function calculatePing(initTime, endTime) {
    var clientInitTime = ntpQueue.shift();
    var clientEndTime = new Date().getTime();
    var serverInitTime = parseInt(initTime);
    var serverEndTime = parseInt(endTime);
    ping = (clientEndTime - clientInitTime) - (serverEndTime - serverInitTime);
}

function updatePingText() {
    var pingDiv = document.getElementById("ping");
    pingDiv.innerHTML = "Ping: " + ping + " ms";
}


function sendWithTime(message) {
    console.log("D_SENDING: " + message);
    var time = new Date().getTime();
    Server.send("message", message + ":" + time);
}
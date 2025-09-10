const WebSocket = require('ws');

const port = 8080;
const server = new WebSocket.Server({ port });

const n  = 6;
const r = 20;

let turn = 0;
let erased = 0;

let players = [];
let waitList = [];


function genPlayerId() {
    let vowls = ["a", "e", "i", "o", "u"];
    let time = Date.now().toString().slice(10);

    let result = "";

    for (let i=0; i<6; i++) {
        result += vowls[Math.floor(Math.random() * vowls.length)]
        result += time[i % time.length];
    }
    
    return result;
}

function remove(player, array) {
    return array.filter(item => item !== player);
}

function isToPlay() {
    let play = players.length == 2;

    broadcast({
        type: "can_play",
        message: play,
    });

    return play;
}

function broadcast(data) {
    server.clients.forEach(client => {
        if (client.readyState === WebSocket.OPEN) {
            client.send(JSON.stringify(data));
        }
    });
}

function initDots() {
    let ans = [];

    let c = 0;
    for (let i = 0; i < n; i++) {
        for (let j = 0; j < (n - i); j++) {

            let x = 40 + (2 * r + 20) * j;
            let y = 85 + (2 * r + 10) * i;
            
            ans.push([x, y, false, c, false]);
            c++;
        }
    }

    return ans;
}

function updateGameState(pairIndex, currentGameState) {
    if (pairIndex.length != 2)  {
        return undefined;
    }

    let i1 = pairIndex[0];
    let i2 = pairIndex[1];

    if (currentGameState[i1][1] != currentGameState[i2][1]) {
        return currentGameState;
    }

    let newGameState = currentGameState;

    if (i1 < i2) {
        for (let i = i1; i <= i2; i++) {
            if (newGameState[i][2]) break;
            newGameState[i][2] = true;
            erased++;
        }
    }

    else {
        for (let i = i1; i >= i2; i--) {
            if (newGameState[i][2]) break;
            newGameState[i][2] = true;
            erased++;
        }
    }

    return newGameState;
}

function handlePlayer(playerId, ws) {
    let dots = initDots();
    let pairIndex = [];
    let play = false;

    ws.send(JSON.stringify({
        type: 'init',
        playerId: playerId,
        whoMovesFirst: players[0],
        gameState: dots,
    }));

    play = isToPlay();

    ws.onmessage = function(event) {
        const data = JSON.parse(event.data);

        play = isToPlay();

        if (data.type == "update" 
            && !waitList.includes(data.playerId) 
            && play
            && data.playerId == players[turn % 2]) {

            pairIndex.push(data.dotIndex);

            let newGameState = updateGameState(pairIndex, data.gameState);

            if (newGameState != undefined) {
                broadcast({
                    type: "updated",
                    playerId: players[(turn+1) % 2],
                    gameState: newGameState,
                });

                if (erased == 21) {
                    broadcast({
                        type: "end",
                        whoWon: players[(turn-1) % 2],
                    });

                    waitList = remove(waitList[0], waitList);
                    waitList = remove(waitList[1], waitList);

                    players = [];
                    erased = 0;
                    turn = -1;
                }

                turn++;
                pairIndex = [];
            }
        }
    }
}

server.on('connection', function connection(ws) { 
    let playerId = genPlayerId();

    if (players.length+1 > 2) {
        console.log(playerId + " is waiting");

        ws.send(JSON.stringify({
            type: "overflow",
            message: "Two users are already playing! Refresh the page until you get connected",
        }));

        if (!waitList.includes(playerId)) {
            waitList.push(playerId);
        }
    }

    else {
        console.log(playerId + " has connected!");
        players.push(playerId);

        handlePlayer(playerId, ws);
    }    

    ws.on('close', function() {
        console.log(`${playerId} has disconnected`);
        players = remove(playerId, players);
    });
});

console.log("WebSocket server running on port " + port);

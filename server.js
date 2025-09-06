const port = 8080;

const WebSocket = require('ws');
const server = new WebSocket.Server({ port });

const n = 6;
const r = 20;

let players = [];
let playerCount = 0;

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
        }
    }

    else {
        for (let i = i1; i >= i2; i--) {
            if (newGameState[i][2]) break;
            newGameState[i][2] = true;
        }
    }

    return newGameState;
}

function broadcast(data) {
    server.clients.forEach(client => {
        if (client.readyState === WebSocket.OPEN) {
            client.send(JSON.stringify(data));
        }
    });
}

server.on('connection', function connection(ws) {
    playerCount++;

    let playerId = "player" + playerCount;
    console.log(playerId + " has connected!");

    players.push(playerId);

    if (players.length >= 2) {
        ws.send(JSON.stringify({
            type: "overflow",
            message: "Two users are already playing!",
        }));
    }
    
    let dots = initDots();
    let pairIndex = [];
    let turn = 0;

    broadcast({
        type: 'init',
        playerId: playerId,
        gameState: dots,
    });

    // console.log(players[turn % 2]);
    console.log(players);

    ws.onmessage = function(event) {
        const data = JSON.parse(event.data);

        if (data.type == "update" && data.playerId == players[turn % 2]) {
        // if (data.type == "update") {
            pairIndex.push(data.dotIndex);

            let newGameState = updateGameState(pairIndex, data.gameState);

            if (newGameState != undefined) {
                broadcast({
                    type: "updated",
                    turn: players[turn % 2],
                    gameState: newGameState,
                });

                pairIndex = [];
                turn++;
            }
        }
    }

    ws.on('close', function() {
        console.log(`${playerId} has disconnected`);
        const playerIndex = playerId.length - 1;

        players.splice(playerIndex, 1);
        playerCount--;

        console.log(players);
    });
});

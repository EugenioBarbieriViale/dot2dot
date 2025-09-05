// replace with real url
const ws = new WebSocket('ws://localhost:8080');

canvas = document.getElementById("game");
canvas.style.background = "white";


let w = 375;
let h = 375;


const game = new Game(canvas, 6, 20, w, h);
game.run();

const gameState = game.dots;

ws.onopen = function(event) {
    console.log("You are connected to the server");
};

ws.onmessage = function(event) {
    const data = JSON.parse(event.data);
    console.log(data);

    ws.send(JSON.stringify({
        type: "gameState",
        data: gameState,
    }));
}

ws.onclose = function(event) {
    console.log("You are disconnected from the server");
};

ws.onerror = function(event) {
    console.log("Server is down");
}


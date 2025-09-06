// replace with real url
const ws = new WebSocket('ws://localhost:8080');

canvas = document.getElementById("game");
canvas.style.background = "white";


const game = new Game(canvas, 6, 20, 375, 375);
game.run();

const gameState = game.dots;

canvas.addEventListener("mousedown", () => {
    if (game.temp % 2 == 0) {
        ws.send(JSON.stringify({
            type: "gameState",
            data: gameState,
        }));
    }
});

ws.onopen = function(event) {
    console.log("You are connected to the server");
};

ws.onmessage = function(event) {
    const data = JSON.parse(event.data);
    console.log(data);
    // game.dots = data;
}

ws.onclose = function(event) {
    console.log("You are disconnected from the server");
};

ws.onerror = function(event) {
    console.log("Server is down");
}

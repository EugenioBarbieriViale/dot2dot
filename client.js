// const ws = new WebSocket('ws://localhost:8080');
// const ws = new WebSocket("https://858650045256.ngrok-free.app");
const ws = new WebSocket("wss://858650045256.ngrok-free.app");

canvas = document.getElementById("game");
canvas.style.background = "white";

ctx = canvas.getContext("2d");
ctx.font = "30px Helvetica";

const r = 20;
const w = 375;
const h = 375;

let myId;
let dots = [];


function isToSend(dots, x, y) {
    for (let i=0; i<dots.length; i++) {
        let dot = dots[i];

        const dx = x - dot[0];
        const dy = y - dot[1];

        const dist = Math.sqrt(dx * dx + dy * dy);

        if (dist < r) {
            return dot[3];
        }
    }

    return null;
}

function displayGame(dots, end) {
    ctx.clearRect(0, 0, w, h);

    for (let i=0; i<dots.length; i++) {
        let dot = dots[i];

        if (!end) {
            if (!dot[2] && !dot[4]) {
                ctx.fillStyle = "black";
            }
            else if (dot[2]) {
                ctx.fillStyle = "white";
            }
            else {
                ctx.fillStyle = "grey";
            }

            ctx.beginPath();
            ctx.arc(dot[0], dot[1], r, 0, Math.PI * 2);
            ctx.fill();
        }
    }
}

function displayText(playerId) {
    ctx.fillText(playerId, 130, 38);
    ctx.fillText("dot2dot", 260, 350);
}

ws.onopen = function(event) {
    console.log("You are connected to the server");
};

ws.onmessage = function(event) {
    const data = JSON.parse(event.data);

    if (data.type == "init") {
        myId = data.playerId; 
        alert("You are " + myId);

        dots = data.gameState;
    }

    else if (data.type == "updated" && data.gameState != undefined) {
        dots = data.gameState;
    }

    displayGame(dots, false, r);

    if (data.turn != undefined) {
        displayText(data.turn);
    } else {
        displayText(myId);
    }
};

canvas.addEventListener("mousedown", (event) => {
    const rect = canvas.getBoundingClientRect();
    const mouseX = event.pageX - rect.left;
    const mouseY = event.pageY - rect.top;

    let index = isToSend(dots, mouseX, mouseY, r);

    if (index != null) {
        ws.send(JSON.stringify({
            type: "update",
            playerId: myId,
            dotIndex: index,
            gameState: dots,
        }));
    }
});

ws.onclose = function(event) {
    console.log("You are disconnected from the server");
};

ws.onerror = function(event) {
    console.log("Server is down");
}

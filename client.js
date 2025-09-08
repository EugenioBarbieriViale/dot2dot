// const ws = new WebSocket('ws://localhost:8080');
const ws = new WebSocket("wss://62750262a593.ngrok-free.app");

canvas = document.getElementById("game");
canvas.style.background = "white";

ctx = canvas.getContext("2d");
ctx.font = "30px Helvetica";

const r = 20;
const w = 375;
const h = 375;

let dots = [];

let myId;
let play;

let iStart;
let updateSent = false;

let winner;


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
    ctx.save();
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
    ctx.restore();
}

function displayText(isMyTurn) {
    if (winner == undefined) {
        if (play) {
            switch (iStart) {
                case 0:
                    ctx.fillText("you start", 130, 38);
                    break;
                case 1:
                    ctx.fillText("your opponent starts", 53, 38);
                    break;
                case 2:
                    if (isMyTurn) {
                        ctx.fillText("your turn", 130, 38);
                    } else {
                        ctx.fillText("your opponent's turn", 53, 38);
                    }
                    break;
            }
        }

        else {
            ctx.fillText("waiting for an opponent", 43, 38);
        }
    }

    else {
        if (myId == winner) {
            ctx.fillText("you won", 130, 38);
        }
        else {
            ctx.fillText("you lost", 130, 38);
        }
    }

    ctx.fillText("dot2dot", 260, 350);
}

ws.onopen = function(event) {
    console.log("You are connected to the server");
};

ws.onmessage = function(event) {
    const data = JSON.parse(event.data);
    let isMyTurn = false;

    if (data.type == "init") {
        myId = data.playerId; 
        // alert("You are " + myId);

        dots = data.gameState;
        iStart = ((myId == data.whoMovesFirst) ? 0 : 1);
    }

    else if (data.type == "can_play") {
        play = data.message;         
    }

    else if (data.type == "overflow") {
        alert(data.message);
    }

    else if (data.type == "updated" && data.gameState != undefined) {
        dots = data.gameState;
        isMyTurn = (myId == data.playerId);
    }

    else if (data.type == "end") {
        winner = data.whoWon;
    }

    if (data.playerId == undefined) isMyTurn = true;

    displayGame(dots, winner == "", r);
    displayText(isMyTurn);

    if (play) iStart = 2;
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

        updateSent = true;
    }
});

ws.onclose = function(event) {
    console.log("You are disconnected from the server");
};

ws.onerror = function(event) {
    console.log("Server is down");
}

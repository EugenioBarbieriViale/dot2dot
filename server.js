const port = 8080;

const WebSocket = require('ws');
const wss = new WebSocket.Server({ port });

function broadcast(data) {
    wss.clients.forEach(client => {
        if (client.readyState === WebSocket.OPEN) {
            client.send(JSON.stringify(data));
        }
    });
}

wss.on('connection', function connection(ws) {
    broadcast({
        type: "init",
        data: "connection established",
    });

    ws.onmessage = function(event) {
        const data = JSON.parse(event.data);
        console.log(data);
    }
});

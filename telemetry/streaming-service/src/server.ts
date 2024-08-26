import net from "net";
import { WebSocket, WebSocketServer } from "ws";
import { tempExceed } from "./exceptions";

export interface VehicleData {
  battery_temperature: number;
  timestamp: number;
}

const TCP_PORT = 12000;
const WS_PORT = 8080;
const tcpServer = net.createServer();
const websocketServer = new WebSocketServer({ port: WS_PORT });

tcpServer.on("connection", (socket) => {
  console.log("TCP client connected");

  let tempExceedRecords: number[] = [];

  socket.on("data", (msg) => {
    console.log(`Received: ${msg.toString()}`);

    const jsonData: VehicleData = JSON.parse(msg.toString());

    // Send JSON over WS to frontend clients
    websocketServer.clients.forEach(function each(client) {
      if (client.readyState === WebSocket.OPEN) {
        let isError = tempExceed(jsonData, tempExceedRecords);

        if (isError) {
          console.log(`Current time: ${Date.now()}`);
          console.log("Received battery temperature exceeds more than 3 times within 5 seconds");
        } else {
          client.send(msg.toString());
        }
      }
    });
  });

  socket.on("end", () => {
    console.log("Closing connection with the TCP client");
  });

  socket.on("error", (err) => {
    console.log("TCP client error: ", err);
  });
});

websocketServer.on("listening", () =>
  console.log(`Websocket server started on port ${WS_PORT}`)
);

websocketServer.on("connection", async (ws: WebSocket) => {
  console.log("Frontend websocket client connected");
  ws.on("error", console.error);
});

tcpServer.listen(TCP_PORT, () => {
  console.log(`TCP server listening on port ${TCP_PORT}`);
});

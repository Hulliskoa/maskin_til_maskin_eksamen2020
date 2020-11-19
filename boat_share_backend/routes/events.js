const express = require('express');
const router = express.Router();
let clients = [];
let nests = [];
const db = require("../database/db")
let data = []
router.get("/", (req, res) => {

    const headers = {
        'Content-Type': 'text/event-stream',
        'Connection': 'keep-alive',
        'Cache-Control': 'no-cache'
    };
    res.writeHead(200, headers);


    let eventInterval = setInterval(() => {
        db.updateFromDB().then((r) => console.log(r))
        data = `data: ${JSON.stringify(db.getAll())}\n\n`;
        res.write(`event: message\n`)
        res.write(data)
    }, 5000);

    // Generate an id based on timestamp and save res
    // object of client connection on clients list
    // Later we'll iterate it and send updates to each client
    const clientId = Date.now();
    const newClient = {
        id: clientId,
        res
    };
    clients.push(newClient);
    // When client closes connection we update the clients list
    // avoiding the disconnected one
    req.on('close', () => {
        console.log(`${clientId} Connection closed`);
        clients = clients.filter(c => c.id !== clientId);
    });
});


module.exports = router;

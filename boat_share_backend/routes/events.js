var express = require('express');
var router = express.Router();

let clients = [];


router.get("/", (req, res) => {
    res.set({
        "Content-Type": "text/event-stream",
        "Cache-Control": "no-cache",
        Connection: "keep-alive",

        // enabling CORS
        "Access-Control-Allow-Origin": "*",
        "Access-Control-Allow-Headers":
            "Origin, X-Requested-With, Content-Type, Accept",
    });

    let eventInterval = setInterval(() => {
        res.write(`event: message\n`);
        res.write(`data: {a:"TEST"}}\n\n`);
    }, 2000);

    req.on("close", (err) => {
        clearInterval(eventInterval);
        res.end();
    });
});

function sendToAll(stream) {
    clients.forEach(c => c.res.write(`data: ${JSON.stringify(stream)}\n\n`))
}

module.exports = router;

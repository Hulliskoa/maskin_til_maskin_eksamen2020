const db = require("../database/db")


let interval;
module.exports = function (io) {
    io.on("connection", socket => {
        console.log("user connected");
        let dbResult = []
        db.updateFromDB().then((r) => dbResult = r)

        if (interval) {
            clearInterval(interval);
        }
        interval = setInterval(() => {
            db.updateFromDB().then((r) => dbResult = r)
            socket.emit("FromAPI", dbResult);
        }, 2000);
        socket.on("disconnect", () => {
            console.log("Client disconnected");
            clearInterval(interval);
        });
    });
};



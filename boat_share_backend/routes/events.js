const db = require("../database/db")
const devices = require("../routes/devices")


let interval;
let interval2
let lastTime = 0;
let lastValue = false;

//Handles all sockets between server and client
module.exports = function (io) {
    io.on("connection", socket => {
        console.log("user connected");
        let dbResult = []
        db.updateFromDB().then((r) => dbResult = r)

        if (interval) {
            clearInterval(interval);
        }
        //sends all markers in the database to the connected client
        interval = setInterval(() => {
            db.updateFromDB().then((r) => dbResult = r)
            socket.emit("FromAPI", dbResult);

        }, 2000);

        interval2 = setInterval(() => {
            //Checks if button can be clicked again and sends boolean value to frontend
            if (lastValue === devices.getCantClick()) {
                if (((new Date() - lastTime) / 600) > 2) {
                    devices.setCantClick(false)
                    lastTime = new Date()
                }
            } else {
                lastTime = new Date()
            }

            lastValue = devices.getCantClick()

            socket.emit("CanReserve", devices.getCantClick());
        }, 1000);
        socket.on("disconnect", () => {
            console.log("Client disconnected");
            clearInterval(interval)
            clearInterval(interval2);
        });
    });
};



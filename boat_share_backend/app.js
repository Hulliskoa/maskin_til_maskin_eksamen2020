const express = require('express');
const path = require('path');
const cookieParser = require('cookie-parser');
const logger = require('morgan');
const cors = require('cors')
const indexRouter = require('./routes/index');
const usersRouter = require('./routes/users');
const eventsRouter = require('./routes/events');
const mqttClient = require('./mqtt/mqtt_backend')
require('dotenv').config()

mqttClient.connectToMqtt()

const app = express();
const getData = mqttClient.getData;



app.use(cors())
app.use(logger('dev'));
app.use(express.json());
app.use(express.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));
app.use(express.json());



//app.use('/events', eventsRouter);

app.get("/events", (req, res) => {
    res.set({
        "Content-Type": "text/event-stream",
        "Cache-Control": "no-cache",
        Connection: "keep-alive",

        // enabling CORS
        'Access-Control-Allow-Credentials': true,
        "Access-Control-Allow-Origin": "*",
        "Access-Control-Allow-Headers":
            "Origin, X-Requested-With, Content-Type, Accept",
    });

    let eventInterval = setInterval(() => {
        res.write(`event: message\n`);
        res.write(`data: ${JSON.stringify(getData())}\n\n`);
        console.log(JSON.stringify(getData()+ "\n\n"))
    }, 20000);

    req.on("close", (err) => {
        clearInterval(eventInterval);
        res.end();
    });
});

app.use('/api', indexRouter);
app.use('/users', usersRouter);

module.exports = app;

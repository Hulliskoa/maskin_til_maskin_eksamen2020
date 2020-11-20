
const path = require('path');
const cookieParser = require('cookie-parser');
const logger = require('morgan');
const cors = require('cors')
const indexRouter = require('./routes/index');
const usersRouter = require('./routes/devices');
const eventsRouter = require('./routes/events');
const mqttClient = require('./mqtt/mqtt_backend')
const express = require('express');

require('dotenv').config()
const app = express()
mqttClient.connectToMqtt()



app.use(cors())
app.use(logger('dev'));
app.use(express.json());
app.use(express.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));
app.use(express.json());

app.use('/api/events', eventsRouter);
//app.use('/api', indexRouter);
app.use('/api/devices', usersRouter);

module.exports = app;

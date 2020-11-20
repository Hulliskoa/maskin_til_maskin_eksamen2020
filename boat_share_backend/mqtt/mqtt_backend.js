const mqtt = require('mqtt')
const db = require("../database/db")


const options = {
    port: 15488,
    clientId: 'mqttjs_' + Math.random().toString(16).substr(2, 8),
    username: 'hwxtryvm',
    password: 'Bga5mnvq1sxC',
    clean: true
};
const client = mqtt.connect('mqtt://hairdresser.cloudmqtt.com', options)

client.subscribe('/addClient', function () {
    console.log("subscribed to /addClient")
    // when a message arrives, do something with it
    client.on('message', function (topic, message) {
        if (message) {
            try {
                let jsonMessage = JSON.parse(message)
                db.findOrCreate(jsonMessage.id, jsonMessage.lat, jsonMessage.lng)
            } catch (e) {
            }
        }
    });
})

client.subscribe('/connected', function () {
    console.log("subscribed to /connected")
    // when a message arrives, do something with it
    client.on('message', function (topic, message) {
        console.log("Received '" + message + "' on '" + topic + "'");
    });
});

client.subscribe('/devicePosition', function () {
    console.log("subscribed to /devicePosition")
    // when a message arrives, do something with it
    client.on('message', function (topic, message) {
        if (message) {
            try {
                let jsonMessage = JSON.parse(message)
                db.findOrCreate(jsonMessage.id, jsonMessage.lat, jsonMessage.lng)
            } catch (e) {
            }
        }
    });
})

client.on('connect', function () { // When connected
    console.log('connected');
});


exports.client = client;
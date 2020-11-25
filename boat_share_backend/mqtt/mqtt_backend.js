const mqtt = require('mqtt')
const db = require("../database/db")

// all mqtt subscriptions and publishing is handled here
const options = {
    port: 15488,
    clientId: 'mqttjs_' + Math.random().toString(16).substr(2, 8),
    username: 'hwxtryvm',
    password: 'Bga5mnvq1sxC',
    clean: true
};
const client = mqtt.connect('mqtt://hairdresser.cloudmqtt.com', options)

client.subscribe('/clientStatus', function () {
    console.log("subscribed to /clientStatus")
    client.on('message', function (topic, message) {
        if (message) {
            try {
                let jsonMessage = JSON.parse(message)
                db.findOrCreate(jsonMessage.id, jsonMessage.lat, jsonMessage.lng, jsonMessage.connectionStatus)
            } catch (e) {
            }
        }
    });
})

client.subscribe('/connected', function () {
    console.log("subscribed to /connected")
    client.on('message', function (topic, message) {
        console.log("Received '" + message + "' on '" + topic + "'");
    });
});

client.subscribe('/devicePosition', function () {
    console.log("subscribed to /devicePosition")
    client.on('message', function (topic, message) {
        if (message) {
            try {
                console.log(message)
                let jsonMessage = JSON.parse(message)
                console.log(jsonMessage.connectionStatus)
                db.findOrCreate(jsonMessage.id, jsonMessage.lat, jsonMessage.lng, jsonMessage.connectionStatus)
            } catch (e) {
            }
        }
    });
})

client.on('connect', function () { // When connected
    console.log('connected');
});


exports.client = client;
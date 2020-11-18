const mqtt = require('mqtt')

//TODO: MongoDB
const boatsPositions = [
    {id: "2", lon: "111", lat: "111"}
];

const getData = () => {
    return boatsPositions
}
const connectToMqtt = () => {
const options = {
    port: 15488,
    clientId: 'mqttjs_' + Math.random().toString(16).substr(2, 8),
    username: 'hwxtryvm',
    password: 'Bga5mnvq1sxC',
    clean:true
};
const client  = mqtt.connect('mqtt://hairdresser.cloudmqtt.com', options)

client.subscribe('/addClient', function() {
    console.log("subscribed to /addClient")
    // when a message arrives, do something with it
    client.on('message', function(topic, message, packet) {
        boatsPositions.push(JSON.parse(message));
        console.log("Received '" + message + "' on '" + topic + "'");
    });
})

client.subscribe('/connected', function() {
    console.log("subscribed to /connected")
    // when a message arrives, do something with it
    client.on('message', function(topic, message, packet) {
        console.log("Received '" + message + "' on '" + topic + "'");
    });
});


client.on('connect', function() { // When connected
    console.log('connected');

});


}
exports.getData = getData;
exports.connectToMqtt = connectToMqtt;
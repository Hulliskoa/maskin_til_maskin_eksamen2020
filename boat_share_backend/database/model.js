const mongoose = require("mongoose");

const Device = mongoose.Schema;

// database model for the device object
let device = new Device(
    {
        _id: String,
        lat: String,
        lng: String,
        connectionStatus: String,
        owner: {type: String, default: "unkown"},
        currentUser: {type: String, default: "unkown"},
        rentable: {type: Boolean},
        lastupdated: {type: Date, default: Date.now}
    },
    {collection: "Devices"}
);

module.exports = mongoose.model("devices", device);
const mongoose = require("mongoose");

const Device = mongoose.Schema;

let device = new Device(
    {
        _id: [Number],
        lat: String,
        lng: String,
        lastupdated: {type: Date, default: Date.now}
    },
    {collection: "Devices"}
);

module.exports = mongoose.model("devices", device);
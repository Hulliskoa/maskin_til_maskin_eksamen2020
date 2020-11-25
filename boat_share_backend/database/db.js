const device = require("./model");
const mongoose = require("mongoose");
let uri = "mongodb://localhost:27017/boatshare";


// Handles all database queries

let currentDB = [];
mongoose.connect(uri, {
    useUnifiedTopology: true,
    useNewUrlParser: true,
    useFindAndModify: false
}).then(() => console.log('DB Connection Successfull'))
    .catch((err) => {
        console.error(err);
    });

const connection = mongoose.connection;
connection.once("open", function () {
    console.log("MongoDB database connection established successfully");
});

function findOrCreate(id, lat, lng, connectionStatus) {
    let conditions = {_id: id}
    let update = {lat: lat, lng: lng, connectionStatus: connectionStatus}
    let options = {upsert: true};
    device.findOneAndUpdate(conditions, update, options, function (error, result) {
        if (error) return;
        console.log(result)
    });
}
//saves status for boat locked/unlocked
function updateBoatStatus(id, boatStatus) {
    let conditions = {_id: id}
    let update = {rentable: boatStatus}
    let options = {upsert: true};
    device.findOneAndUpdate(conditions, update, options, function (error, result) {
        if (error) return;
        console.log(result)
    });
}

const getAll = () => {
    return currentDB
}

const updateFromDB = async () => {
    currentDB = await device.find()
    return currentDB
}

const getAllDevices = function (callback) {
    device.find().exec(function (err, docs) {
        // If there is an error, return the error and no results
        if (err) return callback(err, null)
        // No error, return the docs
        callback(null, docs)
    });
}
exports.updateFromDB = updateFromDB
exports.findOrCreate = findOrCreate
exports.getAllDevices = getAllDevices
exports.getAll = getAll
exports.updateBoatStatus = updateBoatStatus


const express = require('express');
const router = express.Router();
const devices = require("../database/model")
const mqtt = require('../mqtt/mqtt_backend')
const bodyParser = require('body-parser');

router.use(bodyParser.urlencoded({ extended: true }));
/* GET users listing. */
router.get('/', async (req, res, next) => {
    devices.find({},function(err, listing) {
        if (err) {
            res.status(404).send(err);
        } else {
            console.log(listing)
            res.json(listing);
        }
    })

});

const options = {
    qos: 1,
    rap: 1

};

router.post('/', (req, res, next) => {
    console.log('Got body:', req.body.id);
    let clientID = (req.body.id).toString()
    mqtt.client.publish("/openDevice/" + clientID , (req.body.open).toString(),options)
    res.send('POST request to the homepage')

});


module.exports = router;

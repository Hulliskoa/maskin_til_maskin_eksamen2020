const express = require('express');
const router = express.Router();
const devices = require("../database/model")
const mqtt = require('../mqtt/mqtt_backend')
const bodyParser = require('body-parser');
const db = require("../database/db")
let cantClick = false;

router.use(bodyParser.urlencoded({extended: true}));
/* GET users listing. */
router.get('/', async (req, res, next) => {
    devices.find({}, function (err, listing) {
        if (err) {
            res.status(404).send(err);
        } else {
            console.log(listing)
            res.json(listing);
        }
    })

});



router.post('/', (req, res, next) => {


    const options = {
        qos: 1,
        retain : true
    };
    if (!cantClick) {
        // console.log('Got body:', req.body.id);
        let clientID = (req.body.id).toString()
        db.updateBoatStatus(clientID, req.body.open)
        mqtt.client.publish("/openDevice/" + clientID, (req.body.open).toString(), options)
        cantClick = true;
        res.write("Post request sent")
        res.end();
    }else{
        res.write("HELLO")
        res.end();
    }

});


function getCantClick(){
    return cantClick
}

function setCantClick(setCantClick){
    cantClick = setCantClick
}


module.exports = router;
module.exports.getCantClick = getCantClick;
module.exports.setCantClick = setCantClick;
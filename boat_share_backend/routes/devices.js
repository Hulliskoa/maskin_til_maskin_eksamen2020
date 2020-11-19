const express = require('express');
const router = express.Router();
const devices = require("../database/model")

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


module.exports = router;

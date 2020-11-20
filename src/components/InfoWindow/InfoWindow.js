import React, {useState} from "react";

const InfoWindow = (props) => {
    const {place, onCloseClick, lat, lng} = props;
    const [isReserved, setReserved] = useState(false)
    const infoWindowStyle = {
        position: 'relative',
        transformOrigin: 'bottom left',
        borderRadius: 20,
        width: 220,
        color: 'black',
        backgroundColor: 'white',
        boxShadow: '0 2px 7px 1px rgba(0, 0, 0, 0.3)',
        padding: 10,
        fontSize: 14,
        zIndex: 100,
    };

    function openBoat(openOrClose) {
        fetch(process.env.REACT_APP_API_URL + '/api/devices', {
            method: 'post',
            headers: {'Content-Type': 'application/json'},
            body: JSON.stringify({id: place._id[0], open: openOrClose})
        }).then(r => console.log(r));
    }

    if (isReserved) {

        return (
            <div style={infoWindowStyle}>
                <div onClick={onCloseClick} style={{float: "right", cursor: 'pointer', fontWeight: 'bold'}}>X</div>
                <div style={{fontSize: 16}}>
                    <p>BOAT ID: {place._id}</p><b></b>
                    <p>Latitude: {lat} <b/> Longitude: {lng}</p>
                    <button onClick={() => {
                        openBoat(0)
                        setReserved(false)
                    }}>UNRESERVE BOAT
                    </button>
                </div>
            </div>)
    } else {

        return (
            <div style={infoWindowStyle}>
                <div onClick={onCloseClick} style={{float: "right", cursor: 'pointer', fontWeight: 'bold'}}>X</div>
                <div style={{fontSize: 16}}>
                    <p>BOAT ID: {place._id}</p><b></b>
                    <p>Latitude: {lat} <b/> Longitude: {lng}</p>
                    <button onClick={() => {
                        openBoat(1)
                        setReserved(true)
                    }}>RESERVE BOAT
                    </button>
                </div>
            </div>)
    }


};

export default InfoWindow
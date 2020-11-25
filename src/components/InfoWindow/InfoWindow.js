import React, {useEffect, useState} from "react";
import openSocket from "socket.io-client";
import Loader from 'react-loader-spinner'
import "react-loader-spinner/dist/loader/css/react-spinner-loader.css"

//Infowindow which show when map marker is clicked
const InfoWindow = (props) => {
    const {place, onCloseClick, lat, lng} = props;
    const [isReserved, setReserved] = useState(false)
    const [cantSend, setCantSend] = useState(true)
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
    //used when Reserve/Unreserve is clicked
    function openBoat(openOrClose) {

        fetch(process.env.REACT_APP_API_URL + '/api/devices', {
            method: 'post',
            headers: {'Content-Type': 'application/json'},
            body: JSON.stringify({id: place._id, open: openOrClose})
        }).then(response => console.log(response))
    }

    //Websocket listener used for showing loading dots when button is clicked
    useEffect(() => {
        const socket = openSocket(process.env.REACT_APP_SOCKET_URL);
        socket.on("CanReserve", data => {

            setCantSend(data);
        });
    }, []);

    //trigger rerender when cantSend status changes. Used to check if reserve/unreserve button can be clicked
    useEffect(() => {
        setReserved(place.rentable);
    }, []);

    //returns different values based on the condition the map marker is in
    if (!cantSend) {
        if (isReserved) {
            return (
                <div style={infoWindowStyle}>
                    <div onClick={onCloseClick} style={{float: "right", cursor: 'pointer', fontWeight: 'bold'}}>X</div>
                    <div style={{fontSize: 16}}>
                        <p>BOAT ID: {place._id}</p><b></b>
                        <p>Latitude: {lat} <b/> Longitude: {lng}</p>
                        {!cantSend}{}
                        <button disabled={cantSend} onClick={() => {
                            openBoat(0)
                            setReserved(false)
                            setCantSend(true)
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
                        <button disabled={cantSend} onClick={() => {
                            openBoat(1)
                            setReserved(true)
                            setCantSend(true)
                        }}>RESERVE BOAT
                        </button>
                    </div>
                </div>)
        }
    } else {
        return (
            <div style={infoWindowStyle}>
                <div onClick={onCloseClick} style={{float: "right", cursor: 'pointer', fontWeight: 'bold'}}>X</div>
                <div style={{fontSize: 16}}>
                    <p>BOAT ID: {place._id}</p><b></b>
                    <p>Latitude: {lat} <b/> Longitude: {lng}</p>
                    <Loader
                        type="ThreeDots"
                        color="#00BFFF"
                        height={20}
                        width={20}
                        timeout={0}
                    />
                </div>
            </div>
        )


    }
};

export default InfoWindow
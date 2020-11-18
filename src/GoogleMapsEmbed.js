import { GoogleMap, LoadScript } from '@react-google-maps/api';
import React from 'react';
import Marker from './Marker';

const apikey = process.env.REACT_APP_GOOGLE_API
const MapContainer = () => {


    let defaultCenter = {
        lat: 41.3851, lng: 2.1734
    }

    if ("geolocation" in navigator) {
        console.log("Available");
         navigator.geolocation.getCurrentPosition(function(position) {
            console.log("Latitude is :", position.coords.latitude);
            defaultCenter.lat = position.coords.latitude
            console.log("Longitude is :", position.coords.longitude);
            defaultCenter.lng = position.coords.longitude
        });
    } else {
        console.log("Not Available");
    }

    const mapStyles = {
        height: "50vh",
        width: "90vw",
        zIndex: 0};



    return (
        <LoadScript
            googleMapsApiKey={apikey}>
            <GoogleMap
                mapContainerStyle={mapStyles}
                zoom={13}
                center={defaultCenter}
            />
            <Marker
                lat={59.923353999999996}
                lng={10.7706283}
                name="My Marker"
                color="blue"
            />
        </LoadScript>
    )
}

export default MapContainer;
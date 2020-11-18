import GoogleMapReact from 'google-map-react';
import React from 'react';

let coordinates = {
    lat: 59.9233259, lng: 10.7705574
}

const AnyReactComponent = ({ text }) => <div>{text}</div>;
const MapContainer = () => {




    if ("geolocation" in navigator) {
        console.log("Available");
         navigator.geolocation.getCurrentPosition(function(position) {
            console.log("Latitude is :", position.coords.latitude);
             coordinates.lat = position.coords.latitude
            console.log("Longitude is :", position.coords.longitude);
             coordinates.lng = position.coords.longitude
        });
    } else {
        console.log("Geolocation Not Available");
    }

    const mapStyles = {
        height: "50vh",
        width: "90vw",
        zIndex: 0};



    return (
    <div style={mapStyles}>
        <GoogleMapReact
            bootstrapURLKeys={{key:process.env.REACT_APP_GOOGLE_API}}
            defaultCenter={coordinates}
            defaultZoom={11}
        >
            <AnyReactComponent
                lat={59.955413}
                lng={30.337844}
                text="My Marker"
            />
        </GoogleMapReact>
    </div>
    )
}

export default MapContainer;
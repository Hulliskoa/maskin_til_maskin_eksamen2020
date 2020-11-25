import React, {useState, useEffect} from 'react';
import GoogleMapReact from 'google-map-react';
import Marker from '../Marker/Marker';
import openSocket from "socket.io-client";
import InfoWindow from "../InfoWindow/InfoWindow";

//map component used on main page
const SimpleMap = () => {
    const [center] = useState({lat: 59.923063, lng: 10.772860});
    const [zoom] = useState(11);
    const [places, setPlaces] = useState([])
    const [selectedPlace, setSelectedPlace] = useState(null);

    //waits for events from server via websocket. All data about the different boats come through here
    useEffect(() => {
        const socket = openSocket(process.env.REACT_APP_SOCKET_URL);
        socket.on("FromAPI", data => {
            setPlaces([...data]);
        });
    }, []);

    return (
        <div style={{height: '50vh', width: '90vw'}}>
            <GoogleMapReact
                bootstrapURLKeys={{key: process.env.REACT_APP_GOOGLE_API}}
                defaultCenter={center}
                defaultZoom={zoom}
            >
                {places.map((place) => (

                    (place.connectionStatus === "online" ?
                            (<Marker
                                key={place._id}
                                name={place._id}
                                lat={place.lat}
                                lng={place.lng}
                                show={place.show}
                                place={place}
                                onClick={() => {
                                    if (selectedPlace) {
                                        setSelectedPlace(null)
                                    } else {
                                        setSelectedPlace(place);
                                    }
                                }}
                            />) : console.log(place.connectionStatus)
                    )

                ))}


                {selectedPlace && (
                    <InfoWindow
                        lat={selectedPlace.lat}
                        lng={selectedPlace.lng}
                        place={selectedPlace}
                        onCloseClick={() => {
                            setSelectedPlace(null);
                        }}
                        position={{
                            lat: parseFloat(selectedPlace.lat),
                            lng: parseFloat(selectedPlace.lng)
                        }}
                    >
                    </InfoWindow>
                )}

            </GoogleMapReact>
        </div>
    );
}


export default SimpleMap;
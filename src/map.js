
import  {useState, useEffect } from 'react';
import GoogleMapReact from 'google-map-react';
import Marker from './components/Marker';
import openSocket from "socket.io-client";

const handleApiLoaded = (map, maps) =>{

}

const SimpleMap = () => {
    const [center, setCenter] = useState({lat: 59.923063, lng: 10.772860});
    const [zoom] = useState(11);
    const [places, setPlaces] = useState([])

    useEffect(() => {
        const socket = openSocket(process.env.REACT_APP_API_URL);
        socket.on("FromAPI", data => {
            console.log(data)
            setPlaces([...data]);
        });
    }, []);


    return (
        <div style={{ height: '50vh', width: '90vw' }}>
            <GoogleMapReact
                bootstrapURLKeys={{ key: process.env.REACT_APP_GOOGLE_API}}
                defaultCenter={center}
                defaultZoom={zoom}
                yesIWantToUseGoogleMapApiInternals
                onGoogleApiLoaded={({ map, maps }) => handleApiLoaded(map, maps)}
            >
                {places.map((place) => (
                    <Marker
                        key={place._id}
                        text={"test"}
                        lat={place.lat}
                        lng={place.lng}
                    />
                ))}
            </GoogleMapReact>
        </div>
    );
}

export default SimpleMap;
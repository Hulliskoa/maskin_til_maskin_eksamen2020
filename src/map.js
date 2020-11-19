
import  {useState, useEffect } from 'react';
import GoogleMapReact from 'google-map-react';
import Marker from './components/Marker';

const handleApiLoaded = (map, maps) =>{

}

const SimpleMap = (props) => {
    const [center, setCenter] = useState({lat: 59.923063, lng: 10.772860});
    const [zoom, setZoom] = useState(11);
    const [places, setPlaces] = useState([])

    useEffect(() => {
        async function fetchMyAPI() {
        let response =  await fetch(process.env.REACT_APP_API_URL + process.env.REACT_APP_BACKEND_API +"/devices");
        const json = await response.json()
        setPlaces(...[json])
     }
        fetchMyAPI()
    },[]);

    useEffect(() => {
        let eventSource = new EventSource(process.env.REACT_APP_API_URL + process.env.REACT_APP_BACKEND_API +"/events");
        eventSource.onmessage = e => {
            setPlaces([...JSON.parse(e.data)])
        }
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
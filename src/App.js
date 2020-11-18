import './App.css';
import React, { useState, useEffect, useRef } from "react";
import { theme } from './theme';
import { ThemeProvider } from 'styled-components';
import { Burger, Menu } from './components';
import FocusLock from 'react-focus-lock';
import { useOnClickOutside } from './hooks';
import { GlobalStyles } from './global';
import MqttListener from './EventListener';
import MapContainer from "./GoogleMapsEmbed";


function App() {
  const [jsonResponse, setJsonResponse] = useState([]);
  const [isLoading, setIsLoading] = useState(true);
  const [open, setOpen] = useState(false);
  const node = useRef();
  const menuId = "main-menu";

  useOnClickOutside(node, () => setOpen(false));

  useEffect(() => {
    fetch(
        process.env.REACT_APP_API_URL + `/api/test`,
        {
          method: 'GET',
          headers: new Headers({
            Accept: 'application/json'
          })
        }
    )
        .then(res => res.json())
        .then(response => {
          setJsonResponse(response.a);
          setIsLoading(false);
        })
        .catch(error => console.log(error));
  });

  return (
      <ThemeProvider theme={theme}>
          <>

              <GlobalStyles />
              <div ref={node}>
                  <FocusLock disabled={!open}>
                      <Burger open={open} setOpen={setOpen} aria-controls={menuId} />
                      <Menu open={open} setOpen={setOpen} id={menuId} />
                  </FocusLock>
              </div>
              <div>
                  <h1>Boat share</h1>

              </div>

          </>

          <div>
              <MapContainer/>
              <MqttListener />
              {isLoading && <p>Wait I'm Loading comments for you</p>}
              {jsonResponse.length !== 0}
              <p>{jsonResponse}</p>

          </div>
      </ThemeProvider>

  );
}

export default App;


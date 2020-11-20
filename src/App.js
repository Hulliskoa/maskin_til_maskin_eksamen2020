import './App.css';
import React, {useState, useRef} from "react";
import {theme} from './theme';
import {ThemeProvider} from 'styled-components';
import {Burger, Menu} from './components';
import FocusLock from 'react-focus-lock';
import {useOnClickOutside} from './hooks';
import {GlobalStyles} from './global';
import MarkerInfoWindowGmapsObj from './map';


function App() {

    const [open, setOpen] = useState(false);
    const node = useRef();
    const menuId = "main-menu";
    useOnClickOutside(node, () => setOpen(false));



    return (
        <ThemeProvider theme={theme}>
            <>

                <GlobalStyles/>
                <div ref={node}>
                    <FocusLock disabled={!open}>
                        <Burger open={open} setOpen={setOpen} aria-controls={menuId}/>
                        <Menu open={open} setOpen={setOpen} id={menuId}/>
                    </FocusLock>
                </div>
                <div>
                    <h1>Boat share</h1>
                    <MarkerInfoWindowGmapsObj/>
                </div>

            </>
        </ThemeProvider>

    );
}

export default App;


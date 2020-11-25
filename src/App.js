import './App.css';
import React, {useState, useRef} from "react";
import {theme} from './theme';
import {ThemeProvider} from 'styled-components';
import {Burger, Menu} from './components';
import FocusLock from 'react-focus-lock';
import {useOnClickOutside} from './hooks';
import {GlobalStyles} from './global';
import SimpleMap from './components/Map/map';


function App() {

    const [open, setOpen] = useState(false);
    const node = useRef();
    const menuId = "main-menu";
    const [page, setPage] = useState(1)
    useOnClickOutside(node, () => setOpen(false));
    function pageToServe(){
        if(page === 1){
            return <SimpleMap/>
        }else if( page === 2){
            return <SimpleMap/>
        }else if(page === 3){
            return <SimpleMap/>
        }
    }

    return (
        <ThemeProvider theme={theme}>
            <>

                <GlobalStyles/>
                <div ref={node}>
                    <FocusLock disabled={!open}>
                        <Burger open={open} setOpen={setOpen} aria-controls={menuId}/>
                        <Menu open={open} setPage={setPage} setOpen={setOpen} id={menuId}/>
                    </FocusLock>
                </div>
                <div>
                    <h1>Boat share</h1>
                    {pageToServe()}
                </div>

            </>
        </ThemeProvider>

    );
}

export default App;


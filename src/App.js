import './App.css';
import React, { useState, useEffect } from "react";

function App() {
  const [jsonResponse, setJsonResponse] = useState([]);
  const [isLoading, setIsLoading] = useState(true);

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
      <div>
        <h1> API calls with React Hooks </h1>
        {isLoading && <p>Wait I'm Loading comments for you</p>}
        {jsonResponse.length !== 0}
        <p>{jsonResponse}</p>

      </div>
  );
}

export default App;


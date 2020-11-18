
import React, {} from "react";


function MqttListener() {
    const [data, setData] = React.useState([]);

    React.useEffect(() => {
        let eventSource = new EventSource(process.env.REACT_APP_API_URL + "/events");
        eventSource.onmessage = e => updateProductList(JSON.parse(e.data));
    }, []);

    const updateProductList = (input) => {

        setData([...input]);
    }
    const listItems = data.map((d) => <li key={d.id}>{d.id}</li>);
console.log(data)
    return (
        <div >
            {listItems}
        </div>
    );


}

export default MqttListener
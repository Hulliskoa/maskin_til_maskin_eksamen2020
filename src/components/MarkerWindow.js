import {useEffect, useState, useCallback, useRef} from "react";

const MarkerWindow = (place) => {



    return (`
        <div style="color:black">
            <div style="font-size: 16px;">
                Boat id: ${place._id}
            </div>
            <div style="font-size: 14px;">
        <span style="color: grey;">
             ${place.lat}:${place.lng}
        </span>

            </div>
            <div style="font-size: 14px; color: grey;">
              <input type="button" disabled={isSending} onClick={sendRequest} />
            </div>
            <div style="font-size: 14px; color: grey;">

            </div>
            <div style="font-size: 14px; color: green;">

            </div>
        </div>`)
}
export default MarkerWindow();
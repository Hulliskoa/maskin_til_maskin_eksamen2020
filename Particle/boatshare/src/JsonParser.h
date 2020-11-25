// had problems with the json libraries so i had to create som functions myself
#pragma once
/**
 * \brief Class for parsing data into json format to be sent with mqtt
 */
class JsonParser {
public:

  /**
   * Parse input to correct json format for the nodejs server to be able to read later
   */
   String stringifyJsonLocation(String lat, String lng, String id, String connectionStatus)
   {
      return("{\"id\":\"" + id + "\",\"lat\":\"" + lat + "\", \"lng\":\"" + lng + "\",\"connectionStatus\":\""+connectionStatus+"\"}");
   };
};

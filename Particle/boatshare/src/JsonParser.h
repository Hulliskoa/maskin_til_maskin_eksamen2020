// had problems with the json libraries so i had to create som functions myself
#pragma once

class JsonParser {
public:

   String stringifyJsonLocation(String lat, String lng, int id)
   {
      return("{\"id\":\"" + String(id) + "\",\"lat\":\"" + lat + "\", \"lng\":\"" + lng + "\" }");
   };
};

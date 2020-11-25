#pragma once
#include "application.h"
#include "Particle.h"
#include "GsmModule.h"

#include "JsonParser.h"

/**
 *  \brief struct containing the position of a device. the data needs to be organized this way to easily get and put to and from EEPROM.
 */
struct position
{
   char lat[10] = "59.910060";
   char lng[10] = "10.731413";
};

/**
 * \brief Handles all gps related actions. This class also stores the latitude and longitude of the device
 */
class GPS {
private:
   String id;               /*!< the unique device id. Initialized in main.ino*/
   position devicePosition; /*!< used to store the last known position of the device*/

public:
   GsmModule gsm;
   GPS();
   JsonParser json = JsonParser();

   /**
    * Sets the member value id to the specified string value
    */
   void setID(String id /**< [in] device id to be set */);

   /**
    * Returns latitude stored in \ref GPS::devicePosition as String type
    */
   String getLat();

   /**
    * Returns longitude stored in GPS::devicePosition as String type
    */
   String getLng();

   /**
    * Set latitude
    * Converts string int into an char array to be able to save it to EEPROM later
    */
   void setLat(String lat /**< [in] device latitude in String format to be set in the \ref GPS::devicePosition struct */);

   /**
    * Set longitude
    * Converts string int into an char array to be able to save it to EEPROM later
    */
   void setLng(String lng /**< [in] device longitude in String format to be set in the GPS::devicePosition struct */);

   /**
    * Initializes gps to be able to fetch position. This method also checks if there are any GPS::devicePosition stored in device's EEPROM. If not it creates a new object and stores it.
    * The prosess is desribed here: https://m2msupport.net/m2msupport/atcgps-start-stop-gps-session/
    */
   bool initializeGPS();

   /**
    * Checks if the gps can localize the device position.
    * Written by: https://gist.github.com/IdrisCytron/7ed2defe995cec8109efc5561fb00886 and modified by me
    */
   bool getCurrentPosition();
};

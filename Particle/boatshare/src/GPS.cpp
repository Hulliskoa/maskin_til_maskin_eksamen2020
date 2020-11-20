#include "GPS.h"

GPS::GPS()
{
   gsm  = GsmModule();
   mqtt = MqttClient();
};

void GPS::setID(int id)
{
   this->id = id;
};


//https://m2msupport.net/m2msupport/atcgps-start-stop-gps-session/
bool GPS::initializeGPS()
{
   gsm.sendAndReadResponse("AT+CGPS=?");

   delay(2000);
   gsm.sendAndReadResponse("AT+CGPS?");
   delay(1000);
   gsm.sendAndReadResponse("AT+CGPS=0");
   delay(2000);
   gsm.sendAndReadResponse("AT+CGPS=1,1");
   delay(2000);
   gsm.sendAndReadResponse("AT+CGPS?");
   delay(2000);
};

String GPS::getPosition()
{
   return(gsm.sendAndReturnResponse("AT+CGPSINFO?", "+CGPSINFO:"));
};

String GPS::getLat()
{
   return(lat);
};

String GPS::getLng()
{
   return(lng);
};

void GPS::setLat(String lat)
{
   this->lat = lat;
};
void GPS::setLng(String lng)
{
   this->lng = lng;
};

//https://gist.github.com/IdrisCytron/7ed2defe995cec8109efc5561fb00886
bool GPS::GPSPositioning()
{
   uint8_t answer  = 0;
   bool    RecNull = true;
   int     i       = 0;
   char    RecMessage[200];
   char    LatDD[3], LatMM[10], LogDD[4], LogMM[10], DdMmYy[7], UTCTime[7];



   memset(RecMessage, '\0', 200); // Initialize the string
   memset(LatDD, '\0', 3);        // Initialize the string
   memset(LatMM, '\0', 10);       // Initialize the string
   memset(LogDD, '\0', 4);        // Initialize the string
   memset(LogMM, '\0', 10);       // Initialize the string
   memset(DdMmYy, '\0', 7);       // Initialize the string
   memset(UTCTime, '\0', 7);      // Initialize the string

   Serial.print("Start GPS session...\n");

   gsm.sendATcommand("AT+CGPS=1,1", "OK", 1000); // start GPS session, standalone mode

   delay(2000);

   while(RecNull){
         answer = gsm.sendATcommand("AT+CGPSINFO", "+CGPSINFO: ", 1000); // start GPS session, standalone mode

         if(answer == 1){
            answer = 0;
            while(Serial1.available() == 0){
                  ;
                  }
            // this loop reads the data of the GPS
            do {
               // if there are data in the UART input buffer, reads it and checks for the asnwer
               if(Serial1.available() > 0){
                  RecMessage[i] = Serial1.read();
                  i++;
                  // check if the desired answer (OK) is in the response of the module
                  if(strstr(RecMessage, "OK") != NULL){
                     answer = 1;
                     }
                  }
               } while (answer == 0); // Waits for the asnwer with time out

            RecMessage[i] = '\0';
            Serial.print(RecMessage);
            Serial.print("\n");
            //strstr Returns a pointer to the first occurrence of str2 in str1, or a null pointer if str2 is not part of str1. http://www.cplusplus.com/reference/cstring/strstr/
            if(strstr(RecMessage, ",,,,,,,,") != NULL){
               mqtt.publishData("scanning for satellites", "/devicePosition/" + String(this->id));
               memset(RecMessage, '\0', 200); // Initialize the string
               i      = 0;
               answer = 0;
               delay(1000);
               }
            else{
                RecNull = false;
                gsm.sendATcommand("AT+CGPS=0", "OK:", 1000);
                mqtt.publishData("satellites found - getting location", "/devicePosition/" + String(this->id));
                }
            }
         else{
             Serial.print("error \n");
             return(false);
             }
         delay(2000);
         }

   strncpy(LatDD
           , RecMessage, 2);
   LatDD[2] = '\0';

   strncpy(LatMM, RecMessage + 2, 9);
   LatMM[9] = '\0';

   this->lat = String(atoi(LatDD) + (atof(LatMM) / 60));
   if(RecMessage[12] == 'N'){
      Serial.print("Latitude is ");
      Serial.print(this->lat);
      Serial.print(" N\n");
      }
   else if(RecMessage[12] == 'S'){
           Serial.print("Latitude is ");
           Serial.print(this->lat);
           Serial.print(" S\n");
           }
   else{
       return(false);
       }

   strncpy(LogDD, RecMessage + 14, 3);
   LogDD[3] = '\0';

   strncpy(LogMM, RecMessage + 17, 9);
   LogMM[9] = '\0';

   this->lng = String(atoi(LogDD) + (atof(LogMM) / 60));
   if(RecMessage[27] == 'E'){
      Serial.print("Longitude is ");
      Serial.print(this->lng);
      Serial.print(" E\n");
      }
   else if(RecMessage[27] == 'W'){
           Serial.print("Latitude is ");
           Serial.print(this->lat);
           Serial.print(" W\n");
           }
   else{
       return(false);
       }
   mqtt.publishData(json.stringifyJsonLocation(this->lat, this->lng, this->id), "/devicePosition");

   return(true);
}

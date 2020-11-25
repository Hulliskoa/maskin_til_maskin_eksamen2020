#include "GPS.h"

GPS::GPS()
{
   gsm = GsmModule();
};

void GPS::setID(String id)
{
   this->id = id;
};

String GPS::getLat()
{
   return(devicePosition.lat);
};

String GPS::getLng()
{
   return(devicePosition.lng);
};

void GPS::setLat(String lat)
{
    //convert String to char array to be able to save the struct in EEPROM
   strcpy(devicePosition.lat, lat.c_str());
};
void GPS::setLng(String lng)
{

   strcpy(devicePosition.lng, lng.c_str());
};

//https://m2msupport.net/m2msupport/atcgps-start-stop-gps-session/
bool GPS::initializeGPS()
{
   position value;

   EEPROM.get(0, value);


   if(value.lat == "0"){
      value = devicePosition;
      }
   else{
       devicePosition = value;
       }

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

   return(true);
};

//https://gist.github.com/IdrisCytron/7ed2defe995cec8109efc5561fb00886
bool GPS::getCurrentPosition()
{
   uint8_t answer  = 0;
   bool    RecNull = true;
   int     i       = 0;
   long    previous;
   char    RecMessage[200];
   char    LatDD[3], LatMM[10], LogDD[4], LogMM[10];

   memset(RecMessage, '\0', 200);
   memset(LatDD, '\0', 3);
   memset(LatMM, '\0', 10);
   memset(LogDD, '\0', 4);
   memset(LogMM, '\0', 10);

   while(Serial1.available() > 0){
         Serial1.read();
         }

   previous = millis();
   while(RecNull){
         answer = gsm.sendATcommand("AT+CGPSINFO", "+CGPSINFO: ", 1000); // start GPS session, standalone mode

         if(answer == 1){
            answer = 0;
            while(Serial1.available() == 0){
                  Serial1.read();
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
               memset(RecMessage, '\0', 200);
               return(false);
               }
            else{
                RecNull = false;
                gsm.sendATcommand("AT+CGPS=0", "OK:", 1000);
                //  mqtt.publishData("satellites found - getting location", "/devicePosition/" + String(this->id));
                }
            }
         else{
             Serial.print("error \n");
             return(false);
             }
         delay(2000);
         }

   strncpy(LatDD, RecMessage, 2);
   LatDD[2] = '\0';

   strncpy(LatMM, RecMessage + 2, 9);
   LatMM[9] = '\0';

   this->setLat(String(atoi(LatDD) + (atof(LatMM) / 60)));
   if(RecMessage[12] == 'N'){
      Serial.print("Latitude is ");
      Serial.print(this->devicePosition.lat);
      Serial.print(" N\n");
      }
   else if(RecMessage[12] == 'S'){
           Serial.print("Latitude is ");
           Serial.print(this->devicePosition.lat);
           Serial.print(" S\n");
           }
   else{
       return(false);
       }

   strncpy(LogDD, RecMessage + 14, 3);
   LogDD[3] = '\0';

   strncpy(LogMM, RecMessage + 17, 9);
   LogMM[9] = '\0';

   this->setLng(String(atoi(LogDD) + (atof(LogMM) / 60)));
   if(RecMessage[27] == 'E'){
      Serial.print("Longitude is ");
      Serial.print(this->devicePosition.lng);
      Serial.print(" E\n");
      }
   else if(RecMessage[27] == 'W'){
           Serial.print("Latitude is ");
           Serial.print(this->devicePosition.lat);
           Serial.print(" W\n");
           }
   else{
       return(false);
       }

   EEPROM.put(0, this->devicePosition);

   //mqtt.publishData(json.stringifyJsonLocation(this->position.lat, this->position.lng, this->id, "online"), "/devicePosition");

   return(true);
}

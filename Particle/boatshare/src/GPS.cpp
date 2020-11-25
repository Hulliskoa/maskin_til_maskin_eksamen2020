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
   //convert String to char array to be able to save the struct in EEPROM when this is implemented
   strcpy(devicePosition.lat, lat.c_str());
};
void GPS::setLng(String lng)
{
   strcpy(devicePosition.lng, lng.c_str());
};

//https://m2msupport.net/m2msupport/atcgps-start-stop-gps-session/
bool GPS::initializeGPS()
{
   //TODO: Check EEPROM for saved coordinates

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
   char    responseBuffer[200];
   char    LatDD[3], LatMM[10], LogDD[4], LogMM[10];

   memset(responseBuffer, '\0', 200);
   memset(LatDD, '\0', 3);
   memset(LatMM, '\0', 10);
   memset(LogDD, '\0', 4);
   memset(LogMM, '\0', 10);

   while(Serial1.available() > 0){
         Serial1.read();
         }

   previous = millis();
   while(RecNull && ((millis() - previous) < 2000)){
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
                  responseBuffer[i] = Serial1.read();
                  i++;
                  // check if the desired answer (OK) is in the response of the module
                  if(strstr(responseBuffer, "OK") != NULL){
                     answer = 1;
                     }
                  }
               } while (answer == 0);

            responseBuffer[i] = '\0';
            Serial.print(responseBuffer);
            Serial.print("\n");
            //strstr Returns a pointer to the first occurrence of str2 in str1, or a null pointer if str2 is not part of str1. http://www.cplusplus.com/reference/cstring/strstr/
            if(strstr(responseBuffer, ",,,,,,,,") != NULL){
               memset(responseBuffer, '\0', 200);
               Serial.println("no satelites found");
               i      = 0;
               answer = 0;
               delay(1000);
               }
            else{
                RecNull = false;
                gsm.sendATcommand("AT+CGPS=0", "OK:", 1000);

                }
            }
         else{
             Serial.print("error \n");
             return(false);
             }
         delay(2000);
         }

   strncpy(LatDD, responseBuffer, 2);
   LatDD[2] = '\0';

   strncpy(LatMM, responseBuffer + 2, 9);
   LatMM[9] = '\0';

   this->setLat(String(atoi(LatDD) + (atof(LatMM) / 60)));
   if(responseBuffer[12] == 'N'){
      Serial.print("Latitude is ");
      Serial.print(devicePosition.lat);
      Serial.print(" N\n");
      }
   else if(responseBuffer[12] == 'S'){
           Serial.print("Latitude is ");
           Serial.print(devicePosition.lat);
           Serial.print(" S\n");
           }
   else{
       return(false);
       }

   strncpy(LogDD, responseBuffer + 14, 3);
   LogDD[3] = '\0';

   strncpy(LogMM, responseBuffer + 17, 9);
   LogMM[9] = '\0';

   this->setLng(String(atoi(LogDD) + (atof(LogMM) / 60)));
   if(responseBuffer[27] == 'E'){
      Serial.print("Longitude is ");
      Serial.print(devicePosition.lng);
      Serial.print(" E\n");
      }
   else if(responseBuffer[27] == 'W'){
           Serial.print("Latitude is ");
           Serial.print(devicePosition.lat);
           Serial.print(" W\n");
           }
   else{
       return(false);
       }



   return(true);
}

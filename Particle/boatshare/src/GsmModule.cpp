#include "GsmModule.h"
#include "Particle.h"

GsmModule::GsmModule()
{
};

void GsmModule::sendAndReadResponse(String command, int extraWaitInMillisecondsForResponse)
{
   // Send the command
   Serial.print("Sending: ");
   Serial.println(command);
   Serial1.println(command);

   // Setup a timeout
   int  requestStartTime = millis();
   int  millisecondsSinceRequestStarted = 0;
   bool wegotResponse = false;

   // Wait until we get a response (or timeout)
   while(!wegotResponse || millisecondsSinceRequestStarted < 2000){
         millisecondsSinceRequestStarted = millis() - requestStartTime;

         if(Serial1.available() > 0){
            wegotResponse = true;
            }
         Particle.process();
         }

   // Print out the results
   if(millisecondsSinceRequestStarted < 2000){
      Serial.print("\nCommand: '");
      Serial.print(command);
      Serial.println(" timed out?\n");
      }
   else if(Serial1.available() > 0){
           Serial.print("<\n");

           // wait a little longer so we get the whole response
           delay(extraWaitInMillisecondsForResponse);

           // Print out the response to Serial monitor
           while(Serial1.available()){
                 char ch = Serial1.read();

                 if(ch){
                    Serial.print(ch);
                    }
                 }
           Serial.println("\n>");
           }
}

char *GsmModule::returnPayload(String subTopic, String expected_answer, int timeout)
{
   int  x = 0, answer = 0;
   char response[100];
   long previous;
   char payload[100];

   memset(response, '\0', 100); // Initialize the string

   delay(100);
   while(Serial.available() > 0){ // Clean the input buffer
         Serial.read();
         }
   while(Serial1.available() > 0){ // Clean the input buffer
         Serial1.read();
         }
   String subLength = String(strlen(subTopic));

   Serial1.println("AT+CMQTTSUBTOPIC=0," + subLength + ",1");   // Send the AT command
   Serial1.println(subTopic);
   x        = 0;
   previous = millis();

   // This loop waits for the answer
   do {
      if(Serial1.available() != 0){
         // if there are data in the UART input buffer, reads it and checks for the answer
         response[x] = Serial1.read();
         Serial.print(response[x]);
         x++;
         // check if the desired answer is in the response of the module
         if(strstr(response, expected_answer) != NULL){
            Serial.print(response);
            Serial.println("--- Matched response");
            while(Serial1.available() > 0){
                  Serial.println("--- Inside payload");
                  char ch = Serial1.read();
                  Serial.print(ch);
                  int i = 0;
                  if(ch == '\n'){
                     memset(payload, '\0', 100);
                     while((ch = Serial1.read()) != '\n'){
                           payload[i] = ch;
                           i++;
                           }
                     Serial.print(payload);
                     Serial.println("<-- payload");
                     }
                  }
            }
         }
      // Waits for the asnwer with time out
      } while ((answer == 0) && ((millis() - previous) < timeout));

   // SIM7600Serial->print("\n");
   return(payload);
}

//https://gist.github.com/IdrisCytron/7ed2defe995cec8109efc5561fb00886
int GsmModule::sendATcommand(String ATcommand, String expected_answer, int timeout)
{
   int  x = 0, answer = 0;
   char response[100];
   long previous;

   memset(response, '\0', 100); // Initialize the string

   delay(100);

   while(Serial1.available() > 0){  // Clean the input buffer
         Serial1.read();
         }

   Serial1.println(ATcommand);   // Send the AT command

   x        = 0;
   previous = millis();

   // This loop waits for the answer
   do {
      if(Serial1.available() != 0){
         // if there are data in the UART input buffer, reads it and checks for the answer
         response[x] = Serial1.read();
         Serial.print(response[x]);
         x++;
         // check if the desired answer is in the response of the module
         if(strstr(response, expected_answer) != NULL){
            answer = 1;
            }
         }
      // Waits for the asnwer with time out
      } while ((answer == 0) && ((millis() - previous) < timeout));

   // SIM7600Serial->print("\n");
   return(answer);
}

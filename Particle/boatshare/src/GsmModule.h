#pragma once
#include "application.h"
#include "Particle.h"

class GsmModule {
public:
   GsmModule();
   void sendAndReadResponse(String command, int extraWaitInMillisecondsForResponse = 200);
   String sendAndReturnResponse(String command,String returnCommand, int extraWaitInMillisecondsForResponse = 200);
   int sendATcommand(String ATcommand, String expected_answer, int timeout);
};

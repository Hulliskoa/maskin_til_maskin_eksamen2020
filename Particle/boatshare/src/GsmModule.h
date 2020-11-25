#pragma once
#include "application.h"
#include "Particle.h"

/**
 * \brief Handles the communication with the simcom 7600 module when no specific
 *
 */
class GsmModule {
public:


   GsmModule();
   /**
    * Sends a specified AT-command, waits for the response and prints it to Serial. This is done without handling the response in any way.
    * Written by: https://github.com/jenschr
    */
   void sendAndReadResponse(String command, int extraWaitInMillisecondsForResponse = 200);

   /**
    * Sends a specified AT-command, waits for the response, prints and handles the expected return value
    * Written by: https://gist.github.com/IdrisCytron/7ed2defe995cec8109efc5561fb00886 and modified by me
    */
   int sendATcommand(String ATcommand, String expected_answer, int timeout);
};

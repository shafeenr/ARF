#include <Arduino.h>
#include <PhpocDateTime.h>

#define oneHourInMilliseconds 3600000
#define oneMinuteInMilliseconds 60000

extern boolean clientConnection;
extern PhpocDateTime realTimeClock;

extern void feedPet();

unsigned long clientConnectionTimer = 0;
unsigned long feedTimer = 0;

boolean alreadyFedInLastMinute = false;

void initializeTimers()
{
    feedTimer = millis();
    clientConnectionTimer = millis();
}

void sshTimeout()
{
    if (millis() > clientConnectionTimer + oneHourInMilliseconds)
    {
        clientConnectionTimer = millis();
        clientConnection = false;
    }
}

void minuteTimeout()
{
    if (millis() > feedTimer + oneMinuteInMilliseconds)
    {
        feedTimer = millis();
        alreadyFedInLastMinute = false;
    }
}

void checkFeedTime() {
  if (!alreadyFedInLastMinute && realTimeClock.hour() == 9 && realTimeClock.minute() == 30) {
      feedPet();
  }
}
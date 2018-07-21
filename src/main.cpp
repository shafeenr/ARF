#include <Arduino.h>

extern void initializeSSH();
extern void SSH();
extern void checkUserInput();

extern void initializeTimers();
extern void sshTimeout();
extern void minuteTimeout();
extern void checkFeedTime();

extern void initializeEmail();

extern void initializeFood();
extern void checkWaterLevels();

void setup()
{
    initializeTimers();
    initializeSSH();
    initializeEmail();
    initializeFood();
}

void loop()
{
    SSH();
    checkUserInput();
    sshTimeout();
    minuteTimeout();
    checkFeedTime();
    checkWaterLevels();
}
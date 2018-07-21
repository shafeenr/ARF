#include <Arduino.h>
#include <PhpocDateTime.h>
#include <Servo.h>

extern PhpocDateTime realTimeClock;
extern boolean alreadyFedInLastMinute;
extern void emailAlert(String currentTime);

extern unsigned char servoPin;
extern unsigned char trig;
extern unsigned char echo;
extern unsigned char dc;

Servo foodMotor;

void initializeFood()
{
    foodMotor.attach(servoPin);
    pinMode(trig, OUTPUT);
    pinMode(echo, INPUT);
    pinMode(dc, OUTPUT);
}

void feedPet()
{
    for (int i = 0; i <= 180; i++)
    {
        foodMotor.write(i);
        delay(15);
    }
    for (int i = 180; i >= 0; i--)
    {
        foodMotor.write(i);
        delay(15);
    }

    alreadyFedInLastMinute = true;
    emailAlert(realTimeClock.date("Y-m-d H:i:s"));
}

void checkWaterLevels()
{
    long t = 0, h = 0, hp = 0;

    digitalWrite(trig, LOW);
    delayMicroseconds(2);
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);

    t = pulseIn(echo, HIGH);

    h = t / 58;

    h = h - 6;
    h = 50 - h;

    hp = 2 * h;

    if (hp < 100)
    {
        analogWrite(dc, 255);
        delay(1000);
        analogWrite(dc, 0);
    }
}
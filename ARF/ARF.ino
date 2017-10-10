
#include <SPI.h>
#include <Phpoc.h>
#include <Servo.h>

#define oneHourInMilliseconds 3600000
#define oneMinuteInMilliseconds 60000

PhpocServer sshServer(22);
boolean clientConnection = false;
String userTimeInput = "";
PhpocClient sshClient;
PhpocDateTime realTimeClock;
unsigned long clientConnectionTimer = 0;
PhpocEmail feedAlert;
Servo food;
String currentTime;
String userName = "user";
unsigned long feedTimer = 0;
boolean alreadyFedInLastMinute = false;
// last second lazy programming because
// no split function
/*int trig = 12;
int echo = 11;
int dc = 19;*/

void setup() {
  feedTimer = millis();
  clientConnectionTimer = millis();
  Serial.begin(9600);
  Phpoc.begin(PF_LOG_SPI | PF_LOG_NET | PF_LOG_APP);
  sshServer.beginSSH("root", "toor");
  Serial.print("SSH server at ");
  Serial.println(Phpoc.localIP());
  food.attach(9);
  feedAlert.setOutgoingServer("smtp.gmail.com", 587);
  feedAlert.setOutgoingLogin("arf.at.ching", "failurewiththreees");
  feedAlert.setFrom("arf.at.ching@gmail.com", "Automated Robotic Feeder");
  feedAlert.setTo("shafeenr@tuta.io", "Shafeen Rahman");
  feedAlert.setSubject("Mail from your Automated Robotic Feeder");
/*  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(dc, OUTPUT);*/
}

void loop() {
  sshClient = sshServer.available();
  if (sshClient) {
    if (!clientConnection) {
      sshClient.flush();
      Serial.println("New SSH client connection");
      sshClient.println("Welcome to the Automated Robotic Feeder!");
      clientConnection = true;
    }
    if (sshClient.available() > 0) {
      char currentChar = sshClient.read();
      sshServer.write(currentChar);
      userTimeInput.concat(currentChar);
    }
  }
  if (millis() > clientConnectionTimer + oneHourInMilliseconds) {
    clientConnectionTimer = millis();
    clientConnection = false;
  }
  if (millis() > feedTimer + oneMinuteInMilliseconds) {
    feedTimer = millis();
    alreadyFedInLastMinute = false;
  }
  checkUserInput();
  checkFeedTime();
}

void checkUserInput() {
  if (userTimeInput.endsWith("clear")) {
    userTimeInput = "";
  }
  if (userTimeInput.endsWith("feed")) {
    sshClient.println("\nAttempting to feed pet.");
    userTimeInput = "";
    feedPet();
  }
  if (userTimeInput.endsWith("time")) {
    sshClient.println("\n");
    sshClient.println(realTimeClock.date("Y-m-d H:i:s"));
    userTimeInput = "";
  }
  if (userTimeInput.endsWith("setname")) {
    if (userTimeInput.charAt(userTimeInput.indexOf("setname") - 1) == ' ') {
      userName = userTimeInput.substring(0, userTimeInput.indexOf("setname") - 1);
    } else {
      userName = userTimeInput.substring(0, userTimeInput.indexOf("setname"));
    }
    userTimeInput = "";
    sshClient.print("\nUser name set to: ");
    sshClient.println(userName);
  }
}

void feedPet() {
  currentTime = realTimeClock.date("Y-m-d H:i:s");
  if (realTimeClock.hour() > 12 && !alreadyFedInLastMinute) {
    for (int i = 0; i <= 180; i++) {
      food.write(i);
      delay(15);
    }
    for (int i = 180; i >= 0; i--) {
      food.write(i);
      delay(15);
    }
  }
  alreadyFedInLastMinute = true;
  emailAlert("The Automated Robotic Feeder attempted to feed your pet.", currentTime);
}

void emailAlert(String message, String timePerformed) {
  feedAlert.beginMessage();
  feedAlert.print("Hello, ");
  feedAlert.print(userName);
  feedAlert.println("!");
  feedAlert.println(message);
  feedAlert.println("This action was performed at ");
  feedAlert.println(timePerformed);
  feedAlert.endMessage();
  feedAlert.send();
}

void checkFeedTime() {
  //
}

/*void waterLevels() {
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

  if (hp < 100) {
    analogWrite(dc, 255);
    delay(1000);
    analogWrite(dc, 0);
  }
}*/


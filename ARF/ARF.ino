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
Servo dryFood;
Servo wetFood;
String currentTime;
String userName = "user";
unsigned long feedTimer = 0;
boolean alreadyFedInLastMinute = false;
// last second lazy programming because
// no split function
String monmorn;
String moneven;
String tuemorn;
String tueeven;
String wedmorn;
String wedeven;
String thumorn;
String thueven;
String frimorn;
String frieven;
String satmorn;
String sateven;
String sunmorn;
String suneven;
int trig = 12;
int echo = 11;
int dc = 19;

void setup() {
  feedTimer = millis();
  clientConnectionTimer = millis();
  Serial.begin(9600);
  Phpoc.begin(PF_LOG_SPI | PF_LOG_NET | PF_LOG_APP);
  sshServer.beginSSH("root", "toor");
  Serial.print("SSH server at ");
  Serial.println(Phpoc.localIP());
  dryFood.attach(8);
  wetFood.attach(9);
  feedAlert.setOutgoingServer("smtp.gmail.com", 587);
  feedAlert.setOutgoingLogin("arf.at.ching", "");
  feedAlert.setFrom("arf.at.ching@gmail.com", "Automated Robotic Feeder");
  feedAlert.setTo("shafeenr@tuta.io", "Shafeen Rahman");
  feedAlert.setSubject("Mail from your Automated Robotic Feeder");
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(dc, OUTPUT);
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
  if (userTimeInput.endsWith("feedwet")) {
    sshClient.println("\nAttempting to feed pet.");
    userTimeInput = "";
    feedPet(true);
  }
  if (userTimeInput.endsWith("feeddry")) {
    sshClient.println("\nAttempting to feed pet.");
    userTimeInput = "";
    feedPet(true);
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
  if (userTimeInput.endsWith("monmorn")) {
    monmorn = userTimeInput.substring(0, userTimeInput.indexOf("monmorn") - 1);
    userTimeInput = "";
  }
  if (userTimeInput.endsWith("moneven")) {
    moneven = userTimeInput.substring(0, userTimeInput.indexOf("moneven") - 1);
    userTimeInput = "";
  }
  if (userTimeInput.endsWith("tuemorn")) {
    tuemorn = userTimeInput.substring(0, userTimeInput.indexOf("tuemorn") - 1);
    userTimeInput = "";
  }
  if (userTimeInput.endsWith("tueeven")) {
    tueeven = userTimeInput.substring(0, userTimeInput.indexOf("tueeven") - 1);
    userTimeInput = "";
  }
  if (userTimeInput.endsWith("wedmorn")) {
    wedmorn = userTimeInput.substring(0, userTimeInput.indexOf("wedmorn") - 1);
    userTimeInput = "";
  }
  if (userTimeInput.endsWith("wedeven")) {
    wedeven = userTimeInput.substring(0, userTimeInput.indexOf("wedeven") - 1);
    userTimeInput = "";
  }
  if (userTimeInput.endsWith("thumorn")) {
    thumorn = userTimeInput.substring(0, userTimeInput.indexOf("thumorn") - 1);
    userTimeInput = "";
  }
  if (userTimeInput.endsWith("thueven")) {
    thueven = userTimeInput.substring(0, userTimeInput.indexOf("thueven") - 1);
    userTimeInput = "";
  }
  if (userTimeInput.endsWith("frimorn")) {
    frimorn = userTimeInput.substring(0, userTimeInput.indexOf("frimorn") - 1);
    userTimeInput = "";
  }
  if (userTimeInput.endsWith("frieven")) {
    frieven = userTimeInput.substring(0, userTimeInput.indexOf("frieven") - 1);
    userTimeInput = "";
  }
  if (userTimeInput.endsWith("satmorn")) {
    satmorn = userTimeInput.substring(0, userTimeInput.indexOf("satmorn") - 1);
    userTimeInput = "";
  }
  if (userTimeInput.endsWith("sateven")) {
    sateven = userTimeInput.substring(0, userTimeInput.indexOf("sateven") - 1);
    userTimeInput = "";
  }
  if (userTimeInput.endsWith("sunmorn")) {
    sunmorn = userTimeInput.substring(0, userTimeInput.indexOf("sunmorn") - 1);
    userTimeInput = "";
  }
  if (userTimeInput.endsWith("suneven")) {
    suneven = userTimeInput.substring(0, userTimeInput.indexOf("suneven") - 1);
    userTimeInput = "";
  }
}

void feedPet() {
  currentTime = realTimeClock.date("Y-m-d H:i:s");
  if (realTimeClock.hour() > 12 && !alreadyFedInLastMinute) {
    for (int i = 0; i <= 180; i++) {
      dryFood.write(i);
      delay(15);
    }
    for (int i = 180; i >= 0; i--) {
      dryFood.write(i);
      delay(15);
    }
  } else if (!alreadyFedInLastMinute) {
    for (int i = 0; i <= 180; i++) {
      wetFood.write(i);
      delay(15);
    }
    for (int i = 180; i >= 0; i--) {
      wetFood.write(i);
      delay(15);
    }
  }
  alreadyFedInLastMinute = true;
  emailAlert("The Automated Robotic Feeder attempted to feed your pet.", currentTime);
}

void feedPet(boolean wetOrDry) {
  currentTime = realTimeClock.date("Y-m-d H:i:s");
  if (wetOrDry) {
    for (int i = 0; i <= 180; i++) {
      wetFood.write(i);
      delay(15);
    }
    for (int i = 180; i >= 0; i--) {
      wetFood.write(i);
      delay(15);
    }
  } else {
    for (int i = 0; i <= 180; i++) {
      dryFood.write(i);
      delay(15);
    }
    for (int i = 180; i >= 0; i--) {
      dryFood.write(i);
      delay(15);
    }
  }
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
  if (realTimeClock.dayofWeek() == 1) {
    if (realTimeClock.date("Hi") == monmorn.toInt() || realTimeClock.date("Hi") == moneven.toInt()) {
      feedPet();
    }
  }
  if (realTimeClock.dayofWeek() == 2) {
    if (realTimeClock.date("Hi") == tuemorn.toInt() || realTimeClock.date("Hi") == tueeven.toInt()) {
      feedPet();
    }
  }
  if (realTimeClock.dayofWeek() == 3) {
    if (realTimeClock.date("Hi") == wedmorn.toInt() || realTimeClock.date("Hi") == wedeven.toInt()) {
      feedPet();
    }
  }
  if (realTimeClock.dayofWeek() == 4) {
    if (realTimeClock.date("Hi") == thumorn.toInt() || realTimeClock.date("Hi") == thueven.toInt()) {
      feedPet();
    }
  }
  if (realTimeClock.dayofWeek() == 5) {
    if (realTimeClock.date("Hi") == frimorn.toInt() || realTimeClock.date("Hi") == frieven.toInt()) {
      feedPet();
    }
  }
  if (realTimeClock.dayofWeek() == 6) {
    if (realTimeClock.date("Hi") == satmorn.toInt() || realTimeClock.date("Hi") == sateven.toInt()) {
      feedPet();
    }
  }
  if (realTimeClock.dayofWeek() == 7) {
    if (realTimeClock.date("Hi") == sunmorn.toInt() || realTimeClock.date("Hi") == suneven.toInt()) {
      feedPet();
    }
  }
}

void waterLevels() {
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
}


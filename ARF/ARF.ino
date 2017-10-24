#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_MotorShield.h>
#include <SPI.h>
#include <Phpoc.h>
#include <Servo.h>

#define oneHourInMilliseconds 3600000
#define oneMinuteInMilliseconds 60000

// LCD with I2C backpack
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

// SSH server
PhpocServer sshServer(22);
PhpocClient sshClient;
boolean clientConnection = false;
String userInput = "";
String userName = "user";

// Real time clock and related time variables
PhpocDateTime realTimeClock;
unsigned long feedTimer = 0;
unsigned long clientConnectionTimer = 0;
boolean alreadyFedInLastMinute = false;

// Email
PhpocEmail feedAlert;

void setup() {
  feedTimer = millis();
  clientConnectionTimer = millis();

  lcd.begin(16, 2);
  lcd.backlight();
  lcd.clear();

  Phpoc.begin(PF_LOG_SPI | PF_LOG_NET | PF_LOG_APP);
  sshServer.beginSSH("root", "toor");
  sshServerAddressDisplay();

  feedAlert.setOutgoingServer("smtp.gmail.com", 587);
  feedAlert.setOutgoingLogin("arf.at.ching", "failurewiththreees");
  feedAlert.setFrom("arf.at.ching@gmail.com", "Automated Robotic Feeder");
  feedAlert.setTo("shafeenr@tuta.io", "Shafeen Rahman");
  feedAlert.setSubject("Mail from your Automated Robotic Feeder");
}

void loop() {
  sshClient = sshServer.available();
  if (sshClient) {
    if (!clientConnection) {
      sshClient.flush();
      lcd.clear();
      lcd.print("New SSH client");
      delay(100);
      sshClient.println("Welcome to the Automated Robotic Feeder!");
      clientConnection = true;

    }
    if (sshClient.available() > 0) {
      char currentChar = sshClient.read();
      sshServer.write(currentChar);
      userInput.concat(currentChar);
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
  sshServerAddressDisplay();
}

void checkUserInput() {
  if (userInput.endsWith("clear")) {
    userInput = "";
  }

  if (userInput.endsWith("feed")) {
    sshClient.println("\nAttempting to feed pet.");
    lcd.clear();
    lcd.print("Feeding pet...");
    userInput = "";
    feedPet();
    delay(100);
    sshServerAddressDisplay();
  }

  if (userInput.endsWith("time")) {
    sshClient.println("\n");
    sshClient.println(realTimeClock.date("Y - m - d H: i: s"));
    userInput = "";
  }

  if (userInput.endsWith("setname")) {
    if (userInput.charAt(userInput.indexOf("setname") - 1) == ' ') {
      userName = userInput.substring(0, userInput.indexOf("setname") - 1);
    } else {
      userName = userInput.substring(0, userInput.indexOf("setname"));
    }
    userInput = "";
    sshClient.print("\nUser name set to: ");
    sshClient.println(userName);
  }
}

void feedPet() {
  alreadyFedInLastMinute = true;
  emailAlert("The Automated Robotic Feeder attempted to feed your pet.", realTimeClock.date("Y - m - d H: i: s"));
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

void sshServerAddressDisplay() {
  lcd.clear();
  lcd.print(realTimeClock.date("Y - m - d H: i: s"));
  lcd.setCursor(0, 1);
  lcd.print(Phpoc.localIP());
}


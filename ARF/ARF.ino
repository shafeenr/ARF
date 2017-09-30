#include <SPI.h>
#include <Phpoc.h>
#include <Servo.h>

#define clientGreetInterval 3600000

PhpocServer sshServer(22);
PhpocClient sshClient = sshServer.available();
boolean clientConnection = false;

String userTimeInput;

unsigned long clientConnectionTimer = 0;

Servo dryFood;
Servo wetFood;

PhpocEmail feedAlert;

PhpocDateTime realTimeClock;

String currentTime;

int schedule[14];
int spaceLocationX;
int spaceLocationY;

void setup() {
  dryFood.attach(9);
  wetFood.attach(10);

  clientConnectionTimer = millis();

  Serial.begin(9600);

  Phpoc.begin(PF_LOG_SPI | PF_LOG_NET | PF_LOG_APP);

  sshServer.beginSSH("root", "toor");

  Serial.print("SSH server address: ");
  Serial.println(Phpoc.localIP());

  feedAlert.setOutgoingServer("smtp.gmail.com", 587);
  feedAlert.setOutgoingLogin("your_login_id", "your_login_password or app_password");

  feedAlert.setFrom("from_email_address", "from_user_name");
  feedAlert.setTo("to_email_address", "to_user_name");
  feedAlert.setSubject("Mail from your Automated Robotic Feeder");
}

void loop() {
  sshClient = sshServer.available();

  if (sshClient) {
    if (!clientConnection) {
      sshClient.flush();
      Serial.println("Client connected via SSH");
      sshClient.println("Welcome to the Automated Robotic Feeder!");
      clientConnection = true;
    }

    if (sshClient.available() > 0) {
      char thisChar = sshClient.read();
      sshServer.print(thisChar);
      userTimeInput.concat(thisChar);
    }
    if (userTimeInput.endsWith("clear")) {
      userTimeInput = "";
    }
    if (userTimeInput.endsWith("feed")) {
      sshClient.println("Attempting to feed dog.");
      userTimeInput = "";
      feedPet();
    }
    if (userTimeInput.endsWith("time")) {
      sshClient.println(realTimeClock.date("Y-m-d H:i:s"));
    }
    if (userTimeInput.endsWith("OK")) {
      spaceLocationX = userTimeInput.indexOf(" ");
      spaceLocationY = userTimeInput.indexOf(" ", spaceLocationX);
      schedule[0] = userTimeInput.substring(0, spaceLocationX).toInt();
      for (int i = 1; i < sizeof(schedule); i++) {
        schedule[i] = userTimeInput.substring(spaceLocationX + 1, spaceLocationY).toInt();
        spaceLocationX = userTimeInput.indexOf(" ", spaceLocationY);
        spaceLocationY = userTimeInput.indexOf(" ", spaceLocationX);
      }
      userTimeInput = "";
    }
  }
  if (millis() > clientConnectionTimer + clientGreetInterval) {
    clientConnectionTimer = millis();
    clientConnection = false;
  }
}

void feedPet() {
  currentTime = realTimeClock.date("Y-m-d H:i:s");

  for (int i = 0; i <= 180; i++) {
    dryFood.write(i);
    delay(15);
  }
  for (int i = 180; i >= 0; i--) {
    dryFood.write(i);
    delay(15);
  }
  emailAlert("The Automated Robotic Feeder attempted to feed your pet.", currentTime);
}

void emailAlert(String message, String recordedTime) {
  feedAlert.beginMessage();
  feedAlert.println("Hello, user!");
  feedAlert.println(message);
  feedAlert.println("This action was performed at ");
  feedAlert.print(recordedTime);
  feedAlert.endMessage();
  feedAlert.send();
}


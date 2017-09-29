#include "SPI.h"
#include "Phpoc.h"

#define clientGreetInterval 3600000

PhpocServer sshServer(22);
PhpocClient sshClient = sshServer.available();
boolean clientConnection = false;

String userTimeInput;

unsigned long startMillis = 0;

void setup() {
  startMillis = millis();

  Serial.begin(9600);

  Phpoc.begin(PF_LOG_SPI | PF_LOG_NET);

  sshServer.beginSSH("root", "toor");

  Serial.print("SSH server address: ");
  Serial.println(Phpoc.localIP());
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
  }
  if (millis() > startMillis + clientGreetInterval) {
    startMillis = millis();
    clientConnection = false;
  }
}

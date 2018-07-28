#include <Arduino.h>
#include <PhpocEmail.h>

PhpocEmail feedAlert;

extern char userName[];
extern char userEmail[];

extern char outgoingServer[];
extern unsigned short outgoingServerPort;
extern char outgoingLoginEmail[];
extern char outgoingLoginUser[];
extern char outgoingLoginPass[];

void initializeEmail()
{
    feedAlert.setOutgoingServer(outgoingServer, outgoingServerPort);
    feedAlert.setOutgoingLogin(outgoingLoginUser, outgoingLoginPass);
    feedAlert.setFrom(outgoingLoginEmail, "Automated Robotic Feeder");
    feedAlert.setTo(userEmail, userName);
    feedAlert.setSubject("Mail from your Automated Robotic Feeder");
}

void emailAlert(String timePerformed) {
  feedAlert.beginMessage();
  feedAlert.print("Hello, ");
  feedAlert.print(userName);
  feedAlert.println("!");
  feedAlert.println("The Automated Robotic Feeder attempted to feed your pet.");
  feedAlert.println("This action was performed at ");
  feedAlert.println(timePerformed);
  feedAlert.endMessage();
  feedAlert.send();
}
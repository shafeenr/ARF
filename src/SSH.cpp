#include <SPI.h>
#include <Phpoc.h>

extern char sshUser[];
extern char sshPass[];

extern void feedPet();

PhpocServer sshServer(22);
PhpocClient sshClient;
PhpocDateTime realTimeClock;
boolean clientConnection = false;

String userInput = "";

void initializeSSH()
{
    Phpoc.begin(PF_LOG_SPI | PF_LOG_NET | PF_LOG_APP);
    sshServer.beginSSH(sshUser, sshPass);
}

void SSH()
{
    sshClient = sshServer.available();

    if (sshClient)
    {
        if (!clientConnection)
        {
            sshClient.flush();
            sshClient.println("Welcome to the Automated Robotic Feeder!");
            clientConnection = true;
        }
        if (sshClient.available() > 0)
        {
            char currentChar = sshClient.read();
            sshServer.write(currentChar);
            userInput.concat(currentChar);
        }
    }
}

void checkUserInput()
{
    if (userInput.endsWith("clear"))
    {
        userInput = "";
    }
    else if (userInput.endsWith("feed"))
    {
        sshClient.println("\nAttemping to dispense food.");
        userInput = "";
        feedPet();
    }
    else if (userInput.endsWith("time"))
    {
        sshClient.println(realTimeClock.date("Y-m-d H:i:s"));
        userInput = "";
    }
}
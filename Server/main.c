//--External includes-------------------------------------------------
#include <stdio.h>
//--------------------------------------------------------------------

//--Project includes--------------------------------------------------
#include "meterUtils.h"
#include "minMaxValues.h"
//--------------------------------------------------------------------

//--Local includes----------------------------------------------------
#include "serverQueueUtils.h"
#include "serverMessageHandlers.h"
//--------------------------------------------------------------------

//--Private Function Declaration--------------------------------------
void StartServer( void );
void InitServer( void );
void HandleIncomingMessages( void );
void HandleOutgoingMessages( void );
//--------------------------------------------------------------------

int main()
{
    InitServer();
    StartServer();
    // Should not reach this place
    return 0;
}

void StartServer( void )
{
    printf("Meter_srv is running...\r\n");
    while (1)
    {
        ReadStructFromDev();
        StoreMaxMinValues();
        HandleIncomingMessages();
        HandleOutgoingMessages();
    }
}

void InitServer( void )
{
    InitMeter();
    InitServerMessageQueue();
    CleanServerQueue();
}


void HandleIncomingMessages( void )
{
    while ( GetNumberOfMessagesInServerQueue() )
    {
        HandleSingleGetRequest();
        HandleSingleSetRequest();
        HandleResetRequest();
        HandleSubscriptionRequest();
        HandleNotificationResponse();
    }
}

void HandleOutgoingMessages( void )
{
    HandleSendingNotifications();
}

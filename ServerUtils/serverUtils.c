//--External includes-------------------------------------------------
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
//--------------------------------------------------------------------

//--Local includes----------------------------------------------------
#include "dataUtils.h"
#include "queueUtils.h"
#include "messageHandlers.h"
//--------------------------------------------------------------------

//--Private Function Declaration--------------------------------------
void HandleIncomingMessages( void );
void HandleOutgoingMessages( void );
//--------------------------------------------------------------------

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
    CleanSrvQueue();
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

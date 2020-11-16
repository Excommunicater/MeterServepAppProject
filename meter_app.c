//--External includes-------------------------------------------------
#include <stdio.h>              // pritnf()
#include <sys/stat.h>           // mkfifo()
#include <unistd.h>             // usleep()
#include <sys/ipc.h>            // key_t
#include <sys/msg.h>            // msgget()
#include <stdlib.h>             // exit()
//--------------------------------------------------------------------

//--Local includes----------------------------------------------------
#include "includes/serverMessages.h"     //
#include "includes/meter_srv.h"  
//--------------------------------------------------------------------

//--Defines-----------------------------------------------------------
#define APP_PATH "meter_app.c"
#define APP_REV  666
//--------------------------------------------------------------------

//--Global Variables--------------------------------------------------
int serverQueueId = 0;
int appQueueId = 0;
//--------------------------------------------------------------------

//--Function Declaration----------------------------------------------
//--------------------------------------------------------------------

int main( void )
{
    serverQueueId = InitServerMessageQueue();
    appQueueId = InitMessageQueue( APP_PATH );

    requestSingleGet_t request;
    request.mtype = SINGLE_GET_REQUEST;

    requestSingleGetBody_t * pBody = (requestSingleGetBody_t*)request.mtext;
    pBody->queueResponseId = appQueueId;
    printf("Try To send Request!\r\n");
    for (uint8_t i = 0U; i < 10; i++)
    {
        sleep(1);
        printf("Sending Request! Attemp %i\r\n", i);
        PushMessageToQueue( (void*)&request, SINGLE_GET_REQUEST, serverQueueId );
    }

    responseShortConfirmation_t response;
    for (uint8_t i = 0U; i < 10; i++)
    {
        sleep(1);
        printf("WAIT FOR RESPONSE! Attemp %i\r\n", i);
        if ( GetMessageFromQueue( (void*)&response, SHORT_CONFIRMATION_RESPONSE, appQueueId ) )
        {
            responseShortConfirmationBody_t * body = (responseShortConfirmationBody_t*)response.mtext;
            printf("meter app - GET RESPONSE! confValue = %i someData = %i \r\n", body->confirmationValue, body->someData1);
            break;
        }
    }
}

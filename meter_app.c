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
    pBody->attribute = METER_SERVER_VERSION;
    printf("Send request for METER_NUMBER\r\n");
    PushMessageToQueue( (void*)&request, SINGLE_GET_REQUEST, serverQueueId );

    sleep(1);
    responseUint32_t response;
    if ( GetMessageFromQueue( (void*)&response, UINT32_RESPONSE, appQueueId ) )
    {
        responseUint32Body_t * body = (responseUint32Body_t*)response.mtext;
        printf("meter app - GET RESPONSE! METER_NUMBER = %i\r\n", body->value);
    }
    
}

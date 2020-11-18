//--External includes-------------------------------------------------
#include <stdio.h>              // pritnf()
#include <sys/stat.h>           // mkfifo()
#include <unistd.h>             // usleep()
#include <sys/ipc.h>            // key_t
#include <sys/msg.h>            // msgget()
#include <stdlib.h>             // exit()
//--------------------------------------------------------------------

//--Local includes----------------------------------------------------
#include "../ServerUtils/serverUtils.h"     //
#include "../commonIncludes/serverMessages.h" 
//--------------------------------------------------------------------

//--Defines-----------------------------------------------------------
#define APP_PATH "./ApplicationStart/meter_app.c"
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
    printf("Send request for METER_SERVER_VERSION\r\n");
    PushMessageToQueue( (void*)&request, SINGLE_GET_REQUEST, serverQueueId );

    sleep(1);
    responseUint32_t responseUint32;
    if ( GetMessageFromQueue( (void*)&responseUint32, UINT32_RESPONSE, appQueueId ) )
    {
        responseUint32Body_t * body = (responseUint32Body_t*)responseUint32.mtext;
        if ( body->value ==  98765432U )
        {
            printf("meter app - GET proper RESPONSE! \r\n");
        }
        else
        {
            printf("GET NOT EXPECTED RESPONSE!!! response = %i\r\n", body->value);
        }
    }

    sleep(1);

    request.mtype = SINGLE_GET_REQUEST;
    pBody->attribute = METER_NUMBER;
    printf("Send request for METER_NUMBER\r\n");
    PushMessageToQueue( (void*)&request, SINGLE_GET_REQUEST, serverQueueId );

    sleep(1);

    if ( GetMessageFromQueue( (void*)&responseUint32, UINT32_RESPONSE, appQueueId ) )
    {
        responseUint32Body_t * body = (responseUint32Body_t*)responseUint32.mtext;
        if ( body->value ==  01234567U  )
        {
            printf("meter app - GET proper RESPONSE! \r\n");
        }
        else
        {
            printf("GET NOT EXPECTED RESPONSE!!!\r\n");
        }  
    }

    sleep(1);

    request.mtype = SINGLE_GET_REQUEST;
    pBody->attribute = INSTATNTENOUS_PHASE_VOLTAGE;
    pBody->instance = 2U;
    printf("Send request for INSTATNTENOUS_PHASE_VOLTAGE. Existing instance!\r\n");
    PushMessageToQueue( (void*)&request, SINGLE_GET_REQUEST, serverQueueId );

    sleep(1);

    if ( GetMessageFromQueue( (void*)&responseUint32, UINT32_RESPONSE, appQueueId ) )
    {
        responseUint32Body_t * body = (responseUint32Body_t*)responseUint32.mtext;
        if ( body->status == OK )
        {
            printf("meter app - GET proper RESPONSE!. Voltage = %i\r\n", body->value);
        } 
        else
        {
            printf("meter app - bad status received... status = %i; value + %i\r\n", body->status, body->value);
        }
        
    }

    sleep(1);

    request.mtype = SINGLE_GET_REQUEST;
    pBody->attribute = INSTATNTENOUS_PHASE_VOLTAGE;
    pBody->instance = 102U;
    printf("Send request for INSTATNTENOUS_PHASE_VOLTAGE. Existing instance!\r\n");
    PushMessageToQueue( (void*)&request, SINGLE_GET_REQUEST, serverQueueId );

    sleep(1);

    if ( GetMessageFromQueue( (void*)&responseUint32, UINT32_RESPONSE, appQueueId ) )
    {
        responseUint32Body_t * body = (responseUint32Body_t*)responseUint32.mtext;
        if ( body->status == BAD_INSTANCE )
        {
            printf("meter app - GET expected RESPONSE! status = %i; value + %i\r\n", body->status, body->value);
        } 
        else
        {
            printf("meter app - GET unexpected Response! status = %i; value + %i\r\n", body->status, body->value);
        }
        
    }

    sleep(1);

    request.mtype = SINGLE_GET_REQUEST;
    pBody->attribute = INSTATNTENOUS_PHASE_CURRENT;
    pBody->instance = 2U;
    printf("Send request for INSTATNTENOUS_PHASE_CURRENT. Existing instance!\r\n");
    PushMessageToQueue( (void*)&request, SINGLE_GET_REQUEST, serverQueueId );

    sleep(1);

    if ( GetMessageFromQueue( (void*)&responseUint32, UINT32_RESPONSE, appQueueId ) )
    {
        responseUint32Body_t * body = (responseUint32Body_t*)responseUint32.mtext;
        if ( body->status == OK )
        {
            printf("meter app - GET proper RESPONSE!. Current = %i\r\n", body->value);
        } 
        else
        {
            printf("meter app - bad status received... status = %i; value = %i\r\n", body->status, body->value);
        }
        
    }

    sleep(1);

    request.mtype = SINGLE_GET_REQUEST;
    pBody->attribute = INSTATNTENOUS_PHASE_CURRENT;
    pBody->instance = 102U;
    printf("Send request for INSTATNTENOUS_PHASE_CURRENT. Existing instance!\r\n");
    PushMessageToQueue( (void*)&request, SINGLE_GET_REQUEST, serverQueueId );

    sleep(1);

    if ( GetMessageFromQueue( (void*)&responseUint32, UINT32_RESPONSE, appQueueId ) )
    {
        responseUint32Body_t * body = (responseUint32Body_t*)responseUint32.mtext;
        if ( body->status == BAD_INSTANCE )
        {
            printf("meter app - GET expected RESPONSE! status = %i; value + %i\r\n", body->status, body->value);
        } 
        else
        {
            printf("meter app - GET unexpected Response! status = %i; value + %i\r\n", body->status, body->value);
        }
        
    }
}

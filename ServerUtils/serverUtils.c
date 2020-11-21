//--External includes-------------------------------------------------
#include <stdio.h>              // pritnf()
#include <sys/stat.h>           // mkfifo()
#include <unistd.h>             // usleep()
#include <stdint.h>             // uint32_t
#include <stdbool.h>            // bool
//--------------------------------------------------------------------

//--Local includes----------------------------------------------------
#include "../commonIncludes/metering_interface.h" // DEV_FILE, 
#include "../commonIncludes/serverMessages.h"
#include "../commonIncludes/projectGenerals.h"    // SERVER_VERSION
#include "serverUtils.h"
#include "queueUtils.h"
#include "dataUtils.h"
#include "messageUtils.h"
//--------------------------------------------------------------------

//--Defines-----------------------------------------------------------
#define SU_DBG_PRNT
//--------------------------------------------------------------------

//--Consts------------------------------------------------------------
const uint32_t ATTRIBUTE_SERVER_VERSION = SERVER_VERSION; //8 Digit
const uint32_t ATTRIBUTE_METER_NUMBER   = 01234567U; //8 Digit

//--------------------------------------------------------------------

//--File Scope Global Variables---------------------------------------
static int serverReceiveQueueId = 0;
//--------------------------------------------------------------------

//--Private Function Declaration--------------------------------------
void InitFifo( void );
void HandleIncomeMessages( void );
void HandleSingleGetRequest( void );
void HandleResetRequest( void );
//--------------------------------------------------------------------

void StartServer( void )
{
    printf("Meter_srv is running...\r\n");
    while (1)
    {
        ReadStructFromDev();
        StoreMaxMinValues();
        HandleIncomeMessages();
        //usleep(10000); // each secunde
    }
}

void InitServer( void )
{
    InitFifo();
    InitServerMessageQueue();
    (void)GetServerQueueId(&serverReceiveQueueId);
    CleanSrvQueue();
}

void InitFifo( void )
{
    mkfifo(DEV_FILE, 0666);
}

void HandleIncomeMessages( void )
{
    while ( GetNumberOfMessagesInQueue(serverReceiveQueueId) )
    {
        HandleSingleGetRequest();
        HandleResetRequest();
    }
}


void HandleSingleGetRequest( void )
{
    requestSingleGet_t requestMessage;
    if ( GetMessageFromServerQueue( (void *)&requestMessage, SINGLE_GET_REQUEST ) )
    {
        bool responseStatus = false;
        requestSingleGetBody_t * messageBody = (requestSingleGetBody_t*)requestMessage.mtext;
        long responseQueueId = messageBody->queueResponseId;
        uint32_t requestId = messageBody->requestId;
        attributesToGet_t attribute = messageBody->attribute;
        uint8_t instance = messageBody->instance;
        shortConfirmationValues_t status = ERROR;
        uint32_t returnUint32Value = 0U;
        switch ( attribute )
        {
            case METER_NUMBER:
                #ifdef SU_DBG_PRNT
                    printf("HandleSingleGetRequest::METER_NUMBER rID = %i i = %i v = %i stat = %i\r\n", requestId, instance, ATTRIBUTE_METER_NUMBER, OK);
                #endif
                responseStatus = ResponseUint32( ATTRIBUTE_METER_NUMBER, OK, responseQueueId, requestId );
                break;
        
            case METER_SERVER_VERSION:
                #ifdef SU_DBG_PRNT
                    printf("HandleSingleGetRequest::METER_SERVER_VERSION rID = %i i = %i v = %i stat = %i\r\n", requestId, instance, ATTRIBUTE_SERVER_VERSION, OK);
                #endif
                responseStatus = ResponseUint32( ATTRIBUTE_SERVER_VERSION, OK, responseQueueId, requestId );
                break;

            case INSTATNTENOUS_PHASE_VOLTAGE:
                returnUint32Value = GetInstatntenousPhaseVoltage( &status, instance );
                #ifdef SU_DBG_PRNT
                    printf("HandleSingleGetRequest::INSTATNTENOUS_PHASE_VOLTAGE rID = %i i = %i v = %i stat = %i\r\n", requestId,instance, returnUint32Value, status);
                #endif
                responseStatus = ResponseUint32( returnUint32Value, status, responseQueueId, requestId );
                break;

            case INSTATNTENOUS_PHASE_CURRENT:
                returnUint32Value = GetInstatntenousPhaseCurrent( &status, instance );
                #ifdef SU_DBG_PRNT
                    printf("HandleSingleGetRequest::INSTATNTENOUS_PHASE_CURRENT rID = %i i = %i v = %i stat = %i\r\n",  requestId,instance, returnUint32Value, status);
                #endif
                responseStatus = ResponseUint32( returnUint32Value, status, responseQueueId, requestId );
                break;

            case VOLTAGE_PHASE_ANGLE:
                returnUint32Value = GetPhaseAngle( &status, instance, ANGLE_VOLTAGE );
                #ifdef SU_DBG_PRNT
                    printf("HandleSingleGetRequest::VOLTAGE_PHASE_ANGLE rID = %i i = %i v = %i stat = %i\r\n",  requestId,instance, returnUint32Value, status);
                #endif
                responseStatus = ResponseUint32( returnUint32Value, status, responseQueueId, requestId );
                break;
            case CURRENT_PHASE_ANGLE:
                returnUint32Value = GetPhaseAngle( &status, instance, ANGLE_CURRENT );
                #ifdef SU_DBG_PRNT
                    printf("HandleSingleGetRequest::CURRENT_PHASE_ANGLE rID = %i i = %i v = %i stat = %i\r\n",  requestId,instance, returnUint32Value, status);
                #endif
                responseStatus = ResponseUint32( returnUint32Value, status, responseQueueId, requestId );
                break;
            case MINIMUM_PHASE_VOLTAGE:
                returnUint32Value = GetMinMaxPhaseValue( &status, instance, ANGLE_VOLTAGE, ANGLE_MIN );
                #ifdef SU_DBG_PRNT
                    printf("HandleSingleGetRequest::MINIMUM_PHASE_VOLTAGE rID = %i i = %i v = %i stat = %i\r\n",  requestId,instance, returnUint32Value, status);
                #endif
                responseStatus = ResponseUint32( returnUint32Value, status, responseQueueId, requestId );
                break;
            case MAXIMUM_PHASE_VOLTAGE:
                returnUint32Value = GetMinMaxPhaseValue( &status, instance, ANGLE_VOLTAGE, ANGLE_MAX );
                #ifdef SU_DBG_PRNT
                    printf("HandleSingleGetRequest::MAXIMUM_PHASE_VOLTAGE rID = %i i = %i v = %i stat = %i\r\n",  requestId,instance, returnUint32Value, status);
                #endif
                responseStatus = ResponseUint32( returnUint32Value, status, responseQueueId, requestId );
                break;
            case MINIMUM_PHASE_CURRENT:
                returnUint32Value = GetMinMaxPhaseValue( &status, instance, ANGLE_CURRENT, ANGLE_MIN );
                #ifdef SU_DBG_PRNT
                    printf("HandleSingleGetRequest::MINIMUM_PHASE_CURRENT rID = %i i = %i v = %i stat = %i\r\n",  requestId,instance, returnUint32Value, status);
                #endif
                responseStatus = ResponseUint32( returnUint32Value, status, responseQueueId, requestId );
                break;
            case MAXIMUM_PHASE_CURRENT:
                returnUint32Value = GetMinMaxPhaseValue( &status, instance, ANGLE_CURRENT, ANGLE_MAX );
                #ifdef SU_DBG_PRNT
                    printf("HandleSingleGetRequest::MAXIMUM_PHASE_CURRENT rID = %i i = %i v = %i stat = %i\r\n",  requestId,instance, returnUint32Value, status);
                #endif
                responseStatus = ResponseUint32( returnUint32Value, status, responseQueueId, requestId );
                break;
        
            default:
                // Attribute not supported
                #ifdef SU_DBG_PRNT
                    printf("HandleSingleGetRequest ATTRIBUTE NOT SUPPORTED!\r\n");
                #endif
                responseStatus = ResponseShortConfirmation( BAD_ATTRIBUTE, responseQueueId, requestId );
                break;
        }

        if ( !responseStatus )
        {
            #ifdef SU_DBG_PRNT
                printf("ERROR DURING RESPONDING WITH UINT32_T!\r\n");
            #endif

        }
    }
}

void HandleResetRequest( void )
{
    requestReset_t requestMessage;
    if ( GetMessageFromServerQueue( (void *)&requestMessage, RESET_REQUEST ) )
    {
        bool responseStatus = false;
        requestResetBody_t * messageBody = (requestResetBody_t*)requestMessage.mtext;
        long responseQueueId = messageBody->queueResponseId;
        uint32_t requestId = messageBody->requestId;
        attributesToGet_t attribute = messageBody->attribute;
        uint8_t instance = messageBody->instance;
        shortConfirmationValues_t status = ERROR;
        switch ( attribute )
        {
            case MINIMUM_PHASE_VOLTAGE:
                status = ResetMinMaxPhaseValue( instance, ANGLE_VOLTAGE, ANGLE_MIN );
                #ifdef SU_DBG_PRNT
                    printf("HandleResetRequest::MINIMUM_PHASE_VOLTAGE rID = %i i = %i stat = %i\r\n", requestId, instance, status);
                #endif
                responseStatus = ResponseShortConfirmation( status, responseQueueId, requestId );
                break;
            case MAXIMUM_PHASE_VOLTAGE:
                status = ResetMinMaxPhaseValue( instance, ANGLE_VOLTAGE, ANGLE_MAX );
                #ifdef SU_DBG_PRNT
                    printf("HandleResetRequest::MAXIMUM_PHASE_VOLTAGE rID = %i i = %i stat = %i\r\n", requestId, instance, status);
                #endif
                responseStatus = ResponseShortConfirmation( status, responseQueueId, requestId );
                break;
            case MINIMUM_PHASE_CURRENT:
                status = ResetMinMaxPhaseValue( instance, ANGLE_CURRENT, ANGLE_MIN );
                #ifdef SU_DBG_PRNT
                    printf("HandleResetRequest::MINIMUM_PHASE_CURRENT rID = %i i = %i stat = %i\r\n", requestId, instance, status);
                #endif
                responseStatus = ResponseShortConfirmation( status, responseQueueId, requestId );
                break;
            case MAXIMUM_PHASE_CURRENT:
                status = ResetMinMaxPhaseValue( instance, ANGLE_CURRENT, ANGLE_MIN );
                #ifdef SU_DBG_PRNT
                    printf("HandleResetRequest::MINIMUM_PHASE_CURRENT rID = %i i = %i stat = %i\r\n", requestId, instance, status);
                #endif
                responseStatus = ResponseShortConfirmation( status, responseQueueId, requestId );
                break;
            default:
                // Attribute not supported
                #ifdef SU_DBG_PRNT
                    printf("HandleResetRequest ATTRIBUTE NOT SUPPORTED!\r\n");
                #endif
                responseStatus = ResponseShortConfirmation( BAD_ATTRIBUTE, responseQueueId, requestId );
                break;
        }

        if ( !responseStatus )
        {
            #ifdef SU_DBG_PRNT
                printf("ERROR DURING RESPONDING!\r\n");
            #endif

        }
    }
}

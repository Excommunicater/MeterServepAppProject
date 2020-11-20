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
//--------------------------------------------------------------------

//--Defines-----------------------------------------------------------
//#define DEBUG_PRINTOUT
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
//--------------------------------------------------------------------

void StartServer( void )
{
    printf("Meter_srv is running...\r\n");
    while (1)
    {
        ReadStructFromDev();
        StoreMaxMinValues();
        HandleIncomeMessages();
        usleep(10000); // each secunde
    }
}

void InitServer( void )
{
    InitFifo();
    InitServerMessageQueue();
}

void InitFifo( void )
{
    mkfifo(DEV_FILE, 0666);
}

void HandleIncomeMessages( void )
{
    HandleSingleGetRequest();
}


void HandleSingleGetRequest( void )
{
    requestSingleGet_t requestMessage;
    while ( GetMessageFromServerQueue( (void *)&requestMessage, SINGLE_GET_REQUEST ) )
    {
        bool responseStatus = false;
        requestSingleGetBody_t * messageBody = (requestSingleGetBody_t*)requestMessage.mtext;
        long responseQueueId = messageBody->queueResponseId;
        uint32_t requestId = messageBody->requestId;
        attributesToGet_t attribute = messageBody->attribute;
        uint8_t instance = messageBody->instance;
        switch ( attribute )
        {
            case METER_NUMBER:
                #ifdef DEBUG_PRINTOUT
                    printf("HandleSingleGetRequest::METER_NUMBER rID = %i i = %i v = %i stat = %i\r\n", requestId, instance, ATTRIBUTE_METER_NUMBER, OK);
                #endif
                responseStatus = ResponseUint32( ATTRIBUTE_METER_NUMBER, OK, responseQueueId, requestId );
                break;
        
            case METER_SERVER_VERSION:
                #ifdef DEBUG_PRINTOUT
                    printf("HandleSingleGetRequest::METER_SERVER_VERSION rID = %i i = %i v = %i stat = %i\r\n", requestId, instance, ATTRIBUTE_SERVER_VERSION, OK);
                #endif
                responseStatus = ResponseUint32( ATTRIBUTE_SERVER_VERSION, OK, responseQueueId, requestId );
                break;

            case INSTATNTENOUS_PHASE_VOLTAGE:
            {
                uint8_t  status = 0U;
                uint32_t returnValue = GetInstatntenousPhaseVoltage( &status, instance );
                #ifdef DEBUG_PRINTOUT
                    printf("HandleSingleGetRequest::INSTATNTENOUS_PHASE_VOLTAGE rID = %i i = %i v = %i stat = %i\r\n", requestId,instance, returnValue, status);
                #endif
                responseStatus = ResponseUint32( returnValue, status, responseQueueId, requestId );
                break;
            }

            case INSTATNTENOUS_PHASE_CURRENT:
            {
                uint8_t  status = 0U;
                uint32_t returnValue = GetInstatntenousPhaseCurrent( &status, instance );
                #ifdef DEBUG_PRINTOUT
                    printf("HandleSingleGetRequest::INSTATNTENOUS_PHASE_CURRENT rID = %i i = %i v = %i stat = %i\r\n",  requestId,instance, returnValue, status);
                #endif
                responseStatus = ResponseUint32( returnValue, status, responseQueueId, requestId );
                break;
            }

            case VOLTAGE_PHASE_ANGLE:
            {
                int8_t   status = 0U;
                uint32_t returnValue = GetPhaseAngle( &status, instance, ANGLE_VOLTAGE );
                #ifdef DEBUG_PRINTOUT
                    printf("HandleSingleGetRequest::VOLTAGE_PHASE_ANGLE rID = %i i = %i v = %i stat = %i\r\n",  requestId,instance, returnValue, status);
                #endif
                responseStatus = ResponseUint32( returnValue, status, responseQueueId, requestId );
                break;
            }
            case CURRENT_PHASE_ANGLE:
            {
                int8_t   status = 0U;
                uint32_t returnValue = GetPhaseAngle( &status, instance, ANGLE_CURRENT );
                #ifdef DEBUG_PRINTOUT
                    printf("HandleSingleGetRequest::CURRENT_PHASE_ANGLE rID = %i i = %i v = %i stat = %i\r\n",  requestId,instance, returnValue, status);
                #endif
                responseStatus = ResponseUint32( returnValue, status, responseQueueId, requestId );
                break;
            }           
        
            default:
                // Attribute not supported
                break;
        }

        if ( !responseStatus )
        {
            #ifdef DEBUG_PRINTOUT
                printf("ERROR DURING RESPONDING WITH UINT32_T!\r\n");
            #endif
        }
    }
}

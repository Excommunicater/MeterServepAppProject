//--External includes-------------------------------------------------
#include <stdio.h>              // pritnf()
#include <fcntl.h>              // open(), read()
#include <sys/stat.h>           // mkfifo()
#include <unistd.h>             // usleep()
#include <sys/ipc.h>            // key_t
#include <errno.h>              // erno
#include <string.h>             // memcpy()
#include <sys/msg.h>            // msgget()
#include <stdlib.h>             // exit()
//--------------------------------------------------------------------

//--Defines-----------------------------------------------------------
//#define DEBUG_PRINTOUT
//--------------------------------------------------------------------

//--Local includes----------------------------------------------------
#include "../commonIncludes/metering_interface.h" // DEV_FILE, 
#include "../commonIncludes/serverMessages.h"     //
#include "serverUtils.h"        //
//--------------------------------------------------------------------

//--Local Structures--------------------------------------------------
typedef struct messageTypeSize
{
    long   type;
    size_t size;
} messageTypeSize_t;

typedef enum angleValue
{
    VOLTAGE = 0,
    CURRENT
} angleValue_t;
//--------------------------------------------------------------------

//--Consts------------------------------------------------------------
const uint32_t ATTRIBUTE_SERVER_VERSION = 01234567U; //8 Digit
const uint32_t ATTRIBUTE_METER_NUMBER   = 98765432U; //8 Digit

const messageTypeSize_t SIZE_TYPE_ARRAY[NUMBER_OF_MESSAGE_TYPES] =
{
    { SINGLE_GET_REQUEST,           sizeof(requestSingleGetBody_t) },
    { RESET_REQUEST,                1 },
    { SET_EVENT_REQUEST,            1 },
    { RESET_EVENT_REQUEST,          1 },
    { SHORT_CONFIRMATION_RESPONSE,  sizeof(responseShortConfirmationBody_t) },
    { UINT32_RESPONSE,              sizeof(responseUint32Body_t) },
    { UINT64_RESPONSE,              sizeof(responseUint64_t)} 
};
//--------------------------------------------------------------------

//--Global Variables--------------------------------------------------
bool isQueueCreated = false;
int serverReceiveQueueId = 0;
meter_hw_registers_t lastReadHardwareRegister;
//--------------------------------------------------------------------

//--Private Function Declaration--------------------------------------
void InitFifo( void );
void ReadStructFromDev( meter_hw_registers_t * pMeterReg );
void HandleIncomeMessages( void );
bool GetMessageFromServerQueue( void * message, long messageType );
void HandleSingleGetRequest( void );
bool PushMessageToQueue( void * message, long messageType, int queueId );
bool ResponseUint32( uint32_t valueToResponse, uint8_t status, long responseQueue, uint32_t requestId );
uint32_t GetInstatntenousPhaseVoltage( uint8_t * status, uint8_t phase );
uint32_t GetInstatntenousPhaseCurrent( uint8_t * status, uint8_t phase );
uint32_t GetPhaseAngle( uint8_t * status, uint8_t phase, angleValue_t valueToGet );
void RemoveQueue( int queueId );
//--------------------------------------------------------------------
int InitMessageQueue( const char * filePath )
{
    key_t key = ftok(filePath, PROJECT_ID);

    if ( key < 0 )
    {
        ReportAndExit("Couldn't get key...\r\n");
    }
    printf("Properly generated key! Key: %i\r\n", key);

    int queueId = msgget(key, 0666 | IPC_CREAT);
    if ( queueId < 0 )
    {
        ReportAndExit("Couldn't get queue id...\r\n");
    }
    printf("Properly generated queue ID! ID: %i\r\n", queueId);
    isQueueCreated = true;

    return queueId;

}

int InitServerMessageQueue( void )
{
    return InitMessageQueue( SERVER_PATH_NAME );
}

void InitServer( void )
{
    InitFifo();
    serverReceiveQueueId = InitServerMessageQueue();
}

void StartServer( void )
{
    printf("Meter_srv is running...\r\n");
    while (1)
    {
        ReadStructFromDev( &lastReadHardwareRegister );
        HandleIncomeMessages();
        usleep(10000); // each secunde
    }
}

void ReportAndExit( const char* errorMsg )
{
    perror(errorMsg);
    exit(-1);
}

void HandleIncomeMessages( void )
{

    HandleSingleGetRequest();
}

bool GetMessageFromServerQueue( void * message, long messageType )
{
    return GetMessageFromQueue( message, messageType, serverReceiveQueueId );
}

bool GetMessageFromQueue( void * message, long messageType, int queueId )
{
    bool status = (bool)( NumberOfMessagesInQueue( queueId ) != 0U );
    if ( ( message != (void*)NULL ) && status )
    {
        if ( msgrcv(queueId, 
                    message, 
                    GetMessageSize( messageType ), 
                    messageType, 
                    MSG_NOERROR | IPC_NOWAIT ) == -1 )
        {
            if ( errno != ENOMSG ) 
            {
                ReportAndExit("Unexpected error while getting message from queue...\r\n");
            }
        }
        else
        {
            status = true;
        }
    }
    return status;
}


bool PushMessageToQueue( void * message, long messageType, int queueId )
{
    bool status = false;
    if ( message != (void*)NULL )
    {
        if ( msgsnd(queueId, message, GetMessageSize( messageType ), IPC_NOWAIT) == -1) 
        {
            ReportAndExit("Unexpected error pushing message to queue...");
        }
        else
        {
            status = true;
        }
    }
    return status;
}

size_t GetMessageSize( long type )
{
    size_t response = 0;
    for ( uint8_t i = 0U; i < NUMBER_OF_MESSAGE_TYPES; i++)
    {
        if ( SIZE_TYPE_ARRAY[i].type == type )
        {
            response = SIZE_TYPE_ARRAY[i].size;
            break;
        }
    }
    return response;
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
                uint32_t returnValue = GetPhaseAngle( &status, instance, VOLTAGE );
                #ifdef DEBUG_PRINTOUT
                    printf("HandleSingleGetRequest::VOLTAGE_PHASE_ANGLE rID = %i i = %i v = %i stat = %i\r\n",  requestId,instance, returnValue, status);
                #endif
                responseStatus = ResponseUint32( returnValue, status, responseQueueId, requestId );
                break;
            }
            case CURRNT_PHASE_ANGLE:
            {
                int8_t   status = 0U;
                uint32_t returnValue = GetPhaseAngle( &status, instance, CURRENT );
                #ifdef DEBUG_PRINTOUT
                    printf("HandleSingleGetRequest::CURRNT_PHASE_ANGLE rID = %i i = %i v = %i stat = %i\r\n",  requestId,instance, returnValue, status);
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

bool ResponseUint32( uint32_t valueToResponse, uint8_t status, long responseQueue, uint32_t requestId )
{
    bool funStatus = false;
    responseUint32_t response;
    response.mtype = UINT32_RESPONSE;

    responseUint32Body_t * responseBody = (responseUint32Body_t*)response.mtext;
    responseBody->requestId = requestId;
    responseBody->value     = valueToResponse;
    responseBody->status    = status;
    
    if ( PushMessageToQueue((void*)&response, UINT32_RESPONSE, responseQueue ) )
    {
        #ifdef DEBUG_PRINTOUT
            printf("Respond properly with UINT32_RESPONSE \r\n" );
        #endif
        funStatus = true;
    }

    return funStatus;
}

uint32_t GetInstatntenousPhaseVoltage( uint8_t * status, uint8_t phase )
{
    uint32_t response = 0U;

    if ( status == (uint8_t*)NULL )
    {
        ReportAndExit("GetInstatntenousPhaseVoltage - passed NULL argument!");
    }

    if ( phase <= PHASE_CNT )
    {
        response = lastReadHardwareRegister.per_phase[phase].v;
        *status  = OK;
    }
    else
    {
        *status = BAD_INSTANCE;
    }
    return response;
}

uint32_t GetInstatntenousPhaseCurrent( uint8_t * status, uint8_t phase )
{
    uint32_t response = 0U;

    if ( status == (uint8_t*)NULL )
    {
        ReportAndExit("GetInstatntenousPhaseCurrent - passed NULL argument!");
    }

    if ( phase <= PHASE_CNT )
    {
        response = lastReadHardwareRegister.per_phase[phase].i;
        *status  = OK;
    }
    else
    {
        *status = BAD_INSTANCE;
    }
    return response;
}

uint32_t GetPhaseAngle( uint8_t * status, uint8_t phase, angleValue_t valueToGet )
{
    uint32_t response = 0U;
    if ( status == (uint8_t*)NULL )
    {
        ReportAndExit("GetPhaseAngle - passed NULL argument!");
    }
    if ( ( valueToGet > CURRENT ) || ( valueToGet < VOLTAGE ) )
    {
        ReportAndExit("GetPhaseAngle - passed bad value to get!");
    }

    if ( phase <= PHASE_CNT )
    {
        if ( valueToGet == VOLTAGE )
        {
            response = lastReadHardwareRegister.voltage_angles[phase];
        }
        else
        {
            response = lastReadHardwareRegister.current_angles[phase];
        }
        *status  = OK;
    }
    else
    {
        *status = BAD_INSTANCE;
    }
    return response;
}


void ReadStructFromDev( meter_hw_registers_t * pMeterReg )
{
    int fifoFile = open(DEV_FILE, O_RDONLY);
    read(fifoFile, pMeterReg, sizeof(meter_hw_registers_t)); 
    close(fifoFile);
    //printf("V[2] = %i A[2] = %i A+[2] = %li A-[2] = %li\r\n", pMeterReg->per_phase[2].v, pMeterReg->per_phase[2].i, pMeterReg->per_phase[2].ai,  pMeterReg->per_phase[2].ae);
}

void InitFifo( void )
{
    mkfifo(DEV_FILE, 0666);
}

uint32_t NumberOfMessagesInQueue( int queueId )
{
    struct msqid_ds queueStructure;
    if ( msgctl(queueId, IPC_STAT, &queueStructure) != 0 )
    {
        ReportAndExit("NumberOfMessagesInQueue - Problem getting queue struct...");
    }
    //msg_qnum is msqid_ds type which is unsigned long.
    return (uint32_t)queueStructure.msg_qnum; 
}

void RemoveQueue( int queueId )
{
    if ( NumberOfMessagesInQueue( queueId ) != 0U )
    {
        // Queue not empty - cleanup the Queue
        struct msqid_ds queueStructure; // This is ignored with IPC_RMID
        if ( msgctl(queueId, IPC_RMID, &queueStructure) != 0 )
        {
            ReportAndExit("Error during queue cleanup...\r\n");
        }
        else
        {
            printf("Properly clean queue!\r\n");
        }
    }

}
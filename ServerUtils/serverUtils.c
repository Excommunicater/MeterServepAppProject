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

//--------------------------------------------------------------------

//--Local includes----------------------------------------------------
#include "../commonIncludes/metering_interface.h" // DEV_FILE
#include "serverMessages.h"     //
#include "serverUtils.h"        //
//--------------------------------------------------------------------

//--Local Structures--------------------------------------------------
typedef struct messageTypeSize
{
    long   type;
    size_t size;
} messageTypeSize_t;
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
int serverReceiveQueueId = 0;
//meter_hw_registers_t lastReadHardwareRegister;
//--------------------------------------------------------------------

//--Private Function Declaration--------------------------------------
//void InitFifo( void );
//void ReadStructFromDev( meter_hw_registers_t * pMeterReg );
void HandleIncomeMessages( void );
bool GetMessageFromServerQueue( void * message, long messageType );
void HandleSingleGetRequest( void );
bool PushMessageToQueue( void * message, long messageType, int queueId );
bool ResponseUint32( uint32_t valueToResponse, uint8_t status, long responseQueue );
uint32_t GetInstatntenousPhaseVoltage( uint8_t * status, uint8_t phase );
uint32_t GetInstatntenousPhaseCurrent( uint8_t * status, uint8_t phase );
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

    return queueId;

}

int InitServerMessageQueue( void )
{
    return InitMessageQueue( SERVER_PATH_NAME );
}

void InitServer( void )
{
    //InitFifo();
    serverReceiveQueueId = InitServerMessageQueue();
}

void StartServer( void )
{
    printf("Meter_srv is running...\r\n");
    while (1)
    {
        //ReadStructFromDev( &lastReadHardwareRegister );
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
    bool status = false;
    if ( message != (void*)NULL )
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
            //printf("Do not find any messages\r\n");
        }
        else
        {
            printf("Get message!\r\n");
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
            printf("queue id = %i\r\n", queueId);
            ReportAndExit("Unexpected error pushing message to queue...");
        }
        else
        {
            printf("Message sent! queueId = %i messageType = %li\r\n", queueId, messageType );
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
    if ( GetMessageFromServerQueue( (void *)&requestMessage, SINGLE_GET_REQUEST ) )
    {
        bool responseStatus = false;
        requestSingleGetBody_t * messageBody = (requestSingleGetBody_t*)requestMessage.mtext;
        long responseQueueId = messageBody->queueResponseId;
        attributesToGet_t attribute = messageBody->attribute;
        uint8_t instance = messageBody->instance;
        switch ( attribute )
        {
            case METER_NUMBER:
                responseStatus = ResponseUint32( ATTRIBUTE_METER_NUMBER, OK, responseQueueId );
                break;
        
            case METER_SERVER_VERSION:
                responseStatus = ResponseUint32( ATTRIBUTE_SERVER_VERSION, OK, responseQueueId );
                break;

            case INSTATNTENOUS_PHASE_VOLTAGE:
            {
                uint8_t  status = 0U;
                uint32_t returnValue = GetInstatntenousPhaseVoltage( &status, instance );
                printf("HandleSingleGetRequest::INSTATNTENOUS_PHASE_VOLTAGE instance = %i value = %i status %i\r\n", instance, returnValue, status);
                responseStatus = ResponseUint32( returnValue, status, responseQueueId );
                break;
            }

            case INSTATNTENOUS_PHASE_CURRENT:
            {
                uint8_t  status = 0U;
                uint32_t returnValue = GetInstatntenousPhaseCurrent( &status, instance );
                printf("HandleSingleGetRequest::INSTATNTENOUS_PHASE_CURRENT instance = %i value = %i status %i\r\n", instance, returnValue, status);
                responseStatus = ResponseUint32( returnValue, status, responseQueueId );
                break;
            }
        
            default:
                // Attribute not supported
                break;
        }

        if ( !responseStatus )
        {
            // HANDLE IT!
        }
    }
}

bool ResponseUint32( uint32_t valueToResponse, uint8_t status, long responseQueue )
{
    bool funStatus = false;
    responseUint32_t response;
    response.mtype = UINT32_RESPONSE;

    responseUint32Body_t * responseBody = (responseUint32Body_t*)response.mtext;
    responseBody->value  = valueToResponse;
    responseBody->status = status;
    
    if ( PushMessageToQueue((void*)&response, UINT32_RESPONSE, responseQueue ) )
    {
        printf("Respond properly with UINT32_RESPONSE \r\n" );
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

    /*if ( phase <= PHASE_CNT )
    {
        response = lastReadHardwareRegister.per_phase[phase].v;
        *status  = OK;
    }
    else
    {
        *status = BAD_INSTANCE;
    }*/
    return response;
}

uint32_t GetInstatntenousPhaseCurrent( uint8_t * status, uint8_t phase )
{
    uint32_t response = 0U;

    if ( status == (uint8_t*)NULL )
    {
        ReportAndExit("GetInstatntenousPhaseCurrent - passed NULL argument!");
    }

    /*if ( phase <= PHASE_CNT )
    {
        response = lastReadHardwareRegister.per_phase[phase].i;
        *status  = OK;
    }
    else
    {
        *status = BAD_INSTANCE;
    }*/
    return response;
}


/*void ReadStructFromDev( meter_hw_registers_t * pMeterReg )
{
    int fifoFile = open(DEV_FILE, O_RDONLY);
    read(fifoFile, pMeterReg, sizeof(meter_hw_registers_t)); 
    close(fifoFile);
    printf("V[2] = %i A[2] = %i A+[2] = %li A-[2] = %li\r\n", pMeterReg->per_phase[2].v, pMeterReg->per_phase[2].i, pMeterReg->per_phase[2].ai,  pMeterReg->per_phase[2].ae);
}

void InitFifo( void )
{
    mkfifo(DEV_FILE, 0666);
}
*/
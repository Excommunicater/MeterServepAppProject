//--External includes-------------------------------------------------
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
//--------------------------------------------------------------------

//--Project includes--------------------------------------------------
#include "queueUtils.h"
#include "messagingUtils.h"
#include "projectGenerals.h"
#include "errorHandling.h"
//--------------------------------------------------------------------

//--Defines-----------------------------------------------------------
#define APPLICATION_FILE "./ApplicationExample/main.c"
//--------------------------------------------------------------------

//--Local Structures--------------------------------------------------
typedef struct dataToDisplay
{
    uint32_t serverVersion;
    uint32_t meterNumber;
    uint32_t IPV1;
    uint32_t IPV2;
    uint32_t IPV3;
    uint32_t IPC1;
    uint32_t IPC2;
    uint32_t IPC3;
    uint32_t VA1;
    uint32_t VA2;
    uint32_t VA3;
    uint32_t CA1;
    uint32_t CA2;
    uint32_t CA3;
    uint32_t lostMessages;
} dataToDisplay_t;
//--------------------------------------------------------------------

//--File scope Variables----------------------------------------------
static int serverQueue = 0;
static int applicationQueue = 0;
static dataToDisplay_t dataToDisplay;
//--------------------------------------------------------------------

//--Function Declaration----------------------------------------------
static void InitApplication( void );
static void GetDataFromServer( void );
static void PrintData( void );
static uint32_t GetUniqueRequestId( void );
static void GetUint32Blocking( attributesToGet_t attribute, uint32_t instance, uint32_t * pDestination );
//--------------------------------------------------------------------

int main( void )
{
    InitApplication();
    while(1)
    {
        GetDataFromServer();
        PrintData();
        usleep(500000);
    }
    return 0;
}

static void InitApplication( void )
{
    applicationQueue = InitMessageQueue( APPLICATION_FILE );
    CleanQueue(applicationQueue);
    serverQueue = InitMessageQueue( SERVER_PATH_NAME );
}

static void GetDataFromServer( void )
{
    static bool getOnce = true;
    bool status = false;
    if ( getOnce )
    {
        getOnce = false;
        GetUint32Blocking( METER_SERVER_VERSION,    0, &(dataToDisplay.serverVersion) );
        GetUint32Blocking( METER_NUMBER,            0, &(dataToDisplay.meterNumber) );
    }
    GetUint32Blocking( INSTATNTENOUS_PHASE_VOLTAGE, 0, &(dataToDisplay.IPV1) );
    GetUint32Blocking( INSTATNTENOUS_PHASE_VOLTAGE, 1, &(dataToDisplay.IPV2) );
    GetUint32Blocking( INSTATNTENOUS_PHASE_VOLTAGE, 2, &(dataToDisplay.IPV3) );
    GetUint32Blocking( INSTATNTENOUS_PHASE_CURRENT, 0, &(dataToDisplay.IPC1) );
    GetUint32Blocking( INSTATNTENOUS_PHASE_CURRENT, 1, &(dataToDisplay.IPC2) );
    GetUint32Blocking( INSTATNTENOUS_PHASE_CURRENT, 2, &(dataToDisplay.IPC3) );

    GetUint32Blocking( VOLTAGE_PHASE_ANGLE,         0, &(dataToDisplay.VA1) );
    GetUint32Blocking( VOLTAGE_PHASE_ANGLE,         1, &(dataToDisplay.VA2) );
    GetUint32Blocking( VOLTAGE_PHASE_ANGLE,         2, &(dataToDisplay.VA3) );
    GetUint32Blocking( CURRENT_PHASE_ANGLE,         0, &(dataToDisplay.CA1) );
    GetUint32Blocking( CURRENT_PHASE_ANGLE,         1, &(dataToDisplay.CA2) );
    GetUint32Blocking( CURRENT_PHASE_ANGLE,         2, &(dataToDisplay.CA3) );
}

static void PrintData( void )
{
    system("clear");
    printf("--------------------------------------------------------------------\r\n");
    printf("                          Silly APP\r\n");
    printf("METER_SERVER_VERSION: %i  METER_NUMBER = %i\r\n", dataToDisplay.serverVersion, dataToDisplay.meterNumber);
    printf("--------------------------------------------------------------------\r\n");
    printf("PHASE:              |       1       |       2       |       3       |\r\n");
    printf("--------------------------------------------------------------------\r\n");
    printf("VOLTAGE:            |    %07.3f    |    %07.3f    |    %07.3f    |\r\n", (float)(dataToDisplay.IPV1/1000), (float)(dataToDisplay.IPV2/1000), (float)(dataToDisplay.IPV3/1000));
    printf("CURRENT:            |    %07.3f    |    %07.3f    |    %07.3f    |\r\n", (float)(dataToDisplay.IPC1/1000), (float)(dataToDisplay.IPC2/1000), (float)(dataToDisplay.IPC3/1000));
    printf("VOLTAGE ANGLE:      |    %07.3f    |    %07.3f    |    %07.3f    |\r\n", (float)(dataToDisplay.VA1), (float)(dataToDisplay.VA2), (float)(dataToDisplay.VA3));
    printf("CURRENT ANGLE:      |    %07.3f    |    %07.3f    |    %07.3f    |\r\n", (float)(dataToDisplay.CA1), (float)(dataToDisplay.CA2), (float)(dataToDisplay.CA3));
    printf("--------------------------------------------------------------------\r\n");
    printf("LOST MESSAGES: %i\r\n", dataToDisplay.lostMessages);
}

static uint32_t GetUniqueRequestId( void )
{
    static uint32_t id = 1;
    id++;
    return id;
}

static void GetUint32Blocking( attributesToGet_t attribute, uint32_t instance, uint32_t * pDestination )
{
    responseUint32_t responseUint32;
    responseUint32Body_t * pResponseBody = (responseUint32Body_t*)responseUint32.mtext;
    uint32_t requestId = GetUniqueRequestId();
    if ( !SendRequestGetSingle( attribute, instance, serverQueue, applicationQueue, requestId ) )
    {
        ReportAndExit("GetUint32Blocking - Problem sending message...\r\n");
    }
    // Blocking waiting for response
    while ( GetNumberOfMessagesInQueue(applicationQueue) == 0U );

    if ( GetMessageFromQueue( (void*)&responseUint32, UINT32_RESPONSE, applicationQueue ) )
    {
        if ( pResponseBody->status != OK )
        {
            ReportAndExit("GetUint32Blocking - Bad status in response...\r\n");
        }
        else if ( pResponseBody->requestId != requestId )
        {
            ReportAndExit("GetUint32Blocking - Segmentation error...\r\n");
        }
        
        else
        {
            *pDestination = pResponseBody->value;   
        }
    }
    else
    {
        dataToDisplay.lostMessages++;
    }
    
}
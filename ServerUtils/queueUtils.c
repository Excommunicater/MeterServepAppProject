//--External includes-------------------------------------------------
#include <sys/ipc.h>
#include <stdio.h>    
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
//--------------------------------------------------------------------

//--Project includes--------------------------------------------------
#include "queueUtils.h"
#include "errorHandling.h"
#include "messageUtils.h"
#include "../commonIncludes/projectGenerals.h"
//--------------------------------------------------------------------

//--File Scope Global Variables---------------------------------------
static bool serverQueueStatus = false;
static int serverReceiveQueueId = 0;
//--------------------------------------------------------------------


int GetServerQueueId( void )
{
    InitServerMessageQueue();
    return serverReceiveQueueId;
}

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
    serverQueueStatus = true;


    return queueId;

}

void InitServerMessageQueue( void )
{
    serverReceiveQueueId = InitMessageQueue( SERVER_PATH_NAME );
}

bool PushMessageToQueue( void * message, long messageType, int queueId )
{
    bool status = false;
    if ( message != (void*)NULL )
    {
        if ( msgsnd(queueId, message, GetMessageSize( messageType ), IPC_NOWAIT) == -1) 
        {
            RemoveQueue(queueId);
            ReportAndExit("Unexpected error pushing message to queue...");
        }
        else
        {
            status = true;
        }
    }
    return status;
}

uint32_t GetNumberOfMessagesInServerQueue( void )
{
    return GetNumberOfMessagesInQueue( serverReceiveQueueId );
}

uint32_t GetNumberOfMessagesInQueue( int queueId )
{
    
    struct msqid_ds queueStructure;
    if ( msgctl(queueId, IPC_STAT, &queueStructure) != 0 )
    {
        ReportAndExit("GetNumberOfMessagesInQueue - Problem getting queue struct...");
    }
    //msg_qnum is msqid_ds type which is unsigned long.
    return (uint32_t)queueStructure.msg_qnum; 
}

bool GetMessageFromServerQueue( void * message, long messageType  )
{
    return GetMessageFromQueue( message, messageType, serverReceiveQueueId );
}

bool GetMessageFromQueue( void * message, long messageType, int queueId )
{
    bool status = false;
    if ( ( message != (void*)NULL ) && ( GetNumberOfMessagesInQueue( queueId ) != 0U ) )
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

void RemoveQueue( int queueId )
{
    if ( GetNumberOfMessagesInQueue( queueId ) != 0U )
    {
        // Queue not empty - cleanup the Queue
        struct msqid_ds queueStructure; // This is ignored with IPC_RMID
        if ( msgctl(queueId, IPC_RMID, &queueStructure) != 0 )
        {
            ReportAndExit("Error during queue cleanup...\r\n");
        }
        else
        {
            printf("Properly removed queue!\r\n");
        }
    }

}

void CleanAppQueue( int queueId )
{
    uint32_t messagesInQueue = GetNumberOfMessagesInQueue(queueId);
    //RemoveQueue(queueId);
    for (uint32_t i = 0U; i < messagesInQueue; i++)
    {
        responseUint32_t responseUint32;
        responseShortConfirmation_t responseShort;
        responseSubscription_t responseSubscription;
        notificationMessage_t notificationMesssage;
        GetMessageFromQueue( (void*)&responseUint32, UINT32_RESPONSE, queueId );
        GetMessageFromQueue( (void*)&responseShort, SHORT_CONFIRMATION_RESPONSE, queueId );
        GetMessageFromQueue( (void*)&responseSubscription, SUBSCRIPTION_RESPONSE, queueId );
        GetMessageFromQueue( (void*)&notificationMesssage, NOTIFICATION, queueId );
    }
}

void CleanSrvQueue( void )
{
    uint32_t messagesInQueue = GetNumberOfMessagesInQueue(serverReceiveQueueId);
    //Temporary solution - have to get all types of responses....
    for (uint32_t i = 0U; i < messagesInQueue; i++)
    {
        requestSingleGet_t requestSingle;
        requestSingleSet_t requestSingleSet;
        requestReset_t requestReset;
        requestSubscription_t requestSubscription;

        GetMessageFromServerQueue( (void*)&requestSingle, SET_SINGLE_REQUEST );
        GetMessageFromServerQueue( (void*)&requestSingle, GET_SINGLE_REQUEST );
        GetMessageFromServerQueue( (void*)&requestReset, RESET_REQUEST );
        GetMessageFromServerQueue( (void*)&requestSubscription, SUBSCRIBE_REQUEST );
    }
}

/*void LookForMessageInQueue( int queue )
{
    for (long i = 1U; i < 2147483646; i++)
    {
        uint8_t respBuff[2048];
        if ( msgrcv(queue, 
                    respBuff, 
                    128, 
                    i, 
                    MSG_NOERROR | IPC_NOWAIT ) == -1 )
        {
            if ( errno != ENOMSG ) 
            {
                ReportAndExit("Unexpected error while getting message from queue...\r\n");
            }
        }
        else
        {
            printf("FOUND MESSAGE TYPE! %i\r\n", i);
            return i;
        }
    }

    for (long i = -1U; i > -2147483646; i--)
    {
        uint8_t respBuff[2048];

        if ( GetMessageFromQueue( (void*)&respBuff, i , queue) == true)
        {
            printf("FOUND MESSAGE TYPE! %i\r\n", i);
            return i;
        }
    }

    return 0;
}
*/
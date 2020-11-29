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
#include "messageSize.h"
#include "projectGenerals.h"
#include "protocol.h"
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


void RemoveQueue( int queueId )
{
    struct msqid_ds queueStructure; // This is ignored with IPC_RMID
    if ( msgctl(queueId, IPC_RMID, &queueStructure) != 0 )
    {
        ReportAndExit("Error during queue removing...\r\n");
    }
    else
    {
        printf("Properly removed queue!\r\n");
    }
}

void CleanQueue( int queueId )
{
    // I know it's ugly....
    // When I found better way, I'll fix it ;)
    uint32_t messagesInQueue = GetNumberOfMessagesInQueue( queueId );
    for (uint32_t i = 0U; i < messagesInQueue; i++)
    {
        responseUint32_t responseUint32;
        responseUint64_t responseUint64;
        responseShortConfirmation_t responseShort;
        responseSubscription_t responseSubscription;
        notificationMessage_t notificationMesssage;
        requestSingleGet_t requestSingle;
        requestSingleSet_t requestSingleSet;
        requestReset_t requestReset;
        requestSubscription_t requestSubscription;
        GetMessageFromQueue( (void*)&responseUint32,       UINT32_RESPONSE,             queueId );
        GetMessageFromQueue( (void*)&responseUint64,       UINT64_RESPONSE,             queueId );
        GetMessageFromQueue( (void*)&responseShort,        SHORT_CONFIRMATION_RESPONSE, queueId );
        GetMessageFromQueue( (void*)&responseSubscription, SUBSCRIPTION_RESPONSE,       queueId );
        GetMessageFromQueue( (void*)&notificationMesssage, NOTIFICATION,                queueId );
        GetMessageFromQueue( (void*)&requestSingle,        SET_SINGLE_REQUEST,          queueId );
        GetMessageFromQueue( (void*)&requestSingle,        GET_SINGLE_REQUEST,          queueId );
        GetMessageFromQueue( (void*)&requestReset,         RESET_REQUEST,               queueId );
        GetMessageFromQueue( (void*)&requestSubscription,  SUBSCRIBE_REQUEST,           queueId );
    }
}

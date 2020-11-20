#include <sys/ipc.h>            // key_t
#include <stdio.h>              // printf()      
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>            // msgget()
#include <errno.h>              // erno
#include "queueUtils.h"
#include "errorHandling.h" // ReportAndExit()
#include "messageUtils.h"  // GetMessageSize()
#include "../commonIncludes/projectGenerals.h"            // SERVER_VERSION

//--File Scope Global Variables---------------------------------------
static bool serverQueueStatus = false;
static int serverReceiveQueueId = 0;
//--------------------------------------------------------------------
bool GetServerQueueStatus( void )
{
    return serverQueueStatus;
}

bool GetServerQueueId( int * serverQueueId )
{
    *serverQueueId = serverReceiveQueueId;
    return serverQueueStatus;
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
            ReportAndExit("Unexpected error pushing message to queue...");
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

bool GetMessageFromServerQueue( void * message, long messageType  )
{
    return GetMessageFromQueue( message, messageType, serverReceiveQueueId );
}

bool GetMessageFromQueue( void * message, long messageType, int queueId )
{
    bool status = (bool)( GetNumberOfMessagesInQueue( queueId ) != 0U );
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
            printf("Properly clean queue!\r\n");
        }
    }

}
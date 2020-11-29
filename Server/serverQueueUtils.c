//--Project includes--------------------------------------------------
#include "queueUtils.h"
#include "projectGenerals.h"
//--------------------------------------------------------------------

//--File scope Variables----------------------------------------------
static int serverReceiveQueueId = 0;
//--------------------------------------------------------------------


void InitServerMessageQueue( void )
{
    serverReceiveQueueId = InitMessageQueue( SERVER_PATH_NAME );
}

uint32_t GetNumberOfMessagesInServerQueue( void )
{
    return GetNumberOfMessagesInQueue( serverReceiveQueueId );
}

bool GetMessageFromServerQueue( void * message, long messageType  )
{
    return GetMessageFromQueue( message, messageType, serverReceiveQueueId );
}

void CleanServerQueue( void )
{
    CleanQueue( serverReceiveQueueId );
}

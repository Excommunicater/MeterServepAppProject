#include "messageUtils.h"
#include "../commonIncludes/serverMessages.h"     //

//--Local Structures--------------------------------------------------
typedef struct messageTypeSize
{
    long   type;
    size_t size;
} messageTypeSize_t;

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

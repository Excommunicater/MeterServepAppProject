//--Project includes--------------------------------------------------
#include "messageSize.h"
#include "protocol.h"
//--------------------------------------------------------------------

//--Local Structures--------------------------------------------------
typedef struct messageTypeSize
{
    long   type;
    size_t size;
} messageTypeSize_t;
//--------------------------------------------------------------------

//--File Scope Variables----------------------------------------------
static const messageTypeSize_t SIZE_TYPE_ARRAY[NUMBER_OF_MESSAGE_TYPES] =
{
    { GET_SINGLE_REQUEST,           sizeof(requestSingleGetBody_t) },
    { RESET_REQUEST,                sizeof(requestResetBody_t) },
    { SET_SINGLE_REQUEST,           sizeof(requestSingleSetBody_t) },
    { SUBSCRIBE_REQUEST,            sizeof(requestSubscriptionBody_t)},
    { NOTIFICATION,                 sizeof(notificationMessageBody_t)},
    { SHORT_CONFIRMATION_RESPONSE,  sizeof(responseShortConfirmationBody_t) },
    { UINT32_RESPONSE,              sizeof(responseUint32Body_t) },
    { UINT64_RESPONSE,              sizeof(responseUint64Body_t)},
    { SUBSCRIPTION_RESPONSE,        sizeof(responseSubscriptionBody_t)}
};
//--------------------------------------------------------------------

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

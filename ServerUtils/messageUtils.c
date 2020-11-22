#include "messageUtils.h"
#include "queueUtils.h"
#include "../commonIncludes/serverMessages.h"     //

//--Local Structures--------------------------------------------------
typedef struct messageTypeSize
{
    long   type;
    size_t size;
} messageTypeSize_t;

const messageTypeSize_t SIZE_TYPE_ARRAY[NUMBER_OF_MESSAGE_TYPES] =
{
    { GET_SINGLE_REQUEST,           sizeof(requestSingleGetBody_t) },
    { RESET_REQUEST,                sizeof(requestResetBody_t) },
    { SET_SINGLE_REQUEST,           sizeof(requestSingleSetBody_t) },
    { SUBSCRIBE_REQUEST,            sizeof(requestSubscriptionBody_t)},
    { NOTIFICATION,                 sizeof(notificationBody_t)},
    { SHORT_CONFIRMATION_RESPONSE,  sizeof(responseShortConfirmationBody_t) },
    { UINT32_RESPONSE,              sizeof(responseUint32Body_t) },
    { UINT64_RESPONSE,              1},
    { SUBSCRIPTION_RESPONSE,        sizeof(responseSubscriptionBody_t)}
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

bool ResponseUint32( uint32_t valueToResponse, shortConfirmationValues_t status, long responseQueue, uint32_t requestId )
{
    bool retVal = false;
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
        retVal = true;
    }

    return retVal;
}

bool ResponseShortConfirmation( shortConfirmationValues_t valueToresponse, long responseQueue, uint32_t requestId )
{
    bool retVal = false;
    responseShortConfirmation_t response;
    response.mtype = SHORT_CONFIRMATION_RESPONSE;

    responseShortConfirmationBody_t * responseBody = (responseShortConfirmationBody_t*)response.mtext;
    responseBody->requestId         = requestId;
    responseBody->confirmationValue = valueToresponse;

    if ( PushMessageToQueue((void*)&response, SHORT_CONFIRMATION_RESPONSE, responseQueue ) )
    {
        #ifdef DEBUG_PRINTOUT
            printf("Respond properly with SHORT_CONFIRMATION_RESPONSE \r\n" );
        #endif
        retVal = true;
    }

    return retVal;
}

bool ResponseOnSubscriptionRequest( subscriptionRegistrationStatus_t confirmation, uint8_t notificationId, long responseQueue, uint32_t requestId )
{
    bool retVal = false;
    responseSubscription_t response;
    response.mtype = SUBSCRIPTION_RESPONSE;

    responseSubscriptionBody_t * responseBody = (responseSubscriptionBody_t*)response.mtext;
    responseBody->requestId         = requestId;
    responseBody->confirmationValue = confirmation;
    responseBody->notificationId    = notificationId;

    if ( PushMessageToQueue((void*)&response, SUBSCRIPTION_RESPONSE, responseQueue ) )
    {
        #ifdef DEBUG_PRINTOUT
            printf("Respond properly with SUBSCRIPTION_RESPONSE \r\n" );
        #endif
        retVal = true;
    }
    
    return retVal;
}

bool SendNotificationMessage( notification_t * pNotification )
{
    bool retVal = false;
    notificationMessage_t messageToSend;
    notificationBody_t * messageBody = (notificationBody_t*)messageToSend.mtext;

    messageBody->requestId       = pNotification->notificationMessageId;
    messageBody->notificationId  = pNotification->notificationID;
    messageBody->timeStamp       = pNotification->timeStamp;

    if ( PushMessageToQueue((void*)&messageToSend, NOTIFICATION, pNotification->queueToSend ) )
    {
        #ifdef DEBUG_PRINTOUT
            printf("Sent properly NOTIFICATION \r\n" );
        #endif
        retVal = true;
    }
    return retVal;
}
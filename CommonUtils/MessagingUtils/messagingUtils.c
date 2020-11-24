
#include "messagingUtils.h"
#include "queueUtils.h"

bool SendResponseUint32( uint32_t valueToResponse, shortConfirmationValues_t status, long responseQueue, uint32_t requestId )
{
    bool retVal = false;
    responseUint32_t response;
    response.mtype = UINT32_RESPONSE;

    responseUint32Body_t * pResponseBody = (responseUint32Body_t*)response.mtext;
    pResponseBody->requestId = requestId;
    pResponseBody->value     = valueToResponse;
    pResponseBody->status    = status;
    
    if ( PushMessageToQueue( (void*)&response, UINT32_RESPONSE, responseQueue ) )
    {
        #ifdef DEBUG_PRINTOUT
            printf("Respond properly with UINT32_RESPONSE \r\n" );
        #endif
        retVal = true;
    }
    return retVal;
} 

bool SendResponseUint64( uint64_t valueToResponse, shortConfirmationValues_t status, long responseQueue, uint32_t requestId )
{
    bool retVal = false;
    responseUint64_t response;
    response.mtype = UINT64_RESPONSE;

    responseUint64Body_t * pResponseBody = (responseUint64Body_t*)response.mtext;
    pResponseBody->requestId = requestId;
    pResponseBody->value     = valueToResponse;
    pResponseBody->status    = status;
    
    if ( PushMessageToQueue( (void*)&response, UINT64_RESPONSE, responseQueue ) )
    {
        #ifdef DEBUG_PRINTOUT
            printf("Respond properly with UINT32_RESPONSE \r\n" );
        #endif
        retVal = true;
    }
    return retVal;
}

bool SendResponseShortConfirmation( shortConfirmationValues_t valueToresponse, long responseQueue, uint32_t requestId )
{
    bool retVal = false;
    responseShortConfirmation_t response;
    response.mtype = SHORT_CONFIRMATION_RESPONSE;

    responseShortConfirmationBody_t * pResponseBody = (responseShortConfirmationBody_t*)response.mtext;
    pResponseBody->requestId         = requestId;
    pResponseBody->confirmationValue = valueToresponse;

    if ( PushMessageToQueue( (void*)&response, SHORT_CONFIRMATION_RESPONSE, responseQueue ) )
    {
        #ifdef DEBUG_PRINTOUT
            printf("Respond properly with SHORT_CONFIRMATION_RESPONSE \r\n" );
        #endif
        retVal = true;
    }
    return retVal;
}

bool SendResponseSubscription( subscriptionRegistrationStatus_t confirmation, uint8_t notificationId, long responseQueue, uint32_t requestId )
{
    bool retVal = false;
    responseSubscription_t response;
    response.mtype = SUBSCRIPTION_RESPONSE;

    responseSubscriptionBody_t * pResponseBody = (responseSubscriptionBody_t*)response.mtext;
    pResponseBody->requestId         = requestId;
    pResponseBody->confirmationValue = confirmation;
    pResponseBody->notificationId    = notificationId;

    if ( PushMessageToQueue( (void*)&response, SUBSCRIPTION_RESPONSE, responseQueue ) )
    {
        #ifdef DEBUG_PRINTOUT
            printf("Respond properly with SUBSCRIPTION_RESPONSE \r\n" );
        #endif
        retVal = true;
    }
    return retVal;
}

bool SendRequestGetSingle( attributesToGet_t attributeToGet, uint8_t instanceToGet, long requestQueue, long responseQueue, uint32_t requestId )
{
    bool retVal = false;
    requestSingleGet_t request;
    request.mtype = GET_SINGLE_REQUEST;

    requestSingleGetBody_t * pRequestBody = (requestSingleGetBody_t*)request.mtext;
    pRequestBody->attribute       = attributeToGet;
    pRequestBody->instance        = instanceToGet;
    pRequestBody->queueResponseId = responseQueue;
    pRequestBody->requestId       = requestId;

    if ( PushMessageToQueue( (void*)&request, GET_SINGLE_REQUEST, requestQueue ) )
    {
        #ifdef DEBUG_PRINTOUT
            printf("Send request properly GET_SINGLE_REQUEST \r\n" );
        #endif
        retVal = true;
    }
    return retVal;
}

bool SendRequestSetSingle( uint32_t valueToSet, attributesToSet_t attributeToSet, uint8_t instanceToSet, long requestQueue, long responseQueue, uint32_t requestId )
{
    bool retVal = false;
    requestSingleSet_t request;
    request.mtype = SET_SINGLE_REQUEST;

    requestSingleSetBody_t * pRequestBody = (requestSingleSetBody_t*)request.mtext;
    pRequestBody->valueToSet      = valueToSet;
    pRequestBody->attribute       = attributeToSet;
    pRequestBody->instance        = instanceToSet;
    pRequestBody->queueResponseId = responseQueue;
    pRequestBody->requestId       = requestId;

    if ( PushMessageToQueue( (void*)&request, SET_SINGLE_REQUEST, requestQueue ) )
    {
        #ifdef DEBUG_PRINTOUT
            printf("Send request properly SET_SINGLE_REQUEST \r\n" );
        #endif
        retVal = true;
    }
    return retVal;
}

bool SendRequestReset( attributesToReset_t attributeToReset, uint8_t instanceToReset, long requestQueue, long responseQueue, uint32_t requestId )
{
    bool retVal = false;
    requestReset_t request;
    request.mtype = RESET_REQUEST;

    requestResetBody_t * pRequestBody = (requestResetBody_t*)request.mtext; 
    pRequestBody->attribute       = attributeToReset;
    pRequestBody->instance        = instanceToReset;
    pRequestBody->queueResponseId = responseQueue;
    pRequestBody->requestId       = requestId;

    if ( PushMessageToQueue( (void*)&request, RESET_REQUEST, requestQueue ) )
    {
        #ifdef DEBUG_PRINTOUT
            printf("Send request properly RESET_REQUEST \r\n" );
        #endif
        retVal = true;
    }
    return retVal;   
}

bool SendRequestSubsciption( subscription_t subscriptionToRequest, uint8_t instanceToSubscribe, long requestQueue, long responseQueue, uint32_t requestId )
{
    bool retVal = false;
    requestSubscription_t request;
    request.mtype = SUBSCRIBE_REQUEST;

    requestSubscriptionBody_t * pRequestBody = (requestSubscriptionBody_t*)request.mtext; 
    pRequestBody->attribute       = subscriptionToRequest;
    pRequestBody->instance        = instanceToSubscribe;
    pRequestBody->queueResponseId = responseQueue;
    pRequestBody->requestId       = requestId;

    if ( PushMessageToQueue( (void*)&request, SUBSCRIBE_REQUEST, requestQueue ) )
    {
        #ifdef DEBUG_PRINTOUT
            printf("Send request properly SUBSCRIBE_REQUEST \r\n" );
        #endif
        retVal = true;
    }
    return retVal;   
}

#if SERVER_64_BIT == true
bool SendNotification( uint8_t notificationId, long queue, uint32_t requestId, uint64_t timeStamp )
#elif
bool SendNotification( uint8_t notificationId, uint32_t requestId, uint32_t timeStamp )
#endif
{
    bool retVal = false;
    notificationMessage_t messageToSend;
    messageToSend.mtype = NOTIFICATION;
    notificationMessageBody_t * messageBody = (notificationMessageBody_t*)messageToSend.mtext;
    
    messageBody->requestId       = requestId;
    messageBody->notificationId  = notificationId;
    messageBody->timeStamp       = timeStamp;

    if ( PushMessageToQueue( (void*)&messageToSend, NOTIFICATION, queue ) )
    {
        #ifdef DEBUG_PRINTOUT
            printf("Sent properly NOTIFICATION \r\n" );
        #endif
        retVal = true;
    }
    return retVal;
}
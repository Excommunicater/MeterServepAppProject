#include "testUtils.h"
#include "queueUtils.h"
#include "messagingUtils.h"
#include <stdio.h>

testResponses_t TestSingleRequestWithUint32Response( 
    uint8_t instance, 
    attributesToGet_t attribute, 
    int responseQueueId, 
    int serverQueueId,
    bool validateResponseValue, 
    uint32_t expectedResponse,
    shortConfirmationValues_t expectedStatus,
    uint32_t requestId )
{
    bool operationStatus = false;
    responseUint32_t responseUint32;
    responseUint32Body_t * pResponseBody = (responseUint32Body_t*)responseUint32.mtext;

    // Try to push Request to server
    operationStatus = SendRequestGetSingle( attribute, instance, serverQueueId, responseQueueId, requestId );

    // I know it's good to have only one return from function... 
    // But it will increase readibility - appliest to whole function
    if ( operationStatus == false )
    {
        return TEST_ERROR_SENDING_REQUEST;
    }

    // Wait for response
    while ( GetNumberOfMessagesInQueue(responseQueueId) == 0U );
    
    if ( GetMessageFromQueue( (void*)&responseUint32, UINT32_RESPONSE, responseQueueId ) )
    {
        pResponseBody = (responseUint32Body_t*)responseUint32.mtext;
        if( pResponseBody->requestId != requestId )
        {
            return TEST_ERROR_SEGMENTATION;
        }

        if ( ( validateResponseValue == true ) && ( pResponseBody->value != expectedResponse ) )
        {
            return TEST_ERROR_NOT_EXPECTED_VALUE;
        }

        if ( pResponseBody->status != expectedStatus )
        {
            return TEST_ERROR_NOT_EXPECTED_STATUS;
        }
    }
    else
    {
        return TEST_ERROR_RECIVE_RESPONSE;
    }
    return TEST_OK;
}

testResponses_t TestSingleRequestWithUint64Response( 
    uint8_t instance, 
    attributesToGet_t attribute, 
    int responseQueueId, 
    int serverQueueId,
    bool validateResponseValue, 
    uint64_t expectedResponse,
    shortConfirmationValues_t expectedStatus,
    uint32_t requestId )
{
    bool operationStatus = false;
    responseUint64_t responseUint64;
    responseUint64Body_t * pResponseBody = (responseUint64Body_t*)responseUint64.mtext;

    // Try to push Request to server
    operationStatus = SendRequestGetSingle( attribute, instance, serverQueueId, responseQueueId, requestId );

    // I know it's good to have only one return from function... 
    // But it will increase readibility - appliest to whole function
    if ( operationStatus == false )
    {
        return TEST_ERROR_SENDING_REQUEST;
    }

    // Wait for response
    while ( GetNumberOfMessagesInQueue(responseQueueId) == 0U );
    
    if ( GetMessageFromQueue( (void*)&responseUint64, UINT64_RESPONSE, responseQueueId ) )
    {
        pResponseBody = (responseUint64Body_t*)responseUint64.mtext;
        if( pResponseBody->requestId != requestId )
        {
            return TEST_ERROR_SEGMENTATION;
        }

        if ( ( validateResponseValue == true ) && ( pResponseBody->value != expectedResponse ) )
        {
            return TEST_ERROR_NOT_EXPECTED_VALUE;
        }

        if ( pResponseBody->status != expectedStatus )
        {
            return TEST_ERROR_NOT_EXPECTED_STATUS;
        }
    }
    else
    {
        return TEST_ERROR_RECIVE_RESPONSE;
    }
    return TEST_OK;
}

testResponses_t TestResetRequestWithShortConfirmationResponse( 
    uint8_t instance, 
    attributesToGet_t attribute, 
    int responseQueueId, 
    int serverQueueId,
    shortConfirmationValues_t expectedStatus,
    uint32_t requestId )
{
    bool operationStatus = false;
    responseShortConfirmation_t responseShort;
    responseShortConfirmationBody_t * pResponseBody = (responseShortConfirmationBody_t*)responseShort.mtext;

    // Try to push Request to server
    operationStatus = SendRequestReset( attribute, instance, serverQueueId, responseQueueId, requestId );

    // I know it's good to have only one return from function... 
    // But it will increase readibility - appliest to whole function
    if ( operationStatus == false )
    {
        return TEST_ERROR_SENDING_REQUEST;
    }
    // Wait for response
    while ( GetNumberOfMessagesInQueue(responseQueueId) == 0U );

    if ( GetMessageFromQueue( (void*)&responseShort, SHORT_CONFIRMATION_RESPONSE, responseQueueId ) )
    {
        pResponseBody = (responseShortConfirmationBody_t*)responseShort.mtext;
        if( pResponseBody->requestId != requestId )
        {
            return TEST_ERROR_SEGMENTATION;
        }
        if ( pResponseBody->confirmationValue != expectedStatus )
        {
            return TEST_ERROR_NOT_EXPECTED_STATUS;
        }
    }
    else
    {
        return TEST_ERROR_RECIVE_RESPONSE;
    }
    return TEST_OK;
}

testResponses_t TestGetSingleRequestShortConfirmationResponse( 
    uint8_t instance, 
    attributesToGet_t attribute, 
    int responseQueueId, 
    int serverQueueId,
    shortConfirmationValues_t expectedStatus,
    uint32_t requestId )
{
    bool operationStatus = false;
    responseShortConfirmation_t responseShort;
    responseShortConfirmationBody_t * pResponseBody = (responseShortConfirmationBody_t*)responseShort.mtext;

    // Try to push Request to server
    operationStatus = SendRequestGetSingle( attribute, instance, serverQueueId, responseQueueId, requestId );

    // I know it's good to have only one return from function... 
    // But it will increase readibility - appliest to whole function
    if ( operationStatus == false )
    {
        return TEST_ERROR_SENDING_REQUEST;
    }
    // Wait for response
    while ( GetNumberOfMessagesInQueue(responseQueueId) == 0U );

    if ( GetMessageFromQueue( (void*)&responseShort, SHORT_CONFIRMATION_RESPONSE, responseQueueId ) )
    {
        pResponseBody = (responseShortConfirmationBody_t*)responseShort.mtext;
        if( pResponseBody->requestId != requestId )
        {
            return TEST_ERROR_SEGMENTATION;
        }
        if ( pResponseBody->confirmationValue != expectedStatus )
        {
            return TEST_ERROR_NOT_EXPECTED_STATUS;
        }
    }
    else
    {
        return TEST_ERROR_RECIVE_RESPONSE;
    }
    return TEST_OK;
}

testResponses_t TestSetRequestWithShortConfirmationResponse( 
    uint8_t instance, 
    attributesToSet_t attribute, 
    int responseQueueId, 
    int serverQueueId,
    shortConfirmationValues_t expectedStatus,
    uint32_t requestId,
    uint32_t valueToSet )
{
    bool operationStatus = false;
    responseShortConfirmation_t responseShort;
    responseShortConfirmationBody_t * pResponseBody = (responseShortConfirmationBody_t*)responseShort.mtext;

    // Try to push Request to server
    operationStatus = SendRequestSetSingle( valueToSet, attribute, instance, serverQueueId, responseQueueId, requestId);

    // I know it's good to have only one return from function... 
    // But it will increase readibility - appliest to whole function
    if ( operationStatus == false )
    {
        return TEST_ERROR_SENDING_REQUEST;
    }
    // Wait for response
    while ( GetNumberOfMessagesInQueue(responseQueueId) == 0U );

    if ( GetMessageFromQueue( (void*)&responseShort, SHORT_CONFIRMATION_RESPONSE, responseQueueId ) )
    {
        pResponseBody = (responseShortConfirmationBody_t*)responseShort.mtext;
        if( pResponseBody->requestId != requestId )
        {
            return TEST_ERROR_SEGMENTATION;
        }
        if ( pResponseBody->confirmationValue != expectedStatus )
        {
            return TEST_ERROR_NOT_EXPECTED_STATUS;
        }
    }
    else
    {
        return TEST_ERROR_RECIVE_RESPONSE;
    }
    return TEST_OK;
}

testResponses_t TestSubscribeRequestWithSubscriptionResponse( 
    uint8_t instance,  
    subscription_t subscriptionToRequest,
    int responseQueueId, 
    int serverQueueId,
    subscriptionRegistrationStatus_t expectedStatus,
    uint32_t requestId,
    uint8_t * notificationId )
{
    bool operationStatus = false;
    responseSubscription_t response;
    responseSubscriptionBody_t * pResponseBody;

    // Try to push Request to server
    operationStatus = SendRequestSubsciption( subscriptionToRequest, instance, serverQueueId, responseQueueId, requestId );

    // I know it's good to have only one return from function... 
    // But it will increase readibility - appliest to whole function
    if ( operationStatus == false )
    {
        return TEST_ERROR_SENDING_REQUEST;
    }
    // Wait for response
    while ( GetNumberOfMessagesInQueue(responseQueueId) == 0U );

    if ( GetMessageFromQueue( (void*)&response, SUBSCRIPTION_RESPONSE, responseQueueId ) )
    {
        pResponseBody = (responseSubscriptionBody_t*)response.mtext;
        *notificationId = pResponseBody->notificationId;
        if( pResponseBody->requestId != requestId )
        {
            return TEST_ERROR_SEGMENTATION;
        }
        if ( pResponseBody->confirmationValue != expectedStatus )
        {
            return TEST_ERROR_NOT_EXPECTED_STATUS;
        }
    }
    else
    {
        return TEST_ERROR_RECIVE_RESPONSE;
    }
    return TEST_OK;
}

testResponses_t TestGetNotification( 
    uint8_t expectedNotificationId,
    uint32_t * requestId,
    int queue,
    #if SERVER_64_BIT == true
        uint64_t * timeStamp
    #elif
        uint32_t * timeStamp
    #endif

    )
{
    notificationMessage_t notificationMessage;
    notificationMessageBody_t * notificationMessageBody = (notificationMessageBody_t*)notificationMessage.mtext;
    // Wait for response
    while ( GetNumberOfMessagesInQueue(queue) == 0U );
    if ( GetMessageFromQueue( (void*)&notificationMessage, NOTIFICATION, queue ) )
    {
        *requestId = notificationMessageBody->requestId;
        *timeStamp = notificationMessageBody->timeStamp;
        if ( notificationMessageBody->notificationId != expectedNotificationId )
        {
            return TEST_ERROR_NOT_EXPECTED_STATUS;
        }
    }
    return TEST_OK;    
}

testResponses_t TestResponseToNotification( 
    uint32_t responseId,
    int queue,
    shortConfirmationValues_t valueToResponse )
{
    // Try to push Request to server
    bool operationStatus = SendResponseShortConfirmation( valueToResponse, queue, responseId );

    // I know it's good to have only one return from function... 
    // But it will increase readibility - appliest to whole function
    if ( operationStatus == false )
    {
        return TEST_ERROR_SENDING_REQUEST;
    }

    return OK;
}

void ParseTestResponse( testResponses_t singleTestResponse, wholeTestResponse_t * pWholeTestResponse )
{
    pWholeTestResponse->numberOfTests++;
    printf("Test No.%i - ", pWholeTestResponse->numberOfTests);
    if ( singleTestResponse == TEST_OK )
    {
        pWholeTestResponse->passTest++;
    }
    PrintTestResponse(singleTestResponse);
}

void PrintTestResults( wholeTestResponse_t result, const char * testPath  )
{
    printf("--------------------------------------------------------------------\r\n");
    printf("                     T E S T  C O M P L E T E D                     \r\n");
    printf("%s", testPath);
    printf("\r\n");
    printf("--------------------------------------------------------------------\r\n");
    printf("\033[1;32m"); //Set the text to the bold green  
    printf(" TEST PASSED:  %i\r\n", result.passTest);
    printf("\033[1;31m"); //Set the text to the bold red
    printf(" TEST FAILED:  %i\r\n", result.numberOfTests - result.passTest);
    printf("\033[1;0m"); //Resets the text to default color
    printf("--------------------------------------------------------------------\r\n");
}

void PrintTestResponse( testResponses_t response )
{
    switch (response)
    {
        case TEST_OK:
            printf("TEST_OK\r\n");
            break;
        case TEST_ERROR_SENDING_REQUEST:
            printf("TEST_ERROR_SENDING_REQUEST\r\n");
            break;
        case TEST_ERROR_RECIVE_RESPONSE:
            printf("TEST_ERROR_RECIVE_RESPONSE\r\n");
            break;
        case TEST_ERROR_NOT_EXPECTED_VALUE:
            printf("TEST_ERROR_NOT_EXPECTED_VALUE\r\n");
            break;
        case TEST_ERROR_NOT_EXPECTED_STATUS:
            printf("TEST_ERROR_NOT_EXPECTED_STATUS\r\n");
            break;
        case TEST_ERROR_SEGMENTATION:
            printf("TEST_ERROR_SEGMENTATION\r\n");
            break;
        default:
            printf("Some strange response...\r\n");
            break;
    }
}

int TestUtilGetAndPrepareTestQueue( const char * testPath )
{
    int testQueue = InitMessageQueue( testPath );
    CleanQueue(testQueue);
    return testQueue;
}

int TestUtilGetServerQueue( void )
{
    int serverQueue = InitMessageQueue( SERVER_PATH_NAME );
    CleanQueue(serverQueue);
    return serverQueue;
}

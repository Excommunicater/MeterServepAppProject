#include "testUtils.h"
#include "../../../ServerUtils/queueUtils.h"
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
    requestSingleGet_t request;
    responseUint32_t responseUint32;
    requestSingleGetBody_t * pRequestBody = (requestSingleGetBody_t*)request.mtext;
    responseUint32Body_t * pResponseBody = (responseUint32Body_t*)responseUint32.mtext;

    //--Set Request Message-----------------------------------------------
    request.mtype                   = GET_SINGLE_REQUEST;
    pRequestBody->requestId         = requestId;
    pRequestBody->attribute         = attribute;
    pRequestBody->instance          = instance;
    pRequestBody->queueResponseId   = responseQueueId;
    //--------------------------------------------------------------------

    // Try to push Request to server
    operationStatus = PushMessageToQueue( (void*)&request, GET_SINGLE_REQUEST, serverQueueId );

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

testResponses_t TestResetRequestWithShortConfirmationResponse( 
    uint8_t instance, 
    attributesToGet_t attribute, 
    int responseQueueId, 
    int serverQueueId,
    shortConfirmationValues_t expectedStatus,
    uint32_t requestId )
{
    bool operationStatus = false;
    requestReset_t request;
    responseShortConfirmation_t responseShort;
    requestResetBody_t * pRequestBody = (requestResetBody_t*)request.mtext;
    responseShortConfirmationBody_t * pResponseBody = (responseShortConfirmationBody_t*)responseShort.mtext;

    //--Set Request Message-----------------------------------------------
    request.mtype                   = RESET_REQUEST;
    pRequestBody->requestId         = requestId;
    pRequestBody->attribute         = attribute;
    pRequestBody->instance          = instance;
    pRequestBody->queueResponseId   = responseQueueId;
    //--------------------------------------------------------------------

    // Try to push Request to server
    operationStatus = PushMessageToQueue( (void*)&request, RESET_REQUEST, serverQueueId );

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

testResponses_t TestSingleRequestShortConfirmationResponse( 
    uint8_t instance, 
    attributesToGet_t attribute, 
    int responseQueueId, 
    int serverQueueId,
    shortConfirmationValues_t expectedStatus,
    uint32_t requestId )
{
    bool operationStatus = false;
    requestSingleGet_t request;
    responseShortConfirmation_t responseShort;
    requestSingleGetBody_t * pRequestBody = (requestSingleGetBody_t*)request.mtext;
    responseShortConfirmationBody_t * pResponseBody = (responseShortConfirmationBody_t*)responseShort.mtext;

    //--Set Request Message-----------------------------------------------
    request.mtype                   = GET_SINGLE_REQUEST;
    pRequestBody->requestId         = requestId;
    pRequestBody->attribute         = attribute;
    pRequestBody->instance          = instance;
    pRequestBody->queueResponseId   = responseQueueId;
    //--------------------------------------------------------------------
    // Try to push Request to server
    operationStatus = PushMessageToQueue( (void*)&request, RESET_REQUEST, serverQueueId );

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
    attributesToGet_t attribute, 
    int responseQueueId, 
    int serverQueueId,
    shortConfirmationValues_t expectedStatus,
    uint32_t requestId,
    uint32_t valueToSet )
{
    bool operationStatus = false;
    requestSingleSet_t request;
    responseShortConfirmation_t responseShort;
    requestSingleSetBody_t * pRequestBody = (requestSingleSetBody_t*)request.mtext;
    responseShortConfirmationBody_t * pResponseBody = (responseShortConfirmationBody_t*)responseShort.mtext;

    //--Set Request Message-----------------------------------------------
    request.mtype                   = SET_SINGLE_REQUEST;
    pRequestBody->requestId         = requestId;
    pRequestBody->queueResponseId   = responseQueueId;
    pRequestBody->instance          = instance;
    pRequestBody->attribute         = attribute;
    pRequestBody->valueToSet        = valueToSet;
    //--------------------------------------------------------------------
    // Try to push Request to server
    operationStatus = PushMessageToQueue( (void*)&request, RESET_REQUEST, serverQueueId );

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
    attributesToGet_t attribute, 
    int responseQueueId, 
    int serverQueueId,
    subscriptionRegistrationStatus_t expectedStatus,
    uint32_t requestId,
    uint8_t * notificationId )
{
    bool operationStatus = false;
    requestSubscription_t request;
    responseSubscription_t response;
    requestSubscriptionBody_t * pRequestBody = (requestSubscriptionBody_t*)request.mtext;
    responseSubscriptionBody_t * pResponseBody;

    //--Set Request Message-----------------------------------------------
    request.mtype                   = SUBSCRIBE_REQUEST;
    pRequestBody->requestId         = requestId;
    pRequestBody->queueResponseId   = responseQueueId;
    pRequestBody->instance          = instance;
    pRequestBody->attribute         = attribute;
    //--------------------------------------------------------------------
    // Try to push Request to server
    operationStatus = PushMessageToQueue( (void*)&request, SUBSCRIBE_REQUEST, serverQueueId );

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
            printf("GetId = %i; Expected = %i\r\n", pResponseBody->requestId, requestId );
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
    printf(testPath);
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

int TestUtilGetServerQueue( void )
{
    int response = 0;
    GetServerQueueId(&response);
    return response;
}

int TestUtilGetAndPrepareTestQueue( const char * testPath )
{
    int testQueue = InitMessageQueue( testPath );
    CleanAppQueue(testQueue);
    return testQueue;
}

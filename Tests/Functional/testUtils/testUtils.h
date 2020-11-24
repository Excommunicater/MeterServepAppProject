#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <stdbool.h> // bool
#include <stdint.h>  // uint32_t
#include "protocol.h"

typedef enum testResponses
{
    TEST_OK = 0,
    TEST_ERROR_SENDING_REQUEST,
    TEST_ERROR_RECIVE_RESPONSE,
    TEST_ERROR_NOT_EXPECTED_VALUE,
    TEST_ERROR_NOT_EXPECTED_STATUS,
    TEST_ERROR_SEGMENTATION
} testResponses_t;

typedef struct wholeTestResponse
{
    uint8_t passTest;
    uint8_t numberOfTests;
} wholeTestResponse_t;

testResponses_t TestSingleRequestWithUint32Response( 
    uint8_t instance, 
    attributesToGet_t attribute, 
    int responseQueueId, 
    int serverQueueId,
    bool validateResponseValue, 
    uint32_t expectedResponse,
    shortConfirmationValues_t expectedStatus,
    uint32_t requestId );

testResponses_t TestSingleRequestWithUint64Response( 
    uint8_t instance, 
    attributesToGet_t attribute, 
    int responseQueueId, 
    int serverQueueId,
    bool validateResponseValue, 
    uint64_t expectedResponse,
    shortConfirmationValues_t expectedStatus,
    uint32_t requestId );

testResponses_t TestGetSingleRequestShortConfirmationResponse( 
    uint8_t instance, 
    attributesToGet_t attribute, 
    int responseQueueId, 
    int serverQueueId,
    shortConfirmationValues_t expectedStatus,
    uint32_t requestId );

testResponses_t TestResetRequestWithShortConfirmationResponse( 
    uint8_t instance, 
    attributesToGet_t attribute, 
    int responseQueueId, 
    int serverQueueId,
    shortConfirmationValues_t expectedStatus,
    uint32_t requestId );

testResponses_t TestSetRequestWithShortConfirmationResponse( 
    uint8_t instance, 
    attributesToSet_t attribute, 
    int responseQueueId, 
    int serverQueueId,
    shortConfirmationValues_t expectedStatus,
    uint32_t requestId,
    uint32_t valueToSet );

testResponses_t TestSubscribeRequestWithSubscriptionResponse( 
    uint8_t instance,  
    subscription_t subscriptionToRequest,
    int responseQueueId, 
    int serverQueueId,
    subscriptionRegistrationStatus_t expectedStatus,
    uint32_t requestId,
    uint8_t * notificationId );

testResponses_t TestGetNotification( 
    uint8_t expectedNotificationId,
    uint32_t * requestId,
    int queue,
    #if SERVER_64_BIT == true
        uint64_t * timeStamp
    #elif
        uint32_t * timeStamp
    #endif
    );
testResponses_t TestResponseToNotification( 
    uint32_t responseId,
    int queue,
    shortConfirmationValues_t valueToResponse );

void PrintTestResults( wholeTestResponse_t result, const char * testPath );
void PrintTestResponse( testResponses_t response );
void ParseTestResponse( testResponses_t singleTestResponse, wholeTestResponse_t * pWholeTestResponse );

int TestUtilGetServerQueue( void );
int TestUtilGetAndPrepareTestQueue( const char * testPath );
#endif //TEST_UTILS_H
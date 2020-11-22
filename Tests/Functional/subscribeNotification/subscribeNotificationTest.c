#include "../testUtils/testUtils.h"

#define THIS_TEST_PATH "./Tests/Functional/subscribeNotification/subscribeNotificationTest.c"

int main()
{
    int serverQueueId = TestUtilGetServerQueue();
    int appQueueId    = TestUtilGetAndPrepareTestQueue( THIS_TEST_PATH );
    uint32_t requestId = 1U;

    wholeTestResponse_t wholeTestResponse = {0};
    testResponses_t testResponse = TEST_OK;

    uint8_t uniqueSubscriptionId = 0U;
    uint8_t expectedNumberOfSubscription = 0U;

    // Test 1 - check number of subscriptions before adding any subscription
    testResponse = TestSingleRequestWithUint32Response(
        0,                              //< Instance is not important
        NUMBER_OF_SUBSCRIPTION,         //< Check attribute METER_SERVER_VERSION
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        true,                           //< Validate Response
        expectedNumberOfSubscription,   //< Expected Response - no subscriptions
        OK,                             //< Expected response status
        requestId                       //< Request ID
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);

    PrintTestResults( wholeTestResponse, THIS_TEST_PATH );
}
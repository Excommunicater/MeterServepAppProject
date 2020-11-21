#include "../testUtils/testUtils.h"

#define THIS_TEST_PATH "./Tests/Functional/subscribeRequest/subscribeRequestTest.c"

int main()
{
    int serverQueueId = TestUtilGetServerQueue();
    int appQueueId    = TestUtilGetAndPrepareTestQueue( THIS_TEST_PATH );
    uint32_t requestId = 1U;

    wholeTestResponse_t wholeTestResponse = {0};
    testResponses_t testResponse = TEST_OK;

    uint8_t uniqueSubscriptionId = 0U;

    // Test 1 - Subscribe for UNDER_VOLTAGE_SUBSCRIPTION - Proper instance
    testResponse = TestSubscribeRequestWithSubscriptionResponse(
        1U,                             //< Good instance number
        UNDER_VOLTAGE_SUBSCRIPTION,     //< Reset attribute MINIMUM_PHASE_VOLTAGE
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        SUBSCRIPTION_REGISTERED,        //< Expected response status
        requestId,                      //< Request ID  
        &uniqueSubscriptionId  
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);

    // Test 2 - Try to request once again for same subscribtion
    testResponse = TestSubscribeRequestWithSubscriptionResponse(
        1U,                             //< Good instance number
        UNDER_VOLTAGE_SUBSCRIPTION,     //< Reset attribute MINIMUM_PHASE_VOLTAGE
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        SUBSCRIBTION_ALREADY_EXIST,     //< Expected response status
        requestId,                      //< Request ID  
        &uniqueSubscriptionId  
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);

    // Test 2 - Try for UNDER_VOLTAGE_SUBSCRIPTION - Different instance than in Test 1
    testResponse = TestSubscribeRequestWithSubscriptionResponse(
        2U,                             //< Good instance number
        UNDER_VOLTAGE_SUBSCRIPTION,     //< Reset attribute MINIMUM_PHASE_VOLTAGE
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        SUBSCRIPTION_REGISTERED,        //< Expected response status
        requestId,                      //< Request ID  
        &uniqueSubscriptionId  
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);

    PrintTestResults( wholeTestResponse, THIS_TEST_PATH );
}
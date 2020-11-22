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
    uint8_t test3UniqueSubId = 0U;

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

    // Test 2 - Subscribe for UNDER_VOLTAGE_SUBSCRIPTION - Proper instance
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
    expectedNumberOfSubscription++;

    // Test 3 - check number of subscription after adding first subscription
    testResponse = TestSingleRequestWithUint32Response(
        0,                              //< Instance is not important
        NUMBER_OF_SUBSCRIPTION,         //< Check attribute METER_SERVER_VERSION
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        true,                           //< Validate Response
        expectedNumberOfSubscription,   //< Expected Response
        OK,                             //< Expected response status
        requestId                       //< Request ID
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);

    // Test 4 - Try to request once again for same subscribtion
    testResponse = TestSubscribeRequestWithSubscriptionResponse(
        1U,                             //< Good instance number
        UNDER_VOLTAGE_SUBSCRIPTION,     //< Reset attribute MINIMUM_PHASE_VOLTAGE
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        SUBSCRIPTION_ALREADY_EXIST,     //< Expected response status
        requestId,                      //< Request ID  
        &uniqueSubscriptionId  
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);

    // Test 5 - Subscribe for UNDER_VOLTAGE_SUBSCRIPTION - Different instance than in Test 1
    testResponse = TestSubscribeRequestWithSubscriptionResponse(
        2U,                             //< Good instance number
        UNDER_VOLTAGE_SUBSCRIPTION,     //< Reset attribute MINIMUM_PHASE_VOLTAGE
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        SUBSCRIPTION_REGISTERED,        //< Expected response status
        requestId,                      //< Request ID  
        &test3UniqueSubId 
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);
    expectedNumberOfSubscription++;

    // Test 6 - Subscribe for UNDER_VOLTAGE_SUBSCRIPTION - Different instance than in Test 1 and 3
    testResponse = TestSubscribeRequestWithSubscriptionResponse(
        0U,                             //< Good instance number
        UNDER_VOLTAGE_SUBSCRIPTION,     //< Reset attribute MINIMUM_PHASE_VOLTAGE
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        SUBSCRIPTION_REGISTERED,        //< Expected response status
        requestId,                      //< Request ID  
        &uniqueSubscriptionId  
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);
    expectedNumberOfSubscription++;

    // Test 7 - Subscribe for OVER_VOLTAGE_SUBSCRIPTION - good instance
    testResponse = TestSubscribeRequestWithSubscriptionResponse(
        0U,                             //< Good instance number
        OVER_VOLTAGE_SUBSCRIPTION,      //< Reset attribute MINIMUM_PHASE_VOLTAGE
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        SUBSCRIPTION_REGISTERED,        //< Expected response status
        requestId,                      //< Request ID  
        &uniqueSubscriptionId  
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);
    expectedNumberOfSubscription++;

    // Test 8 - check number of subscription after adding a few subs
    testResponse = TestSingleRequestWithUint32Response(
        0,                              //< Instance is not important
        NUMBER_OF_SUBSCRIPTION,         //< Check attribute METER_SERVER_VERSION
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        true,                           //< Validate Response
        expectedNumberOfSubscription,   //< Expected Response
        OK,                             //< Expected response status
        requestId                       //< Request ID
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);


    // Test 9 - Try to remove subscription form test 3
    testResponse = TestResetRequestWithShortConfirmationResponse(
        test3UniqueSubId,               //< ID of sub from Test 3
        UNSUBSCRIBE,                    //< Unsubscribe 
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        OK,                             //< Expected response status
        requestId                       //< Request ID    
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);
    expectedNumberOfSubscription--;

    // Test 10 - check number of subscription after removing one of the sub
    testResponse = TestSingleRequestWithUint32Response(
        0,                              //< Instance is not important
        NUMBER_OF_SUBSCRIPTION,         //< Check attribute NUMBER_OF_SUBSCRIPTION
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        true,                           //< Validate Response
        expectedNumberOfSubscription,   //< Expected Response
        OK,                             //< Expected response status
        requestId                       //< Request ID
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);

    // Test 11 - Try to remove all subscriptions
    testResponse = TestResetRequestWithShortConfirmationResponse(
        1U,                             //< instance - not important
        UNSUBSCRIBE_ALL,                //< Remove all subscription
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        OK,                             //< Expected response status
        requestId                       //< Request ID    
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);
    expectedNumberOfSubscription = 0U;

    // Test 12 - check number of subscription after removing all
    testResponse = TestSingleRequestWithUint32Response(
        0,                              //< Instance is not important
        NUMBER_OF_SUBSCRIPTION,         //< Check attribute NUMBER_OF_SUBSCRIPTION
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        true,                           //< Validate Response
        expectedNumberOfSubscription,   //< Expected Response
        OK,                             //< Expected response status
        requestId                       //< Request ID
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);

    PrintTestResults( wholeTestResponse, THIS_TEST_PATH );
}
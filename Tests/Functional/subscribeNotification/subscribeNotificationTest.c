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
        NUMBER_OF_SUBSCRIPTION,         //< 
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        true,                           //< Validate Response
        expectedNumberOfSubscription,   //< Expected Response - no subscriptions
        OK,                             //< Expected response status
        requestId                       //< Request ID
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);

    // Test 2 - Subscribe for OVER_VOLTAGE_SUBSCRIPTION - Proper instance
    testResponse = TestSubscribeRequestWithSubscriptionResponse(
        1U,                             //< Good instance number
        OVER_VOLTAGE_SUBSCRIPTION,      //< 
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

    // Test 4 - check number of active subscription. 
    // Should be the same as NUMBER_OF_SUBSCRIPTION until THRESEHOLD is not set
    testResponse = TestSingleRequestWithUint32Response(
        0,                              //< Instance is not important
        NUMBER_OF_ACTIVE_SUBSCRIPTION,  //< Check attribute METER_SERVER_VERSION
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        true,                           //< Validate Response
        expectedNumberOfSubscription,   //< Expected Response
        OK,                             //< Expected response status
        requestId                       //< Request ID
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);

    // Test 5 - set THRESEHOLD for instance and attribute to which we subscribed
    // After seting this value, notification should be sent. 
    // Set value which trigger for 100% notification
    testResponse = TestSetRequestWithShortConfirmationResponse(
        1U,                             //< Good instance number
        OVER_VOLTAGE_THRESEHOLD,        //< Set attribute OVER_VOLTAGE_THRESEHOLD
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        OK,                             //< Expected response status
        requestId,                      //< Request ID   
        1                               //< Really small value, notification should be sent
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);

    // Test 6 
    uint32_t notificationRequestId = 0U;
    #if SERVER_64_BIT == true
        uint64_t timeStamp;
    #elif
        uint32_t timeStamp;
    #endif

    testResponse = TestGetNotification( 
        uniqueSubscriptionId,
        &notificationRequestId,
        appQueueId,
        &timeStamp
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);

    // Test 7 - Notification was sent! - Hopefully :D
    // After sending the notification, subscription should be blocked.
    // Check number of subscription (should be 1) and active subscription (should be 0)
    testResponse = TestSingleRequestWithUint32Response(
        0,                              //< Instance is not important
        NUMBER_OF_SUBSCRIPTION,         //< 
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        true,                           //< Validate Response
        expectedNumberOfSubscription,   //< Expected Response
        OK,                             //< Expected response status
        requestId                       //< Request ID
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);

    // Test 8
    testResponse = TestSingleRequestWithUint32Response(
        0,                              //< Instance is not important
        NUMBER_OF_ACTIVE_SUBSCRIPTION,  //< 
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        true,                           //< Validate Response
        0U,                             //< Expected Response
        OK,                             //< Expected response status
        requestId                       //< Request ID
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);

    // Test 9 - Respond OK to notification
    testResponse = TestResponseToNotification( 
        notificationRequestId,
        serverQueueId,
        OK
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);

    // Test 10 - check is next notification is sent
    testResponse = TestGetNotification( 
        uniqueSubscriptionId,
        &notificationRequestId,
        appQueueId,
        &timeStamp
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);

    // Test 11 - Respond to notification with ERROR code
    testResponse = TestResponseToNotification( 
        notificationRequestId,
        serverQueueId,
        ERROR
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);

    // Test 12 - check is subscription is deleted
    testResponse = TestSingleRequestWithUint32Response(
        0,                              //< Instance is not important
        NUMBER_OF_SUBSCRIPTION,         //< 
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        true,                           //< Validate Response
        0,   //< Expected Response
        OK,                             //< Expected response status
        requestId                       //< Request ID
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);

    // Test 13 - check is subscription is deleted
    testResponse = TestSingleRequestWithUint32Response(
        0,                              //< Instance is not important
        NUMBER_OF_ACTIVE_SUBSCRIPTION,  //< 
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        true,                           //< Validate Response
        0U,                             //< Expected Response
        OK,                             //< Expected response status
        requestId                       //< Request ID
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);


    // Test 14 - Try to remove all subscriptions
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

    // Test 15 - check number of subscription after removing all
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
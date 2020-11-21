#include "../testUtils/testUtils.h"

#define THIS_TEST_PATH "./Tests/Functional/resetRequest/resetRequestTest.c"

int main()
{
    int serverQueueId = TestUtilGetServerQueue();
    int appQueueId    = TestUtilGetAndPrepareTestQueue( THIS_TEST_PATH );
    uint32_t requestId = 1U;

    wholeTestResponse_t wholeTestResponse = {0};
    testResponses_t testResponse = TEST_OK;

    // Test 1 - Reset MINIMUM_PHASE_VOLTAGE - Proper instance
    testResponse = TestResetRequestWithShortConfirmationResponse(
        1U,                             //< Good instance number
        RESET_MINIMUM_PHASE_VOLTAGE,    //< Reset attribute MINIMUM_PHASE_VOLTAGE
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        OK,                             //< Expected response status
        requestId                       //< Request ID    
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);

    // Test 2 - Reset MINIMUM_PHASE_VOLTAGE - Bad instance
    testResponse = TestResetRequestWithShortConfirmationResponse(
        7U,                             //< Bad instance number
        RESET_MINIMUM_PHASE_VOLTAGE,    //< Reset attribute MINIMUM_PHASE_VOLTAGE
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        BAD_INSTANCE,                   //< Expected response status
        requestId                       //< Request ID    
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);

    // Test 3 - Reset MAXIMUM_PHASE_VOLTAGE - Proper instance
    testResponse = TestResetRequestWithShortConfirmationResponse(
        1U,                             //< Good instance number
        RESET_MAXIMUM_PHASE_VOLTAGE,    //< Reset attribute MAXIMUM_PHASE_VOLTAGE
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        OK,                             //< Expected response status
        requestId                       //< Request ID    
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);

    // Test 4 - Reset MAXIMUM_PHASE_VOLTAGE - Bad instance
    testResponse = TestResetRequestWithShortConfirmationResponse(
        7U,                             //< Bad instance number
        RESET_MAXIMUM_PHASE_VOLTAGE,    //< Reset attribute MAXIMUM_PHASE_VOLTAGE
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        BAD_INSTANCE,                   //< Expected response status
        requestId                       //< Request ID    
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);

    // Test 5 - Reset MINIMUM_PHASE_CURRENT - Proper instance
    testResponse = TestResetRequestWithShortConfirmationResponse(
        1U,                             //< Good instance number
        RESET_MINIMUM_PHASE_CURRENT,    //< Reset attribute MINIMUM_PHASE_CURRENT
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        OK,                             //< Expected response status
        requestId                       //< Request ID    
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);

    // Test 6 - Reset MINIMUM_PHASE_CURRENT - Bad instance
    testResponse = TestResetRequestWithShortConfirmationResponse(
        7U,                             //< Bad instance number
        RESET_MINIMUM_PHASE_CURRENT,    //< Reset attribute MINIMUM_PHASE_CURRENT
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        BAD_INSTANCE,                   //< Expected response status
        requestId                       //< Request ID    
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);

    // Test 7 - Reset MAXIMUM_PHASE_CURRENT - Proper instance
    testResponse = TestResetRequestWithShortConfirmationResponse(
        1U,                             //< Good instance number
        RESET_MAXIMUM_PHASE_CURRENT,    //< Reset attribute MAXIMUM_PHASE_CURRENT
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        OK,                             //< Expected response status
        requestId                       //< Request ID    
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);

    // Test 8 - Reset MAXIMUM_PHASE_CURRENT - Bad instance
    testResponse = TestResetRequestWithShortConfirmationResponse(
        7U,                             //< Bad instance number
        RESET_MAXIMUM_PHASE_CURRENT,    //< Reset attribute MAXIMUM_PHASE_CURRENT
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        BAD_INSTANCE,                   //< Expected response status
        requestId                       //< Request ID    
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);

    // Test 9 - Reset RESET_UNDER_VOLTAGE_THRESEHOLD - Proper instance
    testResponse = TestResetRequestWithShortConfirmationResponse(
        1U,                             //< Good instance number
        RESET_UNDER_VOLTAGE_THRESEHOLD, //< Reset attribute RESET_UNDER_VOLTAGE_THRESEHOLD
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        OK,                             //< Expected response status
        requestId                       //< Request ID    
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);

    // Test 10 - Reset RESET_UNDER_VOLTAGE_THRESEHOLD - Bad instance
    testResponse = TestResetRequestWithShortConfirmationResponse(
        7U,                             //< Bad instance number
        RESET_UNDER_VOLTAGE_THRESEHOLD, //< Reset attribute RESET_UNDER_VOLTAGE_THRESEHOLD
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        BAD_INSTANCE,                   //< Expected response status
        requestId                       //< Request ID    
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);

    // Test 11 - Reset RESET_OVER_VOLTAGE_THRESEHOLD - Proper instance
    testResponse = TestResetRequestWithShortConfirmationResponse(
        1U,                             //< Good instance number
        RESET_OVER_VOLTAGE_THRESEHOLD,  //< Reset attribute RESET_OVER_VOLTAGE_THRESEHOLD
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        OK,                             //< Expected response status
        requestId                       //< Request ID    
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);

    // Test 12 - Reset RESET_OVER_VOLTAGE_THRESEHOLD - Bad instance
    testResponse = TestResetRequestWithShortConfirmationResponse(
        7U,                             //< Bad instance number
        RESET_OVER_VOLTAGE_THRESEHOLD,  //< Reset attribute RESET_OVER_VOLTAGE_THRESEHOLD
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        BAD_INSTANCE,                   //< Expected response status
        requestId                       //< Request ID    
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);

    // Test 13 - Attribute not supported
    testResponse = TestResetRequestWithShortConfirmationResponse(
        1U,                             //< Good instance number
        METER_NUMBER,                   //< Bad attribute
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        BAD_ATTRIBUTE,                  //< Expected response status
        requestId                       //< Request ID    
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);

    PrintTestResults( wholeTestResponse, THIS_TEST_PATH );
}
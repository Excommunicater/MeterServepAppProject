#include "../testUtils/testUtils.h"
#include "../../../commonIncludes/projectGenerals.h"

#define THIS_TEST_PATH "./Tests/Functional/setSingle/setSingleTest.c"

int main()
{
    int serverQueueId = TestUtilGetServerQueue();
    int appQueueId    = TestUtilGetAndPrepareTestQueue( THIS_TEST_PATH );
    uint32_t requestId = 1U;

    wholeTestResponse_t wholeTestResponse = {0};
    testResponses_t testResponse = TEST_OK;

    // Test 1 - Set UNDER_VOLTAGE_THRESEHOLD - Proper instance
    testResponse = TestSetRequestWithShortConfirmationResponse(
        1U,                             //< Good instance number
        UNDER_VOLTAGE_THRESEHOLD,       //< Set attribute UNDER_VOLTAGE_THRESEHOLD
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        OK,                             //< Expected response status
        requestId,                      //< Request ID   
        12345                           //< Value to set 
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);

    // Test 2 - Set UNDER_VOLTAGE_THRESEHOLD - Bad instance
    testResponse = TestSetRequestWithShortConfirmationResponse(
        12U,                            //< Bad instance number
        UNDER_VOLTAGE_THRESEHOLD,       //< Set attribute UNDER_VOLTAGE_THRESEHOLD
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        BAD_INSTANCE,                   //< Expected response status
        requestId,                      //< Request ID   
        12345                           //< Value to set 
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);

    // Test 3 - Set OVER_VOLTAGE_THRESEHOLD - Proper instance
    testResponse = TestSetRequestWithShortConfirmationResponse(
        1U,                             //< Good instance number
        OVER_VOLTAGE_THRESEHOLD,        //< Set attribute OVER_VOLTAGE_THRESEHOLD
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        OK,                             //< Expected response status
        requestId,                      //< Request ID   
        12345                           //< Value to set 
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);

    // Test 4 - Set OVER_VOLTAGE_THRESEHOLD - Bad instance
    testResponse = TestSetRequestWithShortConfirmationResponse(
        12U,                            //< Bad instance number
        UNDER_VOLTAGE_THRESEHOLD,       //< Set attribute OVER_VOLTAGE_THRESEHOLD
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        BAD_INSTANCE,                   //< Expected response status
        requestId,                      //< Request ID   
        12345                           //< Value to set 
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);

    // Test 5 - Set not supported attribute
    testResponse = TestSetRequestWithShortConfirmationResponse(
        1U,                             //< Good instance number
        METER_NUMBER,                   //< Set not settable attribute
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        BAD_ATTRIBUTE,                  //< Expected response status
        requestId,                      //< Request ID   
        12345                           //< Value to set 
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);


    PrintTestResults( wholeTestResponse, THIS_TEST_PATH );
}

#include "../testUtils/testUtils.h"
#include "../../../commonIncludes/projectGenerals.h"

#define THIS_TEST_PATH "./Tests/Functional/getSingle/getSingleTest.c"

int main()
{
    int serverQueueId = TestUtilGetServerQueue();
    int appQueueId    = TestUtilGetAndPrepareTestQueue( THIS_TEST_PATH );

    uint32_t requestId = 1U;

    wholeTestResponse_t wholeTestResponse = {0};
    testResponses_t testResponse = TEST_OK;

    // Test 1 - Request for Meter server version
    testResponse = TestSingleRequestWithUint32Response(
        0,                      //< Instance is not important
        METER_SERVER_VERSION,   //< Check attribute METER_SERVER_VERSION
        appQueueId,             //< Pass test response queue ID
        serverQueueId,          //< Pass server request queue ID
        true,                   //< Validate Response
        SERVER_VERSION,         //< Expected Response
        OK,                     //< Expected response status
        requestId               //< Request ID
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);

    // Test 2 - Request for Meter Number
    testResponse = TestSingleRequestWithUint32Response(
        0,                      //< Instance is not important
        METER_NUMBER,           //< Check attribute METER_NUMBER
        appQueueId,             //< Pass test response queue ID
        serverQueueId,          //< Pass server request queue ID
        true,                   //< Validate Response
        01234567U,              //< Expected Response
        OK,                     //< Expected response status
        requestId               //< Request ID
    );

    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);

    // Test 3 - Request for INSTATNTENOUS_PHASE_VOLTAGE - Proper instance
    testResponse = TestSingleRequestWithUint32Response(
        1,                              //< Proper instance number
        INSTATNTENOUS_PHASE_VOLTAGE,    //< Check attribute INSTATNTENOUS_PHASE_VOLTAGE
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        false,                          //< Do not validate Response
        0,                              //< No matter
        OK,                             //< Expected response status
        requestId                       //< Request ID
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);

    // Test 4 - Request for INSTATNTENOUS_PHASE_VOLTAGE - Bad instance
    testResponse = TestSingleRequestWithUint32Response(
        9U,                             //< Bad instance number
        INSTATNTENOUS_PHASE_VOLTAGE,    //< Check attribute INSTATNTENOUS_PHASE_VOLTAGE
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        false,                          //< Do not validate Response
        0,                              //< No matter
        BAD_INSTANCE,                   //< Expected response status
        requestId                       //< Request ID
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);

    // Test 5 - Request for INSTATNTENOUS_PHASE_CURRENT - Proper instance
    testResponse = TestSingleRequestWithUint32Response(
        1,                              //< Proper instance number
        INSTATNTENOUS_PHASE_CURRENT,    //< Check attribute INSTATNTENOUS_PHASE_CURRENT
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        false,                          //< Do not validate Response
        0,                              //< No matter
        OK,                             //< Expected response status
        requestId                       //< Request ID
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);

    // Test 6 - Request for INSTATNTENOUS_PHASE_CURRENT - Bad instance
    testResponse = TestSingleRequestWithUint32Response(
        9U,                             //< Bad instance number
        INSTATNTENOUS_PHASE_CURRENT,    //< Check attribute INSTATNTENOUS_PHASE_CURRENT
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        false,                          //< Do not validate Response
        0,                              //< No matter
        BAD_INSTANCE,                   //< Expected response status
        requestId                       //< Request ID
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);

    // Test 7 - Request for VOLTAGE_PHASE_ANGLE - Proper instance
    testResponse = TestSingleRequestWithUint32Response(
        1,                              //< Proper instance number
        VOLTAGE_PHASE_ANGLE,            //< Check attribute VOLTAGE_PHASE_ANGLE
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        false,                          //< Do not validate Response
        0,                              //< No matter
        OK,                             //< Expected response status
        requestId                       //< Request ID
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);

    // Test 8 - Request for VOLTAGE_PHASE_ANGLE - Bad instance
    testResponse = TestSingleRequestWithUint32Response(
        9U,                             //< Bad instance number
        VOLTAGE_PHASE_ANGLE,            //< Check attribute VOLTAGE_PHASE_ANGLE
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        false,                          //< Do not validate Response
        0,                              //< No matter
        BAD_INSTANCE,                   //< Expected response status
        requestId                       //< Request ID       
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);

    // Test 9 - Request for CURRENT_PHASE_ANGLE - Proper instance
    testResponse = TestSingleRequestWithUint32Response(
        1,                              //< Proper instance number
        CURRENT_PHASE_ANGLE,            //< Check attribute CURRENT_PHASE_ANGLE
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        false,                          //< Do not validate Response
        0,                              //< No matter
        OK,                             //< Expected response status
        requestId                       //< Request ID
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);

    // Test 10 - Request for CURRENT_PHASE_ANGLE - Bad instance
    testResponse = TestSingleRequestWithUint32Response(
        9U,                             //< Bad instance number
        CURRENT_PHASE_ANGLE,            //< Check attribute CURRENT_PHASE_ANGLE
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        false,                          //< Do not validate Response
        0,                              //< No matter
        BAD_INSTANCE,                   //< Expected response status
        requestId                       //< Request ID       
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);

    // Test 11 - Request for MINIMUM_PHASE_VOLTAGE - Proper instance
    testResponse = TestSingleRequestWithUint32Response(
        2U,                             //< Good instance number
        MINIMUM_PHASE_VOLTAGE,          //< Check attribute CURRENT_PHASE_ANGLE
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        false,                          //< Do not validate Response
        0,                              //< No matter
        OK,                             //< Expected response status
        requestId                       //< Request ID       
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);

    // Test 12 - Request for MINIMUM_PHASE_VOLTAGE - Bad instance
    testResponse = TestSingleRequestWithUint32Response(
        12U,                            //< Bad instance number
        MINIMUM_PHASE_VOLTAGE,          //< Check attribute CURRENT_PHASE_ANGLE
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        false,                          //< Do not validate Response
        0,                              //< No matter
        BAD_INSTANCE,                   //< Expected response status
        requestId                       //< Request ID       
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);

    // Test 13 - Request for MAXIMUM_PHASE_VOLTAGE - Proper instance
    testResponse = TestSingleRequestWithUint32Response(
        2U,                             //< Good instance number
        MAXIMUM_PHASE_VOLTAGE,          //< Check attribute CURRENT_PHASE_ANGLE
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        false,                          //< Do not validate Response
        0,                              //< No matter
        OK,                             //< Expected response status
        requestId                       //< Request ID       
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);

    // Test 14 - Request for MAXIMUM_PHASE_VOLTAGE - Bad instance
    testResponse = TestSingleRequestWithUint32Response(
        12U,                            //< Bad instance number
        MAXIMUM_PHASE_VOLTAGE,          //< Check attribute CURRENT_PHASE_ANGLE
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        false,                          //< Do not validate Response
        0,                              //< No matter
        BAD_INSTANCE,                   //< Expected response status
        requestId                       //< Request ID       
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);

    // Test 15 - Request for MINIMUM_PHASE_CURRENT - Proper instance
    testResponse = TestSingleRequestWithUint32Response(
        2U,                             //< Good instance number
        MINIMUM_PHASE_CURRENT,          //< Check attribute CURRENT_PHASE_ANGLE
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        false,                          //< Do not validate Response
        0,                              //< No matter
        OK,                             //< Expected response status
        requestId                       //< Request ID       
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);

    // Test 16 - Request for MINIMUM_PHASE_CURRENT - Bad instance
    testResponse = TestSingleRequestWithUint32Response(
        12U,                            //< Bad instance number
        MINIMUM_PHASE_CURRENT,          //< Check attribute CURRENT_PHASE_ANGLE
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        false,                          //< Do not validate Response
        0,                              //< No matter
        BAD_INSTANCE,                   //< Expected response status
        requestId                       //< Request ID       
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);

    // Test 17 - Request for MAXIMUM_PHASE_CURRENT - Proper instance
    testResponse = TestSingleRequestWithUint32Response(
        2U,                             //< Good instance number
        MAXIMUM_PHASE_CURRENT,          //< Check attribute CURRENT_PHASE_ANGLE
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        false,                          //< Do not validate Response
        0,                              //< No matter
        OK,                             //< Expected response status
        requestId                       //< Request ID       
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);

    // Test 18 - Request for MAXIMUM_PHASE_CURRENT - Bad instance
    testResponse = TestSingleRequestWithUint32Response(
        12U,                            //< Bad instance number
        MAXIMUM_PHASE_CURRENT,          //< Check attribute CURRENT_PHASE_ANGLE
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        false,                          //< Do not validate Response
        0,                              //< No matter
        BAD_INSTANCE,                   //< Expected response status
        requestId                       //< Request ID       
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);

    // Test 19 - Request Not supported attribute
    testResponse = TestGetSingleRequestShortConfirmationResponse(
        1U,                             //< Good instance number
        97U,                            //< Get not existing attribute
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        BAD_ATTRIBUTE,                  //< Expected response status
        requestId                       //< Request ID    
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);

    // Test 20 - Request for PHASE_TIME_INTEGRAL_A_PLUS - Good instance
    testResponse = TestSingleRequestWithUint64Response(
        12U,                            //< Good instance number
        PHASE_TIME_INTEGRAL_A_PLUS,     //< Check attribute PHASE_TIME_INTEGRAL_A_PLUS
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        false,                          //< Do not validate Response
        0,                              //< No matter
        BAD_INSTANCE,                   //< Expected response status
        requestId                       //< Request ID       
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);

    // Test 21 - Request for PHASE_TIME_INTEGRAL_A_PLUS - Bad instance
    testResponse = TestSingleRequestWithUint64Response(
        12U,                            //< Bad instance number
        PHASE_TIME_INTEGRAL_A_PLUS,     //< Check attribute PHASE_TIME_INTEGRAL_A_PLUS
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        false,                          //< Do not validate Response
        0,                              //< No matter
        BAD_INSTANCE,                   //< Expected response status
        requestId                       //< Request ID       
    );
    requestId++;
    ParseTestResponse(testResponse, &wholeTestResponse);

    PrintTestResults( wholeTestResponse, THIS_TEST_PATH );
}

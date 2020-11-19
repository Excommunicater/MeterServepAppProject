//--External includes-------------------------------------------------
#include <stdio.h>              // pritnf()
#include <sys/stat.h>           // mkfifo()
#include <unistd.h>             // usleep()
#include <sys/ipc.h>            // key_t
#include <sys/msg.h>            // msgget()
#include <stdlib.h>             // exit()
//--------------------------------------------------------------------

//--Local includes----------------------------------------------------
#include "../ServerUtils/serverUtils.h"     //
#include "../commonIncludes/serverMessages.h" 
//--------------------------------------------------------------------

//--Defines-----------------------------------------------------------
#define APP_PATH "./ApplicationStart/meter_app.c"
#define APP_REV  666

//--------------------------------------------------------------------
typedef enum testResponses
{
    TEST_OK = 0,
    TEST_ERROR_SENDING_REQUEST,
    TEST_ERROR_RECIVE_RESPONSE,
    TEST_ERROR_NOT_EXPECTED_VALUE,
    TEST_ERROR_NOT_EXPECTED_STATUS
} testResponses_t;

typedef struct wholeTestResponse
{
    uint8_t passTest;
    uint8_t numberOfTests;
} wholeTestResponse_t;


//--Global Variables--------------------------------------------------
//--------------------------------------------------------------------

//--Function Declaration----------------------------------------------
testResponses_t TestSingleRequestWithUint32Response( 
    uint8_t instance, 
    attributesToGet_t attribute, 
    int responseQueueId, 
    int serverQueueId,
    bool validateResponseValue, 
    uint32_t expectedResponse,
    uint8_t expectedStatus );
void PrintTestResults( wholeTestResponse_t result );
void PrintTestResponse( testResponses_t response );
void ParseTestResponse( testResponses_t singleTestResponse, wholeTestResponse_t * pWholeTestResponse );
//--------------------------------------------------------------------

int main( void )
{
    int serverQueueId = InitServerMessageQueue();
    int appQueueId = InitMessageQueue( APP_PATH );

    wholeTestResponse_t wholeTestResponse = {0};

    testResponses_t testResponse = TEST_OK;

    // Test 1 - Request for Meter server version
    testResponse = TestSingleRequestWithUint32Response(
        0,                      //< Instance is not important
        METER_SERVER_VERSION,   //< Check attribute METER_SERVER_VERSION
        appQueueId,             //< Pass test response queue ID
        serverQueueId,          //< Pass server request queue ID
        true,                   //< Validate Response
        01234567U,              //< Expected Response
        OK                      //< Expected response status
    );
    ParseTestResponse(testResponse, &wholeTestResponse);

    // Test 2 - Request for Meter Number
    testResponse = TestSingleRequestWithUint32Response(
        0,                      //< Instance is not important
        METER_NUMBER,           //< Check attribute METER_NUMBER
        appQueueId,             //< Pass test response queue ID
        serverQueueId,          //< Pass server request queue ID
        true,                   //< Validate Response
        98765432U,              //< Expected Response
        OK                      //< Expected response status
    );
    ParseTestResponse(testResponse, &wholeTestResponse);

    // Test 3 - Request for INSTATNTENOUS_PHASE_VOLTAGE - Proper instance
    testResponse = TestSingleRequestWithUint32Response(
        1,                              //< Proper instance number
        INSTATNTENOUS_PHASE_VOLTAGE,    //< Check attribute INSTATNTENOUS_PHASE_VOLTAGE
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        false,                          //< Do not validate Response
        0,                              //< No matter
        OK                              //< Expected response status
    );
    ParseTestResponse(testResponse, &wholeTestResponse);

    // Test 4 - Request for INSTATNTENOUS_PHASE_VOLTAGE - Bad instance
    testResponse = TestSingleRequestWithUint32Response(
        9U,                             //< Bad instance number
        INSTATNTENOUS_PHASE_VOLTAGE,    //< Check attribute INSTATNTENOUS_PHASE_VOLTAGE
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        false,                          //< Do not validate Response
        0,                              //< No matter
        BAD_INSTANCE                    //< Expected response status
    );
    ParseTestResponse(testResponse, &wholeTestResponse);

    // Test 5 - Request for INSTATNTENOUS_PHASE_CURRENT - Proper instance
    testResponse = TestSingleRequestWithUint32Response(
        1,                              //< Proper instance number
        INSTATNTENOUS_PHASE_CURRENT,    //< Check attribute INSTATNTENOUS_PHASE_CURRENT
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        false,                          //< Do not validate Response
        0,                              //< No matter
        OK                              //< Expected response status
    );
    ParseTestResponse(testResponse, &wholeTestResponse);

    // Test 6 - Request for INSTATNTENOUS_PHASE_CURRENT - Bad instance
    testResponse = TestSingleRequestWithUint32Response(
        9U,                             //< Bad instance number
        INSTATNTENOUS_PHASE_CURRENT,    //< Check attribute INSTATNTENOUS_PHASE_CURRENT
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        false,                          //< Do not validate Response
        0,                              //< No matter
        BAD_INSTANCE                    //< Expected response status
    );
    ParseTestResponse(testResponse, &wholeTestResponse);

    // Test 7 - Request for VOLTAGE_PHASE_ANGLE - Proper instance
    testResponse = TestSingleRequestWithUint32Response(
        1,                              //< Proper instance number
        VOLTAGE_PHASE_ANGLE,            //< Check attribute VOLTAGE_PHASE_ANGLE
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        false,                          //< Do not validate Response
        0,                              //< No matter
        OK                              //< Expected response status
    );
    ParseTestResponse(testResponse, &wholeTestResponse);

    // Test 8 - Request for VOLTAGE_PHASE_ANGLE - Bad instance
    testResponse = TestSingleRequestWithUint32Response(
        9U,                             //< Bad instance number
        VOLTAGE_PHASE_ANGLE,            //< Check attribute VOLTAGE_PHASE_ANGLE
        appQueueId,                     //< Pass test response queue ID
        serverQueueId,                  //< Pass server request queue ID
        false,                          //< Do not validate Response
        0,                              //< No matter
        BAD_INSTANCE                    //< Expected response status
    );
    ParseTestResponse(testResponse, &wholeTestResponse);


    PrintTestResults( wholeTestResponse );
    
}

testResponses_t TestSingleRequestWithUint32Response( 
    uint8_t instance, 
    attributesToGet_t attribute, 
    int responseQueueId, 
    int serverQueueId,
    bool validateResponseValue, 
    uint32_t expectedResponse,
    uint8_t expectedStatus )
{
    bool operationStatus = false;
    requestSingleGet_t request;
    responseUint32_t responseUint32;
    requestSingleGetBody_t * pRequestBody = (requestSingleGetBody_t*)request.mtext;
    responseUint32Body_t * pResponseBody = (responseUint32Body_t*)responseUint32.mtext;

    //--Set Request Message-----------------------------------------------
    request.mtype                   = SINGLE_GET_REQUEST;
    pRequestBody->attribute         = attribute;
    pRequestBody->instance          = instance;
    pRequestBody->queueResponseId   = responseQueueId;
    //--------------------------------------------------------------------

    // Try to push Request to server
    operationStatus = PushMessageToQueue( (void*)&request, SINGLE_GET_REQUEST, serverQueueId );

    // I know it's good to have only one return from function... 
    // But it will increase readibility - appliest to whole function
    if ( operationStatus == false )
    {
        return TEST_ERROR_SENDING_REQUEST;
    }

    // Wait a while
    sleep(1);
    
    if ( GetMessageFromQueue( (void*)&responseUint32, UINT32_RESPONSE, responseQueueId ) )
    {
        pResponseBody = (responseUint32Body_t*)responseUint32.mtext;

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

void PrintTestResults( wholeTestResponse_t result )
{
    printf("--------------------------------------------------------------------\r\n");
    printf("                     T E S T  C O M P L E T E D                     \r\n");
    printf("--------------------------------------------------------------------\r\n");
    printf(" TEST PASSED:  %i\r\n", result.passTest);
    printf(" TEST FAILED:  %i\r\n", result.numberOfTests - result.passTest);
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

        default:
            printf("Some strange response...\r\n");
            break;
    }
}

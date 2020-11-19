#ifndef SERVER_MESSAGES_H
#define SERVER_MESSAGES_H

#include <stdint.h>

#define PROJECT_ID 123
#define SERVER_PATH_NAME  "./commonIncludes/serverMessages.h"

#define NUMBER_OF_REQUEST_TYPES  4
#define NUMBER_OF_RESPONSE_TYPES 3

#define NUMBER_OF_MESSAGE_TYPES NUMBER_OF_REQUEST_TYPES + NUMBER_OF_RESPONSE_TYPES

//--Request Data Types and Structures----------------------------------
typedef enum messageTypeRequest
{
    SINGLE_GET_REQUEST = 1U,
    RESET_REQUEST,
    SET_EVENT_REQUEST,
    RESET_EVENT_REQUEST
} messageTypeRequest_t;

typedef enum attributesToGet
{
    METER_NUMBER = 0U,
    METER_SERVER_VERSION,
    INSTATNTENOUS_PHASE_VOLTAGE,
    INSTATNTENOUS_PHASE_CURRENT,
    VOLTAGE_PHASE_ANGLE,
    CURRENT_PHASE_ANGLE
} attributesToGet_t;

typedef struct requestSingleGetBody
{
    uint32_t requestId;          //< Id of partiqular request
    int queueResponseId;         //< To this queue ID response shall be sent
    uint8_t instance;            //< Phaze number - if attribute not phaze related - just ignore it!
    attributesToGet_t attribute; //< Attribute number to get
} requestSingleGetBody_t;

typedef struct requestSingleGet
{
    long mtype;
    char mtext[sizeof(requestSingleGetBody_t)];
} requestSingleGet_t;

//--------------------------------------------------------------------

//--Response Data Types and Structures--------------------------------
typedef enum messageTypeResponse
{
    SHORT_CONFIRMATION_RESPONSE = NUMBER_OF_REQUEST_TYPES + 1,
    UINT32_RESPONSE,
    UINT64_RESPONSE
} messageTypeResponse_t;

typedef enum shortConfirmationValues
{
    OK = 0U,
    ERROR,
    BAD_INSTANCE
} shortConfirmationValues_t;

typedef struct responseShortConfirmationBody
{
        uint32_t requestId; 
    shortConfirmationValues_t confirmationValue;
    uint32_t someData1;
} responseShortConfirmationBody_t;

typedef struct responseShortConfirmation
{
    long mtype;
    char mtext[sizeof(responseShortConfirmationBody_t)];
} responseShortConfirmation_t;

typedef struct responseUint32Body
{
    uint32_t requestId; 
    uint32_t value;
    uint8_t  status;
} responseUint32Body_t;

typedef struct responseUint32
{
    long mtype;
    char mtext[sizeof(responseUint32Body_t)];
} responseUint32_t;


typedef struct responseUint64
{
    uint32_t requestId; 
    uint64_t value;
    uint32_t status;
} responseUint64_t;
//--------------------------------------------------------------------

#endif // SERVER_MESSAGES_H
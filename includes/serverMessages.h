#ifndef SERVER_MESSAGES_H
#define SERVER_MESSAGES_H

#include <stdint.h>

#define PROJECT_ID 123
#define SERVER_PATH_NAME  "./includes/serverMessages.h"

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

typedef struct requestSingleGetBody
{
    int queueResponseId;
    uint32_t someData1;
    uint32_t someData2;
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
    SIMPLE_OK = 0U,
    SIMPLE_ERROR
} shortConfirmationValues_t;

typedef struct responseShortConfirmationBody
{
    shortConfirmationValues_t confirmationValue;
    uint32_t someData1;
} responseShortConfirmationBody_t;

typedef struct responseShortConfirmation
{
    long mtype;
    char mtext[sizeof(responseShortConfirmationBody_t)];
} responseShortConfirmation_t;

typedef struct responseUint32
{
    uint32_t value;
    uint32_t status;
} responseUint32_t;

typedef struct responseUint64
{
    uint64_t value;
    uint32_t status;
} responseUint64_t;
//--------------------------------------------------------------------

#endif // SERVER_MESSAGES_H
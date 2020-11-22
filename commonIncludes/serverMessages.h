#ifndef SERVER_MESSAGES_H
#define SERVER_MESSAGES_H

#include <stdint.h>

#include "projectGenerals.h"

#define NUMBER_OF_REQUEST_TYPES  5
#define NUMBER_OF_RESPONSE_TYPES 4

#define NUMBER_OF_MESSAGE_TYPES NUMBER_OF_REQUEST_TYPES + NUMBER_OF_RESPONSE_TYPES

typedef enum messageTypeRequest
{
    GET_SINGLE_REQUEST = 1U,
    RESET_REQUEST,
    SET_SINGLE_REQUEST,
    SUBSCRIBE_REQUEST,
    NOTIFICATION,
    // Next value have to be the last one and is invalid to use. Add new ones before it!
    LAST_REQUEST_TYPE 
} messageTypeRequest_t;

typedef enum messageTypeResponse
{
    SHORT_CONFIRMATION_RESPONSE = LAST_REQUEST_TYPE,
    UINT32_RESPONSE,
    UINT64_RESPONSE,
    SUBSCRIPTION_RESPONSE,
    // Next value have to be the last one and is invalid to use. Add new ones before it!
    LAST_RESPONSE_TYPE 
} messageTypeResponse_t;

typedef enum attributesToGet
{
    METER_NUMBER = 0U,
    METER_SERVER_VERSION,
    INSTATNTENOUS_PHASE_VOLTAGE,
    INSTATNTENOUS_PHASE_CURRENT,
    VOLTAGE_PHASE_ANGLE,
    CURRENT_PHASE_ANGLE,
    MINIMUM_PHASE_VOLTAGE,
    MAXIMUM_PHASE_VOLTAGE,
    MINIMUM_PHASE_CURRENT,
    MAXIMUM_PHASE_CURRENT,
    NUMBER_OF_SUBSCRIPTION,
    NUMBER_OF_ACTIVE_SUBSCRIPTION,
    // Next value have to be the last one and is invalid to use. Add new ones before it!
    LAST_ATTRIBUTE_NUMBER_TO_GET  
} attributesToGet_t;

typedef enum attributesToSet
{
    UNDER_VOLTAGE_THRESEHOLD = LAST_ATTRIBUTE_NUMBER_TO_GET,
    OVER_VOLTAGE_THRESEHOLD,
    // Next value have to be the last one and is invalid to use. Add new ones before it!
    LAST_ATTRIBUTE_NUMBER_TO_SET
} attributesToSet_t;

typedef enum attributesToReset
{
    RESET_MINIMUM_PHASE_VOLTAGE = LAST_ATTRIBUTE_NUMBER_TO_SET,
    RESET_MAXIMUM_PHASE_VOLTAGE,
    RESET_MINIMUM_PHASE_CURRENT,
    RESET_MAXIMUM_PHASE_CURRENT,
    RESET_UNDER_VOLTAGE_THRESEHOLD,
    RESET_OVER_VOLTAGE_THRESEHOLD,
    UNSUBSCRIBE,
    UNSUBSCRIBE_ALL,
    // Next value have to be the last one and is invalid to use. Add new ones before it!
    LAST_ATTRIBUTE_NUMBER_TO_RESET
} attributesToReset_t;

typedef enum subscription
{
    UNDER_VOLTAGE_SUBSCRIPTION = 0U,
    OVER_VOLTAGE_SUBSCRIPTION
} subscription_t;

typedef enum subscriptionRegistrationStatus
{
    SUBSCRIPTION_REGISTERED = 0U,
    SUBSCRIPTION_BAD_SUBSCRIPTION_REQUEST,
    SUBSCRIPTION_LIST_FULL,
    SUBSCRIPTION_ALREADY_EXIST
} subscriptionRegistrationStatus_t;

typedef enum shortConfirmationValues
{
    OK = 0U,
    ERROR,
    BAD_INSTANCE,
    BAD_ATTRIBUTE
} shortConfirmationValues_t;
//--------------------------------------------------------------------

//--Request Data Types -----------------------------------------------
typedef struct requestSingleGetBody
{
    uint32_t requestId;          //< Id of particular request
    int queueResponseId;         //< To this queue ID response shall be sent
    uint8_t instance;            //< Phaze number - if attribute not phaze related - just ignore it!
    attributesToGet_t attribute; //< Attribute number to get
} requestSingleGetBody_t;

typedef struct requestSingleGet
{
    long mtype;
    char mtext[sizeof(requestSingleGetBody_t)];
} requestSingleGet_t;

typedef struct requestResetBody
{
    uint32_t requestId;            //< Id of particular request
    int queueResponseId;           //< To this queue ID response shall be sent
    uint8_t instance;              //< Phaze number or notificationId
    attributesToReset_t attribute; //< Attribute to reset
    uint8_t additionalData;
} requestResetBody_t;

typedef struct requestReset
{
    long mtype;
    char mtext[sizeof(requestResetBody_t)];
} requestReset_t;

typedef struct requestSingleSetBody
{
    uint32_t requestId;          //< Id of particular request
    int queueResponseId;         //< To this queue ID response shall be sent
    uint8_t instance;            //< Phaze number
    attributesToSet_t attribute; //< Attribute number to set
    uint32_t valueToSet;         //< Value to set
} requestSingleSetBody_t;

typedef struct requestSingleSet
{
    long mtype;
    char mtext[sizeof(requestSingleSetBody_t)];
} requestSingleSet_t;

typedef struct requestSubscriptionBody
{
    uint32_t requestId;       //< Id of particular request
    int queueResponseId;      //< To this queue ID response shall be sent
    uint8_t instance;         //< Phaze number
    subscription_t attribute; //< Subscription type
} requestSubscriptionBody_t;

typedef struct requestSubscription
{
    long mtype;
    char mtext[sizeof(requestSubscriptionBody_t)];
} requestSubscription_t;

typedef struct notificationBody
{
    uint32_t requestId; 
    uint8_t notificationId;
    #if SERVER_64_BIT == true
        uint64_t timeStamp;
    #elif
        uint32_t timeStamp;
    #endif

} notificationBody_t;

typedef struct notificationMessage
{
    long mtype;
    char mtext[sizeof(notificationBody_t)];
} notificationMessage_t;

//--------------------------------------------------------------------

//--Response Data Types ----------------------------------------------
typedef struct responseShortConfirmationBody
{
    uint32_t requestId; 
    shortConfirmationValues_t confirmationValue;
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
    shortConfirmationValues_t status;
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
    shortConfirmationValues_t status;
} responseUint64_t;

typedef struct responseSubscriptionBody
{
    uint32_t requestId; 
    subscriptionRegistrationStatus_t confirmationValue;
    uint8_t notificationId;
} responseSubscriptionBody_t;

typedef struct responseSubscription
{
    long mtype;
    char mtext[sizeof(responseSubscriptionBody_t)];
} responseSubscription_t;

//--------------------------------------------------------------------

#endif // SERVER_MESSAGES_H
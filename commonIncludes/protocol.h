#ifndef PROTOCOL_H
#define PROTOCOL_H
//--External includes-------------------------------------------------
#include <stdint.h>
//--------------------------------------------------------------------

//--Project includes--------------------------------------------------
#include "projectGenerals.h"
//--------------------------------------------------------------------

//--Defines-----------------------------------------------------------
#define NUMBER_OF_MESSAGE_TYPES (LAST_RESPONSE_TYPE - 1)
//--------------------------------------------------------------------

//--------------------------------------------------------------------
// enum: messageTypeRequest 
// Keeps all types of messages used for request
//
// @warning - LAST_REQUEST_TYPE have to be the last one 
//            and is invalid to use. Add new ones before it!
//--------------------------------------------------------------------
typedef enum messageTypeRequest
{                                                       // Request direction
    GET_SINGLE_REQUEST = 1U,                            // APP -> SERVER
    RESET_REQUEST,                                      // APP -> SERVER
    SET_SINGLE_REQUEST,                                 // APP -> SERVER
    SUBSCRIBE_REQUEST,                                  // APP -> SERVER
    NOTIFICATION,                                       // APP <- SERVER
    LAST_REQUEST_TYPE 
} messageTypeRequest_t;
//--------------------------------------------------------------------

//--------------------------------------------------------------------
// enum: messageTypeResponse 
// Keeps all types of messages used as responses
//
// @warning - LAST_RESPONSE_TYPE have to be the last one 
//            and is invalid to use. Add new ones before it!
//--------------------------------------------------------------------
typedef enum messageTypeResponse
{                                                       // Response direction
    SHORT_CONFIRMATION_RESPONSE = LAST_REQUEST_TYPE,    // APP <-> SERVER
    UINT32_RESPONSE,                                    // APP <-  SERVER
    UINT64_RESPONSE,                                    // APP <-  SERVER
    SUBSCRIPTION_RESPONSE,                              // APP <-  SERVER
    LAST_RESPONSE_TYPE 
} messageTypeResponse_t;
//--------------------------------------------------------------------


//--------------------------------------------------------------------
// enum: attributesToGet 
// Keeps all valid attributes to get by request GET_SINGLE_REQUEST
//
// @warning - LAST_ATTRIBUTE_NUMBER_TO_GET have to be the last one 
//            and is invalid to use. Add new ones before it!
//--------------------------------------------------------------------
typedef enum attributesToGet
{
    METER_NUMBER = 0U,
    METER_SERVER_VERSION,
    INSTATNTENOUS_PHASE_VOLTAGE,
    INSTATNTENOUS_PHASE_CURRENT,
    PHASE_TIME_INTEGRAL_A_PLUS,
    PHASE_TIME_INTEGRAL_A_MINUS,
    VOLTAGE_PHASE_ANGLE,
    CURRENT_PHASE_ANGLE,
    MINIMUM_PHASE_VOLTAGE,
    MAXIMUM_PHASE_VOLTAGE,
    MINIMUM_PHASE_CURRENT,
    MAXIMUM_PHASE_CURRENT,
    NUMBER_OF_SUBSCRIPTION,
    NUMBER_OF_ACTIVE_SUBSCRIPTION,
    LAST_ATTRIBUTE_NUMBER_TO_GET  
} attributesToGet_t;
//--------------------------------------------------------------------

//--------------------------------------------------------------------
// enum: attributesToSet 
// Keeps all valid attributes to set by request SET_SINGLE_REQUEST
//
// @warning - LAST_ATTRIBUTE_NUMBER_TO_SET have to be the last one 
//            and is invalid to use. Add new ones before it!
//--------------------------------------------------------------------
typedef enum attributesToSet
{
    UNDER_VOLTAGE_THRESEHOLD = LAST_ATTRIBUTE_NUMBER_TO_GET,
    OVER_VOLTAGE_THRESEHOLD,
    LAST_ATTRIBUTE_NUMBER_TO_SET
} attributesToSet_t;
//--------------------------------------------------------------------

//--------------------------------------------------------------------
// enum: attributesToReset 
// Keeps all valid attributes to reset by request RESET_REQUEST
//
// @warning - LAST_ATTRIBUTE_NUMBER_TO_RESET have to be the last one 
//            and is invalid to use. Add new ones before it!
//--------------------------------------------------------------------
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
    LAST_ATTRIBUTE_NUMBER_TO_RESET
} attributesToReset_t;
//--------------------------------------------------------------------

//--------------------------------------------------------------------
// enum: subscription 
// Keeps all types of subscriptions for which can apply in SUBSCRIBE_REQUEST 
//--------------------------------------------------------------------
typedef enum subscription
{
    UNDER_VOLTAGE_SUBSCRIPTION = 0U,
    OVER_VOLTAGE_SUBSCRIPTION
} subscription_t;
//--------------------------------------------------------------------

//--------------------------------------------------------------------
// enum: subscriptionRegistrationStatus 
// Keeps all responses types sent in  SUBSCRIPTION_RESPONSE
//--------------------------------------------------------------------
typedef enum subscriptionRegistrationStatus
{
    SUBSCRIPTION_REGISTERED = 0U,
    SUBSCRIPTION_BAD_SUBSCRIPTION_REQUEST,
    SUBSCRIPTION_LIST_FULL,
    SUBSCRIPTION_ALREADY_EXIST
} subscriptionRegistrationStatus_t;
//--------------------------------------------------------------------

//--------------------------------------------------------------------
// enum: shortConfirmationValues 
// Keeps all responses types sent in SHORT_CONFIRMATION_RESPONSE
//--------------------------------------------------------------------
typedef enum shortConfirmationValues
{
    OK = 0U,
    ERROR,
    BAD_INSTANCE,
    BAD_ATTRIBUTE
} shortConfirmationValues_t;
//--------------------------------------------------------------------

//--------------------------------------------------------------------
// struct: requestSingleGetBody 
// Body of GET_SINGLE_REQUEST message
//--------------------------------------------------------------------
typedef struct requestSingleGetBody
{
    uint32_t requestId;          //< Id of particular request
    int queueResponseId;         //< To this queue ID response shall be sent
    uint8_t instance;            //< Phaze number - if attribute not phaze related - just ignore it!
    attributesToGet_t attribute; //< Attribute number to get
} requestSingleGetBody_t;
//--------------------------------------------------------------------

//--------------------------------------------------------------------
// struct: requestSingleGet 
// Full message for requesting single attribute (GET_SINGLE_REQUEST)
//--------------------------------------------------------------------
typedef struct requestSingleGet
{
    long mtype;                 //< Have to contain value GET_SINGLE_REQUEST
    char mtext[sizeof(requestSingleGetBody_t)];
} requestSingleGet_t;
//--------------------------------------------------------------------

//--------------------------------------------------------------------
// struct: requestResetBody 
// Body of RESET_REQUEST message 
//--------------------------------------------------------------------
typedef struct requestResetBody
{
    uint32_t requestId;            //< Id of particular request
    int queueResponseId;           //< To this queue ID response shall be sent
    uint8_t instance;              //< Phaze number or notificationId
    attributesToReset_t attribute; //< Attribute to reset
    //uint8_t additionalData;
} requestResetBody_t;
//--------------------------------------------------------------------

//--------------------------------------------------------------------
// struct: requestReset 
// Full message for reseting attribute (RESET_REQUEST)
//--------------------------------------------------------------------
typedef struct requestReset
{
    long mtype;             //< Have to contain value RESET_REQUEST
    char mtext[sizeof(requestResetBody_t)];
} requestReset_t;
//--------------------------------------------------------------------

//--------------------------------------------------------------------
// struct: requestSingleSetBody 
// Body of SET_SINGLE_REQUEST message 
//--------------------------------------------------------------------
typedef struct requestSingleSetBody
{
    uint32_t requestId;          //< Id of particular request
    int queueResponseId;         //< To this queue ID response shall be sent
    uint8_t instance;            //< Phaze number
    attributesToSet_t attribute; //< Attribute number to set
    uint32_t valueToSet;         //< Value to set
} requestSingleSetBody_t;
//--------------------------------------------------------------------

//--------------------------------------------------------------------
// struct: requestSingleGet 
// Full message for requesting single attribute (SET_SINGLE_REQUEST)
//--------------------------------------------------------------------
typedef struct requestSingleSet
{
    long mtype;             //< Have to contain value SET_SINGLE_REQUEST
    char mtext[sizeof(requestSingleSetBody_t)];
} requestSingleSet_t;
//--------------------------------------------------------------------

//--------------------------------------------------------------------
// struct: requestSubscriptionBody 
// Body of SUBSCRIBE_REQUEST message 
//--------------------------------------------------------------------
typedef struct requestSubscriptionBody
{
    uint32_t requestId;       //< Id of particular request
    int queueResponseId;      //< To this queue ID response shall be sent
    uint8_t instance;         //< Phaze number
    subscription_t attribute; //< Subscription type
} requestSubscriptionBody_t;
//--------------------------------------------------------------------

//--------------------------------------------------------------------
// struct: requestSubscription 
// Full message for requesting for a subscription (SUBSCRIBE_REQUEST)
//--------------------------------------------------------------------
typedef struct requestSubscription
{
    long mtype;             //< Have to contain value SUBSCRIBE_REQUEST
    char mtext[sizeof(requestSubscriptionBody_t)];
} requestSubscription_t;
//--------------------------------------------------------------------

//--------------------------------------------------------------------
// struct: notificationBody 
// Body of NOTIFICATION message 
// @note: This message is sent as request from server to app
//
// @warning: Keep in mind that after receiving this message, 
//           notification on subscription is blocked until 
//           response is back on server side.
//           Application have to respond with the same requestId 
//           if want to enable or remove this notification.
//           
// If any questions, look closer to this test:
// ../Tests/Functional/subscribeNotification 
//           
//--------------------------------------------------------------------
typedef struct notificationBody
{
    uint32_t requestId;         //< Id of particular request.
    uint8_t notificationId;     //< Unique ID of notification
    #if SERVER_64_BIT == true
        uint64_t timeStamp;     // Contains time stamp
    #elif
        uint32_t timeStamp;
    #endif

} notificationMessageBody_t;
//--------------------------------------------------------------------

//--------------------------------------------------------------------
// struct: requestSubscription 
// Full message notification (NOTIFICATION) 
//--------------------------------------------------------------------
typedef struct notificationMessage
{
    long mtype;         //< Have to contain value NOTIFICATION
    char mtext[sizeof(notificationMessageBody_t)];
} notificationMessage_t;
//--------------------------------------------------------------------

//--------------------------------------------------------------------
// struct: responseShortConfirmationBody 
// Body of SHORT_CONFIRMATION_RESPONSE message 
//
// @note This message type can be sent from APP to Server and Server to APP
//
// Usage: 
//          -> response after request for bad attribute or instance
//          -> response after SET_SINGLE_REQUEST and RESET_REQUEST
//          -> response (APP side!) after NOTIFICATION
//-------------------------------------------------------------------- 
typedef struct responseShortConfirmationBody
{
    uint32_t requestId;                          //< Id of particular request.
    shortConfirmationValues_t confirmationValue; //< Value to respond
} responseShortConfirmationBody_t;

//--------------------------------------------------------------------
// struct: responseShortConfirmation 
// Full message notification (SHORT_CONFIRMATION_RESPONSE) 
//--------------------------------------------------------------------
typedef struct responseShortConfirmation
{
    long mtype;         //< Have to contain value SHORT_CONFIRMATION_RESPONSE
    char mtext[sizeof(responseShortConfirmationBody_t)];
} responseShortConfirmation_t;

//--------------------------------------------------------------------
// struct: responseUint32Body 
// Body of UINT32_RESPONSE message 
//
// Usage: 
//          -> most responses for GET_SINGLE_REQUEST
//--------------------------------------------------------------------
typedef struct responseUint32Body
{
    uint32_t requestId;              //< Id of particular request.
    uint32_t value;                  //< Value to respond
    shortConfirmationValues_t status;//< Can contain some additional information (i.e contain error if requesting for bad instance)
} responseUint32Body_t;
//--------------------------------------------------------------------

//--------------------------------------------------------------------
// struct: responseUint32 
// Full message reponse (UINT32_RESPONSE) 
//--------------------------------------------------------------------
typedef struct responseUint32
{
    long mtype;     //< Have to contain value UINT32_RESPONSE
    char mtext[sizeof(responseUint32Body_t)];
} responseUint32_t;
//--------------------------------------------------------------------

//--------------------------------------------------------------------
// struct: responseUint64Body 
// Body of UINT64_RESPONSE message 
//
// Usage: 
//          -> some responses for GET_SINGLE_REQUEST
//--------------------------------------------------------------------
typedef struct responseUint64Body
{
    uint32_t requestId;              //< Id of particular request.
    uint64_t value;                  //< Value to respond
    shortConfirmationValues_t status;//< Can contain some additional information
} responseUint64Body_t;
//--------------------------------------------------------------------

//--------------------------------------------------------------------
// struct: responseUint64_t 
// Full message reponse (UINT64_RESPONSE) 
//--------------------------------------------------------------------
typedef struct responseUint64
{
    long mtype;     //< Have to contain value UINT64_RESPONSE
    char mtext[sizeof(responseUint64Body_t)];
} responseUint64_t;
//--------------------------------------------------------------------

//--------------------------------------------------------------------
// struct: responseSubscriptionBody 
// Body of SUBSCRIBE_REQUEST message 
//
// Usage: 
//          -> response after SUBSCRIBE_REQUEST
//--------------------------------------------------------------------
typedef struct responseSubscriptionBody
{
    uint32_t requestId;                                 //< Id of particular request.
    subscriptionRegistrationStatus_t confirmationValue; //< Status of adding subscription
    uint8_t notificationId;                             //< If success - contains ID of particular notification
} responseSubscriptionBody_t;
//--------------------------------------------------------------------

//--------------------------------------------------------------------
// struct: responseSubscription 
// Full message reponse (SUBSCRIPTION_RESPONSE) 
//--------------------------------------------------------------------
typedef struct responseSubscription
{
    long mtype;     //< Have to contain value SUBSCRIPTION_RESPONSE
    char mtext[sizeof(responseSubscriptionBody_t)];
} responseSubscription_t;
//--------------------------------------------------------------------

#endif // PROTOCOL_H

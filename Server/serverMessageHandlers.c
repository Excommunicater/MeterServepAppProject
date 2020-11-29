//--External includes-------------------------------------------------
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
//--------------------------------------------------------------------

//--Project includes--------------------------------------------------
#include "projectGenerals.h"
#include "protocol.h"
#include "messagingUtils.h"
#include "instantaneousMeterValues.h"
#include "minMaxValues.h"
//--------------------------------------------------------------------

//--Local includes----------------------------------------------------
#include "serverQueueUtils.h"
#include "notificationUtils.h"
//--------------------------------------------------------------------

//--Defines-----------------------------------------------------------
#define MH_DBG_PRNT
//--------------------------------------------------------------------

//--Consts------------------------------------------------------------
static const uint32_t ATTRIBUTE_SERVER_VERSION = SERVER_VERSION; // For now it's hardcodded - Should it be get from device?
static const uint32_t ATTRIBUTE_METER_NUMBER   = 01234567U;      // For now it's hardcodded - Should it be get from device?
//--------------------------------------------------------------------

void HandleSingleGetRequest( void )
{
    requestSingleGet_t requestMessage;
    if ( GetMessageFromServerQueue( (void *)&requestMessage, GET_SINGLE_REQUEST ) )
    {
        bool responseStatus                  = false;
        requestSingleGetBody_t * messageBody = (requestSingleGetBody_t*)requestMessage.mtext;
        long responseQueueId                 = messageBody->queueResponseId;
        uint32_t requestId                   = messageBody->requestId;
        attributesToGet_t attribute          = messageBody->attribute;
        uint8_t instance                     = messageBody->instance;
        shortConfirmationValues_t status     = ERROR;
        uint32_t returnUint32Value           = 0U;
        uint64_t returnUint64Value           = 0U;
        switch ( attribute )
        {
            case METER_NUMBER:
                #ifdef MH_DBG_PRNT
                    printf("HandleSingleGetRequest::METER_NUMBER rID = %i i = %i v = %i stat = %i\r\n", requestId, instance, ATTRIBUTE_METER_NUMBER, OK);
                #endif
                responseStatus = SendResponseUint32( ATTRIBUTE_METER_NUMBER, OK, responseQueueId, requestId );
                break;
        
            case METER_SERVER_VERSION:
                #ifdef MH_DBG_PRNT
                    printf("HandleSingleGetRequest::METER_SERVER_VERSION rID = %i i = %i v = %i stat = %i\r\n", requestId, instance, ATTRIBUTE_SERVER_VERSION, OK);
                #endif
                responseStatus = SendResponseUint32( ATTRIBUTE_SERVER_VERSION, OK, responseQueueId, requestId );
                break;

            case INSTATNTENOUS_PHASE_VOLTAGE:
                returnUint32Value = GetInstatntenousPhaseVoltage( &status, instance );
                #ifdef MH_DBG_PRNT
                    printf("HandleSingleGetRequest::INSTATNTENOUS_PHASE_VOLTAGE rID = %i i = %i v = %i stat = %i\r\n", requestId,instance, returnUint32Value, status);
                #endif
                responseStatus = SendResponseUint32( returnUint32Value, status, responseQueueId, requestId );
                break;

            case INSTATNTENOUS_PHASE_CURRENT:
                returnUint32Value = GetInstatntenousPhaseCurrent( &status, instance );
                #ifdef MH_DBG_PRNT
                    printf("HandleSingleGetRequest::INSTATNTENOUS_PHASE_CURRENT rID = %i i = %i v = %i stat = %i\r\n",  requestId,instance, returnUint32Value, status);
                #endif
                responseStatus = SendResponseUint32( returnUint32Value, status, responseQueueId, requestId );
                break;
            case PHASE_TIME_INTEGRAL_A_PLUS:
                returnUint64Value = GetTimeIntegrals( &status, instance, A_PLUS );
                #ifdef MH_DBG_PRNT
                    printf("HandleSingleGetRequest::PHASE_TIME_INTEGRAL_A_PLUS rID = %i i = %i v = %li stat = %i\r\n",  requestId,instance, returnUint64Value, status);
                #endif
                responseStatus = SendResponseUint64( returnUint64Value, status, responseQueueId, requestId );
                break;            
            case PHASE_TIME_INTEGRAL_A_MINUS:
                returnUint64Value = GetTimeIntegrals( &status, instance, A_PLUS );
                #ifdef MH_DBG_PRNT
                    printf("HandleSingleGetRequest::PHASE_TIME_INTEGRAL_A_PLUS rID = %i i = %i v = %li stat = %i\r\n",  requestId,instance, returnUint64Value, status);
                #endif
                responseStatus = SendResponseUint64( returnUint64Value, status, responseQueueId, requestId );
                break;
            case VOLTAGE_PHASE_ANGLE:
                returnUint32Value = GetPhaseAngle( &status, instance, ANGLE_VOLTAGE );
                #ifdef MH_DBG_PRNT
                    printf("HandleSingleGetRequest::VOLTAGE_PHASE_ANGLE rID = %i i = %i v = %i stat = %i\r\n",  requestId,instance, returnUint32Value, status);
                #endif
                responseStatus = SendResponseUint32( returnUint32Value, status, responseQueueId, requestId );
                break;
            case CURRENT_PHASE_ANGLE:
                returnUint32Value = GetPhaseAngle( &status, instance, ANGLE_CURRENT );
                #ifdef MH_DBG_PRNT
                    printf("HandleSingleGetRequest::CURRENT_PHASE_ANGLE rID = %i i = %i v = %i stat = %i\r\n",  requestId,instance, returnUint32Value, status);
                #endif
                responseStatus = SendResponseUint32( returnUint32Value, status, responseQueueId, requestId );
                break;
            case MINIMUM_PHASE_VOLTAGE:
                returnUint32Value = GetMinMaxPhaseValue( &status, instance, ANGLE_VOLTAGE, ANGLE_MIN );
                #ifdef MH_DBG_PRNT
                    printf("HandleSingleGetRequest::MINIMUM_PHASE_VOLTAGE rID = %i i = %i v = %i stat = %i\r\n",  requestId,instance, returnUint32Value, status);
                #endif
                responseStatus = SendResponseUint32( returnUint32Value, status, responseQueueId, requestId );
                break;
            case MAXIMUM_PHASE_VOLTAGE:
                returnUint32Value = GetMinMaxPhaseValue( &status, instance, ANGLE_VOLTAGE, ANGLE_MAX );
                #ifdef MH_DBG_PRNT
                    printf("HandleSingleGetRequest::MAXIMUM_PHASE_VOLTAGE rID = %i i = %i v = %i stat = %i\r\n",  requestId,instance, returnUint32Value, status);
                #endif
                responseStatus = SendResponseUint32( returnUint32Value, status, responseQueueId, requestId );
                break;
            case MINIMUM_PHASE_CURRENT:
                returnUint32Value = GetMinMaxPhaseValue( &status, instance, ANGLE_CURRENT, ANGLE_MIN );
                #ifdef MH_DBG_PRNT
                    printf("HandleSingleGetRequest::MINIMUM_PHASE_CURRENT rID = %i i = %i v = %i stat = %i\r\n",  requestId,instance, returnUint32Value, status);
                #endif
                responseStatus = SendResponseUint32( returnUint32Value, status, responseQueueId, requestId );
                break;
            case MAXIMUM_PHASE_CURRENT:
                returnUint32Value = GetMinMaxPhaseValue( &status, instance, ANGLE_CURRENT, ANGLE_MAX );
                #ifdef MH_DBG_PRNT
                    printf("HandleSingleGetRequest::MAXIMUM_PHASE_CURRENT rID = %i i = %i v = %i stat = %i\r\n",  requestId,instance, returnUint32Value, status);
                #endif
                responseStatus = SendResponseUint32( returnUint32Value, status, responseQueueId, requestId );
                break;

            case NUMBER_OF_SUBSCRIPTION:
                returnUint32Value = GetNumberOfSubscriptions();
                #ifdef MH_DBG_PRNT
                    printf("HandleSingleGetRequest::NUMBER_OF_SUBSCRIPTION rID = %i v = %i\r\n", requestId, returnUint32Value);
                #endif
                responseStatus = SendResponseUint32( returnUint32Value, OK, responseQueueId, requestId );
                break;

            case NUMBER_OF_ACTIVE_SUBSCRIPTION:
                returnUint32Value = GetNumberOfActiveSubscriptions();
                #ifdef MH_DBG_PRNT
                    printf("HandleSingleGetRequest::NUMBER_OF_ACTIVE_SUBSCRIPTION rID = %i v = %i\r\n", requestId, returnUint32Value);
                #endif
                responseStatus = SendResponseUint32( returnUint32Value, OK, responseQueueId, requestId );
                break;
            default:
                // Attribute not supported
                #ifdef MH_DBG_PRNT
                    printf("HandleSingleGetRequest ATTRIBUTE NOT SUPPORTED!\r\n");
                #endif
                responseStatus = SendResponseShortConfirmation( BAD_ATTRIBUTE, responseQueueId, requestId );
                break;
        }

        if ( !responseStatus )
        {
            #ifdef MH_DBG_PRNT
                printf("ERROR DURING RESPONDING WITH UINT32_T!\r\n");
            #endif

        }
    }
}

void HandleSingleSetRequest( void )
{
    requestSingleSet_t requestMessage;
    if ( GetMessageFromServerQueue( (void *)&requestMessage, SET_SINGLE_REQUEST ) )
    {
        bool responseStatus = false;
        requestSingleSetBody_t * messageBody = (requestSingleSetBody_t*)requestMessage.mtext;
        long responseQueueId                 = messageBody->queueResponseId;
        uint32_t requestId                   = messageBody->requestId;
        attributesToSet_t attribute          = messageBody->attribute;
        uint8_t instance                     = messageBody->instance;
        uint32_t valueToSet                  = messageBody->valueToSet;
        shortConfirmationValues_t status     = ERROR;
        
        switch ( attribute )
        {
            case UNDER_VOLTAGE_THRESEHOLD:
                status = SetVoltageThresehold( instance, UNDERVOLTAGE, valueToSet );

                #ifdef MH_DBG_PRNT
                    printf("HandleSingleSetRequest::UNDER_VOLTAGE_THRESEHOLD rID = %i v = i = %i stat = %i\r\n", requestId, instance, status);
                #endif
                responseStatus = SendResponseShortConfirmation( status, responseQueueId, requestId );
                break;
            case OVER_VOLTAGE_THRESEHOLD:
                status = SetVoltageThresehold( instance, OVERVOLTAGE, valueToSet );
                #ifdef MH_DBG_PRNT
                    printf("HandleResetRequest::OVER_VOLTAGE_THRESEHOLD rID = %i i = %i stat = %i\r\n", requestId, instance, status);
                #endif
                responseStatus = SendResponseShortConfirmation( status, responseQueueId, requestId );
                break;
            default:
                // Attribute not supported
                #ifdef MH_DBG_PRNT
                    printf("HandleResetRequest ATTRIBUTE NOT SUPPORTED!\r\n");
                #endif
                responseStatus = SendResponseShortConfirmation( BAD_ATTRIBUTE, responseQueueId, requestId );
                break;
        }
        if ( !responseStatus )
        {
            #ifdef MH_DBG_PRNT
                printf("ERROR DURING RESPONDING!\r\n");
            #endif

        }
    }
}

void HandleResetRequest( void )
{
    requestReset_t requestMessage;
    if ( GetMessageFromServerQueue( (void *)&requestMessage, RESET_REQUEST ) )
    {
        bool responseStatus              = false;
        requestResetBody_t * messageBody = (requestResetBody_t*)requestMessage.mtext;
        long responseQueueId             = messageBody->queueResponseId;
        uint32_t requestId               = messageBody->requestId;
        attributesToReset_t attribute    = messageBody->attribute;
        uint8_t instance                 = messageBody->instance;
        shortConfirmationValues_t status = ERROR;
        switch ( attribute )
        {
            case RESET_MINIMUM_PHASE_VOLTAGE:
                status = ResetMinMaxPhaseValue( instance, ANGLE_VOLTAGE, ANGLE_MIN );
                #ifdef MH_DBG_PRNT
                    printf("HandleResetRequest::RESET_MINIMUM_PHASE_VOLTAGE rID = %i i = %i stat = %i\r\n", requestId, instance, status);
                #endif
                responseStatus = SendResponseShortConfirmation( status, responseQueueId, requestId );
                break;
            case RESET_MAXIMUM_PHASE_VOLTAGE:
                status = ResetMinMaxPhaseValue( instance, ANGLE_VOLTAGE, ANGLE_MAX );
                #ifdef MH_DBG_PRNT
                    printf("HandleResetRequest::RESET_MAXIMUM_PHASE_VOLTAGE rID = %i i = %i stat = %i\r\n", requestId, instance, status);
                #endif
                responseStatus = SendResponseShortConfirmation( status, responseQueueId, requestId );
                break;
            case RESET_MINIMUM_PHASE_CURRENT:
                status = ResetMinMaxPhaseValue( instance, ANGLE_CURRENT, ANGLE_MIN );
                #ifdef MH_DBG_PRNT
                    printf("HandleResetRequest::RESET_MINIMUM_PHASE_CURRENT rID = %i i = %i stat = %i\r\n", requestId, instance, status);
                #endif
                responseStatus = SendResponseShortConfirmation( status, responseQueueId, requestId );
                break;
            case RESET_MAXIMUM_PHASE_CURRENT:
                status = ResetMinMaxPhaseValue( instance, ANGLE_CURRENT, ANGLE_MIN );
                #ifdef MH_DBG_PRNT
                    printf("HandleResetRequest::RESET_MAXIMUM_PHASE_CURRENT rID = %i i = %i stat = %i\r\n", requestId, instance, status);
                #endif
                responseStatus = SendResponseShortConfirmation( status, responseQueueId, requestId );
                break;
            case RESET_UNDER_VOLTAGE_THRESEHOLD:
                status = ResetVoltageThresehold( instance, UNDERVOLTAGE );
                #ifdef MH_DBG_PRNT
                    printf("HandleResetRequest::RESET_UNDER_VOLTAGE_THRESEHOLD rID = %i i = %i stat = %i\r\n", requestId, instance, status);
                #endif
                responseStatus = SendResponseShortConfirmation( status, responseQueueId, requestId );
                break;
            case RESET_OVER_VOLTAGE_THRESEHOLD:
                status = ResetVoltageThresehold( instance, OVERVOLTAGE );
                #ifdef MH_DBG_PRNT
                    printf("HandleResetRequest::RESET_OVER_VOLTAGE_THRESEHOLD rID = %i i = %i stat = %i\r\n", requestId, instance, status);
                #endif
                responseStatus = SendResponseShortConfirmation( status, responseQueueId, requestId );
                break;
            case UNSUBSCRIBE:
                status = Unsubscribe( instance );
                #ifdef MH_DBG_PRNT
                    printf("HandleResetRequest::UNSUBSCRIBE rID = %i i = %i stat = %i\r\n", requestId, instance, status);
                #endif
                responseStatus = SendResponseShortConfirmation( status, responseQueueId, requestId );
                break;
            case UNSUBSCRIBE_ALL:
                status = UnsubscribeAll();
                #ifdef MH_DBG_PRNT
                    printf("HandleResetRequest::UNSUBSCRIBE_ALL rID = %i stat = %i\r\n", requestId, status);
                #endif
                responseStatus = SendResponseShortConfirmation( status, responseQueueId, requestId );
                break;
            default:
                // Attribute not supported
                #ifdef MH_DBG_PRNT
                    printf("HandleResetRequest ATTRIBUTE NOT SUPPORTED!\r\n");
                #endif
                responseStatus = SendResponseShortConfirmation( BAD_ATTRIBUTE, responseQueueId, requestId );
                break;
        }
        if ( !responseStatus )
        {
            #ifdef MH_DBG_PRNT
                printf("ERROR DURING RESPONDING!\r\n");
            #endif

        }
    }
}

void HandleSubscriptionRequest( void )
{
    requestSubscription_t requestMessage;
    if ( GetMessageFromServerQueue( (void *)&requestMessage, SUBSCRIBE_REQUEST ) )
    {
        bool responseStatus                     = false;
        requestSubscriptionBody_t * messageBody = (requestSubscriptionBody_t*)requestMessage.mtext;
        long responseQueueId                    = messageBody->queueResponseId;
        uint32_t requestId                      = messageBody->requestId;
        subscription_t attribute                = messageBody->attribute;
        uint8_t instance                        = messageBody->instance;
        uint8_t notificationId                  = 0U;

        subscriptionRegistrationStatus_t status = SUBSCRIPTION_BAD_SUBSCRIPTION_REQUEST;
        switch ( attribute )
        {
            case UNDER_VOLTAGE_SUBSCRIPTION:
                status = RegisterSubscription( instance, UNDERVOLTAGE, &notificationId, responseQueueId );
                #ifdef MH_DBG_PRNT
                    printf("HandleSubscriptionRequest::UNDER_VOLTAGE_SUBSCRIPTION rID = %i i = %i stat = %i\r\n", requestId, instance, status);
                #endif
                responseStatus = SendResponseSubscription( status, notificationId, responseQueueId, requestId );
                break;
            case OVER_VOLTAGE_SUBSCRIPTION:
                status = RegisterSubscription( instance, OVERVOLTAGE, &notificationId, responseQueueId );
                #ifdef MH_DBG_PRNT
                    printf("HandleSubscriptionRequest::OVER_VOLTAGE_SUBSCRIPTION rID = %i i = %i stat = %i\r\n", requestId, instance, status);
                #endif
                responseStatus = SendResponseSubscription( status, notificationId, responseQueueId, requestId );
                break;
            default:
                // Attribute not supported
                #ifdef MH_DBG_PRNT
                    printf("HandleSubscriptionRequest ATTRIBUTE NOT SUPPORTED!\r\n");
                #endif
                responseStatus = SendResponseSubscription( SUBSCRIPTION_BAD_SUBSCRIPTION_REQUEST, notificationId, responseQueueId, requestId );
                break;
        }
        if ( !responseStatus )
        {
            #ifdef MH_DBG_PRNT
                printf("ERROR DURING RESPONDING!\r\n");
            #endif
        }
    }
}

void HandleNotificationResponse( void )
{
    responseShortConfirmation_t response;
    if ( GetMessageFromServerQueue( (void *)&response, SHORT_CONFIRMATION_RESPONSE ) )
    {
        responseShortConfirmationBody_t * pResponseBody = (responseShortConfirmationBody_t*)response.mtext;

        if ( pResponseBody->confirmationValue == OK )
        {
            #ifdef MH_DBG_PRNT
                printf("HandleNotificationResponse: Unblock subscription!\r\n");
            #endif
            UnblockSubscriptionAfterNotification( pResponseBody->requestId );
        }
        else
        {
            #ifdef MH_DBG_PRNT
                printf("HandleNotificationResponse: Unsubscribe!\r\n");
            #endif
            UnsubscribeAfterNotification( pResponseBody->requestId ); 
        }
    }
}

void HandleSendingNotifications( void )
{
    notification_t notification;
    while ( PopNotification( &notification ) )
    {
        bool sendStatus = SendNotification( notification.notificationID, 
                                            notification.queueToSend, 
                                            notification.notificationMessageId, 
                                            notification.timeStamp );
        if ( !sendStatus )
        {
            // HANDLE IT!
            #ifdef MH_DBG_PRNT
                printf("ERROR DURING SENDING notification!\r\n");
            #endif
        }
    }
}


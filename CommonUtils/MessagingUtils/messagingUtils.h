#ifndef MESSAGING_UTILS_H
#define MESSAGING_UTILS_H

#include <stdbool.h>
#include <stdint.h>

#include "protocol.h"
#include "projectGenerals.h"

bool SendResponseUint32( uint32_t valueToResponse, shortConfirmationValues_t status, long responseQueue, uint32_t requestId );
bool SendResponseUint64( uint64_t valueToResponse, shortConfirmationValues_t status, long responseQueue, uint32_t requestId );

bool SendResponseShortConfirmation( shortConfirmationValues_t valueToResponse, long responseQueue, uint32_t requestId );
bool SendResponseSubscription( subscriptionRegistrationStatus_t confirmation, uint8_t notificationId, long responseQueue, uint32_t requestId );

bool SendRequestGetSingle( attributesToGet_t attributeToGet, uint8_t instanceToGet, long requestQueue, long responseQueue, uint32_t requestId );
bool SendRequestSetSingle( uint32_t valueToSet, attributesToSet_t attributeToSet, uint8_t instanceToSet, long requestQueue, long responseQueue, uint32_t requestId );
bool SendRequestReset( attributesToReset_t attributeToReset, uint8_t instanceToReset, long requestQueue, long responseQueue, uint32_t requestId );

bool SendRequestSubsciption( subscription_t subscriptionToRequest, uint8_t instanceToSubscribe,  long requestQueue, long responseQueue, uint32_t requestId );
#if SERVER_64_BIT == true
bool SendNotification( uint8_t notificationId, long queue, uint32_t requestId, uint64_t timeStamp );
#elif
bool SendNotification( uint8_t notificationId, uint32_t requestId, uint32_t timeStamp );
#endif

#endif // MESSAGING_UTILS_H
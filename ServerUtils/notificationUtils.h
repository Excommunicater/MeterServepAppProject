#ifndef NOTIFICATION_UTILS_H
#define NOTIFICATION_UTILS_H

#include <stdint.h>
#include <stdbool.h>

#include "../commonIncludes/serverMessages.h"

typedef enum theseholdType
{
    UNDERVOLTAGE = 0,
    OVERVOLTAGE
} theseholdType_t;

typedef struct notification
{
    uint8_t notificationID;
    int queueToSend;
    uint32_t notificationMessageId;
    #if SERVER_64_BIT == true
        uint64_t timeStamp;
    #elif
        uint32_t timeStamp;
    #endif
} notification_t;

shortConfirmationValues_t SetVoltageThresehold( uint8_t phase, theseholdType_t typeToSet, uint32_t valueToSet );
shortConfirmationValues_t ResetVoltageThresehold( uint8_t phase, theseholdType_t typeToReset );
subscriptionRegistrationStatus_t RegisterSubscription( uint8_t phase, theseholdType_t type, uint8_t * notificationId, int notificationQueue );
shortConfirmationValues_t Unsubscribe( uint8_t notificationId );
shortConfirmationValues_t UnsubscribeAll( void );
uint32_t GetNumberOfSubscriptions( void );
uint32_t GetNumberOfActiveSubscriptions( void );
bool PopNotification( notification_t * notification );
#endif // NOTIFICATION_UTILS_H
#ifndef NOTIFICATION_UTILS_H
#define NOTIFICATION_UTILS_H

#include <stdint.h>

#include "../commonIncludes/serverMessages.h"

typedef enum theseholdSetType
{
    UNDERVOLTAGE = 0,
    OVERVOLTAGE
} theseholdSetType_t;

shortConfirmationValues_t SetVoltageThresehold( uint8_t phase, theseholdSetType_t typeToSet, uint32_t valueToSet );
shortConfirmationValues_t ResetVoltageThresehold( uint8_t phase, theseholdSetType_t typeToReset );
#endif // NOTIFICATION_UTILS_H
#ifndef INSTANTANEOUS_METER_VALUES_H
#define INSTANTANEOUS_METER_VALUES_H

#include <stdint.h>

#include "protocol.h"
#include "meterUtils.h"

typedef enum timeIntegrals
{
    A_PLUS = 0,
    A_MINUS
} timeIntegrals_t;

uint32_t GetInstatntenousPhaseVoltage( shortConfirmationValues_t * status, uint8_t phase );
uint32_t GetInstatntenousPhaseCurrent( shortConfirmationValues_t * status, uint8_t phase );
uint32_t GetPhaseAngle( shortConfirmationValues_t * status, uint8_t phase, angleValue_t valueToGet );
uint64_t GetTimeIntegrals( shortConfirmationValues_t * status, uint8_t phase, timeIntegrals_t type );

#endif // INSTANTANEOUS_METER_VALUES_H
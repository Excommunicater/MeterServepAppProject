#ifndef MIN_MAX_VALUES_H
#define MIN_MAX_VALUES_H

#include "protocol.h"
#include "meterUtils.h"

#define MU_MMV_DBG_PRNT

typedef enum angleMinMax
{
    ANGLE_MIN = 0,
    ANGLE_MAX
} angleMinMax_t;

void StoreMaxMinValues( void );
uint32_t GetMinMaxPhaseValue( shortConfirmationValues_t * status, uint8_t phase, angleValue_t valueToGet, angleMinMax_t minOrMax );
shortConfirmationValues_t ResetMinMaxPhaseValue( uint8_t phase, angleValue_t valueToReset, angleMinMax_t minOrMax );

#endif // MIN_MAX_VALUES_H
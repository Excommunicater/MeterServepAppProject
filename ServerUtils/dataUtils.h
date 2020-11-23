#ifndef DATA_UTILS_H
#define DATA_UTILS_H

#include <stdint.h>
#include "../commonIncludes/serverMessages.h"

typedef enum angleValue
{
    ANGLE_VOLTAGE = 0,
    ANGLE_CURRENT
} angleValue_t;

typedef enum angleMinMax
{
    ANGLE_MIN = 0,
    ANGLE_MAX
} angleMinMax_t;

void InitMeter( void );
void ReadStructFromDev( void );
void StoreMaxMinValues( void );
uint32_t GetInstatntenousPhaseVoltage( shortConfirmationValues_t * status, uint8_t phase );
uint32_t GetInstatntenousPhaseCurrent( shortConfirmationValues_t * status, uint8_t phase );
uint32_t GetPhaseAngle( shortConfirmationValues_t * status, uint8_t phase, angleValue_t valueToGet );
uint32_t GetMinMaxPhaseValue( shortConfirmationValues_t * status, uint8_t phase, angleValue_t valueToGet, angleMinMax_t minOrMax );
shortConfirmationValues_t ResetMinMaxPhaseValue( uint8_t phase, angleValue_t valueToReset, angleMinMax_t minOrMax );

#endif // DATA_UTILS_H
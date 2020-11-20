#ifndef DATA_UTILS_H
#define DATA_UTILS_H

#include <stdint.h>  // uint32_t

typedef enum angleValue
{
    ANGLE_VOLTAGE = 0,
    ANGLE_CURRENT
} angleValue_t;

void ReadStructFromDev( void );
void StoreMaxMinValues( void );
uint32_t GetInstatntenousPhaseVoltage( uint8_t * status, uint8_t phase );
uint32_t GetInstatntenousPhaseCurrent( uint8_t * status, uint8_t phase );
uint32_t GetPhaseAngle( uint8_t * status, uint8_t phase, angleValue_t valueToGet );

#endif // DATA_UTILS_H
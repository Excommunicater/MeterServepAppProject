#ifndef METER_UTILS_H
#define METER_UTILS_H

#define BLOCKING_METER_READOUT              1
#define NON_BLOCKING_METER_READOUT          2
#define NO_METER_HARDCODED_VALUES           3

//#define METER_UTILS_STORE_AND_RESTORE_DATA
#define METER_UTILS_TYPE_OF_DATA_READOUT    BLOCKING_METER_READOUT

//#define MU_MU_DBG_PRNT

typedef enum angleValue
{
    ANGLE_VOLTAGE = 0,
    ANGLE_CURRENT
} angleValue_t;

void InitMeter( void );
void ReadStructFromDev( void );

#endif // METER_UTILS_H

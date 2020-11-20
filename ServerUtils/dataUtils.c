#include "dataUtils.h"
#include "errorHandling.h"
#include "../commonIncludes/serverMessages.h"     // OK, BAD_INSTANCE
#include "../commonIncludes/metering_interface.h" // PHASE_CNT

#include <stddef.h> // NULL - XD
#include <fcntl.h>  // open(), read()

//--Local Structures--------------------------------------------------
typedef struct maxMinPerPhaseVoltageAndCurrent
{
    uint32_t voltage;
    uint32_t current;
} maxMinPerPhaseVoltageAndCurrent_t;
//--------------------------------------------------------------------

//--File Scope Global Variables---------------------------------------
static meter_hw_registers_t lastReadHardwareRegister = {0};
static maxMinPerPhaseVoltageAndCurrent_t maxMinVI[PHASE_CNT] = {0};
//--------------------------------------------------------------------

uint32_t GetInstatntenousPhaseVoltage( uint8_t * status, uint8_t phase )
{
    uint32_t response = 0U;

    if ( status == (uint8_t*)NULL )
    {
        ReportAndExit("GetInstatntenousPhaseVoltage - passed NULL argument!");
    }

    if ( phase <= PHASE_CNT )
    {
        response = lastReadHardwareRegister.per_phase[phase].v;
        *status  = OK;
    }
    else
    {
        *status = BAD_INSTANCE;
    }
    return response;
}

uint32_t GetInstatntenousPhaseCurrent( uint8_t * status, uint8_t phase )
{
    uint32_t response = 0U;

    if ( status == (uint8_t*)NULL )
    {
        ReportAndExit("GetInstatntenousPhaseCurrent - passed NULL argument!");
    }

    if ( phase <= PHASE_CNT )
    {
        response = lastReadHardwareRegister.per_phase[phase].i;
        *status  = OK;
    }
    else
    {
        *status = BAD_INSTANCE;
    }
    return response;
}

uint32_t GetPhaseAngle( uint8_t * status, uint8_t phase, angleValue_t valueToGet )
{
    uint32_t response = 0U;
    if ( status == (uint8_t*)NULL )
    {
        ReportAndExit("GetPhaseAngle - passed NULL argument!");
    }
    if ( ( valueToGet > ANGLE_CURRENT ) || ( valueToGet < ANGLE_VOLTAGE ) )
    {
        ReportAndExit("GetPhaseAngle - passed bad value to get!");
    }

    if ( phase <= PHASE_CNT )
    {
        if ( valueToGet == ANGLE_VOLTAGE )
        {
            response = lastReadHardwareRegister.voltage_angles[phase];
        }
        else
        {
            response = lastReadHardwareRegister.current_angles[phase];
        }
        *status  = OK;
    }
    else
    {
        *status = BAD_INSTANCE;
    }
    return response;
}

void ReadStructFromDev( void )
{
    int fifoFile = open(DEV_FILE, O_RDONLY);
    read(fifoFile, &lastReadHardwareRegister, sizeof(meter_hw_registers_t)); 
    close(fifoFile);
    #ifdef DEBUG_PRINTOUT
        printf("V[2] = %i A[2] = %i A+[2] = %li A-[2] = %li\r\n", lastReadHardwareRegister.per_phase[2].v, pMeterReg.per_phase[2].i, pMeterReg.per_phase[2].ai,  pMeterReg.per_phase[2].ae);
    #endif
}

void StoreMaxMinValues( void )
{
    for (uint8_t i = 0U; i < PHASE_CNT; i++ )
    {
        if ( maxMinVI[i].current < lastReadHardwareRegister.per_phase[i].i )
        {
            maxMinVI[i].current = lastReadHardwareRegister.per_phase[i].i;
        }
        if ( maxMinVI[i].voltage < lastReadHardwareRegister.per_phase[i].v )
        {
            maxMinVI[i].voltage = lastReadHardwareRegister.per_phase[i].v;
        }
    }
}

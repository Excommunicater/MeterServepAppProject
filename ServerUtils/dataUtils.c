//--External includes-------------------------------------------------
#include <stddef.h>
#include <stdio.h> 
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <poll.h>
//--------------------------------------------------------------------

//--Project includes--------------------------------------------------
#include "dataUtils.h"
#include "fileUtils.h"
#include "errorHandling.h"
#include "../commonIncludes/serverMessages.h"
#include "../commonIncludes/metering_interface.h"
//--------------------------------------------------------------------

//--Defines-----------------------------------------------------------
#define BLOCKING_METER_READOUT     1
#define NON_BLOCKING_METER_READOUT 2
#define NO_METER_HARDCODED_VALUES  3

#define TYPE_OF_DATA_READOUT BLOCKING_METER_READOUT

//--------------------------------------------------------------------

//--Local Structures--------------------------------------------------
typedef struct maxMinPerPhaseVoltageAndCurrent
{
    uint32_t maxVoltage;
    uint32_t minVoltage;
    uint32_t maxCurrent;
    uint32_t minCurrent;
} maxMinPerPhaseVoltageAndCurrent_t;
//--------------------------------------------------------------------

//--File Scope Global Variables---------------------------------------
static meter_hw_registers_t lastReadHardwareRegister = {0};
static meter_hw_registers_t storedLastPowerCycleHardwareRegister = {0};
static maxMinPerPhaseVoltageAndCurrent_t maxMinVI[PHASE_CNT] = {0};
//--------------------------------------------------------------------


void InitMeter( void )
{
    mkfifo(DEV_FILE, 0666);

    // Restore stored values saved before power cycle  
    RestoreNonVolatileData( &storedLastPowerCycleHardwareRegister, sizeof(meter_hw_registers_t) );
}


uint32_t GetInstatntenousPhaseVoltage( shortConfirmationValues_t * status, uint8_t phase )
{
    uint32_t response = 0U;

    if ( status == (shortConfirmationValues_t*)NULL )
    {
        ReportAndExit("GetInstatntenousPhaseVoltage - passed NULL argument!");
    }

    if ( phase < PHASE_CNT )
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

uint32_t GetInstatntenousPhaseCurrent( shortConfirmationValues_t * status, uint8_t phase )
{
    uint32_t response = 0U;

    if ( status == (shortConfirmationValues_t*)NULL )
    {
        ReportAndExit("GetInstatntenousPhaseCurrent - passed NULL argument!");
    }

    if ( phase < PHASE_CNT )
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

uint32_t GetPhaseAngle( shortConfirmationValues_t * status, uint8_t phase, angleValue_t valueToGet )
{
    if ( status == (shortConfirmationValues_t*)NULL )
    {
        ReportAndExit("GetPhaseAngle - passed NULL argument!");
    }
    if ( ( valueToGet > ANGLE_CURRENT ) || ( valueToGet < ANGLE_VOLTAGE ) )
    {
        ReportAndExit("GetPhaseAngle - passed bad value to get!");
    }

    uint32_t response = 0U;

    if ( phase < PHASE_CNT )
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
#if TYPE_OF_DATA_READOUT == BLOCKING_METER_READOUT
    int fifoFile = open(DEV_FILE, O_RDONLY);
    read(fifoFile, &lastReadHardwareRegister, sizeof(meter_hw_registers_t));
    // Add to last read values data from previous powercycle
    lastReadHardwareRegister.per_phase[0].ai += storedLastPowerCycleHardwareRegister.per_phase[0].ai; 
    lastReadHardwareRegister.per_phase[1].ai += storedLastPowerCycleHardwareRegister.per_phase[1].ai; 
    lastReadHardwareRegister.per_phase[2].ai += storedLastPowerCycleHardwareRegister.per_phase[2].ai;
    lastReadHardwareRegister.per_phase[0].ae += storedLastPowerCycleHardwareRegister.per_phase[0].ae;
    lastReadHardwareRegister.per_phase[1].ae += storedLastPowerCycleHardwareRegister.per_phase[1].ae;
    lastReadHardwareRegister.per_phase[1].ae += storedLastPowerCycleHardwareRegister.per_phase[1].ae;
    StoreNonVolatileData( &lastReadHardwareRegister, sizeof(meter_hw_registers_t) );
    close(fifoFile);
#elif TYPE_OF_DATA_READOUT == NON_BLOCKING_METER_READOUT
    struct pollfd fdarray [1];
    int fifoFile = open(DEV_FILE, O_RDONLY);
    fdarray[0].fd = fifoFile;
    fdarray[0].events = POLLIN;
    int rc = poll(fdarray, 1, 1);
    if ( ( rc == 1 ) && ( fdarray[0].revents == POLLIN ) )
    {
        read(fifoFile, &lastReadHardwareRegister, sizeof(meter_hw_registers_t));
        // Add to last read values data from previous powercycle
        lastReadHardwareRegister.per_phase[0].ai += storedLastPowerCycleHardwareRegister.per_phase[0].ai; 
        lastReadHardwareRegister.per_phase[1].ai += storedLastPowerCycleHardwareRegister.per_phase[1].ai; 
        lastReadHardwareRegister.per_phase[2].ai += storedLastPowerCycleHardwareRegister.per_phase[2].ai;
        lastReadHardwareRegister.per_phase[0].ae += storedLastPowerCycleHardwareRegister.per_phase[0].ae;
        lastReadHardwareRegister.per_phase[1].ae += storedLastPowerCycleHardwareRegister.per_phase[1].ae;
        lastReadHardwareRegister.per_phase[1].ae += storedLastPowerCycleHardwareRegister.per_phase[1].ae;
        StoreNonVolatileData( &lastReadHardwareRegister, sizeof(meter_hw_registers_t) );
    }
    close(fifoFile);
#elif TYPE_OF_DATA_READOUT == NO_METER_HARDCODED_VALUES
        lastReadHardwareRegister.per_phase[0].v    = 102U;
        lastReadHardwareRegister.per_phase[0].i    = 102U;
        lastReadHardwareRegister.per_phase[0].ai   = 102U;
        lastReadHardwareRegister.per_phase[0].ae   = 102U;
        lastReadHardwareRegister.per_phase[1].v    = 102U;
        lastReadHardwareRegister.per_phase[1].i    = 102U;
        lastReadHardwareRegister.per_phase[1].ai   = 102U;
        lastReadHardwareRegister.per_phase[1].ae   = 102U;
        lastReadHardwareRegister.per_phase[2].v    = 102U;
        lastReadHardwareRegister.per_phase[2].i    = 102U;
        lastReadHardwareRegister.per_phase[2].ai   = 102U;
        lastReadHardwareRegister.per_phase[2].ae   = 102U;
        lastReadHardwareRegister.voltage_angles[0] = 102U;
        lastReadHardwareRegister.voltage_angles[1] = 102U;
        lastReadHardwareRegister.voltage_angles[2] = 102U;
        lastReadHardwareRegister.current_angles[0] = 102U;
        lastReadHardwareRegister.current_angles[1] = 102U;
        lastReadHardwareRegister.current_angles[2] = 102U;
#endif
}


void StoreMaxMinValues( void )
{
    for ( uint8_t i = 0U; i < PHASE_CNT; i++ )
    {
        if ( maxMinVI[i].maxCurrent < lastReadHardwareRegister.per_phase[i].i )
        {
            maxMinVI[i].maxCurrent = lastReadHardwareRegister.per_phase[i].i;
        }
        if ( maxMinVI[i].maxVoltage < lastReadHardwareRegister.per_phase[i].v )
        {
            maxMinVI[i].maxVoltage = lastReadHardwareRegister.per_phase[i].v;
        }
        if ( maxMinVI[i].minCurrent > lastReadHardwareRegister.per_phase[i].i )
        {
            maxMinVI[i].minCurrent = lastReadHardwareRegister.per_phase[i].i;
        }
        if ( maxMinVI[i].minVoltage > lastReadHardwareRegister.per_phase[i].v )
        {
            maxMinVI[i].minVoltage = lastReadHardwareRegister.per_phase[i].v;
        }
    }
}

uint32_t GetMinMaxPhaseValue( shortConfirmationValues_t * status, uint8_t phase, angleValue_t valueToGet, angleMinMax_t minOrMax )
{
    if ( status == (shortConfirmationValues_t*)NULL )
    {
        ReportAndExit("GetMinMaxPhaseValue - passed NULL argument!");
    }
    if ( ( valueToGet > ANGLE_CURRENT ) || ( valueToGet < ANGLE_VOLTAGE ) )
    {
        ReportAndExit("GetMinMaxPhaseValue - passed bad value to get!");
    }
    if ( ( minOrMax > ANGLE_MAX ) || ( minOrMax < ANGLE_MIN ) )
    {
        ReportAndExit("GetMinMaxPhaseValue - passed bad min or max descriptor!");
    }

    uint32_t response = 0U;

    if ( phase < PHASE_CNT )
    {
        if ( valueToGet == ANGLE_VOLTAGE )
        {
            if ( minOrMax == ANGLE_MAX )
            {
                response = maxMinVI[phase].maxVoltage;
            }
            else
            {
                response = maxMinVI[phase].minVoltage;
            }
        }
        else
        {
            if ( minOrMax == ANGLE_MAX )
            {
                response = maxMinVI[phase].maxCurrent;
            }
            else
            {
                response = maxMinVI[phase].minCurrent;
            }
        }
        *status  = OK;
    }
    else
    {
        *status = BAD_INSTANCE;
    }
    return response;
}

shortConfirmationValues_t ResetMinMaxPhaseValue( uint8_t phase, angleValue_t valueToReset, angleMinMax_t minOrMax )
{
    if ( ( valueToReset > ANGLE_CURRENT ) || ( valueToReset < ANGLE_VOLTAGE ) )
    {
        ReportAndExit("ResetMinMaxPhaseValue - passed bad value to get!");
    }
    if ( ( minOrMax > ANGLE_MAX ) || ( minOrMax < ANGLE_MIN ) )
    {
        ReportAndExit("ResetMinMaxPhaseValue - passed bad min or max descriptor!");
    }

    shortConfirmationValues_t retVal = ERROR;

    if ( phase < PHASE_CNT )
    {
        if ( valueToReset == ANGLE_VOLTAGE )
        {
            if ( minOrMax == ANGLE_MAX )
            {
                maxMinVI[phase].maxVoltage = 0U;
            }
            else
            {
                maxMinVI[phase].minVoltage = 0U;
            }
        }
        else
        {
            if ( minOrMax == ANGLE_MAX )
            {
                maxMinVI[phase].maxCurrent = 0U;
            }
            else
            {
                maxMinVI[phase].minCurrent = 0U;
            }
        }
        retVal  = OK; 
    }
    else
    {
        retVal = BAD_INSTANCE;
    }
    
    return retVal;
}

uint64_t GetTimeIntegrals( shortConfirmationValues_t * status, uint8_t phase, timeIntegrals_t type )
{
    if ( ( type > A_MINUS ) || ( type < A_PLUS ) )
    {
        ReportAndExit("GetTimeIntegrals - passed type to get!");
    }

    uint64_t response = 0U;
    if ( phase < PHASE_CNT )
    {
        if ( type == A_PLUS )
        {
            response = lastReadHardwareRegister.per_phase[phase].ai;
        }
        else
        {
            response = lastReadHardwareRegister.per_phase[phase].ae;
        }
    }
    else
    {
        *status = BAD_INSTANCE;
    }
        
    return response;
}

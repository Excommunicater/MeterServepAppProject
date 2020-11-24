//--External includes-------------------------------------------------
#include <stddef.h>
//--------------------------------------------------------------------

//--Project includes--------------------------------------------------
#include "errorHandling.h"
#include "instantaneousMeterValues.h"
#include "metering_interface.h"
//--------------------------------------------------------------------

//--Variables defined in different files------------------------------
extern meter_hw_registers_t lastReadHardwareRegister; //< Defined in meterUtils.c
//--------------------------------------------------------------------

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

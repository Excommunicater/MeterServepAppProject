//--External includes-------------------------------------------------
#include <stddef.h>
//--------------------------------------------------------------------

//--Project includes--------------------------------------------------
#include "minMaxValues.h"
#include "metering_interface.h"
#include "errorHandling.h"
//--------------------------------------------------------------------

#ifdef MU_MMV_DBG_PRNT
#include <stdio.h>
#endif

//--Local Structures--------------------------------------------------
typedef struct maxMinPerPhaseVoltageAndCurrent
{
    uint32_t maxVoltage;
    uint32_t minVoltage;
    uint32_t maxCurrent;
    uint32_t minCurrent;
} maxMinPerPhaseVoltageAndCurrent_t;
//--------------------------------------------------------------------

//--Variables defined in different files------------------------------
extern meter_hw_registers_t lastReadHardwareRegister; //< Defined in meterUtils.c
//--------------------------------------------------------------------

//--File Scope Global Variables---------------------------------------
static maxMinPerPhaseVoltageAndCurrent_t maxMinVI[PHASE_CNT] = {0};
//--------------------------------------------------------------------
void StoreMaxMinValues( void )
{
    for ( uint8_t i = 0U; i < PHASE_CNT; i++ )
    {
        if ( maxMinVI[i].maxCurrent < lastReadHardwareRegister.per_phase[i].i )
        {
            maxMinVI[i].maxCurrent = lastReadHardwareRegister.per_phase[i].i;
#ifdef MU_MMV_DBG_PRNT
            printf("StoreMaxMinValues maxMinVI[%i].maxCurrent=%i\r\n", i, maxMinVI[i].maxCurrent);
#endif
        }
        if ( maxMinVI[i].maxVoltage < lastReadHardwareRegister.per_phase[i].v )
        {
            maxMinVI[i].maxVoltage = lastReadHardwareRegister.per_phase[i].v;
#ifdef MU_MMV_DBG_PRNT
            printf("StoreMaxMinValues maxMinVI[%i].maxVoltage=%i\r\n", i, maxMinVI[i].maxVoltage);
#endif
        }
        if ( maxMinVI[i].minCurrent > lastReadHardwareRegister.per_phase[i].i )
        {
            maxMinVI[i].minCurrent = lastReadHardwareRegister.per_phase[i].i;
#ifdef MU_MMV_DBG_PRNT
            printf("StoreMaxMinValues maxMinVI[%i].minCurrent=%i\r\n", i, maxMinVI[i].minCurrent);
#endif
        }
        if ( maxMinVI[i].minVoltage > lastReadHardwareRegister.per_phase[i].v )
        {
            maxMinVI[i].minVoltage = lastReadHardwareRegister.per_phase[i].v;
#ifdef MU_MMV_DBG_PRNT
            printf("StoreMaxMinValues maxMinVI[%i].minVoltage=%i\r\n", i, maxMinVI[i].minVoltage);
#endif
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

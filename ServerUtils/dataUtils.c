#include "dataUtils.h"
#include "errorHandling.h"
#include "../commonIncludes/serverMessages.h"     // OK, BAD_INSTANCE
#include "../commonIncludes/metering_interface.h" // PHASE_CNT

#include <stdio.h>  // printf()
#include <stddef.h> // NULL - XD
#include <fcntl.h>  // open(), read()


//#define DU_DBG_PRNT

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
    #ifdef DU_DBG_PRNT
        //printf("V[2] = %i A[2] = %i A+[2] = %li A-[2] = %li\r\n", lastReadHardwareRegister.per_phase[2].v, lastReadHardwareRegister.per_phase[2].i, lastReadHardwareRegister.per_phase[2].ai,  lastReadHardwareRegister.per_phase[2].ae);
    #endif
}

void StoreMaxMinValues( void )
{
    for ( uint8_t i = 0U; i < PHASE_CNT; i++ )
    {
        if ( maxMinVI[i].maxCurrent < lastReadHardwareRegister.per_phase[i].i )
        {
            maxMinVI[i].maxCurrent = lastReadHardwareRegister.per_phase[i].i;
            #ifdef DU_DBG_PRNT
                printf("StoreMaxMinValues - Stored new max current[%i] = %i\r\n", i, maxMinVI[i].maxCurrent);
            #endif
        }
        if ( maxMinVI[i].maxVoltage < lastReadHardwareRegister.per_phase[i].v )
        {
            maxMinVI[i].maxVoltage = lastReadHardwareRegister.per_phase[i].v;
            #ifdef DU_DBG_PRNT
                printf("StoreMaxMinValues - Stored new max voltage[%i] = %i\r\n", i, maxMinVI[i].maxVoltage);
            #endif
        }
        if ( maxMinVI[i].minCurrent > lastReadHardwareRegister.per_phase[i].i )
        {
            #ifdef DU_DBG_PRNT
                printf("StoreMaxMinValues - Stored new min current[%i] = %i\r\n", i, maxMinVI[i].minCurrent);
            #endif
            maxMinVI[i].minCurrent = lastReadHardwareRegister.per_phase[i].i;
        }
        if ( maxMinVI[i].minVoltage > lastReadHardwareRegister.per_phase[i].v )
        {
            #ifdef DU_DBG_PRNT
                printf("StoreMaxMinValues - Stored new min voltage[%i] = %i\r\n", i, maxMinVI[i].minVoltage);
            #endif
            maxMinVI[i].minVoltage = lastReadHardwareRegister.per_phase[i].v;
        }
    }
}

uint32_t GetMinMaxPhaseValue( uint8_t * status, uint8_t phase, angleValue_t valueToGet, angleMinMax_t minOrMax )
{
    if ( status == (uint8_t*)NULL )
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

    if ( phase <= PHASE_CNT )
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
//--Local includes----------------------------------------------------
#include "notificationUtils.h"
#include "errorHandling.h"
#include "../commonIncludes/metering_interface.h" // PHASE_CNT
//--------------------------------------------------------------------
#include <stdbool.h>

//--Local Structures--------------------------------------------------
typedef struct thresehold
{
    uint32_t value;
    bool isSet;
} thresehold_t;
//--------------------------------------------------------------------
//--File Scope Global Variables---------------------------------------
static thresehold_t underVoltageThresehold[PHASE_CNT] = {0};
static thresehold_t overVoltageThresehold[PHASE_CNT] = {0};
//--------------------------------------------------------------------

shortConfirmationValues_t SetVoltageThresehold( uint8_t phase, theseholdSetType_t typeToSet, uint32_t valueToSet )
{
    
    if ( ( typeToSet > OVERVOLTAGE ) || ( typeToSet < UNDERVOLTAGE ) )
    {
        ReportAndExit("SetVoltageThresehold - passed bad value to set!");
    }

    shortConfirmationValues_t respVal = ERROR;
    if ( phase <= PHASE_CNT )
    {
        if ( typeToSet == UNDERVOLTAGE )
        {
            underVoltageThresehold[phase].value = valueToSet;
            underVoltageThresehold[phase].isSet = true;
        }
        else
        {
            overVoltageThresehold[phase].value = valueToSet;
            overVoltageThresehold[phase].isSet = true;
        }
        respVal = OK;
    }
    else
    {
        respVal = BAD_INSTANCE;
    }
    return respVal;
}

shortConfirmationValues_t ResetVoltageThresehold( uint8_t phase, theseholdSetType_t typeToReset )
{
    if ( ( typeToReset > OVERVOLTAGE ) || ( typeToReset < UNDERVOLTAGE ) )
    {
        ReportAndExit("ResetVoltageThresehold - passed bad value to set!");
    }
    shortConfirmationValues_t respVal = ERROR;
    if ( phase <= PHASE_CNT )
    {
        if ( typeToReset == UNDERVOLTAGE )
        {
            underVoltageThresehold[phase].isSet = false;
        }
        else
        {
            overVoltageThresehold[phase].isSet = false;
        }
        respVal = OK;
    }
    else
    {
        respVal = BAD_INSTANCE;
    }
    return respVal;
}
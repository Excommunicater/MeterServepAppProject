//--Project includes--------------------------------------------------
#include "metering_interface.h"
#include "meterUtils.h"
#include "errorHandling.h"
//--------------------------------------------------------------------

//--External includes-------------------------------------------------
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <fcntl.h>
#include <poll.h>
#include <stdbool.h>
//--------------------------------------------------------------------

#ifdef METER_UTILS_STORE_AND_RESTORE_DATA
#include "fileUtils.h"
static meter_hw_registers_t storedLastPowerCycleHardwareRegister = {0};
#endif

#ifdef MU_MU_DBG_PRNT
#include <stdio.h>
#endif

//--Global Variables--------------------------------------------------
meter_hw_registers_t lastReadHardwareRegister = {0};
//--------------------------------------------------------------------

void InitMeter( void )
{
    mkfifo(DEV_FILE, 0666);

#ifdef METER_UTILS_STORE_AND_RESTORE_DATA
    // Restore stored values saved before power cycle  
    RestoreNonVolatileData( &storedLastPowerCycleHardwareRegister, sizeof(meter_hw_registers_t) );
#endif
}

void ReadStructFromDev( void )
{
#if METER_UTILS_TYPE_OF_DATA_READOUT == BLOCKING_METER_READOUT
    int fifoFile = open(DEV_FILE, O_RDONLY);
    read(fifoFile, &lastReadHardwareRegister, sizeof(meter_hw_registers_t));
#elif METER_UTILS_TYPE_OF_DATA_READOUT == NON_BLOCKING_METER_READOUT
    struct pollfd fdarray [1];
    bool isReadNewData = false;
    int fifoFile = open(DEV_FILE, O_RDONLY|O_NONBLOCK );

    fdarray[0].fd = fifoFile;
    fdarray[0].events = POLLIN | POLLRDNORM;
    int rc = poll(fdarray, 1, 1);
    if ( ( rc == 1 ) && ( fdarray[0].revents & POLLRDNORM ) )
    {
        read(fifoFile, &lastReadHardwareRegister, sizeof(meter_hw_registers_t));
        isReadNewData = true;
    }
    
    close(fifoFile);
#elif METER_UTILS_TYPE_OF_DATA_READOUT == NO_METER_HARDCODED_VALUES
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

#if METER_UTILS_TYPE_OF_DATA_READOUT == NON_BLOCKING_METER_READOUT
    if ( isReadNewData )
    {
#endif
#ifdef METER_UTILS_STORE_AND_RESTORE_DATA
    // Add to last read values data from previous powercycle
    lastReadHardwareRegister.per_phase[0].ai += storedLastPowerCycleHardwareRegister.per_phase[0].ai; 
    lastReadHardwareRegister.per_phase[1].ai += storedLastPowerCycleHardwareRegister.per_phase[1].ai; 
    lastReadHardwareRegister.per_phase[2].ai += storedLastPowerCycleHardwareRegister.per_phase[2].ai;
    lastReadHardwareRegister.per_phase[0].ae += storedLastPowerCycleHardwareRegister.per_phase[0].ae;
    lastReadHardwareRegister.per_phase[1].ae += storedLastPowerCycleHardwareRegister.per_phase[1].ae;
    lastReadHardwareRegister.per_phase[1].ae += storedLastPowerCycleHardwareRegister.per_phase[1].ae;
    StoreNonVolatileData( &lastReadHardwareRegister, sizeof(meter_hw_registers_t) );
#endif

#if METER_UTILS_TYPE_OF_DATA_READOUT == BLOCKING_METER_READOUT
    close(fifoFile);
#elif METER_UTILS_TYPE_OF_DATA_READOUT == NON_BLOCKING_METER_READOUT
    } 
#endif
#ifdef MU_MU_DBG_PRNT
    printf("ReadStructFromDev - read data from meter! ");
    printf("RR.per_phase[0].v=%i\r\n",lastReadHardwareRegister.per_phase[0].v);
#endif
}
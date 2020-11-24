#include "metering_interface.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>


/* NOTES:
 * compile with: gcc metering_hw.c -o metering_hw
 *
 * this "emulator" is badly written on purpose, feel free to fix it
 * Hope I fixed it XD
 */


int main() {

    struct timespec tnow, tstart;
    meter_hw_registers_t hwregs;

    clock_gettime(CLOCK_MONOTONIC, &tstart);
    mkfifo(DEV_FILE, 0666);
    

    while (1)
    {
        int fd = open(DEV_FILE, O_WRONLY);
        clock_gettime(CLOCK_MONOTONIC, &tnow);
        const int32_t secs_since_start = tnow.tv_sec - tstart.tv_sec;

        for (int phase = 0; phase < PHASE_CNT; ++phase) {
            hwregs.per_phase[phase].v = (220 + phase * 10 + secs_since_start % 5) * 1000;
            hwregs.per_phase[phase].i = phase * 1000 + (secs_since_start % 10) * 100;
            hwregs.per_phase[phase].ai = secs_since_start * 100 + phase * 10;
            hwregs.per_phase[phase].ae = secs_since_start * 10 + phase * 1;
        }
        hwregs.voltage_angles[0] = 120;
        hwregs.voltage_angles[1] = 120 - secs_since_start % 30;
        hwregs.voltage_angles[2] = 240 - hwregs.voltage_angles[1];

        hwregs.current_angles[1] = 120;
        hwregs.current_angles[2] = 120 - secs_since_start % 30;
        hwregs.current_angles[0] = 240 - hwregs.current_angles[2];

        write(fd, &hwregs, sizeof(hwregs));
        close(fd);

        usleep(200 * 1000); // new data approximately 5 times a second
    }

    
    return 0;
}


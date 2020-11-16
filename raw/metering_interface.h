#ifndef METERING_INTERFACE_H
#define METERING_INTERFACE_H

#include <stdint.h>

#define DEV_FILE "/tmp/meterhw"

struct meter_hw_registers_per_phase_s {
    uint32_t v;     /* instatntenous voltage in mV */
    uint32_t i;     /* instatntenous current in mA */
    uint64_t ai;    /* A+ since start in Ws */
    uint64_t ae;    /* A- since start in Ws */
};

#define PHASE_CNT 3

struct meter_hw_registers_s {
    struct meter_hw_registers_per_phase_s per_phase[PHASE_CNT];
    uint32_t voltage_angles[3]; /* [0] -> L1-L2, [1] -> L2-L3, [3] -> L3-L1 */
    uint32_t current_angles[3]; /* [0] -> L1-L2, [1] -> L2-L3, [3] -> L3-L1 */
};

typedef struct meter_hw_registers_s meter_hw_registers_t;

#endif

#pragma once
#include "sim.hpp"
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#define OUTPUT 1
#define HIGH 1
#define LOW 0

#ifdef __cplusplus
extern "C" {
#endif

// Mapping based on SIDE_RIGHT physical alignment found in sim.hpp
// Hardware LATCH (15) -> SORTIE_1 (15)
// Hardware OE (14)    -> SORTIE_3 (14)
// Remaining: CLOCK -> SORTIE_2 (16), DATA -> SORTIE_4 (17)

inline int wiringPiSetupGpio() {
    sim_init();
    return 0;
}

inline void pinMode(int pin, int mode) {
    (void)pin;
    (void)mode;
}

inline void delayMicroseconds(int us) {
    (void)us;
}

inline void delay(int ms) {
    usleep(ms * 1000);
}

inline void digitalWrite(int pin, int val) {
    if (pin == 25 || pin == 15) {        // LATCH
        sim(SORTIE_1, val);
    } else if (pin == 8 || pin == 18) {  // CLOCK
        sim(SORTIE_2, val);
    } else if (pin == 24 || pin == 14) { // OE
        sim(SORTIE_3, val);
    } else if (pin == 7 || pin == 23) {  // DATA
        sim(SORTIE_4, val);
    }
}

#ifdef __cplusplus
}
#endif

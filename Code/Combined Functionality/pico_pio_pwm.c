/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
// #include "pwm.pio.h"
#include "D:\Course Stuff\Fall'22\ESE 519\lab\SDK\pico-sdk\src\boards\include\boards\adafruit_qtpy_rp2040.h"

//hackster.io code
#include "hardware/clocks.h"
#include "hardware/pio.h"
#include "pico/stdlib.h"
#include "pico_pio_pwm.pio.h"

#define MIN_DC 650
#define MAX_DC 2250
const uint SERVO_PIN = 29;

/* Write `period` to the input shift register */
void pio_pwm_set_period(PIO pio, uint sm, uint32_t period) {
    pio_sm_set_enabled(pio, sm, false);
    pio_sm_put_blocking(pio, sm, period);
    pio_sm_exec(pio, sm, pio_encode_pull(false, false));
    pio_sm_exec(pio, sm, pio_encode_out(pio_isr, 32));
    pio_sm_set_enabled(pio, sm, true);
}

/* Write `level` to TX FIFO. State machine will copy this into X */
void pio_pwm_set_level(PIO pio, uint sm, uint32_t level) {
    pio_sm_put_blocking(pio, sm, level);
}

int main() {
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &pico_servo_pio_program);

    float freq = 50.0f; /* servo except 50Hz */
    uint clk_div = 64;  /* make the clock slower */

    pico_servo_pio_program_init(pio, sm, offset, clk_div, SERVO_PIN);

    uint cycles = clock_get_hz(clk_sys) / (freq * clk_div);
    uint32_t period = (cycles -3) / 3;  
    pio_pwm_set_period(pio, sm, period);

    uint level;
    // int ms = (MAX_DC - MIN_DC) / 2;
    int ms = 0;
    bool clockwise = false;

    // while(!stdio_usb_connected());

    // printf("Starting servo");
    while (true) {
        level = (ms / 20000.f) * period;
        pio_pwm_set_level(pio, sm, level);

        clockwise = !clockwise;

        if (clockwise) {
            ms = 600;
        } else {
            ms = 0;
        }

        sleep_ms(500);
    }
    }

// #include "hardware/clocks.h"
// #include "hardware/pio.h"
// #include "pico/stdlib.h"
// #include "pico_servo.pio.h"

// #define MIN_DC 650
// #define MAX_DC 2250
// const uint SERVO_PIN = 16;

// /* Write `period` to the input shift register */
// void pio_pwm_set_period(PIO pio, uint sm, uint32_t period) {
//     pio_sm_set_enabled(pio, sm, false);
//     pio_sm_put_blocking(pio, sm, period);
//     pio_sm_exec(pio, sm, pio_encode_pull(false, false));
//     pio_sm_exec(pio, sm, pio_encode_out(pio_isr, 32));
//     pio_sm_set_enabled(pio, sm, true);
// }

// /* Write `level` to TX FIFO. State machine will copy this into X */
// void pio_pwm_set_level(PIO pio, uint sm, uint32_t level) {
//     pio_sm_put_blocking(pio, sm, level);
// }

// int main() {
//     PIO pio = pio0;
//     int sm = 0;
//     uint offset = pio_add_program(pio, &pico_servo_pio_program);

//     float freq = 50.0f; /* servo except 50Hz */
//     uint clk_div = 64;  /* make the clock slower */

//     pico_servo_pio_program_init(pio, sm, offset, clk_div, SERVO_PIN);

//     uint cycles = clock_get_hz(clk_sys) / (freq * clk_div);
//     uint32_t period = (cycles -3) / 3;  
//     pio_pwm_set_period(pio, sm, period);

//     uint level;
//     int ms = (MAX_DC - MIN_DC) / 2;
//     bool clockwise = false;

//     while(!stdio_usb_connected());
//     printf("Starting\n");

//         while(true) {
//         level = (ms / 20000.f) * period;
//         pio_pwm_set_level(pio, sm, level);

//         if (ms <= MIN_DC || ms >= MAX_DC) {
//             clockwise = !clockwise;
//         }

//         if (clockwise) {
//             ms -= 100;
//         } else {
//             ms += 100;
//         }

//         sleep_ms(500);
//   }
// }


// // Write `period` to the input shift register
// void pio_pwm_set_period(PIO pio, uint sm, uint32_t period) {
//     pio_sm_set_enabled(pio, sm, false);
//     pio_sm_put_blocking(pio, sm, period);
//     pio_sm_exec(pio, sm, pio_encode_pull(false, false));
//     pio_sm_exec(pio, sm, pio_encode_out(pio_isr, 32));
//     pio_sm_set_enabled(pio, sm, true);
// }

// // Write `level` to TX FIFO. State machine will copy this into X.
// void pio_pwm_set_level(PIO pio, uint sm, uint32_t level) {
//     pio_sm_put_blocking(pio, sm, level);
// }

// int main() {
//     stdio_init_all();
// // #ifndef PICO_DEFAULT_LED_PIN
// // #warning pio/pwm example requires a board with a regular LED
// //     puts("Default LED pin was not defined");
// // #else

//     // todo get free sm
//     PIO pio = pio0;
//     int sm = 0;
//     uint offset = pio_add_program(pio, &pwm_program);
//     printf("Loaded program at %d\n", offset);

//     pwm_program_init(pio, sm, offset, PICO_PWM_PIN);
//     // pio_pwm_set_period(pio, sm, (1u << 16) - 1);
//     pio_pwm_set_period(pio, sm, (1u << 16) - 1);



//     int level=0;
//     while(!stdio_usb_connected());
//     while (true) {
//         printf("Level = %d\n", level);
//         pio_pwm_set_level(pio, sm, level * level);
//         level = 100;
//         sleep_ms(100);
//     }

// }

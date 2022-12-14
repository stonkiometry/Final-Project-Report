#include <stdio.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "pio_i2c.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"

#include "registers.h"
#include "adps_registers.h"
#include "D:\Course Stuff\Fall'22\ESE 519\lab\SDK\pico-sdk\src\boards\include\boards\adafruit_qtpy_rp2040.h"
#include "pico_pio_pwm.pio.h"

#define PIN_SDA 22
#define PIN_SCL 23

#define IS_RGBW true
#define NUM_PIXELS 150

//Defined for project
uint16_t r_value;
uint16_t b_value;
uint16_t g_value;

const uint SERVO_PIN = 29;

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

void servo_trigger(PIO pio, int sm, uint offset, float freq, uint clk_div){
    uint cycles = clock_get_hz(clk_sys) / (freq * clk_div);
    uint32_t period = (cycles -3) / 3;  
    pio_pwm_set_period(pio, sm, period);

    uint level;
    int ms =1500;
    level = (ms / 20000.f) * period;
    pio_pwm_set_level(pio, sm, level);
    sleep_ms(500);
    ms=0;
    level = (ms / 20000.f) * period;
    pio_pwm_set_level(pio, sm, level);
}

void servo_trigger_set(PIO pio, int sm, uint offset, float freq, uint clk_div){
    uint cycles = clock_get_hz(clk_sys) / (freq * clk_div);
    uint32_t period = (cycles -3) / 3;  
    pio_pwm_set_period(pio, sm, period);

    uint level;
    int ms =0;
    level = (ms / 20000.f) * period;
    pio_pwm_set_level(pio, sm, level);
}


void config_adps(PIO pio, uint sm){
    // to power on APDS Sensor
    // The register address for the slave needs to be prepended to the data.
    uint8_t txbuf[2] = {0};

    // Setting Color Integration time to '1' i.e. 256 = 0xFF
    txbuf[0] = ATIME_REGISTER;
    txbuf[1] = (uint8_t)(0xFF);
    pio_i2c_write_blocking(pio, sm, ADPS_ADDRESS, txbuf, 2);

    // Config the Cotrol Register.
    txbuf[0] = ADPS_CONTROL_ONE_REGISTER;
    txbuf[1] = ADPS_CONTROL_ONE_AGAIN;
    pio_i2c_write_blocking(pio, sm, ADPS_ADDRESS, txbuf, 2);

    // Enable Ambient Light and Proximity Sensor
    txbuf[0] = ADPS_ENABLE_REGISTER;
    txbuf[1] = ADPS_ENABLE_PON | ADPS_ENABLE_AEN | ADPS_ENABLE_PEN;
    pio_i2c_write_blocking(pio, sm, ADPS_ADDRESS, txbuf, 2);
}

void adps_read(PIO pio, uint sm, uint8_t reg_addr, uint8_t *rxbuf, uint num_bytes) {
    // Read from `reg_addr`.
    pio_i2c_write_blocking(pio, sm, ADPS_ADDRESS, &reg_addr, 1);  
    pio_i2c_read_blocking(pio, sm, ADPS_ADDRESS, rxbuf, num_bytes);
}

void read_prox_and_color(PIO pio, uint sm)
{
    // Check the status register, to know if we can read the values
    // from the ALS and Proximity engine.
    uint8_t rxbuf[1] = {0};
    adps_read(pio, sm, STATUS_REGISTER, rxbuf, 1);
    adps_read(pio, sm, ID_REGISTER, rxbuf, 1);

    // Initialize Color Data
    uint16_t c_val = 0;
    uint16_t r_val = 0;
    uint16_t g_val = 0;
    uint16_t b_val =0;
    uint32_t color_data=0;

    // Use the mask to check if our Proximity and color data is ready to be read.
    uint8_t data_arr[8] = {0};
    if ((rxbuf[0] & STATUS_REGISTER_PVALID) == STATUS_REGISTER_PVALID) {
        adps_read(pio, sm, PROXIMITY_DATA_REGISTER, data_arr, 1);
        // printf("The Proximity Data : %d\n", data_arr[0] - 230);
    } 
    if ((rxbuf[0] & STATUS_REGISTER_AVALID) == STATUS_REGISTER_AVALID) {
        adps_read(pio, sm, RGBC_DATA_REGISTER_CDATAL, data_arr, 16);
        c_val = (data_arr[1] << 8 | data_arr[0]); 
        r_val = (data_arr[3] << 8 | data_arr[2]); 
        g_val = (data_arr[5] << 8 | data_arr[4]); 
        b_val = (data_arr[7] << 8 | data_arr[6]); 
        printf("(%d, %d, %d, %d)\n", r_val, g_val, b_val, c_val);
        r_value = r_val;
        g_value = g_val;
        b_value = b_val;
    }
    // uint32_t final_color_packet = 0;
    // if ((r_val > g_val) && (r_val > b_val)) {
    //     final_color_packet = ((uint8_t)((r_val*255) / 65536) << 16);
    // } else if ((g_val > r_val) && (g_val > b_val)) {
    //     final_color_packet = ((uint8_t)((g_val*255) / 65536) << 8);
    // } else if ((b_val > r_val) && (b_val > g_val)) 
    // {
    //     final_color_packet = (uint8_t)((b_val*255) / 65536);
    // }
}

int main() {
    // const uint POWER_PIN_NAME = PICO_DEFAULT_WS2812_POWER_PIN;
    // gpio_init(POWER_PIN_NAME);
    // gpio_set_dir(POWER_PIN_NAME, GPIO_OUT);
    // gpio_put(POWER_PIN_NAME, 1);
    stdio_init_all();

    PIO pio_0 = pio0;
    int sm_0 = 0;
    uint offset_0 = pio_add_program(pio_0, &pico_servo_pio_program);

    float freq = 50.0f; /* servo except 50Hz */
    uint clk_div = 64;  /* make the clock slower */
    pico_servo_pio_program_init(pio_0, sm_0, offset_0, clk_div, SERVO_PIN); 
    

    PIO pio_1 = pio1;//changed from pio0 to pio1
    uint sm_1 = 1;
    uint offset_1 = pio_add_program(pio_1, &i2c_program);
    i2c_program_init(pio_1, sm_1, offset_1, PIN_SDA, PIN_SCL);
    
    // Wait until USB is connected.
    while(!stdio_usb_connected());

    printf("Initiatializing\n");

    // Configure the ADPS Sensor.
    config_adps(pio_1, sm_1);
    
    //Creating Color 1,2 and 3
    uint16_t r_1=0;
    uint16_t r_2=0;
    uint16_t r_3=0;
    uint16_t r_4=0;
    //uint16_t r_5=0;


    uint16_t g_1=0;
    uint16_t g_2=0;
    uint16_t g_3=0;
    uint16_t g_4=0;
    //uint16_t g_5=0;


    uint16_t b_1=0;
    uint16_t b_2=0;
    uint16_t b_3=0;
    uint16_t b_4=0;
    //uint16_t b_5=0;

    uint16_t r_diff,g_diff,b_diff;
    uint16_t reaction_count=0;
    uint16_t epsilon=512;
    uint8_t color_change_count=0;

    uint8_t conf=0;
    uint8_t conf_prev=0;
    uint8_t conf_limit = 2;
    uint8_t r_weight=1;
    uint8_t g_weight=1;
    uint8_t b_weight=1;
    bool reaction=true;

    printf("Initilaizing motor\n");
    servo_trigger_set(pio_0,sm_0,offset_0,freq,clk_div);
    printf("Motor Initialized, stabilizing APDS\n");
    sleep_ms(10000);
    printf("stabilization over\n");
    while(1){
        printf("Reaction count:%d\n", reaction_count);
        if (reaction){
            servo_trigger(pio_0,sm_0,offset_0,freq,clk_div);
            sleep_ms(3500);
        }
        read_prox_and_color(pio_1, sm_1);

        //r_5=r_4;
        r_4=r_3;
        r_3=r_2;
        r_2=r_1;
        r_1 = r_value;

        //g_5=g_4;
        g_4=g_3;
        g_3=g_2;
        g_2=g_1;
        g_1 = g_value;

        //b_5=b_4;
        b_4=b_3;
        b_3=b_2;
        b_2=b_1;
        b_1 = b_value;
        printf("r_1:%d\t",r_1);
        printf("r_2:%d\t",r_2);
        printf("r_3:%d\n",r_3);
        printf("r_4%d\n",r_4);
        //printf("r_5:%d\n",r_5);
        // If reaction end
        if(reaction_count>=3){
            //comparison logic
            printf("Checking logic started\n");
            r_diff = abs(r_1-r_4);
            printf("r_diff:%d\t",r_diff);
            if(r_diff>epsilon){
                conf=conf+r_weight;
            }
            g_diff = abs(g_1-g_4);
            printf("g_diff:%d\t",g_diff);
            if(g_diff>epsilon){
                conf=conf+g_weight;
            }
            b_diff = abs(b_1-b_4);
            printf("b_diff:%d\n",b_diff);
            if(b_diff>epsilon){
                conf=conf+b_weight;
            }
            printf("Conf:%d\t",conf);
            printf("Conf_prev:%d\n",conf_prev);
            if(conf>conf_limit){
                reaction=false;
                color_change_count+=1;
                printf("Color change count: %d\n",color_change_count);
                if(color_change_count>=3){
                    printf("Reaction Over. \n");
                    break;
                }
                sleep_ms(1000);
            }
            if(conf<conf_prev){
                reaction=true;
                color_change_count=0;

            }
            conf_prev=conf;
            conf = 0;
        }
        reaction_count+=1;
    }
    return 0;
}

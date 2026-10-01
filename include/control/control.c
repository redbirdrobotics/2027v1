#include "control.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include <stdio.h>

// initialize the pins and set their output directions
uint8_t init_driver(driver_config_t * driver_config,
    uint8_t reverse0, uint8_t reverse1) {
    driver_config->motor[0].dir = reverse0;
    driver_config->motor[1].dir = reverse1;
    
    // initialize pins
    init_motor(&driver_config->motor[0]);
    init_motor(&driver_config->motor[1]);
    
    return 0; // nothing went wrong
}

// helper pin initializer class
static uint8_t init_motor(motor_config_t * motor_config) {
    gpio_init(motor_config->in1);
    gpio_init(motor_config->in2);
    gpio_init(motor_config->en);

    gpio_set_dir(motor_config->in1, GPIO_OUT);
    gpio_set_dir(motor_config->in2, GPIO_OUT);
    gpio_set_dir(motor_config->en, GPIO_OUT);
    
    gpio_set_function(motor_config->en, GPIO_FUNC_PWM);
    return 0;
}

// motor direction is controlled by either 
uint8_t set_motor_direction(driver_config_t * driver, uint8_t motor, uint8_t direction) {
    // motor will be 1 or 0
    // the directions for the line 1 and 2
    // motor direction is set to be inX * direction * direction offset
    uint8_t mask1, mask2;
    if (direction < 0) {
        mask1 = 0 ^ driver->motor[motor].dir;
        mask2 = 1 ^ driver->motor[motor].dir;
    } else if (direction > 0) {
        mask1 = 1 ^ driver->motor[motor].dir;
        mask2 = 0 ^ driver->motor[motor].dir;
    } else {
        mask1 = 0;
        mask2 = 0;
    }
    
    driver->motor[motor].in1 = mask1;
    driver->motor[motor].in2 = mask2;
    
    return 0;
}

uint8_t set_motor_speed(driver_config_t * driver, uint8_t motor, uint16_t speed) {
    if (speed < 0) {
        printf("Error: %n is not a valid speed, it must be 65535 > x > 0.");
        return -1;
    }
    pwm_set_gpio_level(driver->motor[motor].en, speed);

    return 0;
}

// kill the motors immediately, don't use as controlled stop
uint8_t kill_motor(driver_config_t * driver) {
    gpio_put(driver->motor[0].in1, false);
    gpio_put(driver->motor[0].in2, false);
    gpio_put(driver->motor[1].in1, false);
    gpio_put(driver->motor[1].in2, false);

    return 0;
}
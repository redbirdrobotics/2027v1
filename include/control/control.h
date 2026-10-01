#ifndef CONTROL_H
#define CONTROL_H

#include "hardware/gpio.h"
#include "hardware/pwm.h"

// motor driver control implementation
// each driver only handles 2 motors so each motor config will only handle 2

// data struct for maintaining the state of the motor

typedef struct {
    uint8_t en, in1, in2, dir;
} motor_config_t;

typedef struct driver_config {
    char id; // identifying character
    motor_config_t motor[2];
} driver_config_t;

uint8_t init_driver(driver_config_t * driver_config, uint8_t reverse0, uint8_t reverse1);
static uint8_t init_motor(motor_config_t * motor_config);
uint8_t set_motor_speed(driver_config_t * driver, uint8_t motor, uint16_t speed);
uint8_t set_motor_direction(driver_config_t * driver, uint8_t motor, uint8_t direction);
uint8_t kill_motor(driver_config_t * driver);

#endif
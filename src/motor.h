#ifndef MOTOR_H
#define MOTOR_H

#include <stdint.h>

typedef enum {
    MOTOR_DIR_FORWARD,
    MOTOR_DIR_BACKWARD,
    MOTOR_DIR_STOP
} motor_dir_t;

typedef struct {
    uint32_t pwm_channel;
    uint32_t max_duty;
    motor_dir_t direction;
} motor_t;

void motor_init(motor_t *motor, uint32_t pwm_channel, uint32_t max_duty);
void motor_set_speed(motor_t *motor, float speed_pct);
void motor_set_direction(motor_t *motor, motor_dir_t dir);
void motor_stop(motor_t *motor);

#endif /* MOTOR_H */ 

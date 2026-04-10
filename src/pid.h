#ifndef PID_H
#define PID_H

#include <stdint.h>

typedef struct {
    float kp;
    float ki;
    float kd;
    float setpoint;
    float integral;
    float prev_error;
    float output_min;
    float output_max;
    float dt;
} pid_controller_t;

void  pid_init(pid_controller_t *pid, float kp, float ki, float kd,
               float dt, float out_min, float out_max);
float pid_compute(pid_controller_t *pid, float measured);
void  pid_reset(pid_controller_t *pid);
void  pid_set_gains(pid_controller_t *pid, float kp, float ki, float kd);
void  pid_set_setpoint(pid_controller_t *pid, float setpoint);

#endif /* PID_H */ 

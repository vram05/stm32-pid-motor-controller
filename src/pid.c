 
#include "pid.h"

void pid_init(pid_controller_t *pid, float kp, float ki, float kd,
              float dt, float out_min, float out_max)
{
    pid->kp         = kp;
    pid->ki         = ki;
    pid->kd         = kd;
    pid->dt         = dt;
    pid->output_min = out_min;
    pid->output_max = out_max;
    pid->integral   = 0.0f;
    pid->prev_error = 0.0f;
    pid->setpoint   = 0.0f;
}

float pid_compute(pid_controller_t *pid, float measured)
{
    float error      = pid->setpoint - measured;
    float derivative = (error - pid->prev_error) / pid->dt;

    pid->integral   += error * pid->dt;

    /* Anti-windup: clamp integral */
    if (pid->integral > pid->output_max) pid->integral = pid->output_max;
    if (pid->integral < pid->output_min) pid->integral = pid->output_min;

    float output = (pid->kp * error) +
                   (pid->ki * pid->integral) +
                   (pid->kd * derivative);

    /* Clamp output */
    if (output > pid->output_max) output = pid->output_max;
    if (output < pid->output_min) output = pid->output_min;

    pid->prev_error = error;
    return output;
}

void pid_reset(pid_controller_t *pid)
{
    pid->integral   = 0.0f;
    pid->prev_error = 0.0f;
}

void pid_set_gains(pid_controller_t *pid, float kp, float ki, float kd)
{
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
}

void pid_set_setpoint(pid_controller_t *pid, float setpoint)
{
    pid->setpoint = setpoint;
}
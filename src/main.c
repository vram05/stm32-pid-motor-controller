#include <stdint.h>
#include "pid.h"
#include "motor.h"
#include "encoder.h"
#include "uart_debug.h"

/* Control loop timing */
#define CONTROL_LOOP_MS   10
#define TARGET_POSITION   180.0f   /* degrees */

/* Simple millisecond delay using SysTick */
#define SYSTICK_BASE      0xE000E010UL
#define SYSTICK_CTRL      (*(volatile uint32_t *)(SYSTICK_BASE + 0x00))
#define SYSTICK_LOAD      (*(volatile uint32_t *)(SYSTICK_BASE + 0x04))
#define SYSTICK_VAL       (*(volatile uint32_t *)(SYSTICK_BASE + 0x08))
#define CPU_CLOCK_HZ      168000000UL

static void delay_ms(uint32_t ms)
{
    SYSTICK_LOAD = (CPU_CLOCK_HZ / 1000U) - 1U;
    SYSTICK_VAL  = 0;
    SYSTICK_CTRL = 0x05;
    for (uint32_t i = 0; i < ms; i++) {
        while (!(SYSTICK_CTRL & (1U << 16)));
    }
    SYSTICK_CTRL = 0;
}

int main(void)
{
    pid_controller_t pid;
    motor_t          motor;
    encoder_t        encoder;

    /* Initialise peripherals */
    uart_debug_init(115200);
    uart_debug_send("=== STM32 PID Motor Controller ===\r\n");

    encoder_init(&encoder, 500);   /* 500 PPR encoder */
    motor_init(&motor, 1, 1000);   /* TIM2 CH1, ARR=1000 */

    /* PID gains — tune via UART in production */
    pid_init(&pid,
             2.0f,    /* Kp */
             0.5f,    /* Ki */
             0.1f,    /* Kd */
             (float)CONTROL_LOOP_MS / 1000.0f,
             -100.0f, /* output min */
             100.0f); /* output max */

    pid_set_setpoint(&pid, TARGET_POSITION);
    uart_debug_send("Target: 180 degrees\r\n");

    motor_set_direction(&motor, MOTOR_DIR_FORWARD);

    while (1) {
        /* Read encoder */
        encoder_update(&encoder);
        float position = encoder_get_position(&encoder);
        float velocity = encoder_get_velocity(&encoder);

        /* Compute PID output */
        float output = pid_compute(&pid, position);

        /* Apply output to motor */
        if (output >= 0.0f) {
            motor_set_direction(&motor, MOTOR_DIR_FORWARD);
            motor_set_speed(&motor, output);
        } else {
            motor_set_direction(&motor, MOTOR_DIR_BACKWARD);
            motor_set_speed(&motor, -output);
        }

        /* Debug output over UART */
        uart_ 

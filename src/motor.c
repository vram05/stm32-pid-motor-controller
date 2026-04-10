#include "motor.h"
#include <stddef.h>

/* STM32 TIM2 PWM registers — direct register access, no HAL */
#define TIM2_BASE     0x40000000UL
#define TIM2_CR1      (*(volatile uint32_t *)(TIM2_BASE + 0x00))
#define TIM2_ARR      (*(volatile uint32_t *)(TIM2_BASE + 0x2C))
#define TIM2_CCR1     (*(volatile uint32_t *)(TIM2_BASE + 0x34))
#define TIM2_CCMR1    (*(volatile uint32_t *)(TIM2_BASE + 0x18))
#define TIM2_CCER     (*(volatile uint32_t *)(TIM2_BASE + 0x20))

/* GPIO for direction control — GPIOA */
#define GPIOA_BASE    0x40020000UL
#define GPIOA_MODER   (*(volatile uint32_t *)(GPIOA_BASE + 0x00))
#define GPIOA_ODR     (*(volatile uint32_t *)(GPIOA_BASE + 0x14))

#define PWM_PERIOD    1000U   /* ARR value = 1000 ticks */

void motor_init(motor_t *motor, uint32_t pwm_channel, uint32_t max_duty)
{
    if (!motor) return;

    motor->pwm_channel = pwm_channel;
    motor->max_duty    = max_duty;
    motor->direction   = MOTOR_DIR_STOP;

    /* Configure TIM2 CH1 as PWM mode 1 */
    TIM2_ARR   = PWM_PERIOD;
    TIM2_CCMR1 = (6U << 4);   /* OC1M = PWM mode 1 */
    TIM2_CCER  = (1U << 0);   /* CC1E enable */
    TIM2_CCR1  = 0;
    TIM2_CR1   = (1U << 0);   /* CEN: counter enable */

    /* GPIOA pin 0/1 as output for direction */
    GPIOA_MODER |= (1U << 0) | (1U << 2);
}

void motor_set_speed(motor_t *motor, float speed_pct)
{
    if (!motor) return;

    if (speed_pct < 0.0f) speed_pct = 0.0f;
    if (speed_pct > 100.0f) speed_pct = 100.0f;

    uint32_t duty = (uint32_t)((speed_pct / 100.0f) * motor->max_duty);
    TIM2_CCR1 = duty;
}

void motor_set_direction(motor_t *motor, motor_dir_t dir)
{
    if (!motor) return;
    motor->direction = dir;

    switch (dir) {
        case MOTOR_DIR_FORWARD:
            GPIOA_ODR |=  (1U << 0);
            GPIOA_ODR &= ~(1U << 1);
            break;
        case MOTOR_DIR_BACKWARD:
            GPIOA_ODR &= ~(1U << 0);
            GPIOA_ODR |=  (1U << 1);
            break;
        case MOTOR_DIR_STOP:
        default:
            GPIOA_ODR &= ~((1U << 0) | (1U << 1));
            break;
    }
}

void motor_stop(motor_t *motor)
{
    motor_set_speed(motor, 0.0f);
    motor_set_direction(motor, MOTOR_DIR_STOP);
} 

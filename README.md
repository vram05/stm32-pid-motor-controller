\# STM32 PID Motor Controller



PID-based DC motor position controller on STM32F4.

Direct register access — no HAL, no CubeMX.

Encoder feedback via TIM3 quadrature mode, PWM output via TIM2.



\## Features

\- PID controller with anti-windup clamping

\- Quadrature encoder interface via TIM3 (direct registers)

\- PWM motor drive via TIM2 CH1 (direct registers)

\- UART debug interface at 115200 baud (position, velocity, PID output)

\- 10ms control loop using SysTick

\- Clean modular structure: pid, motor, encoder, uart\_debug



\## Hardware Target

\- STM32F4 Discovery (STM32F407VG)

\- DC motor with 500 PPR quadrature encoder

\- H-bridge driver (e.g. L298N)



\## Project Structure



| File | Purpose |

|------|---------|

| src/main.c | App entry, control loop |

| src/pid.c/h | PID controller with anti-windup |

| src/motor.c/h | PWM motor driver (TIM2, direct registers) |

| src/encoder.c/h | Quadrature encoder (TIM3, direct registers) |

| src/uart\_debug.c/h | UART debug output (USART2, direct registers) |

| CMakeLists.txt | Build configuration |



\## PID Tuning



Default gains:

\- Kp = 2.0

\- Ki = 0.5

\- Kd = 0.1



Tune via uart\_debug\_send commands over USART2 at 115200 baud.



\## Control Loop



Setpoint (degrees) → PID → PWM duty cycle → Motor → Encoder → feedback



Loop runs every 10ms via SysTick.



\## Author

Venkata Ram

ramnelluri05@gmail.com


cmake_minimum_required(VERSION 3.20.0)
project(pid_motor_controller C)

set(CMAKE_C_STANDARD 11)

# STM32F4 target flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard")

add_executable(pid_motor_controller
    src/main.c
    src/pid.c
    src/motor.c
    src/encoder.c
    src/uart_debug.c
)

target_include_directories(pid_motor_controller PRIVATE src) 

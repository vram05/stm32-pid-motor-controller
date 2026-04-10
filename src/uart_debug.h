#ifndef UART_DEBUG_H
#define UART_DEBUG_H

#include <stdint.h>

void uart_debug_init(uint32_t baud_rate);
void uart_debug_send(const char *msg);
void uart_debug_send_float(const char *label, float value);
void uart_debug_send_int(const char *label, int32_t value);

#endif /* UART_DEBUG_H */ 

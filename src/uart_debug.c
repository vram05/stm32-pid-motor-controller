#include "uart_debug.h"
#include <stdio.h>
#include <string.h>

/* STM32 USART2 registers — direct register access */
#define USART2_BASE   0x40004400UL
#define USART2_SR     (*(volatile uint32_t *)(USART2_BASE + 0x00))
#define USART2_DR     (*(volatile uint32_t *)(USART2_BASE + 0x04))
#define USART2_BRR    (*(volatile uint32_t *)(USART2_BASE + 0x08))
#define USART2_CR1    (*(volatile uint32_t *)(USART2_BASE + 0x0C))

#define USART_SR_TXE  (1U << 7)
#define USART_CR1_TE  (1U << 3)
#define USART_CR1_UE  (1U << 13)

#define APB1_CLOCK_HZ 42000000UL

void uart_debug_init(uint32_t baud_rate)
{
    /* Set baud rate */
    USART2_BRR = APB1_CLOCK_HZ / baud_rate;

    /* Enable USART2, transmitter */
    USART2_CR1 = USART_CR1_UE | USART_CR1_TE;
}

static void uart_send_char(char c)
{
    while (!(USART2_SR & USART_SR_TXE));
    USART2_DR = (uint32_t)c;
}

void uart_debug_send(const char *msg)
{
    if (!msg) return;
    while (*msg) {
        uart_send_char(*msg++);
    }
}

void uart_debug_send_float(const char *label, float value)
{
    char buf[64];
    snprintf(buf, sizeof(buf), "%s: %.3f\r\n", label, (double)value);
    uart_debug_send(buf);
}

void uart_debug_send_int(const char *label, int32_t value)
{
    char buf[64];
    snprintf(buf, sizeof(buf), "%s: %ld\r\n", label, (long)value);
    uart_debug_send(buf);
} 

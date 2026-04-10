#include "encoder.h"

/* STM32 TIM3 in encoder mode — direct register access */
#define TIM3_BASE   0x40000400UL
#define TIM3_CR1    (*(volatile uint32_t *)(TIM3_BASE + 0x00))
#define TIM3_SMCR   (*(volatile uint32_t *)(TIM3_BASE + 0x08))
#define TIM3_CCMR1  (*(volatile uint32_t *)(TIM3_BASE + 0x18))
#define TIM3_CCER   (*(volatile uint32_t *)(TIM3_BASE + 0x20))
#define TIM3_CNT    (*(volatile uint32_t *)(TIM3_BASE + 0x24))
#define TIM3_ARR    (*(volatile uint32_t *)(TIM3_BASE + 0x2C))

#define COUNTS_PER_REV_FACTOR  4U   /* quadrature = 4x PPR */

void encoder_init(encoder_t *enc, uint32_t ppr)
{
    if (!enc) return;

    enc->count        = 0;
    enc->prev_count   = 0;
    enc->position_deg = 0.0f;
    enc->velocity_rpm = 0.0f;
    enc->ppr          = ppr;

    /* Configure TIM3 as quadrature encoder interface */
    TIM3_SMCR  = 0x0003;          /* SMS = 011: encoder mode 3 */
    TIM3_CCMR1 = 0x0101;          /* CC1S=01, CC2S=01 (inputs) */
    TIM3_CCER  = 0x0000;          /* no inversion */
    TIM3_ARR   = 0xFFFFFFFF;      /* max count */
    TIM3_CNT   = 0;
    TIM3_CR1  |= (1U << 0);       /* CEN: enable counter */
}

void encoder_update(encoder_t *enc)
{
    if (!enc) return;

    int32_t raw        = (int32_t)TIM3_CNT;
    int32_t delta      = raw - enc->prev_count;
    enc->count        += delta;
    enc->prev_count    = raw;

    uint32_t cpr       = enc->ppr * COUNTS_PER_REV_FACTOR;
    enc->position_deg  = ((float)enc->count / (float)cpr) * 360.0f;

    /* velocity in RPM — assumes called every 10ms */
    enc->velocity_rpm  = ((float)delta / (float)cpr) * 6000.0f;
}

float encoder_get_position(encoder_t *enc)
{
    return enc ? enc->position_deg : 0.0f;
}

float encoder_get_velocity(encoder_t *enc)
{
    return enc ? enc->velocity_rpm : 0.0f;
}

void encoder_reset(encoder_t *enc)
{
    if (!enc) return;
    enc->count        = 0;
    enc->prev_count   = 0;
    enc->position_deg = 0.0f;
    enc->velocity_rpm = 0.0f;
    TIM3_CNT          = 0;
} 

#ifndef ENCODER_H
#define ENCODER_H

#include <stdint.h>

typedef struct {
    int32_t  count;
    int32_t  prev_count;
    float    position_deg;
    float    velocity_rpm;
    uint32_t ppr;           /* pulses per revolution */
} encoder_t;

void  encoder_init(encoder_t *enc, uint32_t ppr);
void  encoder_update(encoder_t *enc);
float encoder_get_position(encoder_t *enc);
float encoder_get_velocity(encoder_t *enc);
void  encoder_reset(encoder_t *enc);

#endif /* ENCODER_H */ 

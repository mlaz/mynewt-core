#include <stdint.h>

#ifndef _EASING_
#define _EASING_

#ifndef MAX_VAL
#define MAX_VAL 255
#endif /* MAX_BNESS */

#ifndef MAX_STEPS
#define MAX_STEPS 100
#endif /* MAX_STEPS */

#define HALF_PI 1.57079632679f   /* PI/2 */
#define ONE_DIV_E 0.36787944117f /* 1/e */
#define PI_TIMES2 6.28318530718f  /* 2PI */

typedef uint32_t (*easing_func_t)(float step);

/* Custom, used for breathing */
uint32_t exponential_custom_io(float step);
uint32_t exp_sin_custom_io(float step);
uint32_t sine_custom_io(int step);

/* Linear */
uint32_t linear_io(int step);

/* Exponential */
uint32_t exponential_in(int step);
uint32_t exponential_out(int step);
uint32_t exponential_io(int step);

/* Quadratic */
uint32_t quadratic_in(int step);
uint32_t quadratic_out(int step);
uint32_t quadratic_io(int step);

/* Cubic */
uint32_t cubic_in(int step);
uint32_t cubic_out(int step);
uint32_t cubic_io(int step);

/* Quartic */
uint32_t quartic_in(int step);
uint32_t quartic_out(int step);
uint32_t quartic_io(int step);

/* Quintic */
uint32_t quintic_in(int step);
uint32_t quintic_out(int step);
uint32_t quintic_io(int step);

/* Circular */
uint32_t circular_in(int step);
uint32_t circular_out(int step);
uint32_t circular_io(int step);

/* Sine */
uint32_t sine_in(int step);
uint32_t sine_out(int step);
uint32_t sine_io(int step);

/* Bounce */
uint32_t bounce_in(int step);
uint32_t bounce_out(int step);
uint32_t bounce_io(int step);

/* Back */
uint32_t back_in(int step);
uint32_t back_out(int step);
uint32_t back_io(int step);

#endif /* _EASING_ */

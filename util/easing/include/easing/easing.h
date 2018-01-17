#include <stdint.h>

#ifndef _EASING_
#define _EASING_

/* #ifndef MAX_VAL */
/* #define MAX_VAL 255 */
/* #endif /\* MAX_VAL *\/ */

/* #ifndef MAX_STEPS */
/* #define MAX_STEPS 200 */
/* #endif /\* MAX_STEPS *\/ */

#define HALF_PI 1.57079632679f   /* PI/2 */
#define ONE_DIV_E 0.36787944117f /* 1/e */
#define PI_TIMES2 6.28318530718f  /* 2PI */

typedef uint32_t (*easing_func_t)(int step, int max_steps, int max_val);

/* Custom, used for breathing */
uint32_t exponential_custom_io(int step, int max_steps, int max_val);
uint32_t exp_sin_custom_io(int step, int max_steps, int max_val);
uint32_t sine_custom_io(int step, int max_steps, int max_val);

/* Linear */
uint32_t linear_io(int step, int max_steps, int max_val);

/* Exponential */
uint32_t exponential_in(int step, int max_steps, int max_val);
uint32_t exponential_out(int step, int max_steps, int max_val);
uint32_t exponential_io(int step, int max_steps, int max_val);

/* Quadratic */
uint32_t quadratic_in(int step, int max_steps, int max_val);
uint32_t quadratic_out(int step, int max_steps, int max_val);
uint32_t quadratic_io(int step, int max_steps, int max_val);

/* Cubic */
uint32_t cubic_in(int step, int max_steps, int max_val);
uint32_t cubic_out(int step, int max_steps, int max_val);
uint32_t cubic_io(int step, int max_steps, int max_val);

/* Quartic */
uint32_t quartic_in(int step, int max_steps, int max_val);
uint32_t quartic_out(int step, int max_steps, int max_val);
uint32_t quartic_io(int step, int max_steps, int max_val);

/* Quintic */
uint32_t quintic_in(int step, int max_steps, int max_val);
uint32_t quintic_out(int step, int max_steps, int max_val);
uint32_t quintic_io(int step, int max_steps, int max_val);

/* Circular */
uint32_t circular_in(int step, int max_steps, int max_val);
uint32_t circular_out(int step, int max_steps, int max_val);
uint32_t circular_io(int step, int max_steps, int max_val);

/* Sine */
uint32_t sine_in(int step, int max_steps, int max_val);
uint32_t sine_out(int step, int max_steps, int max_val);
uint32_t sine_io(int step, int max_steps, int max_val);

/* Bounce */
uint32_t bounce_in(int step, int max_steps, int max_val);
uint32_t bounce_out(int step, int max_steps, int max_val);
uint32_t bounce_io(int step, int max_steps, int max_val);

/* Back */
uint32_t back_in(int step, int max_steps, int max_val);
uint32_t back_out(int step, int max_steps, int max_val);
uint32_t back_io(int step, int max_steps, int max_val);

uint32_t back_out_custom(int step, int max_steps, int max_val);
#endif /* _EASING_ */

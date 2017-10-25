#include "easing/easing.h"
#include <math.h>

static float R = (MAX_STEPS * log10(2)) / (log10(MAX_VAL));

uint32_t exponential_custom_io(int step)
{
    return powf(2, ((float)step / R)) - 1;
}

static float mplier = MAX_VAL / 2.35040238729f; /* MAX_VAL / */
static float pi_d_maxs = M_PI / MAX_STEPS;

/* http://www.wolframalpha.com/input/?i=(exp(sin(x%2F100pi%2Bpi%2F2))+-+1%2Fe)+*+255%2F(e-1%2Fe) */
uint32_t exp_sin_custom_io(int step)
{
    return ( exp( sin(((float)step * pi_d_maxs) + M_PI_2)) - ONE_DIV_E ) * mplier;
}

/* https://www.wolframalpha.com/input/?i=255+*+cos((2pi+*+x%2F100)+%2B+pi)+%2B+255 */
uint32_t sine_custom_io(int step)
{
    return MAX_VAL * cos((PI_TIMES2 * step/MAX_STEPS) + M_PI) + MAX_VAL;
}

/* static float half_maxv = MAX_VAL / 2.0; */
/* static float half_maxs = MAX_STEPS / 2.0; */
/* static float four_m_maxv = MAX_VAL * 4.0; */
/* static float one_d_maxv = 1.0 / MAX_VAL; */

/* /\* https://www.wolframalpha.com/input/?i=255+*+4+*+((x-100%2F2)%2F100)%5E2 *\/ */
/* uint32_t quadratic_io(int step) */
/* { */
/*     return four_m_maxv * pow(2, (0.01 * (step - half_maxv)) ); */
/* } */

/* /\* http://www.wolframalpha.com/input/?i=((((x-100%2F2)%2F(100%2F2)+)%5E3)%2B1)*255%2F2,+x%3D100 *\/ */
/* uint32_t cubic_io(int step) */
/* { */
/*     float base = (step - half_maxs) / half_maxs; */
/*     return (pow(base, 3) + 1) * half_maxv; */
/* } */

/* Linear */
uint32_t linear_io(int step)
{
	return step * MAX_VAL / MAX_STEPS;
}

/* Exponential */
uint32_t exponential_in(int step)
{
    return (step == 0) ?
        0 :
        MAX_VAL * pow(2, 10 * (step / MAX_STEPS - 1));
}

uint32_t exponential_out(int step)
{
	return (step == MAX_STEPS) ?
        MAX_VAL :
        MAX_VAL * ( -pow (2, -10 * step / MAX_STEPS) + 1);
}

uint32_t exponential_io(int step)
{
	float ratio = ( step / (MAX_STEPS / 2.0) ) - 1;

	if (step == 0)
		return 0;

	if (step == MAX_STEPS)
		return MAX_VAL;

	if (ratio < 1)
		return MAX_VAL / 2 * pow(2, 10 * (ratio));

	return MAX_VAL / 2 * (-pow(2, -10 * ratio) + 2);
}

/* Quadratic */
uint32_t quadratic_in(int step)
{
	float ratio = step / MAX_STEPS;

	return MAX_VAL * pow(ratio, 2);
}

uint32_t quadratic_out(int step)
{
    float ratio = step / MAX_STEPS;

	return -MAX_VAL * ratio * (ratio - 2);
}

uint32_t quadratic_io(int step)
{
	float ratio = step / (MAX_STEPS / 2.0);

	if (ratio < 1)
		return MAX_VAL / 2 * pow(ratio, 2);

    ratio--;
	return -MAX_VAL / 2 * (ratio * (ratio - 2) - 1);
}

/* Cubic */
uint32_t cubic_in(int step)
{
	float ratio = step / MAX_STEPS;

	return MAX_VAL * pow(ratio, 3);
}

uint32_t cubic_out(int step)
{
	float ratio = step / (MAX_STEPS - 1.0);
	
	return MAX_VAL * (pow(ratio, 3) + 1);
}

uint32_t cubic_io(int step)
{
	float ratio = step / (MAX_STEPS / 2.0);

	if (ratio < 1)
		return MAX_VAL / 2 * pow(ratio, 3);

	return MAX_VAL / 2 * (pow(ratio - 2, 3) + 2);
}

/* Quartic */
uint32_t quartic_in(int step)
{
	float ratio = step / MAX_STEPS;

	return MAX_VAL * pow(ratio, 4);
}

uint32_t quartic_out(int step)
{
	float ratio = step / (MAX_STEPS - 1.0);

	return - MAX_VAL * (pow(ratio, 4) - 1);
}

uint32_t quartic_io(int step)
{
	float ratio = step / (MAX_STEPS / 2.0);

	if (ratio < 1)
		return MAX_VAL / 2 * pow(ratio, 5);

	return - MAX_VAL / 2 * (pow(ratio - 2, 4) - 2);
}

/* Quintic */
uint32_t quintic_in(int step)
{
	float ratio = step / MAX_STEPS;

	return MAX_VAL * pow(ratio, 5);
}

uint32_t quintic_out(int step)
{
	float ratio = step / (MAX_STEPS - 1.0);

	return MAX_VAL * (pow(ratio, 5) + 1);
}

uint32_t quintic_io(int step)
{
	float ratio = step / (MAX_STEPS / 2.0);

	if (ratio < 1)
		return MAX_VAL / 2 * pow(ratio, 5);

	return MAX_VAL / 2 * (pow(ratio, 5) + 2);
}

/* Circular */
uint32_t circular_in(int step)
{
	float ratio = step / MAX_STEPS;

	return - MAX_VAL * (sqrt(1 - pow(ratio, 2) ) - 1);
}

uint32_t circular_out(int step)
{
	float ratio = step / (MAX_STEPS - 1.0);
	return MAX_VAL * sqrt(1 - pow(ratio, 2));
}

uint32_t circular_io(int step)
{
	float ratio = step / (MAX_STEPS / 2.0);

	if (ratio < 1)
		return - MAX_VAL / 2 * (sqrt(1 - pow(ratio, 2)) - 1);

	return MAX_VAL / 2 * (sqrt(1 - pow(ratio - 2, 2)) + 1);
}

/* Sine */
uint32_t sine_in(int step)
{
	return -MAX_VAL * cos(step / MAX_STEPS * M_PI_2) + MAX_VAL;
}

uint32_t sine_out(int step)
{
	return MAX_VAL * sin(step / MAX_STEPS * M_PI_2);
}

uint32_t sine_io(int step)
{
	return -MAX_VAL / 2 * (cos(M_PI * step / MAX_STEPS) - 1);
}


/* Bounce */
uint32_t bounce_in(int step)
{
	return MAX_VAL - bounce_out(MAX_STEPS - step);
}

uint32_t bounce_out(int step)
{
	float ratio = step / MAX_STEPS;

	if (ratio < (1 / 2.75))
		return MAX_VAL * (7.5625 * ratio * ratio);

	if (ratio < (2 / 2.75))
	{
        ratio -= 1.5 / 2.75;
        return MAX_VAL * (7.5625 * ratio * ratio + 0.75);
	}

	if ( ratio < (2.5 / 2.75))
	{
		ratio -= 2.25 / 2.75;
		return MAX_VAL * (7.5625 * ratio * ratio + 0.9375);
	}

	ratio -= 2.625 / 2.75;
	return MAX_VAL * (7.5625 * ratio * ratio + 0.984375);
}

uint32_t bounce_io(int step)
{
	if (step < MAX_STEPS / 2)
		return bounce_in(step * 2) * 0.5;
	else
		return bounce_out(step * 2 - MAX_STEPS) * 0.5 + MAX_VAL * 0.5;
}

/* Back */
uint32_t back_in(int step)
{
	float s = 1.70158f;
    float ratio = step / MAX_STEPS;
	return MAX_VAL * ratio * ratio * ((s + 1) * ratio - s);
}

uint32_t back_out(int step)
{
	float s = 1.70158f;
	float ratio = step / MAX_STEPS - 1;
	return MAX_VAL * (ratio * ratio * ((s + 1) * ratio + s) + 1);
}

uint32_t back_io(int step)
{
    float s = 1.70158f * 1.525;
    float ratio = step / (MAX_STEPS / 2);

	if (ratio < 1)
        return MAX_VAL / 2 * (ratio * ratio * ((s + 1) * ratio - s));

	ratio -= 2;
	return MAX_VAL / 2 * (ratio * ratio * ((s + 1) * ratio + s) + 2);
}

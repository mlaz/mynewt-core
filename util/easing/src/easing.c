#include "easing/easing.h"
#include <math.h>

/* static float R = max_steps * log10(2) / log10(max_val); */

uint32_t exponential_custom_io(int step, int max_steps, int max_val)
{
    float R = (float) max_steps * log10(2) / log10((float) max_val);
    return pow(2, ((float) step / R)) - 1;
}

/* static float mplier = max_val / 2.35040238729f; /\* max_val / *\/ */
/* static float pi_d_maxs = M_PI / max_steps; */

/* http://www.wolframalpha.com/input/?i=(exp(sin(x%2F100pi%2Bpi%2F2))+-+1%2Fe)+*+255%2F(e-1%2Fe) */
uint32_t exp_sin_custom_io(int step, int max_steps, int max_val)
{
    float mplier = max_val / 2.35040238729f;
    float pi_d_maxs = M_PI / max_steps;
    return ( exp( sin((step * pi_d_maxs) + M_PI_2)) - ONE_DIV_E ) * mplier;
}

/* https://www.wolframalpha.com/input/?i=255+*+cos((2pi+*+x%2F100)+%2B+pi)+%2B+255 */
uint32_t sine_custom_io(int step, int max_steps, int max_val)
{
    return max_val * cos((PI_TIMES2 * step/max_steps) + M_PI) + max_val;
}

/* static float half_maxv = max_val / 2.0; */
/* static float half_maxs = max_steps / 2.0; */
/* static float four_m_maxv = max_val * 4.0; */
/* static float one_d_maxv = 1.0 / max_val; */

/* /\* https://www.wolframalpha.com/input/?i=255+*+4+*+((x-100%2F2)%2F100)%5E2 *\/ */
/* uint32_t quadratic_io(int step, int max_steps, int max_val) */
/* { */
/*     return four_m_maxv * pow(2, (0.01 * (step - half_maxv)) ); */
/* } */

/* /\* http://www.wolframalpha.com/input/?i=((((x-100%2F2)%2F(100%2F2)+)%5E3)%2B1)*255%2F2,+x%3D100 *\/ */
/* uint32_t cubic_io(int step, int max_steps, int max_val) */
/* { */
/*     float base = (step - half_maxs) / half_maxs; */
/*     return (pow(base, 3) + 1) * half_maxv; */
/* } */

/* Linear */
uint32_t linear_io(int step, int max_steps, int max_val)
{
	return step * max_val / max_steps;
}

/* Exponential */
uint32_t exponential_in(int step, int max_steps, int max_val)
{
    return (step == 0) ?
        0 :
        pow(max_val, (float)step/max_steps);
}

uint32_t exponential_out(int step, int max_steps, int max_val)
{
	return (step == max_steps) ?
        max_val :
        max_val - pow(max_val, 1.0 - (float)step/max_steps);
}

uint32_t exponential_io(int step, int max_steps, int max_val)
{
	float ratio = ( (float)step / (max_steps / 2.0) );

	if (step == 0)
		return 0;

	if (step == max_steps)
		return max_val;

	if (ratio < 1)
        return powf((float)max_val/2.0, (float)step/(max_steps/2.0));

	return max_val/2.0 + max_val/2.0 -
        pow(max_val/2.0, 1.0 - ((float)step - (max_steps / 2)) / (max_steps / 2) );
}

/* Quadratic */
uint32_t quadratic_in(int step, int max_steps, int max_val)
{
	float ratio = (float) step / max_steps;

	return max_val * pow(ratio, 2);
}

uint32_t quadratic_out(int step, int max_steps, int max_val)
{
    float ratio = (float) step / max_steps;

	return -max_val * ratio * (ratio - 2.0);
}

uint32_t quadratic_io(int step, int max_steps, int max_val)
{
	float ratio = (float) step / (max_steps / 2.0);

	if (ratio < 1)
		return max_val / 2.0 * pow(ratio, 2);

    ratio = (step - (max_steps/2.0)) / (max_steps/2.0);
    return (max_val / 2.0) - (max_val / 2.0) * ratio * (ratio - 2.0);
}

/* Cubic */
uint32_t cubic_in(int step, int max_steps, int max_val)
{
	float ratio = step / max_steps;

	return max_val * pow(ratio, 3);
}

uint32_t cubic_out(int step, int max_steps, int max_val)
{
	float ratio = step / (max_steps - 1.0);

	return max_val * (pow(ratio, 3) + 1);
}

uint32_t cubic_io(int step, int max_steps, int max_val)
{
	float ratio = (float) step / (max_steps / 2.0);

	if (ratio < 1)
		return max_val / 2 * pow(ratio, 3);

	return max_val / 2 * (pow(ratio - 2, 3) + 2);
}

/* Quartic */
uint32_t quartic_in(int step, int max_steps, int max_val)
{
	float ratio = (float) step / max_steps;

	return max_val * pow(ratio, 4);
}

uint32_t quartic_out(int step, int max_steps, int max_val)
{
	float ratio = ((float) step / max_steps) - 1.0;

	return max_val + max_val * pow(ratio, 5);
}

uint32_t quartic_io(int step, int max_steps, int max_val)
{
	float ratio = (float) step / (max_steps / 2.0);

	if (ratio < 1)
		return max_val / 2 * pow(ratio, 4);

	return max_val + max_val / 2 * pow(ratio -2, 5);
}

/* Quintic */
uint32_t quintic_in(int step, int max_steps, int max_val)
{
	float ratio = (float) step / max_steps;

	return max_val * pow(ratio, 5);
}

uint32_t quintic_out(int step, int max_steps, int max_val)
{
	float ratio = ((float) step / max_steps) - 1.0;

	return max_val + max_val * pow(ratio, 5);
}

uint32_t quintic_io(int step, int max_steps, int max_val)
{
	float ratio = (float) step / (max_steps / 2.0);

	if (ratio < 1)
		return max_val / 2 * pow(ratio, 5);

    ratio -= 2;
	return max_val + max_val / 2 * pow(ratio, 5);
}

/* Circular */
uint32_t circular_in(int step, int max_steps, int max_val)
{
	float ratio = (float) step / max_steps;

	return - max_val * (sqrt(1 - pow(ratio, 2) ) - 1);
}

uint32_t circular_out(int step, int max_steps, int max_val)
{
	float ratio = ((float) step - max_steps) / (max_steps - 1.0);
	return max_val * sqrt(1 - pow(ratio, 2));
}

uint32_t circular_io(int step, int max_steps, int max_val)
{
	float ratio = (float) step / (max_steps / 2.0);

	if (ratio < 1)
		return - max_val / 2 * (sqrt(1 - pow(ratio, 2)) - 1);

	return max_val / 2 * (sqrt(1 - pow(ratio - 2, 2)) + 1);
}

/* Sine */
uint32_t sine_in(int step, int max_steps, int max_val)
{
	return -max_val * cos((float)step / max_steps * M_PI_2) + max_val;
}

uint32_t sine_out(int step, int max_steps, int max_val)
{
	return max_val * sin( (((float)step / max_steps) + 1) * M_PI_2);
}

uint32_t sine_io(int step, int max_steps, int max_val)
{
	return -max_val / 2 * (cos(M_PI * step / max_steps) - 1);
}


/* Bounce */
uint32_t bounce_in(int step, int max_steps, int max_val)
{
	return max_val - bounce_out(max_steps - step, max_steps, max_val);
}

uint32_t bounce_out(int step, int max_steps, int max_val)
{
	float ratio = step / max_steps;

	if (ratio < (1 / 2.75))
		return max_val * (7.5625 * ratio * ratio);

	if (ratio < (2 / 2.75))
	{
        ratio -= 1.5 / 2.75;
        return max_val * (7.5625 * ratio * ratio + 0.75);
	}

	if ( ratio < (2.5 / 2.75))
	{
		ratio -= 2.25 / 2.75;
		return max_val * (7.5625 * ratio * ratio + 0.9375);
	}

	ratio -= 2.625 / 2.75;
	return max_val * (7.5625 * ratio * ratio + 0.984375);
}

uint32_t bounce_io(int step, int max_steps, int max_val)
{
	if (step < max_steps / 2)
		return bounce_in(step * 2, max_steps, max_val) * 0.5;
	else
		return bounce_out(step * 2 - max_steps, max_steps, max_val) * 0.5 + max_val * 0.5;
}

/* Back */
uint32_t back_in(int step, int max_steps, int max_val)
{
	float s = 1.70158f;
    float ratio = step / max_steps;
	return max_val * ratio * ratio * ((s + 1) * ratio - s);
}

uint32_t back_out(int step, int max_steps, int max_val)
{
	float s = 1.70158f;
	float ratio = step / max_steps - 1;
	return max_val * (ratio * ratio * ((s + 1) * ratio + s) + 1);
}

/* http://www.wolframalpha.com/input/?i=120+*+0.90909803921+*+((x%2F128+-1)+*+(x%2F128+-+1)+*+((1.70158+%2B+1)+*+(x%2F128+-+1)+%2B+1.70158)+%2B+1),+x+%3D+74.2531 */
uint32_t back_out_custom(int step, int max_steps, int max_val)
{
    float s;
    float ratio;

    if (step <=  (max_steps * 0.58221171875)) {
        s = 1.70158f;
        ratio = (float) step / (float) max_steps - 1;
        return (float) max_val * 0.90909803921f * (ratio * ratio * ((s + 1) * ratio + s) + 1);
    }
    return -pow((float)step - ((float)max_steps * 0.578125), 2)/((float)max_steps * 2.7) + (float)max_val;
}

uint32_t back_io(int step, int max_steps, int max_val)
{
    float s = 1.70158f * 1.525;
    float ratio = step / (max_steps / 2);

	if (ratio < 1)
        return max_val / 2 * (ratio * ratio * ((s + 1) * ratio - s));

	ratio -= 2;
	return max_val / 2 * (ratio * ratio * ((s + 1) * ratio + s) + 2);
}

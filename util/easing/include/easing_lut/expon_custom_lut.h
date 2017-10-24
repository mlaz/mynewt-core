#ifndef __EASING_LUT__
#define __EASING_LUT__

/* */
#ifndef __EASING__

#ifndef MAX_VAL
#define MAX_VAL 255
#endif /* MAX_BNESS */

#ifndef MAX_STEPS
#define MAX_STEPS 100
#endif /* MAX_STEPS */

#enfif /* __EASING__ */

#ifndef __EXPON_CUSTOM_EASING_LUT__
#define __EXPON_CUSTOM_EASING_LUT__

expon_custom_lut[] = {0,
                      1,
                      2,
};

inline uint32_t expon_custom_lut_io(uint32_t step)
{
    return expon_custom_lut[step];
}

#endif /* __EXPON_CUSTOM_EASING_LUT__ */

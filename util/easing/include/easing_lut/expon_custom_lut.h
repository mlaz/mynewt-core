#ifndef __EASING__

#ifndef MAX_VAL
#define MAX_VAL 255
#endif /* MAX_BNESS */

#ifndef MAX_STEPS
#define MAX_STEPS 100
#endif /* MAX_STEPS */

#endif /* __EASING__ */

#ifndef __EXPON_CUSTOM_EASING_LUT__
#define __EXPON_CUSTOM_EASING_LUT__

uint32_t expon_custom_lut[] = { 0,
                                0,
                                0,
                                0,
                                0,
                                0,
                                0,
                                0,
                                0,
                                0,
                                0,
                                0,
                                0,
                                1,
                                1,
                                1,
                                1,
                                1,
                                1,
                                1,
                                2,
                                2,
                                2,
                                2,
                                2,
                                2,
                                3,
                                3,
                                3,
                                3,
                                4,
                                4,
                                4,
                                5,
                                5,
                                5,
                                6,
                                6,
                                7,
                                7,
                                8,
                                8,
                                9,
                                9,
                                10,
                                11,
                                11,
                                12,
                                13,
                                14,
                                14,
                                15,
                                16,
                                17,
                                18,
                                20,
                                21,
                                22,
                                23,
                                25,
                                26,
                                28,
                                30,
                                31,
                                33,
                                35,
                                37,
                                39,
                                42,
                                44,
                                47,
                                50,
                                53,
                                56,
                                59,
                                62,
                                66,
                                70,
                                74,
                                78,
                                83,
                                87,
                                93,
                                98,
                                104,
                                110,
                                116,
                                123,
                                130,
                                137,
                                145,
                                153,
                                162,
                                172,
                                181,
                                192,
                                203,
                                214,
                                227,
                                240,
                                253
};

uint32_t expon_custom_lut_io(int step)
{
    return expon_custom_lut[step];
}

#endif /* __EXPON_CUSTOM_EASING_LUT__ */

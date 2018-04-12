/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

/*
 * Functional test program for easing functions.
 *
 * Compile with:
 *    gcc -Wall -I../include -o /tmp/easing easing-csv.c ../src/easing.c -lm
 *
 * Iterates through the different steps of each easing function and prints
 * a CSV that can then further be processed.
 *
 * Each value is prefixed by an optional tag (if specified in the command
 * line, the easing function name, it's direction (in, out, io), type api
 * type (float, int) and then the step/value pair.
 *
 * See easing-plot.R for an example.
 */
#include "easing/easing.h"
#include <stdint.h>
#include <stdio.h>


# define  FUNC(x) {                             \
    #x,                                         \
    {x ## _f_in, x ## _f_out, x ## _f_io},      \
    {x ## _int_in, x ## _int_out, x ## _int_io} \
}

const int32_t steps = 100000;

typedef struct {
    const char *name;
    easing_f_func_t   f[3];
    easing_int_func_t i[3];
} func_t;

func_t func[] = {
    FUNC(quadratic),
    FUNC(cubic),
    FUNC(quartic),
    FUNC(quintic),
    FUNC(circular),
    FUNC(sine),
    FUNC(bounce),
    FUNC(back),
};

const char *inout[] = {
    "in", "out", "io"
};


int main(int argc, char *argv[]) {
    if (argc > 1) {
        fprintf(stdout, "tag,func,dir,type,step,value\n");
    } else {
        fprintf(stdout, "func,dir,type,step,value\n");
    }

    int tests = sizeof(func) / sizeof(func[0]);
    for (int t=0; t<tests; ++t) {
        func_t *f = &func[t];
        for (int dir=0; dir<3; ++dir) {
            const char *typ = inout[dir];
            for (int32_t s=0; s<steps; s+=1) {
                float   val_f = f->f[dir](s, steps, INT32_MAX);
                int32_t val_i = f->i[dir](s, steps, INT32_MAX);
                if (argc > 1) {
                    fprintf(stdout, "%s,%s,%s,float,%d,%f\n", argv[1], f->name, typ, s, val_f);
                    fprintf(stdout, "%s,%s,%s,int,%d,%d\n", argv[1], f->name, typ, s, val_i);
                } else {
                    fprintf(stdout, "%s,%s,float,%d,%f\n", f->name, typ, s, val_f);
                    fprintf(stdout, "%s,%s,int,%d,%d\n", f->name, typ, s, val_i);
                }
            }
        }
    }
    return 0;
}

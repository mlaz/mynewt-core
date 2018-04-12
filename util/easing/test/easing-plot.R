#!/usr/bin/r

# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements.  See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership.  The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License.  You may obtain a copy of the License at
#
#  http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied.  See the License for the
# specific language governing permissions and limitations
# under the License.

# R script to generate a plot from a CSV file as produced by
# easing-csv.c
# In order to run the script R has to be installed with the ggplot2
# library. No other dependencies should exist on a typical Linux
# installation.
#
# Exammple invocation:
#    gcc -Wall -I../include -o /tmp/easing easing-csv.c ../src/easing.c -lm 
#    /tmp/easing | ./easing-plot.R
#
# use your favourite image viewer to look at easing.png

library(ggplot2)

print('Reading data')
d = read.csv(file('stdin'))

print('Processing plot into easing.png')
p = ggplot(d, aes(x=step, color=dir)) + geom_line(aes(y=value)) + facet_grid(func~type, scale='free')
ggsave('easing.png', p, device='png', width=180, height=240, units='mm')

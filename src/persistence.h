// The MIT License (MIT)
//
// Copyright (c) 2014 Nick Penkov <nick at npenkov dot org>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef __PebSlee__persistence__
#define __PebSlee__persistence__

#include <stdio.h>
#include "logic.h"


#define CONFIG_PERSISTENT_KEY 0

#define PERSISTENT_COUNT_KEY 1
#define PERSISTENT_VALUES_KEY 2
// use 2, 3, 4 - every each with 240 bytes

#define PERSISTENT_START_TIME_KEY 1
#define PERSISTENT_END_TIME_KEY 2
#define PERSISTENT_STATS_KEY 3

#define STAT_START 10
#define STAT_STEP 3

#define MAX_STAT_COUNT 10

// Steps are formed:
// STAT_START + (stat * STAT_STEP) + PERSISTENT_START_TIME_KEY
// STAT_START + (stat * STAT_STEP) + PERSISTENT_END_TIME_KEY
// STAT_START + (stat * STAT_STEP) + PERSISTENT_STATS_KEY

// Max persistem
#define COUNT_STATS_KEY 100
#define VERSION_KEY 254

#define MAX_PERSIST_BUFFER 240

typedef struct {
    uint32_t start_time;
    uint32_t end_time;
    uint16_t stat[COUNT_PHASES];
} StatData;


void store_data(SleepData* data);
void read_last_sleep_data(SleepData *sd);

int count_stat_data();
StatData** read_stat_data();
StatData* read_stat_data_rec(int index);

void migrate_version();

void clear_sleep_stats();


#endif /* defined(__PebSlee__persistence__) */

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

#include <stdio.h>
#include <pebble.h>

#include "persistence.h"
#include "logic.h"

#ifdef DEBUG
void dump_stat_data(StatData *sd) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Stat data %ld %ld %d/%d/%d/%d", sd->start_time, sd->end_time, sd->stat[0], sd->stat[1], sd->stat[2], sd->stat[3]);
}
#endif

void store_data(SleepData* data) {

    // Prevent storing empty sleep data
    if (data->count_values <= 0)
        return;
        
    // Store first the values
    persist_write_int(PERSISTENT_COUNT_KEY, data->count_values);
    // Transform values from 0->5000 to 0-255 scale
    uint8_t *values = malloc(sizeof(uint8_t) * data->count_values);
    for (int i = 0; i < data->count_values; i++) {
        values[i] = data->minutes_value[i]*MEASURE_COEFICENT;
    }
    // Now write the values
    //#define PERSISTENT_VALUES_KEY 2
    // use 2, 3, 4 - every each with 240 bytes
    int chunks = data->count_values / MAX_PERSIST_BUFFER;
    if (data->count_values % MAX_PERSIST_BUFFER > 0)
        chunks++;
    
    for (int i = 0; i < chunks; i++) {
        int size = MAX_PERSIST_BUFFER;
        if (i == chunks - 1) { // Last chunk
            if (data->count_values % MAX_PERSIST_BUFFER > 0) {
                size = data->count_values % MAX_PERSIST_BUFFER;
            }
        }
        persist_write_data(PERSISTENT_VALUES_KEY+i, &values[i*MAX_PERSIST_BUFFER], size);
    }
    
    // Write statistics
    int csd = count_stat_data();
    
    StatData *new_stat = malloc(sizeof(StatData));
    new_stat->start_time = data->start_time;
    new_stat->end_time = data->end_time;
    for (int i = 0; i < COUNT_PHASES; i++) {
        new_stat->stat[i] = data->stat[i];
    }
    
    StatData **stat_data = malloc(sizeof(StatData*)*csd);
    for (int i = 0; i < csd; i++) {
        StatData *sd = malloc(sizeof(StatData));
        persist_read_data(STAT_START+i, sd, sizeof(StatData));
        stat_data[i] = sd;
#ifdef DEBUG
        dump_stat_data(sd);
#endif
    }
    
    if (csd + 1 < MAX_STAT_COUNT) {
#ifdef DEBUG
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Persist new stat record %d", (csd + 1));
#endif
        // Just add one more at the end
        persist_write_data(STAT_START + csd, new_stat, sizeof(StatData));
        persist_write_int(COUNT_STATS_KEY, csd + 1);
    } else {
#ifdef DEBUG
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Persist new stat record at the end");
#endif
        // Write from 1..MAX_STAT_COUNT
        for (int i = 0; i < MAX_STAT_COUNT - 1; i++) {
            persist_write_data(STAT_START + i, &stat_data[i+1], sizeof(stat_data[i+1]));
        }
        // ...and the newest one
        persist_write_data(STAT_START + MAX_STAT_COUNT - 1, &new_stat, sizeof(new_stat));
        persist_write_int(COUNT_STATS_KEY, MAX_STAT_COUNT);
    }
}

SleepData* read_last_sleep_data() {
    // Read the stats from DB
    int csd = count_stat_data();
    StatData *stat = malloc(sizeof(StatData));
    persist_read_data(STAT_START+csd-1, stat, sizeof(StatData));
    
    // Create using stats
    SleepData *sleep_data = malloc(sizeof(SleepData));
    sleep_data->start_time = stat->start_time;
    sleep_data->end_time = stat->end_time;
    for (int i = 0; i < COUNT_PHASES; i++) {
        sleep_data->stat[i] = stat->stat[i];
    }
    
    // Read values
    sleep_data->count_values = persist_read_int(PERSISTENT_COUNT_KEY);
    
    
    uint8_t *values = malloc(sizeof(uint8_t) * sleep_data->count_values);
    // Now write the values
    //#define PERSISTENT_VALUES_KEY 2
    // use 2, 3, 4 - every each with 240 bytes
    int chunks = sleep_data->count_values / MAX_PERSIST_BUFFER;
    if (sleep_data->count_values % MAX_PERSIST_BUFFER > 0)
        chunks++;
    
    for (int i = 0; i < chunks; i++) {
        int size = MAX_PERSIST_BUFFER;
        if (i == chunks - 1) { // Last chunk
            if (sleep_data->count_values % MAX_PERSIST_BUFFER > 0) {
                size = sleep_data->count_values % MAX_PERSIST_BUFFER;
            }
        }
        persist_read_data(PERSISTENT_VALUES_KEY+i, &values[i*MAX_PERSIST_BUFFER], size);
    }
    for (int i = 0; i < sleep_data->count_values; i++) {
        sleep_data->minutes_value[i] = values[i]; // Cast from uint8 to uint16
    }
    
    return sleep_data;
}

/*
 * Storing and retrieving statistics data
 */
int count_stat_data() {
    if (persist_exists(COUNT_STATS_KEY)) {
        return persist_read_int(COUNT_STATS_KEY);
    } else {
        persist_write_int(COUNT_STATS_KEY, 0);
        return 0;
    }
}

StatData** read_stat_data() {
    int csd = count_stat_data();
#ifdef DEBUG
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Read stat data %d", csd);
#endif
    StatData **stat_data = malloc(sizeof(StatData*)*csd);
    for (int i = 0; i < csd; i++) {
        StatData *sd = malloc(sizeof(StatData));
        persist_read_data(STAT_START+i, sd, sizeof(StatData));
#ifdef DEBUG
        dump_stat_data(sd);
#endif
        stat_data[i] = sd;
    }
    return stat_data;
}

/*
 * Migrate the DB version
 */
void migrate_version() {
    if (!persist_exists(VERSION_KEY)) {
        // In version 1.0 we have 4 values
        if (persist_exists(1))
            persist_delete(1);
        if (persist_exists(2))
            persist_delete(2);
        if (persist_exists(3))
            persist_delete(3);
        if (persist_exists(4))
            persist_delete(4);
        persist_write_int(COUNT_STATS_KEY, 0);
        persist_write_int(VERSION_KEY, 1);
    }
    // Reset data
//    for (int i = 0; i <= COUNT_STATS_KEY; i++) {
//        if (persist_exists(i))
//            persist_delete(i);
//    }
}

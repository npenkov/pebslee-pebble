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

#include "constants.h"
#include "persistence.h"
#include "logic.h"

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
        D("Write values with key %d", (PERSISTENT_VALUES_KEY+i));

        persist_write_data(PERSISTENT_VALUES_KEY+i, &values[i*MAX_PERSIST_BUFFER], size);
    }
    free(values);
    
    // Write statistics
    int csd = count_stat_data();
    D("Count stat data %d", csd);

    
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

        D("Stat data %ld %ld %d/%d/%d/%d", sd->start_time, sd->end_time, sd->stat[0], sd->stat[1], sd->stat[2], sd->stat[3]);
    }
    
    if (csd < MAX_STAT_COUNT) {
        D("Persist new stat record %d at %d", (csd + 1), (STAT_START + csd));

        // Just add one more at the end
        persist_write_data(STAT_START + csd, new_stat, sizeof(StatData));
        persist_write_int(COUNT_STATS_KEY, csd + 1);
    } else {
        D("Persist new stat record at the end at %d", (STAT_START + csd));

        // Write from 1..MAX_STAT_COUNT
        for (int i = 0; i < MAX_STAT_COUNT - 1; i++) {
            persist_write_data(STAT_START + i, stat_data[i+1], sizeof(StatData));
        }
        // ...and the newest one
        persist_write_data(STAT_START + csd - 1, new_stat, sizeof(StatData));
        persist_write_int(COUNT_STATS_KEY, csd);
    }
    for (int i = 0; i < csd; i++) {
        free(stat_data[i]);
    }
    free(stat_data);
    free(new_stat);
}

void read_last_sleep_data(SleepData *sd) {
    // Read the stats from DB
    int csd = count_stat_data();
    StatData *stat = malloc(sizeof(StatData));
    
    persist_read_data(STAT_START+csd-1, stat, sizeof(StatData));

    if (stat == NULL) {
        return;
    }
    
    sd->start_time = stat->start_time;
    sd->end_time = stat->end_time;
    
    for (int i = 0; i < COUNT_PHASES; i++) {
        sd->stat[i] = stat->stat[i];
    }
    free(stat);
    
    // Read values
    sd->count_values = persist_read_int(PERSISTENT_COUNT_KEY);
    
    uint8_t *values = malloc(sizeof(uint8_t) * sd->count_values);
    // Now write the values
    //#define PERSISTENT_VALUES_KEY 2
    // use 2, 3, 4 - every each with 240 bytes
    int chunks = sd->count_values / MAX_PERSIST_BUFFER;
    if (sd->count_values % MAX_PERSIST_BUFFER > 0)
        chunks++;
    
    for (int i = 0; i < chunks; i++) {
        int size = MAX_PERSIST_BUFFER;
        if (i == chunks - 1) { // Last chunk
            if (sd->count_values % MAX_PERSIST_BUFFER > 0) {
                size = sd->count_values % MAX_PERSIST_BUFFER;
            }
        }
        persist_read_data(PERSISTENT_VALUES_KEY+i, &values[i*MAX_PERSIST_BUFFER], size);
    }
    for (int i = 0; i < sd->count_values; i++) {
        sd->minutes_value[i] = values[i]; // Cast from uint8 to uint16
    }
    free(values);
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
    
    D("Read stat data %d reserve: %d bytes", csd, sizeof(StatData*)*csd);

    StatData **stat_data = malloc(sizeof(StatData*)*csd);
    for (int i = 0; i < csd; i++) {
        StatData *sd = malloc(sizeof(StatData));
        persist_read_data(STAT_START+i, sd, sizeof(StatData));

        D("Stat data %ld %ld %d/%d/%d/%d", sd->start_time, sd->end_time, sd->stat[0], sd->stat[1], sd->stat[2], sd->stat[3]);
        
        stat_data[i] = sd;
    }
    return stat_data;
}

StatData* read_stat_data_rec(int index) {
    StatData *sd = malloc(sizeof(StatData));
    persist_read_data(STAT_START+index, sd, sizeof(StatData));

    D("Stat data %ld %ld %d/%d/%d/%d", sd->start_time, sd->end_time, sd->stat[0], sd->stat[1], sd->stat[2], sd->stat[3]);
    
    return sd;
}


/*
 * Migrate the DB version
 * Current version is 2
 */
void migrate_version() {
    const int current_db_version = 5;
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
        persist_write_int(VERSION_KEY, current_db_version);
    } else {
        int version = persist_read_int(VERSION_KEY);
        if (version == 1) {
            // Drop statistics as they were stored in a wrong way
            persist_delete(COUNT_STATS_KEY);
        } else if (version == 2) {
            StatData **sds = read_stat_data();
            int csd = count_stat_data();
            uint32_t old_start_time = 0;
            uint32_t old_end_time = 0;
            if (csd >= 1) {
                old_start_time = sds[0]->start_time;
                old_end_time = sds[0]->end_time;
            }
            int lastCount = csd;
            for (int i = 1; i < csd; i++) {
                if (old_start_time == sds[i]->start_time && old_end_time == sds[i]->end_time) {
                    // Break and set counter
                    lastCount = i;
                    break;
                } else {
                    old_start_time = sds[i]->start_time;
                    old_end_time = sds[i]->end_time;
                }
            }
            persist_write_int(COUNT_STATS_KEY, lastCount);
            free(sds);
        }
        // Migrate sleep data structure
        if (version < current_db_version) {
            D("Migrate configuraiton data");

            persist_read_config();
            set_config_up_coef(UP_COEF_NORMAL);
            set_config_down_coef(DOWN_COEF_NORMAL);
            set_config_snooze(0);
            persist_write_config();
            persist_write_int(VERSION_KEY, current_db_version);
        }
    }
    
    // Reset data
//    for (int i = 0; i <= COUNT_STATS_KEY; i++) {
//        if (persist_exists(i))
//            persist_delete(i);
//    }
}

void clear_sleep_stats() {
    persist_write_int(COUNT_STATS_KEY, 0);
}

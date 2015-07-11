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

int count_motion_values() {
    return persist_read_int(PERSISTENT_COUNT_KEY);
}

uint8_t *read_motion_data() {
    int cntVals = persist_read_int(PERSISTENT_COUNT_KEY);

    uint8_t *motionVals = malloc(cntVals);

    if (motionVals == NULL) {
        D("Error allocating memory %d", cntVals);
        return motionVals;
    }

    if (cntVals == 0) {
        D("Count is 0");
        return motionVals;
    }

    int chunks = cntVals / MAX_PERSIST_BUFFER;
    if (cntVals % MAX_PERSIST_BUFFER > 0)
        chunks++;

    int rest = cntVals % MAX_PERSIST_BUFFER;
    for (int i = 0; i < chunks; i++) {
        int size = MAX_PERSIST_BUFFER;
        if (i == chunks - 1) { // Last chunk
            if (rest > 0) {
                size = rest;
            }
        }
        persist_read_data(PERSISTENT_VALUES_KEY+i, &motionVals[i*MAX_PERSIST_BUFFER], size);
    }
    return motionVals;
}

StatData* read_last_stat_data() {
    int csd = count_stat_data();
    StatData *stat = malloc(sizeof(StatData));
    persist_read_data(STAT_START+csd-1, stat, sizeof(StatData));
    return stat;
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
    const int current_db_version = 7;
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
            // Next verison - set default active profile
            set_config_active_profile(ACTIVE_PROFILE_NORMAL);

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

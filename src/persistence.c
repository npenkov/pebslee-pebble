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

//
// ----- Current record index handling
//
static int get_current_persistent_index() {
    if (persist_exists(COUNT_PERSISTENT_KEY)) {
        int ret = persist_read_int(COUNT_PERSISTENT_KEY);
#ifdef DEBUG
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Get current persistent index %d", ret);
#endif
        return ret;
    } else {
        persist_write_int(COUNT_PERSISTENT_KEY, 0);
#ifdef DEBUG
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Get current persistent index %d", 0);
#endif
        return 0;
    }
}

static int increse_current_persistent_index() {
    int currentIndex = get_current_persistent_index();
#ifdef DEBUG
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Increse current index %d", currentIndex);
#endif
    if (currentIndex + 1 < COUNT_PERSISTENT_KEY) {
        persist_write_int(COUNT_PERSISTENT_KEY, ++currentIndex);
#ifdef DEBUG
        APP_LOG(APP_LOG_LEVEL_DEBUG, "...return %d", currentIndex);
#endif
        return currentIndex;
    } else {
        currentIndex = 0; // Make a round
        persist_write_int(COUNT_PERSISTENT_KEY, 0);
#ifdef DEBUG
        APP_LOG(APP_LOG_LEVEL_DEBUG, "...return %d", currentIndex);
#endif
        return currentIndex;
    }
}


//
// ---- Read and store
//
static void store_data_with_index(SleepData* data, int recIndex) {
#ifdef DEBUG
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Store data with recIndex %d", recIndex);
#endif
    
    int offset = recIndex * PERSISTENT_SLEEP_STEP;
    // Store in DB
    persist_write_int(PERSISTENT_START_TIME_KEY+offset, data->start_time);
    persist_write_int(PERSISTENT_END_TIME_KEY+offset, data->end_time);
    
    int32_t cnt_values = data->count_values;
    persist_write_int(PERSISTENT_COUNT_KEY+offset, cnt_values);
    
    int dci = 0;
    int totalSize = data->count_values*2;
    if (totalSize < MAX_PERSIST_BUFFER) {
        // Write it and return
        persist_write_data(PERSISTENT_VALUES_KEY+offset, data->minutes_value, totalSize);
    } else {
        for (int i = 0; i < PERSISTENT_SLEEP_STEP; i++) {
            int alreadyWrittenBytes = i*MAX_PERSIST_BUFFER;
            int leftBytes = totalSize - alreadyWrittenBytes;
            int currentSize = leftBytes < MAX_PERSIST_BUFFER ? leftBytes : MAX_PERSIST_BUFFER;
            
            persist_write_data(PERSISTENT_VALUES_KEY+offset+i, (data->minutes_value+i), currentSize);
            
            if (leftBytes < MAX_PERSIST_BUFFER)
                break;
        }
    }
    
}

void store_data(SleepData* data) {
    int recIndex = increse_current_persistent_index();
#ifdef DEBUG
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Store data with index %d", recIndex);
#endif
    store_data_with_index(data, recIndex);
}

// Dont forget to free the data after used
static SleepData* read_data(int recIndex) {
#ifdef DEBUG
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Read data with recIndex %d", recIndex);
#endif
    SleepData *data = malloc(sizeof(SleepData));
    int offset = recIndex * PERSISTENT_SLEEP_STEP;
    
    if (persist_exists(PERSISTENT_START_TIME_KEY+offset) &&
        persist_exists(PERSISTENT_END_TIME_KEY+offset) &&
        persist_exists(PERSISTENT_COUNT_KEY+offset)) {
        data->start_time = persist_read_int(PERSISTENT_START_TIME_KEY+offset);
        data->end_time = persist_read_int(PERSISTENT_END_TIME_KEY+offset);
        
        int32_t cnt_values = persist_read_int(PERSISTENT_COUNT_KEY+offset);
        data->count_values = cnt_values;
        int dci = 0;
        while (dci < PERSISTENT_SLEEP_STEP) {
            uint32_t key = PERSISTENT_VALUES_KEY+offset+dci;
            if (persist_exists(key)) {
                int size = persist_get_size(key);
                persist_read_data(key, (data->minutes_value+(dci*PERSISTENT_SLEEP_STEP)), size);
            } else {
                break;
            }
            dci++;
        }
    }
    return data;
}

// Dont forget to free the data after used
SleepData* read_last_data() {
    int recIndex = get_current_persistent_index();
#ifdef DEBUG
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Read last data with recIndex %d", recIndex);
#endif
    return read_data(recIndex);
}
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

#include <pebble_worker.h>
#include "constants.h"

static GlobalConfig config;
static AppTimer *timer;

static SleepData sleep_data;
static SleepPhases current_sleep_phase;

static bool alarm_in_motion = NO;
static bool snooze_active = NO;

static uint16_t motion_peek_in_min = 0;

const int ALARM_TIME_BETWEEN_ITERATIONS = 5000; // 5 sec
const int ALARM_MAX_ITERATIONS = 10; // Vibrate max 10 times

static int16_t last_x = 0;
static int16_t last_y = 0;
static int16_t last_z = 0;

// The interval we collect values
const int ACCEL_STEP_MS = 300;

const int DELTA = 0;
// For debugging purposes - this is the interval that current state is printed in console
const int REPORTING_STEP_MS = 20000;

#define DEEP_SLEEP_THRESHOLD 100
#define REM_SLEEP_THRESHOLD 101
#define LIGHT_THRESHOLD 800

// Start with this value down
#define START_PEEK_MOTION 1000

// The number of value types/thresholds
#define COUNT_TRESHOLDS 5

static int thresholds[COUNT_TRESHOLDS] = { 0, DEEP_SLEEP_THRESHOLD, REM_SLEEP_THRESHOLD, LIGHT_THRESHOLD, 65535 };
const uint8_t LAST_MIN_WAKE = 2;

int count_stat_data() {
    if (persist_exists(COUNT_STATS_KEY)) {
        return persist_read_int(COUNT_STATS_KEY);
    } else {
        persist_write_int(COUNT_STATS_KEY, 0);
        return 0;
    }
}

void store_data(SleepData* data) {

    // Prevent storing empty sleep data less than 5 min
    if (data->count_values <= 5)
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
    free(values);

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
    }

    if (csd < MAX_STAT_COUNT) {
        // Just add one more at the end
        persist_write_data(STAT_START + csd, new_stat, sizeof(StatData));
        persist_write_int(COUNT_STATS_KEY, csd + 1);
    } else {
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

void stop_sleep_data_capturing() {
    if (sleep_data.finished == false) {
        time_t temp;
        time(&temp);
        sleep_data.end_time = temp;
        sleep_data.finished = true;
        //
        // store_data(&sleep_data);
        // show_sleep_stats();
    }
}


void main_app_exec_alarm() {
    worker_launch_app();

    // Construct a data packet
    AppWorkerMessage msg_data = {
        .data0 = 0
    };

    // Send the data to the foreground app
    app_worker_send_message(WORKER_CMD_EXEC_ALARM, &msg_data);
}

void check_alarm() {
    if (alarm_in_motion)
        return;
    if (snooze_active)
        return;

    if (config.mode == MODE_WORKDAY) {
        time_t t1;
        time(&t1);
        struct tm *tt = localtime(&t1);
        int h = tt->tm_hour;
        int m = tt->tm_min;

        // We have active alarm
        // so check interval
        if (h >= config.start_wake_hour &&
            h <= config.end_wake_hour) {

            bool inTime = YES;
            if (h == config.start_wake_hour) {
                if (m >= config.start_wake_min)
                    inTime = YES;
                else
                    return;
            }

            if (h == config.end_wake_hour) {
                if (m <= config.end_wake_min)
                    inTime = YES;
                else
                    return;
            }

            if (inTime && current_sleep_phase == LIGHT) {
                main_app_exec_alarm();
                return;
            }
            // Check the last delta minutes
            uint8_t delta_time_h = config.end_wake_hour;
            uint8_t delta_time_m = config.end_wake_min;
            if (config.end_wake_min - LAST_MIN_WAKE < 0 ) {
                if (delta_time_h == 0)
                    delta_time_h = 23;
                delta_time_m = 60 + (config.end_wake_min - LAST_MIN_WAKE);
            } else {
                delta_time_m = config.end_wake_min - LAST_MIN_WAKE;
            }

            if (delta_time_h < h || (delta_time_h == h && delta_time_m < m)) {
                main_app_exec_alarm();
                return;
            }
        }
    }
}

static void calc_and_store_motion_value() {
    if (sleep_data.count_values >= MAX_COUNT-1)
        return;
    uint16_t prev_value = sleep_data.minutes_value[sleep_data.count_values];

    int med_val = abs(motion_peek_in_min - prev_value)/2;
    uint16_t median_peek = (motion_peek_in_min - prev_value) > 0
    ? prev_value + (med_val*((float)config.up_coef/10))
    : prev_value - (med_val*((float)config.down_coef/10));

    for (int i = 1; i < COUNT_TRESHOLDS; i++) {
        if (median_peek > thresholds[i-1] && median_peek <= thresholds[i]) {
            current_sleep_phase = i;
            break;
        }
    }
    sleep_data.stat[current_sleep_phase-1] += 1;

    sleep_data.count_values += 1;

    // Store modified motion data
    sleep_data.minutes_value[sleep_data.count_values] = median_peek;
    // APP_LOG(APP_LOG_LEVEL_DEBUG, "Tic-tac: %d %d ", median_peek, motion_peek_in_min);
    
    // Alternative - store original value
    //sleep_data.minutes_value[sleep_data.count_values] = motion_peek_in_min;
    motion_peek_in_min = 0;
}

void start_sleep_data_capturing() {
    time_t temp = time(NULL);
    sleep_data.start_time = temp;
    sleep_data.finished = false;

    for (int i = 0; i < COUNT_PHASES; i++)
        sleep_data.stat[i] = 0;

    //uint8_t *minutes_value;
    sleep_data.count_values = 0;
    sleep_data.minutes_value[sleep_data.count_values] = START_PEEK_MOTION;
    current_sleep_phase = AWAKE;
}


static void memo_motion(uint16_t peek) {
    if (peek > motion_peek_in_min)
        motion_peek_in_min = peek;
}

static void motion_timer_callback(void *data) {
    AccelData accel = (AccelData) { .x = 0, .y = 0, .z = 0 };
    int res = accel_service_peek(&accel);
    if (res == -1 || res == -2 || accel.did_vibrate) {
    	// When accel is not running or already subscribed
    	// Not interested in values from vibration
    	// Log 0 to keep the frequency
		memo_motion(0);
		return;
    }

    int16_t delta_x;
    int16_t delta_y;
    int16_t delta_z;

    if (last_x == 0 && last_y == 0 && last_z == 0) {
        delta_x = 0;
        delta_y = 0;
        delta_z = 0;
        // We don't know if there is a motion, when last values are initial
    } else {
        delta_x = abs(accel.x - last_x);
        delta_y = abs(accel.y - last_y);
        delta_z = abs(accel.z - last_z);

        // Don't take into account value that are less than delta
        if (delta_x < DELTA)
            delta_x = 0;

        if (delta_y < DELTA)
            delta_y = 0;

        if (delta_z < DELTA)
            delta_z = 0;

        uint16_t delta_value = (delta_x + delta_y + delta_z)/3;

        memo_motion(delta_value);
    }

    last_x = accel.x;
    last_y = accel.y;
    last_z = accel.z;

    timer = app_timer_register(ACCEL_STEP_MS, motion_timer_callback, NULL);
}

void persist_read_config() {
    persist_read_data(CONFIG_PERSISTENT_KEY, &config, sizeof(config));
    if (config.up_coef != UP_COEF_NOTSENSITIVE &&
        config.up_coef != UP_COEF_NORMAL &&
        config.up_coef != UP_COEF_VERYSENSITIVE) {
        config.up_coef = UP_COEF_NORMAL;
    }
    if (config.down_coef != DOWN_COEF_SLOW &&
        config.down_coef != DOWN_COEF_NORMAL &&
        config.down_coef != DOWN_COEF_FAST) {
        config.down_coef = DOWN_COEF_NORMAL;
    }
}
// Every minute
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
    persist_read_config(); // It might be changed from UI
    calc_and_store_motion_value();
    check_alarm();
}

static void pebslee_app_message_handler(uint16_t type, AppWorkerMessage *data) {
    if (type == APP_CMD_STOP_CAPTURING) {
        stop_sleep_data_capturing();
        store_data(&sleep_data);
    }
}

static void init() {
    // APP_LOG(APP_LOG_LEVEL_DEBUG, "Init worker");
    // Initialize your worker here
    persist_read_config();
    //app_worker_message_subscribe(pebslee_app_message_handler);
    motion_peek_in_min = 0;
    start_sleep_data_capturing();
    timer = app_timer_register(ACCEL_STEP_MS, motion_timer_callback, NULL);
    tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void deinit() {
    // APP_LOG(APP_LOG_LEVEL_DEBUG, "Deinit worker");
    // Deinitialize your worker here
    stop_sleep_data_capturing();
    store_data(&sleep_data);

    app_timer_cancel(timer);
    tick_timer_service_unsubscribe();
    //app_worker_message_unsubscribe();
}

int main(void) {
    init();
    worker_event_loop();
    deinit();
}

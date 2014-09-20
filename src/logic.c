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

#include <pebble.h>
#include "logic.h"
#include "sleep_window.h"

#define DEBUG 1

static uint8_t vib_count;
static bool alarm_in_motion = NO;
static AppTimer *alarm_timer;
const int ALARM_TIME_BETWEEN_ITERATIONS = 5000; // 3 sec
const int ALARM_MAX_ITERATIONS = 10;

static GlobalConfig config;
static uint16_t motion_peek_in_min = 0;
static int app_active = NO;
static int16_t last_x = 0;
static int16_t last_y = 0;
static int16_t last_z = 0;

const int ACCEL_STEP_MS = 300;
const int DELTA = 43;
const int REPORTING_STEP_MS = 2000;

const int DEEP_SLEEP_THRESHOLD = 130;
const int REM_SLEEP_THRESHOLD = 300;
const int LIGHT_THRESHOLD = 600;

const uint8_t LAST_MIN_WAKE = 2;

static SleepData sleep_data;

typedef enum {
    DEEP = 1,
    REM = 2,
    LIGHT = 3,
    AWAKE = 4
} SleepPhases;

static SleepPhases current_sleep_phase;

static AppTimer *timer;

#ifdef DEBUG
static AppTimer *timerRep;
#endif

GlobalConfig *get_config() {
	return &config;
}

void set_config_mode(int a_mode) {
    config.mode = a_mode;
}
void set_config_status(int a_status) {
    config.status = a_status;
}
void set_config_start_time(uint8_t a_hour, uint8_t a_min) {
    config.start_wake_hour = a_hour;
    config.start_wake_min = a_min;
}
void set_config_end_time(uint8_t a_hour, uint8_t a_min) {
    config.end_wake_hour = a_hour;
    config.end_wake_min = a_min;
}

void persist_write_config() {
    persist_write_data(CONFIG_PERSISTENT_KEY, &config, sizeof(config));
}
void persist_read_config() {
    persist_read_data(CONFIG_PERSISTENT_KEY, &config, sizeof(config));
}

void start_sleep_data_capturing() {
    time_t temp = time(NULL);
    sleep_data.start_time = localtime(&temp);
    sleep_data.finished = false;
    
    sleep_data.stat_deep_sleep_min = 0;
    sleep_data.stat_light_sleep_min = 0;
    sleep_data.stat_rem_sleep_min = 0;
    sleep_data.stat_awake_min = 0;
    
    //uint8_t *minutes_value;
    sleep_data.count_values = 0;
}

void stop_sleep_data_capturing() {
    time_t temp = time(NULL);
    sleep_data.end_time = localtime(&temp);
    sleep_data.finished = true;
    
#ifdef DEBUG
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Stop capturing: ");
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Started:  %02d:%02d", sleep_data.start_time->tm_hour, sleep_data.start_time->tm_min);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Finished: %02d:%02d", sleep_data.end_time->tm_hour, sleep_data.end_time->tm_min);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Deep  sleep:  %d min", sleep_data.stat_deep_sleep_min);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "REM   sleep:  %d min", sleep_data.stat_rem_sleep_min);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Light sleep:  %d min", sleep_data.stat_light_sleep_min);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Awake sleep:  %d min", sleep_data.stat_awake_min);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Count values: %d", sleep_data.count_values);
#endif
}

void increase_start_hour() {
    if (config.start_wake_hour == 23) {
        config.start_wake_hour = 0;
    } else {
        config.start_wake_hour = config.start_wake_hour + 1;
    }
}
void increase_start_min() {
    if (config.start_wake_min == 59) {
        config.start_wake_min = 0;
    } else {
        config.start_wake_min = config.start_wake_min + 1;
    }
    
}
void increase_end_hour() {
    if (config.end_wake_hour == 23) {
        config.end_wake_hour = 0;
    } else {
        config.end_wake_hour = config.end_wake_hour + 1;
    }
}
void increase_end_min() {
    if (config.end_wake_min == 59) {
        config.end_wake_min = 0;
    } else {
        config.end_wake_min = config.end_wake_min + 1;
    }
    
}

void decrease_start_hour() {
    if (config.start_wake_hour == 0) {
        config.start_wake_hour = 23;
    } else {
        config.start_wake_hour = config.start_wake_hour - 1;
    }
}
void decrease_start_min() {
    if (config.start_wake_min == 0) {
        config.start_wake_min = 59;
    } else {
        config.start_wake_min = config.start_wake_min - 1;
    }
    
}
void decrease_end_hour() {
    if (config.end_wake_hour == 0) {
        config.end_wake_hour = 23;
    } else {
        config.end_wake_hour = config.end_wake_hour - 1;
    }
}
void decrease_end_min() {
    if (config.end_wake_min == 0) {
        config.end_wake_min = 59;
    } else {
        config.end_wake_min = config.end_wake_min - 1;
    }
    
}

static void memo_motion(uint16_t peek) {
    if (peek > motion_peek_in_min)
        motion_peek_in_min = peek;
}


/*
 * Process motion data, and take the peeks of body motion
 */
static void motion_timer_callback(void *data) {
	AccelData accel = (AccelData ) { .x = 0, .y = 0, .z = 0 };
	accel_service_peek(&accel);
    
    // Not interested in values from vibration
    if (accel.did_vibrate) {
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
        delta_x = abs(abs(accel.x) - abs(last_x));
        delta_y = abs(abs(accel.y) - abs(last_y));
        delta_z = abs(abs(accel.z) - abs(last_z));
        
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

#ifdef DEBUG
static void reporting_timer_callback(void *data) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Motion vectors %u %d/%d/%d current phase: %d", motion_peek_in_min, last_x, last_y, last_z, current_sleep_phase);
    timerRep = app_timer_register(REPORTING_STEP_MS, reporting_timer_callback, NULL);
}
#endif

/*
 * Alarm timer loop
 */
static void recure_alarm() {
    if (vib_count >= ALARM_MAX_ITERATIONS) {
        alarm_in_motion = NO;
        return;
    }
    
    // Vibrate
    vibes_long_pulse();
    
    alarm_timer = app_timer_register(ALARM_TIME_BETWEEN_ITERATIONS, recure_alarm, NULL);
    
    vib_count++;
    
    if (vib_count % 3 == 0) {
        light_enable_interaction();
    }
}

static void execute_alarm() {
    stop_motion_capturing();
    app_active = NO;
    vib_count = 0;
    alarm_in_motion = YES;
    config.status = STATUS_NOTACTIVE;
    recure_alarm();
}

void call_stop_alarm_if_running() {
    if (alarm_in_motion) {
        app_timer_cancel(alarm_timer);
        alarm_in_motion = NO;
        refresh_display();
    }
}

static void check_alarm(struct tm *tick_time) {
     if (config.mode == MODE_WORKDAY) {
         // We have active alarm
         // so check interval
         if (tick_time->tm_hour >= config.start_wake_hour &&
             tick_time->tm_hour <= config.end_wake_hour) {
             bool inTime = YES;
             if (tick_time->tm_hour == config.start_wake_hour) {
                 if (tick_time->tm_min >= config.start_wake_min)
                     inTime = YES;
                 else
                     return;
             }
             if (tick_time->tm_hour == config.end_wake_hour) {
                 if (tick_time->tm_min <= config.end_wake_min)
                     inTime = YES;
                 else
                     return;
             }
             if (inTime && current_sleep_phase == LIGHT) {
                 execute_alarm();
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
             if (delta_time_h < tick_time->tm_hour || (delta_time_h == tick_time->tm_hour && delta_time_m < tick_time->tm_min)) {
                 execute_alarm();
                 return;
             }
          }
     }
}

static void persist_motion(struct tm *tick_time) {
#ifdef DEBUG
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Persist motion %u", motion_peek_in_min);
#endif
    if (motion_peek_in_min > DEEP_SLEEP_THRESHOLD) {
        if (motion_peek_in_min > REM_SLEEP_THRESHOLD) {
            if (motion_peek_in_min > LIGHT_THRESHOLD) {
                current_sleep_phase = AWAKE;
                sleep_data.stat_awake_min += 1;
            } else {
                current_sleep_phase = LIGHT;
                sleep_data.stat_light_sleep_min += 1;
            }
        } else {
            current_sleep_phase = REM;
            sleep_data.stat_rem_sleep_min += 1;
        }
    } else {
        current_sleep_phase = DEEP;
        sleep_data.stat_deep_sleep_min += 1;
    }
    
    if (sleep_data.count_values < MAX_COUNT)
        sleep_data.minutes_value[sleep_data.count_values] = motion_peek_in_min;
    
    sleep_data.count_values += 1;

    // TODO: Maybe store in DB or post in sync layer
    
    motion_peek_in_min = 0;
}

/*
 * Here is the main logic of the application. If active - every minute get/calculate and store
 * motion data, check for wake time and trigger alarm if necessary.
 * If not active - listen for configuration/communication events
 */
void minute_timer_tick() {
    if (app_active) {
        time_t temp = time(NULL);
        struct tm *tick_time = localtime(&temp);
        persist_motion(tick_time);
        check_alarm(tick_time);
    }
}

void notify_status_update(int a_status) {
    if (a_status == STATUS_ACTIVE) {
        vibes_short_pulse();
        start_motion_capturing();
        start_sleep_data_capturing();
        app_active = YES;
    } else if (a_status == STATUS_NOTACTIVE) {
        vibes_double_pulse();
        stop_motion_capturing();
        stop_sleep_data_capturing();
        app_active = NO;
    }
}

void notify_mode_update(int a_mode) {
    // if (a_mode == MODE_WORKDAY) {
    //   vibes_short_pulse();
    // } else if (a_mode == MODE_WEEKEND) {
    //   vibes_double_pulse();
    // }
}

void start_motion_capturing() {
    motion_peek_in_min = 0;
    timer = app_timer_register(ACCEL_STEP_MS, motion_timer_callback, NULL);
    
#ifdef DEBUG
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Start motion capturing");
    timerRep = app_timer_register(REPORTING_STEP_MS, reporting_timer_callback, NULL);
#endif
}

void stop_motion_capturing() {
#ifdef DEBUG
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Stop motion capturing");
    app_timer_cancel(timerRep);
#endif
    
    app_timer_cancel(timer);

}
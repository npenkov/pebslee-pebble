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
#include "sleep_stats.h"
#include "comm.h"
#include "persistence.h"
#include "localize.h"

static uint8_t vib_count;
static bool alarm_in_motion = NO;
static bool snooze_active = NO;
static AppTimer *alarm_timer;
static AppTimer *snooze_timer;

const int ALARM_TIME_BETWEEN_ITERATIONS = 5000; // 5 sec
const int ALARM_MAX_ITERATIONS = 10; // Vibrate max 10 times

static GlobalConfig config;

const int DELTA = 0;

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
void set_config_up_coef(int coef) {
    config.up_coef = coef;
}
void set_config_down_coef(int coef) {
    config.down_coef = coef;
}
void set_config_snooze(char snooze) {
    config.snooze  = snooze;
}

void set_config_active_profile(int profile) {
    config.active_profile = profile;
}

void persist_write_config() {
    D("Persist config with up/down : %d/%d", config.up_coef, config.down_coef);

    persist_write_data(CONFIG_PERSISTENT_KEY, &config, sizeof(config));
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
    D("Read config with up/down : %d/%d", config.up_coef, config.down_coef);
}

#ifdef DEBUG
static char* decode_phase(int a_phase) {
    switch (a_phase) {
        case DEEP:
            return _("Deep sleep");
        case REM:
            return _("REM sleep");
        case LIGHT:
            return _("Light sleep");
        case AWAKE:
            return _("Awake");
        default:
            return _("unknown");
            break;
    }
}

static void dump_current_state() {
    time_t t1 = sleep_data.start_time;
    struct tm *tt = localtime(&t1);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "* Started:      %02d:%02d", tt->tm_hour, tt->tm_min);
    for (int i = 0; i < COUNT_PHASES; i ++) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "* %s:  %d min", decode_phase(i+1), sleep_data.stat[i]);
    }
    APP_LOG(APP_LOG_LEVEL_DEBUG, "* Count values: %d", sleep_data.count_values);
}
#endif

void stop_sleep_data_capturing() {
    //show_sleep_stats();
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

/*
 * Alarm timer loop
 */
static void recure_alarm() {
    if (vib_count >= ALARM_MAX_ITERATIONS) {
        alarm_in_motion = NO;
        // Reschedule if no stop alarm and snooze is active
        if (config.snooze > 0) { // This should not be changed in the meanwhile
            snooze_active = YES;
            snooze_timer = app_timer_register(config.snooze * 1000 * 60, snooze_tick, NULL);
        }
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

void execute_alarm() {
    vib_count = 0;
    alarm_in_motion = YES;

    recure_alarm();
}

void snooze_tick() {
    execute_alarm();
}

bool is_alarm_running() {
    return alarm_in_motion == YES;
}

bool is_tracking_active() {
    return config.status == STATUS_ACTIVE;
}

bool is_snooze_active() {
    return snooze_active;
}

void stop_alarm_timer() {
    if (alarm_in_motion) {
        if (alarm_timer != NULL)
            app_timer_cancel(alarm_timer);
        alarm_in_motion = NO;
    }
}

void stop_snooze_timer() {
    if (snooze_active) {
        if (snooze_timer != NULL)
            app_timer_cancel(snooze_timer);
        snooze_active = NO;
    }
}


void ui_click(bool longClick) {
    if (longClick) {
        if (alarm_in_motion) {
            stop_alarm_timer();
            stop_snooze_timer();

            stop_motion_capturing();

            config.status = STATUS_NOTACTIVE;
            refresh_display();
            stop_sleep_data_capturing();
        }
    } else {
        if (alarm_in_motion) {
            // Check znooze
            if (snooze_active) { // Stop everything - as we have already snoozed
                stop_alarm_timer();

                // Start snooze timer
                snooze_active = YES;
                if (config.snooze > 0) { // This should not be changed in the meanwhile
                    snooze_timer = app_timer_register(config.snooze * 1000 * 60, snooze_tick, NULL);
                }
            } else { // activate snooze if set
                if (config.snooze > 0) {
                    stop_alarm_timer();

                    // Start snooze timer
                    snooze_active = YES;
                    snooze_timer = app_timer_register(config.snooze * 1000 * 60, snooze_tick, NULL);
                } else {
                    stop_alarm_timer();
                    stop_snooze_timer();

                    stop_motion_capturing();

                    config.status = STATUS_NOTACTIVE;
                    refresh_display();
                    stop_sleep_data_capturing();
                }
            }
        }
    }
}

void notify_status_update(int a_status) {
    if (a_status == STATUS_ACTIVE) {
        vibes_short_pulse();
        start_motion_capturing();

        snooze_active = NO;
    } else if (a_status == STATUS_NOTACTIVE) {
        vibes_double_pulse();
        stop_motion_capturing();

        if (snooze_active) {
            app_timer_cancel(snooze_timer);
        }
        snooze_active = NO;
        stop_sleep_data_capturing();
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
    AppWorkerResult result = app_worker_launch();
}

void stop_motion_capturing() {
    // AppWorkerMessage msg_data = {
    //     .data0 = 0
    // };
    // // Send the data to the background app
    // app_worker_send_message(APP_CMD_STOP_CAPTURING, &msg_data);

    // Stop the background worker
    AppWorkerResult result = app_worker_kill();
}

void freeLogic() {
//    if (timer != NULL) {
//        free(timer);
//    }
//#ifdef DEBUG
//    if (timerRep != NULL) {
//        free(timerRep);
//    }
//#endif
}

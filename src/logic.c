#include <pebble.h>
#include "logic.h"

static GlobalConfig config;
static uint16_t motion_peek_in_min = 0;
static int app_active = NO;
static int16_t last_x = 0;
static int16_t last_y = 0;
static int16_t last_z = 0;

const int ACCEL_STEP_MS = 2000;
const int DELTA = 43;

static AppTimer *timer;

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
    if (peek > motion_peek_in_min) motion_peek_in_min = peek;
}


/*
 * Process motion data, and take the peeks of body motion
 */
static void motion_timer_callback(void *data) {
	AccelData accel = (AccelData ) { .x = 0, .y = 0, .z = 0 };
	accel_service_peek(&accel);
    
    // Not interested in values from vibration
    if (accel.did_vibrate)
        return;
    int16_t delta_x = 0;
    int16_t delta_y = 0;
    int16_t delta_z = 0;
    
    if (abs(abs(accel.x) - abs(last_x)) > DELTA){
        delta_x = abs(abs(accel.x) - abs(last_x));
    } else {
        delta_x = 0;
    }
    
    if (abs(abs(accel.y) - abs(last_y)) > DELTA){
        delta_y = abs(abs(accel.y) - abs(last_y));
    } else {
        delta_y = 0;
    }
    
    if (abs(abs(accel.z) - abs(last_z)) > DELTA){
        delta_z = abs(abs(accel.z) - abs(last_z));
    } else {
        delta_z = 0;
    }
    uint16_t delta_value = (delta_x + delta_y + delta_z)/3;
    
//    APP_LOG(APP_LOG_LEVEL_DEBUG, "Motion vectors %u %d/%d/%d", delta_value, accel.x, accel.y, accel.z);
    memo_motion(delta_value);
    
    last_x = accel.x;
    last_y = accel.y;
    last_z = accel.z;
    
	timer = app_timer_register(ACCEL_STEP_MS, motion_timer_callback, NULL);
}

static void check_alarm(struct tm *tick_time) {
    
}

static void persist_motion(struct tm *tick_time) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Persist motion %u", motion_peek_in_min);
    // TODO: Do persist the value
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
        app_active = YES;
    } else if (a_status == STATUS_NOTACTIVE) {
        vibes_double_pulse();
        stop_motion_capturing();
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
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Start motion capturing");
    motion_peek_in_min = 0;
    timer = app_timer_register(ACCEL_STEP_MS, motion_timer_callback, NULL);
}

void stop_motion_capturing() {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Stop motion capturing");
    app_timer_cancel(timer);
}
#include <pebble.h>
#include "logic.h"

static GlobalConfig config;
static uint32_t motion_peek_in_min;
static bool motion_active = NO;
static bool app_active = NO;

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
 * Acceleration data comes in range -4000 <-> 4000 - so make it only with natural numbers
 */
static uint16_t naturalize_motion_value(int16_t val) {
	int16_t result = 4000 + val;
	if (result < 0)
		result = 0;
	return result;
}

/*
 * Babylonian method of square root
 */
static float sqrtBabylon(const float m)
{
    float i=0;
    float x1,x2;
    while( (i*i) <= m )
        i+=0.1f;
    x1=i;
    for(int j=0;j<10;j++)
    {
        x2=m;
        x2/=x1;
        x2+=x1;
        x2/=2;
        x1=x2;
    }
    return x2;
}

/*
 * Process motion data, and take the peeks of body motion
 */
static void motion_data_handler(AccelData *data, uint32_t num_samples) {
    
    uint16_t biggestValue = 0;
	AccelData *dt = data;
	for (uint32_t i = 0; i < num_samples; i++, dt++) {
        if (dt->did_vibrate) {
            // We don't want to store vibration values
            return;
        }
        uint32_t x = naturalize_motion_value(dt->x);
        uint32_t y = naturalize_motion_value(dt->y);
        uint32_t z = naturalize_motion_value(dt->z);
        
        float summed_squer = (float)(x * x + y * y + z * z);
        uint32_t absValue = (unsigned int) sqrtBabylon(summed_squer);
        if (absValue > biggestValue) biggestValue = absValue;
	}
    
    memo_motion(biggestValue);
}

static void start_motion_listener() {
    // Accelerometer
    accel_data_service_subscribe(NUMBER_OF_SAMPLES_PER_BATCH, motion_data_handler);
    accel_service_set_sampling_rate(ACCEL_SAMPLING_10HZ);
    motion_active = YES;
}

static void check_alarm(struct tm *tick_time) {
    
}


static void persist_motion(struct tm *tick_time) {
    
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

void notify_app_tracking_stopped() {
    app_active = NO;
    stop_services();
}

void notify_app_tracking_started() {
    app_active = YES;
    start_motion_listener();
}

void notify_status_update(int a_status) {
    if (a_status == STATUS_ACTIVE) {
        vibes_short_pulse();
        notify_app_tracking_started();
    } else if (a_status == STATUS_NOTACTIVE) {
        vibes_double_pulse();
        notify_app_tracking_stopped();
    }
}

void notify_mode_update(int a_mode) {
    // if (a_mode == MODE_WORKDAY) {
    //   vibes_short_pulse();
    // } else if (a_mode == MODE_WEEKEND) {
    //   vibes_double_pulse();
    // }
}

void stop_services() {
    if (motion_active) {
        accel_data_service_unsubscribe();
        motion_active = NO;
    }
}


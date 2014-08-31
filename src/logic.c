#include <pebble.h>
#include "logic.h"

static GlobalConfig config;

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



void notify_status_update(int a_status) {
  if (a_status == STATUS_ACTIVE) {
    vibes_short_pulse();    
  } else if (a_status == STATUS_NOTACTIVE) {
    vibes_double_pulse();  
  }
}

void notify_mode_update(int a_mode) {
  // if (a_mode == MODE_WORKDAY) {
  //   vibes_short_pulse();    
  // } else if (a_mode == MODE_WEEKEND) {
  //   vibes_double_pulse();  
  // }
}


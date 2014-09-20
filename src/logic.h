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

#pragma once

#ifndef LOGIC_H_
#define LOGIC_H_

#define MODE_WORKDAY 0
#define MODE_WEEKEND 1

#define STATUS_NOTACTIVE 0
#define STATUS_ACTIVE 1  

#define NO 0  
#define YES 1

#define CONFIG_PERSISTENT_KEY 0

// Store no more that 12 hours
#define MAX_COUNT 720

typedef struct {
    int mode;
    int status;
    
    uint8_t start_wake_hour;
    uint8_t start_wake_min;
    
    uint8_t end_wake_hour;
    uint8_t end_wake_min;

} GlobalConfig;

typedef struct {
    struct tm *start_time;
    struct tm *end_time;
    bool finished;
    
    uint8_t stat_deep_sleep_min;
    uint8_t stat_light_sleep_min;
    uint8_t stat_rem_sleep_min;
    uint8_t stat_awake_min;
    
    uint16_t minutes_value[MAX_COUNT];
    uint16_t count_values;
    
} SleepData;


void notify_status_update(int a_status);
void notify_mode_update(int a_mode);

void notify_app_tracking_stopped();
void notify_app_tracking_started();

GlobalConfig *get_config();
void set_config_mode(int a_mode);
void set_config_status(int a_status);
void set_config_start_time(uint8_t a_hour, uint8_t a_min);
void set_config_end_time(uint8_t a_hour, uint8_t a_min);

void persist_write_config();
void persist_read_config();

void increase_start_hour();
void increase_start_min();
void increase_end_hour();
void increase_end_min();

void decrease_start_hour();
void decrease_start_min();
void decrease_end_hour();
void decrease_end_min();

void minute_timer_tick();

void accel_data_handler(AccelData *data, uint32_t num_samples);
void start_motion_capturing();
void stop_motion_capturing();

void call_stop_alarm_if_running();

#endif /* LOGIC_H_ */  
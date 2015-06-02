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

#include "constants.h"

#pragma once

#ifndef LOGIC_H_
#define LOGIC_H_

//#define DEBUG

#ifdef DEBUG
#define D(...) APP_LOG(APP_LOG_LEVEL_DEBUG, __VA_ARGS__)
#else
#define D(...)
#endif


#define MODE_WORKDAY 0
#define MODE_WEEKEND 1

#define STATUS_NOTACTIVE 0
#define STATUS_ACTIVE 1

#define NO 0
#define YES 1

#define MAX_MEASURE_VALUE 5000

#define MEASURE_COEFICENT 255/5000;

#define UP_COEF_NOTSENSITIVE    10
#define UP_COEF_NORMAL          15
#define UP_COEF_VERYSENSITIVE   17

#define DOWN_COEF_SLOW      5
#define DOWN_COEF_NORMAL    7
#define DOWN_COEF_FAST      10

typedef struct {
    int mode;
    int status;

    uint8_t start_wake_hour;
    uint8_t start_wake_min;

    uint8_t end_wake_hour;
    uint8_t end_wake_min;

    int up_coef;
    int down_coef;

    char snooze;
} GlobalConfig;

#define PS_APP_TO_WATCH_COMMAND  1

#define PS_APP_TO_WATCH_START_TIME_HOUR 3
#define PS_APP_TO_WATCH_START_TIME_MINUTE 4
#define PS_APP_TO_WATCH_END_TIME_HOUR 5
#define PS_APP_TO_WATCH_END_TIME_MINUTE 6

#define PS_APP_MESSAGE_COMMAND_START_SYNC  21
#define PS_APP_MESSAGE_COMMAND_SET_TIME 22
#define PS_APP_MESSAGE_COMMAND_TOGGLE_SLEEP 23

#define PS_APP_MSG_HEADER_START 0
#define PS_APP_MSG_HEADER_END 1
#define PS_APP_MSG_HEADER_COUNT 2

typedef enum {
    DEEP = 1,
    REM = 2,
    LIGHT = 3,
    AWAKE = 4
} SleepPhases;

typedef struct {
    int countTuplets;
    int currentSendChunk;
    int sendChunkSize;
    uint32_t start_time;
    uint32_t end_time;
    uint16_t count_values;
    uint8_t *motionData;
} SendData;



void notify_status_update(int a_status);
void notify_mode_update(int a_mode);

void notify_app_tracking_stopped();
void notify_app_tracking_started();

GlobalConfig* get_config();
SleepData* get_sleep_data();

void set_config_mode(int a_mode);
void set_config_status(int a_status);
void set_config_start_time(uint8_t a_hour, uint8_t a_min);
void set_config_end_time(uint8_t a_hour, uint8_t a_min);
void set_config_up_coef(int coef);
void set_config_down_coef(int coef);
void set_config_snooze(char snooze);

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

void ui_click(bool longClick);

bool is_alarm_running();
bool is_tracking_active();
bool is_snooze_active();

void out_sent_handler(DictionaryIterator *sent, void *context);
void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context);
void in_received_handler(DictionaryIterator *received, void *context);
void in_dropped_handler(AppMessageResult reason, void *context);

void set_outbox_size(int outbox_size);
SleepData* read_last_data();
void freeLogic();
// This should not be here
void snooze_tick();
#endif /* LOGIC_H_ */

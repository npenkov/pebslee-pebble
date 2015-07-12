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

void notify_status_update(int a_status);
void notify_mode_update(int a_mode);

void notify_app_tracking_stopped();
void notify_app_tracking_started();

GlobalConfig* get_config();

void set_config_mode(int a_mode);
void set_config_status(int a_status);
void set_config_start_time(uint8_t a_hour, uint8_t a_min);
void set_config_end_time(uint8_t a_hour, uint8_t a_min);
void set_config_up_coef(int coef);
void set_config_down_coef(int coef);
void set_config_snooze(char snooze);
void set_config_active_profile(int profile);
void set_config_vibrate_on_change(char vibrate);

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
void execute_alarm();

#endif /* LOGIC_H_ */

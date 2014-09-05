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

// This is the number of samples that will be passed to calculation fuction at once
// Every sample is made with rithm of ACCEL_SAMPLING_10HZ - every 1/10 of second
#define NUMBER_OF_SAMPLES_PER_BATCH 20

typedef struct {
    int mode;
    int status;
    
    uint8_t start_wake_hour;
    uint8_t start_wake_min;
    
    uint8_t end_wake_hour;
    uint8_t end_wake_min;

} GlobalConfig;

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

void stop_services();

void minute_timer_tick();

#endif /* LOGIC_H_ */  
#pragma once

#ifndef LOGIC_H_
#define LOGIC_H_

#define MODE_WORKDAY 0
#define MODE_WEEKEND 1

#define STATUS_NOTACTIVE 0
#define STATUS_ACTIVE 1  

#define NO 0  
#define YES 1

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

GlobalConfig *get_config();
void set_config_mode(int a_mode);
void set_config_status(int a_status);
void set_config_start_time(uint8_t a_hour, uint8_t a_min);
void set_config_end_time(uint8_t a_hour, uint8_t a_min);

void increase_start_hour();
void increase_start_min();
void increase_end_hour();
void increase_end_min();

void decrease_start_hour();
void decrease_start_min();
void decrease_end_hour();
void decrease_end_min();

#endif /* LOGIC_H_ */  
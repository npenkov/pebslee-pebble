#pragma once

#ifndef LOGIC_H_
#define LOGIC_H_

#define MODE_WORKDAY 0
#define MODE_WEEKEND 1
  
#define STATUS_ACTIVE 1  
#define STATUS_NOTACTIVE 0
  
#define NO 0  
#define YES 1

void notify_status_update(int a_status);
void notify_mode_update(int a_mode);

static int mode = MODE_WORKDAY;
static int status = STATUS_NOTACTIVE;

static int start_wake_hour = 0;
static int start_wake_min = 0;

static int end_wake_hour = 0;
static int end_wake_min = 0;

#endif /* LOGIC_H_ */  
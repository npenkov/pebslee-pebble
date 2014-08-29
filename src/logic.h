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

#endif /* LOGIC_H_ */  
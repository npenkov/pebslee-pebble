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

#ifndef PebSlee_constants_h
#define PebSlee_constants_h

// Store no more that 12 hours
#define MAX_COUNT 720
#define COUNT_PHASES 4

#ifndef MIN
  #define MIN(a, b) ((a) < (b)) ? (a) : (b)
#endif
#ifndef MAX
  #define MAX(a, b) ((a) > (b)) ? (a) : (b)
#endif
#ifndef IN_RANGE
  #define IN_RANGE(x, a, b) ( (x) >= (a) && (x) <= (b) )
#endif

typedef struct {
    uint32_t start_time;
    uint32_t end_time;
    bool finished;

    uint16_t stat[COUNT_PHASES];

    uint16_t minutes_value[MAX_COUNT];
    uint16_t count_values;

} SleepData;


#define MODE_WORKDAY 0
#define MODE_WEEKEND 1

#define ACTIVE_PROFILE_NORMAL 0
#define ACTIVE_PROFILE_ALWAYS_ALARM 1
#define ACTIVE_PROFILE_NO_ALARM 2

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

    int active_profile;
} GlobalConfig;

#define PS_APP_TO_WATCH_COMMAND  1

#define PS_APP_TO_WATCH_START_TIME_HOUR 3
#define PS_APP_TO_WATCH_START_TIME_MINUTE 4
#define PS_APP_TO_WATCH_END_TIME_HOUR 5
#define PS_APP_TO_WATCH_END_TIME_MINUTE 6

#define PS_APP_MESSAGE_COMMAND_START_SYNC  21
#define PS_APP_MESSAGE_COMMAND_SET_TIME 22
#define PS_APP_MESSAGE_COMMAND_TOGGLE_SLEEP 23
#define PS_APP_MESSAGE_COMMAND_SET_SETTINGS 24

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

#define CONFIG_PERSISTENT_KEY 0

#define PERSISTENT_COUNT_KEY 1
#define PERSISTENT_VALUES_KEY 2
// use 2, 3, 4 - every each with 240 bytes

#define PERSISTENT_START_TIME_KEY 1
#define PERSISTENT_END_TIME_KEY 2
#define PERSISTENT_STATS_KEY 3

#define STAT_START 10
#define STAT_STEP 3

#define MAX_STAT_COUNT 10

// Steps are formed:
// STAT_START + (stat * STAT_STEP) + PERSISTENT_START_TIME_KEY
// STAT_START + (stat * STAT_STEP) + PERSISTENT_END_TIME_KEY
// STAT_START + (stat * STAT_STEP) + PERSISTENT_STATS_KEY

// Max persistem
#define COUNT_STATS_KEY 100
#define VERSION_KEY 254

#define MAX_PERSIST_BUFFER 240

typedef struct {
    uint32_t start_time;
    uint32_t end_time;
    uint16_t stat[COUNT_PHASES];
} StatData;

#define WORKER_CMD_EXEC_ALARM 0

#endif

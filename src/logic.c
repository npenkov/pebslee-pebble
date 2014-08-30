#include <pebble.h>
#include "logic.h"

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
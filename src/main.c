#include "pebble.h"
#include "logic.h"
#include "sleep_window.h"
  
static void handle_init(void) {
    accel_data_service_subscribe(0, NULL);
	show_sleep_window();
}

static void handle_deinit(void) {
    accel_data_service_unsubscribe();
    hide_sleep_window();
    
    if (get_config()->status == STATUS_ACTIVE) {
        set_config_status(STATUS_NOTACTIVE);
        notify_status_update(get_config()->status);
    }
}

int main(void) {
	handle_init();
	app_event_loop();
	handle_deinit();
}

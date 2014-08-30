#include "pebble.h"
#include "sleep_window.h"

static void handle_init(void) {
	show_sleep_window();
}

static void handle_deinit(void) {
    hide_sleep_window();
}
int main(void) {
	handle_init();
	app_event_loop();
	handle_deinit();
}

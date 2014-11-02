#include <pebble_worker.h>

static void init() {
    // Initialize your worker here
}

static void deinit() {
    // Deinitialize your worker here
}

int main(void) {
    init();
    worker_event_loop();
    deinit();
}

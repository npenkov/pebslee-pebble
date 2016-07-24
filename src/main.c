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

#include "pebble.h"
#include "logic.h"
#include "persistence.h"
#include "sleep_window.h"
#include "localize.h"

static void worker_message_handler(uint16_t type, AppWorkerMessage *data) {
    if (type == WORKER_CMD_EXEC_ALARM) {
        execute_alarm();
    }
}

static void focus_handler(bool in_focus) {
    // APP_LOG(APP_LOG_LEVEL_INFO, "App is %s in focus", in_focus ? "now" : "not");
}

static void handle_init(void) {
    // Migrate DB
    migrate_version();

    accel_data_service_subscribe(0, NULL);
	show_sleep_window();
    int inbox_size = app_message_inbox_size_maximum();
    int outbox_size = app_message_outbox_size_maximum();
    app_message_open(inbox_size, outbox_size);
    set_outbox_size(outbox_size);

    app_message_register_inbox_received(in_received_handler);
    app_message_register_inbox_dropped(in_dropped_handler);
    app_message_register_outbox_sent(out_sent_handler);
    app_message_register_outbox_failed(out_failed_handler);

    // Subscribe to Worker messages
    app_worker_message_subscribe(worker_message_handler);
    app_focus_service_subscribe(focus_handler);
}

static void handle_deinit(void) {
    accel_data_service_unsubscribe();
    hide_sleep_window();

    // We are already in background capturing the sleep

    // if (get_config()->status == STATUS_ACTIVE) {
    //     set_config_status(STATUS_NOTACTIVE);
    //     notify_status_update(get_config()->status);
    // }

    // No more worker updates
    app_worker_message_unsubscribe();

    freeLogic();
}

int main(void) {
    locale_init();
	handle_init();
	app_event_loop();
	handle_deinit();
}

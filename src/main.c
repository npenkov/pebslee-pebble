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

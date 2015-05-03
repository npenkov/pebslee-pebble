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

#include <stdio.h>
#include <pebble.h>

#include "syncprogress_window.h"
#include "localize.h"

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GBitmap *s_res_img_sync;
static BitmapLayer *s_bmp_sync;

static void initialise_ui(void) {
    s_window = window_create();
    window_set_background_color(s_window, GColorBlack);
    
#ifndef PBL_SDK_3    
    window_set_fullscreen(s_window, false);
#endif
    
    s_res_img_sync = gbitmap_create_with_resource(RESOURCE_ID_IMG_SYNC_PROGRESS);
    // s_bmp_sync
    s_bmp_sync = bitmap_layer_create(GRect(0, 0, 144, 168));
    bitmap_layer_set_bitmap(s_bmp_sync, s_res_img_sync);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_bmp_sync);
}

static void destroy_ui(void) {
    window_destroy(s_window);
    bitmap_layer_destroy(s_bmp_sync);
    gbitmap_destroy(s_res_img_sync);
}
// END AUTO-GENERATED UI CODE

static void handle_window_unload(Window* window) {
    destroy_ui();
}

void show_syncprogress_window(void) {
    initialise_ui();
    window_set_window_handlers(s_window, (WindowHandlers) {
        .unload = handle_window_unload,
    });
    window_stack_push(s_window, true);
}

void hide_syncprogress_window(void) {
    window_stack_remove(s_window, true);
}


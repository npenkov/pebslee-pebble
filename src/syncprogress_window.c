//
//  syncprogress_window.c
//  PebSlee
//
//  Created by Nikolai Penkov on 13/11/14.
//  Copyright (c) 2014 Nick Penkov. All rights reserved.
//

#include <stdio.h>
#include <pebble.h>

#include "syncprogress_window.h"

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GBitmap *s_res_img_sync;
static BitmapLayer *s_bmp_sync;

static void initialise_ui(void) {
    s_window = window_create();
    window_set_background_color(s_window, GColorBlack);
    window_set_fullscreen(s_window, true);
    
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


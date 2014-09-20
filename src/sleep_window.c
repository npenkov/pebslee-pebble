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

#include <pebble.h>
#include "sleep_window.h"
#include "logic.h"
#include "language.h"
#include "alarm_config.h"

// First time update date field
static int forceUpdateDate = YES;

static GBitmap *s_res_img_clock_white_22x25;


// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GFont s_res_bitham_42_bold;
static GFont s_res_roboto_condensed_21;
static GBitmap *s_res_img_empty_22x25;
static GBitmap *s_res_img_arrow_right_8x14;
static GBitmap *s_res_img_arrow_left_8x14;
static GBitmap *s_res_img_arrow_right_black_8x14;
static TextLayer *s_tl_time;
static TextLayer *s_tl_date;
static TextLayer *s_tl_status;
static TextLayer *s_tl_mode;
static TextLayer *s_textlayer_1;
static BitmapLayer *s_bm_clock;
static BitmapLayer *s_bm_up_right;
static BitmapLayer *s_bm_down_right;
static BitmapLayer *s_bm_back_left;
static BitmapLayer *s_bm_arrow_right_select;

static void initialise_ui(void) {
    s_window = window_create();
    window_set_background_color(s_window, GColorBlack);
    window_set_fullscreen(s_window, false);
    
    s_res_bitham_42_bold = fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD);
    s_res_roboto_condensed_21 = fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21);
    s_res_img_empty_22x25 = gbitmap_create_with_resource(RESOURCE_ID_IMG_EMPTY_22X25);
    s_res_img_arrow_right_8x14 = gbitmap_create_with_resource(RESOURCE_ID_IMG_ARROW_RIGHT_8X14);
    s_res_img_arrow_left_8x14 = gbitmap_create_with_resource(RESOURCE_ID_IMG_ARROW_LEFT_8X14);
    s_res_img_arrow_right_black_8x14 = gbitmap_create_with_resource(RESOURCE_ID_IMG_ARROW_RIGHT_BLACK_8X14);
    // s_tl_time
    s_tl_time = text_layer_create(GRect(0, 49, 144, 52));
    text_layer_set_text(s_tl_time, "00:00");
    text_layer_set_text_alignment(s_tl_time, GTextAlignmentCenter);
    text_layer_set_font(s_tl_time, s_res_bitham_42_bold);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_tl_time);
    
    // s_tl_date
    s_tl_date = text_layer_create(GRect(-1, 125, 144, 27));
    text_layer_set_text(s_tl_date, "Wed 30");
    text_layer_set_text_alignment(s_tl_date, GTextAlignmentCenter);
    text_layer_set_font(s_tl_date, s_res_roboto_condensed_21);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_tl_date);
    
    // s_tl_status
    s_tl_status = text_layer_create(GRect(-1, 98, 144, 26));
    text_layer_set_background_color(s_tl_status, GColorClear);
    text_layer_set_text_color(s_tl_status, GColorWhite);
    text_layer_set_text(s_tl_status, "not tracking");
    text_layer_set_text_alignment(s_tl_status, GTextAlignmentCenter);
    text_layer_set_font(s_tl_status, s_res_roboto_condensed_21);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_tl_status);
    
    // s_tl_mode
    s_tl_mode = text_layer_create(GRect(1, 21, 144, 28));
    text_layer_set_background_color(s_tl_mode, GColorClear);
    text_layer_set_text_color(s_tl_mode, GColorWhite);
    text_layer_set_text(s_tl_mode, "with alarm");
    text_layer_set_text_alignment(s_tl_mode, GTextAlignmentCenter);
    text_layer_set_font(s_tl_mode, s_res_roboto_condensed_21);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_tl_mode);
    
    // s_textlayer_1
    s_textlayer_1 = text_layer_create(GRect(20, 0, 123, 26));
    text_layer_set_background_color(s_textlayer_1, GColorClear);
    text_layer_set_text_color(s_textlayer_1, GColorWhite);
    text_layer_set_text(s_textlayer_1, "00:00 - 00:00");
    text_layer_set_text_alignment(s_textlayer_1, GTextAlignmentCenter);
    text_layer_set_font(s_textlayer_1, s_res_roboto_condensed_21);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_1);
    
    // s_bm_clock
    s_bm_clock = bitmap_layer_create(GRect(0, 0, 22, 26));
    bitmap_layer_set_bitmap(s_bm_clock, s_res_img_empty_22x25);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_bm_clock);
    
    // s_bm_up_right
    s_bm_up_right = bitmap_layer_create(GRect(135, 30, 8, 14));
    bitmap_layer_set_bitmap(s_bm_up_right, s_res_img_arrow_right_8x14);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_bm_up_right);
    
    // s_bm_down_right
    s_bm_down_right = bitmap_layer_create(GRect(135, 104, 8, 14));
    bitmap_layer_set_bitmap(s_bm_down_right, s_res_img_arrow_right_8x14);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_bm_down_right);
    
    // s_bm_back_left
    s_bm_back_left = bitmap_layer_create(GRect(2, 30, 8, 14));
    bitmap_layer_set_bitmap(s_bm_back_left, s_res_img_arrow_left_8x14);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_bm_back_left);
    
    // s_bm_arrow_right_select
    s_bm_arrow_right_select = bitmap_layer_create(GRect(135, 69, 8, 12));
    bitmap_layer_set_bitmap(s_bm_arrow_right_select, s_res_img_arrow_right_black_8x14);
    bitmap_layer_set_background_color(s_bm_arrow_right_select, GColorWhite);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_bm_arrow_right_select);
}

static void destroy_ui(void) {
    window_destroy(s_window);
    text_layer_destroy(s_tl_time);
    text_layer_destroy(s_tl_date);
    text_layer_destroy(s_tl_status);
    text_layer_destroy(s_tl_mode);
    text_layer_destroy(s_textlayer_1);
    bitmap_layer_destroy(s_bm_clock);
    bitmap_layer_destroy(s_bm_up_right);
    bitmap_layer_destroy(s_bm_down_right);
    bitmap_layer_destroy(s_bm_back_left);
    bitmap_layer_destroy(s_bm_arrow_right_select);
    gbitmap_destroy(s_res_img_empty_22x25);
    gbitmap_destroy(s_res_img_arrow_right_8x14);
    gbitmap_destroy(s_res_img_arrow_left_8x14);
    gbitmap_destroy(s_res_img_arrow_right_black_8x14);
}
// END AUTO-GENERATED UI CODE

// *********************** Update UI fuctions *********************
static void update_mode() {
    if (get_config()->mode == MODE_WEEKEND) {
        text_layer_set_text(s_tl_mode, MODE_WEEKEND_STR);
        bitmap_layer_set_bitmap(s_bm_clock, s_res_img_empty_22x25);
        text_layer_set_text(s_textlayer_1, "");
    } else if (get_config()->mode == MODE_WORKDAY) {
        text_layer_set_text(s_tl_mode, MODE_WORKDAY_STR);
        bitmap_layer_set_bitmap(s_bm_clock, s_res_img_clock_white_22x25);
        static char buffer[] = "00:00 - 00:00";
        snprintf(buffer, sizeof(buffer), "%02d:%02d - %02d:%02d", get_config()->start_wake_hour, get_config()->start_wake_min, get_config()->end_wake_hour, get_config()->end_wake_min);
        text_layer_set_text(s_textlayer_1, buffer);
    }
}

static void update_status() {
    if (get_config()->status == STATUS_ACTIVE) {
        text_layer_set_text(s_tl_status, STATUS_ACTIVE_STR);
    } else if (get_config()->status == STATUS_NOTACTIVE) {
        text_layer_set_text(s_tl_status, STATUS_NOTACTIVE_STR);
    }
}

static void update_time() {
    // Get a tm structure
    time_t temp = time(NULL);
    struct tm *tick_time = localtime(&temp);
    
    // Create a long-lived buffer
    static char buffer[] = "00:00";
    static char bufferDate[] = "Mon 00";
    
    // Write the current hours and minutes into the buffer
    if(clock_is_24h_style() == true) {
        //Use 2h hour format
        strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
    } else {
        //Use 12 hour format
        strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
    }
    
    
    // Display this time on the TextLayer
    text_layer_set_text(s_tl_time, buffer);
    // Update date only when it becomes 00:00
    if ((tick_time->tm_hour == 0 && tick_time->tm_min == 0) || forceUpdateDate ) {
        strftime(bufferDate, sizeof(bufferDate), "%a %d", tick_time);
        text_layer_set_text(s_tl_date, bufferDate);
        forceUpdateDate = NO;
    }
}

// *********************** Calculation functions **************************
static void calculate_mode() {
    // Get a tm structure
    time_t temp = time(NULL);
    struct tm *tick_time = localtime(&temp);
    
    if ((tick_time->tm_wday == 5 && tick_time->tm_hour >= 13) ||   // Friday evening
        (tick_time->tm_wday == 6 && tick_time->tm_hour >= 13) ||   // Saturday
        (tick_time->tm_wday == 0 && tick_time->tm_hour <= 12))     // Sunday morning
        set_config_mode(MODE_WEEKEND);
    else
        set_config_mode(MODE_WORKDAY);
}


// *********************** Window and click handlers ***********************
static void handle_window_unload(Window* window) {
    destroy_ui();
}

static void handle_window_appear(Window* window) {
    persist_write_config();
    update_mode();
}

/*
 * This should be called every minute
 */
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
    update_time();
    minute_timer_tick();
}

static void up_long_click_handler(ClickRecognizerRef recognizer, void *context) {
    // Here we can show a window with allowed fuctions or just a status
    call_stop_alarm_if_running();
}

static void up_long_click_release_handler(ClickRecognizerRef recognizer, void *context) {
    if (get_config()->mode == MODE_WEEKEND) {
        set_config_mode(MODE_WORKDAY);
    } else {
        set_config_mode(MODE_WEEKEND);
    }
    notify_mode_update(get_config()->mode);
    update_mode();
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
    call_stop_alarm_if_running();
    show_alarm_config();
}

static void down_long_click_handler(ClickRecognizerRef recognizer, void *context) {
    // Here we can show a window with allowed fuctions or just a status
    call_stop_alarm_if_running();
}

static void down_long_click_release_handler(ClickRecognizerRef recognizer, void *context) {
    if (get_config()->status == STATUS_ACTIVE) {
        set_config_status(STATUS_NOTACTIVE);
    } else {
        set_config_status(STATUS_ACTIVE);
    }
    notify_status_update(get_config()->status);
    update_status();
}


static void config_provider(void *context) {
    window_long_click_subscribe(BUTTON_ID_UP, 700, up_long_click_handler, up_long_click_release_handler);
    window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
    window_long_click_subscribe(BUTTON_ID_DOWN, 700, down_long_click_handler, down_long_click_release_handler);
}

void show_sleep_window(void) {
    s_res_img_clock_white_22x25 = gbitmap_create_with_resource(RESOURCE_ID_IMG_CLOCK_WHITE_22X25);
    persist_read_config();
    get_config()->status = STATUS_NOTACTIVE;
    
    initialise_ui();
    
    window_set_window_handlers(s_window, (WindowHandlers) {
        .unload = handle_window_unload,
        .appear = handle_window_appear
    });
    window_stack_push(s_window, true);
    if (forceUpdateDate) {
        calculate_mode();
    }
    update_mode();
    update_time();
    update_status();

    // Change time every minute
    tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
    window_set_click_config_provider(s_window, config_provider);
}

void hide_sleep_window(void) {
    window_stack_remove(s_window, true);
}

void refresh_display(void) {
    update_mode();
    update_time();
    update_status();
}

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
#include "alarm_config.h"
#include "action_menu.h"
#include "localize.h"

// First time update date field
static int forceUpdateDate = YES;

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GFont s_res_bitham_42_bold;
static GFont s_res_roboto_condensed_21;
static GBitmap *s_res_img_empty_22x25;
static GBitmap *s_res_img_arrow_right_8x14;
static GBitmap *s_res_img_arrow_left_8x14;
static GBitmap *s_res_img_arrow_right_black_8x14;
static GBitmap *s_res_img_clock_white_22x25;
static TextLayer *s_tl_time;
static TextLayer *s_tl_date;
static TextLayer *s_tl_status;
#if defined(PBL_RECT)
static TextLayer *s_textlayer_1;
static TextLayer *s_tl_mode;
static BitmapLayer *s_bm_clock;
static BitmapLayer *s_bm_up_right;
static BitmapLayer *s_bm_down_right;
static BitmapLayer *s_bm_back_left;
static BitmapLayer *s_bm_arrow_right_select;
#elif defined(PBL_ROUND)
static TextLayer *s_textlayer_1;
#endif

static void initialise_ui(void) {
    s_window = window_create();
    window_set_background_color(s_window, GColorBlack);

#ifndef PBL_SDK_3
    window_set_fullscreen(s_window, false);
#endif

    Layer *window_layer = window_get_root_layer(s_window);
    GRect b = layer_get_bounds(window_layer);

    s_res_bitham_42_bold = fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD);
    s_res_roboto_condensed_21 = fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21);
    s_res_img_empty_22x25 = gbitmap_create_with_resource(RESOURCE_ID_IMG_EMPTY_22X25);
    s_res_img_arrow_right_8x14 = gbitmap_create_with_resource(RESOURCE_ID_IMG_ARROW_RIGHT_8X14);
    s_res_img_arrow_left_8x14 = gbitmap_create_with_resource(RESOURCE_ID_IMG_ARROW_LEFT_8X14);
    s_res_img_arrow_right_black_8x14 = gbitmap_create_with_resource(RESOURCE_ID_IMG_ARROW_RIGHT_BLACK_8X14);
    // s_tl_time
    s_tl_time = text_layer_create(GRect(0, 49, b.size.w, 52));
    text_layer_set_text(s_tl_time, "00:00");
    text_layer_set_text_alignment(s_tl_time, GTextAlignmentCenter);
    text_layer_set_font(s_tl_time, s_res_bitham_42_bold);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_tl_time);

    // s_tl_status
    s_tl_status = text_layer_create(GRect(0, 126, b.size.w, 26));
    text_layer_set_background_color(s_tl_status, GColorClear);
    text_layer_set_text_color(s_tl_status, GColorWhite);
    text_layer_set_text(s_tl_status, _("not tracking"));
    text_layer_set_text_alignment(s_tl_status, GTextAlignmentCenter);
    text_layer_set_font(s_tl_status, s_res_roboto_condensed_21);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_tl_status);

    // s_tl_date
    s_tl_date = text_layer_create(GRect(0, 98, b.size.w, 27));
    text_layer_set_text(s_tl_date, "Wed 30");
    text_layer_set_text_alignment(s_tl_date, GTextAlignmentCenter);
    text_layer_set_font(s_tl_date, s_res_roboto_condensed_21);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_tl_date);

#if defined(PBL_RECT)    
    // s_bm_clock
    s_bm_clock = bitmap_layer_create(GRect(0, 0, 22, 26));
    bitmap_layer_set_bitmap(s_bm_clock, s_res_img_empty_22x25);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_bm_clock);

    // s_textlayer_1
    s_textlayer_1 = text_layer_create(GRect(20, 0, 123, 26));
    text_layer_set_background_color(s_textlayer_1, GColorClear);
    text_layer_set_text_color(s_textlayer_1, GColorWhite);
    text_layer_set_text(s_textlayer_1, "00:00 - 00:00");
    text_layer_set_text_alignment(s_textlayer_1, GTextAlignmentCenter);
    text_layer_set_font(s_textlayer_1, s_res_roboto_condensed_21);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_1);

    // s_tl_mode
    s_tl_mode = text_layer_create(GRect(1, 21, b.size.w, 28));
    text_layer_set_background_color(s_tl_mode, GColorClear);
    text_layer_set_text_color(s_tl_mode, GColorWhite);
    text_layer_set_text(s_tl_mode, _("with alarm"));
    text_layer_set_text_alignment(s_tl_mode, GTextAlignmentCenter);
    text_layer_set_font(s_tl_mode, s_res_roboto_condensed_21);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_tl_mode);
    // s_bm_up_right
    s_bm_up_right = bitmap_layer_create(GRect(b.size.w - 9, 30, 8, 14));
    bitmap_layer_set_bitmap(s_bm_up_right, s_res_img_arrow_right_8x14);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_bm_up_right);

    // s_bm_down_right
    s_bm_down_right = bitmap_layer_create(GRect(b.size.w - 9, 132, 8, 14));
    bitmap_layer_set_bitmap(s_bm_down_right, s_res_img_arrow_right_8x14);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_bm_down_right);

    // s_bm_back_left
    s_bm_back_left = bitmap_layer_create(GRect(2, 30, 8, 14));
    bitmap_layer_set_bitmap(s_bm_back_left, s_res_img_arrow_left_8x14);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_bm_back_left);

    // s_bm_arrow_right_select
    s_bm_arrow_right_select = bitmap_layer_create(GRect(b.size.w - 9, 69, 8, 12));
    bitmap_layer_set_bitmap(s_bm_arrow_right_select, s_res_img_arrow_right_black_8x14);
    bitmap_layer_set_background_color(s_bm_arrow_right_select, GColorWhite);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_bm_arrow_right_select);
#elif defined(PBL_ROUND)
    s_textlayer_1 = text_layer_create(GRect(0, 21, b.size.w, 26));
    text_layer_set_background_color(s_textlayer_1, GColorClear);
    text_layer_set_text_color(s_textlayer_1, GColorWhite);
    text_layer_set_text(s_textlayer_1, "00:00 - 00:00");
    text_layer_set_text_alignment(s_textlayer_1, GTextAlignmentCenter);
    text_layer_set_font(s_textlayer_1, s_res_roboto_condensed_21);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_1);    
#endif
    
}

static void destroy_ui(void) {
    window_destroy(s_window);
    text_layer_destroy(s_tl_time);
    text_layer_destroy(s_tl_date);
    text_layer_destroy(s_tl_status);

#if defined(PBL_RECT)   
    bitmap_layer_destroy(s_bm_clock);
    text_layer_destroy(s_tl_mode);
    text_layer_destroy(s_textlayer_1);

    bitmap_layer_destroy(s_bm_up_right);
    bitmap_layer_destroy(s_bm_down_right);
    bitmap_layer_destroy(s_bm_back_left);
    bitmap_layer_destroy(s_bm_arrow_right_select);
#elif defined(PBL_ROUND)
    text_layer_destroy(s_textlayer_1);
#endif    

    gbitmap_destroy(s_res_img_empty_22x25);
    gbitmap_destroy(s_res_img_arrow_right_8x14);
    gbitmap_destroy(s_res_img_arrow_left_8x14);
    gbitmap_destroy(s_res_img_arrow_right_black_8x14);
    gbitmap_destroy(s_res_img_clock_white_22x25);
}
// END AUTO-GENERATED UI CODE

// *********************** Update UI fuctions *********************
static void update_mode() {
#if defined(PBL_RECT)       
    if (get_config()->mode == MODE_WEEKEND) {
        text_layer_set_text(s_tl_mode, _("no alarm"));
        bitmap_layer_set_bitmap(s_bm_clock, s_res_img_empty_22x25);
        text_layer_set_text(s_textlayer_1, "");
    } else if (get_config()->mode == MODE_WORKDAY) {
        text_layer_set_text(s_tl_mode, _("with alarm"));
        bitmap_layer_set_bitmap(s_bm_clock, s_res_img_clock_white_22x25);
        static char buffer[] = "00:00 - 00:00";
        snprintf(buffer, sizeof(buffer), "%02d:%02d - %02d:%02d", get_config()->start_wake_hour, get_config()->start_wake_min, get_config()->end_wake_hour, get_config()->end_wake_min);
        text_layer_set_text(s_textlayer_1, buffer);
    }
#elif defined(PBL_ROUND)   
    if (get_config()->mode == MODE_WEEKEND) {
        text_layer_set_text(s_textlayer_1, _("no alarm"));
    } else if (get_config()->mode == MODE_WORKDAY) {
        static char buffer[] = "00:00 - 00:00";
        snprintf(buffer, sizeof(buffer), "%02d:%02d - %02d:%02d", get_config()->start_wake_hour, get_config()->start_wake_min, get_config()->end_wake_hour, get_config()->end_wake_min);
        text_layer_set_text(s_textlayer_1, buffer);
    }
#endif   
}

static void update_status() {
    if (get_config()->status == STATUS_ACTIVE) {
        text_layer_set_text(s_tl_status, _("track sleep"));
    } else if (get_config()->status == STATUS_NOTACTIVE) {
        text_layer_set_text(s_tl_status, _("not tracking"));
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
    int prof = get_config()->active_profile;
    if (prof == ACTIVE_PROFILE_NORMAL) {

        if ((tick_time->tm_wday == 5 && tick_time->tm_hour >= 13) ||   // Friday evening
            (tick_time->tm_wday == 6 && tick_time->tm_hour >= 13) ||   // Saturday
            (tick_time->tm_wday == 0 && tick_time->tm_hour <= 12))     // Sunday morning
            set_config_mode(MODE_WEEKEND);
        else
            set_config_mode(MODE_WORKDAY);
    } else if (prof == ACTIVE_PROFILE_NO_ALARM) {
        set_config_mode(MODE_WEEKEND);
    } else if (prof == ACTIVE_PROFILE_ALWAYS_ALARM) {
        set_config_mode(MODE_WORKDAY);
    } else {
        D("Undetermined active profile %d", prof);
    }
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
}

/*
 * ==================== Click Handlers ================== 
 */
// Long clicks

static void up_long_click_handler(ClickRecognizerRef recognizer, void *context) {
}

static void up_long_click_release_handler(ClickRecognizerRef recognizer, void *context) {
    if (is_alarm_running()) {
        ui_click(YES);
    } else {
        if (get_config()->mode == MODE_WEEKEND) {
            set_config_mode(MODE_WORKDAY);
        } else {
            set_config_mode(MODE_WEEKEND);
        }
        notify_mode_update(get_config()->mode);
        update_mode();
    }
}

static void down_long_click_handler(ClickRecognizerRef recognizer, void *context) {
}

static void down_long_click_release_handler(ClickRecognizerRef recognizer, void *context) {
    if (is_alarm_running()) {
        ui_click(YES);
    } else {
        if (get_config()->status == STATUS_ACTIVE) {
            set_config_status(STATUS_NOTACTIVE);
        } else {
            set_config_status(STATUS_ACTIVE);
        }
        notify_status_update(get_config()->status);
        update_status();
    }
}


static void select_long_click_handler(ClickRecognizerRef recognizer, void *context) {
}

static void select_long_click_release_handler(ClickRecognizerRef recognizer, void *context) {
    if (!is_alarm_running()) {
        show_action_menu();
    } else {
        ui_click(YES);
    }
}

// BACK
static void back_long_click_handler(ClickRecognizerRef recognizer, void *context) {
}

static void back_long_click_release_handler(ClickRecognizerRef recognizer, void *context) {
    hide_sleep_window();
}

// Short clicks

static void back_click_handler(ClickRecognizerRef recognizer, void *context) {
    hide_sleep_window();
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
    ui_click(NO);
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
    if (!is_alarm_running()) {
        show_alarm_config();
    } else {
        ui_click(NO);
    }
}


static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
    ui_click(NO);
}


static void config_provider(void *context) {
    // BACK button
    window_long_click_subscribe(BUTTON_ID_BACK, 700, back_long_click_handler, back_long_click_release_handler);
    window_single_click_subscribe(BUTTON_ID_BACK, back_click_handler);

    // UP button
    window_long_click_subscribe(BUTTON_ID_UP, 700, up_long_click_handler, up_long_click_release_handler);
    window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);

    // SELECT/middle button
    window_long_click_subscribe(BUTTON_ID_SELECT, 700, select_long_click_handler, select_long_click_release_handler);
    window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);

    // DOWN button
    window_long_click_subscribe(BUTTON_ID_DOWN, 700, down_long_click_handler, down_long_click_release_handler);
    window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

void show_sleep_window(void) {
    s_res_img_clock_white_22x25 = gbitmap_create_with_resource(RESOURCE_ID_IMG_CLOCK_WHITE_22X25);
    persist_read_config();
    // Check to see if the worker is currently active
    bool worker_is_running = app_worker_is_running();
    if (worker_is_running) {
        get_config()->status = STATUS_ACTIVE;
    } else {
        get_config()->status = STATUS_NOTACTIVE;
    }

    initialise_ui();

    window_set_window_handlers(s_window, (WindowHandlers) {
        .unload = handle_window_unload,
        .appear = handle_window_appear
    });
    window_stack_push(s_window, true);
    if (forceUpdateDate && !worker_is_running) {
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

void toggle_sleep(void) {
    if (get_config()->status == STATUS_ACTIVE) {
        set_config_status(STATUS_NOTACTIVE);
    } else {
        set_config_status(STATUS_ACTIVE);
    }
    notify_status_update(get_config()->status);
    update_status();
}

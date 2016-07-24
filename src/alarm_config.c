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
#include "alarm_config.h"
#include "logic.h"
#include "localize.h"

#define NONE_SELECTED 0
#define START_HOUR_SELECTED 1
#define START_MIN_SELECTED 2
#define END_HOUR_SELECTED 3
#define END_MIN_SELECTED 4

static int current_selection = NONE_SELECTED;

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GFont s_res_bitham_30_black;
static GBitmap *s_res_img_up_arrow_black_8x14;
static GBitmap *s_res_img_down_arrow_black_8x14;
static TextLayer *s_start_hour;
static TextLayer *s_start_min;
static TextLayer *s_start_sep;
static TextLayer *s_end_hour;
static TextLayer *s_end_min;
static TextLayer *s_end_sep;
static BitmapLayer *s_bm_st_h_up;
static BitmapLayer *s_bm_st_h_down;
static BitmapLayer *s_bm_st_m_up;
static BitmapLayer *s_bm_st_m_down;
static BitmapLayer *s_bm_end_h_down;
static BitmapLayer *s_bm_end_m_down;
static BitmapLayer *s_bm_end_h_up;
static BitmapLayer *s_bm_end_m_up;

static void initialise_ui(void) {
    s_window = window_create();

#ifndef PBL_SDK_3
    window_set_fullscreen(s_window, false);
#endif

    Layer *window_layer = window_get_root_layer(s_window);
    GRect b = layer_get_bounds(window_layer);
    int wd = 0;
    int hd = 0;

#if defined(PBL_ROUND)
    wd = (b.size.w - 144)/2;
    hd = (b.size.h - 152)/2;
#endif

    s_res_bitham_30_black = fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK);
    s_res_img_up_arrow_black_8x14 = gbitmap_create_with_resource(RESOURCE_ID_IMG_UP_ARROW_BLACK_8X14);
    s_res_img_down_arrow_black_8x14 = gbitmap_create_with_resource(RESOURCE_ID_IMG_DOWN_ARROW_BLACK_8X14);
    // s_start_hour
    s_start_hour = text_layer_create(GRect(21+wd, 27+hd, 44, 38));
    text_layer_set_text(s_start_hour, "00");
    text_layer_set_text_alignment(s_start_hour, GTextAlignmentRight);
    text_layer_set_font(s_start_hour, s_res_bitham_30_black);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_start_hour);

    // s_start_min
    s_start_min = text_layer_create(GRect(82+wd, 27+hd, 44, 38));
    text_layer_set_text(s_start_min, "00");
    text_layer_set_text_alignment(s_start_min, GTextAlignmentRight);
    text_layer_set_font(s_start_min, s_res_bitham_30_black);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_start_min);

    // s_end_hour
    s_end_hour = text_layer_create(GRect(21+wd, 87+hd, 44, 38));
    text_layer_set_text(s_end_hour, "00");
    text_layer_set_text_alignment(s_end_hour, GTextAlignmentRight);
    text_layer_set_font(s_end_hour, s_res_bitham_30_black);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_end_hour);

    // s_end_min
    s_end_min = text_layer_create(GRect(82+wd, 87+hd, 44, 38));
    text_layer_set_text(s_end_min, "00");
    text_layer_set_text_alignment(s_end_min, GTextAlignmentRight);
    text_layer_set_font(s_end_min, s_res_bitham_30_black);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_end_min);

    // s_start_sep
    s_start_sep = text_layer_create(GRect(69+wd, 24+hd, 10, 38));
    text_layer_set_text(s_start_sep, ":");
    text_layer_set_font(s_start_sep, s_res_bitham_30_black);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_start_sep);

    // s_end_sep
    s_end_sep = text_layer_create(GRect(69+wd, 85+hd, 10, 38));
    text_layer_set_text(s_end_sep, ":");
    text_layer_set_font(s_end_sep, s_res_bitham_30_black);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_end_sep);

    // s_bm_st_h_up
    s_bm_st_h_up = bitmap_layer_create(GRect(36+wd, 19+hd, 14, 8));
    bitmap_layer_set_bitmap(s_bm_st_h_up, s_res_img_up_arrow_black_8x14);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_bm_st_h_up);

    // s_bm_st_h_down
    s_bm_st_h_down = bitmap_layer_create(GRect(36+wd, 64+hd, 14, 8));
    bitmap_layer_set_bitmap(s_bm_st_h_down, s_res_img_down_arrow_black_8x14);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_bm_st_h_down);

    // s_bm_st_m_up
    s_bm_st_m_up = bitmap_layer_create(GRect(96+wd, 19+hd, 14, 8));
    bitmap_layer_set_bitmap(s_bm_st_m_up, s_res_img_up_arrow_black_8x14);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_bm_st_m_up);

    // s_bm_st_m_down
    s_bm_st_m_down = bitmap_layer_create(GRect(96+wd, 64+hd, 14, 8));
    bitmap_layer_set_bitmap(s_bm_st_m_down, s_res_img_down_arrow_black_8x14);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_bm_st_m_down);

    // s_bm_end_h_down
    s_bm_end_h_down = bitmap_layer_create(GRect(36+wd, 124+hd, 14, 12));
    bitmap_layer_set_bitmap(s_bm_end_h_down, s_res_img_down_arrow_black_8x14);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_bm_end_h_down);

    // s_bm_end_m_down
    s_bm_end_m_down = bitmap_layer_create(GRect(96+wd, 124+hd, 14, 8));
    bitmap_layer_set_bitmap(s_bm_end_m_down, s_res_img_down_arrow_black_8x14);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_bm_end_m_down);

    // s_bm_end_h_up
    s_bm_end_h_up = bitmap_layer_create(GRect(36+wd, 79+hd, 14, 8));
    bitmap_layer_set_bitmap(s_bm_end_h_up, s_res_img_up_arrow_black_8x14);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_bm_end_h_up);

    // s_bm_end_m_up
    s_bm_end_m_up = bitmap_layer_create(GRect(96+wd, 80+hd, 14, 8));
    bitmap_layer_set_bitmap(s_bm_end_m_up, s_res_img_up_arrow_black_8x14);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_bm_end_m_up);
}

static void destroy_ui(void) {
    window_destroy(s_window);

    text_layer_destroy(s_start_hour);
    text_layer_destroy(s_start_min);
    text_layer_destroy(s_end_hour);
    text_layer_destroy(s_end_min);
    text_layer_destroy(s_start_sep);
    text_layer_destroy(s_end_sep);
    bitmap_layer_destroy(s_bm_st_h_up);
    bitmap_layer_destroy(s_bm_st_h_down);
    bitmap_layer_destroy(s_bm_st_m_up);
    bitmap_layer_destroy(s_bm_st_m_down);
    bitmap_layer_destroy(s_bm_end_h_down);
    bitmap_layer_destroy(s_bm_end_m_down);
    bitmap_layer_destroy(s_bm_end_h_up);
    bitmap_layer_destroy(s_bm_end_m_up);
    gbitmap_destroy(s_res_img_up_arrow_black_8x14);
    gbitmap_destroy(s_res_img_down_arrow_black_8x14);
}
// END AUTO-GENERATED UI CODE

static void handle_window_unload(Window* window) {
    destroy_ui();
}

static void set_layer_white_text(TextLayer *a_text_layer) {
    text_layer_set_text_color(a_text_layer, GColorWhite);
    text_layer_set_background_color(a_text_layer, GColorBlack);
}

static void set_layer_black_text(TextLayer *a_text_layer) {
    text_layer_set_text_color(a_text_layer, GColorBlack);
    text_layer_set_background_color(a_text_layer, GColorClear);
}

static void set_start_hour() {
    static char bufferH1[] = "00";
    snprintf(bufferH1, sizeof(bufferH1), "%02d", get_config()->start_wake_hour);
    text_layer_set_text(s_start_hour, bufferH1);
}

static void set_start_min() {
    static char bufferM1[] = "00";
    snprintf(bufferM1, sizeof(bufferM1), "%02d", get_config()->start_wake_min);
    text_layer_set_text(s_start_min, bufferM1);
}

static void set_end_hour() {
    static char bufferH2[] = "00";
    snprintf(bufferH2, sizeof(bufferH2), "%02d", get_config()->end_wake_hour);
    text_layer_set_text(s_end_hour, bufferH2);
}

static void set_end_min() {
    static char bufferM2[] = "00";
    snprintf(bufferM2, sizeof(bufferM2), "%02d", get_config()->end_wake_min);
    text_layer_set_text(s_end_min, bufferM2);
}


static void update_ui() {
    set_start_hour();
    set_start_min();
    set_end_hour();
    set_end_min();

    if (current_selection == START_HOUR_SELECTED) {
        set_layer_white_text(s_start_hour);
        set_layer_black_text(s_start_min);
        set_layer_black_text(s_end_hour);
        set_layer_black_text(s_end_min);
    } else if (current_selection == START_MIN_SELECTED) {
        set_layer_black_text(s_start_hour);
        set_layer_white_text(s_start_min);
        set_layer_black_text(s_end_hour);
        set_layer_black_text(s_end_min);
    } else if (current_selection == END_HOUR_SELECTED) {
        set_layer_black_text(s_start_hour);
        set_layer_black_text(s_start_min);
        set_layer_white_text(s_end_hour);
        set_layer_black_text(s_end_min);
    } else if (current_selection == END_MIN_SELECTED) {
        set_layer_black_text(s_start_hour);
        set_layer_black_text(s_start_min);
        set_layer_black_text(s_end_hour);
        set_layer_white_text(s_end_min);
    } else {
        set_layer_black_text(s_start_hour);
        set_layer_black_text(s_start_min);
        set_layer_black_text(s_end_hour);
        set_layer_black_text(s_end_min);
    }
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
    if (current_selection == START_HOUR_SELECTED) {
        increase_start_hour();
        set_start_hour();
    } else if (current_selection == START_MIN_SELECTED) {
        increase_start_min();
        set_start_min();
    } else if (current_selection == END_HOUR_SELECTED) {
        increase_end_hour();
        set_end_hour();
    } else if (current_selection == END_MIN_SELECTED) {
        increase_end_min();
        set_end_min();
    }
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
    if (current_selection == START_HOUR_SELECTED) {
        decrease_start_hour();
        set_start_hour();
    } else if (current_selection == START_MIN_SELECTED) {
        decrease_start_min();
        set_start_min();
    } else if (current_selection == END_HOUR_SELECTED) {
        decrease_end_hour();
        set_end_hour();
    } else if (current_selection == END_MIN_SELECTED) {
        decrease_end_min();
        set_end_min();
    }
}

static void up_repeat_click_handler(ClickRecognizerRef recognizer, void *context) {
    bool is_repeating = click_recognizer_is_repeating(recognizer);
    uint8_t click_count = click_number_of_clicks_counted(recognizer);
    if (!is_repeating) {
        up_click_handler(recognizer, context);
        return;
    }

    if (current_selection == START_HOUR_SELECTED) {
        increase_start_hour();
        set_start_hour();
    } else if (current_selection == START_MIN_SELECTED) {
        for (int i = 0; i < 5; i++)
            increase_start_min();
        set_start_min();
    } else if (current_selection == END_HOUR_SELECTED) {
        increase_end_hour();
        set_end_hour();
    } else if (current_selection == END_MIN_SELECTED) {
        for (int i = 0; i < 5; i++)
            increase_end_min();
        set_end_min();
    }
}

static void down_repeat_click_handler(ClickRecognizerRef recognizer, void *context) {
    bool is_repeating = click_recognizer_is_repeating(recognizer);
    uint8_t click_count = click_number_of_clicks_counted(recognizer);
    if (!is_repeating) {
        down_click_handler(recognizer, context);
        return;
    }

    if (current_selection == START_HOUR_SELECTED) {
        decrease_start_hour();
        set_start_hour();
    } else if (current_selection == START_MIN_SELECTED) {
        for (int i = 0; i < 5; i++)
            decrease_start_min();
        set_start_min();
    } else if (current_selection == END_HOUR_SELECTED) {
        decrease_end_hour();
        set_end_hour();
    } else if (current_selection == END_MIN_SELECTED) {
        for (int i = 0; i < 5; i++)
            decrease_end_min();
        set_end_min();
    }
}

static void update_end_time_if_necessery() {
    uint8_t starth = get_config()->start_wake_hour;
    uint8_t startm = get_config()->start_wake_min;
    uint8_t endh = get_config()->end_wake_hour;
    uint8_t endm = get_config()->end_wake_min;

    if (starth > endh) {
        endh = starth;
        endm = startm + 1;
        set_config_end_time(endh, endm);
        set_end_hour();
        set_end_min();
    } else {
        if (startm > endm) {
            endm = startm + 1;
            set_config_end_time(endh, endm);
            set_end_min();
        }
    }

}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
    if (current_selection == START_HOUR_SELECTED) {
        current_selection = START_MIN_SELECTED;
        set_layer_black_text(s_start_hour);
        set_layer_white_text(s_start_min);
    } else if (current_selection == START_MIN_SELECTED) {
        current_selection = END_HOUR_SELECTED;

        update_end_time_if_necessery();

        set_layer_black_text(s_start_min);
        set_layer_white_text(s_end_hour);
    } else if (current_selection == END_HOUR_SELECTED) {
        current_selection = END_MIN_SELECTED;
        set_layer_black_text(s_end_hour);
        set_layer_white_text(s_end_min);
    } else if (current_selection == END_MIN_SELECTED) {
        current_selection = NONE_SELECTED;
        set_layer_black_text(s_end_min);
    } else {
        current_selection = START_HOUR_SELECTED;
        set_layer_white_text(s_start_hour);
    }
}

static void back_click_handler(ClickRecognizerRef recognizer, void *context) {
    persist_write_config();
    window_stack_remove(s_window, true);
}

static void config_provider(void *context) {
    // window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
    window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
    // window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
    window_single_click_subscribe(BUTTON_ID_BACK, back_click_handler);

    uint16_t repeat_interval_ms = 600;  // Fire every 600 ms while held down

    window_single_repeating_click_subscribe(BUTTON_ID_DOWN, repeat_interval_ms, down_repeat_click_handler);
    window_single_repeating_click_subscribe(BUTTON_ID_UP, repeat_interval_ms, up_repeat_click_handler);
}

void show_alarm_config(void) {
    initialise_ui();
    window_set_window_handlers(s_window, (WindowHandlers) {
        .unload = handle_window_unload,
    });
    window_set_click_config_provider(s_window, config_provider);
    update_ui();
    window_stack_push(s_window, true);
}

void hide_alarm_config(void) {
    persist_write_config();
    window_stack_remove(s_window, true);
}

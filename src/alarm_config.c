#include <pebble.h>
#include "alarm_config.h"
#include "logic.h"

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
    window_set_fullscreen(s_window, false);
    
    s_res_bitham_30_black = fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK);
    s_res_img_up_arrow_black_8x14 = gbitmap_create_with_resource(RESOURCE_ID_IMG_UP_ARROW_BLACK_8X14);
    s_res_img_down_arrow_black_8x14 = gbitmap_create_with_resource(RESOURCE_ID_IMG_DOWN_ARROW_BLACK_8X14);
    // s_start_hour
    s_start_hour = text_layer_create(GRect(21, 27, 44, 38));
    text_layer_set_text(s_start_hour, "00");
    text_layer_set_text_alignment(s_start_hour, GTextAlignmentRight);
    text_layer_set_font(s_start_hour, s_res_bitham_30_black);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_start_hour);
    
    // s_start_min
    s_start_min = text_layer_create(GRect(82, 27, 44, 38));
    text_layer_set_text(s_start_min, "00");
    text_layer_set_text_alignment(s_start_min, GTextAlignmentRight);
    text_layer_set_font(s_start_min, s_res_bitham_30_black);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_start_min);
    
    // s_end_hour
    s_end_hour = text_layer_create(GRect(21, 87, 44, 38));
    text_layer_set_text(s_end_hour, "00");
    text_layer_set_text_alignment(s_end_hour, GTextAlignmentRight);
    text_layer_set_font(s_end_hour, s_res_bitham_30_black);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_end_hour);
    
    // s_end_min
    s_end_min = text_layer_create(GRect(82, 87, 44, 38));
    text_layer_set_text(s_end_min, "00");
    text_layer_set_text_alignment(s_end_min, GTextAlignmentRight);
    text_layer_set_font(s_end_min, s_res_bitham_30_black);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_end_min);
    
    // s_start_sep
    s_start_sep = text_layer_create(GRect(69, 24, 10, 38));
    text_layer_set_text(s_start_sep, ":");
    text_layer_set_font(s_start_sep, s_res_bitham_30_black);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_start_sep);
    
    // s_end_sep
    s_end_sep = text_layer_create(GRect(69, 85, 10, 38));
    text_layer_set_text(s_end_sep, ":");
    text_layer_set_font(s_end_sep, s_res_bitham_30_black);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_end_sep);
    
    // s_bm_st_h_up
    s_bm_st_h_up = bitmap_layer_create(GRect(36, 19, 14, 8));
    bitmap_layer_set_bitmap(s_bm_st_h_up, s_res_img_up_arrow_black_8x14);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_bm_st_h_up);
    
    // s_bm_st_h_down
    s_bm_st_h_down = bitmap_layer_create(GRect(36, 64, 14, 8));
    bitmap_layer_set_bitmap(s_bm_st_h_down, s_res_img_down_arrow_black_8x14);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_bm_st_h_down);
    
    // s_bm_st_m_up
    s_bm_st_m_up = bitmap_layer_create(GRect(96, 19, 14, 8));
    bitmap_layer_set_bitmap(s_bm_st_m_up, s_res_img_up_arrow_black_8x14);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_bm_st_m_up);
    
    // s_bm_st_m_down
    s_bm_st_m_down = bitmap_layer_create(GRect(96, 64, 14, 8));
    bitmap_layer_set_bitmap(s_bm_st_m_down, s_res_img_down_arrow_black_8x14);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_bm_st_m_down);
    
    // s_bm_end_h_down
    s_bm_end_h_down = bitmap_layer_create(GRect(36, 124, 14, 12));
    bitmap_layer_set_bitmap(s_bm_end_h_down, s_res_img_down_arrow_black_8x14);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_bm_end_h_down);
    
    // s_bm_end_m_down
    s_bm_end_m_down = bitmap_layer_create(GRect(96, 124, 14, 8));
    bitmap_layer_set_bitmap(s_bm_end_m_down, s_res_img_down_arrow_black_8x14);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_bm_end_m_down);
    
    // s_bm_end_h_up
    s_bm_end_h_up = bitmap_layer_create(GRect(36, 79, 14, 8));
    bitmap_layer_set_bitmap(s_bm_end_h_up, s_res_img_up_arrow_black_8x14);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_bm_end_h_up);
    
    // s_bm_end_m_up
    s_bm_end_m_up = bitmap_layer_create(GRect(96, 80, 14, 8));
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

static void set_number(TextLayer *a_text_layer, uint8_t number) {
    char *buffer;
    buffer = malloc(sizeof(char)*(2+1));
    snprintf(buffer, sizeof(buffer), "%02d", number);
    text_layer_set_text(a_text_layer, buffer);
}

static void update_ui() {
    set_number(s_start_hour, get_config()->start_wake_hour);
    set_number(s_start_min, get_config()->start_wake_min);
    set_number(s_end_hour, get_config()->end_wake_hour);
    set_number(s_end_min, get_config()->end_wake_min);
    
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
    APP_LOG(APP_LOG_LEVEL_DEBUG, "up click handler: current selection: %d %d:%d %d:%d", current_selection, get_config()->start_wake_hour, get_config()->start_wake_min, get_config()->end_wake_hour, get_config()->end_wake_min);
    if (current_selection == START_HOUR_SELECTED) {
        increase_start_hour();
        set_number(s_start_hour, get_config()->start_wake_hour);
    } else if (current_selection == START_MIN_SELECTED) {
        increase_start_min();
        set_number(s_start_min, get_config()->start_wake_min);
    } else if (current_selection == END_HOUR_SELECTED) {
        increase_end_hour();
        set_number(s_end_hour, get_config()->end_wake_hour);
    } else if (current_selection == END_MIN_SELECTED) {
        increase_end_min();
        set_number(s_end_min, get_config()->end_wake_min);
    }
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "down click handler: current selection: %d %d:%d %d:%d", current_selection, get_config()->start_wake_hour, get_config()->start_wake_min, get_config()->end_wake_hour, get_config()->end_wake_min);
    if (current_selection == START_HOUR_SELECTED) {
        decrease_start_hour();
        set_number(s_start_hour, get_config()->start_wake_hour);
    } else if (current_selection == START_MIN_SELECTED) {
        decrease_start_min();
        set_number(s_start_min, get_config()->start_wake_min);
    } else if (current_selection == END_HOUR_SELECTED) {
        decrease_end_hour();
        set_number(s_end_hour, get_config()->end_wake_hour);
    } else if (current_selection == END_MIN_SELECTED) {
        decrease_end_min();
        set_number(s_end_min, get_config()->end_wake_min);
    }
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
    if (current_selection == START_HOUR_SELECTED) {
        current_selection = START_MIN_SELECTED;
        set_layer_black_text(s_start_hour);
        set_layer_white_text(s_start_min);
    } else if (current_selection == START_MIN_SELECTED) {
        current_selection = END_HOUR_SELECTED;
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
    window_stack_remove(s_window, true);
}

static void config_provider(void *context) {
    window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
    window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
    window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
    window_single_click_subscribe(BUTTON_ID_BACK, back_click_handler);
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
    window_stack_remove(s_window, true);
}

#include "sleep_stats.h"
#include <pebble.h>
#include "logic.h"

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GFont s_res_roboto_condensed_21;
static TextLayer *s_label_awake;
static TextLayer *s_val_awake;
static TextLayer *s_label_light;
static TextLayer *s_val_light;
static TextLayer *s_label_deep;
static TextLayer *s_val_deep;
static TextLayer *s_label_total;
static TextLayer *s_val_total;

static void initialise_ui(void) {
    s_window = window_create();
    window_set_background_color(s_window, GColorWhite);
    window_set_fullscreen(s_window, false);
    
    s_res_roboto_condensed_21 = fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21);
    // s_label_awake
    s_label_awake = text_layer_create(GRect(3, 3, 80, 30));
    text_layer_set_background_color(s_label_awake, GColorClear);
    text_layer_set_text_color(s_label_awake, GColorBlack);
    text_layer_set_text(s_label_awake, "Awake:");
    text_layer_set_font(s_label_awake, s_res_roboto_condensed_21);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_label_awake);
    
    // s_val_awake
    s_val_awake = text_layer_create(GRect(90, 3, 50, 30));
    text_layer_set_background_color(s_val_awake, GColorClear);
    text_layer_set_text_color(s_val_awake, GColorBlack);
    text_layer_set_text(s_val_awake, "00:00");
    text_layer_set_font(s_val_awake, s_res_roboto_condensed_21);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_val_awake);
    
    // s_label_light
    s_label_light = text_layer_create(GRect(3, 43, 80, 30));
    text_layer_set_background_color(s_label_light, GColorClear);
    text_layer_set_text_color(s_label_light, GColorBlack);
    text_layer_set_text(s_label_light, "Light:");
    text_layer_set_font(s_label_light, s_res_roboto_condensed_21);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_label_light);
    
    // s_val_light
    s_val_light = text_layer_create(GRect(90, 43, 50, 30));
    text_layer_set_background_color(s_val_light, GColorClear);
    text_layer_set_text_color(s_val_light, GColorBlack);
    text_layer_set_text(s_val_light, "00:00");
    text_layer_set_font(s_val_light, s_res_roboto_condensed_21);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_val_light);
    
    // s_label_deep
    s_label_deep = text_layer_create(GRect(3, 83, 80, 30));
    text_layer_set_background_color(s_label_deep, GColorClear);
    text_layer_set_text_color(s_label_deep, GColorBlack);
    text_layer_set_text(s_label_deep, "Deep:");
    text_layer_set_font(s_label_deep, s_res_roboto_condensed_21);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_label_deep);
    
    // s_val_deep
    s_val_deep = text_layer_create(GRect(90, 83, 50, 30));
    text_layer_set_background_color(s_val_deep, GColorClear);
    text_layer_set_text_color(s_val_deep, GColorBlack);
    text_layer_set_text(s_val_deep, "00:00");
    text_layer_set_font(s_val_deep, s_res_roboto_condensed_21);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_val_deep);
    
    // s_label_total
    s_label_total = text_layer_create(GRect(3, 123, 80, 30));
    text_layer_set_background_color(s_label_total, GColorClear);
    text_layer_set_text_color(s_label_total, GColorBlack);
    text_layer_set_text(s_label_total, "TOTAL:");
    text_layer_set_font(s_label_total, s_res_roboto_condensed_21);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_label_total);
    
    // s_val_total
    s_val_total = text_layer_create(GRect(90, 123, 50, 30));
    text_layer_set_background_color(s_val_total, GColorClear);
    text_layer_set_text_color(s_val_total, GColorBlack);
    text_layer_set_text(s_val_total, "00:00");
    text_layer_set_font(s_val_total, s_res_roboto_condensed_21);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_val_total);
}

static void destroy_ui(void) {
    window_destroy(s_window);
    text_layer_destroy(s_label_awake);
    text_layer_destroy(s_val_awake);
    text_layer_destroy(s_label_light);
    text_layer_destroy(s_val_light);
    text_layer_destroy(s_label_deep);
    text_layer_destroy(s_val_deep);
    text_layer_destroy(s_label_total);
    text_layer_destroy(s_val_total);
}
// END AUTO-GENERATED UI CODE

static int get_hours(uint16_t min) {
    int ret = 0;
    ret = min/60;
    return ret;
}

static int get_minutes(uint16_t min) {
    int ret = 0;
    ret = min%60;
    return ret;
}


static void update_ui_stat_values() {
    SleepData *sleep_data = get_sleep_data();
//    for (int i = 0; i < COUNT_PHASES; i++)
//        sleep_data.stat[i] = 0;
    char *buffer1;
    buffer1 = malloc(sizeof(char)*(2));
    snprintf(buffer1, sizeof(buffer1), "%02d", sleep_data->stat[AWAKE-1]);
    text_layer_set_text(s_val_awake, buffer1);

    char *buffer2;
    buffer2 = malloc(sizeof(char)*(2));
    snprintf(buffer2, sizeof(buffer2), "%02d", sleep_data->stat[LIGHT-1]);
    text_layer_set_text(s_val_light, buffer2);
    
    char *buffer3;
    buffer3 = malloc(sizeof(char)*(2));
    snprintf(buffer3, sizeof(buffer3), "%02d", sleep_data->stat[DEEP-1]);
    text_layer_set_text(s_val_deep, buffer3);
    
    char *buffer4;
    buffer4 = malloc(sizeof(char)*(2));
    uint16_t total = sleep_data->stat[AWAKE-1] + sleep_data->stat[LIGHT-1] + sleep_data->stat[DEEP-1];
    snprintf(buffer4, sizeof(buffer4), "%02d", total);
    text_layer_set_text(s_val_total, buffer4);
}

static void handle_window_unload(Window* window) {
    destroy_ui();
}

static void back_click_handler(ClickRecognizerRef recognizer, void *context) {
    window_stack_remove(s_window, true);
}

static void config_provider(void *context) {
    window_single_click_subscribe(BUTTON_ID_UP, back_click_handler);
    window_single_click_subscribe(BUTTON_ID_SELECT, back_click_handler);
    window_single_click_subscribe(BUTTON_ID_DOWN, back_click_handler);
    window_single_click_subscribe(BUTTON_ID_BACK, back_click_handler);
}

void show_sleep_stats(void) {
    initialise_ui();
    window_set_window_handlers(s_window, (WindowHandlers) {
        .unload = handle_window_unload,
    });
    window_set_click_config_provider(s_window, config_provider);
    update_ui_stat_values();
    window_stack_push(s_window, true);
}

void hide_sleep_stats(void) {
    window_stack_remove(s_window, true);
}

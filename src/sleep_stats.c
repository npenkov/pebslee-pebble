#include "sleep_stats.h"
#include <pebble.h>

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
  window_set_background_color(s_window, GColorBlack);
  window_set_fullscreen(s_window, false);
  
  s_res_roboto_condensed_21 = fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21);
  // s_label_awake
  s_label_awake = text_layer_create(GRect(3, 3, 80, 30));
  text_layer_set_background_color(s_label_awake, GColorClear);
  text_layer_set_text_color(s_label_awake, GColorWhite);
  text_layer_set_text(s_label_awake, "Awake:");
  text_layer_set_font(s_label_awake, s_res_roboto_condensed_21);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_label_awake);
  
  // s_val_awake
  s_val_awake = text_layer_create(GRect(90, 3, 50, 30));
  text_layer_set_background_color(s_val_awake, GColorClear);
  text_layer_set_text_color(s_val_awake, GColorWhite);
  text_layer_set_text(s_val_awake, "00:00");
  text_layer_set_font(s_val_awake, s_res_roboto_condensed_21);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_val_awake);
  
  // s_label_light
  s_label_light = text_layer_create(GRect(3, 43, 80, 30));
  text_layer_set_background_color(s_label_light, GColorClear);
  text_layer_set_text_color(s_label_light, GColorWhite);
  text_layer_set_text(s_label_light, "Light:");
  text_layer_set_font(s_label_light, s_res_roboto_condensed_21);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_label_light);
  
  // s_val_light
  s_val_light = text_layer_create(GRect(90, 43, 50, 30));
  text_layer_set_background_color(s_val_light, GColorClear);
  text_layer_set_text_color(s_val_light, GColorWhite);
  text_layer_set_text(s_val_light, "00:00");
  text_layer_set_font(s_val_light, s_res_roboto_condensed_21);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_val_light);
  
  // s_label_deep
  s_label_deep = text_layer_create(GRect(3, 83, 80, 30));
  text_layer_set_background_color(s_label_deep, GColorClear);
  text_layer_set_text_color(s_label_deep, GColorWhite);
  text_layer_set_text(s_label_deep, "Deep:");
  text_layer_set_font(s_label_deep, s_res_roboto_condensed_21);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_label_deep);
  
  // s_val_deep
  s_val_deep = text_layer_create(GRect(90, 83, 50, 30));
  text_layer_set_background_color(s_val_deep, GColorClear);
  text_layer_set_text_color(s_val_deep, GColorWhite);
  text_layer_set_text(s_val_deep, "00:00");
  text_layer_set_font(s_val_deep, s_res_roboto_condensed_21);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_val_deep);
  
  // s_label_total
  s_label_total = text_layer_create(GRect(3, 123, 80, 30));
  text_layer_set_background_color(s_label_total, GColorClear);
  text_layer_set_text_color(s_label_total, GColorWhite);
  text_layer_set_text(s_label_total, "TOTAL:");
  text_layer_set_font(s_label_total, s_res_roboto_condensed_21);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_label_total);
  
  // s_val_total
  s_val_total = text_layer_create(GRect(90, 123, 50, 30));
  text_layer_set_background_color(s_val_total, GColorClear);
  text_layer_set_text_color(s_val_total, GColorWhite);
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

static void handle_window_unload(Window* window) {
  destroy_ui();
}

void show_sleep_stats(void) {
  initialise_ui();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_stack_push(s_window, true);
}

void hide_sleep_stats(void) {
  window_stack_remove(s_window, true);
}

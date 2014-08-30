#include "alarm_config.h"
#include <pebble.h>

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GFont s_res_bitham_30_black;
static GBitmap *s_res_img_up_arrow_black_8x14;
static GBitmap *s_res_img_down_arrow_black_8x14;
static TextLayer *s_hour_start;
static TextLayer *s_min_start;
static TextLayer *s_end_hour;
static TextLayer *s_end_min;
static TextLayer *s_sep_start;
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
  // s_hour_start
  s_hour_start = text_layer_create(GRect(21, 27, 44, 38));
  text_layer_set_text(s_hour_start, "00");
  text_layer_set_text_alignment(s_hour_start, GTextAlignmentRight);
  text_layer_set_font(s_hour_start, s_res_bitham_30_black);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_hour_start);
  
  // s_min_start
  s_min_start = text_layer_create(GRect(82, 27, 44, 38));
  text_layer_set_text(s_min_start, "00");
  text_layer_set_text_alignment(s_min_start, GTextAlignmentRight);
  text_layer_set_font(s_min_start, s_res_bitham_30_black);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_min_start);
  
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
  
  // s_sep_start
  s_sep_start = text_layer_create(GRect(69, 24, 10, 38));
  text_layer_set_text(s_sep_start, ":");
  text_layer_set_font(s_sep_start, s_res_bitham_30_black);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_sep_start);
  
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
  text_layer_destroy(s_hour_start);
  text_layer_destroy(s_min_start);
  text_layer_destroy(s_end_hour);
  text_layer_destroy(s_end_min);
  text_layer_destroy(s_sep_start);
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

void show_alarm_config(void) {
  initialise_ui();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_stack_push(s_window, true);
}

void hide_alarm_config(void) {
  window_stack_remove(s_window, true);
}

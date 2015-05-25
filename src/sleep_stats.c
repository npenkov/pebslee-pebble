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

#include "sleep_stats.h"
#include <pebble.h>
#include "logic.h"
#include "persistence.h"
#include "localize.h"

static int current_index = 0;
static int count_recs = 0;
static StatData *stats_data;

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GFont s_res_gothic_18_bold;
static GBitmap *s_res_img_arrow_right_black_8x14;
static GFont s_res_roboto_condensed_21;
static GBitmap *s_res_img_arrow_left_8x14;
static GFont s_res_gothic_24_bold;
static TextLayer *s_textlayer_dash;
static TextLayer *s_tl_to;
static TextLayer *s_tl_from;
static BitmapLayer *s_bl_right;
static TextLayer *s_tl_date;
static BitmapLayer *s_bl_left;
static TextLayer *s_tl_total;
static TextLayer *s_tl_deep;
static TextLayer *s_tl_light;
static TextLayer *s_tv_total;
static TextLayer *s_tv_deep;
static TextLayer *s_tv_light;

static void initialise_ui(void) {
    s_window = window_create();
#ifndef PBL_SDK_3    
    window_set_fullscreen(s_window, false);
#endif
    
    s_res_gothic_18_bold = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
    s_res_img_arrow_right_black_8x14 = gbitmap_create_with_resource(RESOURCE_ID_IMG_ARROW_RIGHT_BLACK_8X14);
    s_res_roboto_condensed_21 = fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21);
    s_res_img_arrow_left_8x14 = gbitmap_create_with_resource(RESOURCE_ID_IMG_ARROW_LEFT_8X14);
    s_res_gothic_24_bold = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);
    // s_textlayer_dash
    s_textlayer_dash = text_layer_create(GRect(-2, 25, 146, 20));
    text_layer_set_background_color(s_textlayer_dash, GColorBlack);
    text_layer_set_text_color(s_textlayer_dash, GColorWhite);
    text_layer_set_text(s_textlayer_dash, " - ");
    text_layer_set_text_alignment(s_textlayer_dash, GTextAlignmentCenter);
    text_layer_set_font(s_textlayer_dash, s_res_gothic_18_bold);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_dash);
    
    // s_tl_to
    s_tl_to = text_layer_create(GRect(78, 25, 67, 20));
    text_layer_set_background_color(s_tl_to, GColorBlack);
    text_layer_set_text_color(s_tl_to, GColorWhite);
    text_layer_set_text(s_tl_to, "00:00");
    text_layer_set_font(s_tl_to, s_res_gothic_18_bold);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_tl_to);
    
    // s_tl_from
    s_tl_from = text_layer_create(GRect(-1, 25, 64, 20));
    text_layer_set_background_color(s_tl_from, GColorBlack);
    text_layer_set_text_color(s_tl_from, GColorWhite);
    text_layer_set_text(s_tl_from, "00:00");
    text_layer_set_text_alignment(s_tl_from, GTextAlignmentRight);
    text_layer_set_font(s_tl_from, s_res_gothic_18_bold);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_tl_from);
    
    // s_bl_right
    s_bl_right = bitmap_layer_create(GRect(136, 118, 11, 15));
    bitmap_layer_set_bitmap(s_bl_right, s_res_img_arrow_right_black_8x14);
    bitmap_layer_set_background_color(s_bl_right, GColorWhite);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_bl_right);
    
    // s_tl_date
    s_tl_date = text_layer_create(GRect(-1, 0, 145, 25));
    text_layer_set_background_color(s_tl_date, GColorBlack);
    text_layer_set_text_color(s_tl_date, GColorWhite);
    text_layer_set_text(s_tl_date, "Xxx 00");
    text_layer_set_text_alignment(s_tl_date, GTextAlignmentCenter);
    text_layer_set_font(s_tl_date, s_res_roboto_condensed_21);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_tl_date);
    
    // s_bl_left
    s_bl_left = bitmap_layer_create(GRect(136, 20, 12, 15));
    bitmap_layer_set_bitmap(s_bl_left, s_res_img_arrow_left_8x14);
    bitmap_layer_set_background_color(s_bl_left, GColorBlack);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_bl_left);
    
    // s_tl_total
    s_tl_total = text_layer_create(GRect(1, 50, 71, 20));
    text_layer_set_text(s_tl_total, _("TOTAL:"));
    text_layer_set_font(s_tl_total, s_res_gothic_18_bold);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_tl_total);
    
    // s_tl_deep
    s_tl_deep = text_layer_create(GRect(1, 76, 71, 20));
    text_layer_set_text(s_tl_deep, _("Deep:"));
    text_layer_set_font(s_tl_deep, s_res_gothic_18_bold);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_tl_deep);
    
    // s_tl_light
    s_tl_light = text_layer_create(GRect(1, 104, 71, 20));
    text_layer_set_text(s_tl_light, _("Light:"));
    text_layer_set_font(s_tl_light, s_res_gothic_18_bold);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_tl_light);
    
    // s_tv_total
    s_tv_total = text_layer_create(GRect(78, 45, 50, 24));
    text_layer_set_text(s_tv_total, "00:00");
    text_layer_set_text_alignment(s_tv_total, GTextAlignmentRight);
    text_layer_set_font(s_tv_total, s_res_gothic_24_bold);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_tv_total);
    
    // s_tv_deep
    s_tv_deep = text_layer_create(GRect(78, 72, 50, 24));
    text_layer_set_text(s_tv_deep, "00:00");
    text_layer_set_text_alignment(s_tv_deep, GTextAlignmentRight);
    text_layer_set_font(s_tv_deep, s_res_gothic_24_bold);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_tv_deep);
    
    // s_tv_light
    s_tv_light = text_layer_create(GRect(78, 99, 50, 24));
    text_layer_set_text(s_tv_light, "00:00");
    text_layer_set_text_alignment(s_tv_light, GTextAlignmentRight);
    text_layer_set_font(s_tv_light, s_res_gothic_24_bold);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_tv_light);
}

static void destroy_ui(void) {
    window_destroy(s_window);
    text_layer_destroy(s_textlayer_dash);
    text_layer_destroy(s_tl_to);
    text_layer_destroy(s_tl_from);
    bitmap_layer_destroy(s_bl_right);
    text_layer_destroy(s_tl_date);
    bitmap_layer_destroy(s_bl_left);
    text_layer_destroy(s_tl_total);
    text_layer_destroy(s_tl_deep);
    text_layer_destroy(s_tl_light);
    text_layer_destroy(s_tv_total);
    text_layer_destroy(s_tv_deep);
    text_layer_destroy(s_tv_light);
    gbitmap_destroy(s_res_img_arrow_right_black_8x14);
    gbitmap_destroy(s_res_img_arrow_left_8x14);
}
// END AUTO-GENERATED UI CODE


static void set_stat_light() {
    uint16_t minutes = stats_data->stat[LIGHT-1];
    int h = (minutes == 0 ? 0 : minutes / 60);
    int m = (minutes == 0 ? 0 : minutes % 60);
    
    static char light_lbl[] = "00:00";
    snprintf(light_lbl, sizeof(light_lbl), "%02d:%02d", h, m);
    
    D("setLayer: param minutes %d set %02d:%02d", minutes, h, m);
    
    text_layer_set_text(s_tv_light, light_lbl);
}

static void set_stat_deep() {
    uint16_t minutes = stats_data->stat[DEEP-1];
    int h = (minutes == 0 ? 0 : minutes / 60);
    int m = (minutes == 0 ? 0 : minutes % 60);
    
    static char deep_lbl[] = "00:00";
    snprintf(deep_lbl, sizeof(deep_lbl), "%02d:%02d", h, m);
    
    D("setLayer: param minutes %d set %02d:%02d", minutes, h, m);
    
    text_layer_set_text(s_tv_deep, deep_lbl);
}

static void set_stat_total() {
    uint16_t minutes = stats_data->stat[LIGHT-1] + stats_data->stat[DEEP-1];
    int h = (minutes == 0 ? 0 : minutes / 60);
    int m = (minutes == 0 ? 0 : minutes % 60);
    
    static char total_lbl[] = "00:00";
    snprintf(total_lbl, sizeof(total_lbl), "%02d:%02d", h, m);
    
    D("setLayer: param minutes %d set %02d:%02d", minutes, h, m);
    
    text_layer_set_text(s_tv_total, total_lbl);
}



static struct tm * get_time(uint32_t *val) {
    D("getTime: %ld", *val);
    struct tm *tms = localtime((const time_t *)val);
    //free(tt);// ??
    return tms;
}

static void update_ui_stat_with_sd() {
    // TMP
    //uint16_t total = sleep_data->stat[AWAKE-1] + sleep_data->stat[LIGHT-1] + sleep_data->stat[DEEP-1];
    //uint16_t total = sd->stat[LIGHT-1] + sd->stat[DEEP-1];
    
    //set_hours_minutes(s_val_awake, sleep_data->stat[AWAKE-1]);
    set_stat_light();
    set_stat_deep();
    set_stat_total();
    
    struct tm *ttd = get_time(&(stats_data->start_time));
    static char date_str[] = "Xxx 00";
    strftime(date_str, sizeof(date_str),"%d %b", ttd);
    text_layer_set_text(s_tl_date, date_str);
        
    static char from_str[] = "00:00";
    strftime(from_str, sizeof(from_str), "%H:%M", ttd);
    text_layer_set_text(s_tl_from, from_str);
    
    static char to_str[] = "00:00";
    struct tm *tte = get_time(&(stats_data->end_time));
    strftime(to_str, sizeof(to_str), "%H:%M", tte);
    text_layer_set_text(s_tl_to, to_str);
    // D("to: %s", tbufto);
}

static void update_ui_stat_values() {
    count_recs = count_stat_data();
    
    D("Count stats data %d. Update stats with index %d", count_recs, count_recs - 1);

    if (count_recs <= 0) {
        hide_sleep_stats();
        return;
    }
    current_index = count_recs - 1;
    stats_data = read_stat_data_rec(current_index);
    update_ui_stat_with_sd();
}

static void handle_window_unload(Window* window) {
    destroy_ui();
    free(stats_data);
}

static void back_click_handler(ClickRecognizerRef recognizer, void *context) {
    hide_sleep_stats();
}

// PREV
static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
    if (current_index > 0) {
        current_index--;
        free(stats_data);
        stats_data = read_stat_data_rec(current_index);
        update_ui_stat_with_sd();
    }
}

// NEXT
static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
    if (current_index < count_recs - 1) {
        current_index++;
        free(stats_data);
        stats_data = read_stat_data_rec(current_index);
        update_ui_stat_with_sd();
    }
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {

}


static void config_provider(void *context) {
    window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
    window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
    window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
    window_single_click_subscribe(BUTTON_ID_BACK, back_click_handler);
}

void show_sleep_stats(void) {
    initialise_ui();
    window_set_window_handlers(s_window, (WindowHandlers) {
        .unload = handle_window_unload,
    });
    window_set_click_config_provider(s_window, config_provider);
    update_ui_stat_values();
//    light_enable(true);
    window_stack_push(s_window, true);
}

void hide_sleep_stats(void) {
//    light_enable(false);
    window_stack_remove(s_window, true);
}

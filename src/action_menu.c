//
//  action_menu.c
//  PebSlee
//
//  Created by Nikolai Penkov on 17/12/14.
//  Copyright (c) 2014 Nick Penkov. All rights reserved.
//
#include <pebble.h>

#include "action_menu.h"
#include "sleep_stats.h"
#include "alarm_config.h"
#include "persistence.h"
#include "logic.h"



// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static MenuLayer *s_menulayer;

// A callback is used to specify the amount of sections of menu items
// With this, you can dynamically add and remove sections
static uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data) {
    return 1;
}

// Each section has a number of items;  we use a callback to specify this
// You can also dynamically add and remove items using this
static uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
    return 6;
}

// A callback is used to specify the height of the section header
static int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
    // This is a define provided in pebble.h that you may use for the default height
    return 0;
}


static char* decode_down_coef(float coef) {
    if (coef == DOWN_COEF_SLOW) {
        return "Slow";
    } else if (coef == DOWN_COEF_NORMAL) {
        return "Normal";
    } else if (coef == DOWN_COEF_FAST) {
        return "Fast";
    } else {
        return "Unknown";
    }
}

static char* decode_up_coef(float coef) {
    if (coef == UP_COEF_NOTSENSITIVE) {
        return "Not sensitive";
    } else if (coef == UP_COEF_NORMAL) {
        return "Normal";
    } else if (coef == UP_COEF_VERYSENSITIVE) {
        return "Very sensitive";
    } else {
        return "Unknown";
    }
}

// Here we draw what each header is
//static void menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
//}

// This is the menu item draw callback where you specify what each item should look like
static void menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
    switch (cell_index->row) {
        case 0:
            menu_cell_title_draw(ctx, cell_layer, "Set alarm time");
            break;
        case 1:
            menu_cell_title_draw(ctx, cell_layer, "View stats");
            break;
        case 2:
            menu_cell_title_draw(ctx, cell_layer, "Clear stats");
            break;
        case 3:
            menu_cell_basic_draw(ctx, cell_layer, "Fall assleep", decode_down_coef(get_config()->down_coef), NULL);
            break;
        case 4:
            menu_cell_basic_draw(ctx, cell_layer, "Sensitivity", decode_up_coef(get_config()->up_coef), NULL);
            break;
        case 5:
            menu_cell_title_draw(ctx, cell_layer, "Version: 1.5");
            break;
    }
}

// Here we capture when a user selects a menu item
void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
    float coefd = get_config()->down_coef;
    float coefu = get_config()->up_coef;
    
    switch (cell_index->row) {
        case 0:
            show_alarm_config();
            break;
        case 1:
            show_sleep_stats();
            break;
        case 2:
            clear_sleep_stats();
            hide_action_menu();
            break;
        case 3:
            
            if (coefd == DOWN_COEF_SLOW) {
                set_config_down_coef(DOWN_COEF_NORMAL);
            } else if (coefd == DOWN_COEF_NORMAL) {
                set_config_down_coef(DOWN_COEF_FAST);
            } else if (coefd == DOWN_COEF_FAST) {
                set_config_down_coef(DOWN_COEF_SLOW);
            } else {
                set_config_down_coef(DOWN_COEF_NORMAL);
            }
            menu_layer_reload_data(s_menulayer);
            break;
        case 4:
            
            if (coefu == UP_COEF_NOTSENSITIVE) {
                set_config_up_coef(UP_COEF_NORMAL);
            } else if (coefu == UP_COEF_NORMAL) {
                set_config_up_coef(UP_COEF_VERYSENSITIVE);
            } else if (coefu == UP_COEF_VERYSENSITIVE) {
                set_config_up_coef(UP_COEF_NOTSENSITIVE);
            } else {
                set_config_up_coef(UP_COEF_NORMAL);
            }
            menu_layer_reload_data(s_menulayer);
            break;
        case 5:
            // Do nothing for version
            break;
    }
}

static void initialise_ui(void) {
    s_window = window_create();
    window_set_fullscreen(s_window, false);
    
    // s_menulayer
    s_menulayer = menu_layer_create(GRect(0, 0, 144, 152));
    
    // Set all the callbacks for the menu layer
    menu_layer_set_callbacks(s_menulayer, NULL, (MenuLayerCallbacks){
        .get_num_sections = menu_get_num_sections_callback,
        .get_num_rows = menu_get_num_rows_callback,
        .get_header_height = menu_get_header_height_callback,
//        .draw_header = menu_draw_header_callback,
        .draw_row = menu_draw_row_callback,
        .select_click = menu_select_callback,
    });
    
    menu_layer_set_click_config_onto_window(s_menulayer, s_window);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_menulayer);
}

static void destroy_ui(void) {
    window_destroy(s_window);
    menu_layer_destroy(s_menulayer);
}
// END AUTO-GENERATED UI CODE

static void handle_window_unload(Window* window) {
    destroy_ui();
}

void show_action_menu(void) {
    initialise_ui();
    window_set_window_handlers(s_window, (WindowHandlers) {
        .unload = handle_window_unload,
    });
    window_stack_push(s_window, true);
}

void hide_action_menu(void) {
    window_stack_remove(s_window, true);
}

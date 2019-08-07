#include <pebble.h>
#include "pebble-assist.h"
#include "watch.h"
#include "battery.h"

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GFont s_res_font_thin_45;
static GFont s_res_font_date_thin_30;
static GFont s_res_font_date_20;
static TextLayer *time_layer;
static TextLayer *day_layer;
static TextLayer *date_layer;
static TextLayer *weather_layer;

// static InverterLayer *inverter;
// bool isInverted;

static char temperature_buffer[8];
static char conditions_buffer[32];
static char weather_layer_buffer[32];

static void update_time() {
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Create a long-lived buffer
  static char buffer[] = "00:00";

  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    // Use 24 hour format
    strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
  } else {
    // Use 12 hour format
    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
  }
  
  static char date_buff[] = "24 Jan 1984";
  strftime(date_buff, sizeof(date_buff), "%e %h %G", tick_time);
  
  static char day_buff[] = "Wednesday";
  strftime(day_buff, sizeof(day_buff), "%A", tick_time);
  
  text_layer_set_text(time_layer, buffer);
  text_layer_set_text(date_layer, date_buff);
  text_layer_set_text(day_layer, day_buff);

}

static void initialise_ui(void) {
  s_window = window_create();
  window_set_background_color(s_window, GColorBlack);
  // window_set_fullscreen(s_window, true);
  
  s_res_font_thin_45 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_THIN_45));
  s_res_font_date_thin_30 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DATE_THIN_30));
  s_res_font_date_20 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DATE_20));
  // time_layer
  time_layer = text_layer_create(GRect(0, 46, 144, 59));
  text_layer_set_background_color(time_layer, GColorClear);
  text_layer_set_text_color(time_layer, GColorWhite);
  text_layer_set_text(time_layer, "09:41");
  text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);
  text_layer_set_font(time_layer, s_res_font_thin_45);
  layer_add_child(window_get_root_layer(s_window), (Layer *)time_layer);
  
  // day_layer
  day_layer = text_layer_create(GRect(11, 87, 122, 47));
  text_layer_set_background_color(day_layer, GColorClear);
  text_layer_set_text_color(day_layer, GColorWhite);
  text_layer_set_text(day_layer, "Tuesday");
  text_layer_set_text_alignment(day_layer, GTextAlignmentCenter);
  text_layer_set_font(day_layer, s_res_font_date_thin_30);
  layer_add_child(window_get_root_layer(s_window), (Layer *)day_layer);
  
  // date_layer
  date_layer = text_layer_create(GRect(2, 23, 143, 24));
  text_layer_set_background_color(date_layer, GColorClear);
  text_layer_set_text_color(date_layer, GColorWhite);
  text_layer_set_text(date_layer, "24 Jan 1984");
  text_layer_set_text_alignment(date_layer, GTextAlignmentCenter);
  text_layer_set_font(date_layer, s_res_font_date_20);
  layer_add_child(window_get_root_layer(s_window), (Layer *)date_layer);
  
  // weather_layer
  weather_layer = text_layer_create(GRect(0, 120, 144, 24));
  text_layer_set_background_color(weather_layer, GColorClear);
  text_layer_set_text_color(weather_layer, GColorWhite);
  text_layer_set_text(weather_layer, "");
  text_layer_set_text_alignment(weather_layer, GTextAlignmentCenter);
  text_layer_set_font(weather_layer, s_res_font_date_20);
  layer_add_child(window_get_root_layer(s_window), (Layer *)weather_layer);
  
  // if (isInverted) {
  //   inverter = inverter_layer_create(GRect(0, 0, 144, 168));
  //   layer_add_child(window_get_root_layer(s_window), (Layer *)inverter);
  // }
  
  SetupBattery(window_get_root_layer(s_window));
  update_time();
}

static void destroy_ui(void) {
  window_destroy(s_window);
  text_layer_destroy(time_layer);
  text_layer_destroy(day_layer);
  text_layer_destroy(date_layer);
  text_layer_destroy(weather_layer);
  fonts_unload_custom_font(s_res_font_thin_45);
  fonts_unload_custom_font(s_res_font_date_thin_30);
  fonts_unload_custom_font(s_res_font_date_20);
}
// END AUTO-GENERATED UI CODE

static void get_weather() {
  // DictionaryIterator *iter;
  // app_message_outbox_begin(&iter);

  // // Add a key-value pair
  // dict_write_uint8(iter, 0, 0);

  // // Send the message!
  // app_message_outbox_send();
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // Get the first pair
  Tuple *t = dict_read_first(iterator);
  
  bool need_to_update_weather = false;

  // Process all pairs present
  while(t != NULL) {
    // Process this pair's key
    switch (t->key) {
      // case INVERT:
      //   APP_LOG(APP_LOG_LEVEL_INFO, "INVERT received with value %d", (int)t->value->int32);
      //   bool old = isInverted;
      //   int val = t->value->int32;
      //   if (val == 1) {
      //     if (old != val) {
      //       inverter = inverter_layer_create(GRect(0, 0, 144, 168));
      //       layer_add_child(window_get_root_layer(s_window), (Layer *)inverter);
      //     }
      //     isInverted = true;
      //   }
      //   else {
      //     isInverted = false;
      //     if (old != val) {
      //       inverter_layer_destroy(inverter);
      //     }
      //   }

      //   persist_write_bool(INVERT, isInverted);
      //   break;
      // case MESSAGE_KEY_Temperature:
      //   snprintf(temperature_buffer, sizeof(temperature_buffer), "%dC", (int)t->value->int32);
      //   need_to_update_weather = true;
      //   break;
      // case MESSAGE_KEY_Conditions:
      //   snprintf(conditions_buffer, sizeof(conditions_buffer), "%s", t->value->cstring);
      //   need_to_update_weather = true;
      //   break;
      // default:
      //   APP_LOG(APP_LOG_LEVEL_ERROR, "Key %d not recognized!", (int)t->key);
      //   break;
    }
    
    if (need_to_update_weather) {
      snprintf(weather_layer_buffer, sizeof(weather_layer_buffer), "%s, %s", temperature_buffer, conditions_buffer);
      text_layer_set_text(weather_layer, weather_layer_buffer);
    }

    // Get next pair, if any
    t = dict_read_next(iterator);
  }
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
  if(tick_time->tm_min % 30 == 0) {
    // Need to update weather
    get_weather();
  }
}

static void handle_window_unload(Window* window) {
  destroy_ui();
}

void show_watch(void) {
  // isInverted = persist_read_bool_safe(INVERT, false);
  initialise_ui();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_stack_push(s_window, true);
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
  app_message_register_inbox_received(inbox_received_callback);
  get_weather();
}

void hide_watch(void) {
  window_stack_remove(s_window, true);
}

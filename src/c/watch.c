#include <pebble.h>
#include "pebble-assist.h"
#include "watch.h"
#include "battery.h"

static Window *s_window;
static Layer *s_window_layer;
static GFont s_res_font_thin_45;
static GFont s_res_font_date_thin_30;
static GFont s_res_font_date_20;
static TextLayer *time_layer;
static TextLayer *day_layer;
static TextLayer *date_layer;
static TextLayer *weather_layer;

// static InverterLayer *inverter;
// bool isInverted;

// static char temperature_buffer[8];
// static char conditions_buffer[32];
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
  s_window_layer = window_get_root_layer(s_window);
  window_set_background_color(s_window, GColorBlack);
  GRect unobstructed_bounds = layer_get_unobstructed_bounds(s_window_layer);
  
  s_res_font_thin_45 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_THIN_45));
  s_res_font_date_thin_30 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DATE_THIN_30));
  s_res_font_date_20 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DATE_20));

  int face_start = unobstructed_bounds.size.h * 0.16;

  // time_layer
  time_layer = text_layer_create(GRect(0, face_start + 23, unobstructed_bounds.size.w, 59));
  text_layer_set_background_color(time_layer, GColorClear);
  text_layer_set_text_color(time_layer, GColorWhite);
  text_layer_set_text(time_layer, "09:41");
  text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);
  text_layer_set_font(time_layer, s_res_font_thin_45);
  layer_add_child(s_window_layer, (Layer *)time_layer);
  
  // day_layer
  day_layer = text_layer_create(GRect(11, face_start + 64, unobstructed_bounds.size.w - 22, 47));
  text_layer_set_background_color(day_layer, GColorClear);
  text_layer_set_text_color(day_layer, GColorWhite);
  text_layer_set_text(day_layer, "Tuesday");
  text_layer_set_text_alignment(day_layer, GTextAlignmentCenter);
  text_layer_set_font(day_layer, s_res_font_date_thin_30);
  layer_add_child(s_window_layer, (Layer *)day_layer);
  
  // date_layer
  date_layer = text_layer_create(GRect(2, face_start, unobstructed_bounds.size.w, 24));
  text_layer_set_background_color(date_layer, GColorClear);
  text_layer_set_text_color(date_layer, GColorWhite);
  text_layer_set_text(date_layer, "24 Jan 1984");
  text_layer_set_text_alignment(date_layer, GTextAlignmentCenter);
  text_layer_set_font(date_layer, s_res_font_date_20);
  layer_add_child(s_window_layer, (Layer *)date_layer);
  
  // weather_layer
  weather_layer = text_layer_create(GRect(0, 120, unobstructed_bounds.size.w, 24));
  text_layer_set_background_color(weather_layer, GColorClear);
  text_layer_set_text_color(weather_layer, GColorWhite);
  text_layer_set_text(weather_layer, "");
  text_layer_set_text_alignment(weather_layer, GTextAlignmentCenter);
  text_layer_set_font(weather_layer, s_res_font_date_20);
  layer_add_child(s_window_layer, (Layer *)weather_layer);
  
  // if (isInverted) {
  //   inverter = inverter_layer_create(GRect(0, 0, 144, 168));
  //   layer_add_child(window_get_root_layer(s_window), (Layer *)inverter);
  // }
  
  SetupBattery(window_get_root_layer(s_window));
  update_time();
}

static void update_screen(GRect screen_bounds) {
  int face_start = 2 + (screen_bounds.size.h - 100) * 0.3;

  layer_set_frame(text_layer_get_layer(time_layer), GRect(0, face_start + 23, screen_bounds.size.w, 59));
  layer_set_frame(text_layer_get_layer(day_layer), GRect(11, face_start + 64, screen_bounds.size.w - 22, 47));
  layer_set_frame(text_layer_get_layer(date_layer), GRect(2, face_start, screen_bounds.size.w, 24));
  layer_set_frame(text_layer_get_layer(weather_layer), GRect(0, 120, screen_bounds.size.w, 24));

  UpdateBatteryFrame(screen_bounds);
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

static void update_weather(const char* temp, const char* cond) {
  snprintf(weather_layer_buffer, sizeof(weather_layer_buffer), "%s, %s", temp, cond);
  text_layer_set_text(weather_layer, weather_layer_buffer);

  bool has_weather = strcmp(temp, "") > 0 || strcmp(cond, "") > 0;
  if (has_weather && persist_read_bool(PERSIST_ENABLE_WEATHER)) {
    text_layer_set_text_color(weather_layer, GColorWhite);
  } else {
    text_layer_set_text_color(weather_layer, GColorBlack);
  }
}

static void get_weather() {
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);

  // Add a key-value pair
  dict_write_uint8(iter, 0, 0);

  // Send the message!
  app_message_outbox_send();
}

static void inbox_received_callback(DictionaryIterator *iter, void *context) {
  bool need_to_update_weather = false;
  char* temp = "";
  char* cond = "";

  Tuple *enable_tuple = dict_find(iter, MESSAGE_KEY_EnableWeather);
  if (enable_tuple) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "EnableWeather: %d", (int)enable_tuple->value->uint8);

    persist_write_bool(PERSIST_ENABLE_WEATHER, enable_tuple->value->uint8 == 1 ? true : false);
    need_to_update_weather = true;
  }

  Tuple *temp_tuple = dict_find(iter, MESSAGE_KEY_Temperature);
  if (temp_tuple) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Temperature: %s", temp_tuple->value->cstring);

    temp = temp_tuple->value->cstring;
    need_to_update_weather = true;
  }

  Tuple *cond_tuple = dict_find(iter, MESSAGE_KEY_Conditions);
  if (cond_tuple) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Conditions: %s", cond_tuple->value->cstring);

    cond = cond_tuple->value->cstring;
    need_to_update_weather = true;
  }

  if (need_to_update_weather) {
    update_weather(temp, cond);
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

static void unobstructed_will_change(GRect final_unobstructed_screen_area, void *context) {
  if (final_unobstructed_screen_area.size.h < 160) {
    text_layer_set_text_color(weather_layer, GColorClear);
  }
}

static void unobstructed_did_change(void *context) {
  GRect bounds = layer_get_unobstructed_bounds(s_window_layer);
  update_screen(bounds);

  if (bounds.size.h > 160) {
    text_layer_set_text_color(weather_layer, GColorWhite);
  }
}

static void unobstructed_change(AnimationProgress progress, void *context) {
  GRect bounds = layer_get_unobstructed_bounds(s_window_layer);
  update_screen(bounds);
}

void show_watch(void) {
  // isInverted = persist_read_bool_safe(INVERT, false);
  initialise_ui();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_stack_push(s_window, true);

  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

  UnobstructedAreaHandlers handlers = {
    .will_change = unobstructed_will_change,
    .did_change = unobstructed_did_change,
    .change = unobstructed_change,
  };
  unobstructed_area_service_subscribe(handlers, NULL);

  GRect bounds = layer_get_unobstructed_bounds(s_window_layer);
  update_screen(bounds);

  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
  app_message_register_inbox_received(inbox_received_callback);
  get_weather();
}

void hide_watch(void) {
  window_stack_remove(s_window, true);
}

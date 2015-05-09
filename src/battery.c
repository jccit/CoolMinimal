#include <pebble.h>
#include "battery.h"

static BitmapLayer *image_layer_battery;
static GBitmap *image_line;
static Layer *current_window;

void RefreshBattery(BatteryChargeState charge_state) {
  int height = 168;
	int bar_size = 2;
	double segment = 1.44;
	
	uint8_t raw_percent = charge_state.charge_percent;
	uint8_t percent_display = raw_percent * segment;
	bool isCharging = charge_state.is_charging;

  percent_display = raw_percent * segment;

  if(isCharging) {
    image_line = gbitmap_create_with_resource(RESOURCE_ID_BATTERY_CHARGE);
  } else {
    image_line = gbitmap_create_with_resource(RESOURCE_ID_BATTERY_NORMAL);
  }
  
  image_layer_battery = bitmap_layer_create(GRect(0,height-bar_size,percent_display,bar_size));

	bitmap_layer_set_bitmap(image_layer_battery, image_line);
	layer_add_child(current_window, bitmap_layer_get_layer(image_layer_battery));
}

static void battbar_handle_battery(BatteryChargeState charge_state) {
	RefreshBattery(charge_state);
}

void SetupBattery(Layer *myWindow) {
	current_window = myWindow;
	battery_state_service_subscribe(&battbar_handle_battery);
  DrawBattery();
}

void DrawBattery() {
	BatteryChargeState charge_state = battery_state_service_peek();
	RefreshBattery(charge_state);
}
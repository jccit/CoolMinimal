#include <pebble.h>
#include "watch.h"
  
int main(void) {
  show_watch();
	app_event_loop();
  hide_watch();
}

#include <pebble.h>
#include "main.h"

#define MAX_FRAME 63

//array of all the resource ids for the clock frames. shifted to make element 0 the midnight frame.
static int clock_image_resource_ids[] = {RESOURCE_ID_IMAGE_CLOCK_32, RESOURCE_ID_IMAGE_CLOCK_33, RESOURCE_ID_IMAGE_CLOCK_34, RESOURCE_ID_IMAGE_CLOCK_35, RESOURCE_ID_IMAGE_CLOCK_36, RESOURCE_ID_IMAGE_CLOCK_37, RESOURCE_ID_IMAGE_CLOCK_38, RESOURCE_ID_IMAGE_CLOCK_39, RESOURCE_ID_IMAGE_CLOCK_40, RESOURCE_ID_IMAGE_CLOCK_41, RESOURCE_ID_IMAGE_CLOCK_42, RESOURCE_ID_IMAGE_CLOCK_43, RESOURCE_ID_IMAGE_CLOCK_44, RESOURCE_ID_IMAGE_CLOCK_45, RESOURCE_ID_IMAGE_CLOCK_46, RESOURCE_ID_IMAGE_CLOCK_47, RESOURCE_ID_IMAGE_CLOCK_48, RESOURCE_ID_IMAGE_CLOCK_49, RESOURCE_ID_IMAGE_CLOCK_50, RESOURCE_ID_IMAGE_CLOCK_51, RESOURCE_ID_IMAGE_CLOCK_52, RESOURCE_ID_IMAGE_CLOCK_53, RESOURCE_ID_IMAGE_CLOCK_54, RESOURCE_ID_IMAGE_CLOCK_55, RESOURCE_ID_IMAGE_CLOCK_56, RESOURCE_ID_IMAGE_CLOCK_57, RESOURCE_ID_IMAGE_CLOCK_58, RESOURCE_ID_IMAGE_CLOCK_59, RESOURCE_ID_IMAGE_CLOCK_60, RESOURCE_ID_IMAGE_CLOCK_61, RESOURCE_ID_IMAGE_CLOCK_62, RESOURCE_ID_IMAGE_CLOCK_63, RESOURCE_ID_IMAGE_CLOCK_00, RESOURCE_ID_IMAGE_CLOCK_01, RESOURCE_ID_IMAGE_CLOCK_02, RESOURCE_ID_IMAGE_CLOCK_03, RESOURCE_ID_IMAGE_CLOCK_04, RESOURCE_ID_IMAGE_CLOCK_05, RESOURCE_ID_IMAGE_CLOCK_06, RESOURCE_ID_IMAGE_CLOCK_07, RESOURCE_ID_IMAGE_CLOCK_08, RESOURCE_ID_IMAGE_CLOCK_09, RESOURCE_ID_IMAGE_CLOCK_10, RESOURCE_ID_IMAGE_CLOCK_11, RESOURCE_ID_IMAGE_CLOCK_12, RESOURCE_ID_IMAGE_CLOCK_13, RESOURCE_ID_IMAGE_CLOCK_14, RESOURCE_ID_IMAGE_CLOCK_15, RESOURCE_ID_IMAGE_CLOCK_16, RESOURCE_ID_IMAGE_CLOCK_17, RESOURCE_ID_IMAGE_CLOCK_18, RESOURCE_ID_IMAGE_CLOCK_19, RESOURCE_ID_IMAGE_CLOCK_20, RESOURCE_ID_IMAGE_CLOCK_21, RESOURCE_ID_IMAGE_CLOCK_22, RESOURCE_ID_IMAGE_CLOCK_23, RESOURCE_ID_IMAGE_CLOCK_24, RESOURCE_ID_IMAGE_CLOCK_25, RESOURCE_ID_IMAGE_CLOCK_26, RESOURCE_ID_IMAGE_CLOCK_27, RESOURCE_ID_IMAGE_CLOCK_28, RESOURCE_ID_IMAGE_CLOCK_29, RESOURCE_ID_IMAGE_CLOCK_30, RESOURCE_ID_IMAGE_CLOCK_31};
static short int current_frame = 0;
static GBitmap *current_bitmap;

static Window *s_window;
static BitmapLayer *s_bitmap_layer;

static void change_current_frame(const int frame) {
  current_frame = frame % MAX_FRAME;
  gbitmap_destroy(current_bitmap);
  current_bitmap = gbitmap_create_with_resource(clock_image_resource_ids[current_frame]);
  bitmap_layer_set_bitmap(s_bitmap_layer, current_bitmap);
}

static short int calculate_current_frame(const time_t *timep) {
  short int frame = ((int)*timep % 86400)/1350;
  return frame;
}

static void check_time() {
  time_t temp = time(NULL);
  change_current_frame(calculate_current_frame(&temp));
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  check_time();
}

static void window_load(Window *window) {
  window_set_background_color(window, GColorBlack);  
  current_bitmap = gbitmap_create_with_resource(clock_image_resource_ids[32]);
  s_bitmap_layer = bitmap_layer_create(GRect(8,20,128,128));
  bitmap_layer_set_bitmap(s_bitmap_layer, current_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_bitmap_layer));

}

static void window_unload(Window *window) {
  gbitmap_destroy(current_bitmap);
  bitmap_layer_destroy(s_bitmap_layer);
}

static void init() {
  s_window = window_create();
  
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload
  });
  
  window_stack_push(s_window, true);
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  check_time();
}

static void deinit() {
  window_destroy(s_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
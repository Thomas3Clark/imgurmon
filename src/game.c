#ifndef GAME_C
#define GAME_C
#include <pebble.h>
#include "includes.h"
#include "classes.c"
#define NUM_IMGURMON 40
  
//Imgurmon_Stats imgurmon_stats[NUM_IMGURMON];

/**Get an imgurmon with a given ID.  No error checking is done!
Be careful!
*/
Imgurmon_Stats* get_imgurmon_stats(int id) {
  return 0;//&(imgurmon_stats[id]);
}
#endif
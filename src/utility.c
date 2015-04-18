#ifndef UTILITY_C
#define UTILITY_C
#include <pebble.h>
#include <stdio.h>
#include "game.h"
#include "classes.h"
  
/*Create and store an imgurmon stats
Stores this to the specified imgurmon pointer
*/
void imgurmon_stats_create(Imgurmon_Stats* stats, char* name, int hp, int attack1, int attack2, int attack3, int type, int evolve, int evolvelvl) {
  
}
  
//Turn a 4 byte array into an integer  
int buildInt(char* buffer) {
  int i = 0;
  int result = 0;
  for (i = 0; i < 4; i++) {
    result = result | (int)buffer[i];
    if (i < 3) {
      result = result << 1;
    }
  }
  //int result = *((int*)buffer); this doesnt work because of endiendness
  return result;
}  
  
/**All the initialization required to start the game.
This involves loading stats, the image, etc
*/
void load_imgurmon() {
  char* s_buffer;
  // Get resource and size
  ResHandle handle = resource_get_handle(RESOURCE_ID_STATS);
  int res_size = resource_size(handle);

  // Copy to buffer
  s_buffer = (char*)malloc(res_size);
  resource_load(handle, (uint8_t*)s_buffer, res_size);
  
  //Read in the imgurmon
  //An imgurmon takes up 28 bytes
  int i;
  for (i = 0; i < res_size / 28; i++) {
    char* substring = &(s_buffer[i*28]);
    
    /*Our input file is structured like such:
				 * 20 bytes - Name (including null terminator)
				 * 2  bytes - HP (0-65535)
				 * 1  byte  - Attack1 id
				 * 1  byte  - Attack2 id
				 * 1  byte  - Attack3 id
				 * 1  byte  - type
				 * 1  byte  - ID of evolution
				 * 1  byte  - Evolution Level
				 * 
				 * Total Size: 28bytes per entry!
				 */
    
    //Bytes 20 and 21 are hp.  Turn this into a buffer
    char buffer[4] = {0,0,substring[20],substring[21]};
    int hp        = buildInt(buffer);
    int attack1   = (int)substring[22];
    int attack2   = (int)substring[23];
    int attack3   = (int)substring[24];
    int type      = (int)substring[25];
    int evolve    = (int)substring[26];
    int evolvelvl = (int)substring[27];
    
    
    //printf("Loading %s. HP:%d, attacks:%d:%d:%d, type:%d, evolvesto:%d, evolvelvl:%d\n",substring,hp,attack1,attack2,attack3,type,evolve,evolvelvl);
    //Store this imgurmon
    //imgurmon_stats[i] = imgurmon_stats_create(&(imgurmon_stats[i]),substring,hp,attack1,attack2,attack3,type,evolve,evolvelvl);
  }
  
  free(s_buffer);
}

void load_map() {
  //Read the map file, and look at the first integer to get the size
  //Handle collision too (separate file)
  char* s_buffer;
  char* collisionBuffer;
  
  // Get resource and size
  ResHandle handle = resource_get_handle(RESOURCE_ID_MAP);
  int res_size = resource_size(handle);
  ResHandle collisionHandle = resource_get_handle(RESOURCE_ID_COLLISION);
  int collision_size = resource_size(collisionHandle);

  // Copy to buffer
  s_buffer = (char*)malloc(res_size);
  resource_load(handle, (uint8_t*)s_buffer, res_size);
  collisionBuffer = (char*)malloc(collision_size);
  resource_load(collisionHandle, (uint8_t*)collisionBuffer, collision_size);
  
  //Now that the map is loaded, lets start copying stuff!
  map_size = buildInt(s_buffer);
  
  char* data = &(s_buffer[4]);
  int row,col;
  
  //Allocate the rows
  map = (char**)malloc(sizeof(char*)*map_size);
  collision = (char**)malloc(sizeof(char*)*map_size);

  int index = 0;  //Where are we in the file?
  
  //Begin reading in the map file
  for (row = 0; row < map_size; row++) {
    //Allocate this column
    map[row] = (char*)malloc(sizeof(char)*map_size);
    collision[row] = (char*)malloc(sizeof(char)*map_size);
    
    //Read the map data and apply it
    for (col = 0; col < map_size; col++) {
      //Read in the map file and store it here!
      map[row][col] = data[index];
      collision[row][col] = collisionBuffer[index];
      index++;
    }
  }
  
  //Free the map and collision buffer
  free(s_buffer);
  free(collisionBuffer);
}

/**Simple drawing method that lets us take advantage of spritesheets
GRect src is the source rectangle (where to pull the image from) and
GRect dest is where to draw it on the screen
Note, this does not do any scaling
*/
void graphics_draw_bitmap(GContext* ctx, GBitmap* bitmap, GRect src, GRect dest) {
  GBitmap* sprite = gbitmap_create_as_sub_bitmap(bitmap, src);
  graphics_draw_bitmap_in_rect(ctx, sprite, dest);
  free(sprite);
}

#endif
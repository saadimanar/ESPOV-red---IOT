#pragma once
#include <stdio.h>
#include "esp_spiffs.h"

/*
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
*/
//for SPIFFS

void initFileSystem()
{
    esp_vfs_spiffs_conf_t conf = {
    .base_path = "/spiffs",
    .partition_label = NULL,
    .max_files = 32,
    .format_if_mount_failed = true
  };
  esp_vfs_spiffs_register(&conf);
  //esp_spiffs_format(0);
  size_t total = 0, used = 0;
  esp_spiffs_info(0, &total, &used);

}

void deinitFileSystem()
{
  //esp_vfs_spiffs_unregister(0);
}

bool readFromFile(const char *fileName, unsigned char *dest, int count)
{
  FILE* f = fopen(fileName, "r");
  if(!f) return false;
  fread(dest, sizeof(unsigned char), count, f);
  return true;
}

bool writeToFile(const char *fileName, unsigned char *src, int count)
{
  FILE* f = fopen(fileName, "w");
  if(!f) return false;
  fwrite(src, sizeof(unsigned char), count, f);
  fclose(f); 
  return true;
}

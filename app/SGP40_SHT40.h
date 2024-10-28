#ifndef SGP40_SHT40_H
#define SGP40_SHT40_H

#include "stdint.h"
#include "freertos/FreeRTOS.h"
#include "my_i2c_master.h"
#include "sgp40_voc_index.h"
#include "sensirion_voc_algorithm.h"
#include "esp_log.h"
#include "math.h"

void SGP40_SHT40_init(void);
void SGP40_SHT40_read(void *parameter);



#endif
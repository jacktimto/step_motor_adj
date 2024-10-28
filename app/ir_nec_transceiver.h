#pragma once


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "driver/rmt_tx.h"
#include "driver/rmt_rx.h"
#include "driver/rmt_common.h"
#include "ir_nec_encoder.h"

void ir_nec_transceiver(void *parameter);
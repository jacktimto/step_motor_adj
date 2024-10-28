#include "my_adc.h"

int adc_raw = 10;
int adc_value = 0;

void vadc_read(void *prvparameter)
{
    adc_oneshot_unit_handle_t adc_oneshot;
    adc_oneshot_unit_init_cfg_t adc_config =
        {
            .unit_id = ADC_UNIT_1,
            .ulp_mode = ADC_ULP_MODE_DISABLE,
            .clk_src = SOC_MOD_CLK_RC_FAST,
        };

    ESP_ERROR_CHECK(adc_oneshot_new_unit(&adc_config, &adc_oneshot));

    adc_oneshot_chan_cfg_t chan_config =
        {
            .atten = ADC_ATTEN_DB_12,
            .bitwidth = ADC_BITWIDTH_DEFAULT,
        };
    adc_channel_t adc_channel = ADC_CHANNEL_3;
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc_oneshot, adc_channel, &chan_config));

    // 创建ESP32_ADC calibration
    adc_cali_handle_t adc_cali_handle = NULL;
    adc_cali_line_fitting_config_t adc_cali_config = {
        .unit_id = ADC_UNIT_1,
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };

    ESP_ERROR_CHECK(adc_cali_create_scheme_line_fitting(&adc_cali_config, &adc_cali_handle));

    while (1)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        // read adc value
        ESP_ERROR_CHECK(adc_oneshot_read(adc_oneshot, adc_channel, &adc_raw));
        ESP_ERROR_CHECK(adc_cali_raw_to_voltage(adc_cali_handle, adc_raw, &adc_value));
        ESP_LOGI("adc", "adc_raw:%d,adc_value:%dmV--------------------------------------------", adc_raw, adc_value);
    }
}

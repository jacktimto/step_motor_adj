idf_component_register(SRCS "main.c"
                            ../app/74HC595.c
                            ../app/step_motor.c
                            ../app/my_adc.c
                            ../app/motor.c
                            ../app/pcnt.c
                            ../app/eeprom.c
                            ../app/htr3236.c
                            ../app/htr3218.c
                            ../app/ir_nec_encoder.c
                            ../app/ir_nec_transceiver.c
                            ../app/SGP40_SHT40.c
                            ../app/my_i2c_master.c
                            ../app/my_gptimer.c
                            ../app/my_uart.c
                            ../SGP40/sensirion_voc_algorithm.c
                            ../SGP40/sgp40_voc_index.c
                            
                       INCLUDE_DIRS "."
                       ../app
                       ../SGP40

                       )

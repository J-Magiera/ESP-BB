idf_component_register(SRCS "app_adc.c" "peripherals.c"
                  INCLUDE_DIRS "."
                  REQUIRES esp_adc_cal  )
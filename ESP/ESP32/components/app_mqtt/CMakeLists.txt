idf_component_register(SRCS "app_mqtt.c"
                  INCLUDE_DIRS "."
                  PRIV_REQUIRES mqtt peripherals CLI)
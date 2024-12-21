set(srcs "main.c"
         "server_gatt.c"
         "server_gap.c"
         "drv/drv_dht11.c")

idf_component_register(SRCS "${srcs}"
                       INCLUDE_DIRS "."
                       INCLUDE_DIRS "drv")
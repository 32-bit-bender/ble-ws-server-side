set(srcs "main.c"
         "gatt_svr.c"
         "gap_svr.c"
         "drv/drv_dht11.c")

idf_component_register(SRCS "${srcs}"
                       INCLUDE_DIRS "."
                       INCLUDE_DIRS "drv")
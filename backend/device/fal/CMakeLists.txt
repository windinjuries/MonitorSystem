set(DIR_SRCS src/fal_flash.c
             src/fal_partition.c
             src/fal.c
             port/fal_flash_sfud_port.c
             )

add_library(fal STATIC ${DIR_SRCS})
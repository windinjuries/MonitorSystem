set(DIR_SRCS src/sfud_sfdp.c
             src/sfud.c 
             port/sfud_port.c)
add_library(sfud STATIC ${DIR_SRCS})
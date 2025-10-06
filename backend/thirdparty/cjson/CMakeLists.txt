set(SRC_DIR
    cJSON/cJSON.c
    cJSON/cJSON_Utils.c
    )

set(INCLUDE_DIR 
    cJSON/
    )
set(PROJECT_INCLUDE_DIRS ${INCLUDE_DIR} PARENT_SCOPE)

add_library(cjson STATIC ${SRC_DIR})
target_include_directories(cjson PUBLIC ${INCLUDE_DIR})
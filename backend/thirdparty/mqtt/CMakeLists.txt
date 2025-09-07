set(SRC_DIR
    paho.mqtt.embedded/MQTTPacket/src/MQTTConnectClient.c
    paho.mqtt.embedded/MQTTPacket/src/MQTTConnectServer.c
    paho.mqtt.embedded/MQTTPacket/src/MQTTDeserializePublish.c
    paho.mqtt.embedded/MQTTPacket/src/MQTTSerializePublish.c
    paho.mqtt.embedded/MQTTPacket/src/MQTTFormat.c
    paho.mqtt.embedded/MQTTPacket/src/MQTTPacket.c
    paho.mqtt.embedded/MQTTPacket/src/MQTTSubscribeClient.c
    paho.mqtt.embedded/MQTTPacket/src/MQTTSubscribeServer.c
    paho.mqtt.embedded/MQTTPacket/src/MQTTUnsubscribeClient.c
    paho.mqtt.embedded/MQTTPacket/src/MQTTUnsubscribeServer.c

    paho.mqtt.embedded/MQTTClient-C/src/MQTTClient.c
    paho.mqtt.embedded/MQTTClient-C/src/linux/MQTTLinux.c
    )

set(INCLUDE_DIR 
    paho.mqtt.embedded/MQTTPacket/src

    paho.mqtt.embedded/MQTTClient-C/src
    paho.mqtt.embedded/MQTTClient-C/src/linux
    )
set(PROJECT_INCLUDE_DIRS ${INCLUDE_DIR} PARENT_SCOPE)
add_library(mqtt STATIC ${SRC_DIR})
target_include_directories(mqtt PUBLIC ${INCLUDE_DIR})
target_compile_definitions(mqtt PRIVATE
             MQTTCLIENT_PLATFORM_HEADER=MQTTLinux.h MQTTCLIENT_QOS2=1)
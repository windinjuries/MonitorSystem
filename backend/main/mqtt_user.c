/*******************************************************************************
 * Copyright (c) 2012, 2016 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution. 
 *
 * The Eclipse Public License is available at 
 *   http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at 
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Ian Craggs - initial contribution
 *    Ian Craggs - change delimiter option from char to string
 *    Al Stockdill-Mander - Version using the embedded C client
 *    Ian Craggs - update MQTTClient function names
 *******************************************************************************/

#include <stdio.h>
#include <memory.h>
#include <signal.h>
#include <sys/time.h>

#include "hlog.h"
#include "hloop.h"
#include "hplatform.h"
#include "hv.h"
#include "mqtt/mqtt_client.h"
#include "mqtt_client.h"

// #define USE_SSL        1
#define USE_AUTH       1

#define MQTT_ADDRESS "h739bd8e.ala.dedicated.aliyun.emqxcloud.cn"
#define MQTT_NAME			"root"
#define MQTT_PASSWD         "root"

#define MQTT_TOPIC_VERSION  "V1"
#define MQTT_TOPIC_ENV  "TEST"
#define MQTT_TOPIC_PRODUCT  "MonitorSystem"
#define MQTT_TOPIC_SN  "MS000"
#define MQTT_TOPIC_MESSAGE_TYPE "DATA"
#if USE_SSL
#define MQTT_PORT 8883 
#else
#define MQTT_PORT 1883 
#endif

static int is_connected = 0;
static int is_inited = 0;
static mqtt_client_t* cli = NULL;
/*
 * workflow:
 * mqtt_client_new -> mqtt_client_xxx -> mqtt_client_run
 *
 * mqtt_client_set_xxx ->
 * mqtt_client_connect ->
 * on_connack -> mqtt_client_publish ->
 * on_puback -> mqtt_client_disconnect ->
 * on_disconnect -> mqtt_client_stop
 *
 */

static void on_mqtt(mqtt_client_t* cli, int type) {
    mqtt_message_t* msg = NULL;
    msg = (mqtt_message_t*)mqtt_client_get_userdata(cli);
    switch(type) {
    case MQTT_TYPE_CONNECT:
        break;
    case MQTT_TYPE_DISCONNECT:
        LOGW("MQTT disconnected!");
        mqtt_message_t* msg = (mqtt_message_t*)mqtt_client_get_userdata(cli);
        HV_FREE(msg);
        mqtt_client_set_userdata(cli, NULL);
        mqtt_client_stop(cli);
        break;
    case MQTT_TYPE_CONNACK:
        LOGI("MQTT connect ack!");
        is_connected = 1;
        break;
    case MQTT_TYPE_PUBACK: /* qos = 1 */
        // printf("mqtt puback mid=%d\n", cli->mid);
        // mqtt_client_disconnect(cli);
        break;
    case MQTT_TYPE_PUBREC: /* qos = 2 */
        // printf("mqtt pubrec mid=%d\n", cli->mid);
        // wait MQTT_TYPE_PUBCOMP
        break;
    case MQTT_TYPE_PUBCOMP: /* qos = 2 */
        // printf("mqtt pubcomp mid=%d\n", cli->mid);
        // mqtt_client_disconnect(cli);
        break;
    case MQTT_TYPE_SUBACK: /* qos = 1 */
        LOGI("MQTT subscrible ack");
        is_inited = 1;
        break;
    case MQTT_TYPE_PUBLISH: /* qos = 1 */
        LOGI("MQTT publish mid=%d\n", cli->mid);
        msg = (mqtt_message_t*)&(cli->message);
        if (msg != NULL) {
        LOGI("recv  qos=%d  retain=%d  topic=%.*s  payload=%.*s\n",
            msg->qos,
            msg->retain,
            msg->topic_len, msg->topic,
            msg->payload_len, (char*)msg->payload);
        }
    default:
        break;
    }
}

/* main function */
int mqtt_loop_thread()
{
	int ssl = 0;

	char *clientId = MQTT_TOPIC_SN;
    cli = mqtt_client_new(NULL);
    if (cli == NULL) return -1;
    cli->keepalive = 20;
    reconn_setting_t reconn = {0};
    reconn.max_retry_cnt = 100;
    reconn.max_delay = 10000; // ms
    reconn.min_delay = 1000; // ms
    reconn.delay_policy = 2;
   
    mqtt_client_set_reconnect(cli, &reconn);


    mqtt_client_set_callback(cli, on_mqtt);

    mqtt_client_set_id(cli, clientId);
#if USE_AUTH
    mqtt_client_set_auth(cli, MQTT_NAME, MQTT_PASSWD);
#endif

#if USE_SSL
    ssl = 1;
#endif 
    mqtt_message_t* msg = NULL;
    HV_ALLOC_SIZEOF(msg);
    mqtt_client_connect(cli, MQTT_ADDRESS, MQTT_PORT, ssl);
    mqtt_client_run(cli);
    return 0;
}

int mqtt_send_thread()
{
	char *topic_header = MQTT_TOPIC_VERSION"/"MQTT_TOPIC_ENV"/"MQTT_TOPIC_PRODUCT"/"MQTT_TOPIC_SN"/";

    char sub_topic[128] = {0};
    char pub_topic[128] = {0};
    strcpy(sub_topic, topic_header);
    strcpy(sub_topic + strlen(topic_header), "#");
    strcpy(pub_topic, topic_header);
    strcpy(pub_topic + strlen(topic_header), MQTT_TOPIC_MESSAGE_TYPE);

    mqtt_message_t* msg = NULL;
    HV_ALLOC_SIZEOF(msg);
    const char* payload = "{\"temperature\":\"15\",\"voltage\":13}";
    hv_delay(5000);
    while(1)
    {
        if (is_connected == 1) 
        {
            if (is_inited == 0) 
            {
                LOGI("subscribe topic: %s", sub_topic);
                mqtt_client_subscribe(cli, sub_topic, 1);
            }
            else 
            {
                msg->topic = pub_topic;
                msg->topic_len = strlen(pub_topic);
                msg->payload = payload;
                msg->payload_len = strlen(payload);
                msg->retain = 0;
                msg->qos = 1;
                mqtt_client_publish(cli, msg);
            }
        }
        hv_delay(5000);
    }
}

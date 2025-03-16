#include <iostream>
#include <thread>
#include <unistd.h>

#include "HttpServer.h"
#include "flashdb.h"
#include "hlog.h"
#include "kconfig.h"
#include "led.h"
#include "machine.h"
#include "router.h"
#include "led.h"
#include "hlog.h"
#include "kconfig.h"

using namespace std;

void handler(int sig);

/* default KV nodes */

static int boot_count = 0;
static int boot_time = 0;

static struct fdb_default_kv_node default_kv_table[] = {
    {"boot_count", &boot_count, sizeof(boot_count)}, /* int type KV */
    {"boot_time", &boot_time, sizeof(boot_time)},    /* int array type KV */
};
/* KVDB object */
static struct fdb_kvdb kvdb = {0};

int flashdb_thread()
{
    struct fdb_blob blob;
    int boot_count = 0;

    struct fdb_default_kv default_kv;
    int res = 0;

    default_kv.kvs = default_kv_table;
    default_kv.num = sizeof(default_kv_table) / sizeof(default_kv_table[0]);

    res = fdb_kvdb_init(&kvdb, "env", "easyflash", &default_kv, NULL);
    if (res != FDB_NO_ERR)
    {
        return -1;
    }
    fdb_kv_get_blob(&kvdb, "boot_count",
                    fdb_blob_make(&blob, &boot_count, sizeof(boot_count)));
    if (blob.saved.len > 0)
    {
        LOGI("get the 'boot_count' value is %d\n", boot_count);
        boot_count += 1;
    }
    else
    {
        LOGW("get the 'boot_count' failed\n");
    }

    /* change the "boot_count" KV's value */
    fdb_kv_set_blob(&kvdb, "boot_count",
                    fdb_blob_make(&blob, &boot_count, sizeof(boot_count)));
    LOGI("set the 'boot_count' value to %d\n", boot_count);

    while (1)
    {
        fdb_kv_get_blob(&kvdb, "boot_count",
                        fdb_blob_make(&blob, &boot_count, sizeof(boot_count)));
        sleep(1);
    }
    return 0;
}

int main()
{
    LOGI("start Monitor System");
    signal(SIGINT, handler);
    signal(SIGTERM, handler);
    LOGI("start Monitor System");
    const char *led = CONFIG_LED_STATUS;
    led_object led_green(led);
    led_green.set_timer_trigger(1000, 1000);

    // start monitor
    std::thread thread_monitor(monitor_thread);
    std::thread thread_flash(flashdb_thread);

    // start http server
    hv::HttpServer g_http_server;
    hv::HttpService g_http_service;

    Router::Register(g_http_service);
    g_http_server.registerHttpService(&g_http_service);
    g_http_server.run(":8000");
    return 0;
}

void handler(int sig)
{
    LOGI("Received signal %d, cleaning up and exiting.", sig);
    const char* led = CONFIG_LED_NETWORK;
    led_object led_green(led);
    led_green.set_trigger(LED_TRIG_NONE);
    exit(0);
}
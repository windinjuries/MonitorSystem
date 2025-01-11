#include <iostream>
#include <thread>
#include <unistd.h>

#include "machine.h"
#include "HttpServer.h"
#include "router.h"
#include "led.h"
#include "hlog.h"

using namespace std;
int main()
{
    LOGI("start Monitor System");
    const char *led = "green_led";
    led_object led_green(led);
    led_green.set_timer_trigger(1000, 1000);

    // start monitor
    std::thread t1(monitor_thread);

    // start http server
    hv::HttpServer g_http_server;
    hv::HttpService g_http_service;

    Router::Register(g_http_service);
    g_http_server.registerHttpService(&g_http_service);
    g_http_server.run(":8000");
    return 0;
}
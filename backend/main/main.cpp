#include "easylogginghelper.hpp"
#include "hv/HttpServer.h"
#include "led.hpp"
#include "machine.h"
#include "router.h"
#include <iostream>
#include <thread>

using namespace std;

int main()
{
    el::InitEasylogging();

    led_object led_green("green_led");
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
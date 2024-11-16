#include <iostream>
#include <thread>
#include "hv/HttpServer.h"
#include "http/router.h"
#include "machine/machine.h"
#include "led/led.hpp"
#include "easyloggingpp/easylogginghelper.hpp"

using namespace std;

int main() 
{
    el::InitEasylogging();

    led_object led_green("green_led");
    led_green.set_timer_trigger(1000, 1000);

    // start monitor
    std::thread t1(monitor_thread);

    //start http server
    hv::HttpServer g_http_server;
    hv::HttpService g_http_service;

    Router::Register(g_http_service);
    g_http_server.registerHttpService(&g_http_service);
    g_http_server.run(":8000");
    return 0;
}
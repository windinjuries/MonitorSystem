#include "hv/HttpServer.h"
#include "http/router.h"
#include "machine.h"
#include <iostream>
#include <thread>
using namespace std;
int main() 
{
    // start monitor
    std::thread(machine_info.monitor_thread);
    //start http server
    hv::HttpServer g_http_server;
    hv::HttpService g_http_service;
    Router::Register(g_http_service);
    g_http_server.registerHttpService(&g_http_service);
    g_http_server.run(":8000");
    return 0;
}
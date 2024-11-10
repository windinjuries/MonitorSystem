#include <iostream>
#include <thread>
#include "hv/HttpServer.h"
#include "http/router.h"
#include "machine/machine.h"
#include "easyloggingpp/easylogginghelper.h"

using namespace std;

int main() 
{
    // start monitor
    std::thread t1(monitor_thread);
    el::InitEasylogging();

    //start http server
    hv::HttpServer g_http_server;
    hv::HttpService g_http_service;

    Router::Register(g_http_service);
    g_http_server.registerHttpService(&g_http_service);
    g_http_server.run(":8000");
    return 0;
}
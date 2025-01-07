#ifndef __CPU_H__
#define __CPU_H__

#include "stdint.h"
// #include "easylogginghelper.hpp"

typedef struct 
{
    char name[16];
    long double user;   //用户模式
    long double nice;   //低优先级的用户模式
    long double system; //内核模式
    long double idle;   //空闲处理器时间
} cpu_occupy;

class machine 
{
    private: 
        cpu_occupy cpu_array[2];
        uint32_t index;
        double cpu_usage;
        double memory_usage;

    public:
        int get_memory_using();
        int get_cpu_using();

        double get_cpu_usage();
        double get_memory_usage();
        uint32_t get_local_time();
        int set_led_status();
        int execute_bash(const char* cmd, char* result);
};


extern machine machine_info;

void monitor_thread(void);

#endif //__CPU__H__
#ifndef __CPU_H__
#define __CPU_H__

#include "stdint.h"
// #include "easylogginghelper.hpp"

typedef struct
{
    char name[16];
    unsigned long long user;     // 用户模式
    unsigned long long nice;     // 低优先级的用户模式
    unsigned long long system;   // 内核模式
    unsigned long long idle;     // 空闲处理器时间
    unsigned long long iowait;   // I/O等待时间
    unsigned long long irq;      // 硬中断
    unsigned long long softirq;  // 软中断
    unsigned long long steal;    // 虚拟化环境下其他OS的时间
} cpu_occupy;

class machine
{
    private:
        cpu_occupy cpu_array[2];
        uint32_t index;
        double cpu_usage;
        double memory_usage;

    public:
        machine() : index(0), cpu_usage(0.0), memory_usage(0.0) {
            memset(cpu_array, 0, sizeof(cpu_array));
        }

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
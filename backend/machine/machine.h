#ifndef __CPU_H__
#define __CPU_H__

#ifdef __cplusplus
#include "stdint.h"

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
    private:
        int get_cpu_using();
        int get_memory_using();
    public:
        double get_cpu_usage();
        double get_memory_usage();
        uint32_t get_local_time();
        int execute_bash(const char* cmd, char* result_);
        void monitor_thread();
};


extern machine machine_info;

#endif

#endif //__CPU__H__
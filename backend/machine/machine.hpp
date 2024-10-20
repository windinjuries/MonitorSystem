#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include <iostream>
using namespace std;

typedef struct _CPU_PACKED {
    char name[16];
    unsigned int user;   //用户模式
    unsigned int nice;   //低优先级的用户模式
    unsigned int system; //内核模式
    unsigned int idle;   //空闲处理器时间
} CPU_OCCUPY;

class Machine
{
    private:
        double cpu_use_percent;
        double memory_use_percent;

    public: uint32_t get_local_time();

    public: double get_cpu_info();
};
    
     
uint32_t Machine::get_local_time()
{
    uint32_t t = time(NULL);
    return t;
}


double Machine::get_cpu_info()
{
    FILE *fd;      
    char buff[256];
    CPU_OCCUPY cpu;
    fd = fopen("/proc/stat", "r");
    if (fd != NULL)
    {
        fgets(buff, sizeof(buff), fd);
        if (strstr(buff, "cpu") != NULL) 
        {
            return -1;
        }
    }
    fd = fopen("/proc/stat", "r");
    if (fd == NULL) {
        perror("open /proc/stat failed\n");
        exit(0);
    }
    fgets(buff, sizeof(buff), fd);

    sscanf(buff, "%s %u %u %u %u", 
    cpu.name, &cpu.user, &cpu.nice, &cpu.system, &cpu.idle);
    fclose(fd);
}
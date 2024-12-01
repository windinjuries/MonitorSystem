#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>
#include "hv/hlog.h"

#include "machine.h"

#define CMD_RESULT_BUF_SIZE 4096

machine machine_info;

uint32_t machine::get_local_time()
{
    unsigned int t = time(NULL);
    return t;
}

int machine::set_led_status()
{

}

int machine::get_cpu_using()
{
    long double loadavg;
    cpu_occupy *cpu_array = this->cpu_array;
    cpu_occupy *cpu = &this->cpu_array[this->index++];
    FILE *fp;

    fp = fopen("/proc/stat", "r");
    fscanf(fp, "%s %Lf %Lf %Lf %Lf", cpu->name, &cpu->user, &cpu->nice, &cpu->system, &cpu->idle);
    fclose(fp);

    if(index == 1)
    {
        long double idle_time = cpu[1].idle - cpu[0].idle;
        long double total_time = (cpu[1].user + cpu[1].nice + cpu[1].system + cpu[1].idle) - 
                                 (cpu[0].user + cpu[0].nice + cpu[0].system + cpu[0].idle);
        this->cpu_usage = (total_time - idle_time) / total_time * 100;
    }
    LOG(DEBUG)  << "cpu usage average: " << this->cpu_usage;
    if(index > 1)
    {
        index = 0;
    }
    return 0;
}
 
 
int machine::get_memory_using()
{
    FILE *fd;
    size_t bytes_read;
    size_t read;
    char *line = NULL;
    int freemem = 0;
    int totalmem = 0;

    fd = fopen("/proc/meminfo", "r");
    if (fd == NULL) 
    {
        LOGE("open /proc/meminfo failed\n");
    }

    while ((read = getline(&line, &bytes_read, fd)) != -1) {

        if (strstr(line, "MemTotal") != NULL) 
        {
            sscanf(line, "%*s%d%*s", &totalmem);
        }
        else if (strstr(line, "Memfree") != NULL) 
        {
            sscanf(line, "%*s%d%*s", &freemem);
            break;
        }
    }
    free(line);
    fclose(fd);
    this->memory_usage = (totalmem - freemem) / (double)totalmem;
    return 0;
}
 

int machine::execute_bash(const char* cmd, char* result)
{
    char buf_temp[CMD_RESULT_BUF_SIZE] = {0};
    FILE *ptr = NULL;
    int ret = -1;

    if((ptr = popen(cmd, "r")) != NULL)
    {
        while(fgets(buf_temp, sizeof(buf_temp), ptr) != NULL)
        {
            if(strlen(result) + strlen(buf_temp) > CMD_RESULT_BUF_SIZE)
            {
                break;
            }
            strcat(result, buf_temp); 
        }
        pclose(ptr);
        ptr = NULL;
        ret = 0; 
    }
    else
    {
        printf("popen %s error\n", cmd);
        ret = -1;
    }
    return ret;
}

double machine::get_cpu_usage()
{
    return this->cpu_usage;
}

double machine::get_memory_usage()
{
    return this->memory_usage;
}

void monitor_thread()
{
    while(1)
    {
        machine_info.get_cpu_using();
        machine_info.get_cpu_using();
        sleep(1);
    }
}
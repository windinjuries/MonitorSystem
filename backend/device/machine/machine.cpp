#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>

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
    FILE *fp;
    cpu_occupy *cpu = &this->cpu_array[this->index];

    fp = fopen("/proc/stat", "r");
    if (fp == NULL)
    {
        return -1;
    }

    // 读取完整的CPU时间字段
    int ret = fscanf(fp, "%s %llu %llu %llu %llu %llu %llu %llu %llu",
                     cpu->name,
                     &cpu->user,
                     &cpu->nice,
                     &cpu->system,
                     &cpu->idle,
                     &cpu->iowait,
                     &cpu->irq,
                     &cpu->softirq,
                     &cpu->steal);
    fclose(fp);

    if (ret < 5)  // 至少需要读取前5个字段
    {
        return -1;
    }

    // 第一次只记录，不计算
    if (this->index == 0)
    {
        this->index = 1;
        return 0;
    }

    // 计算CPU使用率
    cpu_occupy *prev = &this->cpu_array[0];
    cpu_occupy *curr = &this->cpu_array[1];

    // 计算总的空闲时间（idle + iowait）
    unsigned long long prev_idle = prev->idle + prev->iowait;
    unsigned long long curr_idle = curr->idle + curr->iowait;

    // 计算总的CPU时间
    unsigned long long prev_total = prev->user + prev->nice + prev->system + prev->idle +
                                    prev->iowait + prev->irq + prev->softirq + prev->steal;
    unsigned long long curr_total = curr->user + curr->nice + curr->system + curr->idle +
                                    curr->iowait + curr->irq + curr->softirq + curr->steal;

    // 计算差值
    unsigned long long total_diff = curr_total - prev_total;
    unsigned long long idle_diff = curr_idle - prev_idle;

    // 避免除零错误
    if (total_diff == 0)
    {
        this->cpu_usage = 0.0;
    }
    else
    {
        this->cpu_usage = (double)(total_diff - idle_diff) / total_diff * 100.0;
    }

    // 重置index，下次覆盖旧数据
    this->index = 0;

    return 0;
}
 
 
int machine::get_memory_using()
{
    FILE *fd;
    size_t bytes_read;
    size_t read;
    char *line = NULL;
    long totalmem = 0;
    long freemem = 0;
    long buffers = 0;
    long cached = 0;
    long memavailable = 0;
    bool found_total = false;
    bool found_available = false;

    fd = fopen("/proc/meminfo", "r");
    if (fd == NULL)
    {
        return -1;
    }

    // 读取内存信息
    while ((read = getline(&line, &bytes_read, fd)) != -1)
    {
        if (strstr(line, "MemTotal:") != NULL)
        {
            sscanf(line, "MemTotal: %ld kB", &totalmem);
            found_total = true;
        }
        else if (strstr(line, "MemAvailable:") != NULL)
        {
            sscanf(line, "MemAvailable: %ld kB", &memavailable);
            found_available = true;
        }
        else if (strstr(line, "Buffers:") != NULL)
        {
            sscanf(line, "Buffers: %ld kB", &buffers);
        }
        else if (strstr(line, "Cached:") != NULL)
        {
            sscanf(line, "Cached: %ld kB", &cached);
        }

        // 找到所有需要的信息后退出
        if (found_total && found_available)
        {
            break;
        }
    }

    if (line)
    {
        free(line);
    }
    fclose(fd);

    // 计算内存使用率
    if (totalmem > 0)
    {
        if (memavailable > 0)
        {
            // 优先使用 MemAvailable（更准确，包含可回收的缓存）
            this->memory_usage = (double)(totalmem - memavailable) / totalmem;
        }
        else
        {
            // 回退方案：MemAvailable 不可用时使用传统方法
            // 已使用 = total - free - buffers - cached
            freemem = totalmem - buffers - cached;
            this->memory_usage = (double)(totalmem - freemem) / totalmem;
        }
    }
    else
    {
        return -1;
    }

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
    return this->memory_usage * 100;
}

void monitor_thread()
{
    while(1)
    {
        // CPU利用率需要两次采样计算
        machine_info.get_cpu_using();  // 第一次采样
        machine_info.get_memory_using();
        sleep(1);
        machine_info.get_cpu_using();  // 第二次采样并计算
    }
}
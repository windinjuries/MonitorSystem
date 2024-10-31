#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
 

#define CMD_RESULT_BUF_SIZE 4096
typedef struct {
    char name[16];
    long double user;   //用户模式
    long double nice;   //低优先级的用户模式
    long double system; //内核模式
    long double idle;   //空闲处理器时间
} cpu_occupy;

double getCpuUsingInfo()
{
    long double loadavg;
    cpu_occupy cpu1, cpu2;
    FILE *fp;

    // 读取第一次CPU信息
    fp = fopen("/proc/stat", "r");
    fscanf(fp, "%s %Lf %Lf %Lf %Lf", cpu1.name, &cpu1.user, &cpu1.nice, &cpu1.system, &cpu1.idle);
    fclose(fp);
    sleep(1); 

    // 读取第二次CPU信息
    fp = fopen("/proc/stat", "r");
    fscanf(fp, "%s %Lf %Lf %Lf %Lf", cpu2.name, &cpu2.user, &cpu2.nice, &cpu2.system, &cpu2.idle);
    fclose(fp);

    // 计算CPU利用率
    long double idle_time = cpu2.idle - cpu1.idle;
    long double total_time = (cpu2.user + cpu2.nice + cpu2.system + cpu2.idle) - (cpu1.user + cpu1.nice + cpu1.system + cpu1.idle);
    loadavg = (total_time - idle_time) / total_time * 100;

    printf("CPU Utilization: %.2Lf%%\n", loadavg);
    return loadavg;
}
 
 
// ---- get memory info ---- //
void getMemoryInfo()
{
    FILE *fp = fopen("/proc/meminfo", "r");
    if(NULL == fp)
        printf("failed to open meminfo\n");
    char szTest[1000] = {0};
    while(!feof(fp))
    {
        memset(szTest, 0, sizeof(szTest));
        fgets(szTest, sizeof(szTest) - 1, fp);
        printf("%s", szTest);
    }
    fclose(fp);
}
 

int execute_bash(const char* cmd, char* result_)
{
    char result[CMD_RESULT_BUF_SIZE] = {0};
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
        strcpy(result_, result);
        pclose(ptr);
        ptr = NULL;
        ret = 0; 
    }
    else
    {
        printf("popen %s error\n", cmd);
        ret = -1; // 处理失败
    }
    return ret;
}
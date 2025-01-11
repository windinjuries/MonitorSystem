#include <string.h>
// #include <sys/types.h> 
// #include <sys/stat.h> 
#include <fcntl.h> 
#include <unistd.h> 
#include <stdio.h>
#include "led.h"
// #include "easylogginghelper.hpp"

#define ROOT_DIR "/sys/class/leds/"

static char trigger_type[LED_TRIG_MAX][30] = {
    {"none"},
    {"timer"},
    {"transient"},
};

led_object::led_object(const char * name)
{
    strcpy(this->name, name);
}

led_object::~led_object()
{

}

int led_object::set_trigger(led_trig_t trig)
{
    int fd;
    char path[256] = {0};
    sprintf(path, "%s%s/trigger",ROOT_DIR, this->name);

    fd = open(path, O_RDWR); 
    if (fd < 0)
    {
        // LOG(WARNING) << "no led device: " << this->name; 
        return -1;
    } 
    int len = strlen(trigger_type[trig]);
    if (len != write(fd, trigger_type[trig], len)) 
    {

        // LOG(WARNING) << "led device: " << this->name << "write trigger fail"; 
        return -1;
    }
    return 0;
}

int led_object::set_timer_trigger(uint32_t on, uint32_t off)
{
    int fd = 0;
    int len = 0;
    char path[256] = {0};
    char param[32] = {0};
    sprintf(path, "%s%s/trigger", ROOT_DIR, this->name);

    fd = open(path, O_RDWR); 
    if (fd < 0)
    {
        // LOG(WARNING) << "open " << path << " fail"; 
        printf("open %s fail\n", path);
        return -1;
    } 
    len = strlen(trigger_type[LED_TRIG_TIMER]);
    if (len != write(fd, trigger_type[LED_TRIG_TIMER], len)) 
    {

        // LOG(WARNING) << "led device: " << this->name << " write trigger fail"; 
        printf("led device: %s write trigger fail\n", this->name);
        return -1;
    }
    close(fd);

    sprintf(path, "%s%s/delay_on", ROOT_DIR, this->name);
    fd = open(path, O_RDWR); 
    if (fd < 0)
    {
        // LOG(WARNING) << "open " << path << "fail"; 
        printf("open %s fail\n", path);
        return -1;
    } 
    sprintf(param, "%d", on);
    len = strlen(param);
    if (len != write(fd, param, len)) 
    {
        // LOG(WARNING) << "led device: " << this->name << " write delay on fail"; 
        printf("led device: %s write delay on fail\n", this->name);
        return -1;
    }
    close(fd);

    sprintf(path, "%s%s/delay_off", ROOT_DIR, this->name);
    fd = open(path, O_RDWR); 
    if (fd < 0)
    {
        // LOG(WARNING) << "open " << path << "fail"; 
        printf("open %s fail\n", path);
        return -1;
    } 
    len = strlen(param);
    if (len != write(fd, param, len)) 
    {
        // LOG(WARNING) << "led device: " << this->name << " write delay off fail"; 
        printf("led device: %s write delay off fail\n", this->name);
        return -1;
    }
    close(fd);
    return 0;
}

int led_object::set_transient_trigger(uint32_t duration, uint32_t state)
{
    int fd = 0;
    int len = 0;
    char path[256] = {0};
    char param[32] = {0};
    sprintf(path, "%s%s/trigger",ROOT_DIR, this->name);

    fd = open(path, O_RDWR); 
    if (fd < 0)
    {
        // LOG(WARNING) << "open " << path << " fail"; 
        return -1;
    } 
    len = strlen(trigger_type[LED_TRIG_TRANSIENT]);
    if (len != write(fd, trigger_type[LED_TRIG_TRANSIENT], len)) 
    {

        // LOG(WARNING) << "led device: " << this->name << " write trigger fail"; 
        return -1;
    }

    close(fd);
    sprintf(path, "%s%s/state", ROOT_DIR, this->name);
    fd = open(path, O_RDWR); 
    if (fd < 0)
    {
        // LOG(WARNING) << "open " << path << " fail"; 
        return -1;
    }
    sprintf(param, "%d", state);
    len = strlen(param);
    if (len != write(fd, param, len)) 
    {
        // LOG(WARNING) << "led device: " << this->name << " write state fail"; 
        return -1;
    }

    sprintf(path, "%s%s/duration", ROOT_DIR, this->name);
    fd = open(path, O_RDWR); 
    if (fd < 0)
    {
        // LOG(WARNING) << "open " << path << "fail"; 
        return -1;
    } 

    sprintf(param, "%d", duration);
    len = strlen(param);
    if (len != write(fd, param, len)) 
    {
        // LOG(WARNING) << "led device: " << this->name << " write during fail"; 
        return -1;
    }

    sprintf(path, "%s%s/activate", ROOT_DIR, this->name);
    fd = open(path, O_RDWR); 
    if (fd < 0)
    {
        // LOG(WARNING) << "open " << path << "fail"; 
        return -1;
    } 
    sprintf(param, "%d", 0x01);
    len = strlen(param);
    if (len != write(fd, param, len)) 
    if (len != write(fd, param, strlen(param))) 
    {
        // LOG(WARNING) << "led device: " << this->name << " write during fail"; 
        return -1;
    }
    return 0;
    
}
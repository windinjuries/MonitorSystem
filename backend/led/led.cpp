#include <string.h>
// #include <sys/types.h> 
// #include <sys/stat.h> 
#include <fcntl.h> 
#include <unistd.h> 
#include <stdio.h>
#include "led.hpp"
#include "easylogginghelper.hpp"

#define ROOT_DIR "/sys/class/leds/"

static char trigger_type[LED_TRIG_MAX][30] = {
    {"none"},
    {"timer"},
    {"transient"},
};

led_object::led_object(char * name)
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
        LOG(WARNING) << "no led device: " << this->name; 
        return -1;
    } 
    int len = strlen(trigger_type[trig]);
    if (len != write(fd, trigger_type[trig], len)) 
    {

        LOG(WARNING) << "led device: " << this->name << "write trigger fail"; 
        return -1;
    }
    return 0;
}

int led_object::set_timer_trigger(uint32_t on, uint32_t off)
{
    int fd = 0;
    int len = 0;
    char path[256] = {0};
    sprintf(path, "%s%s/trigger",ROOT_DIR, this->name);

    fd = open(path, O_RDWR); 
    if (fd < 0)
    {
        LOG(WARNING) << "open " << path << " fail"; 
        return -1;
    } 
    len = strlen(trigger_type[LED_TRIG_TIMER]);
    if (len != write(fd, trigger_type[LED_TRIG_TIMER], len)) 
    {

        LOG(WARNING) << "led device: " << this->name << " write trigger fail"; 
        return -1;
    }

    sprintf(path, "%s%s/delay_on", ROOT_DIR, this->name);
    fd = open(path, O_RDWR); 
    if (fd < 0)
    {
        LOG(WARNING) << "open " << path << "fail"; 
        return -1;
    } 
    len = strlen(trigger_type[LED_TRIG_TIMER]);
    if (len != write(fd, trigger_type[LED_TRIG_TIMER], len)) 
    {
        LOG(WARNING) << "led device: " << this->name << " write delay on fail"; 
        return -1;
    }

    sprintf(path, "%s%s/delay_off", ROOT_DIR, this->name);
    fd = open(path, O_RDWR); 
    if (fd < 0)
    {
        LOG(WARNING) << "open " << path << "fail"; 
        return -1;
    } 
    len = strlen(trigger_type[LED_TRIG_TIMER]);
    if (len != write(fd, trigger_type[LED_TRIG_TIMER], len)) 
    {

        LOG(WARNING) << "led device: " << this->name << " write delay off fail"; 
        return -1;
    }

    sprintf(path, "%s%s/delay_on", ROOT_DIR, this->name);
    return 0;
}
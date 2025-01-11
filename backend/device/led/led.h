#ifndef __LED_H__
#define __LED_H__

#include "stdint.h"
// #include "easylogginghelper.hpp"

typedef enum led_trigger
{
    LED_TRIG_NONE = 0x00,
    LED_TRIG_TIMER = 0x01,
    LED_TRIG_TRANSIENT = 0x02,
    LED_TRIG_MAX = 0x03,
}led_trig_t;


class led_object 
{
public: 
    char name[16];
    uint32_t fd;
    uint32_t trigger;
public:
    led_object(const char * name);
    ~led_object();
    int set_trigger(led_trig_t trig);
    int set_timer_trigger(uint32_t on, uint32_t off);
    int set_transient_trigger(uint32_t duration, uint32_t state);
};

#endif // __LED_H__
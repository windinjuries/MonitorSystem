
#ifndef __MODBUS_CRC_H__
#define __MODBUS_CRC_H__

#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif  

extern uint16_t crc16_modbus(const uint8_t *data, uint32_t length);

#ifdef __cplusplus
}   
#endif

#endif  // __MODBUS_CRC_H__
#ifndef __CPU_H__
#define __CPU_H__
#include "cpu.h"

#ifdef __cplusplus
extern "C"
{
    double getCpuUsingInfo();

    int execute_bash(const char* cmd, char* result_);
}

#endif

#endif //__CPU__H__
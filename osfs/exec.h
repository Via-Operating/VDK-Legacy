#ifndef EXEC_H
#define EXEC_H

#include "../libsa/stdio.h"

typedef struct
{
    uint8_t Developer[11];      // Developer name
    void (*entrypoint)();       // Entry point
    bool isSys;                 // Is it a system file?
} VD_Exec_Header;

#endif
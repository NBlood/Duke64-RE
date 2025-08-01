#include <os_internal.h>
#include <rcp.h>
#include "piint.h"

#define UPDATE_REG(reg, var)           \
    if (cHandle->var != pihandle->var) \
        IO_WRITE(reg, pihandle->var);

s32 osEPiRawReadIo(OSPiHandle *pihandle, u32 devAddr, u32 *data)
{
    register u32 stat;

    WAIT_ON_IOBUSY(stat);
    
    *data = IO_READ(pihandle->baseAddress | devAddr);
    return 0;
}

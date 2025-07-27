#include "common.h"
#include "build.h"


int gettilesizx(unsigned short picnum)
{
    if (gettileid(picnum) == 1)
        return 0;
    return tileinfo[gettileid(picnum)].tilesizx;
}

int gettilesizy(unsigned short picnum)
{
    if (gettileid(picnum) == 1)
        return 0;
    return tileinfo[gettileid(picnum)].tilesizy;
}

int getpicanm(unsigned short picnum)
{
    return tileinfo[gettileid(picnum)].picanm;
}

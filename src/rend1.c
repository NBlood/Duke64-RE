#include "common.h"
#include "duke3d.h"

#define MOVESECTNUM 40
#define MOVESECTVTXNUM 1266

extern unsigned short ms_list_cnt;
extern unsigned short ms_vtx_cnt;

extern short ms_dx[MOVESECTVTXNUM];
extern short ms_dy[MOVESECTVTXNUM];
extern unsigned short ms_list[MOVESECTNUM];
extern unsigned short ms_list_vtxptr[MOVESECTNUM];
extern int ms_x;
extern int ms_y;
extern unsigned short ms_angle;

#pragma GLOBAL_ASM("nonmatchings/src/rend1/func_80059420.s")

#pragma GLOBAL_ASM("nonmatchings/src/rend1/func_80059468.s")

#pragma GLOBAL_ASM("nonmatchings/src/rend1/func_800594A8.s")

float anglemod(float a)
{
    while (a < 0.0f) a += 360.0f;
    while (a >= 360.0f) a -= 360.0f;
    return a;
}

float getangledelta(float a1, float a2)
{
    if (a1 <= a2)
    {
        a2 -= a1;
        if (a2 <= 180.0f)
            return a2;
        else
            return a2 - 360.0f;
    }
    else
    {
        a1 -= a2;
        if (a1 <= 180.0f)
            return -a1;
        else
            return -(a1 - 360.0f);
    }
}

float getanglef2(float x1, float y1, float x2, float y2)
{
    return 270.0f - (getanglef(y2 - y1, x2 - x1) * 57.29577637f);
}

#pragma GLOBAL_ASM("nonmatchings/src/rend1/scansector.s")

#pragma GLOBAL_ASM("nonmatchings/src/rend1/occladdwall.s")

#pragma GLOBAL_ASM("nonmatchings/src/rend1/drawrooms_.s")

void _memcpy(void *dst, void *src, unsigned long size)
{
    char *d = (char*)dst;
    char *s = (char*)src;

    while (size > 0)
    {
        size--;
        *d++ = *s++;
    }
}

void fastmemcpy(void *dst, void *src, unsigned long size)
{
    long *d = (long *)dst;
    long *s = (long *)src;

    if ((long)s & 3)
    {
        _memcpy(d, s, size);
        return;
    }
    if ((long)d & 3)
    {
        _memcpy(d, s, size);
        return;
    }
    if (size & 3)
    {
        _memcpy(d, s, size);
        return;
    }

    size >>= 2;
    while (size > 0)
    {
        size--;
        *d++ = *s++;
    }
}

void _memset(void *dst, char value, unsigned long size)
{
    char *d = (char *)dst;

    while (size > 0)
    {
        size--;
        *d++ = value;
    }
}


void clearbufbyte(unsigned long dst, char value, unsigned int size)
{
    char val = value;
    long *d = (long *)dst;

    if ((long)d & 3)
    {
        _memset(d, val, size);
        return;
    }
    if (size & 3)
    {
        _memset(d, val, size);
        return;
    }

    size >>= 2;
    while (size > 0)
    {
        size--;
        *d++ = val | (val << 8) | (val << 16) | val << 24;
    }
}

void floorupdatez(unsigned short sectnum)
{
    vertex_t *vtx;
    int floorz;
    unsigned short floorvtxcount;
    unsigned short i;

    vtx = &((vertex_t*)VERTEXBASE)[sector[sectnum].floorvertex];
    floorvtxcount = sector[sectnum].floorvtxcount * 3;

    if ((sector[sectnum].floorstat & 2) == 2)
    {
        for (i = 0; i < floorvtxcount; i++)
        {
            vtx->z = getflorzofslope(sectnum, vtx->x * 2, vtx->y * 2) >> 5;
            vtx++;
        }
    }
    else
    {
        floorz = sector[sectnum].floorz >> 5;
        if (vtx->z != floorz)
        {
            for (i = 0; i < floorvtxcount; i++)
            {
                vtx->z = floorz;
                vtx++;
            }
        }
    }
}

void updateceilz(unsigned short sectnum)
{
    vertex_t *vtx;
    int ceilingz;
    unsigned short ceilingvtxcount;
    unsigned short i;

    vtx = &VERTEXBASE[sector[sectnum].ceilingvertex];
    ceilingvtxcount = sector[sectnum].ceilingvtxcount * 3;

    if ((sector[sectnum].ceilingstat & 2) == 2)
    {
        for (i = 0; i < ceilingvtxcount; i++)
        {
            vtx->z = getceilzofslope(sectnum, vtx->x * 2, vtx->y * 2) >> 5;
            vtx++;
        }
    }
    else
    {
        ceilingz = sector[sectnum].ceilingz >> 5;
        if (vtx->z != ceilingz)
        {
            for (i = 0; i < ceilingvtxcount; i++)
            {
                vtx->z = ceilingz;
                vtx++;
            }
        }
    }
}

void resetmovesectorinfo(void)
{
    ms_list_cnt = 0;
    ms_vtx_cnt = 0;
    clearbufbyte((long)ms_list, -1, sizeof(ms_list));
    clearbufbyte((long)ms_list_vtxptr, -1, sizeof(ms_list_vtxptr));
    clearbufbyte((long)ms_dx, -1, sizeof(ms_dx));
    clearbufbyte((long)ms_dy, -1, sizeof(ms_dy));
}

void ms_add_internal(unsigned short sectnum)
{
    unsigned short vtxnum;
    vertex_t *vtx;
    unsigned short i, j;

    vtxnum = 0;

    for (i = 0; i < MOVESECTNUM; i++)
    {
        if (ms_list[i] == sectnum)
            return;
    }

    if ((sector[sectnum].ceilingstat & 0x40) == 0x40)
        vtxnum += sector[sectnum].ceilingvtxcount * 3;

    if ((sector[sectnum].floorstat & 0x40) == 0x40)
        vtxnum += sector[sectnum].floorvtxcount * 3;

    if ((sector[sectnum].ceilingstat & 0x40) == 0x40)
        vtx = &VERTEXBASE[sector[sectnum].ceilingvertex];

    if ((sector[sectnum].floorstat & 0x40) == 0x40)
        vtx = &VERTEXBASE[sector[sectnum].floorvertex];

    ms_list[ms_list_cnt] = sectnum;
    ms_list_vtxptr[ms_list_cnt] = ms_vtx_cnt;
    ms_list_cnt++;

    if (ms_list_cnt > MOVESECTNUM)
    {
        while(1);
        //printf("_moveSectListCnt: %d\n", _moveSectListCnt);
        //exit(0);
    }

    for (j = 0; j < vtxnum; j++)
    {
        ms_dx[ms_vtx_cnt] = vtx->x - ms_x;
        ms_dy[ms_vtx_cnt] = vtx->y - ms_y;
        vtx++;
        ms_vtx_cnt++;
        if (ms_vtx_cnt > MOVESECTVTXNUM)
        {
            //printf("_moveSectVtxCnt: %d\n", _moveSectVtxCnt);
            //exit(0);
            while(1);
        }
    }
}

void ms_add(unsigned short sectnum, int x, int y)
{
    unsigned short i, lotag;

    ms_x = x / 2;
    ms_y = y / 2;
    lotag = sector[sectnum].lotag;
    ms_add_internal(sectnum);
    if (lotag != 0)
    {
        for (i = sector[sectnum].wallptr; i < sector[sectnum].wallptr + sector[sectnum].wallnum; i++)
        {
            if (wall[i].nextsector != -1 && sector[wall[i].nextsector].lotag == lotag)
                ms_add_internal(wall[i].nextsector);
        }
    }
}

void ms_update_internal(unsigned short sectnum)
{
    long x2;
    long y2;
    unsigned short vtxnum;
    unsigned short i, j, k;
    vertex_t *vtx;

    vtxnum = 0;
    //vtx = NULL;

    if ((sector[sectnum].ceilingstat & 0x40) == 0x40)
        vtxnum = sector[sectnum].ceilingvtxcount * 3;

    if ((sector[sectnum].floorstat & 0x40) == 0x40)
        vtxnum += sector[sectnum].floorvtxcount * 3;

    if ((sector[sectnum].ceilingstat & 0x40) == 0x40)
        vtx = &VERTEXBASE[sector[sectnum].ceilingvertex];

    if ((sector[sectnum].floorstat & 0x40) == 0x40)
        vtx = &VERTEXBASE[sector[sectnum].floorvertex];


    for (j = 0; j < MOVESECTNUM; j++)
    {
        if (ms_list[j] == sectnum)
            goto rotate;
    }
    return;

rotate:
    k = ms_list_vtxptr[j];
    for (i = 0; i < vtxnum; i++)
    {
        rotatepoint(0, 0, ms_dx[k] << 1, ms_dy[k] << 1, ms_angle & 0x7FF, &x2, &y2);
        vtx->x = (ms_x + x2) / 2;
        vtx->y = (ms_y + y2) / 2;
        vtx++;
        k++;
    }
}

void ms_update(unsigned short sectnum, unsigned short ang, int x, int y)
{
    unsigned short i, lotag;
    
    ms_angle = ang;
    ms_x = x;
    ms_y = y;
    lotag = sector[sectnum].lotag;
    ms_update_internal(sectnum);
    if(lotag != 0)
    {
        for (i = sector[sectnum].wallptr; i < sector[sectnum].wallptr+sector[sectnum].wallnum; i++)
        {
            if (wall[i].nextsector != -1 && sector[wall[i].nextsector].lotag == lotag)
                ms_update_internal(wall[i].nextsector);
        }
    }
}

#pragma GLOBAL_ASM("nonmatchings/src/rend1/adjustceilingpanning.s")

#pragma GLOBAL_ASM("nonmatchings/src/rend1/adjustfloorpanning.s")

#pragma GLOBAL_ASM("nonmatchings/src/rend1/resetboardtimer.s")

#pragma GLOBAL_ASM("nonmatchings/src/rend1/demoinput.s")

#pragma GLOBAL_ASM("nonmatchings/src/rend1/D_80104740.s")

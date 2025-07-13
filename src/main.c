#include "common.h"
#include "os.h"
#include "rcp.h"
#include "sched.h"
#include "gbi.h"

typedef struct {
	char f_0[0x50600];
} gfx_pool;

char D_8010AC80;
void *D_8010AC84; // polygondata
char D_8010AC88[16384];
OSThread idleThread;
char D_8010EE38[16384];
OSThread mainThread;
char D_80112FE8[16384];
OSThread contThread;
char D_80117198[16384];
OSThread audioThread;
char D_8011B348[16384];
OSSched scheduler;
char D_8011F5D0[16384];
OSScClient D_801235D0;
OSScClient D_801235D8;
OSScClient D_801235E0;
OSMesg D_801235E8[8];
OSMesgQueue D_80123608;
OSMesgQueue D_80123620;
OSMesgQueue controllerthreadsync;
OSMesgQueue retraceMessageQ;
OSMesgQueue contr_msqueue;
OSMesgQueue D_80123680;
OSMesgQueue D_80123698;
OSMesg D_801236B0[8];
OSMesg D_801236D0[1];
OSMesg D_801236D8[8];
OSMesg D_801236F8[8];
OSMesg D_80123718[8];
OSMesg D_80123738[1];
OSIoMesg D_80123740;
gfx_pool gfxpool[2];
OSScTask D_801C4358[2];
u64 clockfreq;
void* D_801C4410; // viewinfo
void* D_801C4414; // paletteoffset
Gfx* pkt; // pkt
u64 D_801C4420;
u64 D_801C4428;
char D_801C4430[16]; // unused?
char D_801C4440[SP_DRAM_STACK_SIZE8];
char D_801C4840[19120]; // fixme

extern int framecounter;

extern char D_80000400[1]; // framebuffer
extern char D_803DA800[1]; // depth buffer


void idle(void*);
void mainproc(void*);

void boot(void)
{
	// fixme
	"FRAME TIME %d\n";
	"0x%X (0x%X)\n";
	"Code size left = %dk\n";
	"STATIC OVERFLOW\n";
	"Gone over by %dk\n";
	osViBlack(1);
	osInitialize();
	osCreateThread(&idleThread, 1, idle, 0, &D_8010EE38[16384], 10);
	osStartThread(&idleThread);
}

void idle(void *a1)
{
	osCreatePiManager(0x96, &D_80123608, D_801235E8, 8);
	osViBlack(1);
	osCreateThread(&mainThread, 3, mainproc, a1, &D_80112FE8[16384], 10);
	osStartThread(&mainThread);
	osSetThreadPri(&idleThread, 0);
	
	while (1) {}
}

void copyfromrom(char* dest, u32 src, u32 length)
{
	u32 status;
	u32 ptr = 0;

	osWritebackDCacheAll();

	while (length)
	{
		u32 cnt = length;
		if (cnt > 0x4000)
			cnt = 0x4000;
		do
		{
			status = osPiGetStatus();
		} while ((status & PI_STATUS_IO_BUSY) != 0 || (status & PI_STATUS_DMA_BUSY) != 0 || (status & PI_STATUS_ERROR) != 0);
		osPiStartDma(&D_80123740, 0, 0, src + ptr, dest + ptr, cnt, &D_80123620);
		osRecvMesg(&D_80123620, NULL, OS_MESG_BLOCK);
		length -= cnt;
		ptr += cnt;
	}
}

void determineclockfreq(void)
{
	OSTime t1;
	OSTime t2;
	while (retraceMessageQ.msgCount <= retraceMessageQ.validCount)
		osRecvMesg(&retraceMessageQ, NULL, OS_MESG_BLOCK);

	osRecvMesg(&retraceMessageQ, NULL, OS_MESG_BLOCK);
	t1 = osGetTime();
	osRecvMesg(&retraceMessageQ, NULL, OS_MESG_BLOCK);
	t2 = osGetTime();
	clockfreq = t2 - t1;
}

void func_8004B704()
{
	gDPSetDepthImage(pkt++, OS_K0_TO_PHYSICAL(D_803DA800));
	gDPSetCycleType(pkt++, G_CYC_FILL);
	gDPSetColorImage(pkt++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 320, OS_K0_TO_PHYSICAL(D_803DA800));
	gDPSetFillColor(pkt++, GPACK_ZDZ(G_MAXFBZ, 0) << 16 | GPACK_ZDZ(G_MAXFBZ, 0));
	gDPScisFillRectangle(pkt++, 0, 0, 319, 239);
	gDPPipeSync(pkt++);
	gDPSetCycleType(pkt++, G_CYC_1CYCLE);
}

void func_8004B7D4()
{
	gDPSetCycleType(pkt++, G_CYC_FILL);
	gDPSetColorImage(pkt++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 320, OS_K0_TO_PHYSICAL(D_80000400 + 0x25800 * framecounter));
	gDPSetFillColor(pkt++, GPACK_RGBA5551(0,0,0,1) << 16 | GPACK_RGBA5551(0,0,0,1));
	gDPScisFillRectangle(pkt++, 0, 0, 319, 239);
}

#pragma GLOBAL_ASM("nonmatchings/src/main/func_8004B890.s")

#pragma GLOBAL_ASM("nonmatchings/src/main/func_8004B9E4.s")

#pragma GLOBAL_ASM("nonmatchings/src/main/func_8004BA4C.s")

#pragma GLOBAL_ASM("nonmatchings/src/main/func_8004BB84.s")

#pragma GLOBAL_ASM("nonmatchings/src/main/func_8004BBB4.s")

#pragma GLOBAL_ASM("nonmatchings/src/main/func_8004BD60.s")

#pragma GLOBAL_ASM("nonmatchings/src/main/mainproc.s")

#pragma GLOBAL_ASM("nonmatchings/src/main/func_8004C1F8.s")

#pragma GLOBAL_ASM("nonmatchings/src/main/func_8004C344.s")

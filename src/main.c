#include "common.h"
#include "os.h"
#include "rcp.h"
#include "sched.h"
#include "gbi.h"
#include "ucode.h"

#define min(a,b) ((a) < (b) ? (a) : (b))

typedef struct {
	char _f_[16];
} polygondata_t;

typedef struct {
	char _f_0[0x480];
	char _f_480[0x180];
	polygondata_t f_600[10240];
	Gfx f_28600[20480];
} gfxpool_t;

float sky_re = 0.f;
float sky_r = 0.f;
float sky_ge = 0.f;
float sky_g = 0.f;
float sky_be = 0.f;
float sky_b = 0.f;
signed char gamestate = 11;
int gfxtaskcreated = 0;
OSScMsg D_800D36D0 = { 2 };
int framecounter = 0;
s64 prevtime = 0;

char D_8010AC80;
polygondata_t *polygondata;
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
gfxpool_t gfxpool[2];
OSScTask D_801C4358[2];
s64 clockfreq;
gfxpool_t *viewinfo;
char* paletteoffset;
Gfx* pkt;
OSTime nowtime;
OSTime nowtime2;
char D_801C4430[16]; // unused?
char D_801C4440[SP_DRAM_STACK_SIZE8];
char D_801C4840[19120]; // fixme

extern char game_inactive;

extern char D_80000400[1]; // framebuffer
extern char D_803DA800[1]; // depth buffer

extern short D_801012C8;
extern short D_801012CC;
extern char D_801CB922;
extern short D_801012D0;

extern int totalclock;
extern int totalclocklock;

extern char dl_0[1];
extern char dl_1[1];

extern char palettes[1];

extern char palettes_rom[1];
extern char palettes_rom_end[1];

OSMesgQueue *func_8004C654(OSSched*);

void osCreateScheduler2(OSSched* sc, void* stack, OSPri priority,
	u8 numFields);


void idle(void*);
void mainproc(void*);
void menus(void);
void display_game(void);
void display_tileviewer(void);
void display_bonus(int);
void display_intro(void);
void display_loadgame(void);
void display_savegame(void);
void display_logos(void);
void display_pickslot(void);
void loadgame(void);
void stopsong(void);
void stopallsounds(void);
void agecache(void);
void gameplay(void);
void animatemodels(void);

void controllerThread(void* a1);
void __amMain(void* a1);

void soundSetupMsgQueue(void);
void soundInit(void);
void controllervibdisable(void);
void initControllers(void);
void setdefaultcontrollermode(void);
void loadefs(void);
void cacheandtileinit(void);
void updatecontrollers(void);
void __amHandleFrameMsg(void);

extern signed char doinitrumble;

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

void display(void)
{
	switch (gamestate)
	{
		case 1:
			menus();
			break;
		case 3:
			display_game();
			menus();
			break;
		case 2:
			display_game();
			break;
		case 5:
			display_tileviewer();
			break;
		case 7:
			display_bonus(0);
			break;
		case 8:
			display_intro();
			break;
		case 9:
			display_loadgame();
			break;
		case 10:
			display_savegame();
			break;
		case 11:
			display_logos();
			break;
		case 12:
			display_pickslot();
			break;
		case 13:
			gamestate = 1;
			loadgame();
			game_inactive = 0;
			D_801012C8 = 10;
			D_801012CC = -120;
			D_801CB922 = 0;
			break;
		case 14:
			if (!D_801012D0)
			{
				stopsong();
				D_801012D0 = 1;
			}
			stopallsounds();
			break;
	}
}

void timerupdate(void)
{
	totalclock = totalclock + 4;
	totalclocklock = totalclocklock + 4;
	agecache();
	if (gamestate == 2)
	{
		gameplay();
		animatemodels();
	}
}

void set_gfxdata(void)
{
	viewinfo = &gfxpool[framecounter];
	pkt = viewinfo->f_28600;
	polygondata = viewinfo->f_600;

	gSPSegment(pkt++, 0, 0);
	gSPSegment(pkt++, 1, OS_K0_TO_PHYSICAL(paletteoffset));
	gSPSegment(pkt++, 2, OS_K0_TO_PHYSICAL(D_80000400 + framecounter * 0x25800));

	//gSPDisplayList(pkt++, SEGMENT_ADDR(1, 0x1888));
	//gSPDisplayList(pkt++, SEGMENT_ADDR(1, 0x1860));
	gSPDisplayList(pkt++, dl_1);
	gSPDisplayList(pkt++, dl_0);
}

void setupgfx(void)
{
	set_gfxdata();
	func_8004B704();
	func_8004B7D4();
}

#if 1
void creategfxtask(void)
{
	OSScTask* task;

	gDPFullSync(pkt++);
	gSPEndDisplayList(pkt++);

	task = &D_801C4358[framecounter];
	task->list.t.data_ptr = (u64*)viewinfo->f_28600;
	task->list.t.data_size = (pkt - viewinfo->f_28600) * sizeof(Gfx);
	task->list.t.type = M_GFXTASK;
	task->list.t.flags = 0;
	task->list.t.ucode_boot = (u64*)rspbootTextStart;
	task->list.t.ucode_boot_size = (u32)rspbootTextEnd - (u32)rspbootTextStart;
	task->list.t.ucode = (u64*)gspFast3DTextStart;
	task->list.t.ucode_data = (u64*)gspFast3DDataStart;
	task->list.t.ucode_data_size = SP_UCODE_DATA_SIZE;
	task->list.t.dram_stack = (u64*)&D_801C4440;
	task->list.t.dram_stack_size = SP_DRAM_STACK_SIZE8;
	task->list.t.output_buff = 0;
	task->list.t.output_buff_size = 0;
	task->list.t.yield_data_ptr = (u64*)&D_801C4840;
	task->list.t.yield_data_size = 0x900;
	task->next = 0;
	task->flags = OS_SC_NEEDS_RDP | OS_SC_NEEDS_RSP | OS_SC_LAST_TASK | OS_SC_SWAPBUFFER;
	task->msgQ = &contr_msqueue;
	task->msg = &D_800D36D0;
	task->framebuffer = (void*)(&D_80000400 + (framecounter * 0x25800));
	osWritebackDCacheAll();
	osSendMesg(func_8004C654(&scheduler), task, OS_MESG_BLOCK);
}
#else
#pragma GLOBAL_ASM("nonmatchings/src/main/creategfxtask.s")
#endif

void updatetime(void)
{
	nowtime = osGetTime();
}

#ifdef NON_MATCHING
void mainproc(void* a1)
{
	osCreateMesgQueue(&D_80123620, D_801236B0, 8);
	paletteoffset = palettes;
	if ((u32)paletteoffset + (u32)palettes_rom_end - (u32)palettes_rom > (u32)0x802c5000)
	{
		while(1) {}
	}
	copyfromrom(palettes, (u32)palettes_rom, (u32)palettes_rom_end - (u32)palettes_rom);
	osCreateScheduler2(&scheduler, &D_8011F5D0[16384], 64, 1);
	osCreateThread(&contThread, 5, controllerThread, a1, &D_80117198[16384], 20);
	osCreateThread(&audioThread, 6, __amMain, a1, &D_8011B348[16384], 30);
	soundSetupMsgQueue();
	soundInit();
	controllervibdisable();
	initControllers();
	osCreateMesgQueue(&controllerthreadsync, D_801236D0, 1);
	osCreateMesgQueue(&retraceMessageQ, D_801236D8, 8);
	osScAddClient(&scheduler, &D_801235D0, &retraceMessageQ);
	osStartThread(&contThread);
	osStartThread(&audioThread);
	determineclockfreq();
	setdefaultcontrollermode();
	loadefs();
	cacheandtileinit();

	nowtime2 = osGetTime();

	for (;;)
	{
		nowtime2 = osGetTime();
		prevtime = prevtime - nowtime + nowtime2;
		if (prevtime > clockfreq * 6)
			prevtime = clockfreq * 6;
		nowtime = nowtime2;
		while (prevtime > clockfreq * 2)
		{
			timerupdate();
			prevtime -= clockfreq * 2;
		}

		if (prevtime >= 0)
		{
			timerupdate();
			setupgfx();
			display();
			prevtime -= clockfreq * 2;
			if (gfxtaskcreated)
			{
				osRecvMesg(&controllerthreadsync, NULL, OS_MESG_BLOCK);
				gfxtaskcreated = 0;
				if (retraceMessageQ.msgCount <= retraceMessageQ.validCount) {
					osRecvMesg(&retraceMessageQ, NULL, OS_MESG_BLOCK);
				}
				osRecvMesg(&retraceMessageQ, NULL, OS_MESG_BLOCK);
			}
			creategfxtask();
			gfxtaskcreated = 1;
			framecounter ^= 1;
		}
	}
}
#else
#pragma GLOBAL_ASM("nonmatchings/src/main/mainproc.s")
#endif

void controllerThread(void* a1)
{
	OSScMsg* msg;
	osCreateMesgQueue(&contr_msqueue, D_801236F8, 8);
	osScAddClient(&scheduler, &D_801235D8, &contr_msqueue);
	while (1)
	{
		osRecvMesg(&contr_msqueue, (OSMesg*)&msg, OS_MESG_BLOCK);
		switch (msg->type)
		{
			case OS_SC_RETRACE_MSG:
				updatecontrollers();
				break;
			case OS_SC_DONE_MSG:
				osSendMesg(&controllerthreadsync, NULL, OS_MESG_NOBLOCK);
				break;
			case OS_SC_PRE_NMI_MSG:
				osViSetYScale(1.0);
				osViBlack(1);
				doinitrumble = 1;
				gamestate = 14;
				break;
		}
	}
}

void __amMain(void* a1)
{
	OSScMsg* msg;
	osCreateMesgQueue(&D_80123698, D_80123738, 1);
	osCreateMesgQueue(&D_80123680, D_80123718, 8);
	osScAddClient(&scheduler, &D_801235E0, &D_80123680);
	while (1)
	{
		osRecvMesg(&D_80123680, (OSMesg*)&msg, OS_MESG_BLOCK);
		if (msg->type == OS_SC_RETRACE_MSG)
		{
			__amHandleFrameMsg();
		}
	}
}

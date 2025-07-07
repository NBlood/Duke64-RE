// "Build Engine & Tools" Copyright (c) 1993-1997 Ken Silverman
// Ken Silverman's official web site: "http://www.advsys.net/ken"
// See the included license file "BUILDLIC.TXT" for license info.

#include "common.h"
#include "build.h"
#include "pragmas.h"

#define MAXCLIPNUM 512
#define MAXCLIPDIST 1024

extern short editstatus;
extern long rxi[8], ryi[8];
extern short clipnum, hitwalls[4];

typedef struct { long x1, y1, x2, y2; } linetype;
extern linetype clipit[MAXCLIPNUM];
extern short clipsectorlist[MAXCLIPNUM], clipsectnum;
extern short clipobjectval[MAXCLIPNUM];
extern long totalclocklock;

int mulscale(long long a, long long b, long long c)
{
	return (a * b) >> c;
}

int mulscale1(long long a, long long b)
{
	return (a * b) >> 1;
}

int mulscale2(long long a, long long b)
{
	return (a * b) >> 2;
}

int mulscale3(long long a, long long b)
{
	return (a * b) >> 3;
}

int mulscale4(long long a, long long b)
{
	return (a * b) >> 4;
}

int mulscale5(long long a, long long b)
{
	return (a * b) >> 5;
}

int mulscale6(long long a, long long b)
{
	return (a * b) >> 6;
}

int mulscale7(long long a, long long b)
{
	return (a * b) >> 7;
}

int mulscale8(long long a, long long b)
{
	return (a * b) >> 8;
}

int mulscale9(long long a, long long b)
{
	return (a * b) >> 9;
}

int mulscale10(long long a, long long b)
{
	return (a * b) >> 10;
}

int mulscale11(long long a, long long b)
{
	return (a * b) >> 11;
}

int mulscale12(long long a, long long b)
{
	return (a * b) >> 12;
}

int mulscale13(long long a, long long b)
{
	return (a * b) >> 13;
}

int mulscale14(long long a, long long b)
{
	return (a * b) >> 14;
}

int mulscale15(long long a, long long b)
{
	return (a * b) >> 15;
}

int mulscale16(long long a, long long b)
{
	return (a * b) >> 16;
}

int mulscale17(long long a, long long b)
{
	return (a * b) >> 17;
}

int mulscale18(long long a, long long b)
{
	return (a * b) >> 18;
}

int mulscale19(long long a, long long b)
{
	return (a * b) >> 19;
}

int mulscale20(long long a, long long b)
{
	return (a * b) >> 20;
}

int mulscale21(long long a, long long b)
{
	return (a * b) >> 21;
}

int mulscale22(long long a, long long b)
{
	return (a * b) >> 22;
}

int mulscale23(long long a, long long b)
{
	return (a * b) >> 23;
}

int mulscale24(long long a, long long b)
{
	return (a * b) >> 24;
}

int mulscale25(long long a, long long b)
{
	return (a * b) >> 25;
}

int mulscale26(long long a, long long b)
{
	return (a * b) >> 26;
}

int mulscale27(long long a, long long b)
{
	return (a * b) >> 27;
}

int mulscale28(long long a, long long b)
{
	return (a * b) >> 28;
}

int mulscale29(long long a, long long b)
{
	return (a * b) >> 29;
}

int mulscale30(long long a, long long b)
{
	return (a * b) >> 30;
}

int mulscale31(long long a, long long b)
{
	return (a * b) >> 31;
}

int mulscale32(long long a, long long b)
{
	return (a * b) >> 32;
}

int dmulscale1(long long a, long long b, long long c, long long d)
{
    long long r1 = a * b;
    long long r2 = c * d;
	return (r1 + r2) >> 1;
}

int dmulscale2(long long a, long long b, long long c, long long d)
{
    long long r1 = a * b;
    long long r2 = c * d;
	return (r1 + r2) >> 2;
}

int dmulscale3(long long a, long long b, long long c, long long d)
{
    long long r1 = a * b;
    long long r2 = c * d;
	return (r1 + r2) >> 3;
}

int dmulscale4(long long a, long long b, long long c, long long d)
{
    long long r1 = a * b;
    long long r2 = c * d;
	return (r1 + r2) >> 4;
}

int dmulscale5(long long a, long long b, long long c, long long d)
{
    long long r1 = a * b;
    long long r2 = c * d;
	return (r1 + r2) >> 5;
}

int dmulscale6(long long a, long long b, long long c, long long d)
{
    long long r1 = a * b;
    long long r2 = c * d;
	return (r1 + r2) >> 6;
}

int dmulscale7(long long a, long long b, long long c, long long d)
{
    long long r1 = a * b;
    long long r2 = c * d;
	return (r1 + r2) >> 7;
}

int dmulscale8(long long a, long long b, long long c, long long d)
{
    long long r1 = a * b;
    long long r2 = c * d;
	return (r1 + r2) >> 8;
}

int dmulscale9(long long a, long long b, long long c, long long d)
{
    long long r1 = a * b;
    long long r2 = c * d;
	return (r1 + r2) >> 9;
}

int dmulscale10(long long a, long long b, long long c, long long d)
{
    long long r1 = a * b;
    long long r2 = c * d;
	return (r1 + r2) >> 10;
}

int dmulscale11(long long a, long long b, long long c, long long d)
{
    long long r1 = a * b;
    long long r2 = c * d;
	return (r1 + r2) >> 11;
}

int dmulscale12(long long a, long long b, long long c, long long d)
{
    long long r1 = a * b;
    long long r2 = c * d;
	return (r1 + r2) >> 12;
}

int dmulscale13(long long a, long long b, long long c, long long d)
{
    long long r1 = a * b;
    long long r2 = c * d;
	return (r1 + r2) >> 13;
}

int dmulscale14(long long a, long long b, long long c, long long d)
{
    long long r1 = a * b;
    long long r2 = c * d;
	return (r1 + r2) >> 14;
}

int dmulscale15(long long a, long long b, long long c, long long d)
{
    long long r1 = a * b;
    long long r2 = c * d;
	return (r1 + r2) >> 15;
}

int dmulscale16(long long a, long long b, long long c, long long d)
{
    long long r1 = a * b;
    long long r2 = c * d;
	return (r1 + r2) >> 16;
}

int dmulscale17(long long a, long long b, long long c, long long d)
{
    long long r1 = a * b;
    long long r2 = c * d;
	return (r1 + r2) >> 17;
}

int dmulscale18(long long a, long long b, long long c, long long d)
{
    long long r1 = a * b;
    long long r2 = c * d;
	return (r1 + r2) >> 18;
}

int dmulscale19(long long a, long long b, long long c, long long d)
{
    long long r1 = a * b;
    long long r2 = c * d;
	return (r1 + r2) >> 19;
}

int dmulscale20(long long a, long long b, long long c, long long d)
{
    long long r1 = a * b;
    long long r2 = c * d;
	return (r1 + r2) >> 20;
}

int dmulscale21(long long a, long long b, long long c, long long d)
{
    long long r1 = a * b;
    long long r2 = c * d;
	return (r1 + r2) >> 21;
}

int dmulscale22(long long a, long long b, long long c, long long d)
{
    long long r1 = a * b;
    long long r2 = c * d;
	return (r1 + r2) >> 22;
}

int dmulscale23(long long a, long long b, long long c, long long d)
{
    long long r1 = a * b;
    long long r2 = c * d;
	return (r1 + r2) >> 23;
}

int dmulscale24(long long a, long long b, long long c, long long d)
{
    long long r1 = a * b;
    long long r2 = c * d;
	return (r1 + r2) >> 24;
}

int dmulscale25(long long a, long long b, long long c, long long d)
{
    long long r1 = a * b;
    long long r2 = c * d;
	return (r1 + r2) >> 25;
}

int dmulscale26(long long a, long long b, long long c, long long d)
{
    long long r1 = a * b;
    long long r2 = c * d;
	return (r1 + r2) >> 26;
}

int dmulscale27(long long a, long long b, long long c, long long d)
{
    long long r1 = a * b;
    long long r2 = c * d;
	return (r1 + r2) >> 27;
}

int dmulscale28(long long a, long long b, long long c, long long d)
{
    long long r1 = a * b;
    long long r2 = c * d;
	return (r1 + r2) >> 28;
}

int dmulscale29(long long a, long long b, long long c, long long d)
{
    long long r1 = a * b;
    long long r2 = c * d;
	return (r1 + r2) >> 29;
}

int dmulscale30(long long a, long long b, long long c, long long d)
{
    long long r1 = a * b;
    long long r2 = c * d;
	return (r1 + r2) >> 30;
}

int dmulscale31(long long a, long long b, long long c, long long d)
{
    long long r1 = a * b;
    long long r2 = c * d;
	return (r1 + r2) >> 31;
}

int dmulscale32(long long a, long long b, long long c, long long d)
{
    long long r1 = a * b;
    long long r2 = c * d;
	return (r1 + r2) >> 32;
}

int divscale(long long a, long long b, char c)
{
    return (a << c) / b;
}

#if 0

int divscale1(long long a, long long b)
{
    return (a << 1) / b;
}

int divscale2(long long a, long long b)
{
    return (a << 2) / b;
}

int divscale3(long long a, long long b)
{
    return (a << 3) / b;
}

int divscale4(long long a, long long b)
{
    return (a << 4) / b;
}

int divscale5(long long a, long long b)
{
    return (a << 5) / b;
}

int divscale6(long long a, long long b)
{
    return (a << 6) / b;
}

int divscale7(long long a, long long b)
{
    return (a << 7) / b;
}

int divscale8(long long a, long long b)
{
    return (a << 8) / b;
}

int divscale9(long long a, long long b)
{
    return (a << 9) / b;
}

int divscale10(long long a, long long b)
{
    return (a << 10) / b;
}

int divscale11(long long a, long long b)
{
    return (a << 11) / b;
}

int divscale12(long long a, long long b)
{
    return (a << 12) / b;
}

int divscale13(long long a, long long b)
{
    return (a << 13) / b;
}

int divscale14(long long a, long long b)
{
    return (a << 14) / b;
}

int divscale15(long long a, long long b)
{
    return (a << 15) / b;
}

int divscale16(long long a, long long b)
{
    return (a << 16) / b;
}

int divscale17(long long a, long long b)
{
    return (a << 17) / b;
}

int divscale18(long long a, long long b)
{
    return (a << 18) / b;
}

int divscale19(long long a, long long b)
{
    return (a << 19) / b;
}

int divscale20(long long a, long long b)
{
    return (a << 20) / b;
}

int divscale21(long long a, long long b)
{
    return (a << 21) / b;
}

int divscale22(long long a, long long b)
{
    return (a << 22) / b;
}

int divscale23(long long a, long long b)
{
    return (a << 23) / b;
}

int divscale24(long long a, long long b)
{
    return (a << 24) / b;
}

int divscale25(long long a, long long b)
{
    return (a << 25) / b;
}

int divscale26(long long a, long long b)
{
    return (a << 26) / b;
}

int divscale27(long long a, long long b)
{
    return (a << 27) / b;
}

int divscale28(long long a, long long b)
{
    return (a << 28) / b;
}

int divscale29(long long a, long long b)
{
    return (a << 29) / b;
}

int divscale30(long long a, long long b)
{
    return (a << 30) / b;
}

int divscale31(long long a, long long b)
{
    return (a << 31) / b;
}

int divscale32(long long a, long long b)
{
    return (a << 32) / b;
}
#else

#pragma GLOBAL_ASM("asm/nonmatchings/engine/divscale1.s")
#pragma GLOBAL_ASM("asm/nonmatchings/engine/divscale2.s")
#pragma GLOBAL_ASM("asm/nonmatchings/engine/divscale3.s")
#pragma GLOBAL_ASM("asm/nonmatchings/engine/divscale4.s")
#pragma GLOBAL_ASM("asm/nonmatchings/engine/divscale5.s")
#pragma GLOBAL_ASM("asm/nonmatchings/engine/divscale6.s")
#pragma GLOBAL_ASM("asm/nonmatchings/engine/divscale7.s")
#pragma GLOBAL_ASM("asm/nonmatchings/engine/divscale8.s")
#pragma GLOBAL_ASM("asm/nonmatchings/engine/divscale9.s")
#pragma GLOBAL_ASM("asm/nonmatchings/engine/divscale10.s")
#pragma GLOBAL_ASM("asm/nonmatchings/engine/divscale11.s")
#pragma GLOBAL_ASM("asm/nonmatchings/engine/divscale12.s")
#pragma GLOBAL_ASM("asm/nonmatchings/engine/divscale13.s")
#pragma GLOBAL_ASM("asm/nonmatchings/engine/divscale14.s")
#pragma GLOBAL_ASM("asm/nonmatchings/engine/divscale15.s")
#pragma GLOBAL_ASM("asm/nonmatchings/engine/divscale16.s")
#pragma GLOBAL_ASM("asm/nonmatchings/engine/divscale17.s")
#pragma GLOBAL_ASM("asm/nonmatchings/engine/divscale18.s")
#pragma GLOBAL_ASM("asm/nonmatchings/engine/divscale19.s")
#pragma GLOBAL_ASM("asm/nonmatchings/engine/divscale20.s")
#pragma GLOBAL_ASM("asm/nonmatchings/engine/divscale21.s")
#pragma GLOBAL_ASM("asm/nonmatchings/engine/divscale22.s")
#pragma GLOBAL_ASM("asm/nonmatchings/engine/divscale23.s")
#pragma GLOBAL_ASM("asm/nonmatchings/engine/divscale24.s")
#pragma GLOBAL_ASM("asm/nonmatchings/engine/divscale25.s")
#pragma GLOBAL_ASM("asm/nonmatchings/engine/divscale26.s")
#pragma GLOBAL_ASM("asm/nonmatchings/engine/divscale27.s")
#pragma GLOBAL_ASM("asm/nonmatchings/engine/divscale28.s")
#pragma GLOBAL_ASM("asm/nonmatchings/engine/divscale29.s")
#pragma GLOBAL_ASM("asm/nonmatchings/engine/divscale30.s")
#pragma GLOBAL_ASM("asm/nonmatchings/engine/divscale31.s")
#pragma GLOBAL_ASM("asm/nonmatchings/engine/divscale32.s")

#endif

int scale(int a, int b, int c)
{
    long long r = (long long)a * (long long)b;
    return r / c;
}

#pragma GLOBAL_ASM("asm/nonmatchings/engine/nsqrtasm.s")

int klabs(int a) {
    if (a < 0)
        return -a;
    return a;
}

int ksgn(int a) {
    if (a < 0) {
        return -1;
    }
    return 1;
}

extern short D_80276F20;

void initengine()
{
	D_80276F20 = -1;
	totalclock = 0;
	xdim = 320;
	ydim = 240;
}

int clipinsidebox(long x, long y, short wallnum, long walldist)
{
	walltype *wal;
	long x1, y1, x2, y2, r;

	r = (walldist<<1);
	wal = &wall[wallnum];     x1 = wal->x+walldist-x; y1 = wal->y+walldist-y;
	wal = &wall[wal->point2]; x2 = wal->x+walldist-x; y2 = wal->y+walldist-y;

	if ((x1 < 0) && (x2 < 0)) return(0);
	if ((y1 < 0) && (y2 < 0)) return(0);
	if ((x1 >= r) && (x2 >= r)) return(0);
	if ((y1 >= r) && (y2 >= r)) return(0);

	x2 -= x1; y2 -= y1;
	if (x2*(walldist-y1) >= y2*(walldist-x1))  //Front
	{
		if (x2 > 0) x2 *= (0-y1); else x2 *= (r-y1);
		if (y2 > 0) y2 *= (r-x1); else y2 *= (0-x1);
		return(x2 < y2);
	}
	if (x2 > 0) x2 *= (r-y1); else x2 *= (0-y1);
	if (y2 > 0) y2 *= (0-x1); else y2 *= (r-x1);
	return((x2 >= y2)<<1);
}

int clipinsideboxline(long x, long y, long x1, long y1, long x2, long y2, long walldist)
{
	long r;

	r = (walldist<<1);

	x1 += walldist-x; x2 += walldist-x;
	if ((x1 < 0) && (x2 < 0)) return(0);
	if ((x1 >= r) && (x2 >= r)) return(0);

	y1 += walldist-y; y2 += walldist-y;
	if ((y1 < 0) && (y2 < 0)) return(0);
	if ((y1 >= r) && (y2 >= r)) return(0);

	x2 -= x1; y2 -= y1;
	if (x2*(walldist-y1) >= y2*(walldist-x1))  //Front
	{
		if (x2 > 0) x2 *= (0-y1); else x2 *= (r-y1);
		if (y2 > 0) y2 *= (r-x1); else y2 *= (0-x1);
		return(x2 < y2);
	}
	if (x2 > 0) x2 *= (r-y1); else x2 *= (0-y1);
	if (y2 > 0) y2 *= (0-x1); else y2 *= (r-x1);
	return((x2 >= y2)<<1);
}

int inside (long x, long y, short sectnum)
{
	walltype *wal;
	long i, x1, y1, x2, y2;
	unsigned long cnt;

	if ((sectnum < 0) || (sectnum >= numsectors)) return(-1);

	cnt = 0;
	wal = &wall[sector[sectnum].wallptr];
	i = sector[sectnum].wallnum;
	do
	{
		y1 = wal->y-y; y2 = wall[wal->point2].y-y;
		if ((y1^y2) < 0)
		{
			x1 = wal->x-x; x2 = wall[wal->point2].x-x;
			if ((x1^x2) >= 0) cnt ^= x1; else cnt ^= (x1*y2-x2*y1)^y2;
		}
		wal++; i--;
	} while (i);
	return(cnt>>31);
}

int getangle(long xvect, long yvect)
{
	if ((xvect|yvect) == 0) return(0);
	if (xvect == 0) return(512+((yvect<0)<<10));
	if (yvect == 0) return(((xvect<0)<<10));
	if (xvect == yvect) return(256+((xvect<0)<<10));
	if (xvect == -yvect) return(768+((xvect>0)<<10));
	if (klabs2(xvect) > klabs2(yvect))
		return(((radarang[640+scale(160,yvect,xvect)]>>6)+((xvect<0)<<10))&2047);
	return(((radarang[640-scale(160,xvect,yvect)]>>6)+512+((yvect<0)<<10))&2047);
}

#pragma GLOBAL_ASM("asm/nonmatchings/engine/ksqrt.s")

int setsprite(short spritenum, long newx, long newy, long newz)
{
	short bad, j, tempsectnum;

	sprite[spritenum].x = newx;
	sprite[spritenum].y = newy;
	sprite[spritenum].z = newz;

	tempsectnum = sprite[spritenum].sectnum;
	updatesector(newx,newy,&tempsectnum);
	if (tempsectnum < 0)
		return(-1);
	if (tempsectnum != sprite[spritenum].sectnum)
		changespritesect(spritenum,tempsectnum);

	return(0);
}

int animateoffs(short tilenum, short fakevar)
{
	long i, k, offs;

	offs = 0;
	i = (totalclocklock>>((getpicanm(tilenum)>>24)&15));
	if ((getpicanm(tilenum)&63) > 0)
	{
		switch(getpicanm(tilenum)&192)
		{
			case 64:
				k = (i%((getpicanm(tilenum)&63)<<1));
				if (k < (getpicanm(tilenum)&63))
					offs = k;
				else
					offs = (((getpicanm(tilenum)&63)<<1)-k);
				break;
			case 128:
				offs = (i%((getpicanm(tilenum)&63)+1));
					break;
			case 192:
				offs = -(i%((getpicanm(tilenum)&63)+1));
		}
	}
	return(offs);
}


void initspritelists()
{
	long i;
	
	spritecount = 0;

	for (i=0;i<MAXSECTORS;i++)     //Init doubly-linked sprite sector lists
		headspritesect[i] = -1;
	headspritesect[MAXSECTORS] = 0;
	for(i=0;i<MAXSPRITES;i++)
	{
		prevspritesect[i] = i-1;
		nextspritesect[i] = i+1;
		sprite[i].sectnum = MAXSECTORS;
	}
	prevspritesect[0] = -1;
	nextspritesect[MAXSPRITES-1] = -1;


	for(i=0;i<MAXSTATUS;i++)      //Init doubly-linked sprite status lists
		headspritestat[i] = -1;
	headspritestat[MAXSTATUS] = 0;
	for(i=0;i<MAXSPRITES;i++)
	{
		prevspritestat[i] = i-1;
		nextspritestat[i] = i+1;
		sprite[i].statnum = MAXSTATUS;
	}
	prevspritestat[0] = -1;
	nextspritestat[MAXSPRITES-1] = -1;
}

int insertsprite(short sectnum, short statnum)
{
	insertspritestat(statnum);
	spritecount++;
	return(insertspritesect(sectnum));
}

int insertspritesect(short sectnum)
{
	short blanktouse;

	if ((sectnum >= MAXSECTORS) || (headspritesect[MAXSECTORS] == -1))
		return(-1);  //list full

	blanktouse = headspritesect[MAXSECTORS];

	headspritesect[MAXSECTORS] = nextspritesect[blanktouse];
	if (headspritesect[MAXSECTORS] >= 0)
		prevspritesect[headspritesect[MAXSECTORS]] = -1;

	prevspritesect[blanktouse] = -1;
	nextspritesect[blanktouse] = headspritesect[sectnum];
	if (headspritesect[sectnum] >= 0)
		prevspritesect[headspritesect[sectnum]] = blanktouse;
	headspritesect[sectnum] = blanktouse;

	sprite[blanktouse].sectnum = sectnum;

	return(blanktouse);
}

int insertspritestat(short statnum)
{
	short blanktouse;

	if ((statnum >= MAXSTATUS) || (headspritestat[MAXSTATUS] == -1))
		return(-1);  //list full

	blanktouse = headspritestat[MAXSTATUS];

	headspritestat[MAXSTATUS] = nextspritestat[blanktouse];
	if (headspritestat[MAXSTATUS] >= 0)
		prevspritestat[headspritestat[MAXSTATUS]] = -1;

	prevspritestat[blanktouse] = -1;
	nextspritestat[blanktouse] = headspritestat[statnum];
	if (headspritestat[statnum] >= 0)
		prevspritestat[headspritestat[statnum]] = blanktouse;
	headspritestat[statnum] = blanktouse;

	sprite[blanktouse].statnum = statnum;

	return(blanktouse);
}

int deletesprite(short spritenum)
{
	deletespritestat(spritenum);
	spritecount--;
	return(deletespritesect(spritenum));
}

int deletespritesect(short deleteme)
{
	if (sprite[deleteme].sectnum == MAXSECTORS)
		return(-1);

	if (headspritesect[sprite[deleteme].sectnum] == deleteme)
		headspritesect[sprite[deleteme].sectnum] = nextspritesect[deleteme];

	if (prevspritesect[deleteme] >= 0) nextspritesect[prevspritesect[deleteme]] = nextspritesect[deleteme];
	if (nextspritesect[deleteme] >= 0) prevspritesect[nextspritesect[deleteme]] = prevspritesect[deleteme];

	if (headspritesect[MAXSECTORS] >= 0) prevspritesect[headspritesect[MAXSECTORS]] = deleteme;
	prevspritesect[deleteme] = -1;
	nextspritesect[deleteme] = headspritesect[MAXSECTORS];
	headspritesect[MAXSECTORS] = deleteme;

	sprite[deleteme].sectnum = MAXSECTORS;
	return(0);
}

int deletespritestat (short deleteme)
{
	if (sprite[deleteme].statnum == MAXSTATUS)
		return(-1);

	if (headspritestat[sprite[deleteme].statnum] == deleteme)
		headspritestat[sprite[deleteme].statnum] = nextspritestat[deleteme];

	if (prevspritestat[deleteme] >= 0) nextspritestat[prevspritestat[deleteme]] = nextspritestat[deleteme];
	if (nextspritestat[deleteme] >= 0) prevspritestat[nextspritestat[deleteme]] = prevspritestat[deleteme];

	if (headspritestat[MAXSTATUS] >= 0) prevspritestat[headspritestat[MAXSTATUS]] = deleteme;
	prevspritestat[deleteme] = -1;
	nextspritestat[deleteme] = headspritestat[MAXSTATUS];
	headspritestat[MAXSTATUS] = deleteme;

	sprite[deleteme].statnum = MAXSTATUS;
	return(0);
}

int changespritesect(short spritenum, short newsectnum)
{
	if ((newsectnum < 0) || (newsectnum > MAXSECTORS)) return(-1);
	if (sprite[spritenum].sectnum == newsectnum) return(0);
	if (sprite[spritenum].sectnum == MAXSECTORS) return(-1);
	if (deletespritesect(spritenum) < 0) return(-1);
	insertspritesect(newsectnum);
	return(0);
}

int changespritestat(short spritenum, short newstatnum)
{
	if ((newstatnum < 0) || (newstatnum > MAXSTATUS)) return(-1);
	if (sprite[spritenum].statnum == newstatnum) return(0);
	if (sprite[spritenum].statnum == MAXSTATUS) return(-1);
	if (deletespritestat(spritenum) < 0) return(-1);
	insertspritestat(newstatnum);
	return(0);
}

int nextsectorneighborz(short sectnum, long thez, short topbottom, short direction)
{
	walltype *wal;
	long i, testz, nextz;
	short sectortouse;

	if (direction == 1) nextz = 0x7fffffff; else nextz = 0x80000000;

	sectortouse = -1;

	wal = &wall[sector[sectnum].wallptr];
	i = sector[sectnum].wallnum;
	do
	{
		if (wal->nextsector >= 0)
		{
			if (topbottom == 1)
			{
				testz = sector[wal->nextsector].floorz;
				if (direction == 1)
				{
					if ((testz > thez) && (testz < nextz))
					{
						nextz = testz;
						sectortouse = wal->nextsector;
					}
				}
				else
				{
					if ((testz < thez) && (testz > nextz))
					{
						nextz = testz;
						sectortouse = wal->nextsector;
					}
				}
			}
			else
			{
				testz = sector[wal->nextsector].ceilingz;
				if (direction == 1)
				{
					if ((testz > thez) && (testz < nextz))
					{
						nextz = testz;
						sectortouse = wal->nextsector;
					}
				}
				else
				{
					if ((testz < thez) && (testz > nextz))
					{
						nextz = testz;
						sectortouse = wal->nextsector;
					}
				}
			}
		}
		wal++;
		i--;
	} while (i != 0);

	return(sectortouse);
}

int cansee(long x1, long y1, long z1, short sect1, long x2, long y2, long z2, short sect2)
{
	sectortype *sec;
	walltype *wal, *wal2;
	long i, cnt, nexts, x, y, z, cz, fz, dasectnum, dacnt, danum;
	long x21, y21, z21, x31, y31, x34, y34, bot, t;

	if ((x1 == x2) && (y1 == y2)) return(sect1 == sect2);

	x21 = x2-x1; y21 = y2-y1; z21 = z2-z1;

	clipsectorlist[0] = sect1; danum = 1;
	for(dacnt=0;dacnt<danum;dacnt++)
	{
		dasectnum = clipsectorlist[dacnt]; sec = &sector[dasectnum];
		for(cnt=sec->wallnum,wal=&wall[sec->wallptr];cnt>0;cnt--,wal++)
		{
			wal2 = &wall[wal->point2];
			x31 = wal->x-x1; x34 = wal->x-wal2->x;
			y31 = wal->y-y1; y34 = wal->y-wal2->y;

			bot = y21*x34-x21*y34; if (bot <= 0) continue;
			t = y21*x31-x21*y31; if ((t < 0) || (t >= bot)) continue;
			t = y31*x34-x31*y34; if ((t < 0) || (t >= bot)) continue;

			nexts = wal->nextsector;
			if ((nexts < 0) || (wal->cstat&32)) return(0);

			t = divscale24(t,bot);
			x = x1 + mulscale24(x21,t);
			y = y1 + mulscale24(y21,t);
			z = z1 + mulscale24(z21,t);

			getzsofslope((short)dasectnum,x,y,&cz,&fz);
			if ((z <= cz) || (z >= fz)) return(0);
			getzsofslope((short)nexts,x,y,&cz,&fz);
			if ((z <= cz) || (z >= fz)) return(0);

			for(i=danum-1;i>=0;i--) if (clipsectorlist[i] == nexts) break;
			if (i < 0) clipsectorlist[danum++] = nexts;
		}
	}
	for(i=danum-1;i>=0;i--) if (clipsectorlist[i] == sect2) return(1);
	return(0);
}

int hitscan(long xs, long ys, long zs, short sectnum, long vx, long vy, long vz,
	short *hitsect, short *hitwall, short *hitsprite,
	long *hitx, long *hity, long *hitz, unsigned long cliptype)
{
	sectortype *sec;
	walltype *wal, *wal2;
	spritetype *spr;
	long z, zz, x1, y1, z1, x2, y2, z2, x3, y3, x4, y4, intx, inty, intz;
	long topt, topu, bot, dist, offx, offy, cstat;
	long i, j, k, l, tilenum, xoff, yoff, dax, day, daz, daz2;
	long ang, cosang, sinang, xspan, yspan, xrepeat, yrepeat;
	long dawalclipmask, dasprclipmask;
	short tempshortcnt, tempshortnum, dasector, startwall, endwall;
	short nextsector;
	char clipyou;

	*hitsect = -1; *hitwall = -1; *hitsprite = -1;
	if (sectnum < 0) return(-1);

	*hitx = (1<<29)-1; *hity = (1<<29)-1;

	dawalclipmask = (cliptype&65535);
	dasprclipmask = (cliptype>>16);

	clipsectorlist[0] = sectnum;
	tempshortcnt = 0; tempshortnum = 1;
	do
	{
		dasector = clipsectorlist[tempshortcnt]; sec = &sector[dasector];

		x1 = 0x7fffffff;
		if (sec->ceilingstat&2)
		{
			wal = &wall[sec->wallptr]; wal2 = &wall[wal->point2];
			dax = wal2->x-wal->x; day = wal2->y-wal->y;
			i = nsqrtasm(dax*dax+day*day); if (i == 0) continue;
			i = divscale15(sec->ceilingheinum,i);
			dax *= i; day *= i;

			j = (vz<<8)-dmulscale15(dax,vy,-day,vx);
			if (j != 0)
			{
				i = ((sec->ceilingz-zs)<<8)+dmulscale15(dax,ys-wal->y,-day,xs-wal->x);
				if (((i^j) >= 0) && ((klabs2(i)>>1) < klabs2(j)))
				{
					i = divscale30(i,j);
					x1 = xs + mulscale30(vx,i);
					y1 = ys + mulscale30(vy,i);
					z1 = zs + mulscale30(vz,i);
				}
			}
		}
		else if ((vz < 0) && (zs >= sec->ceilingz))
		{
			z1 = sec->ceilingz; i = z1-zs;
			if ((klabs2(i)>>1) < -vz)
			{
				i = divscale30(i,vz);
				x1 = xs + mulscale30(vx,i);
				y1 = ys + mulscale30(vy,i);
			}
		}
		if ((x1 != 0x7fffffff) && (klabs2(x1-xs)+klabs2(y1-ys) < klabs2((*hitx)-xs)+klabs2((*hity)-ys)))
			if (inside(x1,y1,dasector) != 0)
			{
				*hitsect = dasector; *hitwall = -1; *hitsprite = -1;
				*hitx = x1; *hity = y1; *hitz = z1;
			}

		x1 = 0x7fffffff;
		if (sec->floorstat&2)
		{
			wal = &wall[sec->wallptr]; wal2 = &wall[wal->point2];
			dax = wal2->x-wal->x; day = wal2->y-wal->y;
			i = nsqrtasm(dax*dax+day*day); if (i == 0) continue;
			i = divscale15(sec->floorheinum,i);
			dax *= i; day *= i;

			j = (vz<<8)-dmulscale15(dax,vy,-day,vx);
			if (j != 0)
			{
				i = ((sec->floorz-zs)<<8)+dmulscale15(dax,ys-wal->y,-day,xs-wal->x);
				if (((i^j) >= 0) && ((klabs2(i)>>1) < klabs2(j)))
				{
					i = divscale30(i,j);
					x1 = xs + mulscale30(vx,i);
					y1 = ys + mulscale30(vy,i);
					z1 = zs + mulscale30(vz,i);
				}
			}
		}
		else if ((vz > 0) && (zs <= sec->floorz))
		{
			z1 = sec->floorz; i = z1-zs;
			if ((klabs2(i)>>1) < vz)
			{
				i = divscale30(i,vz);
				x1 = xs + mulscale30(vx,i);
				y1 = ys + mulscale30(vy,i);
			}
		}
		if ((x1 != 0x7fffffff) && (klabs2(x1-xs)+klabs2(y1-ys) < klabs2((*hitx)-xs)+klabs2((*hity)-ys)))
			if (inside(x1,y1,dasector) != 0)
			{
				*hitsect = dasector; *hitwall = -1; *hitsprite = -1;
				*hitx = x1; *hity = y1; *hitz = z1;
			}

		startwall = sec->wallptr; endwall = startwall + sec->wallnum;
		for(z=startwall,wal=&wall[startwall];z<endwall;z++,wal++)
		{
			wal2 = &wall[wal->point2];
			x1 = wal->x; y1 = wal->y; x2 = wal2->x; y2 = wal2->y;

			if ((x1-xs)*(y2-ys) < (x2-xs)*(y1-ys)) continue;
			if (rintersect(xs,ys,zs,vx,vy,vz,x1,y1,x2,y2,&intx,&inty,&intz) == 0) continue;

			if (klabs2(intx-xs)+klabs2(inty-ys) >= klabs2((*hitx)-xs)+klabs2((*hity)-ys)) continue;

			nextsector = wal->nextsector;
			if ((nextsector < 0) || (wal->cstat&dawalclipmask))
			{
				*hitsect = dasector; *hitwall = z; *hitsprite = -1;
				*hitx = intx; *hity = inty; *hitz = intz;
				continue;
			}
			getzsofslope(nextsector,intx,inty,&daz,&daz2);
			if ((intz <= daz) || (intz >= daz2))
			{
				*hitsect = dasector; *hitwall = z; *hitsprite = -1;
				*hitx = intx; *hity = inty; *hitz = intz;
				continue;
			}

			for(zz=tempshortnum-1;zz>=0;zz--)
				if (clipsectorlist[zz] == nextsector) break;
			if (zz < 0) clipsectorlist[tempshortnum++] = nextsector;
		}

		for(z=headspritesect[dasector];z>=0;z=nextspritesect[z])
		{
			spr = &sprite[z];
			cstat = spr->cstat;
			if ((cstat&dasprclipmask) == 0) continue;

			x1 = spr->x; y1 = spr->y; z1 = spr->z;
			switch(cstat&48)
			{
				case 0:
					topt = vx*(x1-xs) + vy*(y1-ys); if (topt <= 0) continue;
					bot = vx*vx + vy*vy; if (bot == 0) continue;

					intz = zs+scale(vz,topt,bot);

					i = (gettilesizy(spr->picnum)*spr->yrepeat<<2);
					if (cstat&128) z1 += (i>>1);
					if (getpicanm(spr->picnum)&0x00ff0000) z1 -= ((long)((signed char)((getpicanm(spr->picnum)>>16)&255))*spr->yrepeat<<2);
					if ((intz > z1) || (intz < z1-i)) continue;
					topu = vx*(y1-ys) - vy*(x1-xs);

					offx = scale(vx,topu,bot);
					offy = scale(vy,topu,bot);
					dist = offx*offx + offy*offy;
					i = gettilesizx(spr->picnum)*spr->xrepeat; i *= i;
					if (dist > (i>>7)) continue;
					intx = xs + scale(vx,topt,bot);
					inty = ys + scale(vy,topt,bot);

					if (klabs2(intx-xs)+klabs2(inty-ys) > klabs2((*hitx)-xs)+klabs2((*hity)-ys)) continue;

					*hitsect = dasector; *hitwall = -1; *hitsprite = z;
					*hitx = intx; *hity = inty; *hitz = intz;
					break;
				case 16:
						//These lines get the 2 points of the rotated sprite
						//Given: (x1, y1) starts out as the center point
					tilenum = spr->picnum;
					xoff = (long)((signed char)((getpicanm(tilenum)>>8)&255))+((long)spr->xoffset);
					if ((cstat&4) > 0) xoff = -xoff;
					k = spr->ang; l = spr->xrepeat;
					dax = sintable[k&2047]*l; day = sintable[(k+1536)&2047]*l;
					l = gettilesizx(tilenum); k = (l>>1)+xoff;
					x1 -= mulscale16(dax,k); x2 = x1+mulscale16(dax,l);
					y1 -= mulscale16(day,k); y2 = y1+mulscale16(day,l);

					if ((cstat&64) != 0)   //back side of 1-way sprite
						if ((x1-xs)*(y2-ys) < (x2-xs)*(y1-ys)) continue;

					if (rintersect(xs,ys,zs,vx,vy,vz,x1,y1,x2,y2,&intx,&inty,&intz) == 0) continue;

					if (klabs2(intx-xs)+klabs2(inty-ys) > klabs2((*hitx)-xs)+klabs2((*hity)-ys)) continue;

					k = ((gettilesizy(spr->picnum)*spr->yrepeat)<<2);
					if (cstat&128) daz = spr->z+(k>>1); else daz = spr->z;
					if (getpicanm(spr->picnum)&0x00ff0000) daz -= ((long)((signed char)((getpicanm(spr->picnum)>>16)&255))*spr->yrepeat<<2);
					if ((intz < daz) && (intz > daz-k))
					{
						*hitsect = dasector; *hitwall = -1; *hitsprite = z;
						*hitx = intx; *hity = inty; *hitz = intz;
					}
					break;
				case 32:
					if (vz == 0) continue;
					intz = z1;
					if (((intz-zs)^vz) < 0) continue;
					if ((cstat&64) != 0)
						if ((zs > intz) == ((cstat&8)==0)) continue;

					intx = xs+scale(intz-zs,vx,vz);
					inty = ys+scale(intz-zs,vy,vz);

					if (klabs2(intx-xs)+klabs2(inty-ys) > klabs2((*hitx)-xs)+klabs2((*hity)-ys)) continue;

					tilenum = spr->picnum;
					xoff = (long)((signed char)((getpicanm(tilenum)>>8)&255))+((long)spr->xoffset);
					yoff = (long)((signed char)((getpicanm(tilenum)>>16)&255))+((long)spr->yoffset);
					if ((cstat&4) > 0) xoff = -xoff;
					if ((cstat&8) > 0) yoff = -yoff;

					ang = spr->ang;
					cosang = sintable[(ang+512)&2047]; sinang = sintable[ang];
					xspan = gettilesizx(tilenum); xrepeat = spr->xrepeat;
					yspan = gettilesizy(tilenum); yrepeat = spr->yrepeat;

					dax = ((xspan>>1)+xoff)*xrepeat; day = ((yspan>>1)+yoff)*yrepeat;
					x1 += dmulscale16(sinang,dax,cosang,day)-intx;
					y1 += dmulscale16(sinang,day,-cosang,dax)-inty;
					l = xspan*xrepeat;
					x2 = x1 - mulscale16(sinang,l);
					y2 = y1 + mulscale16(cosang,l);
					l = yspan*yrepeat;
					k = -mulscale16(cosang,l); x3 = x2+k; x4 = x1+k;
					k = -mulscale16(sinang,l); y3 = y2+k; y4 = y1+k;

					clipyou = 0;
					if ((y1^y2) < 0)
					{
						if ((x1^x2) < 0) clipyou ^= (x1*y2<x2*y1)^(y1<y2);
						else if (x1 >= 0) clipyou ^= 1;
					}
					if ((y2^y3) < 0)
					{
						if ((x2^x3) < 0) clipyou ^= (x2*y3<x3*y2)^(y2<y3);
						else if (x2 >= 0) clipyou ^= 1;
					}
					if ((y3^y4) < 0)
					{
						if ((x3^x4) < 0) clipyou ^= (x3*y4<x4*y3)^(y3<y4);
						else if (x3 >= 0) clipyou ^= 1;
					}
					if ((y4^y1) < 0)
					{
						if ((x4^x1) < 0) clipyou ^= (x4*y1<x1*y4)^(y4<y1);
						else if (x4 >= 0) clipyou ^= 1;
					}

					if (clipyou != 0)
					{
						*hitsect = dasector; *hitwall = -1; *hitsprite = z;
						*hitx = intx; *hity = inty; *hitz = intz;
					}
					break;
			}
		}
		tempshortcnt++;
	} while (tempshortcnt < tempshortnum);
	return(0);
}

int neartag (long xs, long ys, long zs, short sectnum, short ange, short *neartagsector, short *neartagwall, short *neartagsprite, long *neartaghitdist, long neartagrange, char tagsearch)
{
	walltype *wal, *wal2;
	spritetype *spr;
	long i, z, zz, xe, ye, ze, x1, y1, z1, x2, y2, z2, intx, inty, intz;
	long topt, topu, bot, dist, offx, offy, vx, vy, vz;
	short tempshortcnt, tempshortnum, dasector, startwall, endwall;
	short nextsector, good;

	*neartagsector = -1; *neartagwall = -1; *neartagsprite = -1;
	*neartaghitdist = 0;

	if (sectnum < 0) return(0);
	if ((tagsearch < 1) || (tagsearch > 3)) return(0);

	vx = mulscale14(sintable[(ange+2560)&2047],neartagrange); xe = xs+vx;
	vy = mulscale14(sintable[(ange+2048)&2047],neartagrange); ye = ys+vy;
	vz = 0; ze = 0;

	clipsectorlist[0] = sectnum;
	tempshortcnt = 0; tempshortnum = 1;

	do
	{
		dasector = clipsectorlist[tempshortcnt];

		startwall = sector[dasector].wallptr;
		endwall = startwall + sector[dasector].wallnum - 1;
		for(z=startwall,wal=&wall[startwall];z<=endwall;z++,wal++)
		{
			wal2 = &wall[wal->point2];
			x1 = wal->x; y1 = wal->y; x2 = wal2->x; y2 = wal2->y;

			nextsector = wal->nextsector;

			good = 0;
			if (nextsector >= 0)
			{
				if ((tagsearch&1) && sector[nextsector].lotag) good |= 1;
				if ((tagsearch&2) && sector[nextsector].hitag) good |= 1;
			}
			if ((tagsearch&1) && wal->lotag) good |= 2;
			if ((tagsearch&2) && wal->hitag) good |= 2;

			if ((good == 0) && (nextsector < 0)) continue;
			if ((x1-xs)*(y2-ys) < (x2-xs)*(y1-ys)) continue;

			if (lintersect(xs,ys,zs,xe,ye,ze,x1,y1,x2,y2,&intx,&inty,&intz) == 1)
			{
				if (good != 0)
				{
					if (good&1) *neartagsector = nextsector;
					if (good&2) *neartagwall = z;
					*neartaghitdist = dmulscale14(intx-xs,sintable[(ange+2560)&2047],inty-ys,sintable[(ange+2048)&2047]);
					xe = intx; ye = inty; ze = intz;
				}
				if (nextsector >= 0)
				{
					for(zz=tempshortnum-1;zz>=0;zz--)
						if (clipsectorlist[zz] == nextsector) break;
					if (zz < 0) clipsectorlist[tempshortnum++] = nextsector;
				}
			}
		}

		for(z=headspritesect[dasector];z>=0;z=nextspritesect[z])
		{
			spr = &sprite[z];

			good = 0;
			if ((tagsearch&1) && spr->lotag) good |= 1;
			if ((tagsearch&2) && spr->hitag) good |= 1;
			if (good != 0)
			{
				x1 = spr->x; y1 = spr->y; z1 = spr->z;

				topt = vx*(x1-xs) + vy*(y1-ys);
				if (topt > 0)
				{
					bot = vx*vx + vy*vy;
					if (bot != 0)
					{
						intz = zs+scale(vz,topt,bot);
						i = gettilesizy(spr->picnum)*spr->yrepeat;
						if (spr->cstat&128) z1 += (i<<1);
						if (getpicanm(spr->picnum)&0x00ff0000) z1 -= ((long)((signed char)((getpicanm(spr->picnum)>>16)&255))*spr->yrepeat<<2);
						if ((intz <= z1) && (intz >= z1-(i<<2)))
						{
							topu = vx*(y1-ys) - vy*(x1-xs);

							offx = scale(vx,topu,bot);
							offy = scale(vy,topu,bot);
							dist = offx*offx + offy*offy;
							i = (gettilesizx(spr->picnum)*spr->xrepeat); i *= i;
							if (dist <= (i>>7))
							{
								intx = xs + scale(vx,topt,bot);
								inty = ys + scale(vy,topt,bot);
								if (klabs2(intx-xs)+klabs2(inty-ys) < klabs2(xe-xs)+klabs2(ye-ys))
								{
									*neartagsprite = z;
									*neartaghitdist = dmulscale14(intx-xs,sintable[(ange+2560)&2047],inty-ys,sintable[(ange+2048)&2047]);
									xe = intx;
									ye = inty;
									ze = intz;
								}
							}
						}
					}
				}
			}
		}

		tempshortcnt++;
	} while (tempshortcnt < tempshortnum);
	return(0);
}

int lintersect(long x1, long y1, long z1, long x2, long y2, long z2, long x3,
			  long y3, long x4, long y4, long *intx, long *inty, long *intz)
{     //p1 to p2 is a line segment
	long x21, y21, x34, y34, x31, y31, bot, topt, topu, t;

	x21 = x2-x1; x34 = x3-x4;
	y21 = y2-y1; y34 = y3-y4;
	bot = x21*y34 - y21*x34;
	if (bot >= 0)
	{
		if (bot == 0) return(0);
		x31 = x3-x1; y31 = y3-y1;
		topt = x31*y34 - y31*x34; if ((topt < 0) || (topt >= bot)) return(0);
		topu = x21*y31 - y21*x31; if ((topu < 0) || (topu >= bot)) return(0);
	}
	else
	{
		x31 = x3-x1; y31 = y3-y1;
		topt = x31*y34 - y31*x34; if ((topt > 0) || (topt <= bot)) return(0);
		topu = x21*y31 - y21*x31; if ((topu > 0) || (topu <= bot)) return(0);
	}
	t = divscale24(topt,bot);
	*intx = x1 + mulscale24(x21,t);
	*inty = y1 + mulscale24(y21,t);
	*intz = z1 + mulscale24(z2-z1,t);
	return(1);
}

int rintersect(long x1, long y1, long z1, long vx, long vy, long vz, long x3,
			  long y3, long x4, long y4, long *intx, long *inty, long *intz)
{     //p1 towards p2 is a ray
	long x34, y34, x31, y31, bot, topt, topu, t;

	x34 = x3-x4; y34 = y3-y4;
	bot = vx*y34 - vy*x34;
	if (bot >= 0)
	{
		if (bot == 0) return(0);
		x31 = x3-x1; y31 = y3-y1;
		topt = x31*y34 - y31*x34; if (topt < 0) return(0);
		topu = vx*y31 - vy*x31; if ((topu < 0) || (topu >= bot)) return(0);
	}
	else
	{
		x31 = x3-x1; y31 = y3-y1;
		topt = x31*y34 - y31*x34; if (topt > 0) return(0);
		topu = vx*y31 - vy*x31; if ((topu > 0) || (topu <= bot)) return(0);
	}
	t = divscale16(topt,bot);
	*intx = x1 + mulscale16(vx,t);
	*inty = y1 + mulscale16(vy,t);
	*intz = z1 + mulscale16(vz,t);
	return(1);
}

int dragpoint(short pointhighlight, long dax, long day)
{
	short cnt, tempshort;

	wall[pointhighlight].x = dax;
	wall[pointhighlight].y = day;

	cnt = MAXWALLS;
	tempshort = pointhighlight;    //search points CCW
	do
	{
		if (wall[tempshort].nextwall >= 0)
		{
			tempshort = wall[wall[tempshort].nextwall].point2;
			wall[tempshort].x = dax;
			wall[tempshort].y = day;
		}
		else
		{
			tempshort = pointhighlight;    //search points CW if not searched all the way around
			do
			{
				if (wall[lastwall(tempshort)].nextwall >= 0)
				{
					tempshort = wall[lastwall(tempshort)].nextwall;
					wall[tempshort].x = dax;
					wall[tempshort].y = day;
				}
				else
				{
					break;
				}
				cnt--;
			}
			while ((tempshort != pointhighlight) && (cnt > 0));
			break;
		}
		cnt--;
	}
	while ((tempshort != pointhighlight) && (cnt > 0));
}

short lastwall(short point)
{
	long i, j, cnt;

	if ((point > 0) && (wall[point-1].point2 == point)) return(point-1);
	i = point;
	cnt = MAXWALLS;
	do
	{
		j = wall[i].point2;
		if (j == point) return(i);
		i = j;
		cnt--;
	} while (cnt > 0);
	return(point);
}

#define addclipline(dax1, day1, dax2, day2, daoval)      \
{                                                        \
	clipit[clipnum].x1 = dax1; clipit[clipnum].y1 = day1; \
	clipit[clipnum].x2 = dax2; clipit[clipnum].y2 = day2; \
	clipobjectval[clipnum] = daoval;                      \
	clipnum++;                                            \
}                                                        \

//long clipmoveboxtracenum = 3;
extern long clipmoveboxtracenum;
int clipmove (long *x, long *y, long *z, short *sectnum,
			 long xvect, long yvect,
			 long walldist, long ceildist, long flordist, unsigned long cliptype)
{
	walltype *wal, *wal2;
	spritetype *spr;
	sectortype *sec, *sec2;
	long i, j, templong1, templong2;
	long oxvect, oyvect, goalx, goaly, intx, inty, lx, ly, retval;
	long k, l, clipsectcnt, startwall, endwall, cstat, dasect;
	long x1, y1, x2, y2, cx, cy, rad, xmin, ymin, xmax, ymax, daz, daz2;
	long bsz, dax, day, xoff, yoff, xspan, yspan, cosang, sinang, tilenum;
	long xrepeat, yrepeat, gx, gy, dx, dy, dasprclipmask, dawalclipmask;
	long hitwall, cnt, clipyou;

	if (((xvect|yvect) == 0) || (*sectnum < 0)) return(0);
	retval = 0;

	oxvect = xvect;
	oyvect = yvect;

	goalx = (*x) + (xvect>>14);
	goaly = (*y) + (yvect>>14);


	clipnum = 0;

	cx = (((*x)+goalx)>>1);
	cy = (((*y)+goaly)>>1);
		//Extra walldist for sprites on sector lines
	gx = goalx-(*x); gy = goaly-(*y);
	rad = nsqrtasm(gx*gx + gy*gy) + MAXCLIPDIST+walldist + 8;
	xmin = cx-rad; ymin = cy-rad;
	xmax = cx+rad; ymax = cy+rad;

	dawalclipmask = (cliptype&65535);        //CLIPMASK0 = 0x00010001
	dasprclipmask = (cliptype>>16);          //CLIPMASK1 = 0x01000040

	clipsectorlist[0] = (*sectnum);
	clipsectcnt = 0; clipsectnum = 1;
	do
	{
		dasect = clipsectorlist[clipsectcnt++];
		sec = &sector[dasect];
		startwall = sec->wallptr; endwall = startwall + sec->wallnum;
		for(j=startwall,wal=&wall[startwall];j<endwall;j++,wal++)
		{
			wal2 = &wall[wal->point2];
			if ((wal->x < xmin) && (wal2->x < xmin)) continue;
			if ((wal->x > xmax) && (wal2->x > xmax)) continue;
			if ((wal->y < ymin) && (wal2->y < ymin)) continue;
			if ((wal->y > ymax) && (wal2->y > ymax)) continue;

			x1 = wal->x; y1 = wal->y; x2 = wal2->x; y2 = wal2->y;

			dx = x2-x1; dy = y2-y1;
			if (dx*((*y)-y1) < ((*x)-x1)*dy) continue;  //If wall's not facing you

			if (dx > 0) dax = dx*(ymin-y1); else dax = dx*(ymax-y1);
			if (dy > 0) day = dy*(xmax-x1); else day = dy*(xmin-x1);
			if (dax >= day) continue;

			clipyou = 0;
			if ((wal->nextsector < 0) || (wal->cstat&dawalclipmask)) clipyou = 1;
			else if (editstatus == 0)
			{
				if (rintersect(*x,*y,0,gx,gy,0,x1,y1,x2,y2,&dax,&day,&daz) == 0)
					dax = *x, day = *y;
				daz = getflorzofslope((short)dasect,dax,day);
				daz2 = getflorzofslope(wal->nextsector,dax,day);

				sec2 = &sector[wal->nextsector];
				if (daz2 < daz-(1<<8))
					if ((sec2->floorstat&1) == 0)
						if ((*z) >= daz2-(flordist-1)) clipyou = 1;
				if (clipyou == 0)
				{
					daz = getceilzofslope((short)dasect,dax,day);
					daz2 = getceilzofslope(wal->nextsector,dax,day);
					if (daz2 > daz+(1<<8))
						if ((sec2->ceilingstat&1) == 0)
							if ((*z) <= daz2+(ceildist-1)) clipyou = 1;
				}
			}

			if (clipyou)
			{
					//Add 2 boxes at endpoints
				bsz = walldist; if (gx < 0) bsz = -bsz;
				addclipline(x1-bsz,y1-bsz,x1-bsz,y1+bsz,(short)j+32768);
				addclipline(x2-bsz,y2-bsz,x2-bsz,y2+bsz,(short)j+32768);
				bsz = walldist; if (gy < 0) bsz = -bsz;
				addclipline(x1+bsz,y1-bsz,x1-bsz,y1-bsz,(short)j+32768);
				addclipline(x2+bsz,y2-bsz,x2-bsz,y2-bsz,(short)j+32768);

				dax = walldist; if (dy > 0) dax = -dax;
				day = walldist; if (dx < 0) day = -day;
				addclipline(x1+dax,y1+day,x2+dax,y2+day,(short)j+32768);
			}
			else
			{
				for(i=clipsectnum-1;i>=0;i--)
					if (wal->nextsector == clipsectorlist[i]) break;
				if (i < 0) clipsectorlist[clipsectnum++] = wal->nextsector;
			}
		}

		for(j=headspritesect[dasect];j>=0;j=nextspritesect[j])
		{
			spr = &sprite[j];
			cstat = spr->cstat;
			if ((cstat&dasprclipmask) == 0) continue;
			x1 = spr->x; y1 = spr->y;
			switch(cstat&48)
			{
				case 0:
					if ((x1 >= xmin) && (x1 <= xmax) && (y1 >= ymin) && (y1 <= ymax))
					{
						k = ((gettilesizy(spr->picnum)*spr->yrepeat)<<2);
						if (cstat&128) daz = spr->z+(k>>1); else daz = spr->z;
						if (getpicanm(spr->picnum)&0x00ff0000) daz -= ((long)((signed char)((getpicanm(spr->picnum)>>16)&255))*spr->yrepeat<<2);
						if (((*z) < daz+ceildist) && ((*z) > daz-k-flordist))
						{
							bsz = (spr->clipdist<<2)+walldist; if (gx < 0) bsz = -bsz;
							addclipline(x1-bsz,y1-bsz,x1-bsz,y1+bsz,(short)j+49152);
							bsz = (spr->clipdist<<2)+walldist; if (gy < 0) bsz = -bsz;
							addclipline(x1+bsz,y1-bsz,x1-bsz,y1-bsz,(short)j+49152);
						}
					}
					break;
				case 16:
					k = ((gettilesizy(spr->picnum)*spr->yrepeat)<<2);
					if (cstat&128) daz = spr->z+(k>>1); else daz = spr->z;
					if (getpicanm(spr->picnum)&0x00ff0000) daz -= ((long)((signed char)((getpicanm(spr->picnum)>>16)&255))*spr->yrepeat<<2);
					daz2 = daz-k;
					daz += ceildist; daz2 -= flordist;
					if (((*z) < daz) && ((*z) > daz2))
					{
							//These lines get the 2 points of the rotated sprite
							//Given: (x1, y1) starts out as the center point
						tilenum = spr->picnum;
						xoff = (long)((signed char)((getpicanm(tilenum)>>8)&255))+((long)spr->xoffset);
						if ((cstat&4) > 0) xoff = -xoff;
						k = spr->ang; l = spr->xrepeat;
						dax = sintable[k&2047]*l; day = sintable[(k+1536)&2047]*l;
						l = gettilesizx(tilenum); k = (l>>1)+xoff;
						x1 -= mulscale16(dax,k); x2 = x1+mulscale16(dax,l);
						y1 -= mulscale16(day,k); y2 = y1+mulscale16(day,l);
						if (clipinsideboxline(cx,cy,x1,y1,x2,y2,rad) != 0)
						{
							dax = mulscale14(sintable[(spr->ang+256+512)&2047],walldist);
							day = mulscale14(sintable[(spr->ang+256)&2047],walldist);

							if ((x1-(*x))*(y2-(*y)) >= (x2-(*x))*(y1-(*y)))   //Front
							{
								addclipline(x1+dax,y1+day,x2+day,y2-dax,(short)j+49152);
							}
							else
							{
								if ((cstat&64) != 0) continue;
								addclipline(x2-dax,y2-day,x1-day,y1+dax,(short)j+49152);
							}

								//Side blocker
							if ((x2-x1)*((*x)-x1) + (y2-y1)*((*y)-y1) < 0)
								{ addclipline(x1-day,y1+dax,x1+dax,y1+day,(short)j+49152); }
							else if ((x1-x2)*((*x)-x2) + (y1-y2)*((*y)-y2) < 0)
								{ addclipline(x2+day,y2-dax,x2-dax,y2-day,(short)j+49152); }
						}
					}
					break;
				case 32:
					daz = spr->z+ceildist;
					daz2 = spr->z-flordist;
					if (((*z) < daz) && ((*z) > daz2))
					{
						if ((cstat&64) != 0)
							if (((*z) > spr->z) == ((cstat&8)==0)) continue;

						tilenum = spr->picnum;
						xoff = (long)((signed char)((getpicanm(tilenum)>>8)&255))+((long)spr->xoffset);
						yoff = (long)((signed char)((getpicanm(tilenum)>>16)&255))+((long)spr->yoffset);
						if ((cstat&4) > 0) xoff = -xoff;
						if ((cstat&8) > 0) yoff = -yoff;

						k = spr->ang;
						cosang = sintable[(k+512)&2047]; sinang = sintable[k];
						xspan = gettilesizx(tilenum); xrepeat = spr->xrepeat;
						yspan = gettilesizy(tilenum); yrepeat = spr->yrepeat;

						dax = ((xspan>>1)+xoff)*xrepeat; day = ((yspan>>1)+yoff)*yrepeat;
						rxi[0] = x1 + dmulscale16(sinang,dax,cosang,day);
						ryi[0] = y1 + dmulscale16(sinang,day,-cosang,dax);
						l = xspan*xrepeat;
						rxi[1] = rxi[0] - mulscale16(sinang,l);
						ryi[1] = ryi[0] + mulscale16(cosang,l);
						l = yspan*yrepeat;
						k = -mulscale16(cosang,l); rxi[2] = rxi[1]+k; rxi[3] = rxi[0]+k;
						k = -mulscale16(sinang,l); ryi[2] = ryi[1]+k; ryi[3] = ryi[0]+k;

						dax = mulscale14(sintable[(spr->ang-256+512)&2047],walldist);
						day = mulscale14(sintable[(spr->ang-256)&2047],walldist);

						if ((rxi[0]-(*x))*(ryi[1]-(*y)) < (rxi[1]-(*x))*(ryi[0]-(*y)))
						{
							if (clipinsideboxline(cx,cy,rxi[1],ryi[1],rxi[0],ryi[0],rad) != 0)
								addclipline(rxi[1]-day,ryi[1]+dax,rxi[0]+dax,ryi[0]+day,(short)j+49152);
						}
						else if ((rxi[2]-(*x))*(ryi[3]-(*y)) < (rxi[3]-(*x))*(ryi[2]-(*y)))
						{
							if (clipinsideboxline(cx,cy,rxi[3],ryi[3],rxi[2],ryi[2],rad) != 0)
								addclipline(rxi[3]+day,ryi[3]-dax,rxi[2]-dax,ryi[2]-day,(short)j+49152);
						}

						if ((rxi[1]-(*x))*(ryi[2]-(*y)) < (rxi[2]-(*x))*(ryi[1]-(*y)))
						{
							if (clipinsideboxline(cx,cy,rxi[2],ryi[2],rxi[1],ryi[1],rad) != 0)
								addclipline(rxi[2]-dax,ryi[2]-day,rxi[1]-day,ryi[1]+dax,(short)j+49152);
						}
						else if ((rxi[3]-(*x))*(ryi[0]-(*y)) < (rxi[0]-(*x))*(ryi[3]-(*y)))
						{
							if (clipinsideboxline(cx,cy,rxi[0],ryi[0],rxi[3],ryi[3],rad) != 0)
								addclipline(rxi[0]+dax,ryi[0]+day,rxi[3]+day,ryi[3]-dax,(short)j+49152);
						}
					}
					break;
			}
		}
	} while (clipsectcnt < clipsectnum);


	hitwall = 0;
	cnt = clipmoveboxtracenum;
	do
	{
		intx = goalx; inty = goaly;
		if ((hitwall = raytrace(*x, *y, &intx, &inty)) >= 0)
		{
			lx = clipit[hitwall].x2-clipit[hitwall].x1;
			ly = clipit[hitwall].y2-clipit[hitwall].y1;
			templong2 = lx*lx + ly*ly;
			if (templong2 > 0)
			{
				templong1 = (goalx-intx)*lx + (goaly-inty)*ly;

				if ((klabs2(templong1)>>11) < templong2)
					i = divscale20(templong1,templong2);
				else
					i = 0;
				goalx = mulscale20(lx,i)+intx;
				goaly = mulscale20(ly,i)+inty;
			}

			templong1 = dmulscale6(lx,oxvect,ly,oyvect);
			for(i=cnt+1;i<=clipmoveboxtracenum;i++)
			{
				j = hitwalls[i];
				templong2 = dmulscale6(clipit[j].x2-clipit[j].x1,oxvect,clipit[j].y2-clipit[j].y1,oyvect);
				if ((templong1^templong2) < 0)
				{
					updatesector(*x,*y,sectnum);
					return(retval);
				}
			}

			keepaway(&goalx, &goaly, hitwall);
			xvect = ((goalx-intx)<<14);
			yvect = ((goaly-inty)<<14);

			if (cnt == clipmoveboxtracenum) retval = clipobjectval[hitwall];
			hitwalls[cnt] = hitwall;
		}
		cnt--;

		*x = intx;
		*y = inty;
	} while (((xvect|yvect) != 0) && (hitwall >= 0) && (cnt > 0));

	for(j=0;j<clipsectnum;j++)
		if (inside(*x,*y,clipsectorlist[j]) == 1)
		{
			*sectnum = clipsectorlist[j];
			return(retval);
		}

	*sectnum = -1; templong1 = 0x7fffffff;
	for(j=numsectors-1;j>=0;j--)
		if (inside(*x,*y,j) == 1)
		{
			if (sector[j].ceilingstat&2)
				templong2 = (getceilzofslope((short)j,*x,*y)-(*z));
			else
				templong2 = (sector[j].ceilingz-(*z));

			if (templong2 > 0)
			{
				if (templong2 < templong1)
					{ *sectnum = j; templong1 = templong2; }
			}
			else
			{
				if (sector[j].floorstat&2)
					templong2 = ((*z)-getflorzofslope((short)j,*x,*y));
				else
					templong2 = ((*z)-sector[j].floorz);

				if (templong2 <= 0)
				{
					*sectnum = j;
					return(retval);
				}
				if (templong2 < templong1)
					{ *sectnum = j; templong1 = templong2; }
			}
		}

	return(retval);
}

void keepaway (long *x, long *y, long w)
{
	long dx, dy, ox, oy, x1, y1;
	char first;

	x1 = clipit[w].x1; dx = clipit[w].x2-x1;
	y1 = clipit[w].y1; dy = clipit[w].y2-y1;
	ox = ksgn(-dy); oy = ksgn(dx);
	first = (klabs2(dx) <= klabs2(dy));
	while (1)
	{
		if (dx*(*y-y1) > (*x-x1)*dy) return;
		if (first == 0) *x += ox; else *y += oy;
		first ^= 1;
	}
}

int raytrace (long x3, long y3, long *x4, long *y4)
{
	long x1, y1, x2, y2, t, bot, topu, nintx, ninty, cnt, z, hitwall;
	long x21, y21, x43, y43;

	hitwall = -1;
	for(z=clipnum-1;z>=0;z--)
	{
		x1 = clipit[z].x1; x2 = clipit[z].x2; x21 = x2-x1;
		y1 = clipit[z].y1; y2 = clipit[z].y2; y21 = y2-y1;

		topu = x21*(y3-y1) - (x3-x1)*y21; if (topu <= 0) continue;
		if (x21*(*y4-y1) > (*x4-x1)*y21) continue;
		x43 = *x4-x3; y43 = *y4-y3;
		if (x43*(y1-y3) > (x1-x3)*y43) continue;
		if (x43*(y2-y3) <= (x2-x3)*y43) continue;
		bot = x43*y21 - x21*y43; if (bot == 0) continue;

		cnt = 256;
		do
		{
			cnt--; if (cnt < 0) { *x4 = x3; *y4 = y3; return(z); }
			nintx = x3 + scale(x43,topu,bot);
			ninty = y3 + scale(y43,topu,bot);
			topu--;
		} while (x21*(ninty-y1) <= (nintx-x1)*y21);

		if (klabs2(x3-nintx)+klabs2(y3-ninty) < klabs2(x3-*x4)+klabs2(y3-*y4))
			{ *x4 = nintx; *y4 = ninty; hitwall = z; }
	}
	return(hitwall);
}

int pushmove (long *x, long *y, long *z, short *sectnum,
			 long walldist, long ceildist, long flordist, unsigned long cliptype)
{
	sectortype *sec, *sec2;
	walltype *wal, *wal2;
	spritetype *spr;
	long i, j, k, t, dx, dy, dax, day, daz, daz2, bad, dir;
	long dasprclipmask, dawalclipmask;
	short startwall, endwall, clipsectcnt;
	char bad2;

	if ((*sectnum) < 0) return(-1);

	dawalclipmask = (cliptype&65535);
	dasprclipmask = (cliptype>>16);

	k = 32;
	dir = 1;
	do
	{
		bad = 0;

		clipsectorlist[0] = *sectnum;
		clipsectcnt = 0; clipsectnum = 1;
		do
		{
			/*Push FACE sprites
			for(i=headspritesect[clipsectorlist[clipsectcnt]];i>=0;i=nextspritesect[i])
			{
				spr = &sprite[i];
				if (((spr->cstat&48) != 0) && ((spr->cstat&48) != 48)) continue;
				if ((spr->cstat&dasprclipmask) == 0) continue;

				dax = (*x)-spr->x; day = (*y)-spr->y;
				t = (spr->clipdist<<2)+walldist;
				if ((klabs(dax) < t) && (klabs(day) < t))
				{
					t = ((tilesizy[spr->picnum]*spr->yrepeat)<<2);
					if (spr->cstat&128) daz = spr->z+(t>>1); else daz = spr->z;
					if (picanm[spr->picnum]&0x00ff0000) daz -= ((long)((signed char)((picanm[spr->picnum]>>16)&255))*spr->yrepeat<<2);
					if (((*z) < daz+ceildist) && ((*z) > daz-t-flordist))
					{
						t = (spr->clipdist<<2)+walldist;

						j = getangle(dax,day);
						dx = (sintable[(j+512)&2047]>>11);
						dy = (sintable[(j)&2047]>>11);
						bad2 = 16;
						do
						{
							*x = (*x) + dx; *y = (*y) + dy;
							bad2--; if (bad2 == 0) break;
						} while ((klabs((*x)-spr->x) < t) && (klabs((*y)-spr->y) < t));
						bad = -1;
						k--; if (k <= 0) return(bad);
						updatesector(*x,*y,sectnum);
					}
				}
			}*/

			sec = &sector[clipsectorlist[clipsectcnt]];
			if (dir > 0)
				startwall = sec->wallptr, endwall = startwall + sec->wallnum;
			else
				endwall = sec->wallptr, startwall = endwall + sec->wallnum;

			for(i=startwall,wal=&wall[startwall];i!=endwall;i+=dir,wal+=dir)
				if (clipinsidebox(*x,*y,i,walldist-4) == 1)
				{
					j = 0;
					if (wal->nextsector < 0) j = 1;
					if (wal->cstat&dawalclipmask) j = 1;
					if (j == 0)
					{
						sec2 = &sector[wal->nextsector];


							//Find closest point on wall (dax, day) to (*x, *y)
						dax = wall[wal->point2].x-wal->x;
						day = wall[wal->point2].y-wal->y;
						daz = dax*((*x)-wal->x) + day*((*y)-wal->y);
						if (daz <= 0)
							t = 0;
						else
						{
							daz2 = dax*dax+day*day;
							if (daz >= daz2) t = (1<<30); else t = divscale30(daz,daz2);
						}
						dax = wal->x + mulscale30(dax,t);
						day = wal->y + mulscale30(day,t);


						daz = getflorzofslope(clipsectorlist[clipsectcnt],dax,day);
						daz2 = getflorzofslope(wal->nextsector,dax,day);
						if ((daz2 < daz-(1<<8)) && ((sec2->floorstat&1) == 0))
							if (*z >= daz2-(flordist-1)) j = 1;

						daz = getceilzofslope(clipsectorlist[clipsectcnt],dax,day);
						daz2 = getceilzofslope(wal->nextsector,dax,day);
						if ((daz2 > daz+(1<<8)) && ((sec2->ceilingstat&1) == 0))
							if (*z <= daz2+(ceildist-1)) j = 1;
					}
					if (j != 0)
					{
						j = getangle(wall[wal->point2].x-wal->x,wall[wal->point2].y-wal->y);
						dx = (sintable[(j+1024)&2047]>>11);
						dy = (sintable[(j+512)&2047]>>11);
						bad2 = 16;
						do
						{
							*x = (*x) + dx; *y = (*y) + dy;
							bad2--; if (bad2 == 0) break;
						} while (clipinsidebox(*x,*y,i,walldist-4) != 0);
						bad = -1;
						k--; if (k <= 0) return(bad);
						updatesector(*x,*y,sectnum);
					}
					else
					{
						for(j=clipsectnum-1;j>=0;j--)
							if (wal->nextsector == clipsectorlist[j]) break;
						if (j < 0) clipsectorlist[clipsectnum++] = wal->nextsector;
					}
				}

			clipsectcnt++;
		} while (clipsectcnt < clipsectnum);
		dir = -dir;
	} while (bad != 0);

	return(bad);
}

void updatesector(long x, long y, short *sectnum)
{
	walltype *wal;
	long i, j;

	if (inside(x,y,*sectnum) == 1) return;

	if ((*sectnum >= 0) && (*sectnum < numsectors))
	{
		wal = &wall[sector[*sectnum].wallptr];
		j = sector[*sectnum].wallnum;
		do
		{
			i = wal->nextsector;
			if (i >= 0)
				if (inside(x,y,(short)i) == 1)
				{
					*sectnum = i;
					return;
				}
			wal++;
			j--;
		} while (j != 0);
	}

	for(i=numsectors-1;i>=0;i--)
		if (inside(x,y,(short)i) == 1)
		{
			*sectnum = i;
			return;
		}

	*sectnum = -1;
}

void rotatepoint(long xpivot, long ypivot, long x, long y, short daang, long *x2, long *y2)
{
	long dacos, dasin;

	dacos = sintable[(daang+2560)&2047];
	dasin = sintable[(daang+2048)&2047];
	x -= xpivot;
	y -= ypivot;
	*x2 = dmulscale14(x,dacos,-y,dasin) + xpivot;
	*y2 = dmulscale14(y,dacos,x,dasin) + ypivot;
}

extern signed char game_inactive;

int krand2()
{
	if (game_inactive)
		return 63543;
	randomseed = (randomseed*27584621)+1;
	return(((unsigned long)randomseed)>>16);
}

int krand()
{
	randomseed = (randomseed*27584621)+1;
	return(((unsigned long)randomseed)>>16);
}

int getzrange(long x, long y, long z, short sectnum,
			 long *ceilz, long *ceilhit, long *florz, long *florhit,
			 long walldist, unsigned long cliptype)
{
	sectortype *sec;
	walltype *wal, *wal2;
	spritetype *spr;
	long clipsectcnt, startwall, endwall, tilenum, xoff, yoff, dax, day;
	long xmin, ymin, xmax, ymax, i, j, k, l, daz, daz2, dx, dy;
	long x1, y1, x2, y2, x3, y3, x4, y4, ang, cosang, sinang;
	long xspan, yspan, xrepeat, yrepeat, dasprclipmask, dawalclipmask;
	short cstat;
	char bad, clipyou;

	if (sectnum < 0)
	{
		*ceilz = 0x80000000; *ceilhit = -1;
		*florz = 0x7fffffff; *florhit = -1;
		return;
	}

		//Extra walldist for sprites on sector lines
	i = walldist+MAXCLIPDIST+1;
	xmin = x-i; ymin = y-i;
	xmax = x+i; ymax = y+i;

	getzsofslope(sectnum,x,y,ceilz,florz);
	*ceilhit = sectnum+16384; *florhit = sectnum+16384;

	dawalclipmask = (cliptype&65535);
	dasprclipmask = (cliptype>>16);

	clipsectorlist[0] = sectnum;
	clipsectcnt = 0; clipsectnum = 1;

	do  //Collect sectors inside your square first
	{
		sec = &sector[clipsectorlist[clipsectcnt]];
		startwall = sec->wallptr; endwall = startwall + sec->wallnum;
		for(j=startwall,wal=&wall[startwall];j<endwall;j++,wal++)
		{
			k = wal->nextsector;
			if (k >= 0)
			{
				wal2 = &wall[wal->point2];
				x1 = wal->x; x2 = wal2->x;
				if ((x1 < xmin) && (x2 < xmin)) continue;
				if ((x1 > xmax) && (x2 > xmax)) continue;
				y1 = wal->y; y2 = wal2->y;
				if ((y1 < ymin) && (y2 < ymin)) continue;
				if ((y1 > ymax) && (y2 > ymax)) continue;

				dx = x2-x1; dy = y2-y1;
				if (dx*(y-y1) < (x-x1)*dy) continue; //back
				if (dx > 0) dax = dx*(ymin-y1); else dax = dx*(ymax-y1);
				if (dy > 0) day = dy*(xmax-x1); else day = dy*(xmin-x1);
				if (dax >= day) continue;

				if (wal->cstat&dawalclipmask) continue;
				sec = &sector[k];
				if (editstatus == 0)
				{
					if (((sec->ceilingstat&1) == 0) && (z <= sec->ceilingz+(3<<8))) continue;
					if (((sec->floorstat&1) == 0) && (z >= sec->floorz-(3<<8))) continue;
				}

				for(i=clipsectnum-1;i>=0;i--) if (clipsectorlist[i] == k) break;
				if (i < 0) clipsectorlist[clipsectnum++] = k;

				if ((x1 < xmin+MAXCLIPDIST) && (x2 < xmin+MAXCLIPDIST)) continue;
				if ((x1 > xmax-MAXCLIPDIST) && (x2 > xmax-MAXCLIPDIST)) continue;
				if ((y1 < ymin+MAXCLIPDIST) && (y2 < ymin+MAXCLIPDIST)) continue;
				if ((y1 > ymax-MAXCLIPDIST) && (y2 > ymax-MAXCLIPDIST)) continue;
				if (dx > 0) dax += dx*MAXCLIPDIST; else dax -= dx*MAXCLIPDIST;
				if (dy > 0) day -= dy*MAXCLIPDIST; else day += dy*MAXCLIPDIST;
				if (dax >= day) continue;

					//It actually got here, through all the continue's!!!
				getzsofslope((short)k,x,y,&daz,&daz2);
				if (daz > *ceilz) { *ceilz = daz; *ceilhit = k+16384; }
				if (daz2 < *florz) { *florz = daz2; *florhit = k+16384; }
			}
		}
		clipsectcnt++;
	} while (clipsectcnt < clipsectnum);

	for(i=0;i<clipsectnum;i++)
	{
		for(j=headspritesect[clipsectorlist[i]];j>=0;j=nextspritesect[j])
		{
			spr = &sprite[j];
			cstat = spr->cstat;
			if (cstat&dasprclipmask)
			{
				x1 = spr->x; y1 = spr->y;

				clipyou = 0;
				switch(cstat&48)
				{
					case 0:
						k = walldist+(spr->clipdist<<2)+1;
						if ((klabs2(x1-x) <= k) && (klabs2(y1-y) <= k))
						{
							daz = spr->z;
							k = ((gettilesizy(spr->picnum)*spr->yrepeat)<<1);
							if (cstat&128) daz += k;
							if (getpicanm(spr->picnum)&0x00ff0000) daz -= ((long)((signed char)((getpicanm(spr->picnum)>>16)&255))*spr->yrepeat<<2);
							daz2 = daz - (k<<1);
							clipyou = 1;
						}
						break;
					case 16:
						tilenum = spr->picnum;
						xoff = (long)((signed char)((getpicanm(tilenum)>>8)&255))+((long)spr->xoffset);
						if ((cstat&4) > 0) xoff = -xoff;
						k = spr->ang; l = spr->xrepeat;
						dax = sintable[k&2047]*l; day = sintable[(k+1536)&2047]*l;
						l = gettilesizx(tilenum); k = (l>>1)+xoff;
						x1 -= mulscale16(dax,k); x2 = x1+mulscale16(dax,l);
						y1 -= mulscale16(day,k); y2 = y1+mulscale16(day,l);
						if (clipinsideboxline(x,y,x1,y1,x2,y2,walldist+1) != 0)
						{
							daz = spr->z; k = ((gettilesizy(spr->picnum)*spr->yrepeat)<<1);
							if (cstat&128) daz += k;
							if (getpicanm(spr->picnum)&0x00ff0000) daz -= ((long)((signed char)((getpicanm(spr->picnum)>>16)&255))*spr->yrepeat<<2);
							daz2 = daz-(k<<1);
							clipyou = 1;
						}
						break;
					case 32:
						daz = spr->z; daz2 = daz;

						if ((cstat&64) != 0)
							if ((z > daz) == ((cstat&8)==0)) continue;

						tilenum = spr->picnum;
						xoff = (long)((signed char)((getpicanm(tilenum)>>8)&255))+((long)spr->xoffset);
						yoff = (long)((signed char)((getpicanm(tilenum)>>16)&255))+((long)spr->yoffset);
						if ((cstat&4) > 0) xoff = -xoff;
						if ((cstat&8) > 0) yoff = -yoff;

						ang = spr->ang;
						cosang = sintable[(ang+512)&2047]; sinang = sintable[ang];
						xspan = gettilesizx(tilenum); xrepeat = spr->xrepeat;
						yspan = gettilesizy(tilenum); yrepeat = spr->yrepeat;

						dax = ((xspan>>1)+xoff)*xrepeat; day = ((yspan>>1)+yoff)*yrepeat;
						x1 += dmulscale16(sinang,dax,cosang,day)-x;
						y1 += dmulscale16(sinang,day,-cosang,dax)-y;
						l = xspan*xrepeat;
						x2 = x1 - mulscale16(sinang,l);
						y2 = y1 + mulscale16(cosang,l);
						l = yspan*yrepeat;
						k = -mulscale16(cosang,l); x3 = x2+k; x4 = x1+k;
						k = -mulscale16(sinang,l); y3 = y2+k; y4 = y1+k;

						dax = mulscale14(sintable[(spr->ang-256+512)&2047],walldist+4);
						day = mulscale14(sintable[(spr->ang-256)&2047],walldist+4);
						x1 += dax; x2 -= day; x3 -= dax; x4 += day;
						y1 += day; y2 += dax; y3 -= day; y4 -= dax;

						if ((y1^y2) < 0)
						{
							if ((x1^x2) < 0) clipyou ^= (x1*y2<x2*y1)^(y1<y2);
							else if (x1 >= 0) clipyou ^= 1;
						}
						if ((y2^y3) < 0)
						{
							if ((x2^x3) < 0) clipyou ^= (x2*y3<x3*y2)^(y2<y3);
							else if (x2 >= 0) clipyou ^= 1;
						}
						if ((y3^y4) < 0)
						{
							if ((x3^x4) < 0) clipyou ^= (x3*y4<x4*y3)^(y3<y4);
							else if (x3 >= 0) clipyou ^= 1;
						}
						if ((y4^y1) < 0)
						{
							if ((x4^x1) < 0) clipyou ^= (x4*y1<x1*y4)^(y4<y1);
							else if (x4 >= 0) clipyou ^= 1;
						}
						break;
				}

				if (clipyou != 0)
				{
					if ((z > daz) && (daz > *ceilz)) { *ceilz = daz; *ceilhit = j+49152; }
					if ((z < daz2) && (daz2 < *florz)) { *florz = daz2; *florhit = j+49152; }
				}
			}
		}
	}
}

int getceilzofslope(short sectnum, long dax, long day)
{
	long dx, dy, i, j;
	walltype *wal;

	if (!(sector[sectnum].ceilingstat&2)) return(sector[sectnum].ceilingz);
	wal = &wall[sector[sectnum].wallptr];
	dx = wall[wal->point2].x-wal->x; dy = wall[wal->point2].y-wal->y;
	i = (nsqrtasm(dx*dx+dy*dy)<<5); if (i == 0) return(sector[sectnum].ceilingz);
	j = dmulscale3(dx,day-wal->y,-dy,dax-wal->x);
	return(sector[sectnum].ceilingz+scale(sector[sectnum].ceilingheinum,j,i));
}

int getflorzofslope(short sectnum, long dax, long day)
{
	long dx, dy, i, j;
	walltype *wal;

	if (!(sector[sectnum].floorstat&2)) return(sector[sectnum].floorz);
	wal = &wall[sector[sectnum].wallptr];
	dx = wall[wal->point2].x-wal->x; dy = wall[wal->point2].y-wal->y;
	i = (nsqrtasm(dx*dx+dy*dy)<<5); if (i == 0) return(sector[sectnum].floorz);
	j = dmulscale3(dx,day-wal->y,-dy,dax-wal->x);
	return(sector[sectnum].floorz+scale(sector[sectnum].floorheinum,j,i));
}

void getzsofslope(short sectnum, long dax, long day, long *ceilz, long *florz)
{
	long dx, dy, i, j;
	walltype *wal, *wal2;
	sectortype *sec;

	sec = &sector[sectnum];
	*ceilz = sec->ceilingz; *florz = sec->floorz;
	if ((sec->ceilingstat|sec->floorstat)&2)
	{
		wal = &wall[sec->wallptr]; wal2 = &wall[wal->point2];
		dx = wal2->x-wal->x; dy = wal2->y-wal->y;
		i = (nsqrtasm(dx*dx+dy*dy)<<5); if (i == 0) return;
		j = dmulscale3(dx,day-wal->y,-dy,dax-wal->x);
		if (sec->ceilingstat&2) *ceilz = (*ceilz)+scale(sec->ceilingheinum,j,i);
		if (sec->floorstat&2) *florz = (*florz)+scale(sec->floorheinum,j,i);
	}
}

void alignflorslope(short dasect, long x, long y, long z)
{
	long i, dax, day;
	walltype *wal;

	wal = &wall[sector[dasect].wallptr];
	dax = wall[wal->point2].x-wal->x;
	day = wall[wal->point2].y-wal->y;

	i = (y-wal->y)*dax - (x-wal->x)*day; if (i == 0) return;
	sector[dasect].floorheinum = scale((z-sector[dasect].floorz)<<8,
												  nsqrtasm(dax*dax+day*day),i);

	if (sector[dasect].floorheinum == 0) sector[dasect].floorstat &= ~2;
											  else sector[dasect].floorstat |= 2;
}

#pragma GLOBAL_ASM("asm/nonmatchings/engine/FindDistance2D.s")

#pragma GLOBAL_ASM("asm/nonmatchings/engine/FindDistance3D.s")

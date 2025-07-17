// "Build Engine & Tools" Copyright (c) 1993-1997 Ken Silverman
// Ken Silverman's official web site: "http://www.advsys.net/ken"
// See the included license file "BUILDLIC.TXT" for license info.
#include "common.h"
#include "pragmas.h"

void fastmemcpy(void*, void*, unsigned long);

//   This module keeps track of a standard linear cacheing system.
//   To use this module, here's all you need to do:
//
//   Step 1: Allocate a nice BIG buffer, like from 1MB-4MB and
//           Call initcache(long cachestart, long cachesize) where
//
//              cachestart = (long)(pointer to start of BIG buffer)
//              cachesize = length of BIG buffer
//
//   Step 2: Call allocache(long *bufptr, long bufsiz, char *lockptr)
//              whenever you need to allocate a buffer, where:
//
//              *bufptr = pointer to 4-byte pointer to buffer
//                 Confused?  Using this method, cache2d can remove
//                 previously allocated things from the cache safely by
//                 setting the 4-byte pointer to 0.
//              bufsiz = number of bytes to allocate
//              *lockptr = pointer to locking char which tells whether
//                 the region can be removed or not.  If *lockptr = 0 then
//                 the region is not locked else its locked.
//
//   Step 3: If you need to remove everything from the cache, or every
//           unlocked item from the cache, you can call uninitcache();
//              Call uninitcache(0) to remove all unlocked items, or
//              Call uninitcache(1) to remove everything.
//           After calling uninitcache, it is still ok to call allocache
//           without first calling initcache.

#define MAXCACHEOBJECTS 1024

static long cachesize = 0;
long cachecount = 0;
char zerochar = 0;
long cachestart = 0, cacnum = 0;
typedef struct { long* hand, leng; char* lock; } cactype;
cactype cac[MAXCACHEOBJECTS];
static long lockrecip[200];

void reportandexit(void);

void initcache(long dacachestart, long dacachesize)
{
	long i;

	for (i = 0; i < MAXCACHEOBJECTS; i++) cac[i].leng = 0;
	
	for(i=1;i<200;i++) lockrecip[i] = (1<<28)/(200-i);

	cachestart = dacachestart;
	cachesize = dacachesize;

	cac[0].leng = cachesize;
	cac[0].lock = &zerochar;
	cacnum = 1;
}

int allocache (long *newhandle, long newbytes, char *newlockptr)
{
	long i, j, z, zz, bestz, daval, bestval, besto, o1, o2, sucklen, suckz;

	newbytes = ((newbytes+15)&0xfffffff0);

	if ((unsigned)newbytes > (unsigned)cachesize)
	{
		//printf("Cachesize: %ld\n",cachesize);
		//printf("*Newhandle: 0x%x, Newbytes: %ld, *Newlock: %d\n",newhandle,newbytes,*newlockptr);
		"Data too big to fit in cache\n";
		reportandexit();
	}

	if (*newlockptr == 0)
	{
		"Lock pointer = 0\n";
		reportandexit();
	}

	if (*newlockptr > 200)
	{
		"Lock pointer > 200\n";
		reportandexit();
	}

		//Find best place
	bestval = 0x7fffffff; o1 = cachesize;
	for(z=cacnum-1;z>=0;z--)
	{
		o1 -= cac[z].leng;
		o2 = o1+newbytes; if (o2 > cachesize) continue;

		daval = 0;
		for(i=o1,zz=z;i<o2;i+=cac[zz++].leng)
		{
			if (*cac[zz].lock == 0) continue;
			if (*cac[zz].lock >= 100) { daval = 0x7fffffff; break; }
			daval += mulscale32(cac[zz].leng+65536,lockrecip[*cac[zz].lock]);
			if (daval >= bestval) break;
		}
		if (daval < bestval)
		{
			bestval = daval; besto = o1; bestz = z;
			if (bestval == 0) break;
		}
	}

	//printf("%ld %ld %ld\n",besto,newbytes,*newlockptr);

	if (bestval == 0x7fffffff)
	{
		"CACHE SPACE ALL LOCKED UP!\n";
		return 0;
	}

		//Suck things out
	for(sucklen=-newbytes,suckz=bestz;sucklen<0;sucklen+=cac[suckz++].leng)
		if (*cac[suckz].lock) *cac[suckz].hand = 0;

		//Remove all blocks except 1
	suckz -= (bestz+1); cacnum -= suckz;
	fastmemcpy(&cac[bestz],&cac[bestz + suckz],(cacnum-bestz)*sizeof(cactype));
	cac[bestz].hand = newhandle; *newhandle = cachestart+besto;
	cac[bestz].leng = newbytes;
	cac[bestz].lock = newlockptr;
	cachecount++;

		//Add new empty block if necessary
	if (sucklen <= 0) return 1;

	bestz++;
	if (bestz == cacnum)
	{
		cacnum++;// if (cacnum > MAXCACHEOBJECTS) reportandexit("Too many objects in cache! (cacnum > MAXCACHEOBJECTS)");
		cac[bestz].leng = sucklen;
		cac[bestz].lock = &zerochar;
		return 1;
	}

	if (*cac[bestz].lock == 0) { cac[bestz].leng += sucklen; return 1; }

	cacnum++;// if (cacnum > MAXCACHEOBJECTS) reportandexit("Too many objects in cache! (cacnum > MAXCACHEOBJECTS)");
	for(z=cacnum-1;z>bestz;z--) cac[z] = cac[z-1];
	cac[bestz].leng = sucklen;
	cac[bestz].lock = &zerochar;
	return 1;
}

void suckcache (long *suckptr)
{
	long i;

		//Can't exit early, because invalid pointer might be same even though lock = 0
	for(i=0;i<cacnum;i++)
		if ((long)(cac[i].hand) == (long)suckptr)
		{
			if (*cac[i].lock) *cac[i].hand = 0;
			cac[i].lock = &zerochar;
			cac[i].hand = 0;

				//Combine empty blocks
			if ((i > 0) && (*cac[i-1].lock == 0))
			{
				cac[i-1].leng += cac[i].leng;
				cacnum--; fastmemcpy(&cac[i],&cac[i+1],(cacnum-i)*sizeof(cactype));
			}
			else if ((i < cacnum-1) && (*cac[i+1].lock == 0))
			{
				cac[i+1].leng += cac[i].leng;
				cacnum--; fastmemcpy(&cac[i],&cac[i+1],(cacnum-i)*sizeof(cactype));
			}
		}
}

void agecache(void)
{
	long cnt;
	char ch;

	for(cnt=0;cnt<cacnum;cnt++)
	{
		ch = (*cac[cnt].lock);
		if (((ch-2)&255) < 198)
			(*cac[cnt].lock) = ch-1;
	}
}

void allocache2(long* newhandle, long newbytes, char* newlockptr)
{
	while (allocache(newhandle, newbytes, newlockptr) == 0)
	{
		agecache();
	}
}

void reportandexit(void)
{
	while (1)
	{
	}
}

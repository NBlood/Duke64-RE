//-------------------------------------------------------------------------
/*
Copyright (C) 1996, 2003 - 3D Realms Entertainment

This file is NOT part of Duke Nukem 3D version 1.5 - Atomic Edition
However, it is either an older version of a file that is, or is
some test code written during the development of Duke Nukem 3D.
This file is provided purely for educational interest.

Duke Nukem 3D is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

Prepared for public release: 03/21/2003 - Charlie Wiederhold, 3D Realms
*/
//-------------------------------------------------------------------------

#include "common.h"
#include "duke3d.h"

extern short otherp;

static short g_i, g_p;
static long g_x;
static long* g_t;
static spritetype* g_sp;

long *insptr;

int iscorpse(spritetype *s)
{
	if (s->picnum == LIZTROOPDSPRITE || s->picnum == TILE1957 || s->picnum == PIGCOPDEADSPRITE
		|| s->picnum == LIZMANDEADSPRITE || s->picnum == TILE2685 || s->picnum == TILE2809 || s->picnum == TILE4689)
		return 1;
	return 0;
}

short getincangle(short a,short na)
{
    a &= 2047;
    na &= 2047;

    if(klabs2(a-na) < 1024)
        return (na-a);
    else
    {
        if(na > 1024) na -= 2048;
        if(a > 1024) a -= 2048;

        na -= 2048;
        a -= 2048;
        return (na-a);
    }
}

void getglobalz(short i)
{
    long hz,lz,zr;

    spritetype *s = &sprite[i];

    if( s->statnum == 10 || s->statnum == 6 || s->statnum == 2 || s->statnum == 1 || s->statnum == 4)
    {
        if(s->statnum == 4)
            zr = 4L;
        else zr = 127L;

        getzrange(s->x,s->y,s->z-(FOURSLEIGHT),s->sectnum,&hittype[i].ceilingz,&hz,&hittype[i].floorz,&lz,zr,CLIPMASK0);

        if ((lz & 0xfff) >= MAXSPRITES)
            gameexit("Sprite overflow caused by ONION\n");
        if( (lz&49152) == 49152 && (sprite[lz&0xfff].cstat&48) == 0 )
        {
            lz &= 0xfff;
            if( badguy(&sprite[lz]) && sprite[lz].pal != 1)
            {
                if( s->statnum != 4 )
                {
                    hittype[i].dispicnum = -4; // No shadows on actors
                    s->xvel = -256;
                    ssp(i,CLIPMASK0);
                }
            }
            else if(sprite[lz].picnum == APLAYER && badguy(s) )
            {
                hittype[i].dispicnum = -4; // No shadows on actors
                s->xvel = -256;
                ssp(i,CLIPMASK0);
            }
            else if(s->statnum == 4 && sprite[lz].picnum == APLAYER)
                if(s->owner == lz)
            {
                hittype[i].ceilingz = sector[s->sectnum].ceilingz;
                hittype[i].floorz   = sector[s->sectnum].floorz;
            }
        }
    }
    else
    {
        hittype[i].ceilingz = sector[s->sectnum].ceilingz;
        hittype[i].floorz   = sector[s->sectnum].floorz;
    }
}


void makeitfall(short i)
{
    spritetype *s = &sprite[i];
    long hz,lz,c;

    if( floorspace(s->sectnum) )
        c = 0;
    else
    {
        if( ceilingspace(s->sectnum) || sector[s->sectnum].lotag == 2)
            c = gc/6;
        else c = gc;
    }

    if( ( s->statnum == 1 || s->statnum == 10 || s->statnum == 2 || s->statnum == 6 ) )
        getzrange(s->x,s->y,s->z-(FOURSLEIGHT),s->sectnum,&hittype[i].ceilingz,&hz,&hittype[i].floorz,&lz,127L,CLIPMASK0);
    else
    {
        hittype[i].ceilingz = sector[s->sectnum].ceilingz;
        hittype[i].floorz   = sector[s->sectnum].floorz;
    }

    if( s->z < hittype[i].floorz-(FOURSLEIGHT) )
    {
        s->zvel += c;
        if( sector[s->sectnum].lotag == 2 && s->zvel > 3072 )
            s->zvel = 3072;
        if(s->zvel > 6144)
            s->zvel = 6144;
        s->z += s->zvel;
    }
    if( s->z >= hittype[i].floorz-(FOURSLEIGHT) )
    {
        s->z = hittype[i].floorz - FOURSLEIGHT;
        s->zvel = 0;
    }
}

char dodge(spritetype *s)
{
    short i;
    long bx,by,mx,my,bxvect,byvect,mxvect,myvect,d;

    mx = s->x;
    my = s->y;
    mxvect = sintable[(s->ang+512)&2047]; myvect = sintable[s->ang&2047];

    for(i=headspritestat[4];i>=0;i=nextspritestat[i]) //weapons list
    {
        if( OW == i || SECT != s->sectnum)
            continue;

        bx = SX-mx;
        by = SY-my;
        bxvect = sintable[(SA+512)&2047]; byvect = sintable[SA&2047];

        if (mxvect*bx + myvect*by >= 0)
            if (bxvect*bx + byvect*by < 0)
        {
            d = bxvect*by - byvect*bx;
            if (klabs2(d) < 65536*64)
            {
                s->ang -= 512+(TRAND&1024);
                return 1;
            }
        }
    }
    return 0;
}

short furthestangle(short i,short angs)
{
    short j, hitsect,hitwall,hitspr,furthest_angle, angincs;
    long hx, hy, hz, d, greatestd;
    spritetype *s = &sprite[i];

    greatestd = -(1<<30);
    angincs = 2048/angs;

    if(s->picnum != APLAYER)
        if( (g_t[0]&63) > 2 ) return( s->ang + 1024 );

    for(j=s->ang;j<(2048+s->ang);j+=angincs)
    {
        hitscan(s->x, s->y, s->z-(8<<8), s->sectnum,
            sintable[(j+512)&2047],
            sintable[j&2047],0,
            &hitsect,&hitwall,&hitspr,&hx,&hy,&hz,CLIPMASK1);

        d = klabs2(hx-s->x) + klabs2(hy-s->y);

        if(d > greatestd)
        {
            greatestd = d;
            furthest_angle = j;
        }
    }
    return (furthest_angle&2047);
}

short furthestcanseepoint(short i,spritetype *ts,long *dax,long *day)
{
    short j, hitsect,hitwall,hitspr, angincs, tempang;
    long hx, hy, hz, d, da;//, d, cd, ca,tempx,tempy,cx,cy;
    spritetype *s = &sprite[i];

    if( (g_t[0]&63) ) return -1;

    if(ud.multimode < 2 && ud.player_skill < 3)
        angincs = 2048/2;
    else angincs = 2048/(1+(TRAND&1));

    for(j=ts->ang;j<(2048+ts->ang);j+=(angincs-(TRAND&511)))
    {
        hitscan(ts->x, ts->y, ts->z-(16<<8), ts->sectnum,
            sintable[(j+512)&2047],
            sintable[j&2047],16384-(TRAND&32767),
            &hitsect,&hitwall,&hitspr,&hx,&hy,&hz,CLIPMASK1);

        d = klabs2(hx-ts->x)+klabs2(hy-ts->y);
        da = klabs2(hx-s->x)+klabs2(hy-s->y);

        if( d < da )
            if(cansee(hx,hy,hz,hitsect,s->x,s->y,s->z-(16<<8),s->sectnum))
        {
            *dax = hx;
            *day = hy;
            return hitsect;
        }
    }
    return -1;
}




void alterang(short a)
{
    short aang, angdif, goalang,j;
    long ticselapsed, *moveptr;

    moveptr = (long *)g_t[1];

    ticselapsed = (g_t[0])&31;

    aang = g_sp->ang;

    g_sp->xvel += (*moveptr-g_sp->xvel)/5;
    if(g_sp->zvel < 648) g_sp->zvel += ((*(moveptr+1)<<4)-g_sp->zvel)/5;

    if(a&seekplayer)
    {
        j = ps[g_p].holoduke_on;

        if(j >= 0 && cansee(sprite[j].x,sprite[j].y,sprite[j].z,sprite[j].sectnum,g_sp->x,g_sp->y,g_sp->z,g_sp->sectnum) )
            g_sp->owner = j;
        else g_sp->owner = ps[g_p].i;

        if(sprite[g_sp->owner].picnum == APLAYER)
            goalang = getangle(hittype[g_i].lastvx-g_sp->x,hittype[g_i].lastvy-g_sp->y);
        else
            goalang = getangle(sprite[g_sp->owner].x-g_sp->x,sprite[g_sp->owner].y-g_sp->y);

        if(g_sp->xvel && g_sp->picnum != DRONE)
        {
            angdif = getincangle(aang,goalang);

            if(ticselapsed < 2)
            {
                if( klabs2(angdif) < 256)
                {
                    j = 128-(TRAND&256);
                    g_sp->ang += j;
                    if( hits(g_i) < 844 )
                        g_sp->ang -= j;
                }
            }
            else if(ticselapsed > 18 && ticselapsed < 26) // choose
            {
                if(klabs2(angdif>>2) < 128) g_sp->ang = goalang;
                else g_sp->ang += angdif>>2;
            }
        }
        else g_sp->ang = goalang;
    }

    if(ticselapsed < 1)
    {
        j = 2;
        if(a&furthestdir)
        {
            goalang = furthestangle(g_i,j);
            g_sp->ang = goalang;
            g_sp->owner = ps[g_p].i;
        }

        if(a&fleeenemy)
        {
            goalang = furthestangle(g_i,j);
            g_sp->ang = goalang; // += angdif; //  = getincangle(aang,goalang)>>1;
        }
    }
}

void move()
{
    long l, *moveptr;
    short a, goalang, angdif;
    long daxvel;

    a = g_sp->hitag;

    if(a == -1) a = 0;

    g_t[0]++;

    if(a&face_player)
    {
        if(ps[g_p].newowner >= 0)
            goalang = getangle(ps[g_p].oposx-g_sp->x,ps[g_p].oposy-g_sp->y);
        else goalang = getangle(ps[g_p].posx-g_sp->x,ps[g_p].posy-g_sp->y);
        angdif = getincangle(g_sp->ang,goalang)>>2;
        if(angdif > -8 && angdif < 0) angdif = 0;
        g_sp->ang += angdif;
    }

    if(a&spin)
        g_sp->ang += sintable[ ((g_t[0]<<3)&2047) ]>>6;

    if(a&face_player_slow)
    {
        if(ps[g_p].newowner >= 0)
            goalang = getangle(ps[g_p].oposx-g_sp->x,ps[g_p].oposy-g_sp->y);
        else goalang = getangle(ps[g_p].posx-g_sp->x,ps[g_p].posy-g_sp->y);
        angdif = ksgn(getincangle(g_sp->ang,goalang))<<5;
        if(angdif > -32 && angdif < 0)
        {
            angdif = 0;
            g_sp->ang = goalang;
        }
        g_sp->ang += angdif;
    }


    if((a&jumptoplayer) == jumptoplayer)
    {
        if(g_t[0] < 16)
            g_sp->zvel -= (sintable[(512+(g_t[0]<<4))&2047]>>5);
    }

    if(a&face_player_smart)
    {
        long newx,newy;

        newx = ps[g_p].posx+(ps[g_p].posxv/768);
        newy = ps[g_p].posy+(ps[g_p].posyv/768);
        goalang = getangle(newx-g_sp->x,newy-g_sp->y);
        angdif = getincangle(g_sp->ang,goalang)>>2;
        if(angdif > -8 && angdif < 0) angdif = 0;
        g_sp->ang += angdif;
    }

    if( g_t[1] == 0 || a == 0 )
    {
        if( ( badguy(g_sp) && g_sp->extra <= 0 ) || (hittype[g_i].bposx != g_sp->x) || (hittype[g_i].bposy != g_sp->y) )
        {
            hittype[g_i].bposx = g_sp->x;
            hittype[g_i].bposy = g_sp->y;
            setsprite(g_i,g_sp->x,g_sp->y,g_sp->z);
        }
        return;
    }

    moveptr = (long *)g_t[1];

    if(a&geth) g_sp->xvel += (*moveptr-g_sp->xvel)>>1;
    if(a&getv) g_sp->zvel += ((*(moveptr+1)<<4)-g_sp->zvel)>>1;

    if(a&dodgebullet)
        dodge(g_sp);

    if(g_sp->picnum != APLAYER)
        alterang(a);

    if(g_sp->xvel > -6 && g_sp->xvel < 6 ) g_sp->xvel = 0;

    a = badguy(g_sp);

    if(g_sp->xvel || g_sp->zvel)
    {
        if(a && g_sp->picnum != ROTATEGUN)
        {
            if( (g_sp->picnum == DRONE || g_sp->picnum == COMMANDER) && g_sp->extra > 0)
            {
                if(g_sp->picnum == COMMANDER)
                {
                    hittype[g_i].ceilingz = l = getceilzofslope(g_sp->sectnum,g_sp->x,g_sp->y);
                    if( (g_sp->z-l) < (80<<8) )
                    {
                        g_sp->z = l+(80<<8);
                        g_sp->zvel = 0;
                    }

                    hittype[g_i].floorz = l = getflorzofslope(g_sp->sectnum,g_sp->x,g_sp->y);
                    if( g_sp->z > (l-(8<<8)) )
                    {
                        if( g_sp->z > (l-(8<<8)) ) g_sp->z = l-(8<<8);
                        g_sp->zvel = 0;
                    }
                }
                else
                {
                    if( g_sp->zvel > 0 )
                    {
                        hittype[g_i].floorz = l = getflorzofslope(g_sp->sectnum,g_sp->x,g_sp->y);
                        if( g_sp->z > (l-(30<<8)) )
                            g_sp->z = l-(30<<8);
                    }
                    else
                    {
                        hittype[g_i].ceilingz = l = getceilzofslope(g_sp->sectnum,g_sp->x,g_sp->y);
                        if( (g_sp->z-l) < (50<<8) )
                        {
                            g_sp->z = l+(50<<8);
                            g_sp->zvel = 0;
                        }
                    }
                }
            }
            else if(g_sp->picnum != ORGANTIC)
            {
                if(g_sp->zvel > 0 && hittype[g_i].floorz < g_sp->z)
                    g_sp->z = hittype[g_i].floorz;
                if( g_sp->zvel < 0)
                {
                    l = getceilzofslope(g_sp->sectnum,g_sp->x,g_sp->y);
                    if( (g_sp->z-l) < (66<<8) )
                    {
                        g_sp->z = l+(66<<8);
                        g_sp->zvel >>= 1;
                    }
                }
            }
        }
        else if(g_sp->picnum == APLAYER)
            if( (g_sp->z-hittype[g_i].ceilingz) < (32<<8) )
                g_sp->z = hittype[g_i].ceilingz+(32<<8);

        daxvel = g_sp->xvel;
        angdif = g_sp->ang;

        if( a && g_sp->picnum != ROTATEGUN )
        {
            if( g_x < 960 && g_sp->xrepeat > 16 )
            {

                daxvel = -(1024-g_x);
                angdif = getangle(ps[g_p].posx-g_sp->x,ps[g_p].posy-g_sp->y);

                if(g_x < 512)
                {
                    ps[g_p].posxv = 0;
                    ps[g_p].posyv = 0;
                }
                else
                {
                    ps[g_p].posxv = mulscale(ps[g_p].posxv,dukefriction-0x2000,16);
                    ps[g_p].posyv = mulscale(ps[g_p].posyv,dukefriction-0x2000,16);
                }
            }
            else if(g_sp->picnum != DRONE && g_sp->picnum != SHARK && g_sp->picnum != COMMANDER)
            {
                if( hittype[g_i].bposz != g_sp->z || ( ud.multimode < 2 && ud.player_skill < 2 ) )
                {
                    if( (g_t[0]&1) || ps[g_p].actorsqu == g_i ) return;
                    else daxvel <<= 1;
                }
                else
                {
                    if( (g_t[0]&3) || ps[g_p].actorsqu == g_i ) return;
                    else daxvel <<= 2;
                }
            }
        }

        hittype[g_i].movflag = movesprite(g_i,
            (daxvel*(sintable[(angdif+512)&2047]))>>14,
            (daxvel*(sintable[angdif&2047]))>>14,g_sp->zvel,CLIPMASK0);
   }

   if( a )
   {
       if (sector[g_sp->sectnum].ceilingstat&1)
           g_sp->shade += (sector[g_sp->sectnum].ceilingshade-g_sp->shade)>>1;
       else g_sp->shade += (sector[g_sp->sectnum].floorshade-g_sp->shade)>>1;

       if( sector[g_sp->sectnum].floorpicnum == MIRROR )
           deletesprite(g_i);
   }
}

char parse(void);

void parseifelse(long condition)
{
    if( condition )
    {
        insptr+=2;
        parse();
    }
    else
    {
        insptr = (long *) *(insptr+1);
        if (insptr < &script[0] || insptr > &script[MAXSCRIPTSIZE-1])
            gameexit("Script overflow 1\n");
        if(*insptr == 10)
        {
            insptr+=2;
            parse();
        }
    }
}

#ifdef NON_MATCHING
char parse(void)
{
    unsigned short picnum;
    short n, n2;
    long j, l, s;

    if(killit_flag) return 1;
    if (insptr < &script[0] || insptr > &script[MAXSCRIPTSIZE-1]) gameexit("Script overflow 2\n");

//    if(*it == 1668249134L) gameexit("\nERR");

    switch(*insptr)
    {
        case 3:
            insptr++;
            parseifelse( rnd(*insptr));
            break;
        case 45:

            if(g_x > 1024)
            {
                short temphit, sclip, angdif;

                if( badguy(g_sp) && g_sp->xrepeat > 56 )
                {
                    sclip = 3084;
                    angdif = 48;
                }
                else
                {
                    sclip = 768;
                    angdif = 16;
                }

                j = hitasprite(g_i,&temphit);
                if(j == (1<<30))
                {
                    parseifelse(1);
                    break;
                }
                if(j > sclip)
                {
                    if(temphit >= 0 && sprite[temphit].picnum == g_sp->picnum)
                        j = 0;
                    else
                    {
                        g_sp->ang += angdif;j = hitasprite(g_i,&temphit);g_sp->ang -= angdif;
                        if(j > sclip)
                        {
                            if(temphit >= 0 && sprite[temphit].picnum == g_sp->picnum)
                                j = 0;
                            else
                            {
                                g_sp->ang -= angdif;j = hitasprite(g_i,&temphit);g_sp->ang += angdif;
                                if( j > 768 )
                                {
                                    if(temphit >= 0 && sprite[temphit].picnum == g_sp->picnum)
                                        j = 0;
                                    else j = 1;
                                }
                                else j = 0;
                            }
                        }
                        else j = 0;
                    }
                }
                else j =  0;
            }
            else j = 1;

            parseifelse(j);
            break;
        case 91:
            j = cansee(g_sp->x,g_sp->y,g_sp->z-((TRAND&41)<<8),g_sp->sectnum,ps[g_p].posx,ps[g_p].posy,ps[g_p].posz/*-((TRAND&41)<<8)*/,sprite[ps[g_p].i].sectnum);
            parseifelse(j);
            if( j ) hittype[g_i].timetosleep = SLEEPTIME;
            break;

        case 49:
            parseifelse(hittype[g_i].actorstayput == -1);
            break;
        case 5:
        {
            spritetype *s;
            short sect;

            if(ps[g_p].holoduke_on >= 0)
            {
                s = &sprite[ps[g_p].holoduke_on];
                j = cansee(g_sp->x,g_sp->y,g_sp->z-(TRAND&((32<<8)-1)),g_sp->sectnum,
                       s->x,s->y,s->z,s->sectnum);
                if(j == 0)
                    s = &sprite[ps[g_p].i];
            }
            else s = &sprite[ps[g_p].i];

            j = cansee(g_sp->x,g_sp->y,g_sp->z-(TRAND&((47<<8))),g_sp->sectnum,
                s->x,s->y,s->z-(24<<8),s->sectnum);

            if(j == 0)
            {
                if( ( klabs2(hittype[g_i].lastvx-g_sp->x)+klabs2(hittype[g_i].lastvy-g_sp->y) ) <
                    ( klabs2(hittype[g_i].lastvx-s->x)+klabs2(hittype[g_i].lastvy-s->y) ) )
                        j = 0;

                if( j == 0 )
                {
                    j = furthestcanseepoint(g_i,s,&hittype[g_i].lastvx,&hittype[g_i].lastvy);

                    if(j == -1) j = 0;
                    else j = 1;
                }
            }
            else
            {
                hittype[g_i].lastvx = s->x;
                hittype[g_i].lastvy = s->y;
            }

            if( j == 1 && ( g_sp->statnum == 1 || g_sp->statnum == 6 ) )
                hittype[g_i].timetosleep = SLEEPTIME;

            parseifelse(j == 1);
            break;
        }

        case 6:
            parseifelse(ifhitbyweapon(g_i) >= 0);
            break;
        case 27:
            parseifelse( ifsquished(g_i, g_p) == 1);
            break;
        case 26:
            {
                j = g_sp->extra;
                if(g_sp->picnum == APLAYER)
                    j--;
                parseifelse(j < 0);
            }
            break;
        case 24:
            insptr++;
            g_t[5] = *insptr;
            g_t[4] = *(long *)(g_t[5]);       // Action
            g_t[1] = *(long *)(g_t[5]+4);       // move
            g_sp->hitag = *(long *)(g_t[5]+8);    // Ai
            g_t[0] = g_t[2] = g_t[3] = 0;
            if(g_sp->hitag&random_angle)
                g_sp->ang = TRAND&2047;
            insptr++;
            break;
        case 7:
            insptr++;
            g_t[2] = 0;
            g_t[3] = 0;
            g_t[4] = *insptr;
            insptr++;
            break;

        case 8:
            insptr++;
            parseifelse(g_x < *insptr);
            if(g_x > MAXSLEEPDIST && hittype[g_i].timetosleep == 0)
                hittype[g_i].timetosleep = SLEEPTIME;
            break;
        case 9:
            insptr++;
            parseifelse(g_x > *insptr);
            if(g_x > MAXSLEEPDIST && hittype[g_i].timetosleep == 0)
                hittype[g_i].timetosleep = SLEEPTIME;
            break;
        case 10:
            insptr = (long *) *(insptr+1);
            if (insptr < &script[0] || insptr > &script[MAXSCRIPTSIZE-1]) gameexit("Script overflow 3\n");
            break;
        case 100:
            insptr++;
            g_sp->extra += *insptr;
            insptr++;
            break;
        case 11:
            insptr++;
            g_sp->extra = *insptr;
            hittype[g_i].extra = -1;
            insptr++;
            break;
        case 94:
            insptr++;

            if(ud.coop >= 1 && ud.multimode > 1)
            {
                if(*insptr == 0)
                {
                    for(j=0;j < ps[g_p].weapreccnt;j++)
                        if( ps[g_p].weaprecs[j] == g_sp->picnum )
                            break;

                    parseifelse(j < ps[g_p].weapreccnt && g_sp->owner == g_i);
                }
                else if(ps[g_p].weapreccnt < 16)
                {
                    ps[g_p].weaprecs[ps[g_p].weapreccnt++] = g_sp->picnum;
                    parseifelse(g_sp->owner == g_i);
                }
            }
            else parseifelse(0);
            break;
        case 95:
            insptr++;
            if(g_sp->picnum == APLAYER)
                g_sp->pal = ps[g_sp->yvel].palookup;
            else g_sp->pal = hittype[g_i].tempang;
            hittype[g_i].tempang = 0;
            break;
        case 104:
            insptr++;
            checkweapons(&ps[g_sp->yvel]);
            break;
        case 106:
            insptr++;
            break;
        case 97:
            insptr++;
            if(Sound[g_sp->yvel].num == 0)
                spritesound(g_sp->yvel,g_i);
            break;
        case 96:
            insptr++;

            if( ud.multimode > 1 && g_sp->picnum == APLAYER )
            {
                if(ps[otherp].quick_kick == 0)
                    ps[otherp].quick_kick = 14;
            }
            else if(g_sp->picnum != APLAYER && ps[g_p].quick_kick == 0)
                ps[g_p].quick_kick = 14;
            break;
        case 28:
            insptr++;

            j = ((*insptr)-g_sp->xrepeat)<<1;
            g_sp->xrepeat += ksgn(j);

            insptr++;

            if( ( g_sp->picnum == APLAYER && g_sp->yrepeat < 36 ) || *insptr < g_sp->yrepeat || ((g_sp->yrepeat*(gettilesizy(g_sp->picnum)+8))<<2) < (hittype[g_i].floorz - hittype[g_i].ceilingz) )
            {
                j = ((*insptr)-g_sp->yrepeat)<<1;
                if( klabs2(j) ) g_sp->yrepeat += ksgn(j);
            }

            insptr++;

            break;
        case 99:
            insptr++;
            g_sp->xrepeat = (char) *insptr;
            insptr++;
            g_sp->yrepeat = (char) *insptr;
            insptr++;
            break;
        case 13:
            insptr++;
            shoot(g_i,(short)*insptr);
            insptr++;
            break;
        case 87:
            insptr++;
            if( Sound[*insptr].num == 0 )
                spritesound((short) *insptr,g_i);
            insptr++;
            break;
        case 89:
            insptr++;
            if( Sound[*insptr].num > 0 )
                stopsound((short)*insptr);
            insptr++;
            break;
        case 92:
            insptr++;
            spritesound((short) *insptr,ps[g_p].i);
            insptr++;
            break;
        case 15:
            insptr++;
            spritesound((short) *insptr,g_i);
            insptr++;
            break;
        case 84:
            insptr++;
            ps[g_p].tipincs = 26;
            break;
        case 16:
            insptr++;
            g_sp->xoffset = 0;
            g_sp->yoffset = 0;
//            if(!gotz)
            {
                long c;

                if( floorspace(g_sp->sectnum) )
                    c = 0;
                else
                {
                    if( ceilingspace(g_sp->sectnum) || sector[g_sp->sectnum].lotag == 2)
                        c = gc/6;
                    else c = gc;
                }

                if( hittype[g_i].cgg <= 0 || (sector[g_sp->sectnum].floorstat&2) )
                {
                    getglobalz(g_i);
                    hittype[g_i].cgg = 6;
                }
                else hittype[g_i].cgg --;

                if( g_sp->z < (hittype[g_i].floorz-FOURSLEIGHT) )
                {
                    g_sp->zvel += c;
                    g_sp->z+=g_sp->zvel;

                    if(g_sp->zvel > 6144) g_sp->zvel = 6144;
                }
                else
                {
                    g_sp->z = hittype[g_i].floorz - FOURSLEIGHT;

                    if( badguy(g_sp) || ( g_sp->picnum == APLAYER && g_sp->owner >= 0) )
                    {

                        if( g_sp->zvel > 3084 && g_sp->extra <= 1)
                        {
                            if(g_sp->pal != 1 && g_sp->picnum != DRONE)
                            {
                                if(g_sp->picnum == APLAYER && g_sp->extra > 0)
                                    goto SKIPJIBS;
                                guts(g_sp,JIBS6,15,g_p);
                                spritesound(SQUISHED,g_i);
                                spawn(g_i,BLOODPOOL);
                            }

                            SKIPJIBS:

                            hittype[g_i].picnum = SHOTSPARK1;
                            hittype[g_i].extra = 1;
                            g_sp->zvel = 0;
                        }
                        else if(g_sp->zvel > 2048 && sector[g_sp->sectnum].lotag != 1)
                        {

                            j = g_sp->sectnum;
                            pushmove(&g_sp->x,&g_sp->y,&g_sp->z,&j,128L,(4L<<8),(4L<<8),CLIPMASK0);
                            if(j != g_sp->sectnum && j >= 0 && j < MAXSECTORS)
                                changespritesect(g_i,j);

                            spritesound(THUD,g_i);
                        }
                    }
                    if(sector[g_sp->sectnum].lotag == 1)
                        switch (g_sp->picnum)
                        {
                            case OCTABRAIN:
                            case COMMANDER:
                            case DRONE:
                                break;
                            default:
                                g_sp->z += (24<<8);
                                break;
                        }
                    else g_sp->zvel = 0;
                }
            }

            break;
        case 4:
        case 12:
        case 18:
            return 1;
        case 30:
            insptr++;
            return 1;
        case 2:
            insptr++;
            if( ps[g_p].ammo_amount[*insptr] >= max_ammo_amount[*insptr] )
            {
                killit_flag = 2;
                break;
            }
            addammo( *insptr, &ps[g_p], *(insptr+1) );
            if(ps[g_p].curr_weapon == KNEE_WEAPON)
                if( ps[g_p].gotweapon[*insptr] )
                    addweapon( &ps[g_p], *insptr );
            insptr += 2;
            break;
        case 86:
            insptr++;
            lotsofmoney(g_sp,*insptr);
            insptr++;
            break;
        case 102:
            insptr++;
            lotsofmail(g_sp,*insptr);
            insptr++;
            break;
        case 105:
            insptr++;
            hittype[g_i].timetosleep = (short)*insptr;
            insptr++;
            break;
        case 103:
            insptr++;
            lotsofpaper(g_sp,*insptr);
            insptr++;
            break;
        case 88:
            insptr++;
            if (sprite[g_i].picnum == 0xEDD || sprite[g_i].picnum == 0xED5 || sprite[g_i].picnum == 0xEED)
                ps[g_p].f_36e += *insptr;
            else
                ps[g_p].actors_killed += *insptr;
            hittype[g_i].actorstayput = -1;
            insptr++;
            break;
        case 93:
            insptr++;
            spriteglass(g_i,*insptr);
            insptr++;
            break;
        case 22:
            insptr++;
            killit_flag = 1;
            break;
        case 23:
            insptr++;
            if( ps[g_p].gotweapon[*insptr] == 0 ) addweapon( &ps[g_p], *insptr );
            else if( ps[g_p].ammo_amount[*insptr] >= max_ammo_amount[*insptr] )
            {
                 killit_flag = 2;
                 break;
            }
            addammo( *insptr, &ps[g_p], *(insptr+1) );
            if(ps[g_p].curr_weapon == KNEE_WEAPON)
                if( ps[g_p].gotweapon[*insptr] )
                    addweapon( &ps[g_p], *insptr );
            insptr+=2;
            break;
        /*case 68:
            insptr++;
            printf("%ld\n",*insptr);
            insptr++;
            break;*/
        case 69:
            insptr++;
            ps[g_p].timebeforeexit = *insptr;
            ps[g_p].customexitsound = -1;
            ud.eog = 1;
            earthquaketime = *insptr;
            insptr++;
            break;
        case 25:
            insptr++;

            if(ps[g_p].newowner >= 0)
            {
                ps[g_p].newowner = -1;
                ps[g_p].posx = ps[g_p].oposx;
                ps[g_p].posy = ps[g_p].oposy;
                ps[g_p].posz = ps[g_p].oposz;
                ps[g_p].ang = ps[g_p].oang;
                updatesector(ps[g_p].posx,ps[g_p].posy,&ps[g_p].cursectnum);
                //setpal(&ps[g_p]);

                j = headspritestat[1];
                while(j >= 0)
                {
                    if(sprite[j].picnum==CAMERA1)
                        sprite[j].yvel = 0;
                    j = nextspritestat[j];
                }
            }

            j = sprite[ps[g_p].i].extra;

            if(g_sp->picnum != ATOMICHEALTH)
            {
                if( j > max_player_health && *insptr > 0 )
                {
                    insptr++;
                    break;
                }
                else
                {
                    if(j > 0)
                        j += *insptr;
                    if ( j > max_player_health && *insptr > 0 )
                        j = max_player_health;
                }
            }
            else
            {
                if( j > 0 )
                    j += *insptr;
                if ( j > (max_player_health<<1) )
                    j = (max_player_health<<1);
            }

            if(j < 0) j = 0;

            if(ud.god == 0)
            {
                if(*insptr > 0)
                {
                    if( ( j - *insptr ) < (max_player_health>>2) &&
                        j >= (max_player_health>>2) )
                            spritesound(DUKE_GOTHEALTHATLOW,ps[g_p].i);

                    ps[g_p].last_extra = j;
                }

                sprite[ps[g_p].i].extra = j;
            }

            insptr++;
            break;
        case 17:
            {
                long *tempscrptr;

                tempscrptr = insptr+2;

                insptr = (long *) *(insptr+1);
                if (insptr < &script[0] || insptr > &script[MAXSCRIPTSIZE-1]) gameexit("Script overflow 5\n");
                while(1) if(parse()) break;
                insptr = tempscrptr;
            }
            break;
        case 29:
            insptr++;
            while(1) if(parse()) break;
            break;
        case 32:
            g_t[0]=0;
            insptr++;
            g_t[1] = *insptr;
            insptr++;
            g_sp->hitag = *insptr;
            insptr++;
            if(g_sp->hitag&random_angle)
                g_sp->ang = TRAND&2047;
            break;
        case 31:
            insptr++;
            if(g_sp->sectnum >= 0 && g_sp->sectnum < MAXSECTORS)
                spawn(g_i,*insptr);
            insptr++;
            break;
        case 33:
            insptr++;
            picnum = hittype[g_i].picnum;
            if (picnum == 0xA27 || picnum == 0xA2D)
                picnum = 0x686;
            if (picnum == 0xA26 || picnum == 0xA25)
                picnum = 0xA23;
            parseifelse( picnum == *insptr);
            break;
        case 21:
            insptr++;
            parseifelse(g_t[5] == *insptr);
            break;
        case 34:
            insptr++;
            parseifelse(g_t[4] == *insptr);
            break;
        case 35:
            insptr++;
            parseifelse(g_t[2] >= *insptr);
            break;
        case 36:
            insptr++;
            g_t[2] = 0;
            break;
        case 37:
            {
                short dnum;

                insptr++;
                dnum = *insptr;
                insptr++;
                n2 = *insptr;
                n = max(1,  n2 / numplayers);
                
                if(g_sp->sectnum >= 0 && g_sp->sectnum < MAXSECTORS)
                    for(j=n-1;j>=0;j--)
                {
                    if(g_sp->picnum == BLIMP && dnum == SCRAP1)
                        s = 0;
                    else s = (krand()%3);

                    l = EGS(g_sp->sectnum,
                            g_sp->x+(krand()&255)-128,g_sp->y+(krand()&255)-128,g_sp->z-(8<<8)-(krand()&8191),
                            dnum+s,g_sp->shade,32+(krand()&15),32+(krand()&15),
                            krand()&2047,(krand()&127)+32,
                            -(krand()&2047),g_i,5);
                    if(g_sp->picnum == BLIMP && dnum == SCRAP1)
                        sprite[l].yvel = weaponsandammosprites[j%14];
                    else sprite[l].yvel = -1;
                    sprite[l].pal = g_sp->pal;
                }
                insptr++;
            }
            break;
        case 52:
            insptr++;
            g_t[0] = (short) *insptr;
            insptr++;
            break;
        case 101:
            insptr++;
            g_sp->cstat |= (short)*insptr;
            insptr++;
            break;
        case 110:
            insptr++;
            g_sp->clipdist = (short) *insptr;
            insptr++;
            break;
        case 40:
            insptr++;
            g_sp->cstat = (short) *insptr;
            insptr++;
            break;
        case 41:
            insptr++;
            parseifelse(g_t[1] == *insptr);
            break;
        case 42:
            insptr++;

            if(ud.multimode < 2)
            {
                ps[g_p].gm = MODE_RESTART;
                death_fade = 5;
                killit_flag = 2;
            }
            else if (ud.coop != 0 || dukematch_mode != 4)
            {
                pickrandomspot(g_p);
                g_sp->x = hittype[g_i].bposx = ps[g_p].bobposx = ps[g_p].oposx = ps[g_p].posx;
                g_sp->y = hittype[g_i].bposy = ps[g_p].bobposy = ps[g_p].oposy =ps[g_p].posy;
                g_sp->z = hittype[g_i].bposy = ps[g_p].oposz =ps[g_p].posz;
                updatesector(ps[g_p].posx,ps[g_p].posy,&ps[g_p].cursectnum);
                setsprite(ps[g_p].i,ps[g_p].posx,ps[g_p].posy,ps[g_p].posz+PHEIGHT);
                g_sp->cstat = 257;

                g_sp->shade = -12;
                g_sp->clipdist = 64;
                g_sp->xrepeat = 42;
                g_sp->yrepeat = 36;
                g_sp->owner = g_i;
                g_sp->xoffset = 0;
                g_sp->pal = ps[g_p].palookup;

                ps[g_p].last_extra = g_sp->extra = max_player_health;
                ps[g_p].wantweaponfire = -1;
                ps[g_p].horiz = 100;
                ps[g_p].on_crane = -1;
                ps[g_p].frag_ps = g_p;
                ps[g_p].horizoff = 0;
                ps[g_p].opyoff = 0;
                ps[g_p].wackedbyactor = -1;
                ps[g_p].shield_amount = max_armour_amount;
                ps[g_p].dead_flag = 0;
                ps[g_p].pals_time = 0;
                ps[g_p].footprintcount = 0;
                ps[g_p].weapreccnt = 0;
                ps[g_p].fta = 0;
                ps[g_p].ftq = 0;
                ps[g_p].posxv = ps[g_p].posyv = 0;
                ps[g_p].rotscrnang = 0;

                ps[g_p].falling_counter = 0;

                hittype[g_i].extra = -1;
                hittype[g_i].owner = g_i;

                hittype[g_i].cgg = 0;
                hittype[g_i].movflag = 0;
                hittype[g_i].tempang = 0;
                hittype[g_i].actorstayput = -1;
                hittype[g_i].dispicnum = 0;
                hittype[g_i].owner = ps[g_p].i;

                resetinventory(g_p);
                resetweapons(g_p);

                cameradist = 0;
                cameraclock = totalclock;
            }
            //setpal(&ps[g_p]);

            break;
        case 43:
            parseifelse( klabs2(g_sp->z-sector[g_sp->sectnum].floorz) < (32<<8) && sector[g_sp->sectnum].lotag == 1);
            break;
        case 44:
            parseifelse( sector[g_sp->sectnum].lotag == 2);
            break;
        case 46:
            insptr++;
            parseifelse(g_t[0] >= *insptr);
            break;
        case 53:
            insptr++;
            parseifelse(g_sp->picnum == *insptr);
            break;
        case 47:
            insptr++;
            g_t[0] = 0;
            break;
        case 48:
            insptr+=2;
            switch(*(insptr-1))
            {
                case 0:
                    ps[g_p].steroids_amount = *insptr;
                    ps[g_p].inven_icon = 2;
                    break;
                case 1:
                    ps[g_p].shield_amount +=          *insptr;// 100;
                    if(ps[g_p].shield_amount > max_player_health)
                        ps[g_p].shield_amount = max_player_health;
                    ps[g_p].inven_icon = 8;
                    break;
                case 2:
                    ps[g_p].scuba_amount =             *insptr;// 1600;
                    ps[g_p].inven_icon = 6;
                    break;
                case 3:
                    ps[g_p].holoduke_amount =          *insptr;// 1600;
                    ps[g_p].inven_icon = 3;
                    break;
                case 4:
                    ps[g_p].jetpack_amount =           *insptr;// 1600;
                    ps[g_p].inven_icon = 4;
                    break;
                case 6:
                    switch(g_sp->pal)
                    {
                        case  0: ps[g_p].got_access |= 1;break;
                        case 21: ps[g_p].got_access |= 2;break;
                        case 23: ps[g_p].got_access |= 4;break;
                    }
                    break;
                case 7:
                    ps[g_p].heat_amount = *insptr;
                    ps[g_p].inven_icon = 5;
                    break;
                case 9:
                    ps[g_p].inven_icon = 1;
                    ps[g_p].firstaid_amount = *insptr;
                    break;
                case 10:
                    ps[g_p].inven_icon = 7;
                    ps[g_p].boot_amount = *insptr;
                    break;
            }
            insptr++;
            break;
        case 50:
            hitradius(g_i,*(insptr+1),*(insptr+2),*(insptr+3),*(insptr+4),*(insptr+5));
            insptr+=6;
            break;
        case 51:
            {
                insptr++;

                l = *insptr;
                j = 0;

                s = g_sp->xvel;

                if( (l&8) && ps[g_p].on_ground && (sync[g_p].bits&2) )
                       j = 1;
                else if( (l&16) && ps[g_p].jumping_counter == 0 && !ps[g_p].on_ground &&
                    ps[g_p].poszv > 2048 )
                        j = 1;
                else if( (l&32) && ps[g_p].jumping_counter > 348 )
                       j = 1;
                else if( (l&1) && s >= 0 && s < 8)
                       j = 1;
                else if( (l&2) && s >= 8 && !(sync[g_p].bits&(1<<5)) )
                       j = 1;
                else if( (l&4) && s >= 8 && sync[g_p].bits&(1<<5) )
                       j = 1;
                else if( (l&64) && ps[g_p].posz < (g_sp->z-(48<<8)) )
                       j = 1;
                else if( (l&128) && s <= -8 && !(sync[g_p].bits&(1<<5)) )
                       j = 1;
                else if( (l&256) && s <= -8 && (sync[g_p].bits&(1<<5)) )
                       j = 1;
                else if( (l&512) && ( ps[g_p].quick_kick > 0 || ( ps[g_p].curr_weapon == KNEE_WEAPON && ps[g_p].kickback_pic > 0 ) ) )
                       j = 1;
                else if( (l&1024) && sprite[ps[g_p].i].xrepeat < 32 )
                       j = 1;
                else if( (l&2048) && ps[g_p].jetpack_on )
                       j = 1;
                else if( (l&4096) && ps[g_p].steroids_amount > 0 && ps[g_p].steroids_amount < 400 )
                       j = 1;
                else if( (l&8192) && ps[g_p].on_ground)
                       j = 1;
                else if( (l&16384) && sprite[ps[g_p].i].xrepeat > 32 && sprite[ps[g_p].i].extra > 0 )
                       j = 1;
                else if( (l&32768) && sprite[ps[g_p].i].extra <= 0)
                       j = 1;
                else if( (l&65536L) )
                {
                    if(g_sp->picnum == APLAYER && ud.multimode > 1)
                        j = getincangle(ps[otherp].ang,getangle(ps[g_p].posx-ps[otherp].posx,ps[g_p].posy-ps[otherp].posy));
                    else
                        j = getincangle(ps[g_p].ang,getangle(g_sp->x-ps[g_p].posx,g_sp->y-ps[g_p].posy));

                    if( j > -128 && j < 128 )
                        j = 1;
                    else
                        j = 0;
                }

                parseifelse((long) j);

            }
            break;
        case 56:
            insptr++;
            parseifelse(g_sp->extra <= *insptr);
            break;
        case 58:
            insptr += 2;
            guts(g_sp,*(insptr-1),*insptr,g_p);
            insptr++;
            break;
        case 59:
            insptr++;
//            if(g_sp->owner >= 0 && sprite[g_sp->owner].picnum == *insptr)
  //              parseifelse(1);
//            else
            parseifelse( hittype[g_i].picnum == *insptr);
            break;
        case 61:
            insptr++;
            forceplayerangle(&ps[g_p]);
            return 0;
        case 62:
            insptr++;
            parseifelse( (( hittype[g_i].floorz - hittype[g_i].ceilingz ) >> 8 ) < *insptr);
            break;
        case 63:
            parseifelse( sync[g_p].bits&(1<<29));
            break;
        case 64:
            parseifelse(sector[g_sp->sectnum].ceilingstat&1);
            break;
        case 65:
            parseifelse(ud.multimode > 1);
            break;
        case 66:
            insptr++;
            if( sector[g_sp->sectnum].lotag == 0 )
            {
                neartag(g_sp->x,g_sp->y,g_sp->z-(32<<8),g_sp->sectnum,g_sp->ang,&neartagsector,&neartagwall,&neartagsprite,&neartaghitdist,768L,1);
                if( neartagsector >= 0 && isanearoperator(sector[neartagsector].lotag) )
                    if( (sector[neartagsector].lotag&0xff) == 23 || sector[neartagsector].floorz == sector[neartagsector].ceilingz )
                        if( (sector[neartagsector].lotag&16384) == 0 )
                            if( (sector[neartagsector].lotag&32768) == 0 )
                        {
                            j = headspritesect[neartagsector];
                            while(j >= 0)
                            {
                                if(sprite[j].picnum == ACTIVATOR)
                                    break;
                                j = nextspritesect[j];
                            }
                            if(j == -1)
                                operatesectors(neartagsector,g_i);
                        }
            }
            break;
        case 67:
            parseifelse(ceilingspace(g_sp->sectnum));
            break;

        case 74:
            insptr++;
            if(g_sp->picnum != APLAYER)
                hittype[g_i].tempang = g_sp->pal;
            g_sp->pal = *insptr;
            insptr++;
            break;

        case 77:
            insptr++;
            g_sp->picnum = *insptr;
            insptr++;
            break;

        case 70:
            parseifelse( dodge(g_sp) == 1);
            break;
        case 71:
            if( badguy(g_sp) || iscorpse(g_sp) )
                parseifelse( ud.respawn_monsters );
            else if( inventory(g_sp) )
                parseifelse( ud.respawn_inventory );
            else
                parseifelse( ud.respawn_items );
            break;
        case 72:
            insptr++;
//            getglobalz(g_i);
            parseifelse( (hittype[g_i].floorz - g_sp->z) <= ((*insptr)<<8));
            break;
        case 73:
            insptr++;
//            getglobalz(g_i);
            parseifelse( ( g_sp->z - hittype[g_i].ceilingz ) <= ((*insptr)<<8));
            break;
        case 14:

            insptr++;
            ps[g_p].pals_time = *insptr;
            insptr++;
            for(j=0;j<3;j++)
            {
                ps[g_p].pals[j] = *insptr;
                insptr++;
            }
            break;

/*        case 74:
            insptr++;
            getglobalz(g_i);
            parseifelse( (( hittype[g_i].floorz - hittype[g_i].ceilingz ) >> 8 ) >= *insptr);
            break;
*/
        case 78:
            insptr++;
            parseifelse( sprite[ps[g_p].i].extra < *insptr);
            break;

        case 75:
            {
                insptr++;
                j = 0;
                switch(*(insptr++))
                {
                    case 0:if( ps[g_p].steroids_amount != *insptr)
                           j = 1;
                        break;
                    case 1:if(ps[g_p].shield_amount != max_player_health )
                            j = 1;
                        break;
                    case 2:if(ps[g_p].scuba_amount != *insptr) j = 1;break;
                    case 3:if(ps[g_p].holoduke_amount != *insptr) j = 1;break;
                    case 4:if(ps[g_p].jetpack_amount != *insptr) j = 1;break;
                    case 6:
                        switch(g_sp->pal)
                        {
                            case  0: if(ps[g_p].got_access&1) j = 1;break;
                            case 21: if(ps[g_p].got_access&2) j = 1;break;
                            case 23: if(ps[g_p].got_access&4) j = 1;break;
                        }
                        break;
                    case 7:if(ps[g_p].heat_amount != *insptr) j = 1;break;
                    case 9:
                        if(ps[g_p].firstaid_amount != *insptr) j = 1;break;
                    case 10:
                        if(ps[g_p].boot_amount != *insptr) j = 1;break;
                }

                parseifelse(j);
                break;
            }
        case 38:
            insptr++;
            if( ps[g_p].knee_incs == 0 && sprite[ps[g_p].i].xrepeat >= 40 )
                if( cansee(g_sp->x,g_sp->y,g_sp->z-(4<<8),g_sp->sectnum,ps[g_p].posx,ps[g_p].posy,ps[g_p].posz+(16<<8),sprite[ps[g_p].i].sectnum) )
            {
                ps[g_p].knee_incs = 1;
                if(ps[g_p].weapon_pos == 0)
                    ps[g_p].weapon_pos = -1;
                ps[g_p].actorsqu = g_i;
            }
            break;
        case 90:
            {
                short s1;

                s1 = g_sp->sectnum;

                j = 0;

                    updatesector(g_sp->x+108,g_sp->y+108,&s1);
                    if( s1 == g_sp->sectnum )
                    {
                        updatesector(g_sp->x-108,g_sp->y-108,&s1);
                        if( s1 == g_sp->sectnum )
                        {
                            updatesector(g_sp->x+108,g_sp->y-108,&s1);
                            if( s1 == g_sp->sectnum )
                            {
                                updatesector(g_sp->x-108,g_sp->y+108,&s1);
                                if( s1 == g_sp->sectnum )
                                    j = 1;
                            }
                        }
                    }
                    parseifelse( j );
            }

            break;
        case 80:
            insptr++;
            FTA(*insptr,&ps[g_p]);
            insptr++;
            break;
        case 81:
            parseifelse( floorspace(g_sp->sectnum));
            break;
        case 82:
            parseifelse( (hittype[g_i].movflag&49152) > 16384 );
            break;
        case 83:
            insptr++;
            switch(g_sp->picnum)
            {
                case FEM1:
                case FEM2:
                case FEM3:
                case FEM4:
                case FEM5:
                case FEM6:
                case FEM7:
                case FEM8:
                case FEM9:
                case FEM10:
                case PODFEM1:
                case NAKED1:
                case STATUE:
                    if(g_sp->yvel) operaterespawns(g_sp->yvel);
                    break;
                default:
                    if(g_sp->hitag >= 0) operaterespawns(g_sp->hitag);
                    break;
            }
            break;
        case 85:
            insptr++;
            parseifelse( g_sp->pal == *insptr);
            break;

        case 111:
            insptr++;
            j = klabs2(getincangle(ps[g_p].ang,g_sp->ang));
            parseifelse( j <= *insptr);
            break;

        case 109:

            for(j=1;j<NUM_SOUNDS;j++)
                if( SoundOwner[j][0].i == g_i )
                    break;

            j = NUM_SOUNDS;
            parseifelse( j == NUM_SOUNDS );
            break;
        default:
            killit_flag = 1;
            break;
    }
    return 0;
}
#else
#pragma GLOBAL_ASM("nonmatchings/src/gamedef/parse.s")
#endif

void execute(short i,short p,long x)
{
    char done;

    g_i = i;
    g_p = p;
    g_x = x;
    g_sp = &sprite[g_i];
    g_t = &hittype[g_i].temp_data[0];

    if( actorscrptr[g_sp->picnum] == 0 ) return;

    insptr = &script[4 + actorscrptr[g_sp->picnum]];
    if (insptr < &script[0] || insptr > &script[MAXSCRIPTSIZE-1])
        gameexit("Script overflow 6\n");

    killit_flag = 0;

    if(g_sp->sectnum < 0 || g_sp->sectnum >= MAXSECTORS)
    {
        if(badguy(g_sp))
            ps[g_p].actors_killed++;
        deletesprite(g_i);
        return;
    }

    if(g_t[4])
    {
        g_sp->lotag += TICSPERFRAME;
        if(g_sp->lotag > *(long *)(g_t[4]+16) )
        {
            g_t[2]++;
            g_sp->lotag = 0;
            g_t[3] +=  *(long *)( g_t[4]+12 );
        }
        if( klabs2(g_t[3]) >= klabs2( *(long *)(g_t[4]+4) * *(long *)(g_t[4]+12) ) )
            g_t[3] = 0;
    }

    do
        done = parse();
    while( done == 0 );

    if(killit_flag == 1)
    {
        if(ps[g_p].actorsqu == g_i)
            ps[g_p].actorsqu = -1;
        deletesprite(g_i);
    }
    else
    {
        move();

        if( g_sp->statnum == 1)
        {
            if( badguy(g_sp) )
            {
                if( g_sp->xrepeat > 60 ) return;
                if( ud.respawn_monsters == 1 && g_sp->extra <= 0 ) return;
            }
            else if( ud.respawn_items == 1 && (g_sp->cstat&32768) ) return;

            if(hittype[g_i].timetosleep > 1)
                hittype[g_i].timetosleep--;
            else if(hittype[g_i].timetosleep == 1)
                 changespritestat(g_i,2);
        }

        else if(g_sp->statnum == 6)
            switch(g_sp->picnum)
            {
                case RUBBERCAN:
                case EXPLODINGBARREL:
                case WOODENHORSE:
                case HORSEONSIDE:
                case CANWITHSOMETHING:
                case FIREBARREL:
                case NUKEBARREL:
                case NUKEBARRELDENTED:
                case NUKEBARRELLEAKED:
                case TRIPBOMB:
                case EGG:
                    if(hittype[g_i].timetosleep > 1)
                        hittype[g_i].timetosleep--;
                    else if(hittype[g_i].timetosleep == 1)
                        changespritestat(g_i,2);
                    break;
            }
    }
}

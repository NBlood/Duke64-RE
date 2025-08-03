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

long cameradist = 0, cameraclock = 0;

void updatesectorz(long x, long y, long z, short *sectnum)
{
    walltype *wal;
    long i, j, cz, fz;

    getzsofslope(*sectnum,x,y,&cz,&fz);
    if ((z >= cz) && (z <= fz))
        if (inside(x,y,*sectnum) != 0) return;

    if ((*sectnum >= 0) && (*sectnum < numsectors))
    {
        wal = &wall[sector[*sectnum].wallptr];
        j = sector[*sectnum].wallnum;
        do
        {
            i = wal->nextsector;
            if (i >= 0)
            {
                getzsofslope(i,x,y,&cz,&fz);
                if ((z >= cz) && (z <= fz))
                    if (inside(x,y,(short)i) == 1)
                        { *sectnum = i; return; }
            }
            wal++; j--;
        } while (j != 0);
    }

    for(i=numsectors-1;i>=0;i--)
    {
        getzsofslope(i,x,y,&cz,&fz);
        if ((z >= cz) && (z <= fz))
            if (inside(x,y,(short)i) == 1)
                { *sectnum = i; return; }
    }

    *sectnum = -1;
}

void view(struct player_struct *pp, long *vx, long *vy,long *vz,short *vsectnum, short ang, short horiz)
{
     spritetype *sp;
     long i, nx, ny, nz, hx, hy, hz, hitx, hity, hitz;
     short bakcstat, hitsect, hitwall, hitsprite, daang;

     nx = (sintable[(ang+1536)&2047]>>4);
     ny = (sintable[(ang+1024)&2047]>>4);
     nz = (horiz-100)*128;

     sp = &sprite[pp->i];

     bakcstat = sp->cstat;
     sp->cstat &= (short)~0x101;

     updatesectorz(*vx,*vy,*vz,vsectnum);

     nx = (nx << 14) / 8;
     ny = (ny << 14) / 8;

     for (i = 0; i < 8; i++)
     {
         if (clipmove(vx, vy, vz, vsectnum, nx, ny, 128, 1024, 1024, CLIPMASK0) != 0)
             break;
     }

     if(*vsectnum < 0)
     {
        sp->cstat = bakcstat;
        return;
     }

     cameradist += ((totalclock-cameraclock)<<10);
     if (cameradist > 65536)
         cameradist = 65536;
     cameraclock = totalclock;

     updatesectorz(*vx,*vy,*vz,vsectnum);

     sp->cstat = bakcstat;
}

void animatecamsprite(void)
{
    short i;

    if(camsprite <= 0) return;

    i = camsprite;

    if(T1 >= 11)
    {
        T1 = 0;

        if(ps[screenpeek].newowner >= 0)
            OW = ps[screenpeek].newowner;

        //else if(OW >= 0 && dist(&sprite[ps[screenpeek].i],&sprite[i]) < 2048)
        //    xyzmirror(OW,PN);
    }
    else T1++;
}

extern long lastvisinc;

void displayrooms(short snum,long smoothratio)
{
    long cposx,cposy,cposz,dst,j,fz,cz,hz,lz;
    short sect, cang, k, choriz,tsect;
    struct player_struct *p;
    long tposx,tposy,tposz,dx,dy,thoriz,i;
    short tang;

    p = &ps[snum];

    sect = p->cursectnum;
    if(sect < 0 || sect >= MAXSECTORS) return;

    dointerpolations(smoothratio);

    animatecamsprite();

    if(ud.camerasprite >= 0)
    {
        spritetype *s;

        s = &sprite[ud.camerasprite];

        if(s->yvel < 0) s->yvel = -100;
        else if(s->yvel > 199) s->yvel = 300;

        cang = hittype[ud.camerasprite].tempang+mulscale16((long)(((s->ang+1024-hittype[ud.camerasprite].tempang)&2047)-1024),smoothratio);

        globalsectnum = s->sectnum;
        globalang = cang;
        globalhoriz = s->yvel;
        globalposx = s->x;
        globalposy = s->y;
        globalposz = s->z-(4<<8);
    }
    else
    {
        i = divscale22(1,sprite[p->i].yrepeat+28);

				cposx = p->oposx+mulscale16((long)(p->posx-p->oposx),smoothratio);
				cposy = p->oposy+mulscale16((long)(p->posy-p->oposy),smoothratio);
				cposz = p->oposz+mulscale16((long)(p->posz-p->oposz),smoothratio);
				cang = p->oang+mulscale16((long)(((p->ang+1024-p->oang)&2047)-1024),smoothratio);
				choriz = p->ohoriz+p->ohorizoff+mulscale16((long)(p->horiz+p->horizoff-p->ohoriz-p->ohorizoff),smoothratio);

		  cang += p->look_ang;

		  if (p->newowner >= 0)
		  {
              i = p->newowner;
				cposx = sprite[i].x;
				cposy = sprite[i].y;
				cposz = sprite[i].z;
				cang = sprite[i].ang;
				sect = sprite[i].sectnum;
				smoothratio = 65536L;
		  }

		  else if( p->over_shoulder_on == 0 )
				cposz += p->opyoff+mulscale16((long)(p->pyoff-p->opyoff),smoothratio);
		  else view(p,&cposx,&cposy,&cposz,&sect,cang,choriz);

        cz = hittype[p->i].ceilingz;
        fz = hittype[p->i].floorz;

        if(earthquaketime > 0)
        {
            short u = earthquaketime+snum;
            cposz += 256-(((u)&1)<<9);
            cang += (2-((u)&2))<<2;
        }

        if(sprite[p->i].pal == 1) cposz -= (18<<8);

        if(p->newowner >= 0)
            choriz = 100+sprite[p->newowner].shade;
        else
        {
            choriz = choriz + p->f_370;
            if(p->spritebridge == 0)
            {
                if( cposz < ( p->truecz + (4<<8) ) ) cposz = cz + (4<<8);
                else if( cposz > ( p->truefz - (4<<8) ) ) cposz = fz - (4<<8);
            }
        }

        if (sect >= 0)
        {
            getzsofslope(sect,cposx,cposy,&cz,&fz);
            if (cposz < cz+(4<<8)) cposz = cz+(4<<8);
            if (cposz > fz-(4<<8)) cposz = fz-(4<<8);
        }

        choriz = max(-28, min(228, choriz));

        globalsectnum = sect;
        globalang = cang;
        globalhoriz = choriz;
        globalposx = cposx;
        globalposy = cposy;
        globalposz = cposz;
    }

    restoreinterpolations();

    if (totalclock < lastvisinc)
    {
        if (klabs(p->visibility-512) > 8)
            p->visibility += (512-p->visibility)>>2;
    }
    else p->visibility = 512;
}

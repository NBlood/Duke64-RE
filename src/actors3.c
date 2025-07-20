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

short getanglediff(short ang1, short ang2) {
    short ma;

    ma = ang1 < ang2 ? ang1 : ang2;
    ang1 -= ma;
    ang2 -= ma;
    if (ang1 < ang2) {
        if (ang2 <= 1024)
            return ang2;
        return ang2 - 2048;
    }
    else
    {
        if (ang1 <= 1024)
            return -ang1;
        return 2048 - ang1;
    }
}

void hitradius( short i, long  r, long  hp1, long  hp2, long  hp3, long  hp4 )
{
    spritetype *s,*sj;
    walltype *wal;
    long d, q, x1, y1;
    long sectcnt, sectend, dasect, startwall, endwall, nextsect;
    short j,k,p,x,nextj,sect;
    char statlist[] = {0,1,6,10,12,2,5};
    short *tempshort = (short *)tempbuf;

    s = &sprite[i];

    //if(s->picnum == RPG && s->xrepeat < 11) goto SKIPWALLCHECK;

    if(s->picnum != SHRINKSPARK)
    {
        tempshort[0] = s->sectnum;
        dasect = s->sectnum;
        sectcnt = 0; sectend = 1;

        do
        {
            dasect = tempshort[sectcnt++];
            if(((sector[dasect].ceilingz-s->z)>>8) < r)
            {
               d = klabs2(wall[sector[dasect].wallptr].x-s->x)+klabs2(wall[sector[dasect].wallptr].y-s->y);
               if(d < r)
                    checkhitceiling(dasect);
               else
               {
                    d = klabs2(wall[wall[wall[sector[dasect].wallptr].point2].point2].x-s->x)+klabs2(wall[wall[wall[sector[dasect].wallptr].point2].point2].y-s->y);
                    if(d < r)
                        checkhitceiling(dasect);
               }
           }

           startwall = sector[dasect].wallptr;
           endwall = startwall+sector[dasect].wallnum;
           for(x=startwall,wal=&wall[startwall];x<endwall;x++,wal++)
               if( ( klabs2(wal->x-s->x)+klabs2(wal->y-s->y) ) < r)
           {
               nextsect = wal->nextsector;
               if (nextsect >= 0)
               {
                   for(dasect=sectend-1;dasect>=0;dasect--)
                       if (tempshort[dasect] == nextsect) break;
                   if (dasect < 0) tempshort[sectend++] = nextsect;
               }
               x1 = (((wal->x+wall[wal->point2].x)>>1)+s->x)>>1;
               y1 = (((wal->y+wall[wal->point2].y)>>1)+s->y)>>1;
               updatesector(x1,y1,&sect);
               if( sect >= 0 && cansee(x1,y1,s->z,sect,s->x,s->y,s->z,s->sectnum ) )
                   checkhitwall(i,x,wal->x,wal->y,s->z,s->picnum);
           }
        }
        while (sectcnt < sectend);
    }

    SKIPWALLCHECK:

    q = -(16<<8)+(TRAND&((32<<8)-1));

    for(x = 0;x<7;x++)
    {
        j = headspritestat[statlist[x]];
        while(j >= 0)
        {
            nextj = nextspritestat[j];
            sj = &sprite[j];

            if( x == 0 || x >= 5 || AFLAMABLE(sj->picnum) )
            {
                if( s->picnum != SHRINKSPARK || (sj->cstat&257) )
                    if( dist( s, sj ) < r )
                    {
                        if( badguy(sj) && !cansee( sj->x, sj->y,sj->z+q, sj->sectnum, s->x, s->y, s->z+q, s->sectnum) )
                            goto BOLT;
                        checkhitsprite( j, i );
                    }
            }
            else if( sj->extra >= 0 && sj != s && ( sj->picnum == TRIPBOMB || badguy(sj) || sj->picnum == QUEBALL || sj->picnum == STRIPEBALL || (sj->cstat&257) || sj->picnum == DUKELYINGDEAD ) )
            {
                if( s->picnum == SHRINKSPARK && sj->picnum != SHARK && ( j == s->owner || sj->xrepeat < 24 ) )
                {
                    j = nextj;
                    continue;
                }
                if( (s->picnum == MORTER || s->picnum == TILE3841) && j == s->owner)
                {
                    j = nextj;
                    continue;
                }

                if(sj->picnum == APLAYER) sj->z -= PHEIGHT;
                d = dist( s, sj );
                if(sj->picnum == APLAYER) sj->z += PHEIGHT;

                if ( d < r && cansee( sj->x, sj->y, sj->z-(8<<8), sj->sectnum, s->x, s->y, s->z-(12<<8), s->sectnum) )
                {
                    hittype[j].ang = getangle(sj->x-s->x,sj->y-s->y);

                    if ( s->picnum == RPG && sj->extra > 0)
                        hittype[j].picnum = RPG;
                    else
                    {
                        if( s->picnum == SHRINKSPARK )
                            hittype[j].picnum = SHRINKSPARK;
                        else if( s->picnum == TILE3841 )
                            hittype[j].picnum = TILE3841;
                        else hittype[j].picnum = RADIUSEXPLOSION;
                    }

                    if(s->picnum != SHRINKSPARK)
                    {
                        if ( d < r/3 )
                        {
                            if(hp4 == hp3) hp4++;
                            hittype[j].extra = hp3 + (TRAND%(hp4-hp3));
                        }
                        else if ( d < 2*r/3 )
                        {
                            if(hp3 == hp2) hp3++;
                            hittype[j].extra = hp2 + (TRAND%(hp3-hp2));
                        }
                        else if ( d < r )
                        {
                            if(hp2 == hp1) hp2++;
                            hittype[j].extra = hp1 + (TRAND%(hp2-hp1));
                        }

                        if( sprite[j].picnum != ROTATEGUN && sprite[j].picnum != RECON && sprite[j].picnum != BOSS1 && sprite[j].picnum != BOSS2 && sprite[j].picnum != BOSS3 )
                        {
                            if(sj->xvel < 0) sj->xvel = 0;
                            sj->xvel += (s->extra<<2);
                        }

                        if( sj->picnum == PODFEM1 || sj->picnum == FEM1 ||
                            sj->picnum == FEM2 || sj->picnum == FEM3 ||
                            sj->picnum == FEM4 || sj->picnum == FEM5 ||
                            sj->picnum == FEM6 || sj->picnum == FEM7 ||
                            sj->picnum == FEM8 || sj->picnum == FEM9 ||
                            sj->picnum == FEM10 || sj->picnum == STATUE ||
                            sj->picnum == STATUEFLASH || sj->picnum == SPACEMARINE || sj->picnum == QUEBALL || sj->picnum == STRIPEBALL)
                                checkhitsprite( j, i );
                    }
                    else if(s->extra == 0) hittype[j].extra = 0;

                    if ( sj->picnum != RADIUSEXPLOSION &&
                        s->owner >= 0 && sprite[s->owner].statnum < MAXSTATUS )
                    {
                        if(sj->picnum == APLAYER)
                        {
                            p = sj->yvel;
                            if(ps[p].newowner >= 0)
                            {
                                ps[p].newowner = -1;
                                ps[p].posx = ps[p].oposx;
                                ps[p].posy = ps[p].oposy;
                                ps[p].posz = ps[p].oposz;
                                ps[p].ang = ps[p].oang;
                                updatesector(ps[p].posx,ps[p].posy,&ps[p].cursectnum);
                                //setpal(&ps[p]);

                                k = headspritestat[1];
                                while(k >= 0)
                                {
                                    if(sprite[k].picnum==CAMERA1)
                                        sprite[k].yvel = 0;
                                    k = nextspritestat[k];
                                }
                            }
                        }
                        hittype[j].owner = s->owner;
                    }
                }
            }
            BOLT:
            j = nextj;
        }
    }
}

void moveweapons(void)
{
    short i, j, k, nexti, p, q, tempsect;
    long dax,day,daz, x, l, ll, x1, y1;
    unsigned long qq;
    spritetype *s;

    i = headspritestat[4];
    while(i >= 0)
    {
        nexti = nextspritestat[i];
        s = &sprite[i];

        if(s->sectnum < 0) KILLIT(i);

        hittype[i].bposx = s->x;
        hittype[i].bposy = s->y;
        hittype[i].bposz = s->z;

        switch(s->picnum)
        {
            case RADIUSEXPLOSION:
            case KNEE:
                KILLIT(i);
            case TONGUE:
                T1 = sintable[(T2)&2047]>>9;
                T2 += 32;
                if(T2 > 2047) KILLIT(i);

                if(sprite[s->owner].statnum == MAXSTATUS)
                    if(badguy(&sprite[s->owner]) == 0)
                        KILLIT(i);

                s->ang = sprite[s->owner].ang;
                s->x = sprite[s->owner].x;
                s->y = sprite[s->owner].y;
                if(sprite[s->owner].picnum == APLAYER)
                    s->z = sprite[s->owner].z-(34<<8);
                for(k=0;k<T1;k++)
                {
                    q = EGS(s->sectnum,
                        s->x+((k*sintable[(s->ang+512)&2047])>>9),
                        s->y+((k*sintable[s->ang&2047])>>9),
                        s->z+((k*ksgn(s->zvel))*klabs2(s->zvel/12)),TONGUE,-40+(k<<1),
                        8,8,0,0,0,i,5);
                    sprite[q].cstat = 128;
                    sprite[q].pal = 8;
                }
                q = EGS(s->sectnum,
                    s->x+((k*sintable[(s->ang+512)&2047])>>9),
                    s->y+((k*sintable[s->ang&2047])>>9),
                    s->z+((k*ksgn(s->zvel))*klabs2(s->zvel/12)),INNERJAW,-40,
                    32,32,0,0,0,i,5);
                sprite[q].cstat = 128;
                if( T2 > 512 && T2 < (1024) )
                    sprite[q].picnum = INNERJAW+1;

                goto BOLT;

            /*case FREEZEBLAST:
                if(s->yvel < 1 || s->extra < 2 || (s->xvel|s->zvel) == 0)
                {
                    j = spawn(i,TRANSPORTERSTAR);
                    sprite[j].pal = 1;
                    sprite[j].xrepeat = 32;
                    sprite[j].yrepeat = 32;
                    KILLIT(i);
                }*/
            case SHRINKSPARK:
            case RPG:
            case FIRELASER:
            case SPIT:
            case COOLEXPLOSION1:
            case TILE3841:

                if( s->picnum == COOLEXPLOSION1 )
                    if( Sound[WIERDSHOT_FLY].num == 0 )
                        spritesound(WIERDSHOT_FLY,i);

                p = -1;

                if(s->picnum == RPG && sector[s->sectnum].lotag == 2)
                {
                    k = s->xvel>>1;
                    ll = s->zvel>>1;
                }
                else
                {
                    k = s->xvel;
                    ll = s->zvel;
                }

                dax = s->x; day = s->y; daz = s->z;

                getglobalz(i);
                qq = CLIPMASK1;

                if(s->picnum == RPG)
                {
                    int ang;
                    int zvel;
                    if(hittype[i].picnum != BOSS2 && sector[s->sectnum].lotag != 2 && numplayers == 1)
                    {
                        j = spawn(i,SMALLSMOKE);
                        sprite[j].z += (1<<8);
                    }
                    ang = s->ang;
                    zvel = ll;
                    if (s->xvel < 640)
                    {
                        if (s->xvel != 512)
                        {
                            ang = s->ang + (sintable[(totalclock << 5) & 2047] >> 9);
                            zvel = ll + (sintable[((totalclock << 5) + 512) & 2047] >> 4);
                        }
                        else
                        {
                            ang = s->ang + (sintable[((totalclock << 5) + 512) & 2047] >> 9);
                            zvel = ll + (sintable[(totalclock << 5) & 2047] >> 4);
                        }
                    }
                    j = movesprite(i,
                        (k*(sintable[(ang+512)&2047]))>>14,
                        (k*(sintable[ang&2047]))>>14,zvel,qq);
                }
                else
                    j = movesprite(i,
                        (k*(sintable[(s->ang+512)&2047]))>>14,
                        (k*(sintable[s->ang&2047]))>>14,ll,qq);

                if(s->picnum == RPG && s->yvel >= 0)
                {
                    short ts = s->yvel;
                    if (s->xvel < 640)
                    {
                        int v1;
                        v1 = getangle(sprite[ts].x - s->x, sprite[ts].y - s->y);
                        v1 = getanglediff(s->ang, v1);
                        v1 = max(-20, min(20, v1));
                        s->ang += v1;
                        v1 = sprite[ts].z - s->z - (32 << 8);
                        v1 = max(-1024, min(1024, v1));
                        s->z += v1;
                        
                    }
                    if( FindDistance2D(s->x-sprite[ts].x,s->y-sprite[ts].y) < 256 )
                        j = 49152|ts;
                }

                if(s->sectnum < 0) { KILLIT(i); }

                if( (j&49152) != 49152)
                    //if(s->picnum != FREEZEBLAST)
                {
                    if(s->z < hittype[i].ceilingz)
                    {
                        j = 16384|(s->sectnum);
                        s->zvel = -1;
                    }
                    else
                        if( ( s->z > hittype[i].floorz && sector[s->sectnum].lotag != 1 ) ||
                            ( s->z > hittype[i].floorz+(16<<8) && sector[s->sectnum].lotag == 1 ) )
                    {
                        j = 16384|(s->sectnum);
                        if(sector[s->sectnum].lotag != 1)
                            s->zvel = 1;
                    }
                }

                if (s->picnum == RPG)
                {
                    x = EGS(s->sectnum, s->x, s->y, s->z, TILE3845,
                            0, 64, 32, 0, 0, 0, s->owner, 5);
                    sprite[x].cstat = 128;
                }

                if(s->picnum == FIRELASER)
                {
                    for(k=-3;k<2;k++)
                    {
                        x = EGS(s->sectnum,
                            s->x+((k*sintable[(s->ang+512)&2047])>>9),
                            s->y+((k*sintable[s->ang&2047])>>9),
                            s->z+((k*ksgn(s->zvel))*klabs2(s->zvel/24)),FIRELASER,-40+(k<<2),
                            s->xrepeat,s->yrepeat,0,0,0,s->owner,5);

                        sprite[x].cstat = 128;
                        sprite[x].pal = s->pal;
                    }
                }
                else if(s->picnum == SPIT) if(s->zvel < 6144)
                    s->zvel += gc-112;

                if( j != 0 )
                {
                    if(s->picnum == COOLEXPLOSION1)
                    {
                        if( (j&49152) == 49152 && sprite[j&0xfff].picnum != APLAYER)
                        {
                            if ((j&0xfff) >= MAXSPRITES)
                                gameexit("Sprite overflow caused by LARD\n");
                            goto BOLT;
                        }
                        s->xvel = 0;
                        s->zvel = 0;
                    }

                    if( (j&49152) == 49152 )
                    {
                        j &= 0xfff;
                        if (j >= MAXSPRITES)
                            gameexit("Sprite overflow caused by DOG\n");

                        /*if(s->picnum == FREEZEBLAST && sprite[j].pal == 1 )
                            if( badguy(&sprite[j]) || sprite[j].picnum == APLAYER )
                        {
                            j = spawn(i,TRANSPORTERSTAR);
                            sprite[j].pal = 1;
                            sprite[j].xrepeat = 32;
                            sprite[j].yrepeat = 32;

                            KILLIT(i);
                        }*/

                        checkhitsprite(j,i);

                        if(sprite[j].picnum == APLAYER)
                        {
                            p = sprite[j].yvel;
                            spritesound(PISTOL_BODYHIT,j);

                            if(s->picnum == SPIT)
                            {
                                ps[p].horiz += 32;
                                ps[p].return_to_center = 8;

                                if(ps[p].loogcnt == 0)
                                {
                                    if(Sound[DUKE_LONGTERM_PAIN].num < 1)
                                        spritesound(DUKE_LONGTERM_PAIN,ps[p].i);

                                    j = 3+(TRAND&3);
                                    ps[p].numloogs = j;
                                    ps[p].loogcnt = 24*4;
                                    for(x=0;x < j;x++)
                                    {
                                        ps[p].loogiex[x] = TRAND%xdim;
                                        ps[p].loogiey[x] = TRAND%ydim;
                                    }
                                }
                            }
                        }
                    }
                    else if( (j&49152) == 32768 )
                    {
                        j &= 0x1fff;

                        if(s->picnum != RPG && s->picnum != TILE3841 && s->picnum != SPIT && ( wall[j].overpicnum == MIRROR || wall[j].picnum == MIRROR ) )
                        {
                            k = getangle(
                                    wall[wall[j].point2].x-wall[j].x,
                                    wall[wall[j].point2].y-wall[j].y);
                            s->ang = ((k<<1) - s->ang)&2047;
                            s->owner = i;
                            spawn(i,TRANSPORTERSTAR);
                            goto BOLT;
                        }
                        else
                        {
                            setsprite(i,dax,day,daz);
                            checkhitwall(i,j,s->x,s->y,s->z,s->picnum);

                            /*if(s->picnum == FREEZEBLAST)
                            {
                                if( wall[j].overpicnum != MIRROR && wall[j].picnum != MIRROR )
                                {
                                    s->extra >>= 1;
                                    s->yvel--;
                                }

                                k = getangle(
                                    wall[wall[j].point2].x-wall[j].x,
                                    wall[wall[j].point2].y-wall[j].y);
                                s->ang = ((k<<1) - s->ang)&2047;
                                goto BOLT;
                            }*/
                        }
                    }
                    else if( (j&49152) == 16384)
                    {
                        setsprite(i,dax,day,daz);

                        if(s->zvel < 0)
                        {
                            if( sector[s->sectnum].ceilingstat&1 )
                                if(sector[s->sectnum].ceilingpal == 0)
                                    KILLIT(i);

                            checkhitceiling(s->sectnum);
                        }

                        /*if(s->picnum == FREEZEBLAST)
                        {
                            bounce(i);
                            ssp(i,qq);
                            s->extra >>= 1;
                            if(s->xrepeat > 8)
                                s->xrepeat -= 2;
                            if(s->yrepeat > 8)
                                s->yrepeat -= 2;
                            s->yvel--;
                            goto BOLT;
                        }*/
                    }

                    //if(s->picnum != SPIT)
                    //{
                        if(s->picnum == RPG)
                        {
                            k = spawn(i,EXPLOSION2);
                            sprite[k].x = dax;
                            sprite[k].y = day;
                            sprite[k].z = daz;

                            if( (j&49152) == 16384)
                            {
                                if( s->zvel <= 0)
                                { sprite[k].cstat |= 8; sprite[k].z += (48<<8); }
                            }
                            spritesound(RPG_EXPLODE,i);
                            x = s->extra;
                            hitradius( i,rpgblastradius, x>>2,x>>1,x-(x>>2),x);
                        }
                        if(s->picnum == SHRINKSPARK)
                        {
                            spawn(i,SHRINKEREXPLOSION);
                            spritesound(SHRINKER_HIT,i);
                            hitradius(i,shrinkerblastradius,0,0,0,0);
                        }
                        if(s->picnum == TILE3841)
                        {
                            k = EGS(s->sectnum, s->x, s->y, s->z, FORCERIPPLE, -127, 64, 64,
                                   0, 0, 0, i, 5);
                            sprite[k].cstat = 128;
                            if (s->yvel > 88)
                            {
                                spritesound(SND271, i);
                                for (p = 0; p < numplayers; p++)
                                {
                                    if (ps[p].last_extra > 0 && cansee(s->x, s->y, s->z, s->sectnum,
                                        ps[p].posx, ps[p].posy, ps[p].posz, ps[p].cursectnum))
                                    {
                                        ps[p].pals[0] = 64;
                                        ps[p].pals[1] = 96;
                                        ps[p].pals[2] = 96;
                                        ps[p].pals_time = s->yvel - 32;
                                    }
                                }
                                earthquaketime = 15;
                                addexplosionmodel(s->x >> 1, s->y >> 1, s->z >> 5, 4);
                            }
                            else
                            {
                                spritesound(RPG_EXPLODE, i);
                                addexplosionmodel(s->x >> 1, s->y >> 1, s->z >> 5, 6);
                            }
                            x = s->extra;
                            hitradius(i, s->yvel * 30 + 1780,x>>2,x>>1,x-(x>>2),x);
                        }
                    //}
                    if(s->picnum != COOLEXPLOSION1) KILLIT(i);
                }
                if(s->picnum == COOLEXPLOSION1)
                {
                    s->shade++;
                    if(s->shade >= 40) KILLIT(i);
                }
                else if(s->picnum == RPG && sector[s->sectnum].lotag == 2 && rnd(140))
                    spawn(i,WATERBUBBLE);

                goto BOLT;


            case SHOTSPARK1:
            case SHOTSPARK1+1:
            case SHOTSPARK1+2:
            case SHOTSPARK1+3:
            case SHOTSPARK1+4:
                p = findplayer(s,&x);
                execute(i,p,x);
                goto BOLT;
        }
        BOLT:
        i = nexti;
    }
}

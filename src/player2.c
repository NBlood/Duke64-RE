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

void shoot(short i,short atwith)
{
    short sect, hitsect, hitspr, hitwall, l, sa, p, j, k, atwith2;
    long sx, sy, sz, vel, zvel, hitx, hity, hitz, x, oldzvel, dal;
    unsigned char sizx,sizy;
    spritetype *s;

    s = &sprite[i];
    sect = s->sectnum;
    zvel = 0;

    if( s->picnum == APLAYER )
    {
        p = s->yvel;

        sx = ps[p].posx;
        sy = ps[p].posy;
        sz = ps[p].posz+ps[p].pyoff+(4<<8);
        sa = ps[p].ang;

        ps[p].crack_time = 777;

    }
    else
    {
        p = -1;
        sa = s->ang;
        sx = s->x;
        sy = s->y;
        sz = s->z-((s->yrepeat*gettilesizy(s->picnum))<<1)+(4<<8);
        if(s->picnum != ROTATEGUN)
        {
            sz -= (7<<8);
            if(badguy(s) && PN != COMMANDER)
            {
                sx += (sintable[(sa+1024+96)&2047]>>7);
                sy += (sintable[(sa+512+96)&2047]>>7);
            }
        }
    }

    switch(atwith)
    {
        case BLOODSPLAT1:
        case BLOODSPLAT2:
        case BLOODSPLAT3:
        case BLOODSPLAT4:

            if(p >= 0)
                sa += 64 - (TRAND&127);
            else sa += 1024 + 64 - (TRAND&127);
            zvel = 1024-(TRAND&2047);
        case KNEE:
            if(atwith == KNEE )
            {
                if(p >= 0)
                {
                    zvel = (100-ps[p].horiz-ps[p].horizoff)<<5;
                    sz += (6<<8);
                    sa += 15;
                }
                else
                {
                    j = ps[findplayer(s,&x)].i;
                    zvel = ( (sprite[j].z-sz)<<8 ) / (x+1);
                    sa = getangle(sprite[j].x-sx,sprite[j].y-sy);
                }
            }

//            writestring(sx,sy,sz,sect,sintable[(sa+512)&2047],sintable[sa&2047],zvel<<6);

            hitscan(sx,sy,sz,sect,
                sintable[(sa+512)&2047],
                sintable[sa&2047],zvel<<6,
                &hitsect,&hitwall,&hitspr,&hitx,&hity,&hitz,CLIPMASK1);

            if( atwith == BLOODSPLAT1 ||
                atwith == BLOODSPLAT2 ||
                atwith == BLOODSPLAT3 ||
                atwith == BLOODSPLAT4 )
            {
                if( FindDistance2D(sx-hitx,sy-hity) < 1024 )
                    if( hitwall >= 0 && wall[hitwall].overpicnum != BIGFORCE )
                        if( ( wall[hitwall].nextsector >= 0 && hitsect >= 0 &&
                            sector[wall[hitwall].nextsector].lotag == 0 &&
                                sector[hitsect].lotag == 0 &&
                                    sector[wall[hitwall].nextsector].lotag == 0 &&
                                        (sector[hitsect].floorz-sector[wall[hitwall].nextsector].floorz) > (16<<8) ) ||
                                            ( wall[hitwall].nextsector == -1 && sector[hitsect].lotag == 0 ) )
                                                if( (wall[hitwall].cstat&16) == 0)
                {
                    if(wall[hitwall].nextsector >= 0)
                    {
                        k = headspritesect[wall[hitwall].nextsector];
                        while(k >= 0)
                        {
                            if(sprite[k].statnum == 3 && sprite[k].lotag == 13)
                                return;
                            k = nextspritesect[k];
                        }
                    }

                    if( wall[hitwall].nextwall >= 0 &&
                        wall[wall[hitwall].nextwall].hitag != 0 )
                            return;

                    if(wall[hitwall].hitag == 0)
                    {
                        k = spawn(i,atwith);
                        sprite[k].xvel = -12;
                        sprite[k].ang = getangle(wall[hitwall].x-wall[wall[hitwall].point2].x,
                            wall[hitwall].y-wall[wall[hitwall].point2].y)+512;
                        sprite[k].ang = (sprite[k].ang+1024)&2047;
                        sprite[k].x = hitx;
                        sprite[k].y = hity;
                        sprite[k].z = hitz;
                        sprite[k].x += (sintable[(sprite[k].ang+512)&2047] >> 10);
                        sprite[k].y += (sintable[sprite[k].ang] >> 10);
                        sprite[k].cstat |= (TRAND&4);
                        ssp(k,CLIPMASK0);
                        setsprite(k,sprite[k].x,sprite[k].y,sprite[k].z);
                        if( PN == OOZFILTER || PN == NEWBEAST )
                            sprite[k].pal = 6;
                    }
                }
                return;
            }

            if(hitsect < 0) break;

            if( ( klabs2(sx-hitx)+klabs2(sy-hity) ) < 1024 )
            {
                if(hitwall >= 0 || hitspr >= 0)
                {
                    j = EGS(hitsect,hitx,hity,hitz,KNEE,-15,0,0,sa,32,0,i,4);
                    sprite[j].extra += (TRAND&7);
                    if(p >= 0)
                    {
                        k = spawn(j,SMALLSMOKE);
                        sprite[k].z -= (8<<8);
                        spritesound(KICK_HIT,j);
                    }

                    if ( p >= 0 && ps[p].steroids_amount > 0 && ps[p].steroids_amount < 400 )
                        sprite[j].extra += (max_player_health>>2);

                    if( hitspr >= 0 && sprite[hitspr].picnum != ACCESSSWITCH && sprite[hitspr].picnum != ACCESSSWITCH2 )
                    {
                        checkhitsprite(hitspr,j);
                        if(p >= 0) checkhitswitch(p,hitspr,1);
                    }

                    else if( hitwall >= 0 )
                    {
                        if( wall[hitwall].cstat&2 )
                            if(wall[hitwall].nextsector >= 0)
                                if(hitz >= (sector[wall[hitwall].nextsector].floorz) )
                                    hitwall = wall[hitwall].nextwall;

                        if( hitwall >= 0 && wall[hitwall].picnum != ACCESSSWITCH && wall[hitwall].picnum != ACCESSSWITCH2 )
                        {
                            checkhitwall(j,hitwall,hitx,hity,hitz,atwith);
                            if(p >= 0) checkhitswitch(p,hitwall,0);
                        }
                    }
                }
                else if(p >= 0 && zvel > 0 && sector[hitsect].lotag == 1)
                {
                    j = spawn(ps[p].i,WATERSPLASH2);
                    sprite[j].x = hitx;
                    sprite[j].y = hity;
                    sprite[j].ang = ps[p].ang; // Total tweek
                    sprite[j].xvel = 32;
                    ssp(i,CLIPMASK0);
                    sprite[j].xvel = 0;

                }
            }

            break;

        case SHOTSPARK1:
        case SHOTSPARK1+1:
        case SHOTSPARK1+2:
        case SHOTSPARK1+3:
        case SHOTSPARK1+4:
        case SHOTGUN:
        case CHAINGUN:

            if (atwith == SHOTGUN) atwith = SHOTSPARK1+1;
            if (atwith == CHAINGUN) atwith = SHOTSPARK1+2;

            if( s->extra >= 0 ) s->shade = -96;

            if(p >= 0)
            {
                j = aim( s, AUTO_AIM_ANGLE );
                if(j >= 0)
                {
                    dal = ((sprite[j].xrepeat*gettilesizy(sprite[j].picnum))<<1)+(5<<8);
                    switch(sprite[j].picnum)
                    {
                        case GREENSLIME:
                        case GREENSLIME+1:
                        case GREENSLIME+2:
                        case GREENSLIME+3:
                        case GREENSLIME+4:
                        case GREENSLIME+5:
                        case GREENSLIME+6:
                        case GREENSLIME+7:
                        case ROTATEGUN:
                            dal -= (8<<8);
                            break;
                    }
                    zvel = ( ( sprite[j].z-sz-dal )<<8 ) / ldist(&sprite[ps[p].i], &sprite[j]) ;
                    sa = getangle(sprite[j].x-sx,sprite[j].y-sy);
                }

                if(atwith == SHOTSPARK1)
                {
                    if(j == -1)
                    {
                        sa += 16-(TRAND&31);
                        zvel = (100-ps[p].horiz-ps[p].horizoff)<<5;
                        zvel += 128-(TRAND&255);
                    }
                }
                else
                {
                    if(j == -1) zvel = (100-ps[p].horiz-ps[p].horizoff)<<5;
                    if (atwith != SHOTSPARK1+4)
                    {
                        zvel += 128-(TRAND&255);
                        sa += 16-(TRAND&31);
                    }
                }
                sz -= (2<<8);
            }
            else
            {
                j = findplayer(s,&x);
                sz -= (4<<8);
                zvel = ( (ps[j].posz-sz) <<8 ) / (ldist(&sprite[ps[j].i], s ) );
                if(s->picnum != BOSS1)
                {
                    zvel += 128-(TRAND&255);
                    sa += 32-(TRAND&63);
                }
                else
                {
                    zvel += 128-(TRAND&255);
                    sa = getangle(ps[j].posx-sx,ps[j].posy-sy)+64-(TRAND&127);
                }
            }

            s->cstat &= ~257;
            hitscan(sx,sy,sz,sect,
                sintable[(sa+512)&2047],
                sintable[sa&2047],
                zvel<<6,&hitsect,&hitwall,&hitspr,&hitx,&hity,&hitz,CLIPMASK1);
            s->cstat |= 257;

            if(hitsect < 0) return;

            if( (TRAND&15) == 0 && sector[hitsect].lotag == 2 )
                tracers(hitx,hity,hitz,sx,sy,sz,p);

            if(p >= 0)
            {
                k = EGS(hitsect,hitx,hity,hitz,atwith,-15,8,8,sa,0,0,i,4);
                sprite[k].cstat |= 128;
                sprite[k].extra = script[actorscrptr[atwith]];
                sprite[k].extra += (TRAND%6);

                sprite[k].x += sintable[(sa + 1536) & 2047] >> 10;
                sprite[k].y += sintable[(sa + 1024) & 2047] >> 10;

                if ((TRAND&255) < 128)
                    sprite[k].cstat |= 4;
                if ((TRAND&255) < 128)
                    sprite[k].cstat |= 8;

                if( hitwall == -1 && hitspr == -1)
                {
                    if( zvel < 0 )
                    {
                        switch (sector[hitsect].ceilingpicnum)
                        {
                            case FLOORSLIME:
                            case WATERTILE2:
                                sprite[k].xrepeat = 0;
                                sprite[k].yrepeat = 0;
                                break;
                        }
                        if( sector[hitsect].ceilingstat&1 )
                        {
                            sprite[k].xrepeat = 0;
                            sprite[k].yrepeat = 0;
                            return;
                        }
                        else
                            checkhitceiling(hitsect);
                    }
                    else
                    {
                        switch (sector[hitsect].floorpicnum)
                        {
                            case FLOORSLIME:
                            case WATERTILE2:
                                sprite[k].xrepeat = 0;
                                sprite[k].yrepeat = 0;
                                break;
                        }
                    }
                    //spawn(k,SMALLSMOKE);
                }

                if (atwith == SHOTSPARK1+4)
                {
                    l = spawn(k,EXPLOSION2);
                    sprite[l].ang = (sprite[l].ang + 1024) & 2047;
                    sprite[l].xvel = 348;
                    ssp(l,CLIPMASK0);
                    hitradius(k,2500,sprite[k].extra>>2,sprite[k].extra>>1,sprite[k].extra-(sprite[k].extra>>2),sprite[k].extra);
                    spritesound(PIPEBOMB_EXPLODE,k);
                }

                if(hitspr >= 0 && atwith != SHOTSPARK1+4)
                {
                    checkhitsprite(hitspr,k);
                    /*if( sprite[hitspr].picnum == APLAYER && (ud.coop != 1 || ud.ffire == 1) )
                    {
                        l = spawn(k,JIBS6);
                        sprite[k].xrepeat = sprite[k].yrepeat = 0;
                        sprite[l].z += (4<<8);
                        sprite[l].xvel = 16;
                        sprite[l].xrepeat = sprite[l].yrepeat = 24;
                        sprite[l].ang += 64-(TRAND&127);
                    }
                    else spawn(k,SMALLSMOKE);*/
                    if (badguy(&sprite[hitspr]))
                    {
                    }

                    sprite[k].xrepeat = sprite[k].yrepeat = 0;

                    if(p >= 0 && (
                        sprite[hitspr].picnum == DIPSWITCH ||
                        sprite[hitspr].picnum == DIPSWITCH+1 ||
                        sprite[hitspr].picnum == DIPSWITCH2 ||
                        sprite[hitspr].picnum == DIPSWITCH2+1 ||
                        sprite[hitspr].picnum == DIPSWITCH3 ||
                        sprite[hitspr].picnum == DIPSWITCH3+1 ||
                        sprite[hitspr].picnum == HANDSWITCH ||
                        sprite[hitspr].picnum == HANDSWITCH+1) )
                    {
                        checkhitswitch(p,hitspr,1);
                        return;
                    }
                }
                else if( hitwall >= 0 )
                {
                    //spawn(k,SMALLSMOKE);

                    if( isadoorwall(wall[hitwall].picnum) == 1 )
                        goto SKIPBULLETHOLE;
                    if(p >= 0 && (
                        wall[hitwall].picnum == DIPSWITCH ||
                        wall[hitwall].picnum == DIPSWITCH+1 ||
                        wall[hitwall].picnum == DIPSWITCH2 ||
                        wall[hitwall].picnum == DIPSWITCH2+1 ||
                        wall[hitwall].picnum == DIPSWITCH3 ||
                        wall[hitwall].picnum == DIPSWITCH3+1 ||
                        wall[hitwall].picnum == HANDSWITCH ||
                        wall[hitwall].picnum == HANDSWITCH+1) )
                    {
                        checkhitswitch(p,hitwall,0);
                        return;
                    }

                    if (numplayers != 1)
                        goto SKIPBULLETHOLE;
                    if(wall[hitwall].hitag != 0 || ( wall[hitwall].nextwall >= 0 && wall[wall[hitwall].nextwall].hitag != 0 ) )
                        goto SKIPBULLETHOLE;

                    if( hitsect >= 0 && sector[hitsect].lotag == 0 )
                        if( wall[hitwall].overpicnum != BIGFORCE )
                            if( (wall[hitwall].nextsector >= 0 && sector[wall[hitwall].nextsector].lotag == 0 ) ||
                                ( wall[hitwall].nextsector == -1 && sector[hitsect].lotag == 0 ) )
                                    if( (wall[hitwall].cstat&16) == 0)
                    {
                        if(wall[hitwall].nextsector >= 0)
                        {
                            l = headspritesect[wall[hitwall].nextsector];
                            while(l >= 0)
                            {
                                if(sprite[l].statnum == 3 && sprite[l].lotag == 13)
                                    goto SKIPBULLETHOLE;
                                l = nextspritesect[l];
                            }
                        }

                        l = headspritestat[5];
                        while(l >= 0)
                        {
                            if(sprite[l].picnum == BULLETHOLE)
                            {
                                if (atwith == SHOTSPARK1+3 || atwith == SHOTSPARK1+4)
                                {
                                    if(dist(&sprite[l],&sprite[k]) < (36+(TRAND&7)) )
                                        goto SKIPBULLETHOLE;
                                }
                                else
                                {
                                    if(dist(&sprite[l],&sprite[k]) < (12+(TRAND&7)) )
                                        goto SKIPBULLETHOLE;
                                }
                            }
                            l = nextspritestat[l];
                        }
                        l = spawn(k,BULLETHOLE);
                        sprite[l].xvel = 0;
                        sprite[l].ang = getangle(wall[hitwall].x-wall[wall[hitwall].point2].x,
                            wall[hitwall].y-wall[wall[hitwall].point2].y)+512;
                        sprite[l].ang = (sprite[l].ang+1024)&2047;
                        if (atwith == SHOTSPARK1+3 || atwith == SHOTSPARK1+4)
                        {
                            sprite[l].xrepeat *= 3;
                            sprite[l].yrepeat *= 3;
                        }
                        ssp(l,CLIPMASK0);
                    }

                    SKIPBULLETHOLE:

                    if( wall[hitwall].cstat&2 )
                        if(wall[hitwall].nextsector >= 0)
                            if(hitz >= (sector[wall[hitwall].nextsector].floorz) )
                                hitwall = wall[hitwall].nextwall;

                    checkhitwall(k,hitwall,hitx,hity,hitz,atwith);
                }
            }
            else
            {
                k = EGS(hitsect,hitx,hity,hitz,atwith,-15,16,16,sa,0,0,i,4);
                sprite[k].cstat |= 128;
                sprite[k].extra = script[actorscrptr[atwith]];

                if( hitspr >= 0 )
                {
                    checkhitsprite(hitspr,k);
                    sprite[k].xrepeat = sprite[k].yrepeat = 0;
                }
                else if( hitwall >= 0 )
                    checkhitwall(k,hitwall,hitx,hity,hitz,atwith);
            }

            if(hitwall >= 0 && (TRAND&255) < 32 && atwith != 2599 && atwith != 2596)
            {
                x = SND272+(TRAND%5);
                xyzsound(x,k,hitx,hity,hitz);
            }

            return;

        case FIRELASER:
        case SPIT:
        case COOLEXPLOSION1:

            if( s->extra >= 0 ) s->shade = -96;

            //scount = 1;
            if(atwith == SPIT) vel = 292;
            else
            {
                if(atwith == COOLEXPLOSION1)
                {
                    if(s->picnum == BOSS2)
                    {
                        vel = 644;
                        sz -= 12288.0;
                        sx += (sintable[(sa+512)&2047] * 1016) >> 14;
                        sy += (sintable[sa&2047] * 1016) >> 14;
                    }
                    else
                    {
                        vel = 348;
                        sz -= (4<<7);
                    }
                }
                else
                {
                    vel = 840;
                    sz -= (4<<7);
                }
            }

            if(p >= 0)
            {
                j = aim( s, AUTO_AIM_ANGLE );

                if(j >= 0)
                {
                    dal = ((sprite[j].xrepeat*gettilesizy(sprite[j].picnum))<<1)-(12<<8);
                    zvel = ((sprite[j].z-sz-dal)*vel ) / ldist(&sprite[ps[p].i], &sprite[j]) ;
                    sa = getangle(sprite[j].x-sx,sprite[j].y-sy);
                }
                else
                    zvel = (100-ps[p].horiz-ps[p].horizoff)*98;
            }
            else
            {
                j = findplayer(s,&x);
//                sa = getangle(ps[j].oposx-sx,ps[j].oposy-sy);
                sa += 16-(TRAND&31);
                zvel = ( ( (ps[j].oposz - sz + (3<<8) ) )*vel ) / FindDistance2D(sprite[ps[j].i].x - sx, sprite[ps[j].i].y - sy);
            }

            oldzvel = zvel;

            if(atwith == SPIT) { sizx = 18;sizy = 18,sz -= (10<<8); }
            else
            {
                if( atwith == FIRELASER )
                {
                    if(p >= 0)
                    {
                        
                        sizx = 34;
                        sizy = 34;
                    }
                    else
                    {
                        sizx = 18;
                        sizy = 18;
                    }
                }
                else
                {
                    sizx = 18;
                    sizy = 18;
                }
            }

            if(p >= 0) sizx = 7,sizy = 7;

            //while(scount > 0)
            //{
                j = EGS(sect,sx,sy,sz,atwith,-127,sizx,sizy,sa,vel,zvel,i,4);
                sprite[j].extra += (TRAND&7);

                if(atwith == COOLEXPLOSION1)
                {
                    sprite[j].shade = 0;
                    if(PN == BOSS2)
                    {
                        /*l = sprite[j].xvel;
                        sprite[j].xvel = 1024;
                        ssp(j,CLIPMASK0);
                        sprite[j].xvel = l;*/
                        sprite[j].ang += 128-(TRAND&255);
                    }
                }

                sprite[j].cstat = 128;
                sprite[j].clipdist = 4;

                /*sa = s->ang+32-(TRAND&63);
                zvel = oldzvel+512-(TRAND&1023);*/

                //scount--;
            //}

            return;

        case TILE3841:
            sz += (3<<8);
        case RPG:
        case RPG+1:

            if( s->extra >= 0 ) s->shade = -96;

            atwith2 = atwith;

            //scount = 1;
            vel = 644;

            j = -1;

            if(p >= 0)
            {
                if (atwith == RPG+1)
                {
                    char t = ps[p].autoaim;
                    ps[p].autoaim = 1;
                    j = dal = aim( s, 192 );
                    ps[p].autoaim = t;
                    vel = 512;
                    atwith2 = RPG;
                }
                else
                    j = dal = aim( s, 48 );
                if(dal >= 0)
                {
                    dal = ((sprite[j].xrepeat*gettilesizy(sprite[j].picnum))<<1)+(8<<8);
                    if (atwith != RPG+1)
                    {
                        zvel = ( (sprite[j].z-sz-dal)*vel ) / ldist(&sprite[ps[p].i], &sprite[j]);
                        if( sprite[j].picnum != RECON )
                            sa = getangle(sprite[j].x-sx,sprite[j].y-sy);
                    }
                    else zvel = (100-ps[p].horiz-ps[p].horizoff)*81;
                }
                else zvel = (100-ps[p].horiz-ps[p].horizoff)*81;
                if(atwith2 == RPG)
                    spritesound(RPG_SHOOT,i);

            }
            else
            {
                j = findplayer(s,&x);
                sa = dal = getangle(ps[j].oposx-sx,ps[j].oposy-sy);
                if(PN == BOSS3)
                {
                    if (sprite[i].pal != 0)
                        sz -= (16<<8);
                    else
                        sz -= (32<<8);
                }
                else if(PN == BOSS2)
                {
                    vel = 772;
                    sx += ((sintable[dal & 2047] * 4) / 56) / 1.3;
                    sy += ((sintable[(dal+1536) & 2047] * 4) / 56) / 1.3;
                    sx += ((sintable[(dal+512) & 2047] * 2048) >> 14) / 1.3;
                    sy += ((sintable[dal & 2047] * 2048) >> 14) / 1.3;
                }

                zvel = ( (ps[j].oposz-sz)*vel) / FindDistance2D(sprite[ps[j].i].x - sx,sprite[ps[j].i].y - sy);

                if( badguy(s) && (s->hitag&face_player_smart) )
                    sa = s->ang+(TRAND&31)-16;
            }

            if( p >= 0 && j >= 0)
               l = j;
            else l = -1;

            j = EGS(sect,
                sx+(sintable[(348+sa+512)&2047]/448),
                sy+(sintable[(sa+348)&2047]/448),
                sz-(1<<8),atwith2,0,14,14,sa,vel,zvel,i,4);

            sprite[j].extra += (TRAND&7);
            if(atwith2 == RPG)
                sprite[j].yvel = l;
            if (atwith == RPG+1)
            {
                if ((TRAND&63) < 32)
                    sprite[j].xvel = 513;
            }
            if (atwith == TILE3841)
            {
                sprite[j].zvel -= (2<<4);
                if (p >= 0)
                    sprite[j].yvel = ps[p].hbomb_hold_delay;
                else
                    sprite[j].yvel = 66;
                sprite[j].extra += sprite[j].yvel * sprite[j].yvel / 10;
            }

            if(p == -1)
            {
                if(PN == BOSS3)
                {
                    short dx, dy;
                    if(TRAND&1)
                    {
                        dx = -sintable[sa&2047]>>6;
                        dy = -sintable[(sa+1024+512)&2047]>>6;
                        sprite[j].ang -= 8;
                    }
                    else
                    {
                        dx = sintable[sa&2047]>>6;
                        dy = sintable[(sa+1024+512)&2047]>>6;
                        sprite[j].ang += 4;
                    }
                    if (sprite[i].pal != 0)
                    {
                        dx /= 2;
                        dy /= 2;
                    }
                    sprite[j].x += dx;
                    sprite[j].y += dy;
                    sprite[j].xrepeat = 21;
                    sprite[j].yrepeat = 21;
                }
                else if(PN == BOSS2)
                {
                    sprite[j].ang -= 8+(TRAND&255)-128;
                    sprite[j].xrepeat = 24;
                    sprite[j].yrepeat = 24;
                }
                else if(atwith != TILE3841)
                {
                    sprite[j].xrepeat = 30;
                    sprite[j].yrepeat = 30;
                    sprite[j].extra >>= 2;
                }
            }

            sprite[j].cstat = 128;
            if(atwith2 == RPG)
                sprite[j].clipdist = 4;
            else
                sprite[j].clipdist = 80;

            break;

        case HANDHOLDINGLASER:

            if(p >= 0)
                zvel = (100-ps[p].horiz-ps[p].horizoff)*32;
            else zvel = 0;

            hitscan(sx,sy,sz-ps[p].pyoff,sect,
                sintable[(sa+512)&2047],
                sintable[sa&2047],
                zvel<<6,&hitsect,&hitwall,&hitspr,&hitx,&hity,&hitz,CLIPMASK1);

            j = 0;
            if(hitspr >= 0) break;

            if(hitwall >= 0 && hitsect >= 0)
                if( ((hitx-sx)*(hitx-sx)+(hity-sy)*(hity-sy)) < (290*290) )
            {
                if( wall[hitwall].nextsector >= 0)
                {
                    if( sector[wall[hitwall].nextsector].lotag <= 2 && sector[hitsect].lotag <= 2 )
                        j = 1;
                }
                else if( sector[hitsect].lotag <= 2 )
                    j = 1;
            }

            if(j == 1)
            {
                k = EGS(hitsect,hitx,hity,hitz,TRIPBOMB,-16,4,5,sa,0,0,i,6);

                sprite[k].hitag = k;
                spritesound(LASERTRIP_ONWALL,k);
                sprite[k].xvel = -20;
                ssp(k,CLIPMASK0);
                sprite[k].cstat = 16;
                hittype[k].temp_data[5] = sprite[k].ang = getangle(wall[hitwall].x-wall[wall[hitwall].point2].x,wall[hitwall].y-wall[wall[hitwall].point2].y)-512;

                if(p >= 0)
                    ps[p].ammo_amount[TRIPBOMB_WEAPON]--;

            }
            return;

        case BOUNCEMINE:
        case MORTER:

            if( s->extra >= 0 ) s->shade = -96;

            j = ps[findplayer(s,&x)].i;
            x = ldist(&sprite[j],s);

            zvel = -x>>1;

            if(zvel < -4096)
                zvel = -2048;
            vel = x>>4;

            EGS(sect,
                sx+(sintable[(512+sa+512)&2047]>>8),
                sy+(sintable[(sa+512)&2047]>>8),
                sz+(6<<8),atwith,-64,32,32,sa,vel,zvel,i,1);
            break;

        case GROWSPARK:

            if( s->extra >= 0 ) s->shade = -96;

            if(p >= 0)
            {
                j = aim( s, AUTO_AIM_ANGLE );
                if(j >= 0)
                {
                    dal = ((sprite[j].xrepeat*gettilesizy(sprite[j].picnum))<<1)+(5<<8);
                    switch(sprite[j].picnum)
                    {
                        case GREENSLIME:
                        case GREENSLIME+1:
                        case GREENSLIME+2:
                        case GREENSLIME+3:
                        case GREENSLIME+4:
                        case GREENSLIME+5:
                        case GREENSLIME+6:
                        case GREENSLIME+7:
                        case ROTATEGUN:
                            dal -= (8<<8);
                            break;
                    }
                    zvel = ( ( sprite[j].z-sz-dal )<<8 ) / (ldist(&sprite[ps[p].i], &sprite[j]) );
                    sa = getangle(sprite[j].x-sx,sprite[j].y-sy);
                }
                else
                {
                    sa += 16-(TRAND&31);
                    zvel = (100-ps[p].horiz-ps[p].horizoff)<<5;
                    zvel += 128-(TRAND&255);
                }

                sz -= (2<<8);
            }
            else
            {
                j = findplayer(s,&x);
                sz -= (4<<8);
                zvel = ( (ps[j].posz-sz) <<8 ) / (ldist(&sprite[ps[j].i], s ) );
                zvel += 128-(TRAND&255);
                sa += 32-(TRAND&63);
            }

            k = 0;

//            RESHOOTGROW:

            s->cstat &= ~257;
            hitscan(sx,sy,sz,sect,
                sintable[(sa+512)&2047],
                sintable[sa&2047],
                zvel<<6,&hitsect,&hitwall,&hitspr,&hitx,&hity,&hitz,CLIPMASK1);

            s->cstat |= 257;

            j = EGS(sect,hitx,hity,hitz,GROWSPARK,-16,28,28,sa,0,0,i,1);

            sprite[j].pal = 2;
            sprite[j].cstat |= 130;
            sprite[j].xrepeat = sprite[j].yrepeat = 1;

            if( hitwall == -1 && hitspr == -1 && hitsect >= 0)
            {
                if( zvel < 0 && (sector[hitsect].ceilingstat&1) == 0)
                    checkhitceiling(hitsect);
            }
            else if(hitspr >= 0) checkhitsprite(hitspr,j);
            else if(hitwall >= 0 && wall[hitwall].picnum != ACCESSSWITCH && wall[hitwall].picnum != ACCESSSWITCH2 )
            {
            /*    if(wall[hitwall].overpicnum == MIRROR && k == 0)
                {
                    l = getangle(
                        wall[wall[hitwall].point2].x-wall[hitwall].x,
                        wall[wall[hitwall].point2].y-wall[hitwall].y);

                    sx = hitx;
                    sy = hity;
                    sz = hitz;
                    sect = hitsect;
                    sa = ((l<<1) - sa)&2047;
                    sx += sintable[(sa+512)&2047]>>12;
                    sy += sintable[sa&2047]>>12;

                    k++;
                    goto RESHOOTGROW;
                }
                else */
                    checkhitwall(j,hitwall,hitx,hity,hitz,atwith);
            }

            sprite[j].ang = (sprite[j].ang + 1024) & 2047;
            sprite[j].xvel = 348;
            ssp(j,CLIPMASK0);

            break;
        case SHRINKER:
            if( s->extra >= 0 ) s->shade = -96;
            if(p >= 0)
            {
                j = aim( s, AUTO_AIM_ANGLE );
                if(j >= 0)
                {
                    dal = ((sprite[j].xrepeat*gettilesizy(sprite[j].picnum))<<1);
                    zvel = ( (sprite[j].z-sz-dal-(4<<8))*768) / (ldist( &sprite[ps[p].i], &sprite[j]));
                    sa = getangle(sprite[j].x-sx,sprite[j].y-sy);
                }
                else zvel = (100-ps[p].horiz-ps[p].horizoff)*98;
            }
            else if(s->statnum != 3)
            {
                j = findplayer(s,&x);
                l = ldist(&sprite[ps[j].i],s);
                zvel = ( (ps[j].oposz-sz)*512) / l ;
            }
            else zvel = 0;

            j = EGS(sect,
                sx+(sintable[(512+sa+512)&2047]>>12),
                sy+(sintable[(sa+512)&2047]>>12),
                sz+(2<<8),SHRINKSPARK,-16,28,28,sa,768,zvel,i,4);

            sprite[j].cstat = 128;
            sprite[j].clipdist = 32;


            return;

        case TILE3634:
        {
            viewhorizang = getanglef(ps[p].horiz + ps[p].horizoff - 100, 128.f);
            viewhorizang *= -57.295776f;
            vel = cosf((viewhorizang * 3.1415927f) / 180.0f) * 768.0f;
            zvel = sinf((viewhorizang * 3.1415927f) / 180.0f) * 768.0f;
            if (sector[sect].lotag == 2)
            {
                vel /= 2;
                zvel /= 2;
            }

            j = EGS(sect,
                sx+(sintable[(512+sa+348)&2047]/448),
                sy+(sintable[(sa+348)&2047]/448),
                sz-(1<<8),atwith,0,18,18,sa,vel,zvel<<4,i,1);
            sprite[j].yvel = 0;
            break;
        }
    }
    return;
}

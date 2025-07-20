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

extern char numenvsnds, actor_tog;

void makeitfall_bomb(short i)
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

    if (s->picnum == HEAVYHBOMB || s->picnum == TILE3634)
        c *= 1.5;

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
    }
    if( s->z + s->zvel >= hittype[i].floorz-(FOURSLEIGHT) )
    {
        s->z = hittype[i].floorz - FOURSLEIGHT;
        s->zvel = 0;
    }
}

int manhdist(short s1, short s2)
{
	int dx, dy, dz;
	dy = klabs2(sprite[s1].y - sprite[s2].y);
	dx = klabs2(sprite[s1].x - sprite[s2].x);
	dz = klabs2(sprite[s1].z - sprite[s2].z);

	return (dz >> 4) + dx + dy;
}

void moveactors(void)
{
    long x, m, l, *t;
    int zvel;
    short a, i, j, nexti, nextj, sect, p;
    spritetype *s;
    unsigned short k;

    i = headspritestat[1];
    while(i >= 0)
    {
        nexti = nextspritestat[i];

        s = &sprite[i];

        sect = s->sectnum;

        if( s->xrepeat == 0 || sect < 0 || sect >= MAXSECTORS)
            KILLIT(i);

        t = &hittype[i].temp_data[0];

        hittype[i].bposx = s->x;
        hittype[i].bposy = s->y;
        hittype[i].bposz = s->z;


        switch(s->picnum)
        {
            case DUCK:
            case TARGET:
                if(s->cstat&32)
                {
                    t[0]++;
                    if(t[0] > 60)
                    {
                        t[0] = 0;
                        s->cstat = 128+257+16;
                        s->extra = 1;
                    }
                }
                else
                {
                    j = ifhitbyweapon(i);
                    if( j >= 0 )
                    {
                        s->cstat = 32+128;
                        k = 1;

                        j = headspritestat[1];
                        while(j >= 0)
                        {
                            if( sprite[j].lotag == s->lotag &&
                                sprite[j].picnum == s->picnum )
                            {
                                if( ( sprite[j].hitag && !(sprite[j].cstat&32) ) ||
                                    ( !sprite[j].hitag && (sprite[j].cstat&32) )
                                  )
                                {
                                    k = 0;
                                    break;
                                }
                            }

                            j = nextspritestat[j];
                        }

                        if(k == 1)
                        {
                            operateactivators(s->lotag,-1);
                            operateforcefields(i,s->lotag);
                            operatemasterswitches(s->lotag);
                        }
                    }
                }
                goto BOLT;

            case RESPAWNMARKERRED:
            case RESPAWNMARKERYELLOW:
            case RESPAWNMARKERGREEN:
                T1++;
                if(T1 > respawnitemtime)
                {
                    KILLIT(i);
                }
                if( T1 >= (respawnitemtime>>1) && T1 < ((respawnitemtime>>1)+(respawnitemtime>>2)) )
                    PN = RESPAWNMARKERYELLOW;
                else if( T1 > ((respawnitemtime>>1)+(respawnitemtime>>2)) )
                    PN = RESPAWNMARKERGREEN;
                makeitfall(i);
                break;

            case HELECOPT:
            case DUKECAR:

                //s->z += s->zvel;
                t[0]++;

                if(t[0] == 4) spritesound(WAR_AMBIENCE2,i);

                if( t[0] > (26*8) )
                {
                    sound(RPG_EXPLODE);
                    for(j=0;j<32;j++) RANDOMSCRAP;
                    earthquaketime = 16;
                    spawn(i,EXPLOSION2);
                    addexplosionmodel(s->x >> 1, s->y >> 1, s->z >> 5, 7);
                    KILLIT(i);
                }
                else if((t[0]&3) == 0)
                    spawn(i,EXPLOSION2);
                ssp(i,CLIPMASK0);
                break;
            case RAT:
                makeitfall(i);
                IFMOVING
                {
                    if( (TRAND&255) < 3 ) spritesound(RATTY,i);
                    s->ang += (TRAND&31)-15+(sintable[(t[0]<<8)&2047]>>11);
                }
                else
                {
                    T1++;
                    if(T1 > 1) { KILLIT(i); }
                    else s->ang = (TRAND&2047);
                }
                if(s->xvel < 128)
                    s->xvel+=2;
                s->ang += (TRAND&3)-6;
                break;
            case QUEBALL:
            case STRIPEBALL:
                if(s->xvel)
                {
                    j = headspritestat[0];
                    while(j >= 0)
                    {
                        nextj = nextspritestat[j];
                        if( sprite[j].picnum == POCKET && ldist(&sprite[j],s) < 52 ) KILLIT(i);
                        j = nextj;
                    }

                    j = clipmove(&s->x,&s->y,&s->z,&s->sectnum,
                        (((s->xvel*(sintable[(s->ang+512)&2047]))>>14)*TICSPERFRAME)<<11,
                        (((s->xvel*(sintable[s->ang&2047]))>>14)*TICSPERFRAME)<<11,
                        24L,(4<<8),(4<<8),CLIPMASK1);

                    if(j&49152)
                    {
                        if( (j&49152) == 32768 )
                        {
                            j &= 0x1fff;
                            k = getangle(
                                wall[wall[j].point2].x-wall[j].x,
                                wall[wall[j].point2].y-wall[j].y);
                            s->ang = ((k<<1) - s->ang)&2047;
                        }
                        else if( (j&49152) == 49152 )
                        {
                            j &= 0xfff;
                            if (j >= MAXSPRITES)
                                gameexit("Sprite overflow caused by TOAD\n");
                            checkhitsprite(i,j);
                        }
                    }
                    s->xvel --;
                    if(s->xvel < 0) s->xvel = 0;
                    if( s->picnum == STRIPEBALL )
                    {
                        s->cstat = 257;
                        s->cstat |= 4&s->xvel;
                        s->cstat |= 8&s->xvel;
                    }
                }
                else
                {
                    p = findplayer(s,&x);

                    if( x < 1596)
                    {

//                        if(s->pal == 12)
                        {
                            j = getincangle(ps[p].ang,getangle(s->x-ps[p].posx,s->y-ps[p].posy));
                            if( j > -64 && j < 64 && (sync[p].bits&(1<<29)) )
                                if(ps[p].toggle_key_flag == 1)
                            {
                                a = headspritestat[1];
                                while(a >= 0)
                                {
                                    if(sprite[a].picnum == QUEBALL || sprite[a].picnum == STRIPEBALL)
                                    {
                                        j = getincangle(ps[p].ang,getangle(sprite[a].x-ps[p].posx,sprite[a].y-ps[p].posy));
                                        if( j > -64 && j < 64 )
                                        {
                                            findplayer(&sprite[a],&l);
                                            if(x > l) break;
                                        }
                                    }
                                    a = nextspritestat[a];
                                }
                                if(a == -1)
                                {
                                    if(s->pal == 12)
                                        s->xvel = 164;
                                    else s->xvel = 140;
                                    s->ang = ps[p].ang;
                                    ps[p].toggle_key_flag = 2;
                                }
                            }
                        }
                    }
                    if( x < 512 && s->sectnum == ps[p].cursectnum )
                    {
                        s->ang = getangle(s->x-ps[p].posx,s->y-ps[p].posy);
                        s->xvel = 48;
                    }
                }

                break;
            case FORCESPHERE:

                if(s->yvel == 0)
                {
                    s->yvel = 1;

                    for(l=512;l<(2048-512);l+= 128)
                        for(j=0;j<2048;j += 128)
                    {
                        k = spawn(i,FORCESPHERE);
                        sprite[k].cstat = 257+128;
                        sprite[k].clipdist = 64;
                        sprite[k].ang = j;
                        sprite[k].zvel = sintable[l&2047]>>5;
                        sprite[k].xvel = sintable[(l+512)&2047]>>9;
                        sprite[k].owner = i;
                    }
                }

                if(t[3] > 0)
                {
                    if(s->zvel < 6144)
                        s->zvel += 192;
                    s->z += s->zvel;
                    if(s->z > sector[sect].floorz)
                        s->z = sector[sect].floorz;
                    t[3]--;
                    if(t[3] == 0)
                        KILLIT(i);
                }
                else if(t[2] > 10)
                {
                    j = headspritestat[5];
                    while(j >= 0)
                    {
                        if(sprite[j].owner == i && sprite[j].picnum == FORCESPHERE)
                            hittype[j].temp_data[1] = 1+(TRAND&63);
                        j = nextspritestat[j];
                    }
                    t[3] = 64;
                }

                goto BOLT;

            case RECON:

                getglobalz(i);

                if (sector[s->sectnum].ceilingstat&1)
                   s->shade += (sector[s->sectnum].ceilingshade-s->shade)>>1;
                else s->shade += (sector[s->sectnum].floorshade-s->shade)>>1;

                if( s->z < sector[sect].ceilingz+(32<<8) )
                    s->z = sector[sect].ceilingz+(32<<8);

                if( ud.multimode < 2 )
                {
                    if( actor_tog == 1)
                    {
                        s->cstat = (short)32768;
                        goto BOLT;
                    }
                    else if(actor_tog == 2) s->cstat = 257;
                }
                IFHIT
                {
                    if( s->extra < 0 && t[0] != -1 )
                    {
                        t[0] = -1;
                        s->extra = 0;
                    }
                    spritesound(RECO_PAIN,i);
                    RANDOMSCRAP;
                }

                p = findplayer(s,&x);

                if(t[0] == -1)
                {
                    s->z += 1024;
                    t[2]++;
                    if( (t[2]&3) == 0) spawn(i,EXPLOSION2);
                    getglobalz(i);
                    s->ang += 96;
                    s->xvel = 128;
                    j = ssp(i,CLIPMASK0);
                    if(j != 1 || s->z > hittype[i].floorz)
                    {
                        for(l=0;l<16;l++)
                            RANDOMSCRAP;
                        spritesound(LASERTRIP_EXPLODE,i);
                        addexplosionmodel2(s->x >> 1, s->y >> 1, s->z >> 5, 1);
                        spawn(i,PIGCOP);
                        ps[p].actors_killed++;
                        KILLIT(i);
                    }
                    goto BOLT;
                }
                else
                {
                    if( s->z > hittype[i].floorz-(48<<8) )
                        s->z = hittype[i].floorz-(48<<8);
                }

                j = s->owner;

                // 3 = findplayerz, 4 = shoot

                if( t[0] >= 4 )
                {
                    t[2]++;
                    if( (t[2]&15) == 0 )
                    {
                        a = s->ang;
                        s->ang = hittype[i].tempang;
                        spritesound(RECO_ATTACK,i);
                        shoot(i,FIRELASER);
                        s->ang = a;
                    }
                    if( t[2] > (26*3) || !cansee(s->x,s->y,s->z-(16<<8),s->sectnum, ps[p].posx,ps[p].posy,ps[p].posz,ps[p].cursectnum ) )
                    {
                        t[0] = 0;
                        t[2] = 0;
                    }
                    else hittype[i].tempang +=
                        getincangle(hittype[i].tempang,getangle(ps[p].posx-s->x,ps[p].posy-s->y))/3;
                }
                else if(t[0] == 2 || t[0] == 3)
                {
                    t[3] = 0;
                    if(s->xvel > 0) s->xvel -= 16;
                    else s->xvel = 0;

                    if(t[0] == 2)
                    {
                        l = ps[p].posz-s->z;
                        if( klabs2(l) < (48<<8) ) t[0] = 3;
                        else s->z += ksgn(ps[p].posz-s->z)<<10;
                    }
                    else
                    {
                        t[2]++;
                        if( t[2] > (26*3) || !cansee(s->x,s->y,s->z-(16<<8),s->sectnum, ps[p].posx,ps[p].posy,ps[p].posz,ps[p].cursectnum ) )
                        {
                            t[0] = 1;
                            t[2] = 0;
                        }
                        else if( (t[2]&15) == 0 )
                        {
                            spritesound(RECO_ATTACK,i);
                            shoot(i,FIRELASER);
                        }
                    }
                    s->ang += getincangle(s->ang,getangle(ps[p].posx-s->x,ps[p].posy-s->y))>>2;
                }

                if( t[0] != 2 && t[0] != 3 )
                {
                    l = ldist(&sprite[j],s);
                    if(l <= 1524)
                    {
                        a = s->ang;
                        s->xvel >>= 1;
                    }
                    else a = getangle(sprite[j].x-s->x,sprite[j].y-s->y);

                    if(t[0] == 1 || t[0] == 4) // Found a locator and going with it
                    {
                        l = dist(&sprite[j],s);

                        if( l <= 1524 ) { if(t[0] == 1) t[0] = 0; else t[0] = 5; }
                        else
                        {
                            // Control speed here
                            if(l > 1524) { if( s->xvel < 256 ) s->xvel += 32; }
                            else
                            {
                                if(s->xvel > 0) s->xvel -= 16;
                                else s->xvel = 0;
                            }
                        }

                        if(t[0] < 2) t[2]++;

                        if( x < 6144 && t[0] < 2 && t[2] > (26*4) )
                        {
                            t[0] = 2+(TRAND&2);
                            t[2] = 0;
                            hittype[i].tempang = s->ang;
                        }
                    }

                    if(t[0] == 0 || t[0] == 5)
                    {
                        if(t[0] == 0)
                            t[0] = 1;
                        else t[0] = 4;
                        j = s->owner = LocateTheLocator(s->hitag,-1);
                        if(j == -1)
                        {
                            s->hitag = j = hittype[i].temp_data[5];
                            s->owner = LocateTheLocator(j,-1);
                            j = s->owner;
                            if(j == -1) KILLIT(i);
                        }
                        else s->hitag++;
                    }

                    t[3] = getincangle(s->ang,a);
                    s->ang += t[3]>>3;

                    if(s->z < sprite[j].z)
                        s->z += 1024;
                    else s->z -= 1024;
                }

                if(Sound[RECO_ROAM].num == 0 )
                    spritesound(RECO_ROAM,i);

                ssp(i,CLIPMASK0);

                goto BOLT;

            case OOZ:
            case OOZ2:

                getglobalz(i);

                j = (hittype[i].floorz-hittype[i].ceilingz)>>9;
                if(j > 255) j = 255;

                x = 25-(j>>1);
                if(x < 8) x = 8;
                else if(x > 48) x = 48;

                s->yrepeat = j;
                s->xrepeat = x;
                s->z = hittype[i].floorz;

                goto BOLT;

            case GREENSLIME:
            case GREENSLIME+1:
            case GREENSLIME+2:
            case GREENSLIME+3:
            case GREENSLIME+4:
            case GREENSLIME+5:
            case GREENSLIME+6:
            case GREENSLIME+7:

// #ifndef VOLUMEONE
                if( ud.multimode < 2 )
                {
                    if( actor_tog == 1)
                    {
                        s->cstat = (short)32768;
                        goto BOLT;
                    }
                    else if(actor_tog == 2) s->cstat = 257;
                }
// #endif

                t[1]+=128;

                if(sector[sect].floorstat&1)
                    KILLIT(i);

                p = findplayer(s,&x);

                if(x > 20480)
                {
                    hittype[i].timetosleep++;
                    if( hittype[i].timetosleep > SLEEPTIME )
                    {
                        hittype[i].timetosleep = 0;
                        changespritestat(i,2);
                        goto BOLT;
                    }
                }

                /*if(t[0] == -5) // FROZEN
                {
                    t[3]++;
                    if(t[3] > 280)
                    {
                        s->pal = 0;
                        t[0] = 0;
                        goto BOLT;
                    }
                    makeitfall(i);
                    s->cstat = 257;
                    s->picnum = GREENSLIME+2;
                    s->extra = 1;
                    s->pal = 1;
                    IFHIT
                    {
                        if(j == FREEZEBLAST) goto BOLT;
                        for(j=16; j >= 0 ;j--)
                        {
                            k = EGS(SECT,SX,SY,SZ,GLASSPIECES+(j%3),-32,36,36,TRAND&2047,32+(TRAND&63),1024-(TRAND&1023),i,5);
                            sprite[k].pal = 1;
                        }
                        spritesound(GLASS_BREAKING,i);
                        KILLIT(i);
                    }
                    else if(x < 1024 && ps[p].quick_kick == 0)
                    {
                        j = getincangle(ps[p].ang,getangle(SX-ps[p].posx,SY-ps[p].posy));
                        if( j > -128 && j < 128 )
                            ps[p].quick_kick = 14;
                    }

                    goto BOLT;
                }*/

                if(x < 1596)
                    s->cstat = 0;
                else s->cstat = 257;

                if(t[0] == -4) //On the player
                {
                    if( sprite[ps[p].i].extra < 1 )
                    {
                        t[0] = 0;
                        goto BOLT;
                    }

                    setsprite(i,s->x,s->y,s->z);

                    s->ang = ps[p].ang;

                    if( ( (sync[p].bits&4) || (ps[p].quick_kick > 0) ) && sprite[ps[p].i].extra > 0 )
                        if( ps[p].quick_kick > 0 || ( ps[p].curr_weapon != HANDREMOTE_WEAPON && ps[p].curr_weapon != HANDBOMB_WEAPON && ps[p].curr_weapon != TRIPBOMB_WEAPON && ps[p].ammo_amount[ps[p].curr_weapon] >= 0) )
                    {
                        for(x=0;x<8;x++)
                        {
                            j = EGS(sect,s->x,s->y,s->z-(8<<8),SCRAP3+(TRAND&3),-8,48,48,TRAND&2047,(TRAND&63)+64,-(TRAND&4095)-(s->zvel>>2),i,5);
                            sprite[j].pal = 6;
                        }

                        spritesound(SLIM_DYING,i);
                        spritesound(SQUISHED,i);
                        if( (TRAND&255) < 32 )
                        {
                            j = spawn(i,BLOODPOOL);
                            sprite[j].pal = 0;
                        }
                        ps[p].actors_killed ++;
                        t[0] = -3;
                        if(ps[p].somethingonplayer == i)
                            ps[p].somethingonplayer = -1;
                        KILLIT(i);
                    }

                    s->z = ps[p].posz+ps[p].pyoff-t[2]+(8<<8);

                    s->z += (100-ps[p].horiz)<<4;

                    if( t[2] > 512)
                        t[2] -= 128;

                    if( t[2] < 348)
                        t[2] += 128;

                    if(ps[p].newowner >= 0)
                    {
                        ps[p].newowner = -1;
                        ps[p].posx = ps[p].oposx;
                        ps[p].posy = ps[p].oposy;
                        ps[p].posz = ps[p].oposz;
                        ps[p].ang = ps[p].oang;

                        updatesector(ps[p].posx,ps[p].posy,&ps[p].cursectnum);
                        //setpal(&ps[p]);

                        j = headspritestat[1];
                        while(j >= 0)
                        {
                            if(sprite[j].picnum==CAMERA1) sprite[j].yvel = 0;
                            j = nextspritestat[j];
                        }
                    }

                    if(t[3]>0)
                    {
                        short frames[] = {5,5,6,6,7,7,6,5};

                        s->picnum = GREENSLIME+frames[t[3]];

                        if( t[3] == 5 )
                        {
                            sprite[ps[p].i].extra += -(5+(TRAND&3));
                            spritesound(SLIM_ATTACK,i);
                        }

                        if(t[3] < 7) t[3]++;
                        else t[3] = 0;

                    }
                    else
                    {
                        s->picnum = GREENSLIME+5;
                        if(rnd(32))
                            t[3] = 1;
                    }

                    s->xrepeat = 20+(sintable[t[1]&2047]>>13);
                    s->yrepeat = 15+(sintable[t[1]&2047]>>13);

                    s->x = ps[p].posx + (sintable[(ps[p].ang+512)&2047]>>7);
                    s->y = ps[p].posy + (sintable[ps[p].ang&2047]>>7);

                    goto BOLT;
                }

                else if(s->xvel < 64 && x < 768)
                {
                    if(ps[p].somethingonplayer == -1)
                    {
                        ps[p].somethingonplayer = i;
                        if(t[0] == 3 || t[0] == 2) //Falling downward
                            t[2] = (12<<8);
                        else t[2] = -(13<<8); //Climbing up duke
                        t[0] = -4;
                    }
                }

                    IFHIT
                    {
                        spritesound(SLIM_DYING,i);

                        ps[p].actors_killed ++;
                        if(ps[p].somethingonplayer == i)
                            ps[p].somethingonplayer = -1;

                        /*if(j == FREEZEBLAST)
                        {
                            spritesound(SOMETHINGFROZE,i); t[0] = -5 ; t[3] = 0 ;
                            goto BOLT;
                        }*/

                        if( (TRAND&255) < 32 )
                        {
                            j = spawn(i,BLOODPOOL);
                            sprite[j].pal = 0;
                        }

                        for(x=0;x<8;x++)
                        {
                            j = EGS(sect,s->x,s->y,s->z-(8<<8),SCRAP3+(TRAND&3),-8,48,48,TRAND&2047,(TRAND&63)+64,-(TRAND&4095)-(s->zvel>>2),i,5);
                            sprite[j].pal = 6;
                        }
                        t[0] = -3;
                        KILLIT(i);
                    }
                        // All weap
                if(t[0] == -1) //Shrinking down
                {
                    makeitfall(i);

                    s->cstat &= 65535-8;
                    s->picnum = GREENSLIME+4;

//                    if(s->yrepeat > 62)
  //                      guts(s,JIBS6,5,myconnectindex);

                    if(s->xrepeat > 32) s->xrepeat -= TRAND&7;
                    if(s->yrepeat > 16) s->yrepeat -= TRAND&7;
                    else
                    {
                        s->xrepeat = 40;
                        s->yrepeat = 16;
                        t[5] = -1;
                        t[0] = 0;
                    }

                    goto BOLT;
                }
                else if(t[0] != -2) getglobalz(i);

                if(t[0] == -2) //On top of somebody
                {
                    makeitfall(i);
                    sprite[t[5]].xvel = 0;

                    l = sprite[t[5]].ang;

                    s->z = sprite[t[5]].z;
                    s->x = sprite[t[5]].x+(sintable[(l+512)&2047]>>11);
                    s->y = sprite[t[5]].y+(sintable[l&2047]>>11);

                    s->picnum =  GREENSLIME+2+(global_random&1);

                    if(s->yrepeat < 64) s->yrepeat+=2;
                    else
                    {
                        if(s->xrepeat < 32) s->xrepeat += 4;
                        else
                        {
                            t[0] = -1;
                            x = ldist(s,&sprite[t[5]]);
                            if(x < 768) sprite[t[5]].xrepeat = 0;
                        }
                    }

                    goto BOLT;
                }

                //Check randomly to see of there is an actor near
                if(rnd(32))
                {
                    j = headspritesect[sect];
                    while(j>=0)
                    {
                        switch(sprite[j].picnum)
                        {
                            case LIZTROOP:
                            case LIZMAN:
                            case PIGCOP:
                            case NEWBEAST:
                                if( ldist(s,&sprite[j]) < 768 && (klabs2(s->z-sprite[j].z)<8192) ) //Gulp them
                                {
                                    t[5] = j;
                                    t[0] = -2;
                                    t[1] = 0;
                                    goto BOLT;
                                }
                        }

                        j = nextspritesect[j];
                    }
                }

                //Moving on the ground or ceiling

                if(t[0] == 0 || t[0] == 2)
                {
                    s->picnum = GREENSLIME;

                    if( (TRAND&511) == 0 )
                        spritesound(SLIM_ROAM,i);

                    if(t[0]==2)
                    {
                        s->zvel = 0;
                        s->cstat &= (65535-8);

                        if( (sector[sect].ceilingstat&1) || (hittype[i].ceilingz+6144) < s->z)
                        {
                            s->z += 2048;
                            t[0] = 3;
                            goto BOLT;
                        }
                    }
                    else
                    {
                        s->cstat |= 8;
                        makeitfall(i);
                    }

                    if( everyothertime&1 ) ssp(i,CLIPMASK0);

                    if(s->xvel > 96)
                    {
                        s->xvel -= 2;
                        goto BOLT;
                    }
                    else
                    {
                        if(s->xvel < 32) s->xvel += 4;
                        s->xvel = 64 - (sintable[(t[1]+512)&2047]>>9);

                        s->ang += getincangle(s->ang,
                               getangle(ps[p].posx-s->x,ps[p].posy-s->y))>>3;
// TJR
                    }

                    s->xrepeat = 36 + (sintable[(t[1]+512)&2047]>>11);
                    s->yrepeat = 16 + (sintable[t[1]&2047]>>13);

                    if(rnd(4) && (sector[sect].ceilingstat&1) == 0 &&
                        klabs2(hittype[i].floorz-hittype[i].ceilingz)
                            < (192<<8) )
                            {
                                s->zvel = 0;
                                t[0]++;
                            }

                }

                if(t[0]==1)
                {
                    s->picnum = GREENSLIME;
                    if(s->yrepeat < 40) s->yrepeat+=8;
                    if(s->xrepeat > 8) s->xrepeat-=4;
                    if(s->zvel > -(2048+1024))
                        s->zvel -= 348;
                    s->z += s->zvel;
                    if(s->z < hittype[i].ceilingz+4096)
                    {
                        s->z = hittype[i].ceilingz+4096;
                        s->xvel = 0;
                        t[0] = 2;
                    }
                }

                if(t[0]==3)
                {
                    s->picnum = GREENSLIME+1;

                    makeitfall(i);

                    if(s->z > hittype[i].floorz-(8<<8))
                    {
                        s->yrepeat-=4;
                        s->xrepeat+=2;
                    }
                    else
                    {
                        if(s->yrepeat < (40-4)) s->yrepeat+=8;
                        if(s->xrepeat > 8) s->xrepeat-=4;
                    }

                    if(s->z > hittype[i].floorz-2048)
                    {
                        s->z = hittype[i].floorz-2048;
                        t[0] = 0;
                        s->xvel = 0;
                    }
                }
                goto BOLT;

            case BOUNCEMINE:
            case MORTER:
                //j = spawn(i,FRAMEEFFECT1);
                //hittype[j].temp_data[0] = 3;

            case HEAVYHBOMB:
            case TILE3634:

                if( (s->cstat&32768) )
                {
                    t[2]--;
                    if(t[2] <= 0)
                    {
                        spritesound(TELEPORTER,i);
                        spawn(i,TRANSPORTERSTAR);
                        s->cstat = 257;
                    }
                    goto BOLT;
                }

                p = findplayer(s,&x);

                if( x < 1220 ) s->cstat &= ~257;
                else s->cstat |= 257;

                if(t[3] == 0 )
                {
                    j = ifhitbyweapon(i);
                    if(j >= 0)
                    {
                        t[3] = 1;
                        t[4] = 0;
                        l = 0;
                        s->xvel = 0;
                        goto DETONATEB;
                    }
                    if (s->picnum == MORTER)
                    {
                        if (s->xvel == 0)
                        {
                            t[3] = 1;
                            t[4] = 0;
                            l = 0;
                            goto DETONATEB;
                        }
                        if (sector[sect].lotag == 2)
                        {
                            if (TRAND & 8)
                                spawn(i, WATERBUBBLE);
                        }
                        else
                            spawn(i, SMALLSMOKE);
                    }
                    if (s->picnum == TILE3634)
                    {
                        if (++s->yvel == 64)
                        {
                            t[3] = 1;
                            t[4] = 0;
                            l = 0;
                            s->xvel = 0;
                            goto DETONATEB;
                        }
                        if (s->yvel > 5 && numplayers == 1)
                        {
                            if (sector[sect].lotag == 2)
                            {
                                if (TRAND & 8)
                                    spawn(i, WATERBUBBLE);
                            }
                            else
                                spawn(i, SMALLSMOKE);
                        }
                    }
                }

                zvel = s->zvel;

                if( s->picnum != BOUNCEMINE )
                {
                    makeitfall_bomb(i);

                    j = movesprite(i,
                        (s->xvel*(sintable[(s->ang+512)&2047]))>>14,
                        (s->xvel*(sintable[s->ang&2047]))>>14,
                        s->zvel,CLIPMASK1);

                    if (s->picnum == TILE3634 && t[3] == 0 && s->yvel > 5)
                    {
                        short otherSprite;
                        spritetype *ospr;
                        otherSprite = headspritestat[1];
                        while(otherSprite != -1)
                        {
                            ospr = &sprite[otherSprite];
                            if (badguy(ospr))
                            {
                                int d1;
                                if (ospr->picnum == BOSS1 || ospr->picnum == BOSS2
                                    || ospr->picnum == BOSS3)
                                    d1 = 3072;
                                else
                                    d1 = 1024;
                                if (manhdist(i, otherSprite) < d1)
                                {
                                    t[3] = 1;
                                    t[4] = 1;
                                    l = 0;
                                    s->xvel = 0;
                                    goto DETONATEB;
                                }
                            }
                            otherSprite = nextspritestat[otherSprite];
                        }
                        otherSprite = headspritestat[10];
                        while (otherSprite != -1)
                        {
                            if (s->owner != otherSprite || s->yvel > 10)
                                if (sprite[otherSprite].extra > 0 && manhdist(i, otherSprite) < 1024)
                                {
                                    t[3] = 1;
                                    t[4] = 1;
                                    l = 0;
                                    s->xvel = 0;
                                    goto DETONATEB;
                                }
                            otherSprite = nextspritestat[otherSprite];
                        }
                        otherSprite = headspritestat[6];
                        while (otherSprite != -1)
                        {
                            if (sprite[otherSprite].picnum >= CRACK1 && sprite[otherSprite].picnum <= CRACK4)
                                if (manhdist(i, otherSprite) < 1024)
                                {
                                    t[3] = 1;
                                    t[4] = 1;
                                    l = 0;
                                    s->xvel = 0;
                                    goto DETONATEB;
                                }
                            otherSprite = nextspritestat[otherSprite];
                        }
                    }


                    if( sector[sect].lotag != 1 && s->z >= hittype[i].floorz-(FOURSLEIGHT) && s->yvel < 3 )
                    {
                        if (s->picnum == HEAVYHBOMB)
                        {
                            if( s->yvel > 0 || (s->yvel == 0 && hittype[i].floorz == sector[sect].floorz ))
                                spritesound(PIPEBOMB_BOUNCE,i);
                            s->zvel = -((4-s->yvel)<<8);
                            s->yvel++;
                            s->cstat ^= 4;
                        }
                        else
                        {
                            spritesound(PIPEBOMB_BOUNCE,i);
                            s->zvel  = -(zvel>>1);
                        }
                        if(sector[sect].lotag == 2)
                            s->zvel >>= 2;
                        s->xvel >>= 1;
                    }
                    if( s->z <= hittype[i].ceilingz ) // && sector[sect].lotag != 2 )
                    {
                        s->z = hittype[i].ceilingz+(3<<8);
                        s->zvel = -(zvel>>1);
                    }
                }
                else
                {
                    j = movesprite(i,
                        (s->xvel*(sintable[(s->ang+512)&2047]))>>14,
                        (s->xvel*(sintable[s->ang&2047]))>>14,
                        s->zvel,CLIPMASK1);
                }

                if(sector[sect].lotag == 1 && s->zvel == 0)
                {
                    s->z += (32<<8);
                    s->zvel = zvel;
                    if(t[5] == 0)
                    {
                        t[5] = 1;
                        spawn(i,WATERSPLASH2);
                    }
                }
                else t[5] = 0;

                if(t[3] == 0 && ( s->picnum == BOUNCEMINE || s->picnum == MORTER ) && (j || x < 844) )
                {
                    t[3] = 1;
                    t[4] = 0;
                    l = 0;
                    s->xvel = 0;
                    goto DETONATEB;
                }

                if(sprite[s->owner].picnum == APLAYER && s->picnum == HEAVYHBOMB)
                    l = sprite[s->owner].yvel;
                else l = -1;

                if(sector[sect].lotag == 2)
                {
                    s->xvel *= 0.95;
                    s->zvel *= 0.95;
                }
                if(s->zvel == 0)
                    s->xvel -= 5;
                if(s->xvel < 0)
                    s->xvel = 0;
                if(s->picnum == HEAVYHBOMB && (s->xvel&8)) s->cstat ^= 4;

                if( (j&49152) == 32768 )
                {
                    j &= 0x1fff;

                    checkhitwall(i,j,s->x,s->y,s->z,s->picnum);

                    k = wall[j].overpicnum;
                    if (k != GLASS && k != GLASS2)
                    {
                        k = getangle(
                            wall[wall[j].point2].x-wall[j].x,
                            wall[wall[j].point2].y-wall[j].y);
                        s->ang = ((k<<1) - s->ang)&2047;
                        spritesound(PIPEBOMB_BOUNCE,i);
                    }
                    s->xvel >>= 1;
                    s->zvel >>= 1;
                }

                DETONATEB:

                if( ( l >= 0 && ps[l].hbomb_on == 0 ) || t[3] == 1)
                {
                    t[4]++;

                    if(t[4] == 2)
                    {
                        x = s->extra;
                        m = 0;
                        switch(s->picnum)
                        {
                            case HEAVYHBOMB: m = pipebombblastradius;break;
                            case MORTER: m = morterblastradius;break;
                            case BOUNCEMINE: m = bouncemineblastradius;break;
                            case TILE3634: m = pipebombblastradius;break;
                        }

                        hitradius( i, m,x>>2,x>>1,x-(x>>2),x);
                        spawn(i,EXPLOSION2);
                        //if( s->zvel == 0 )
                        //    spawn(i,EXPLOSION2BOT);
                        spritesound(PIPEBOMB_EXPLODE,i);
                        for(x=0;x<8;x++)
                            RANDOMSCRAP;
                    }

                    if(s->yrepeat)
                    {
                        s->yrepeat = 0;
                        goto BOLT;
                    }

                    if(t[4] > 20)
                    {
                        if(s->owner != i || ud.respawn_items == 0)
                        {
                            KILLIT(i);
                        }
                        else
                        {
                            t[2] = respawnitemtime;
                            spawn(i,RESPAWNMARKERRED);
                            s->cstat = (short) 32768;
                            s->yrepeat = 9;
                            goto BOLT;
                        }
                    }
                }
                else if(s->picnum == HEAVYHBOMB && x < 788 && t[0] > 7 && s->xvel == 0)
                    if( cansee(s->x,s->y,s->z-(8<<8),s->sectnum,ps[p].posx,ps[p].posy,ps[p].posz,ps[p].cursectnum) )
                        if(ps[p].ammo_amount[HANDBOMB_WEAPON] < max_ammo_amount[HANDBOMB_WEAPON] )
                {
                    addammo(HANDBOMB_WEAPON,&ps[p],1);
                    spritesound(DUKE_GET,ps[p].i);
                    FTA(55,&ps[p]);

                    if( ps[p].gotweapon[HANDBOMB_WEAPON] == 0 || s->owner == ps[p].i )
                        addweapon(&ps[p],HANDBOMB_WEAPON);

                    if( sprite[s->owner].picnum != APLAYER )
                    {
                        ps[p].pals[0] = 0;
                        ps[p].pals[1] = 0;
                        ps[p].pals[2] = 32;
                        ps[p].pals_time = 32;
                    }

                    if( s->owner != i || ud.respawn_items == 0 )
                    {
                        KILLIT(i);
                    }
                    else
                    {
                        t[2] = respawnitemtime;
                        spawn(i,RESPAWNMARKERRED);
                        s->cstat = (short) 32768;
                    }
                }

                if(t[0] < 8) t[0]++;
                goto BOLT;

            case REACTORBURNT:
            case REACTOR2BURNT:
                goto BOLT;

            case REACTOR:
            case REACTOR2:

                if( t[4] == 1 )
                {
                    j = headspritesect[sect];
                    while(j >= 0)
                    {
                        switch(sprite[j].picnum)
                        {
                            case SECTOREFFECTOR:
                                if(sprite[j].lotag == 1)
                                {
                                    sprite[j].lotag = (short) 65535;
                                    sprite[j].hitag = (short) 65535;
                                }
                                break;
                            case REACTOR:
                                sprite[j].picnum = REACTORBURNT;
                                break;
                            case REACTOR2:
                                sprite[j].picnum = REACTOR2BURNT;
                                break;
                            case REACTORSPARK:
                            case REACTOR2SPARK:
                                sprite[j].cstat = (short) 32768;
                                break;
                        }
                        j = nextspritesect[j];
                    }
                    goto BOLT;
                }

                if(t[1] >= 20)
                {
                    t[4] = 1;
                    goto BOLT;
                }

                p = findplayer(s,&x);

                t[2]++;
                if( t[2] == 4 ) t[2]=0;

                if( x < 4096 )
                {
                    if( (TRAND&255) < 16 )
                    {
                        if(Sound[DUKE_LONGTERM_PAIN].num < 1)
                            spritesound(DUKE_LONGTERM_PAIN,ps[p].i);

                        spritesound(SHORT_CIRCUIT,i);

                        sprite[ps[p].i].extra --;
                        ps[p].pals_time = 32;
                        ps[p].pals[0] = 32;
                        ps[p].pals[1] = 32;
                        ps[p].pals[2] = 48;
                    }
                    t[0] += 128;
                    if( t[3] == 0 )
                        t[3] = 1;
                }
                else t[3] = 0;

                if( t[1] )
                {
                    t[1]++;

                    t[4] = s->z;
                    s->z = sector[sect].floorz-(TRAND%(sector[sect].floorz-sector[sect].ceilingz));

                    switch( t[1] )
                    {
                        case 3:
                            //Turn on all of those flashing sectoreffector.
                            hitradius( i, 4096,
                                       impact_damage<<2,
                                       impact_damage<<2,
                                       impact_damage<<2,
                                       impact_damage<<2 );
/*
                            j = headspritestat[3];
                            while(j>=0)
                            {
                                if( sprite[j].lotag  == 3 )
                                    hittype[j].temp_data[4]=1;
                                else if(sprite[j].lotag == 12)
                                {
                                    hittype[j].temp_data[4] = 1;
                                    sprite[j].lotag = 3;
                                    sprite[j].owner = 0;
                                    hittype[j].temp_data[0] = s->shade;
                                }
                                j = nextspritestat[j];
                            }
*/
                            j = headspritestat[6];
                            while(j >= 0)
                            {
                                if(sprite[j].picnum == MASTERSWITCH)
                                    if(sprite[j].hitag == s->hitag)
                                        if(sprite[j].yvel == 0)
                                            sprite[j].yvel = 1;
                                j = nextspritestat[j];
                            }
                            break;

                        case 4:
                        case 7:
                        case 10:
                        case 15:
                            j = headspritesect[sect];
                            while(j >= 0)
                            {
                                l = nextspritesect[j];

                                if(j != i)
                                {
                                    deletesprite(j);
                                    break;
                                }
                                j = l;
                            }
                            break;
                    }
                    j = numplayers > 1 ? 1 : 16;
                    for(x=0;x<j;x++)
                        RANDOMSCRAP;

                    s->z = t[4];
                    t[4] = 0;

                }
                else
                {
                    IFHIT
                    {
                        j = numplayers > 1 ? 1 : 32;
                        for(x=0;x<j;x++)
                            RANDOMSCRAP;
                        if(s->extra < 0)
                            t[1] = 1;
                    }
                }
                goto BOLT;

            case CAMERA1:

                if( t[0] == 0 )
                {
                    t[1]+=8;
                #if 0
                    if(camerashitable)
                    {
                        IFHIT
                        {
                            t[0] = 1; // static
                            s->cstat = (short)32768;
                            for(x=0;x<5;x++) RANDOMSCRAP;
                            goto BOLT;
                        }
                    }
                #endif

                    if(s->hitag > 0)
                    {
                        if(t[1]<s->hitag)
                            s->ang+=8;
                        else if(t[1]<(s->hitag*3))
                            s->ang-=8;
                        else if(t[1] < (s->hitag<<2) )
                            s->ang+=8;
                        else
                        {
                            t[1]=8;
                            s->ang+=16;
                        }
                    }
                }
                goto BOLT;
        }


// #ifndef VOLOMEONE
        if( ud.multimode < 2 && badguy(s) )
        {
            if( actor_tog == 1)
            {
                s->cstat = (short)32768;
                goto BOLT;
            }
            else if(actor_tog == 2) s->cstat = 257;
        }
// #endif

        p = findplayer(s,&x);

        execute(i,p,x);

        BOLT:

        i = nexti;
    }

}

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

extern long lastvisinc;

void processweapon(short snum, unsigned long sb_snum)
{
    long i, j, k;
    short pi;
    struct player_struct *p;
    spritetype *s;
    short *kb;
    int k2;
    short dx, dy, h, ang, shake;
    
    p = &ps[snum];
    pi = p->i;
    s = &sprite[pi];
    kb = &p->kickback_pic;
    
    switch( p->curr_weapon )
    {
        case HANDBOMB_WEAPON:

            if( (*kb) == 6 && (sb_snum&(1<<2)) )
            {
                p->rapid_fire_hold = 1;
                break;
            }
            (*kb)++;
            if((*kb)==12)
            {
                p->ammo_amount[HANDBOMB_WEAPON]--;

                if(p->on_ground && (sb_snum&2) )
                {
                    k2 = (15+(p->hbomb_hold_delay<<5));
                    //k = 15;
                    //i = ((p->horiz+p->horizoff-100)*20);
                }
                else
                {
                    k2 = (140+(p->hbomb_hold_delay<<5));
                    //k = 140;
                    //i = -512-((p->horiz+p->horizoff-100)*20);
                }

                //k = (k+(p->hbomb_hold_delay<<5));
                if (sector[p->cursectnum].lotag == 2)
                    k2 /= 2;

                viewhorizang = getanglef(p->horiz+p->horizoff-100, 128.f);
                viewhorizang *= -57.295776f;
                k = cosf((viewhorizang * 3.1415927f) / 180.0f) * k2;
                i = sinf((viewhorizang * 3.1415927f) / 180.0f) * k2;

                j = EGS(p->cursectnum,
                    p->posx+(sintable[(p->ang+512)&2047]>>6),
                    p->posy+(sintable[p->ang&2047]>>6),
                    p->posz,HEAVYHBOMB,-16,9,9,
                    p->ang,k,i<<4,pi,1);

                if(p->on_ground && (sb_snum&2) )
                {
                    sprite[j].yvel = 3;
                    sprite[j].z += (8<<8);
                }

                k = hits(pi);
                if( k < 512 )
                {
                    sprite[j].ang += 1024;
                    sprite[j].zvel /= 3;
                    sprite[j].xvel /= 3;
                }

                p->hbomb_on = 1;

            }
            else if( (*kb) < 12 && (sb_snum&(1<<2)) )
                p->hbomb_hold_delay++;
            else if( (*kb) > 19 )
            {
                (*kb) = 0;
                p->curr_weapon = HANDREMOTE_WEAPON;
                p->f_372 = HANDREMOTE_WEAPON;
                p->wantweaponfire = HANDREMOTE_WEAPON;
                p->last_weapon = -1;
                p->weapon_pos = 10;
            }

            break;


        case HANDREMOTE_WEAPON:

            (*kb)++;

            if((*kb) == 2)
            {
                p->hbomb_on = 0;
            }

            if((*kb) == 10)
            {
                (*kb) = 0;
                if(p->ammo_amount[HANDBOMB_WEAPON] > 0)
                    addweapon(p,HANDBOMB_WEAPON);
                else checkavailweapon(p);
            }
            break;

        case PISTOL_WEAPON:
        case PISTOL2_WEAPON:
            if( (*kb)==1)
            {
                if (p->curr_weapon == PISTOL2_WEAPON)
                    shoot(pi,SHOTSPARK1+3);
                else
                    shoot(pi,SHOTSPARK1);
                spritesound(PISTOL_FIRE,pi);

                p->f_370 = 2;
                lastvisinc = totalclock+32;
                p->visibility = 0;
            }
            else if((*kb) == 2 && numplayers == 1)
                spawn(pi,SHELL);

            (*kb)++;

            if((*kb) >= 5)
            {
                if( p->ammo_amount[p->curr_weapon] <= 0 || (p->ammo_amount[p->curr_weapon]%12) )
                {
                    (*kb)=0;
                    checkavailweapon(p);
                }
                else
                {
                    switch((*kb))
                    {
                        case 5:
                            spritesound(EJECT_CLIP,pi);
                            break;
                        case 8:
                            spritesound(INSERT_CLIP,pi);
                            break;
                    }
                }
            }

            if((*kb) == 27)
            {
                (*kb) = 0;
                checkavailweapon(p);
            }

            break;

        case SHOTGUN_WEAPON:
        case SHOTGUN2_WEAPON:

            (*kb)++;

            if(*kb == 4)
            {
                if (p->curr_weapon == SHOTGUN2_WEAPON)
                {
                    shoot(pi,SHOTSPARK1+4);
                }
                else
                {
                    shoot(pi,SHOTSPARK1+1);
                    shoot(pi,SHOTSPARK1+1);
                    shoot(pi,SHOTSPARK1+1);
                    shoot(pi,SHOTSPARK1+1);
                    shoot(pi,SHOTSPARK1+1);
                    shoot(pi,SHOTSPARK1+1);
                    shoot(pi,SHOTSPARK1+1);
                }

                p->f_370 = 8;
                p->ammo_amount[p->curr_weapon]--;

                spritesound(SHOTGUN_FIRE,pi);

                lastvisinc = totalclock+32;
                p->visibility = 0;
            }

            switch(*kb)
            {
                case 15:
                    spritesound(SHOTGUN_COCK,pi);
                    break;
                case 17:
                case 20:
                    p->kickback_pic++;
                    break;
                case 24:
                    if (numplayers == 1)
                    {
                        j = spawn(pi,SHOTGUNSHELL);
                        sprite[j].zvel -= 64;
                        sprite[j].ang += 1024;
                        ssp(j,CLIPMASK0);
                        sprite[j].ang += 1024;
                    }
                    p->kickback_pic++;
                    break;
                case 31:
                    *kb = 0;
                    checkavailweapon(p);
                    return;
            }
            break;

        case CHAINGUN_WEAPON:

            (*kb)++;

            if( *(kb) <= 12 )
            {
                if( ((*(kb))%3) == 0 )
                {
                    p->ammo_amount[CHAINGUN_WEAPON] -= 2;

                    if( numplayers == 1 )
                    {
                        h = p->horizoff + p->horiz - 100;
                        if (h > 0)
                            h = 140+h/4;
                        else
                            h = 140-(h/2);
                        
                        j = spawn(pi,SHELL);

                        sprite[j].ang = (s->ang + h) & 2047;
                        dx = sintable[(sprite[j].ang + 512)&2047] >> 7;
                        dy = sintable[sprite[j].ang] >> 7;
                        sprite[j].x = s->x + dx;
                        sprite[j].y = s->y + dy;
                        sprite[j].ang = (s->ang + 512) & 2047;
                        sprite[j].z = p->pyoff + p->posz + (3<<8) - (p->horizoff + p->horiz - 100) * 14;
                        sprite[j].zvel -= 160;
                        sprite[j].xvel = 4;
                        ssp(j,CLIPMASK0);
                        
                        j = spawn(pi,SHELL);

                        sprite[j].ang = (s->ang - h) & 2047;
                        dx = sintable[(sprite[j].ang + 512)&2047] >> 7;
                        dy = sintable[sprite[j].ang] >> 7;
                        sprite[j].x = s->x + dx;
                        sprite[j].y = s->y + dy;
                        sprite[j].ang = (s->ang - 512) & 2047;
                        sprite[j].z = p->pyoff + p->posz + (3<<8) - (p->horizoff + p->horiz - 100) * 14;
                        sprite[j].zvel -= 160;
                        sprite[j].xvel = 4;
                        ssp(j,CLIPMASK0);
                    }

                    spritesound(SND268,pi);
                    ang = p->ang;
                    p->ang = (ang - 32) & 2047;
                    shoot(pi,SHOTSPARK1+2);
                    p->ang = (ang + 32) & 2047;
                    shoot(pi,SHOTSPARK1+2);
                    p->ang = ang;
                    p->f_370 = TRAND&3;
                    lastvisinc = totalclock+32;
                    p->visibility = 0;
                    checkavailweapon(p);

                    if( ( sb_snum&(1<<2) ) == 0 )
                    {
                        *kb = 0;
                        break;
                    }
                }
            }
            else if((*kb) > 10)
            {
                if( sb_snum&(1<<2) ) *kb = 1;
                else *kb = 0;
            }

            break;

        case SHRINKER_WEAPON:
        case GROW_WEAPON:

            if(p->curr_weapon == GROW_WEAPON)
            {
                if((*kb) > 3)
                {
                    *kb = 0;
                    if( screenpeek == snum ) pus = 1;
                    p->ammo_amount[GROW_WEAPON]--;
                    shoot(pi,GROWSPARK);

                    p->f_370 = ((TRAND&1) * 2) - 1;
                    p->visibility = 0;
                    lastvisinc = totalclock+32;
                    checkavailweapon(p);
                }
                else (*kb)++;
            }
            else
            {
                if( (*kb) > 10)
                {
                    (*kb) = 0;

                    p->ammo_amount[SHRINKER_WEAPON]--;
                    shoot(pi,SHRINKER);

                    p->visibility = 0;
                    lastvisinc = totalclock+32;
                    checkavailweapon(p);
                }
                else
                {
                    (*kb)++;
                    p->f_370 = ((TRAND&1) * 2) - 1;
                    if ((TRAND&31) < 10)
                        sprite[pi].shade = -96;
                }
            }
            break;

        case DEVISTATOR_WEAPON:
            if(*kb)
            {
                (*kb)++;

                if( (*kb) == 4 )
                {
                    shoot(pi,TILE3634);
                    p->f_370 = 16;
                    p->ammo_amount[p->curr_weapon]--;
                    checkavailweapon(p);
                    spritesound(SND267,pi);
                }
                if((*kb) == 20) (*kb) = 0;
            }
            break;
        case FREEZE_WEAPON:

            (*kb)++;
            if (sb_snum & 4)
            {
                shake = p->hbomb_hold_delay / 8;
                if (shake == 0)
                    p->f_370 = 0;
                else
                    p->f_370 = (TRAND % shake) - shake / 2;
                if (p->hbomb_hold_delay < min(99, p->ammo_amount[FREEZE_WEAPON] * 3))
                    p->hbomb_hold_delay++;
                if ((TRAND&31) < 10)
                    sprite[pi].shade = -96;

                if (!p->f_388)
                    p->f_388 = spritesound(SND265, pi);
            }
            else if (*kb > 3)
            {
                p->ammo_amount[FREEZE_WEAPON] -= max(1, p->hbomb_hold_delay / 3);
                if (p->ammo_amount[FREEZE_WEAPON] < 0)
                    p->ammo_amount[FREEZE_WEAPON] = 0;
                p->visibility = 0;
                lastvisinc = totalclock+32;
                shoot(pi, TILE3841);
                FX_StopSound(p->f_388);
                p->f_388 = 0;
                if (p->hbomb_hold_delay < 66)
                    spritesound(SND264, pi);
                else
                    spritesound(SND266, pi);
                checkavailweapon(p);
                (*kb) = 0;
            }
            break;

        case TRIPBOMB_WEAPON:
            if(*kb < 4)
            {
                p->posz = p->oposz;
                p->poszv = 0;
                if( (*kb) == 3 )
                    shoot(pi,HANDHOLDINGLASER);
            }
            if((*kb) == 16)
            {
                (*kb) = 0;
                checkavailweapon(p);
                p->weapon_pos = -9;
            }
            else (*kb)++;
            break;
        case KNEE_WEAPON:
            (*kb)++;

            if( (*kb) == 7) shoot(pi,KNEE);
            else if( (*kb) == 14)
            {
                if( sb_snum&(1<<2) )
                    *kb = 1+(TRAND&3);
                else *kb = 0;
            }

            if(p->wantweaponfire >= 0)
                checkavailweapon(p);
            break;

        case RPG_WEAPON:
        case RPG2_WEAPON:
            (*kb)++;
            if( (*kb) == 4 )
            {
                p->ammo_amount[p->curr_weapon]--;
                lastvisinc = totalclock+32;
                p->visibility = 0;
                if (p->curr_weapon == RPG2_WEAPON)
                    shoot(pi,RPG+1);
                else
                    shoot(pi,RPG);
                p->f_370 = 16;
                checkavailweapon(p);
            }
            else if( *kb == 30 )
                *kb = 0;
            break;
    }
}

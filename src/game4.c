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

#include "common.h"

long tempwallptr;
short spawn( short j, short pn )
{
    short i, s, startwall, endwall, sect, clostest;
    long x, y, d;
    spritetype *sp;

    if(j >= 0)
    {
        i = EGS(sprite[j].sectnum,sprite[j].x,sprite[j].y,sprite[j].z
            ,pn,0,0,0,0,0,0,j,0);
        hittype[i].picnum = sprite[j].picnum;
    }
    else
    {
        i = pn;

        hittype[i].picnum = PN;
        hittype[i].timetosleep = 0;
        hittype[i].extra = -1;

        hittype[i].bposx = SX;
        hittype[i].bposy = SY;
        hittype[i].bposz = SZ;

        OW = hittype[i].owner = i;
        hittype[i].cgg = 0;
        hittype[i].movflag = 0;
        hittype[i].tempang = 0;
        hittype[i].dispicnum = 0;
        hittype[i].floorz = sector[SECT].floorz;
        hittype[i].ceilingz = sector[SECT].ceilingz;

        hittype[i].lastvx = 0;
        hittype[i].lastvy = 0;
        hittype[i].actorstayput = -1;

        T1 = T2 = T3 = T4 = T5 = T6 = 0;

        if( PN != SPEAKER && PN != LETTER && PN != DUCK && PN != TARGET && PN != TRIPBOMB && PN != VIEWSCREEN && PN != VIEWSCREEN2 && (CS&48) )
            if( !(PN >= CRACK1 && PN <= CRACK4) )
        {
            if(SS == 127) return i;
            if( wallswitchcheck(i) == 1 && (CS&16) )
            {
                if( PN != ACCESSSWITCH && PN != ACCESSSWITCH2 && sprite[i].pal == 1)
                {
                    if( (ud.multimode < 2) || (ud.multimode > 1 && ud.coop==1) )
                    {
                        sprite[i].xrepeat = sprite[i].yrepeat = 0;
                        sprite[i].cstat = SLT = SHT = 0;
                        return i;
                    }
                }
                CS |= 257;
                if( sprite[i].pal == 69)
                    sprite[i].cstat |= 32768;
                if( sprite[i].pal && PN != ACCESSSWITCH && PN != ACCESSSWITCH2)
                    sprite[i].pal = 0;
                return i;
            }

            if( SHT )
            {
                changespritestat(i,12);
                CS |=  257;
                SH = impact_damage;
                return i;
            }
        }
        
        switch( sprite[i].pal)
        {
            case 69:
                sprite[i].cstat |= 32768;
                sprite[i].pal = 0;
        }

        s = PN;

        if( CS&1 ) CS |= 256;

        if( actorscrptr[s] )
        {
            SH = script[actorscrptr[s]];
            T5 = script[actorscrptr[s]+1];
            T2 = script[actorscrptr[s]+2];
            if( script[actorscrptr[s]+3] && SHT == 0 )
                SHT = script[actorscrptr[s]+3];
        }
        else T2 = T5 = 0;
    }

    sp = &sprite[i];
    sect = sp->sectnum;

    switch(sp->picnum)
    {
            default:

                if( actorscrptr[sp->picnum] )
                {
                    if( j == -1 && sp->lotag > ud.player_skill )
                    {
                        sp->xrepeat=sp->yrepeat=0;
                        changespritestat(i,5);
                        break;
                    }

                        //  Init the size
                    if(sp->xrepeat == 0 || sp->yrepeat == 0)
                        sp->xrepeat = sp->yrepeat = 1;
                    
                    sp->clipdist = 40;
                    sp->owner = i;
                    changespritestat(i,1);

                    hittype[i].timetosleep = 0;

                    if(j >= 0)
                        sp->ang = sprite[j].ang;
                }
                break;
            case FOF:
                sp->xrepeat = sp->yrepeat = 0;
                changespritestat(i,5);
                break;
            case WATERSPLASH2:
                if(j >= 0)
                {
                    setsprite(i,sprite[j].x,sprite[j].y,sprite[j].z);
                    sp->xrepeat = sp->yrepeat = 8+(TRAND&7);
                }
                else sp->xrepeat = sp->yrepeat = 16+(TRAND&15);

                sp->shade = -16;
                sp->cstat |= 128+2;
                if(j >= 0)
                {
                    if(sector[sprite[j].sectnum].lotag == 2)
                    {
                        sp->z = getceilzofslope(SECT,SX,SY)+(16<<8);
                        sp->cstat |= 8;
                    }
                    else if( sector[sprite[j].sectnum].lotag == 1)
                        sp->z = getflorzofslope(SECT,SX,SY);
                }

                if(sector[sect].floorpicnum == FLOORSLIME ||
                    sector[sect].ceilingpicnum == FLOORSLIME)
                        sp->pal = 8;
            case NEON1:
            case NEON2:
            case NEON3:
            case NEON4:
            case NEON5:
            case NEON6:
            case DOMELITE:
                if(sp->picnum != WATERSPLASH2)
                    sp->cstat |= 257;
            case NUKEBUTTON:
                if(sp->picnum == NUKEBUTTON && ud.multimode > 1 && ud.coop == 0 && dukematch_mode != 1)
                    sp->xrepeat = sp->yrepeat = 0;
            case JIBS1:
            case JIBS2:
            case JIBS3:
            case JIBS4:
            case JIBS5:
            case JIBS6:
            case HEADJIB1:
            case ARMJIB1:
            case LEGJIB1:
            case LIZMANHEAD1:
            case LIZMANARM1:
            case LIZMANLEG1:
            case DUKETORSO:
            case DUKEGUN:
            case DUKELEG:
                changespritestat(i,5);
                break;
            case TONGUE:
                if(j >= 0)
                    sp->ang = sprite[j].ang;
                sp->z -= 38<<8;
                sp->zvel = 256-(TRAND&511);
                sp->xvel = 64-(TRAND&127);
                changespritestat(i,4);
                break;
            case NATURALLIGHTNING:
                sp->cstat &= ~257;
                sp->cstat |= 32768+2;
                break;
            case TRANSPORTERSTAR:
            case TRANSPORTERBEAM:
                if(j == -1) break;
                if(sp->picnum == TRANSPORTERBEAM)
                {
                    sp->xrepeat = 31;
                    sp->yrepeat = 1;
                    sp->z = sector[sprite[j].sectnum].floorz-(40<<8);
                }
                else
                {
                    if(sprite[j].statnum == 4)
                    {
                        sp->xrepeat = 8;
                        sp->yrepeat = 8;
                    }
                    else
                    {
                        sp->xrepeat = 48;
                        sp->yrepeat = 64;
                        if(sprite[j].statnum == 10 || badguy(&sprite[j]) )
                            sp->z -= (32<<8);
                    }
                }

                sp->shade = -127;
                sp->cstat = 128|2;
                sp->ang = sprite[j].ang;

                sp->xvel = 128;
                changespritestat(i,5);
                ssp(i,CLIPMASK0);
                setsprite(i,sp->x,sp->y,sp->z);
                break;

            case FRAMEEFFECT1:
                if(j >= 0)
                {
                    sp->xrepeat = sprite[j].xrepeat;
                    sp->yrepeat = sprite[j].yrepeat;
                    T2 = sprite[j].picnum;
                }
                else sp->xrepeat = sp->yrepeat = 0;

                changespritestat(i,5);

                break;

            case LASERLINE:
                sp->yrepeat = 6;
                sp->xrepeat = 32;

                /*if(lasermode == 1)
                    sp->cstat = 16 + 2;
                else if(lasermode == 0 || lasermode == 2)
                    sp->cstat = 16;
                else
                {
                    sp->xrepeat = 0;
                    sp->yrepeat = 0;
                }*/
                sp->cstat = 512+16+2;

                if(j >= 0) sp->ang = hittype[j].temp_data[5]+512;
                changespritestat(i,5);
                break;

            case FORCESPHERE:
                if(j == -1 )
                {
                    sp->cstat = (short) 32768;
                    changespritestat(i,2);
                }
                else
                {
                    sp->xrepeat = sp->yrepeat = 1;
                    changespritestat(i,5);
                }
                break;

            case BLOOD:
               sp->xrepeat = sp->yrepeat = 16;
               sp->z -= (26<<8);
               if( j >= 0 && (sprite[j].pal == 6 || sprite[j].picnum == NEWBEAST) )
                   sp->pal = 6;
               changespritestat(i,5);
               break;
            case BLOODPOOL:
            case PUKE:
                {
                    short s1;
                    s1 = sp->sectnum;

                    updatesector(sp->x+108,sp->y+108,&s1);
                    if(s1 >= 0 && sector[s1].floorz == sector[sp->sectnum].floorz)
                    {
                        updatesector(sp->x-108,sp->y-108,&s1);
                        if(s1 >= 0 && sector[s1].floorz == sector[sp->sectnum].floorz)
                        {
                            updatesector(sp->x+108,sp->y-108,&s1);
                            if(s1 >= 0 && sector[s1].floorz == sector[sp->sectnum].floorz)
                            {
                                updatesector(sp->x-108,sp->y+108,&s1);
                                if(s1 >= 0 && sector[s1].floorz != sector[sp->sectnum].floorz)
                                { sp->xrepeat = sp->yrepeat = 0;changespritestat(i,5);break;}
                            }
                            else { sp->xrepeat = sp->yrepeat = 0;changespritestat(i,5);break;}
                        }
                        else { sp->xrepeat = sp->yrepeat = 0;changespritestat(i,5);break;}
                    }
                    else { sp->xrepeat = sp->yrepeat = 0;changespritestat(i,5);break;}
                }

                if( sector[SECT].lotag == 1 )
                {
                    changespritestat(i,5);
                    break;
                }

                if(j >= 0 && sp->picnum != PUKE)
                {
                    if( sprite[j].pal == 1)
                        sp->pal = 1;
                    else if( sprite[j].pal != 6 && sprite[j].picnum != NUKEBARREL && sprite[j].picnum != TIRE )
                    {
                        if(sprite[j].picnum == FECES)
                            sp->pal = 7; // Brown
                        else sp->pal = 2; // Red
                    }
                    else sp->pal = 0;  // green

                    if(sprite[j].picnum == TIRE)
                        sp->shade = 127;
                }
                sp->cstat |= 16384+32;
            case FECES:
                if( j >= 0)
                    sp->xrepeat = sp->yrepeat = 1;
                changespritestat(i,5);
                break;

            case BLOODSPLAT1:
            case BLOODSPLAT2:
            case BLOODSPLAT3:
            case BLOODSPLAT4:
                sp->cstat |= 16384+16;
                sp->xrepeat = 7+(TRAND&7);
                sp->yrepeat = 7+(TRAND&7);
                sp->z -= (16<<8);
                if(j >= 0 && (sprite[j].pal == 6 || sprite[j].picnum == NEWBEAST))
                    sp->pal = 6;
                insertspriteq(i);
                changespritestat(i,5);
                break;

            case TRIPBOMB:
                if( sp->lotag > ud.player_skill )
                {
                    sp->xrepeat=sp->yrepeat=0;
                    changespritestat(i,5);
                    break;
                }

                sp->xrepeat=4;
                sp->yrepeat=5;

                sp->owner = i;
                sp->hitag = i;

                sp->xvel = 16;
                ssp(i,CLIPMASK0);
                hittype[i].temp_data[0] = 17;
                hittype[i].temp_data[2] = 0;
                hittype[i].temp_data[5] = sp->ang;

            case SPACEMARINE:
                if(sp->picnum == SPACEMARINE)
                {
                    sp->extra = 20;
                    sp->cstat |= 257;
                }
                changespritestat(i,2);
                break;

            case HYDRENT:
            case PANNEL1:
            case PANNEL2:
            case SATELITE:
            case FUELPOD:
            case SOLARPANNEL:
            case ANTENNA:
            case GRATE1:
            case CHAIR1:
            case CHAIR2:
            case CHAIR3:
            case BOTTLE1:
            case BOTTLE2:
            case BOTTLE3:
            case BOTTLE4:
            case BOTTLE5:
            case BOTTLE6:
            case BOTTLE7:
            case BOTTLE8:
            case BOTTLE10:
            case BOTTLE11:
            case BOTTLE12:
            case BOTTLE13:
            case BOTTLE14:
            case BOTTLE15:
            case BOTTLE16:
            case BOTTLE17:
            case BOTTLE18:
            case BOTTLE19:
            case OCEANSPRITE1:
            case OCEANSPRITE2:
            case OCEANSPRITE3:
            case OCEANSPRITE5:
            case MONK:
            case INDY:
            case LUKE:
            case JURYGUY:
            case SCALE:
            case VACUUM:
            case FANSPRITE:
            case CACTUS:
            case CACTUSBROKE:
            case HANGLIGHT:
            case FETUS:
            case FETUSBROKE:
            case CAMERALIGHT:
            case MOVIECAMERA:
            case IVUNIT:
            case POT1:
            case POT2:
            case POT3:
            case TRIPODCAMERA:
            case SUSHIPLATE1:
            case SUSHIPLATE2:
            case SUSHIPLATE3:
            case SUSHIPLATE4:
            case SUSHIPLATE5:
            case WAITTOBESEATED:
            case VASE:
            case PIPE1:
            case PIPE2:
            case PIPE3:
            case PIPE4:
            case PIPE5:
            case PIPE6:
                sp->clipdist = 32;
                sp->cstat |= 257;
            case OCEANSPRITE4:
                changespritestat(i,0);
                break;
            case FEMMAG1:
            case FEMMAG2:
                sp->cstat &= ~257;
                changespritestat(i,0);
                break;
            case DUKETAG:
            case SIGN1:
            case SIGN2:
                if(ud.multimode < 2 && sp->pal)
                {
                    sp->xrepeat = sp->yrepeat = 0;
                    changespritestat(i,5);
                }
                else sp->pal = 0;
                break;
            case MASKWALL1:
            case MASKWALL2:
            case MASKWALL3:
            case MASKWALL4:
            case MASKWALL5:
            case MASKWALL6:
            case MASKWALL7:
            case MASKWALL8:
            case MASKWALL9:
            case MASKWALL10:
            case MASKWALL11:
            case MASKWALL12:
            case MASKWALL13:
            case MASKWALL14:
            case MASKWALL15:
                j = sp->cstat&60;
                sp->cstat = j|1;
                changespritestat(i,0);
                break;
            case FOOTPRINTS:
            case FOOTPRINTS2:
            case FOOTPRINTS3:
            case FOOTPRINTS4:
                if(j >= 0)
                {
                    short s1;
                    s1 = sp->sectnum;

                    updatesector(sp->x+84,sp->y+84,&s1);
                    if(s1 >= 0 && sector[s1].floorz == sector[sp->sectnum].floorz)
                    {
                        updatesector(sp->x-84,sp->y-84,&s1);
                        if(s1 >= 0 && sector[s1].floorz == sector[sp->sectnum].floorz)
                        {
                            updatesector(sp->x+84,sp->y-84,&s1);
                            if(s1 >= 0 && sector[s1].floorz == sector[sp->sectnum].floorz)
                            {
                                updatesector(sp->x-84,sp->y+84,&s1);
                                if(s1 >= 0 && sector[s1].floorz != sector[sp->sectnum].floorz)
                                { sp->xrepeat = sp->yrepeat = 0;changespritestat(i,5);break;}
                            }
                            else { sp->xrepeat = sp->yrepeat = 0;break;}
                        }
                        else { sp->xrepeat = sp->yrepeat = 0;break;}
                    }
                    else { sp->xrepeat = sp->yrepeat = 0;break;}

                    sp->cstat = 32+((ps[sprite[j].yvel].footprintcount&1)<<2);
                    sp->cstat |= 16384;
                    sp->ang = sprite[j].ang;
                }

                sp->z = sector[sect].floorz;
                if(sector[sect].lotag != 1 && sector[sect].lotag != 2)
                    sp->xrepeat = sp->yrepeat = 32;

                insertspriteq(i);
                changespritestat(i,5);
                break;

            case PODFEM1:
            case FEM5:
            case FEM6:
                switch (sp->picnum)
                {
                    case FEM5:
                        sp->picnum = TILE3805;
                        break;
                    case FEM6:
                        sp->picnum = TILE3797;
                        break;
                    case PODFEM1:
                        sp->picnum = TILE3821;
                        break;
                }
            case TILE3797:
            case TILE3805:
            case TILE3821:
                if (ud.multimode > 1 && ud.coop == 0)
                {
                    sp->xrepeat = sp->yrepeat = 0;
                    changespritestat(i,5);
                    break;
                }
                ps[myconnectindex].f_36d++;
            case FEM1:
            case FEM2:
            case FEM3:
            case FEM4:
            //case FEM5:
            //case FEM6:
            case FEM7:
            case FEM8:
            case FEM9:
            case FEM10:
            //case PODFEM1:
            case NAKED1:
            case STATUE:
            case TOUGHGAL:
                sp->yvel = sp->hitag;
                sp->hitag = -1;
                //if(sp->picnum == PODFEM1) sp->extra <<= 1;
            //case BLOODYPOLE:

            case QUEBALL:
            case STRIPEBALL:
            case TILE3804:
            case TILE3809:

                if(sp->picnum == QUEBALL || sp->picnum == STRIPEBALL)
                {
                    sp->cstat = 256;
                    sp->clipdist = 8;
                }
                else
                {
                    sp->cstat |= 257;
                    sp->clipdist = 32;
                }

                changespritestat(i,2);
                break;

            case DUKELYINGDEAD:
                if(j >= 0 && sprite[j].picnum == APLAYER)
                {
                    sp->xrepeat = sprite[j].xrepeat;
                    sp->yrepeat = sprite[j].yrepeat;
                    sp->shade = sprite[j].shade;
                    sp->pal = ps[sprite[j].yvel].palookup;
                }
            case DUKECAR:
            case HELECOPT:
//                if(sp->picnum == HELECOPT || sp->picnum == DUKECAR) sp->xvel = 1024;
                sp->cstat = 128;
                sp->extra = 1;
                if (sp->picnum == DUKECAR)
                {
                    sp->xvel = 275;
                    sp->zvel = 600;
                }
                else
                {
                    sp->xvel = 360;
                    sp->zvel = 300;
                }
            case RESPAWNMARKERRED:
            case BLIMP:

                if(sp->picnum == RESPAWNMARKERRED)
                {
                    sp->xrepeat = sp->yrepeat = 24;
                    if(j >= 0) sp->z = hittype[j].floorz; // -(1<<4);
                }
                else
                {
                    sp->cstat |= 257;
                    sp->clipdist = 128;
                }
            case MIKE:
                if(sp->picnum == MIKE)
                    sp->yvel = sp->hitag;
            case WEATHERWARN:
                changespritestat(i,1);
                break;

            case SPOTLITE:
                T1 = sp->x;
                T2 = sp->y;
                break;
            case BULLETHOLE:
                sp->xrepeat = sp->yrepeat = 3;
                sp->cstat = (16+(krand2()&12))|16384|64;
                insertspriteq(i);
            case MONEY:
            case MAIL:
            case PAPER:
                if( sp->picnum == MONEY || sp->picnum == MAIL || sp->picnum == PAPER )
                {
                    hittype[i].temp_data[0] = TRAND&2047;
                    sp->cstat = TRAND&12;
                    sp->xrepeat = sp->yrepeat = 8;
                    sp->ang = TRAND&2047;
                }
                changespritestat(i,5);
                break;

            case VIEWSCREEN:
            case VIEWSCREEN2:
                sp->owner = i;
                sp->lotag = 1;
                sp->extra = 1;
                changespritestat(i,6);
                break;

            case SHELL: //From the player
            case SHOTGUNSHELL:
                if( j >= 0 )
                {
                    short snum,a;

                    if(sprite[j].picnum == APLAYER)
                    {
                        snum = sprite[j].yvel;
                        a = ps[snum].ang-(TRAND&63)+8;  //Fine tune

                        T1 = TRAND&1;
                        if(sp->picnum == SHOTGUNSHELL)
                            sp->z = (6<<8)+ps[snum].pyoff+ps[snum].posz-((ps[snum].horizoff+ps[snum].horiz-100)<<4);
                        else sp->z = (3<<8)+ps[snum].pyoff+ps[snum].posz-((ps[snum].horizoff+ps[snum].horiz-100)<<4);
                        sp->zvel = -(TRAND&255);
                    }
                    else
                    {
                        a = sp->ang;
                        sp->z = sprite[j].z-PHEIGHT+(3<<8);
                    }

                    sp->x = sprite[j].x+(sintable[(a+512+80)&2047]>>7);
                    sp->y = sprite[j].y+(sintable[(a+80)&2047]>>7);

                    sp->shade = -8;

                    sp->ang = a-512;
                    sp->xvel = 20;

                    sp->xrepeat=sp->yrepeat=2;

                    changespritestat(i,5);
                }
                break;

            case RESPAWN:
                sp->extra = 66-13;
            case MUSICANDSFX:
                if( ud.multimode < 2 && sp->pal == 1)
                {
                    sp->xrepeat = sp->yrepeat = 0;
                    changespritestat(i,5);
                    break;
                }
                sp->cstat = (short)32768;
                changespritestat(i,11);
                break;

            case EXPLOSION2:
                switch (sprite[j].picnum)
                {
                    case HELECOPT:
                    case RECON:
                    case DUKECAR:
                        break;
                    case SHOTSPARK1+4:
                        addexplosionmodel2(sp->x >> 1, sp->y >> 1, sp->z >> 5, 1);
                        break;
                    case RPG:
                        addexplosionmodel(sp->x >> 1, sp->y >> 1, sp->z >> 5, 3);
                        addexplosionmodel2(sp->x >> 1, sp->y >> 1, sp->z >> 5, 0);
                        break;
                    case TRIPBOMB:
                        addexplosionmodel2(sp->x >> 1, sp->y >> 1, sp->z >> 5, 1);
                        break;
                    case HEAVYHBOMB:
                    case TILE3634:
                        addexplosionmodel(sp->x >> 1, sp->y >> 1, sp->z >> 5, 2);
                        addexplosionmodel2(sp->x >> 1, sp->y >> 1, sp->z >> 5, 0);
                        break;
                    default:
                        addexplosionmodel(sp->x >> 1, sp->y >> 1, sp->z >> 5, 0);
                        addexplosionmodel2(sp->x >> 1, sp->y >> 1, sp->z >> 5, 0);
                        break;
                }
            case BURNING:
            case BURNING2:
            case SMALLSMOKE:
            case SHRINKEREXPLOSION:
                if (sp->picnum == SHRINKEREXPLOSION)
                    addexplosionmodel(sp->x >> 1, sp->y >> 1, sp->z >> 5, 5);
            case COOLEXPLOSION1:
            case TILE3953:

                if(j >= 0)
                {
                    sp->ang = sprite[j].ang;
                    sp->shade = -64;
                    sp->cstat = 128|(TRAND&4);
                    sp->cstat |= 2;
                }

                if(sp->picnum == EXPLOSION2)
                {
                    sp->xrepeat = 48;
                    sp->yrepeat = 48;
                    sp->shade = -127;
                    sp->cstat |= 128;
                }
                else if(sp->picnum == SHRINKEREXPLOSION )
                {
                    sp->xrepeat = 32;
                    sp->yrepeat = 32;
                }
                else if( sp->picnum == SMALLSMOKE )
                {
                    // 64 "money"
                    sp->xrepeat = 24;
                    sp->yrepeat = 24;
                    if (TRAND % 256 < 128)
                        sp->cstat |= 4;
                    if (TRAND % 256 < 128)
                        sp->cstat |= 8;
                }
                else if(sp->picnum == BURNING || sp->picnum == BURNING2)
                {
                    sp->xrepeat = 4;
                    sp->yrepeat = 4;
                }
                else if(sp->picnum == TILE3953)
                {
                    sp->xrepeat = 64;
                    sp->yrepeat = 64;
                    sp->z -= 30<<8;
                    if ((TRAND&255) < 128) sp->cstat |= 4;
                    if ((TRAND&255) < 128) sp->cstat |= 8;
                }

                if(j >= 0)
                {
                    x = getflorzofslope(sp->sectnum,sp->x,sp->y);
                    if(sp->z > x-(12<<8) )
                        sp->z = x-(12<<8);
                }

                changespritestat(i,5);

                break;

            case PLAYERONWATER:
                if(j >= 0)
                {
                    sp->xrepeat = sprite[j].xrepeat;
                    sp->yrepeat = sprite[j].yrepeat;
                    sp->zvel = 128;
                    if(sector[sp->sectnum].lotag != 2)
                        sp->cstat |= 32768;
                }
                changespritestat(i,13);
                break;

            case APLAYER:
                sp->xrepeat = sp->yrepeat = 0;
                j = ud.coop;
                if(j == 2) j = 0;

                if( ud.multimode < 2 || (ud.multimode > 1 && j != sp->lotag) )
                    changespritestat(i,5);
                else
                    changespritestat(i,10);
                break;
            case WATERBUBBLE:
                if(j >= 0 && sprite[j].picnum == APLAYER)
                    sp->z -= (16<<8);
                if( sp->picnum == WATERBUBBLE)
                {
                    if( j >= 0 )
                        sp->ang = sprite[j].ang;
                    sp->xrepeat = sp->yrepeat = 4;
                }
                else sp->xrepeat = sp->yrepeat = 32;

                sp->cstat |= 2;
                changespritestat(i,5);
                break;

            case CRANE:

                sp->cstat |= 64|257;

                sp->picnum += 2;
                sp->z = sector[sect].ceilingz+(48<<8);
                T5 = tempwallptr;

                msx[tempwallptr] = sp->x;
                msy[tempwallptr] = sp->y;
                msx[tempwallptr+2] = sp->z;

                s = headspritestat[0];
                while(s >= 0)
                {
                    if( sprite[s].picnum == CRANEPOLE && SHT == (sprite[s].hitag) )
                    {
                        msy[tempwallptr+2] = s;

                        T2 = sprite[s].sectnum;

                        sprite[s].xrepeat = 48;
                        sprite[s].yrepeat = 128;

                        msx[tempwallptr+1] = sprite[s].x;
                        msy[tempwallptr+1] = sprite[s].y;

                        sprite[s].x = sp->x;
                        sprite[s].y = sp->y;
                        sprite[s].z = sp->z;
                        sprite[s].shade = sp->shade;

                        setsprite(s,sprite[s].x,sprite[s].y,sprite[s].z);
                        break;
                    }
                    s = nextspritestat[s];
                }

                tempwallptr += 3;
                sp->owner = -1;
                sp->extra = 8;
                changespritestat(i,6);
                break;

            case WATERDRIP:
                if(j >= 0 && sprite[j].statnum == 10 || sprite[j].statnum == 1)
                {
                    sp->shade = 32;
                    if(sprite[j].pal != 1)
                    {
                        sp->pal = 2;
                        sp->z -= (18<<8);
                    }
                    else sp->z -= (13<<8);
                    sp->ang = getangle(ps[0].posx-sp->x,ps[0].posy-sp->y);
                    sp->xvel = 48-(TRAND&31);
                    ssp(i,CLIPMASK0);
                }
                else if(j == -1)
                {
                    sp->z += (4<<8);
                    T1 = sp->z;
                    T2 = TRAND&127;
                }
                sp->cstat |= 2;
            case TRASH:

                if(sp->picnum != WATERDRIP)
                    sp->ang = TRAND&2047;

            case WATERDRIPSPLASH:

                sp->xrepeat = 24;
                sp->yrepeat = 24;


                changespritestat(i,6);
                break;

            case PLUG:
                sp->lotag = 9999;
                changespritestat(i,6);
                break;
            case TOUCHPLATE:
                T3 = sector[sect].floorz;
                if(sector[sect].lotag != 1 && sector[sect].lotag != 2)
                    sector[sect].floorz = sp->z;
                if(sp->pal && ud.multimode > 1)
                {
                    sp->xrepeat=sp->yrepeat=0;
                    changespritestat(i,5);
                    break;
                }
            case WATERBUBBLEMAKER:
                sp->cstat |= 32768;
                changespritestat(i,6);
                break;
            case BOLT1:
            case BOLT1+1:
            case BOLT1+2:
            case BOLT1+3:
            case SIDEBOLT1:
            case SIDEBOLT1+1:
            case SIDEBOLT1+2:
            case SIDEBOLT1+3:
                T1 = sp->xrepeat;
                T2 = sp->yrepeat;
                sp->cstat |= 2;
            case MASTERSWITCH:
                if(sp->picnum == MASTERSWITCH)
                    sp->cstat |= 32768;
                sp->yvel = 0;
                changespritestat(i,6);
                break;
            case TARGET:
            case DUCK:
            case LETTER:
                sp->extra = 1;
                sp->cstat |= 257;
                changespritestat(i,1);
                break;
            case OCTABRAINSTAYPUT:
            case LIZTROOPSTAYPUT:
            case PIGCOPSTAYPUT:
            case LIZMANSTAYPUT:
            case BOSS1STAYPUT:
            case PIGCOPDIVE:
            case COMMANDERSTAYPUT:
            case NEWBEASTSTAYPUT:
                hittype[i].actorstayput = sp->sectnum;
            case BOSS1:
            case BOSS2:
            case BOSS3:
            case ROTATEGUN:
            case GREENSLIME:
                if(sp->picnum == GREENSLIME)
                    sp->extra = 1;
            case DRONE:
            case LIZTROOPONTOILET:
            case LIZTROOPJUSTSIT:
            case LIZTROOPSHOOT:
            case LIZTROOPJETPACK:
            case LIZTROOPDUCKING:
            case LIZTROOPRUNNING:
            case LIZTROOP:
            case OCTABRAIN:
            case COMMANDER:
            case PIGCOP:
            case LIZMAN:
            case LIZMANSPITTING:
            case LIZMANFEEDING:
            case LIZMANJUMP:
            case RAT:
            case SHARK:
            case NEWBEAST:
            case NEWBEASTHANG:
            case NEWBEASTHANGDEAD:
            case NEWBEASTJUMP:

                if(sp->pal == 0)
                {
                    switch(sp->picnum)
                    {
                        case LIZTROOPONTOILET:
                        case LIZTROOPSHOOT:
                        case LIZTROOPJETPACK:
                        case LIZTROOPDUCKING:
                        case LIZTROOPRUNNING:
                        case LIZTROOPSTAYPUT:
                        case LIZTROOPJUSTSIT:
                        case LIZTROOP:
                            sp->pal = 22;
                            break;
                    }
                }

                if( sp->picnum == BOSS1 || sp->picnum == BOSS2 || sp->picnum == BOSS1STAYPUT || sp->picnum == BOSS3 )
                {
                    if(j >= 0 && sprite[j].picnum == RESPAWN)
                        sp->pal = sprite[j].pal;
                    if(sp->pal)
                    {
                        sp->clipdist = 80;
                        sp->xrepeat = 40;
                        sp->yrepeat = 40;
                    }
                    else
                    {
                        sp->xrepeat = 80;
                        sp->yrepeat = 80;
                        sp->clipdist = 164;
                    }
                    if (sp->picnum == BOSS2)
                    {
                        sp->xrepeat = 120;
                        sp->yrepeat = 120;
                        sp->clipdist = 246;
                    }
                }
                else
                {
                    if(sp->picnum != SHARK)
                    {
                        sp->xrepeat = 40;
                        sp->yrepeat = 40;
                        sp->clipdist = 80;
                    }
                    else
                    {
                        sp->xrepeat = 60;
                        sp->yrepeat = 60;
                        sp->clipdist = 40;
                    }
                }

                if(j >= 0) sp->lotag = 0;

                if( ( sp->lotag > ud.player_skill ) || ud.monsters_off == 1 )
                {
                    sp->xrepeat=sp->yrepeat=0;
                    changespritestat(i,5);
                    break;
                }
                else
                {
                    makeitfall(i);

                    if(sp->picnum == RAT)
                    {
                        sp->ang = TRAND&2047;
                        sp->xrepeat = sp->yrepeat = 48;
                        sp->cstat = 0;
                    }
                    else
                    {
                        sp->cstat |= 257;

                        ps[0].max_actors_killed++;
                    }

                    if(j >= 0)
                    {
                        hittype[i].timetosleep = 0;
                        check_fta_sounds(i);
                        changespritestat(i,1);
                    }
                    else changespritestat(i,2);
                }

                if(sp->picnum == ROTATEGUN)
                    sp->zvel = 0;

                break;

            case LOCATORS:
                sp->cstat |= 32768;
                changespritestat(i,7);
                break;

            case ACTIVATORLOCKED:
            case ACTIVATOR:
                sp->cstat = (short) 32768;
                if(sp->picnum == ACTIVATORLOCKED)
                    sector[sp->sectnum].lotag |= 16384;
                changespritestat(i,8);
                break;

            case DOORSHOCK:
                sp->cstat |= 1+256;
                sp->shade = -12;
                changespritestat(i,6);
                break;

            case OOZ:
            case OOZ2:
                sp->shade = -12;

                if(j >= 0)
                {
                    if( sprite[j].picnum == NUKEBARREL )
                        sp->pal = 8;
                    insertspriteq(i);
                }

                changespritestat(i,1);

                getglobalz(i);

                j = (hittype[i].floorz-hittype[i].ceilingz)>>9;

                sp->yrepeat = j;
                sp->xrepeat = 25-(j>>1);
                sp->cstat |= (TRAND&4);

                break;

            case HEAVYHBOMB:
                if(j >= 0)
                    sp->owner = j;
                else sp->owner = i;
                sp->xrepeat = sp->yrepeat = 9;
                sp->yvel = 4;
            case REACTOR2:
            case REACTOR:
            case RECON:

                if(sp->picnum == RECON)
                {
                    if( sp->lotag > ud.player_skill )
                    {
                        sp->xrepeat = sp->yrepeat = 0;
                        changespritestat(i,5);
                        return i;
                    }
                    ps[0].max_actors_killed++;
                    hittype[i].temp_data[5] = 0;
                    if(ud.monsters_off == 1)
                    {
                        sp->xrepeat = sp->yrepeat = 0;
                        changespritestat(i,5);
                        break;
                    }
                    sp->extra = 130;
                }

                if(sp->picnum == REACTOR || sp->picnum == REACTOR2)
                    sp->extra = impact_damage;

                CS |= 257; // Make it hitable

                if( ud.multimode < 2 && sp->pal != 0)
                {
                    sp->xrepeat = sp->yrepeat = 0;
                    changespritestat(i,5);
                    break;
                }
                sp->pal = 0;
                SS = -17;

                changespritestat(i,2);
                break;

            case ATOMICHEALTH:
            case STEROIDS:
            case HEATSENSOR:
            case SHIELD:
            case AIRTANK:
            case TRIPBOMBSPRITE:
            case JETPACK:
            case HOLODUKE:

            case FIRSTGUNSPRITE:
            case CHAINGUNSPRITE:
            case SHOTGUNSPRITE:
            case RPGSPRITE:
            case SHRINKERSPRITE:
            case FREEZESPRITE:
            case DEVISTATORSPRITE:

            case SHOTGUNAMMO:
            case FREEZEAMMO:
            case HBOMBAMMO:
            case CRYSTALAMMO:
            case GROWAMMO:
            case BATTERYAMMO:
            case DEVISTATORAMMO:
            case RPGAMMO:
            case BOOTS:
            case AMMO:
            case AMMOLOTS:
            case COLA:
            case FIRSTAID:
            case SIXPAK:
            case TILE34:
            case TILE43:
            case TILE50:
                if(j >= 0)
                {
                    sp->lotag = 0;
                    sp->z -= (32<<8);
                    sp->zvel = -1024;
                    ssp(i,CLIPMASK0);
                    sp->cstat = TRAND&4;
                }
                else
                {
                    sp->owner = i;
                    sp->cstat = 0;
                }

                if( ( ud.multimode < 2 && sp->pal != 0) || (sp->lotag > ud.player_skill) )
                {
                    sp->xrepeat = sp->yrepeat = 0;
                    changespritestat(i,5);
                    break;
                }

                sp->pal = 0;

            case ACCESSCARD:

                if(sp->picnum == ATOMICHEALTH)
                    sp->cstat |= 128;

                if(ud.multimode > 1 && ud.coop != 1 && sp->picnum == ACCESSCARD)
                {
                    sp->xrepeat = sp->yrepeat = 0;
                    changespritestat(i,5);
                    break;
                }
                else
                {
                    if(sp->picnum == AMMO || sp->picnum == TILE34)
                        sp->xrepeat = sp->yrepeat = 16;
                    else sp->xrepeat = sp->yrepeat = 32;
                }

                sp->shade = -17;

                if(j >= 0) changespritestat(i,1);
                else
                {
                    changespritestat(i,2);
                    makeitfall(i);
                }
                break;

            case WATERFOUNTAIN:
                SLT = 1;

            case TREE1:
            case TREE2:
            case TIRE:
            case CONE:
            case BOX:
                CS = 257; // Make it hitable
                sprite[i].extra = 1;
                changespritestat(i,6);
                break;

            case FLOORFLAME:
                sp->shade = -127;
                changespritestat(i,6);
                break;

            case BOUNCEMINE:
                sp->owner = i;
                sp->cstat |= 1+256; //Make it hitable
                sp->xrepeat = sp->yrepeat = 24;
                sp->shade = -127;
                sp->extra = impact_damage<<2;
                changespritestat(i,2);
                break;

            case CAMERA1:
            case CAMERA1+1:
            case CAMERA1+2:
            case CAMERA1+3:
            case CAMERA1+4:
            case CAMERAPOLE:
                sp->extra = 1;

                /*if(camerashitable) sp->cstat = 257;
                else */sp->cstat = 0;

            case GENERICPOLE:

                if( ud.multimode < 2 && sp->pal != 0 )
                {
                    sp->xrepeat = sp->yrepeat = 0;
                    changespritestat(i,5);
                    break;
                }
                else sp->pal = 0;
                if(sp->picnum == CAMERAPOLE || sp->picnum == GENERICPOLE) break;
                sp->picnum = CAMERA1;
                changespritestat(i,1);
                break;
            case STEAM:
                if(j >= 0)
                {
                    sp->ang = sprite[j].ang;
                    sp->cstat = 16+128+2;
                    sp->xrepeat=sp->yrepeat=1;
                    sp->xvel = -8;
                    ssp(i,CLIPMASK0);
                }
            case CEILINGSTEAM:
                changespritestat(i,6);
                break;

            case SECTOREFFECTOR:
                sp->yvel = sector[sect].extra;
                sp->cstat |= 32768;
                sp->xrepeat = sp->yrepeat = 0;

                switch(sp->lotag)
                {
                    case 28:
                        T6 = 65;// Delay for lightning
                        break;
                    case 7: // Transporters!!!!
                    case 23:// XPTR END
                        if(sp->lotag != 23)
                        {
                            for(j=0;j<MAXSPRITES;j++)
                                if(sprite[j].statnum < MAXSTATUS && sprite[j].picnum == SECTOREFFECTOR && ( sprite[j].lotag == 7 || sprite[j].lotag == 23 ) && i != j && sprite[j].hitag == SHT)
                                {
                                    OW = j;
                                    break;
                                }
                        }
                        else OW = i;

                        T5 = sector[sect].floorz == SZ;
                        sp->cstat = 0;
                        changespritestat(i,9);
                        return i;
                    case 1:
                        sp->owner = -1;
                        T1 = 1;
                        break;
                    case 18:

                        if(sp->ang == 512)
                        {
                            T2 = sector[sect].ceilingz;
                            if(sp->pal)
                                sector[sect].ceilingz = sp->z;
                        }
                        else
                        {
                            T2 = sector[sect].floorz;
                            if(sp->pal)
                                sector[sect].floorz = sp->z;
                        }

                        sp->hitag <<= 2;
                        break;

                    case 19:
                        sp->owner = -1;
                        break;
                    case 25: // Pistons
                        T4 = sector[sect].ceilingz;
                        T5 = 1;
                        sector[sect].ceilingz = sp->z;
                        setinterpolation(&sector[sect].ceilingz);
                        break;
                    case 35:
                        sector[sect].ceilingz = sp->z;
                        break;
                    case 27:
                        if(ud.recstat == 1)
                        {
                            sp->xrepeat=sp->yrepeat=64;
                            sp->cstat &= 32767;
                        }
                        break;
                    case 12:

                        T2 = sector[sect].floorshade;
                        T3 = sector[sect].ceilingshade;
                        break;

                    case 13:

                        T1 = sector[sect].ceilingz;
                        T2 = sector[sect].floorz;

                        if( klabs2(T1-sp->z) < klabs2(T2-sp->z) )
                            sp->owner = 1;
                        else sp->owner = 0;

                        if(sp->ang == 512)
                        {
                            if(sp->owner)
                                sector[sect].ceilingz = sp->z;
                            else
                                sector[sect].floorz = sp->z;
                        }
                        else
                            sector[sect].ceilingz = sector[sect].floorz = sp->z;

                        if( sector[sect].ceilingstat&1 )
                        {
                            sector[sect].ceilingstat ^= 1;
                            T4 = 1;

                            if(!sp->owner && sp->ang==512)
                            {
                                sector[sect].ceilingstat ^= 1;
                                T4 = 0;
                            }

                            sector[sect].ceilingshade =
                                sector[sect].floorshade;

                            if(sp->ang==512)
                            {
                                startwall = sector[sect].wallptr;
                                endwall = startwall+sector[sect].wallnum;
                                for(j=startwall;j<endwall;j++)
                                {
                                    x = wall[j].nextsector;
                                    if(x >= 0)
                                        if( !(sector[x].ceilingstat&1) )
                                    {
                                        sector[sect].ceilingpicnum =
                                            sector[x].ceilingpicnum;
                                        sector[sect].ceilingshade =
                                            sector[x].ceilingshade;
                                        break; //Leave earily
                                    }
                                }
                            }
                        }

                        break;

                    case 17:

                        T3 = sector[sect].floorz; //Stopping loc

                        T4 = sector[sect].ceilingz;

                        T5 = sector[sect].floorz;

                        if(numplayers < 2)
                        {
                            setinterpolation(&sector[sect].floorz);
                            setinterpolation(&sector[sect].ceilingz);
                        }

                        break;

                    case 24:
                        sp->yvel <<= 1;
                    case 36:
                        break;

                    case 20:
                    {
                        long q;

                        startwall = sector[sect].wallptr;
                        endwall = startwall+sector[sect].wallnum;

                        //find the two most clostest wall x's and y's
                        q = 0x7fffffff;

                        for(s=startwall;s<endwall;s++)
                        {
                            x = wall[s].x;
                            y = wall[s].y;

                            d = FindDistance2D(sp->x-x,sp->y-y);
                            if( d < q )
                            {
                                q = d;
                                clostest = s;
                            }
                        }

                        T2 = clostest;

                        q = 0x7fffffff;

                        for(s=startwall;s<endwall;s++)
                        {
                            x = wall[s].x;
                            y = wall[s].y;

                            d = FindDistance2D(sp->x-x,sp->y-y);
                            if(d < q && s != T2)
                            {
                                q = d;
                                clostest = s;
                            }
                        }

                        T3 = clostest;
                    }

                    break;

                    case 3:

                        T4=sector[sect].floorshade;

                        sector[sect].floorshade = sp->shade;
                        sector[sect].ceilingshade = sp->shade;

                        sp->owner = sector[sect].ceilingpal<<8;
                        sp->owner |= sector[sect].floorpal;

                        //fix all the walls;

                        startwall = sector[sect].wallptr;
                        endwall = startwall+sector[sect].wallnum;

                        for(s=startwall;s<endwall;s++)
                        {
                            if(!(wall[s].hitag&1))
                                wall[s].shade=sp->shade;
                            if( (wall[s].cstat&2) && wall[s].nextwall >= 0)
                                wall[wall[s].nextwall].shade = sp->shade;
                        }
                        break;

                    case 31:
                        T2 = sector[sect].floorz;
                    //    T3 = sp->hitag;
                        if(sp->ang != 1536) sector[sect].floorz = sp->z;

                        startwall = sector[sect].wallptr;
                        endwall = startwall+sector[sect].wallnum;

                        for(s=startwall;s<endwall;s++)
                            if(wall[s].hitag == 0) wall[s].hitag = 9999;

                        setinterpolation(&sector[sect].floorz);

                        break;
                    case 32:
                        T2 = sector[sect].ceilingz;
                        T3 = sp->hitag;
                        if(sp->ang != 1536) sector[sect].ceilingz = sp->z;

                        startwall = sector[sect].wallptr;
                        endwall = startwall+sector[sect].wallnum;

                        for(s=startwall;s<endwall;s++)
                            if(wall[s].hitag == 0) wall[s].hitag = 9999;

                        setinterpolation(&sector[sect].ceilingz);

                        break;

                    case 4: //Flashing lights

                        T3 = sector[sect].floorshade;

                        startwall = sector[sect].wallptr;
                        endwall = startwall+sector[sect].wallnum;

                        sp->owner = sector[sect].ceilingpal<<8;
                        sp->owner |= sector[sect].floorpal;

                        for(s=startwall;s<endwall;s++)
                            if(wall[s].shade > T4)
                                T4 = wall[s].shade;

                        break;

                    case 9:
                        if( sector[sect].lotag &&
                            klabs2(sector[sect].ceilingz-sp->z) > 1024)
                                sector[sect].lotag |= 32768; //If its open
                    case 8:
                        //First, get the ceiling-floor shade

                        T1 = sector[sect].floorshade;
                        T2 = sector[sect].ceilingshade;

                        startwall = sector[sect].wallptr;
                        endwall = startwall+sector[sect].wallnum;

                        for(s=startwall;s<endwall;s++)
                            if(wall[s].shade > T3)
                                T3 = wall[s].shade;

                        T4 = 1; //Take Out;

                        break;

                    case 11://Pivitor rotater
                        if(sp->ang>1024) T4 = 2;
                        else T4 = -2;
                    case 0:
                    case 2://Earthquakemakers
                    case 5://Boss Creature
                    case 6://Subway
                    case 14://Caboos
                    case 15://Subwaytype sliding door
                    case 16://That rotating blocker reactor thing
                    case 26://ESCELATOR
                    case 30://No rotational subways

                        if(sp->lotag == 0)
                        {
                            if( sector[sect].lotag == 30 )
                            {
                                if(sp->pal) sprite[i].clipdist = 1;
                                else sprite[i].clipdist = 0;
                                T4 = sector[sect].floorz;
                                sector[sect].hitag = i;
                            }

                            for(j = 0;j < MAXSPRITES;j++)
                            {
                                if( sprite[j].statnum < MAXSTATUS )
                                if( sprite[j].picnum == SECTOREFFECTOR &&
                                    sprite[j].lotag == 1 &&
                                    sprite[j].hitag == sp->hitag)
                                {
                                    if( sp->ang == 512 )
                                    {
                                        sp->x = sprite[j].x;
                                        sp->y = sprite[j].y;
                                    }
                                    break;
                                }
                            }
                            if(j == MAXSPRITES)
                            {
                                sprintf(tempbuf,"Found lonely Sector Effector (lotag 0) at (%ld,%ld)\n",sp->x,sp->y);
                                gameexit(tempbuf);
                            }
                            sp->owner = j;
                        }

                        ms_add(sect, sp->x, sp->y);

                        startwall = sector[sect].wallptr;
                        endwall = startwall+sector[sect].wallnum;

                        T2 = tempwallptr;
                        for(s=startwall;s<endwall;s++)
                        {
                            msx[tempwallptr] = wall[s].x-sp->x;
                            msy[tempwallptr] = wall[s].y-sp->y;
                            tempwallptr++;
                            if(tempwallptr > 1023)
                            {
                                sprintf(tempbuf,"Too many moving sectors at (%ld,%ld).\n",wall[s].x,wall[s].y);
                                gameexit(tempbuf);
                            }
                        }
                        if( sp->lotag == 30 || sp->lotag == 6 || sp->lotag == 14 || sp->lotag == 5 )
                        {

                            startwall = sector[sect].wallptr;
                            endwall = startwall+sector[sect].wallnum;

                            if(sector[sect].hitag == -1)
                                sp->extra = 0;
                            else sp->extra = 1;

                            sector[sect].hitag = i;

                            j = 0;

                            for(s=startwall;s<endwall;s++)
                            {
                                if( wall[ s ].nextsector >= 0 &&
                                    sector[ wall[ s ].nextsector].hitag == 0 &&
                                        sector[ wall[ s ].nextsector].lotag < 3 )
                                    {
                                        s = wall[s].nextsector;
                                        j = 1;
                                        break;
                                    }
                            }

                            if(j == 0)
                            {
                                sprintf(tempbuf,"Subway found no zero'd sectors with locators\nat (%ld,%ld).\n",sp->x,sp->y);
                                gameexit(tempbuf);
                            }

                            sp->owner = -1;
                            T1 = s;

                            if(sp->lotag != 30)
                                T4 = sp->hitag;
                        }

                        else if(sp->lotag == 16)
                            T4 = sector[sect].ceilingz;

                        else if( sp->lotag == 26 )
                        {
                            T4 = sp->x;
                            T5 = sp->y;
                            if(sp->shade==sector[sect].floorshade) //UP
                                sp->zvel = -256;
                            else
                                sp->zvel = 256;

                            sp->shade = 0;
                        }
                        else if( sp->lotag == 2)
                        {
                            T6 = sector[sp->sectnum].floorheinum;
                            sector[sp->sectnum].floorheinum = 0;
                        }
                }

                switch(sp->lotag)
                {
                    case 6:
                    case 14:
                    case 30:
                        if (sp->lotag != 30 || sp->pal != 0)
                        {
                            j = callsound(sect,i);
                            if(j == -1) j = SUBWAY;
                            hittype[i].lastvx = j;
                        }
                        if(numplayers > 1) break;
                    case 0:
                    case 1:
                    case 5:
                    case 11:
                    case 15:
                    case 16:
                    case 26:
                        setsectinterpolate(i);
                        break;
                }

                switch(sprite[i].lotag)
                {
                    case 40:
                    case 41:
                    case 43:
                    case 44:
                    case 45:
                        changespritestat(i,15);
                        break;
                    default:
                        changespritestat(i,3);
                        break;
                }

                break;


            case SEENINE:
            case OOZFILTER:

                sp->shade = -16;
                if(sp->xrepeat <= 8)
                {
                    sp->cstat = (short)32768;
                    sp->xrepeat=sp->yrepeat=0;
                }
                else sp->cstat = 1+256;
                sp->extra = impact_damage<<2;
                sp->owner = i;

                changespritestat(i,6);
                break;

            case CRACK1:
            case CRACK2:
            case CRACK3:
            case CRACK4:
            case FIREEXT:
                if(sp->picnum == FIREEXT)
                {
                    sp->cstat = 257;
                    sp->extra = impact_damage<<2;
                }
                else
                {
                    sp->cstat |= 17;
                    sp->extra = 1;
                }

                if( ud.multimode < 2 && sp->pal != 0)
                {
                    sp->xrepeat = sp->yrepeat = 0;
                    changespritestat(i,5);
                    break;
                }

                sp->pal = 0;
                sp->owner = i;
                changespritestat(i,6);
                sp->xvel = 8;
                ssp(i,CLIPMASK0);
                break;

            case TOILET:
            case STALL:
                sp->lotag = 1;
                sp->cstat |= 257;
                sp->clipdist = 8;
                sp->owner = i;
                break;
            case CANWITHSOMETHING:
            case CANWITHSOMETHING2:
            case CANWITHSOMETHING3:
            case CANWITHSOMETHING4:
            case RUBBERCAN:
                sp->extra = 0;
            case EXPLODINGBARREL:
            case HORSEONSIDE:
            case FIREBARREL:
            case NUKEBARREL:
            case FIREVASE:
            case NUKEBARRELDENTED:
            case NUKEBARRELLEAKED:
            case WOODENHORSE:

                if(j >= 0)
                    sp->xrepeat = sp->yrepeat = 32;
                sp->clipdist = 72;
                makeitfall(i);
                if(j >= 0)
                    sp->owner = j;
                else sp->owner = i;
            case EGG:
                if( ud.monsters_off == 1 && sp->picnum == EGG )
                {
                    sp->xrepeat = sp->yrepeat = 0;
                    changespritestat(i,5);
                }
                else
                {
                    if(sp->picnum == EGG)
                        sp->clipdist = 24;
                    sp->cstat = 257|(TRAND&4);
                    changespritestat(i,2);
                }
                break;
            case TOILETWATER:
                sp->shade = -16;
                sp->cstat |= 2;
                changespritestat(i,6);
                break;
    }
    return i;
}

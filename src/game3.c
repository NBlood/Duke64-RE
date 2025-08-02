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

char numenvsnds = 0;
int32 AmbienceToggle = 1;
int32 SoundToggle = 1;
char actor_tog = 0;

void gameexit(char *t)
{
    "%s\n";
	while (1) {}
}

int func_80099488(char* s)
{
    short l;
    l = 0;
    while (*s != 0 && *s != '\n' && *s != '\r')
    {
        if (*s == ' ')
            l += 7;
        else
            l += tileinfo[gettileid(*s + STARTALPHANUM - 33)].tilesizx;

        s++;
    }
    return l;
}

void operatefta(void)
{
    int unk[2];
    char i;

    short a;
    short b;
    short c;
    short d;
    short e;
    short f;

    gDPSetScissor(pkt++, G_SC_NON_INTERLACE, 21, 15, 299, 225);

    for (i = 0; i < numplayers; i++)
    {
        e = -1;
        if (ps[i].fta > 1)
        {
            if (death_fade)
                ps[i].fta = min(ps[i].fta, 10);

            switch(i)
            {
                case 0:
                    if (numplayers > 2)
                        e = 23;
                    a = 34;
                    b = 64;
                    c = 200;
                    d = 200;
                    break;
                case 1:
                    if (numplayers > 2)
                    {
                        a = 44;
                        e = 0x129 - func_80099488(fta_quotes[ps[i].ftq]);
                    }
                    else
                        a = 0x82;
                    b = 64;
                    c = 200;
                    d = 64;
                    break;
                case 2:
                    if (numplayers > 2)
                        e = 23;
                    d = 200;
                    a = 140;
                    b = 200;
                    c = 64;
                    break;
                case 3:
                    c = 200;
                    d = 64;
                    b = 200;
                    a = 150;
                    if (numplayers > 2)
                        e = 0x129 - func_80099488(fta_quotes[ps[i].ftq]);
                    break;
            }
            f = (ps[i].fta << 8) / 10;
            if (f >= 256) f = 0x29a;
            setrotatespriteflags(b, c, d, f);
            gametext(e, a, fta_quotes[ps[i].ftq]);
            setrotatespriteflags(0xFF, 0xFF, 0xFF, 0x100);
        }
    }
}

void FTA(short q,struct player_struct *p)
{
    if( ud.fta_on == 1)
    {
        if( p->fta > 0 && (q < 115 || q > 118) )
            if( p->ftq >= 115 && p->ftq <= 118 ) return;
        
        p->fta = 100;
        p->ftq = q;
    }
}

char domovethings(void)
{
    short i, j, b;
    char ch;

    ud.camerasprite = -1;
    lockclock += TICSPERFRAME;

    if(earthquaketime > 0)
    {
        for (i = 0; i < numplayers; i++)
        {
            if (ps[i].last_extra > 0)
                controllersetvib(i, 5, 128);
        }
        earthquaketime--;
    }

    everyothertime++;
    
    lockclock += TICSPERFRAME;

    updateinterpolations();

    if( ud.pause_on == 0 )
    {
        global_random = TRAND;
    }

    b = 0;
    for(i=0;i<numplayers;i++)
    {
        b |= control_button[i];
    }

    if (game_inactive)
    {
        if (b)
            gamestate = 13;
    }
    else if (b & 0x1000)
    {
        if (menu_inputfreeze == 0)
        {
            current_menu = next_menu = 1;
            controllervibstopall();
            doinitrumble = 1;
            gamestate = 3;
            menu_inputfreeze = 50;
            sound(0xaa);
        }
    }
    else
        menu_inputfreeze = 0;

    if (menu_inputfreeze > 0)
        menu_inputfreeze--;

    for(i=0;i<ud.multimode;i++)
    {
        cheatkeys(i);

        if( ud.pause_on == 0 )
        {
            if (i < numplayers)
            {
                getinput(i);
                sync[i].bits = loc.bits;
                sync[i].fvel = loc.fvel;
                sync[i].svel = loc.svel;
                sync[i].avel = loc.avel;
                sync[i].horz = loc.horz;
            }
            else
                computerinput(i, &sync[i]);
            
            processinput(i);
            checksectors(i);
        }
    }

    if( ud.pause_on == 0 )
    {
        movefta();//ST 2
        moveweapons();          //ST 5 (must be last)
        movetransports();       //ST 9

        moveplayers();          //ST 10
        movefallers();          //ST 12
        moveexplosions();       //ST 4

        moveactors();           //ST 1
        moveeffectors();        //ST 3

        movestandables();       //ST 6
        doanimations();
        movefx();               //ST 11
    }

    if( (everyothertime&1) == 0)
    {
        animatewalls();
        movecyclers();
    }


    return 0;
}


void check_fta_sounds(short i)
{
    if(sprite[i].extra > 0) switch(PN)
    {
        case LIZTROOPONTOILET:
        case LIZTROOPJUSTSIT:
        case LIZTROOPSHOOT:
        case LIZTROOPJETPACK:
        case LIZTROOPDUCKING:
        case LIZTROOPRUNNING:
        case LIZTROOP:
            spritesound(PRED_RECOG,i);
            break;
        case LIZMAN:
        case LIZMANSPITTING:
        case LIZMANFEEDING:
        case LIZMANJUMP:
            spritesound(CAPT_RECOG,i);
            break;
        case PIGCOP:
        case PIGCOPDIVE:
            spritesound(PIG_RECOG,i);
            break;
        case RECON:
            spritesound(RECO_RECOG,i);
            break;
        case DRONE:
            spritesound(DRON_RECOG,i);
            break;
        case COMMANDER:
        case COMMANDERSTAYPUT:
            spritesound(COMM_RECOG,i);
            break;
        case OCTABRAIN:
        case OCTABRAINSTAYPUT:
            spritesound(OCTA_RECOG,i);
            break;
        case BOSS1:
            if(sprite[i].pal)
                spritesound(BOS1_RECOG,i);
            else spritesound(SND262,ps[0].i);
            break;
        case BOSS2:
            spritesound(WHIPYOURASS,ps[0].i);
            break;
        case BOSS3:
            if(sprite[i].pal)
                spritesound(BOS3_RECOG,i);
            else spritesound(SND261,ps[0].i);
            break;
        case GREENSLIME:
            spritesound(SLIM_RECOG,i);
            break;
    }
}


short badguy(spritetype *s)
{

    switch(s->picnum)
    {
            case SHARK:
            case RECON:
            case DRONE:
            case LIZTROOPONTOILET:
            case LIZTROOPJUSTSIT:
            case LIZTROOPSTAYPUT:
            case LIZTROOPSHOOT:
            case LIZTROOPJETPACK:
            case LIZTROOPDUCKING:
            case LIZTROOPRUNNING:
            case LIZTROOP:
            case OCTABRAIN:
            case COMMANDER:
            case COMMANDERSTAYPUT:
            case PIGCOP:
            case EGG:
            case PIGCOPSTAYPUT:
            case PIGCOPDIVE:
            case LIZMAN:
            case LIZMANSPITTING:
            case LIZMANFEEDING:
            case LIZMANJUMP:
            case BOSS1:
            case BOSS2:
            case BOSS3:
            case GREENSLIME:
            case GREENSLIME+1:
            case GREENSLIME+2:
            case GREENSLIME+3:
            case GREENSLIME+4:
            case GREENSLIME+5:
            case GREENSLIME+6:
            case GREENSLIME+7:
            case RAT:
            case ROTATEGUN:
            case NEWBEAST:
            case NEWBEASTSTAYPUT:
            case NEWBEASTHANG:
            case NEWBEASTHANGDEAD:
            case NEWBEASTJUMP:
                return 1;
    }
    //if( actortype[s->picnum] ) return 1;

    return 0;
}


short badguypic(short pn)
{

    switch(pn)
    {
            case SHARK:
            case RECON:
            case DRONE:
            case LIZTROOPONTOILET:
            case LIZTROOPJUSTSIT:
            case LIZTROOPSTAYPUT:
            case LIZTROOPSHOOT:
            case LIZTROOPJETPACK:
            case LIZTROOPDUCKING:
            case LIZTROOPRUNNING:
            case LIZTROOP:
            case OCTABRAIN:
            case COMMANDER:
            case COMMANDERSTAYPUT:
            case PIGCOP:
            case EGG:
            case PIGCOPSTAYPUT:
            case PIGCOPDIVE:
            case LIZMAN:
            case LIZMANSPITTING:
            case LIZMANFEEDING:
            case LIZMANJUMP:
            case BOSS1:
            case BOSS2:
            case BOSS3:
            case GREENSLIME:
            case GREENSLIME+1:
            case GREENSLIME+2:
            case GREENSLIME+3:
            case GREENSLIME+4:
            case GREENSLIME+5:
            case GREENSLIME+6:
            case GREENSLIME+7:
            case RAT:
            case ROTATEGUN:
            case NEWBEAST:
            case NEWBEASTSTAYPUT:
            case NEWBEASTHANG:
            case NEWBEASTHANGDEAD:
            case NEWBEASTJUMP:
                return 1;
    }

    //if( actortype[pn] ) return 1;

    return 0;
}

short inventory(spritetype *s)
{
    switch(s->picnum)
    {
        case FIRSTAID:
        case STEROIDS:
        case HEATSENSOR:
        case BOOTS:
        case JETPACK:
        case HOLODUKE:
        case AIRTANK:
            return 1;
    }
    return 0;
}

void lotsofglass(short i,short wallnum,short n)
{
     long j, xv, yv, z, x1, y1;
     short sect, a;

     sect = -1;
     n = n / (numplayers << 1);

     if(wallnum < 0)
     {
        for(j=n-1; j >= 0 ;j--)
        {
            a = SA-256+(TRAND&511)+1024;
            EGS(SECT,SX,SY,SZ,GLASSPIECES+(j%3),-32,36,36,a,32+(TRAND&63),1024-(TRAND&1023),i,5);
        }
        return;
     }

     j = n+1;

     x1 = wall[wallnum].x;
     y1 = wall[wallnum].y;

     xv = wall[wall[wallnum].point2].x-x1;
     yv = wall[wall[wallnum].point2].y-y1;

     x1 -= ksgn(yv);
     y1 += ksgn(xv);

     xv /= j;
     yv /= j;

     for(j=n;j>0;j--)
         {
                  x1 += xv;
                  y1 += yv;

          updatesector(x1,y1,&sect);
          if(sect >= 0)
          {
              z = sector[sect].floorz-(TRAND&(klabs2(sector[sect].ceilingz-sector[sect].floorz)));
              if( z < -(32<<8) || z > (32<<8) )
                  z = SZ-(32<<8)+(TRAND&((64<<8)-1));
              a = SA-1024;
              EGS(SECT,x1,y1,z,GLASSPIECES+(j%3),-32,36,36,a,32+(TRAND&63),-(TRAND&1023),i,5);
          }
         }
}

void spriteglass(short i,short n)
{
    long j, k, a, z;

    n = n / (numplayers << 1);

    for(j=n;j>0;j--)
    {
        a = krand()&2047;
        z = SZ-((krand()&16)<<8);
        k = EGS(SECT,SX,SY,z,GLASSPIECES+(j%3),krand()&15,36,36,a,32+(krand()&63),-512-(krand()&2047),i,5);
        sprite[k].pal = sprite[i].pal;
    }
}

void ceilingglass(short i,short sectnum,short n)
{
     long j, xv, yv, z, x1, y1;
     short a,s, startwall,endwall;

     startwall = sector[sectnum].wallptr;
     endwall = startwall+sector[sectnum].wallnum;

     n = n / (numplayers << 1);

     for(s=startwall;s<(endwall-1);s++)
     {
         x1 = wall[s].x;
         y1 = wall[s].y;

         xv = (wall[s+1].x-x1)/(n+1);
         yv = (wall[s+1].y-y1)/(n+1);

         for(j=n;j>0;j--)
         {
              x1 += xv;
              y1 += yv;
              a = TRAND&2047;
              z = sector[sectnum].ceilingz+((TRAND&15)<<8);
              EGS(sectnum,x1,y1,z,GLASSPIECES+(j%3),-32,36,36,a,(TRAND&31),0,i,5);
          }
     }
}



void lotsofcolourglass(short i,short wallnum,short n)
{
     long j, xv, yv, z, x1, y1;
     short sect = -1, a, k;

     n = n / (numplayers << 1);

     if(wallnum < 0)
     {
        for(j=n-1; j >= 0 ;j--)
        {
            a = TRAND&2047;
            k = EGS(SECT,SX,SY,SZ-(TRAND&(63<<8)),GLASSPIECES+(j%3),-32,36,36,a,32+(TRAND&63),1024-(TRAND&2047),i,5);
            sprite[k].pal = TRAND&15;
        }
        return;
     }

     j = n+1;
     x1 = wall[wallnum].x;
     y1 = wall[wallnum].y;

     xv = (wall[wall[wallnum].point2].x-wall[wallnum].x)/j;
     yv = (wall[wall[wallnum].point2].y-wall[wallnum].y)/j;

     for(j=n;j>0;j--)
         {
                  x1 += xv;
                  y1 += yv;

          updatesector(x1,y1,&sect);
          z = sector[sect].floorz-(TRAND&(klabs2(sector[sect].ceilingz-sector[sect].floorz)));
          if( z < -(32<<8) || z > (32<<8) )
              z = SZ-(32<<8)+(TRAND&((64<<8)-1));
          a = SA-1024;
          k = EGS(SECT,x1,y1,z,GLASSPIECES+(j%3),-32,36,36,a,32+(TRAND&63),-(TRAND&2047),i,5);
          sprite[k].pal = TRAND&7;
         }
}

short LocateTheLocator(short n,short sn)
{
    short i;

    i = headspritestat[7];
    while(i >= 0)
    {
        if( (sn == -1 || sn == SECT) && n == SLT )
            return i;
        i = nextspritestat[i];
    }
    return -1;
}

short EGS(short whatsect,long s_x,long s_y,long s_z,short s_pn,signed char s_s,signed char s_xr,signed char s_yr,short s_a,short s_ve,long s_zv,short s_ow,signed char s_ss)
{
    short i;
    spritetype *s;
    char buf[100];

    i = insertsprite(whatsect,s_ss);

    if( i < 0 )
    {
        sprintf(buf, " Too many sprites spawned : PN = %d OW = %d", s_pn, sprite[s_ow].picnum);
        gameexit(buf);
    }

    hittype[i].bposx = s_x;
    hittype[i].bposy = s_y;
    hittype[i].bposz = s_z;

    s = &sprite[i];

    s->x = s_x;
    s->y = s_y;
    s->z = s_z;
    s->cstat = 0;
    s->picnum = s_pn;
    s->shade = s_s;
    s->xrepeat = s_xr;
    s->yrepeat = s_yr;
    s->pal = 0;

    s->ang = s_a;
    s->xvel = s_ve;
    s->zvel = s_zv;
    s->owner = s_ow;
    s->xoffset = 0;
    s->yoffset = 0;
    s->yvel = 0;
    s->clipdist = 0;
    s->pal = 0;
    s->lotag = 0;

    hittype[i].picnum = sprite[s_ow].picnum;

    hittype[i].lastvx = 0;
    hittype[i].lastvy = 0;

    hittype[i].timetosleep = 0;
    hittype[i].actorstayput = -1;
    hittype[i].extra = -1;
    hittype[i].owner = s_ow;
    hittype[i].cgg = 0;
    hittype[i].movflag = 0;
    hittype[i].tempang = 0;
    hittype[i].dispicnum = 0;
    hittype[i].floorz = hittype[s_ow].floorz;
    hittype[i].ceilingz = hittype[s_ow].ceilingz;

    T1=T3=T4=T6=0;
    if( actorscrptr[s_pn] )
    {
        s->extra = script[actorscrptr[s_pn]];
        T5 = script[actorscrptr[s_pn]+1];
        T2 = script[actorscrptr[s_pn]+2];
        s->hitag = script[actorscrptr[s_pn]+3];
    }
    else
    {
        T2=T5=0;
        s->extra = 0;
        s->hitag = 0;
    }
    
    return(i);
}

char wallswitchcheck(short i)
{
    switch(PN)
    {
        case HANDPRINTSWITCH:
        case HANDPRINTSWITCH+1:
        case ALIENSWITCH:
        case ALIENSWITCH+1:
        case MULTISWITCH:
        case MULTISWITCH+1:
        case MULTISWITCH+2:
        case MULTISWITCH+3:
        case ACCESSSWITCH:
        case ACCESSSWITCH2:
        case PULLSWITCH:
        case PULLSWITCH+1:
        case HANDSWITCH:
        case HANDSWITCH+1:
        case SLOTDOOR:
        case SLOTDOOR+1:
        case LIGHTSWITCH:
        case LIGHTSWITCH+1:
        case SPACELIGHTSWITCH:
        case SPACELIGHTSWITCH+1:
        case SPACEDOORSWITCH:
        case SPACEDOORSWITCH+1:
        case FRANKENSTINESWITCH:
        case FRANKENSTINESWITCH+1:
        case LIGHTSWITCH2:
        case LIGHTSWITCH2+1:
        case POWERSWITCH1:
        case POWERSWITCH1+1:
        case LOCKSWITCH1:
        case LOCKSWITCH1+1:
        case POWERSWITCH2:
        case POWERSWITCH2+1:
        case DIPSWITCH:
        case DIPSWITCH+1:
        case DIPSWITCH2:
        case DIPSWITCH2+1:
        case TECHSWITCH:
        case TECHSWITCH+1:
        case DIPSWITCH3:
        case DIPSWITCH3+1:
            return 1;
    }
    return 0;
}

void animatesprites(long x,long y,short a,long smoothratio,int spr,spritetype *t)
{
    short i, j, k, p, sect;
    long l, t1,t3,t4;
    spritetype *s;
    
    fastmemcpy(t, &sprite[spr], 0x2C);
    t->owner = spr;
    i = t->owner;
    s = &sprite[t->owner];
    switch(t->picnum)
    {
        case BLOODPOOL:
        case PUKE:
        case FOOTPRINTS:
        case FOOTPRINTS2:
        case FOOTPRINTS3:
        case FOOTPRINTS4:
            if(t->shade == 127) goto SKIPSHADE;
            break;
        case RESPAWNMARKERRED:
        case RESPAWNMARKERYELLOW:
        case RESPAWNMARKERGREEN:
            if(ud.marker == 0)
                t->xrepeat = t->yrepeat = 0;
            goto SKIPSHADE;
        case CHAIR3:

            k = (((t->ang+3072+128-a)&2047)>>8)&7;
            if(k>4)
            {
                k = 8-k;
                t->cstat |= 4;
            }
            else t->cstat &= ~4;
            t->picnum = s->picnum+k;
            break;
        case BLOODSPLAT1:
        case BLOODSPLAT2:
        case BLOODSPLAT3:
        case BLOODSPLAT4:
            if(ud.lockout) t->xrepeat = t->yrepeat = 0;
            else if(t->pal == 6)
            {
                t->shade = -127;
                goto SKIPSHADE;
            }
        case BULLETHOLE:
        case CRACK1:
        case CRACK2:
        case CRACK3:
        case CRACK4:
            t->shade = 16;
            goto SKIPSHADE;
        case NEON1:
        case NEON2:
        case NEON3:
        case NEON4:
        case NEON5:
        case NEON6:
            goto SKIPSHADE;
        case GREENSLIME:
        case GREENSLIME+1:
        case GREENSLIME+2:
        case GREENSLIME+3:
        case GREENSLIME+4:
        case GREENSLIME+5:
        case GREENSLIME+6:
        case GREENSLIME+7:
            break;
        default:
            if( ( (t->cstat&16) ) || ( badguy(t) && t->extra > 0) || t->statnum == 10)
                goto SKIPSHADE;
    }

    if (sector[t->sectnum].ceilingstat&1)
        l = sector[t->sectnum].ceilingshade;
    else
        l = sector[t->sectnum].floorshade;

    if(l < -127) l = -127;
    if(l > 128) l =  127;
    t->shade = l;

SKIPSHADE:

    switch(s->picnum)
    {
        case SECTOREFFECTOR:
            if(t->lotag == 27 && ud.recstat == 1)
            {
                t->picnum = 11+((totalclock>>3)&1);
                t->cstat |= 128;
            }
            else
                t->xrepeat = t->yrepeat = 0;
            break;
        case NATURALLIGHTNING:
           t->shade = -127;
           break;
        //case FEM1:
        //case FEM2:
        //case FEM3:
        //case FEM4:
        //case FEM5:
        //case FEM6:
        //case FEM7:
        //case FEM8:
        //case FEM9:
        //case FEM10:
        //case MAN:
        //case MAN2:
        //case WOMAN:
        //case NAKED1:
        //case PODFEM1:
        //case FEMMAG1:
        //case FEMMAG2:
        //case FEMPIC1:
        //case FEMPIC2:
        //case FEMPIC3:
        //case FEMPIC4:
        //case FEMPIC5:
        //case FEMPIC6:
        //case FEMPIC7:
        //case BLOODYPOLE:
        //case FEM6PAD:
        //case STATUE:
        //case STATUEFLASH:
        case OOZ:
        case OOZ2:
        case WALLBLOOD1:
        case WALLBLOOD2:
        case WALLBLOOD3:
        case WALLBLOOD4:
        case WALLBLOOD5:
        case WALLBLOOD7:
        case WALLBLOOD8:
        case SUSHIPLATE1:
        case SUSHIPLATE2:
        case SUSHIPLATE3:
        case SUSHIPLATE4:
        case FETUS:
        case FETUSJIB:
        case FETUSBROKE:
        case HOTMEAT:
        case FOODOBJECT16:
        //case DOLPHIN1:
        //case DOLPHIN2:
        //case TOUGHGAL:
        //case TAMPON:
        //case XXXSTACY:
        //case 4946:
        //case 4947:
        case 693:
        case 2254:
        //case 4560:
        //case 4561:
        //case 4562:
        //case 4498:
        //case 4957:
            if(ud.lockout)
            {
                t->xrepeat = t->yrepeat = 0;
                return;
            }
    }

    if( t->statnum == 99 ) return;
    if( s->statnum != 1 && s->picnum == APLAYER && ps[s->yvel].newowner == -1 && s->owner >= 0 )
    {
        t->x -= mulscale16(65536-smoothratio,ps[s->yvel].posx-ps[s->yvel].oposx);
        t->y -= mulscale16(65536-smoothratio,ps[s->yvel].posy-ps[s->yvel].oposy);
        t->z = ps[s->yvel].oposz + mulscale16(smoothratio,ps[s->yvel].posz-ps[s->yvel].oposz);
        t->z += (40<<8);
    }
    else if( ( s->statnum == 0 && s->picnum != CRANEPOLE) || s->statnum == 10 || s->statnum == 6 || s->statnum == 4 || s->statnum == 5 || s->statnum == 1 )
    {
        t->x -= mulscale16(65536-smoothratio,s->x-hittype[i].bposx);
        t->y -= mulscale16(65536-smoothratio,s->y-hittype[i].bposy);
        t->z -= mulscale16(65536-smoothratio,s->z-hittype[i].bposz);
    }

    sect = s->sectnum;
    t1 = T2;t3 = T4;t4 = T5;

    switch(s->picnum)
    {
        case DUKELYINGDEAD:
            t->z += (24<<8);
            break;
        case BLOODPOOL:
        case FOOTPRINTS:
        case FOOTPRINTS2:
        case FOOTPRINTS3:
        case FOOTPRINTS4:
            if(t->pal == 6)
                t->shade = -127;
        case PUKE:
        case MONEY:
        case MONEY+1:
        case MAIL:
        case MAIL+1:
        case PAPER:
        case PAPER+1:
            if(ud.lockout && s->pal == 2)
            {
                t->xrepeat = t->yrepeat = 0;
                return;
            }
            break;
        case TRIPBOMB:
                return;
        case FORCESPHERE:
            if(t->statnum == 5)
            {
                short sqa,sqb;

                sqa =
                    getangle(
                        sprite[s->owner].x-ps[screenpeek].posx,
                        sprite[s->owner].y-ps[screenpeek].posy);
                sqb =
                    getangle(
                        sprite[s->owner].x-t->x,
                        sprite[s->owner].y-t->y);

                if( klabs2(getincangle(sqa,sqb)) > 512 )
                    if( ldist(&sprite[s->owner],t) < ldist(&sprite[ps[screenpeek].i],&sprite[s->owner]) )
                        t->xrepeat = t->yrepeat = 0;
            }
            return;
        case BURNING:
        case BURNING2:
            if( sprite[s->owner].statnum == 10 )
            {
                if( display_mirror == 0 && sprite[s->owner].yvel == screenpeek && ps[sprite[s->owner].yvel].over_shoulder_on == 0 )
                    t->xrepeat = 0;
                else
                {
                    t->ang = getangle(x-t->x,y-t->y);
                    t->x = sprite[s->owner].x;
                    t->y = sprite[s->owner].y;
                    t->x += sintable[(t->ang+512)&2047]>>10;
                    t->y += sintable[t->ang&2047]>>10;
                }
            }
            break;

        case ATOMICHEALTH:
            t->z -= (4<<8);
            break;
        case CRYSTALAMMO:
        case GROWAMMO:
            t->shade = (sintable[(totalclock<<4)&2047]>>10);
            return;
        case VIEWSCREEN:
        case VIEWSCREEN2:
            if(camsprite >= 0 && hittype[OW].temp_data[0] == 1)
            {
                t->picnum = STATIC;
                t->cstat |= (krand2()&12);
                t->xrepeat += 8;
                t->yrepeat += 8;
            }
            break;

        case SHRINKSPARK:
        case GROWSPARK:
            t->picnum = t->picnum+( (totalclock>>4)&3 );
            break;
        case TILE3841:
            t->xrepeat = t->yrepeat = (t->yvel + 100) / 4;
            break;
        case TILE3634:
            k = t->xvel == 0 ? 0 : (totalclock>>4)%5;
            if (t->zvel)
            {
                if (k > 2)
                {
                    k = 5-k;
                    t->cstat |= 8;
                }
                t->picnum = TILE3634+k;
            }
            else
            {
                if (k > 2)
                {
                    k = 5-k;
                    t->cstat |= 4;
                }
                t->picnum = TILE3831+k;
            }
            break;
        case RPG:
             k = getangle(s->x-x,s->y-y);
             k = (((s->ang+3072+128-k)&2047)/170);
             if(k > 6)
             {
                k = 12-k;
                t->cstat |= 4;
             }
             else t->cstat &= ~4;
             t->picnum = RPG+k;
             break;

        case RECON:

            k = getangle(s->x-x,s->y-y);
            if( T1 < 4 )
                k = (((s->ang+3072+128-k)&2047)/170);
            else k = (((s->ang+3072+128-k)&2047)/170);

            if(k>6)
            {
                k = 12-k;
                t->cstat |= 4;
            }
            else t->cstat &= ~4;

            if( klabs2(t3) > 64 ) k += 7;
            t->picnum = RECON+k;

            break;

        case APLAYER:

            p = s->yvel;

            if(s->owner == -1)
            {
                k = (((s->ang+3072+128-a)&2047)>>8)&7;
                if(k>4)
                {
                    k = 8-k;
                    t->cstat |= 4;
                }
                else t->cstat &= ~4;

                if(sector[t->sectnum].lotag == 2) k += 1795-1405;
                else if( (hittype[i].floorz-s->z) > (64<<8) ) k += 60;

                t->picnum += k;
                t->pal = ps[p].palookup;

                goto PALONLY;
            }

            if( ps[p].on_crane == -1 && (sector[s->sectnum].lotag&0x7ff) != 1 )
            {
                l = s->z-hittype[ps[p].i].floorz+(3<<8);
                if( l > 1024 && s->yrepeat > 32 && s->extra > 0 )
                    s->yoffset = (signed char)(l/(s->yrepeat<<2));
                else s->yoffset=0;
            }

            if(ps[p].newowner > -1)
            {
                t4 = script[actorscrptr[APLAYER]+1];
                t3 = 0;
                t1 = script[actorscrptr[APLAYER]+2];
            }

            if(p == splitscreenpeek && ud.camerasprite == -1 && ps[p].newowner == -1)
                if(s->owner >= 0 && display_mirror == 0 && ps[p].over_shoulder_on == 0 )
            {
                t->owner = -1;
                t->xrepeat = t->yrepeat = 0;
                return;
            }

            PALONLY:

            if( sector[sect].floorpal )
                t->pal = sector[sect].floorpal;

            if(s->owner == -1) return;

            if( t->z > hittype[i].floorz && t->xrepeat < 32 )
                t->z = hittype[i].floorz;

            break;

        case JIBS1:
        case JIBS2:
        case JIBS3:
        case JIBS4:
        case JIBS5:
        case JIBS6:
        case HEADJIB1:
        case LEGJIB1:
        case ARMJIB1:
        case LIZMANHEAD1:
        case LIZMANARM1:
        case LIZMANLEG1:
        case DUKELEG:
        case DUKEGUN:
        case DUKETORSO:
            if(ud.lockout)
            {
                t->xrepeat = t->yrepeat = 0;
                return;
            }
            if(t->pal == 6) t->shade = -120;

        case SCRAP1:
        case SCRAP2:
        case SCRAP3:
        case SCRAP4:
        case SCRAP5:
        case SCRAP6:
        case SCRAP6+1:
        case SCRAP6+2:
        case SCRAP6+3:
        case SCRAP6+4:
        case SCRAP6+5:
        case SCRAP6+6:
        case SCRAP6+7:

            if(hittype[i].picnum == BLIMP && t->picnum == SCRAP1 && s->yvel >= 0)
                t->picnum = s->yvel;
            else t->picnum += T1;
            t->shade -= 6;

            if( sector[sect].floorpal )
                t->pal = sector[sect].floorpal;
            break;

        case WATERBUBBLE:
            if(sector[t->sectnum].floorpicnum == FLOORSLIME)
            {
                t->pal = 7;
                break;
            }
        case ACCESSCARD:
            switch(t->pal)
            {
                case 21:
                    t->picnum = TILE65;
                    break;
                case 23:
                    t->picnum = TILE66;
                    break;
            }
            t->pal = 0;
            break;
        case ACCESSSWITCH:
            switch(t->pal)
            {
                case 21:
                    t->picnum = TILE172;
                    break;
                case 23:
                    t->picnum = TILE174;
                    break;
            }
            t->pal = 0;
            break;
        case ACCESSSWITCH+1:
            switch(t->pal)
            {
                case 21:
                    t->picnum = TILE172+1;
                    break;
                case 23:
                    t->picnum = TILE174+1;
                    break;
            }
            t->pal = 0;
            break;
        case ACCESSSWITCH2:
            switch(t->pal)
            {
                case 21:
                    t->picnum = TILE176;
                    break;
                case 23:
                    t->picnum = TILE178;
                    break;
            }
            t->pal = 0;
            break;
        case ACCESSSWITCH2+1:
            switch(t->pal)
            {
                case 21:
                    t->picnum = TILE176+1;
                    break;
                case 23:
                    t->picnum = TILE178+1;
                    break;
            }
            t->pal = 0;
            break;
        case LASERLINE:
            t->picnum = TILE3922 + (totalclock/4) % 3;
            break;
        default:

            if( sector[sect].floorpal )
                t->pal = sector[sect].floorpal;
            break;
    }

    if( actorscrptr[s->picnum] )
    {
        if((unsigned)t4 >= 70)
        {
            l = *(long *)(t4+8);

            switch( l )
            {
                case 2:
                    k = (((s->ang+3072+128-a)&2047)>>8)&1;
                    break;

                case 3:
                case 4:
                    k = (((s->ang+3072+128-a)&2047)>>7)&7;
                    if(k > 3)
                    {
                        t->cstat |= 4;
                        k = 7-k;
                    }
                    else t->cstat &= ~4;
                    break;

                case 5:
                    k = getangle(s->x-x,s->y-y);
                    k = (((s->ang+3072+128-k)&2047)>>8)&7;
                    if(k>4)
                    {
                        k = 8-k;
                        t->cstat |= 4;
                    }
                    else t->cstat &= ~4;
                    break;
                case 7:
                    k = getangle(s->x-x,s->y-y);
                    k = (((s->ang+3072+128-k)&2047)/170);
                    if(k>6)
                    {
                        k = 12-k;
                        t->cstat |= 4;
                    }
                    else t->cstat &= ~4;
                    break;
                case 8:
                    k = (((s->ang+3072+128-a)&2047)>>8)&7;
                    t->cstat &= ~4;
                    break;
                default:
                    k = 0;
                    break;
            }

            t->picnum += k + ( *(long *)t4 ) + l * t3;

            if(l > 0) while(gettilesizx(t->picnum) == 0 && t->picnum > 0 )
                t->picnum -= l;       //Hack, for actors

            if( hittype[i].dispicnum >= 0)
                hittype[i].dispicnum = t->picnum;
        }
        else if(display_mirror == 1)
            t->cstat |= 4;
    }

    if( s->statnum == 13 || badguy(s) || (s->picnum == APLAYER && s->owner >= 0) )
        if(t->statnum != 99 && s->picnum != EXPLOSION2 && s->picnum != HANGLIGHT && s->picnum != DOMELITE)
            if(s->picnum != HOTMEAT)
    {
        if( hittype[i].dispicnum < 0 )
        {
            hittype[i].dispicnum++;
            return;
        }

        if( ps[splitscreenpeek].heat_amount > 0 && ps[splitscreenpeek].heat_on )
        {
            //t->pal = 6;
            t->shade = -127;
        }
    }


    switch(s->picnum)
    {
        case LASERLINE:
            if(sector[t->sectnum].lotag == 2) t->pal = 8;
            t->z = sprite[s->owner].z-(3<<8);
            if(lasermode == 2 && ps[splitscreenpeek].heat_on == 0 )
                t->yrepeat = 0;
        case EXPLOSION2:
        case ATOMICHEALTH:
        case FIRELASER:
        case SHRINKSPARK:
        case GROWSPARK:
        case CHAINGUN:
        case SHRINKEREXPLOSION:
        case RPG:
        case FLOORFLAME:
        case TILE3841:
            if(t->picnum == EXPLOSION2)
            {
                ps[screenpeek].visibility = -127;
                //lastvisinc = totalclock+32;
                //restorepalette = 1;
            }
            t->shade = -127;
            break;
        case FIRE:
        case FIRE2:
        case BURNING:
        case BURNING2:
            if( sprite[s->owner].picnum != TREE1 && sprite[s->owner].picnum != TREE2 )
                t->z = sector[t->sectnum].floorz;
            t->shade = -127;
            break;
        case COOLEXPLOSION1:
            t->shade = -127;
            t->picnum += (s->shade>>1);
            break;
        case PLAYERONWATER:

            k = (((t->ang+3072+128-a)&2047)>>8)&7;
            if(k>4)
            {
                k = 8-k;
                t->cstat |= 4;
            }
            else t->cstat &= ~4;

            t->picnum = s->picnum+k+((T1<4)*5);
            t->shade = sprite[s->owner].shade;

            break;

        case WATERSPLASH2:
            t->picnum = WATERSPLASH2+t1;
            break;
        case REACTOR2:
            t->picnum = s->picnum + T3;
            break;
        case SHELL:
            t->picnum = s->picnum+(T1&1);
        case SHOTGUNSHELL:
            t->cstat |= 12;
            if(T1 > 1) t->cstat &= ~4;
            if(T1 > 2) t->cstat &= ~12;
            break;
        case FRAMEEFFECT1:
            if(s->owner >= 0 && sprite[s->owner].statnum < MAXSTATUS)
            {
                if(sprite[s->owner].picnum == APLAYER)
                    if(splitscreenpeek == sprite[s->owner].yvel)
                {
                    t->owner = -1;
                    break;
                }
                if( (sprite[s->owner].cstat&32768) == 0 )
                {
                    t->picnum = hittype[s->owner].dispicnum;
                    t->pal = sprite[s->owner].pal;
                    t->shade = sprite[s->owner].shade;
                    t->ang = sprite[s->owner].ang;
                    t->cstat = 2|sprite[s->owner].cstat;
                }
            }
            break;
        
        case CAMERA1:
        case RAT:
            k = (((t->ang+3072+128-a)&2047)>>8)&7;
            if(k>4)
            {
                k = 8-k;
                t->cstat |= 4;
            }
            else t->cstat &= ~4;
            t->picnum = s->picnum+k;
            break;
    }

    hittype[i].dispicnum = t->picnum;
    if(sector[t->sectnum].floorpicnum == MIRROR)
        t->xrepeat = t->yrepeat = 0;
}


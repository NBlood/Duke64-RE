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

// Savage Baggage Masters

#include "common.h"
#include "duke3d.h"

int D_800FF620 = 0;
short numplayers = 1;
short myconnectindex = 0;

int32 turnheldtime; //MED
int32 lastcontroltime; //MED
int D_80279758;
long lastvisinc;
short fistsign;
short stickx, sticky;
unsigned short buttonheld, button_mode;

void incur_damage( struct player_struct *p )
{
    long  damage = 0L, shield_damage = 0L;
    short i, damage_source;

    sprite[p->i].extra -= p->extra_extra8>>8;

    damage = sprite[p->i].extra - p->last_extra;

    if ( damage < 0 )
    {
        p->extra_extra8 = 0;

        if (ud.player_skill == 1)
            damage = min(-1, damage / 2);

        if (sprite[p->i].yvel < numplayers && damage < -3)
            controllersetvib(sprite[p->i].yvel, 5, 255);

        if ( p->shield_amount > 0 )
        {
            shield_damage =  damage * (20 + (TRAND%30)) / 100;
            damage -= shield_damage;

            p->shield_amount += shield_damage;

            if ( p->shield_amount < 0 )
            {
                damage += p->shield_amount;
                p->shield_amount = 0;
            }
        }

        sprite[p->i].extra = p->last_extra + damage;
    }
}

void quickkill(struct player_struct *p)
{
    if (sprite[p->i].extra <= 0)
        return;
    p->pals[0] = 48;
    p->pals[1] = 48;
    p->pals[2] = 48;
    p->pals_time = 48;

    controllersetvib(sprite[p->i].yvel, 20, 255);

    sprite[p->i].extra = 0;
    sprite[p->i].cstat |= 32768;
    if(ud.god == 0) guts(&sprite[p->i],JIBS6,8,myconnectindex);
    return;
}

void forceplayerangle(struct player_struct *p)
{
    short n;

    n = 128-(TRAND&255);

    p->horiz += 64;
    p->return_to_center = 9;
    p->look_ang = n>>1;
    p->rotscrnang = n>>1;
}

void tracers(long x1,long y1,long z1,long x2,long y2,long z2,long p)
{
     long i, xv, yv, zv;
     short sect = -1;

     if (p == -1)
         return;

     sect = ps[p].cursectnum;

	 i = 9 - (ud.multimode >> 1);
	 xv = (x2-x1)/i;
	 yv = (y2-y1)/i;
	 zv = (z2-z1)/i;

     if( ( klabs2(x1-x2)+klabs2(y1-y2) ) < 3084 )
         return;

	 for(i=8-(ud.multimode >> 1);i>0;i--)
	 {
		  x1 += xv;
		  y1 += yv;
		  z1 += zv;
		  updatesector(x1,y1,&sect);
          if(sect >= 0)
          {
              if (ps[p].curr_weapon == GROW_WEAPON)
              {
                  int s = EGS(sect,x1,y1,z1,GROWSPARK,-16,1,1,0,0,0,ps[p].i,1);
                  sprite[s].cstat |= 130;
              }
              else if(sector[sect].lotag == 2)
                  EGS(sect,x1,y1,z1,WATERBUBBLE,-32,4+(TRAND&3),4+(TRAND&3),TRAND&2047,0,0,ps[p].i,5);
              else
                  EGS(sect,x1,y1,z1,SMALLSMOKE,-32,14,14,0,0,0,ps[p].i,5);
          }
	 }
}

long hits(short i)
{
    long sx,sy,sz;
    short sect,hw,hs;
    long zoff;

    if(PN == APLAYER) zoff = (40<<8);
    else zoff = 0;

    hitscan(SX,SY,SZ-zoff,SECT,
        sintable[(SA+512)&2047],
        sintable[SA&2047],
        0,&sect,&hw,&hs,&sx,&sy,&sz,CLIPMASK1);

    return ( FindDistance2D( sx-SX,sy-SY ) );
}

long hitasprite(short i,short *hitsp)
{
    long sx,sy,sz,zoff;
    short sect,hw;

    if(badguy(&sprite[i]) )
        zoff = (42<<8);
    else if(PN == APLAYER) zoff = (39<<8);
    else zoff = 0;

    hitscan(SX,SY,SZ-zoff,SECT,
        sintable[(SA+512)&2047],
        sintable[SA&2047],
        0,&sect,&hw,hitsp,&sx,&sy,&sz,CLIPMASK1);

    if(hw >= 0 && (wall[hw].cstat&16) && badguy(&sprite[i]) )
        return((1<<30));

    return ( FindDistance2D(sx-SX,sy-SY) );
}

/*
long hitaspriteandwall(short i,short *hitsp,short *hitw,short *x, short *y)
{
    long sz;
    short sect;

    hitscan(SX,SY,SZ,SECT,
        sintable[(SA+512)&2047],
        sintable[SA&2047],
        0,&sect,hitw,hitsp,x,y,&sz,CLIPMASK1);

    return ( FindDistance2D(*x-SX,*y-SY) );
}
*/


long hitawall(struct player_struct *p,short *hitw)
{
    long sx,sy,sz;
    short sect,hs;

    hitscan(p->posx,p->posy,p->posz,p->cursectnum,
        sintable[(p->ang+512)&2047],
        sintable[p->ang&2047],
        0,&sect,hitw,&hs,&sx,&sy,&sz,CLIPMASK0);

    return ( FindDistance2D(sx-p->posx,sy-p->posy) );
}

short aim(spritetype *s,short aang)
{
    char gotshrinker;
    short i, j, a, k, cans;
    short aimstats[] = {10,13,1,2};
    long dx1, dy1, dx2, dy2, dx3, dy3, smax, sdist;
    long xv, yv;
    if(s->picnum == APLAYER && ps[s->yvel].autoaim == 0) return -1;

    a = s->ang;

    j = -1;

    gotshrinker = s->picnum == APLAYER && ps[s->yvel].curr_weapon == SHRINKER_WEAPON;

    smax = 0x7fffffff;

    dx1 = sintable[(a+512-aang)&2047];
    dy1 = sintable[(a-aang)&2047];
    dx2 = sintable[(a+512+aang)&2047];
    dy2 = sintable[(a+aang)&2047];

    dx3 = sintable[(a+512)&2047];
    dy3 = sintable[a&2047];

    for(k=0;k<4;k++)
    {
        if( j >= 0 )
            break;
        for(i=headspritestat[aimstats[k]];i >= 0;i=nextspritestat[i])
            if( sprite[i].xrepeat > 0 && sprite[i].extra >= 0 && (sprite[i].cstat&(257+32768)) == 257)
                if( badguy(&sprite[i]) || k < 2 )
            {
                if(badguy(&sprite[i]) || PN == APLAYER || PN == SHARK)
                {
                    if( PN == APLAYER &&
//                        ud.ffire == 0 &&
                        ud.coop == 1 &&
                        s->picnum == APLAYER &&
                        s != &sprite[i])
                            continue;

                    if(gotshrinker && sprite[i].xrepeat < 30 )
                    {
                        switch(PN)
                        {
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
                                continue;
                        }
                    }
                }

                xv = (SX-s->x);
                yv = (SY-s->y);

                if( (dy1*xv) <= (dx1*yv) )
                    if( ( dy2*xv ) >= (dx2*yv) )
                {
                    sdist = mulscale(dx3,xv,14) + mulscale(dy3,yv,14);
                    if( sdist > 512 && sdist < smax )
                    {
                        if(s->picnum == APLAYER)
                        {
                            if (ps[s->yvel].autoaim == 1)
                            {
                                float ta;
                                float dz;
                                viewhorizang = getanglef(ps[s->yvel].horiz+ps[s->yvel].horizoff-100, 128.0);
                                viewhorizang = viewhorizang * 1024.0 / 3.1415927410125732;
                                if (PN == ROTATEGUN)
                                    dz = (SZ - s->z +  0x2000) * 0.0625;
                                else
                                    dz = (SZ - s->z) * 0.0625;
                                ta = getanglef(-dz, sdist);
                                ta = ta * 1024.0 / 3.1415927410125732;
                                if (ta < aang + viewhorizang && ta > viewhorizang - aang)
                                    a = 1;
                                else
                                    a = 0;
                            }
                            else
                                a = (klabs2(scale(SZ-s->z,10,sdist)-(ps[s->yvel].horiz+ps[s->yvel].horizoff-100)) < 100);
                        }
                        else a = 1;

                        if(PN == ROTATEGUN )
                            cans = cansee(SX,SY,SZ,SECT,s->x,s->y,s->z-(32<<8),s->sectnum);
                        else cans = cansee(SX,SY,SZ-(32<<8),SECT,s->x,s->y,s->z-(32<<8),s->sectnum);

                        if( a && cans )
                        {
                            smax = sdist;
                            j = i;
                        }
                    }
                }
            }
    }

    return j;
}

int BUTTON(int a)
{
    switch (a & 0xFFFF0000) {                              /* irregular */
    case 0x0:
        if (buttonheld & a & 0xFFFF) {
            return -1;
        }
        return 0;
    case 0x20000:
        if ((buttonheld & a & 0xFFFF) && !(buttonheld & button_mode)) {
            return -1;
        }
        return 0;
    case 0x10000:
        if ((buttonheld & a & 0xFFFF) && (buttonheld & button_mode)) {
            return -1;
        }
        return 0;
    default:
        return 0;
    }
}

extern int lastcontroltime;

#define TURBOTURNTIME (TICRATE/8) // 7
#define NORMALTURN   15
#define NORMALKEYMOVE 40
#define MAXVEL       ((NORMALKEYMOVE*2)+10)
#define MAXSVEL      ((NORMALKEYMOVE*2)+10)
#define MAXANGVEL    127
#define MAXHORIZ     127

void getinput(short snum)
{
    short j, daang;
    short s;
    int32 tics;
    int32 turnamount;
    int32 keymove;
    int32 momx,momy;
    struct player_struct* p;
    controlmap_t *cm;
    
    momx = momy = 0;
    p = &ps[snum];
    cm = &controlmap[snum];
    
    stickx = control_stickx[snum];
    sticky = control_sticky[snum];
    buttonheld = control_button[snum];
    button_mode = cm->buttonmode;
    if (game_inactive)
    {
        demoinput();
        stickx = demo_stickx;
        sticky = demo_sticky;
        buttonheld = demo_button;
    }
    tics = totalclock- lastcontroltime;
    lastcontroltime = totalclock;

    loc.bits = 0;
    if (p->fist_incs)
        return;
    if ((ud.multimode > 1 && ud.coop == 0 && ps[snum].last_extra <= 0 && dukematch_mode == 4))
        return;
    
    loc.bits =   (BUTTON(cm->jump) < 0);
    loc.bits |=   (BUTTON(cm->crouch) < 0)<<1;
    loc.bits |=   (BUTTON(cm->fire) < 0)<<2;
    loc.bits |=   (BUTTON(cm->aim_up) < 0)<<3;
    loc.bits |=   (BUTTON(cm->aim_down) < 0)<<4;
    
    j=0;
    if (BUTTON(cm->next_weapon) < 0)
        j = 12;
    if (BUTTON(cm->prev_weapon) < 0)
        j = 11;
    
    loc.bits |=   j<<8;
    loc.bits |=   (BUTTON(cm->open) < 0)<<29;
    loc.bits |=   (BUTTON(cm->inv_left) < 0)<<20;
    loc.bits |=   (BUTTON(cm->inv_right) < 0)<<27;
    loc.bits |=   (BUTTON(cm->inventory) < 0)<<30;
                    
    svel = vel = angvel = 0;
    horiz = sync[snum].horz;

    if (1) {}

    if (klabs2(stickx) < 8)
        stickx = 0;
    if (klabs2(sticky) < 8)
        sticky = 0;

    angvel = stickx / 3;

    if (p->aim_mode == 0)
        vel = sticky * 2;
    else
    {
        s = horiz - (-sticky * 1.25);
        if (s > 25)
            s = 25;
        if (s < -25)
            s = -25;
        horiz -= s;
    }

    turnamount = NORMALTURN<<1;
    keymove = NORMALKEYMOVE<<1;
    
    if (BUTTON(cm->strafe_left) < 0)
        svel += keymove;
    if (BUTTON(cm->strafe_right) < 0)
        svel -= keymove;
    if (BUTTON(cm->forward) < 0)
        vel += keymove;
    if (BUTTON(cm->backward) < 0)
        vel -= keymove;

    if(vel < -MAXVEL) vel = -MAXVEL;
    if(vel > MAXVEL) vel = MAXVEL;
    if(svel < -MAXSVEL) svel = -MAXSVEL;
    if(svel > MAXSVEL) svel = MAXSVEL;
    if(angvel < -MAXANGVEL) angvel = -MAXANGVEL;
    if(angvel > MAXANGVEL) angvel = MAXANGVEL;
    if(horiz < -MAXHORIZ) horiz = -MAXHORIZ;
    if(horiz > MAXHORIZ) horiz = MAXHORIZ;

    if(ud.scrollmode && ud.overhead_on)
    {
        ud.folfvel = vel;
        ud.folavel = angvel;
        loc.fvel = 0;
        loc.svel = 0;
        loc.avel = 0;
        loc.horz = 0;
        return;
    }

    daang = p->ang;

    momx = mulscale9(vel,sintable[(daang+2560)&2047]);
    momy = mulscale9(vel,sintable[(daang+2048)&2047]);

    momx += mulscale9(svel,sintable[(daang+2048)&2047]);
    momy += mulscale9(svel,sintable[(daang+1536)&2047]);

    momx += p->fricxv;
    momy += p->fricyv;

    loc.fvel = momx;
    loc.svel = momy;
    loc.avel = angvel;
    loc.horz = horiz;
}

char doincrements(struct player_struct *p)
{
    long /*j,*/i,snum;

    snum = sprite[p->i].yvel;
//    j = sync[snum].avel;
//    p->weapon_ang = -(j/5);

    p->player_par++;

    if(p->invdisptime > 0)
        p->invdisptime--;

    if(p->tipincs > 0) p->tipincs--;

    if(p->last_pissed_time > 0 )
    {
        p->last_pissed_time--;

        if( p->last_pissed_time == (26*219) )
        {
            spritesound(FLUSH_TOILET,p->i);
            if(snum == screenpeek || ud.coop == 1)
                spritesound(DUKE_PISSRELIEF,p->i);
        }

        if( p->last_pissed_time == (26*218) )
        {
            p->holster_weapon = 0;
            p->weapon_pos = 10;
        }
    }

    if(p->crack_time > 0)
    {
        p->crack_time--;
        if(p->crack_time == 0)
        {
            p->knuckle_incs = 1;
            p->crack_time = 777 + (TRAND & 255);
        }
    }

    if( p->steroids_amount > 0 && p->steroids_amount < 400)
    {
        p->steroids_amount--;
        if(p->steroids_amount == 0)
            checkavailinven(p);
        if( !(p->steroids_amount&7) )
            if(snum == screenpeek || ud.coop == 1)
                spritesound(DUKE_HARTBEAT,p->i);
    }

    if(p->heat_on && p->heat_amount > 0)
    {
        p->heat_amount--;
        if( p->heat_amount == 0 )
        {
            p->heat_on = 0;
            checkavailinven(p);
            spritesound(NITEVISION_ONOFF,p->i);
            //setpal(p);
        }
    }

    if( p->holoduke_on >= 0 )
    {
        p->holoduke_amount--;
        if(p->holoduke_amount <= 0)
        {
            spritesound(TELEPORTER,p->i);
            p->holoduke_on = -1;
            checkavailinven(p);
        }
    }

    if( p->jetpack_on && p->jetpack_amount > 0 )
    {
        p->jetpack_amount--;
        if(p->jetpack_amount <= 0)
        {
            p->jetpack_on = 0;
            checkavailinven(p);
            spritesound(DUKE_JETPACK_OFF,p->i);
            stopsound(DUKE_JETPACK_IDLE);
            stopsound(DUKE_JETPACK_ON);
        }
    }

    if(p->quick_kick > 0 && sprite[p->i].pal != 1)
    {
        p->quick_kick--;
        if( p->quick_kick == 8 )
            shoot(p->i,KNEE);
    }

    if(p->access_incs && sprite[p->i].pal != 1)
    {
        if (p->f_388 > 0)
        {
            FX_StopSound(p->f_388);
            p->f_388 = 0;
        }
        p->access_incs++;
        if(sprite[p->i].extra <= 0)
            p->access_incs = 12;
        if(p->access_incs == 12)
        {
            if(p->access_spritenum >= 0)
            {
                checkhitswitch(snum,p->access_spritenum,1);
                switch(sprite[p->access_spritenum].pal)
                {
                    case 0:p->got_access &= (0xffff-0x1);break;
                    case 21:p->got_access &= (0xffff-0x2);break;
                    case 23:p->got_access &= (0xffff-0x4);break;
                }
                p->access_spritenum = -1;
            }
            else
            {
                checkhitswitch(snum,p->access_wallnum,0);
                switch(wall[p->access_wallnum].pal)
                {
                    case 0:p->got_access &= (0xffff-0x1);break;
                    case 21:p->got_access &= (0xffff-0x2);break;
                    case 23:p->got_access &= (0xffff-0x4);break;
                }
            }
        }

        if(p->access_incs > 20)
        {
            p->access_incs = 0;
            p->weapon_pos = 10;
            p->kickback_pic = 0;
        }
    }

    if(p->scuba_on == 0 && sector[p->cursectnum].lotag == 2)
    {
        if(p->scuba_amount > 0)
        {
            p->scuba_on = 1;
            p->inven_icon = 6;
            FTA(76,p);
        }
        else
        {
            if(p->airleft > 0)
                p->airleft--;
            else
            {
                p->extra_extra8 += 32;
                if(p->last_extra < (max_player_health>>1) && (p->last_extra&3) == 0)
                    spritesound(DUKE_LONGTERM_PAIN,p->i);
            }
        }
    }
    else if(p->scuba_amount > 0 && p->scuba_on)
    {
        p->scuba_amount--;
        if(p->scuba_amount == 0)
        {
            p->scuba_on = 0;
            checkavailinven(p);
        }
    }

    if(p->knuckle_incs)
    {
        p->knuckle_incs ++;
        if(p->knuckle_incs==10)
        {
            if(totalclock > 1024)
                if((TRAND & 3) == 0)
            {
                if(TRAND&1)
                    spritesound(DUKE_CRACK2,p->i);
                else spritesound(DUKE_CRACK,p->i);
            }
            spritesound(DUKE_CRACK_FIRST,p->i);
        }
        else if( p->knuckle_incs == 22 || (sync[snum].bits&(1<<2)))
            p->knuckle_incs=0;

        return 1;
    }
    return 0;
}

extern short weapon_sprites[];

void checkweapons(struct player_struct *p)
{
    short j,cw;

    cw = p->curr_weapon;

    if(cw < 1 || cw >= MAX_WEAPONS) return;

    if(cw)
    {
        if(TRAND&1)
        {
            cw = weapon_sprites[cw];
            if (cw == GROWSPRITEICON) cw = SHRINKERSPRITE;
            spawn(p->i,cw);
        }
        else switch(cw)
        {
            case SHOTGUN2_WEAPON:
            case DEVISTATOR_WEAPON:
            case RPG_WEAPON:
            case HANDBOMB_WEAPON:
                spawn(p->i,EXPLOSION2);
                break;
        }
    }
}

#ifdef NON_MATCHING
void processinput(short snum)
{
    long j, i, k, doubvel, fz, cz, hz, lz, truefdist, x, y;
    char shrunk;
    unsigned long sb_snum;
    short psect, psectlotag,*kb, tempsect, pi;
    struct player_struct *p;
    spritetype *s;

    p = &ps[snum];
    pi = p->i;
    s = &sprite[pi];

    kb = &p->kickback_pic;

    sb_snum = sync[snum].bits;

    if (sb_snum)
        p->crack_time = 777 + (TRAND&255);

    psect = p->cursectnum;
    if(psect == -1)
    {
        if(s->extra > 0 && ud.clipping == 0)
        {
            quickkill(p);
            spritesound(SQUISHED,pi);
        }
        psect = 0;
    }

    psectlotag = sector[psect].lotag;
    p->spritebridge = 0;

    shrunk = (s->yrepeat < 32);
    getzrange(p->posx,p->posy,p->posz,psect,&cz,&hz,&fz,&lz,163L,CLIPMASK0);

    j = getflorzofslope(psect,p->posx,p->posy);

    p->truefz = j;
    p->truecz = getceilzofslope(psect,p->posx,p->posy);

    truefdist = klabs2(p->posz-j);
    if( (lz&49152) == 16384 && psectlotag == 1 && truefdist > PHEIGHT+(16<<8) )
        psectlotag = 0;

    hittype[pi].floorz = fz;
    hittype[pi].ceilingz = cz;

    p->ohoriz = p->horiz;
    p->ohorizoff = p->horizoff;

    if( p->aim_mode == 0 && p->on_ground && psectlotag != 2 && (sector[psect].floorstat&2) )
    {
          x = p->posx+(sintable[(p->ang+512)&2047]>>5);
          y = p->posy+(sintable[p->ang&2047]>>5);
          tempsect = psect;
          updatesector(x,y,&tempsect);
          if (tempsect >= 0)
          {
              k = getflorzofslope(psect,x,y);
              if (psect == tempsect)
                  p->horizoff += mulscale16(j-k,160);
              else if (klabs2(getflorzofslope(tempsect,x,y)-k) <= (4<<8))
                  p->horizoff += mulscale16(j-k,160);
          }
     }
     if (p->horizoff > 0) p->horizoff -= ((p->horizoff>>3)+1);
     else if (p->horizoff < 0) p->horizoff += (((-p->horizoff)>>3)+1);

    if( hz >= 0 && (hz&49152) == 49152)
    {
        hz &= 0xfff;
        if (hz >= MAXSPRITES) gameexit("Sprite overflow caused by LEMMING\n");

        if(sprite[hz].statnum == 1 && sprite[hz].extra >= 0)
        {
            hz = 0;
            cz = p->truecz;
        }
    }

    if(lz >= 0 && (lz&49152) == 49152)
    {
        j = lz&0xfff;
        if (j >= MAXSPRITES) gameexit("Sprite overflow caused by LEMMING\n");

        if( (sprite[j].cstat&33) == 33 )
        {
            psectlotag = 0;
            p->footprintcount = 0;
            p->spritebridge = 1;
        }
        else if(badguy(&sprite[j]) && sprite[j].xrepeat > 24 && klabs2(s->z-sprite[j].z) < (84<<8) )
        {
            j = getangle(sprite[j].x-p->posx,sprite[j].y-p->posy);
            p->posxv -= sintable[(j+512)&2047]<<4;
            p->posyv -= sintable[j&2047]<<4;
        }
    }


    if ( s->extra > 0 ) incur_damage( p );
    else
    {
        s->extra = 0;
        p->shield_amount = 0;
    }

    p->last_extra = s->extra;

    if(p->loogcnt > 0) p->loogcnt--;
    else p->loogcnt = 0;

    if(p->fist_incs)
    {
        p->fist_incs++;
        if(p->fist_incs == 28)
        {
            sound(PIPEBOMB_EXPLODE);
            p->pals[0] = 64;
            p->pals[1] = 64;
            p->pals[2] = 64;
            p->pals_time = 48;
        }
        if(p->fist_incs > 42)
        {
            p->timebeforeexit = 17;
            p->fist_incs = 0;

            return;
        }
    }

    if(p->timebeforeexit > 1 && (p->last_extra > 0 || ud.multimode > 1))
    {
        p->timebeforeexit--;
        if(p->timebeforeexit < 19)
        {
            for(i=0;i<ud.multimode;i++)
            {
                short t;
                death_fade = 1;
                ps[i].pals[0] = (char)max(0, ps[i].pals[0] - 6);
                ps[i].pals[1] = (char)max(0, ps[i].pals[1] - 6);
                ps[i].pals[2] = (char)max(0, ps[i].pals[2] - 6);
                t = 73 - p->timebeforeexit * 4;
                ps[i].pals_time = max(ps[i].pals_time, t);
            }
        }
        if(p->timebeforeexit == 26*5)
        {
            if(p->customexitsound >= 0)
            {
                sound(0x9c);
                FTA(102,p);
            }
        }
        else if(p->timebeforeexit == 1)
        {
            stopallsounds();
            for(i=0;i<ud.multimode;i++)
                ps[i].gm = MODE_EOL;
            if(ud.from_bonus)
            {
                ud.level_number = ud.from_bonus;
                ud.m_level_number = ud.level_number;
                ud.from_bonus = 0;
            }
            else
            {
                ud.level_number++;
                ud.m_level_number = ud.level_number;
            }
            return;
        }
    }
/*
    if(p->select_dir)
    {
        if(psectlotag != 15 || (sb_snum&(1<<31)) )
            p->select_dir = 0;
        else
        {
            if(sync[snum].fvel > 127)
            {
                p->select_dir = 0;
                activatewarpelevators(pi,-1);
            }
            else if(sync[snum].fvel <= -127)
            {
                p->select_dir = 0;
                activatewarpelevators(pi,1);
            }
            return;
        }
    }
  */
    if(p->pals_time > 0)
        p->pals_time--;
    else
    {
        death_fade = 0;
        p->pals[0] = 0;
        p->pals[1] = 0;
        p->pals[2] = 0;
    }

    if(p->fta > 0)
    {
        p->fta--;
        if(p->fta == 0)
        {
            pub = NUMPAGES;
            pus = NUMPAGES;
            p->ftq = 0;
        }
    }

    if( s->extra <= 0 )
    {
        p->pals_time = 64;
        p->pals[0] = max(0, p->pals[0] - 1);
        p->pals[1] = max(0, p->pals[1] - 1);
        p->pals[2] = max(0, p->pals[2] - 1);
        if(ud.multimode == 1) death_fade = 0;
        if(p->dead_flag == 0)
        {
            int ft;
            ft = p->i;
            controllersetvib(sprite[ft].yvel, 20, 255);
            p->pals[0] = 63;
            p->pals[1] = 0;
            p->pals[2] = 0;
            p->pals_time = 63;
            p->posz -= (16<<8);
            s->z -= (16<<8);

            p->dead_flag = (512-((TRAND&1)<<10)+(TRAND&255)-512)&2047;

            p->jetpack_on = 0;
            p->holoduke_on = -1;

            stopsound(DUKE_JETPACK_IDLE);
            if(p->scream_voice > 0)
            {
                FX_StopSound(p->scream_voice);
                testcallback(DUKE_SCREAM);
                p->scream_voice = 0;
            }
            if(p->f_388 > 0)
            {
                FX_StopSound(p->f_388);
                p->f_388 = 0;
            }

            if( (s->cstat&32768) == 0) s->cstat = 0;

            if( ud.multimode > 1 )
            {
                if(p->frag_ps != snum)
                {
                    ps[p->frag_ps].frag++;
                    frags[p->frag_ps][snum]++;
                    
                    sprintf(&fta_quotes[115+snum][0],"KILLED BY PLAYER %ld",1+p->frag_ps);
                    FTA(115+snum,p);
                    sprintf(&fta_quotes[115+p->frag_ps][0],"KILLED PLAYER %ld",1+snum);
                    FTA(115+p->frag_ps,&ps[p->frag_ps]);
                }
                else p->fraggedself++;

                p->frag_ps = snum;
                pus = NUMPAGES;
            }
        }

        p->on_warping_sector = 0;

        return;
    }

    if(p->transporter_hold > 0)
    {
        p->transporter_hold--;
        if(p->transporter_hold == 0 && p->on_warping_sector)
            p->transporter_hold = 2;
    }
    if(p->transporter_hold < 0)
        p->transporter_hold++;

    if(p->newowner >= 0)
    {
        p->posxv = p->posyv = s->xvel = 0;
        p->look_ang = 0;
        p->rotscrnang = 0;

        doincrements(p);

        if(p->curr_weapon == HANDREMOTE_WEAPON) goto SHOOTINCODE;

        return;
    }

    doubvel = TICSPERFRAME;

    if (p->rotscrnang > 0) p->rotscrnang -= ((p->rotscrnang>>1)+1);
    else if (p->rotscrnang < 0) p->rotscrnang += (((-p->rotscrnang)>>1)+1);

    p->look_ang -= (p->look_ang>>2);

    if( sb_snum&(1<<6) )
    {
        p->look_ang -= 152;
        p->rotscrnang += 24;
    }

    if( sb_snum&(1<<7) )
    {
        p->look_ang += 152;
        p->rotscrnang -= 24;
    }

    if(p->on_crane >= 0)
        goto HORIZONLY;

    j = ksgn(sync[snum].avel);
    /*
    if( j && ud.screen_tilting == 2)
    {
        k = 4;
        if(sb_snum&(1<<5)) k <<= 2;
        p->rotscrnang -= k*j;
        p->look_ang += k*j;
    }
    */

    if( s->xvel < 32 || p->on_ground == 0 || p->bobcounter == 1024 )
    {
        if( (p->weapon_sway&2047) > (1024+96) )
            p->weapon_sway -= 96;
        else if( (p->weapon_sway&2047) < (1024-96) )
            p->weapon_sway += 96;
        else p->weapon_sway = 1024;
    }
    else p->weapon_sway = p->bobcounter;

    s->xvel =
        ksqrt( (p->posx-p->bobposx)*(p->posx-p->bobposx)+(p->posy-p->bobposy)*(p->posy-p->bobposy));
    if(p->on_ground) p->bobcounter += sprite[p->i].xvel>>1;

    if( ud.clipping == 0 && ( sector[p->cursectnum].floorpicnum == MIRROR || p->cursectnum < 0 || p->cursectnum >= MAXSECTORS) )
    {
        p->posx = p->oposx;
        p->posy = p->oposy;
    }
    else
    {
        p->oposx = p->posx;
        p->oposy = p->posy;
    }

    p->bobposx = p->posx;
    p->bobposy = p->posy;

    p->oposz = p->posz;
    p->opyoff = p->pyoff;
    p->oang = p->ang;

    // Shrinking code

    i = 40;

    if( psectlotag == 2)
    {
        p->jumping_counter = 0;

        p->pycount += 32;
        p->pycount &= 2047;
        p->pyoff = sintable[p->pycount]>>7;

        if( Sound[DUKE_UNDERWATER].num == 0 )
            spritesound(DUKE_UNDERWATER,pi);

        if ( sb_snum&1 )
        {
            if(p->poszv > 0) p->poszv = 0;
            p->poszv -= 348;
            if(p->poszv < -(256*6)) p->poszv = -(256*6);
        }
        else if (sb_snum&(1<<1))
        {
            if(p->poszv < 0) p->poszv = 0;
            p->poszv += 348;
            if(p->poszv > (256*6)) p->poszv = (256*6);
        }
        else
        {
            if(p->poszv < 0)
            {
                p->poszv += 256;
                if(p->poszv > 0)
                    p->poszv = 0;
            }
            if(p->poszv > 0)
            {
                p->poszv -= 256;
                if(p->poszv < 0)
                    p->poszv = 0;
            }
        }

        if(p->poszv > 2048)
            p->poszv >>= 1;

        p->posz += p->poszv;

        if(p->posz > (fz-(15<<8)) )
            p->posz += ((fz-(15<<8))-p->posz)>>1;

        if(p->posz < (cz+(4<<8)) )
        {
            p->posz = cz+(4<<8);
            p->poszv = 0;
        }

        if( p->scuba_on && (TRAND&255) < 8 )
        {
            j = spawn(pi,WATERBUBBLE);
            sprite[j].x +=
                sintable[(p->ang+512+64-(global_random&128))&2047]>>6;
            sprite[j].y +=
                sintable[(p->ang+64-(global_random&128))&2047]>>6;
            sprite[j].xrepeat = 3;
            sprite[j].yrepeat = 2;
            sprite[j].z = p->posz+(8<<8);
        }
    }

    else if(p->jetpack_on)
    {
        p->on_ground = 0;
        p->jumping_counter = 0;
        p->hard_landing = 0;
        p->falling_counter = 0;

        p->pycount += 32;
        p->pycount &= 2047;
        p->pyoff = sintable[p->pycount]>>7;

        if(p->jetpack_on < 11)
        {
            p->jetpack_on++;
            p->posz -= (p->jetpack_on<<7); //Goin up
        }
        else if(p->jetpack_on == 11 && Sound[DUKE_JETPACK_IDLE].num < 1)
            spritesound(DUKE_JETPACK_IDLE,pi);

        if(shrunk) j = 512;
        else j = 2048;

        if ( sb_snum&1 )                            //A (soar high)
        {
            p->posz -= j;
            p->crack_time = 777 + (TRAND&255);
        }

        if (sb_snum&(1<<1))                            //Z (soar low)
        {
            p->posz += j;
            p->crack_time = 777 + (TRAND&255);
        }

        if( shrunk == 0 && (psectlotag == 0 || psectlotag == 2)) k = 32;
        else k = 16;

        if( psectlotag != 2 && p->scuba_on == 1 )
            p->scuba_on = 0;

        if(p->posz > (fz-(k<<8)) )
            p->posz += ((fz-(k<<8))-p->posz)>>1;
        if(p->posz < (hittype[pi].ceilingz+(18<<8)) )
            p->posz = hittype[pi].ceilingz+(18<<8);

    }
    else if( psectlotag != 2 )
    {
        if(p->airleft != 15*26)
            p->airleft = 15*26; //Aprox twenty seconds.

        if(p->scuba_on == 1)
            p->scuba_on = 0;

        if( psectlotag == 1 && p->spritebridge == 0)
        {
            if(shrunk == 0)
            {
                i = 34;
                p->pycount += 32;
                p->pycount &= 2047;
                p->pyoff = sintable[p->pycount]>>6;
            }
            else i = 12;

            if(shrunk == 0 && truefdist <= PHEIGHT)
            {
                if(p->on_ground == 1)
                {
                    /*if( p->dummyplayersprite == -1 )
                        p->dummyplayersprite =
                            spawn(pi,PLAYERONWATER);*/

                    p->footprintcount = 6;
                    if(sector[p->cursectnum].floorpicnum == FLOORSLIME)
                        p->footprintpal = 8;
                    else p->footprintpal = 0;
                    p->footprintshade = 0;
                }
            }
        }
        else
        {
            if(p->footprintcount > 0 && p->on_ground)
                if( (sector[p->cursectnum].floorstat&2) != 2 )
            {
                for(j=headspritesect[psect];j>=0;j=nextspritesect[j])
                    if( sprite[j].picnum == FOOTPRINTS || sprite[j].picnum == FOOTPRINTS2 || sprite[j].picnum == FOOTPRINTS3 || sprite[j].picnum == FOOTPRINTS4 )
                        if (klabs2(sprite[j].x-p->posx) < 384)
                            if (klabs2(sprite[j].y-p->posy) < 384)
                                break;
                if(j < 0)
                {
                    p->footprintcount--;
                    if( sector[p->cursectnum].lotag == 0 && sector[p->cursectnum].hitag == 0 )
                    {
                        switch(TRAND&3)
                        {
                            case 0:  j = spawn(pi,FOOTPRINTS); break;
                            case 1:  j = spawn(pi,FOOTPRINTS2); break;
                            case 2:  j = spawn(pi,FOOTPRINTS3); break;
                            default: j = spawn(pi,FOOTPRINTS4); break;
                        }
                        sprite[j].pal = p->footprintpal;
                        sprite[j].shade = p->footprintshade;
                    }
                }
            }
        }

        if(p->posz < (fz-(i<<8)) ) //falling
        {
            if( (sb_snum&3) == 0 && p->on_ground && (sector[psect].floorstat&2) && p->posz >= (fz-(i<<8)-(16<<8) ) )
                p->posz = fz-(i<<8);
            else
            {
                p->on_ground = 0;
                p->poszv += (gc+80); // (TICSPERFRAME<<6);
                if(p->poszv >= (4096+2048)) p->poszv = (4096+2048);
                if(p->poszv > 2400 && p->falling_counter < 255)
                {
                    p->falling_counter++;
                    if( p->falling_counter == 38 )
                        p->scream_voice = spritesound(DUKE_SCREAM,pi);
                }

                if( (p->posz+p->poszv) >= (fz-(i<<8)) ) // hit the ground
                    if(sector[p->cursectnum].lotag != 1)
                    {
                        if( p->falling_counter > 62 ) quickkill(p);

                        else if( p->falling_counter > 9 )
                        {
                            j = p->falling_counter;
                            s->extra -= j-(TRAND&3);
                            if(s->extra <= 0)
                            {
                                spritesound(SQUISHED,pi);
                                p->pals[0] = 63;
                                p->pals[1] = 0;
                                p->pals[2] = 0;
                                p->pals_time = 63;
                            }
                            else
                            {
                                spritesound(DUKE_LAND,pi);
                                spritesound(DUKE_LAND_HURT,pi);
                            }

                            p->pals[0] = 16;
                            p->pals[1] = 0;
                            p->pals[2] = 0;
                            p->pals_time = 32;
                        }
                        else if(p->poszv > 2048) spritesound(DUKE_LAND,pi);
                    }
            }
        }

        else
        {
            p->falling_counter = 0;
            if(p->scream_voice > 0)
            {
                FX_StopSound(p->scream_voice);
                p->scream_voice = 0;
            }

            if(psectlotag != 1 && psectlotag != 2 && p->on_ground == 0 && p->poszv > (6144>>1))
                p->hard_landing = p->poszv>>10;

            p->on_ground = 1;

            if( i==40 )
            {
                //Smooth on the ground

                k = ((fz-(i<<8))-p->posz)>>1;
                if( klabs2(k) < 256 ) k = 0;
                p->posz += k;
                p->poszv -= 768;
                if(p->poszv < 0) p->poszv = 0;
            }
            else if(p->jumping_counter == 0)
            {
                p->posz += ((fz-(i<<7))-p->posz)>>1; //Smooth on the water
                if(p->on_warping_sector == 0 && p->posz > fz-(16<<8))
                {
                    p->posz = fz-(16<<8);
                    p->poszv >>= 1;
                }
            }

            p->on_warping_sector = 0;

            if( (sb_snum&2) )
            {
                p->posz += (2048+768);
                p->crack_time = 777 + (TRAND&255);
            }

            if( (sb_snum&1) == 0 && p->jumping_toggle == 1)
                p->jumping_toggle = 0;

            else if( (sb_snum&1) && p->jumping_toggle == 0 )
            {
                if( p->jumping_counter == 0 )
                    if( (fz-cz) > (56<<8) )
                    {
                        p->jumping_counter = 1;
                        p->jumping_toggle = 1;
                    }
            }

            if( p->jumping_counter && (sb_snum&1) == 0 )
                p->jumping_toggle = 0;
        }

        if(p->jumping_counter)
        {
            if( (sb_snum&1) == 0 && p->jumping_toggle == 1)
                p->jumping_toggle = 0;

            if( p->jumping_counter < (1024+256) )
            {
                if(psectlotag == 1 && p->jumping_counter > 768)
                {
                    p->jumping_counter = 0;
                    p->poszv = -512;
                }
                else
                {
                    p->poszv -= (sintable[(2048-128+p->jumping_counter)&2047])/12;
                    p->jumping_counter += 180;
                    p->on_ground = 0;
                }
            }
            else
            {
                p->jumping_counter = 0;
                p->poszv = 0;
            }
        }

        p->posz += p->poszv;

        if(p->posz < (cz+(4<<8)))
        {
            p->jumping_counter = 0;
            if(p->poszv < 0)
                p->posxv = p->posyv = 0;
            p->poszv = 128;
            p->posz = cz+(4<<8);
        }
    }

    //Do the quick lefts and rights

    if ( p->fist_incs ||
         p->transporter_hold > 2 ||
         p->hard_landing ||
         p->access_incs > 0 ||
         p->knee_incs > 0 ||
         (p->curr_weapon == TRIPBOMB_WEAPON &&
          *kb > 1 &&
          *kb < 4 ) )
    {
        doubvel = 0;
        p->posxv = 0;
        p->posyv = 0;
    }
    else if ( sync[snum].avel )          //p->ang += syncangvel * constant
    {                         //ENGINE calculates angvel for you
        long tempang;

        tempang = sync[snum].avel<<1;

        if( psectlotag == 2 ) p->angvel =(tempang-(tempang>>3))*ksgn(doubvel);
        else p->angvel = tempang*ksgn(doubvel);

        p->ang += p->angvel;
        p->ang &= 2047;
        p->crack_time = 777 + (TRAND&255);
    }

    if(p->spritebridge == 0)
    {
        j = sector[s->sectnum].floorpicnum;

        if( j == PURPLELAVA || sector[s->sectnum].ceilingpicnum == PURPLELAVA )
        {
            if(p->boot_amount > 0)
            {
                p->boot_amount--;
                p->inven_icon = 7;
                if(p->boot_amount <= 0)
                    checkavailinven(p);
            }
            else
            {
                if(Sound[DUKE_LONGTERM_PAIN].num < 1)
                    spritesound(DUKE_LONGTERM_PAIN,pi);
                p->pals[0] = 0; p->pals[1] = 8; p->pals[2] = 0;
                p->pals_time = 32;
                s->extra--;
            }
        }

        k = 0;

        if(p->on_ground && truefdist <= PHEIGHT+(16<<8))
        {
            switch(j)
            {
                case HURTRAIL:
                    if( rnd(32) )
                    {
                        if(p->boot_amount > 0)
                            k = 1;
                        else
                        {
                            if(Sound[DUKE_LONGTERM_PAIN].num < 1)
                                spritesound(DUKE_LONGTERM_PAIN,pi);
                            p->pals[0] = 64; p->pals[1] = 64; p->pals[2] = 64;
                            p->pals_time = 32;
                            s->extra -= 1+(TRAND&3);
                            if(Sound[SHORT_CIRCUIT].num < 1)
                                spritesound(SHORT_CIRCUIT,pi);
                        }
                    }
                    break;
                case FLOORSLIME:
                    if( rnd(16) )
                    {
                        if(p->boot_amount > 0)
                            k = 1;
                        else
                        {
                            if(Sound[DUKE_LONGTERM_PAIN].num < 1)
                                spritesound(DUKE_LONGTERM_PAIN,pi);
                            p->pals[0] = 0; p->pals[1] = 8; p->pals[2] = 0;
                            p->pals_time = 32;
                            s->extra -= 1+(TRAND&3);
                        }
                    }
                    break;
                case FLOORPLASMA:
                    if( rnd(32) )
                    {
                        if( p->boot_amount > 0 )
                            k = 1;
                        else
                        {
                            if(Sound[DUKE_LONGTERM_PAIN].num < 1)
                                spritesound(DUKE_LONGTERM_PAIN,pi);
                            p->pals[0] = 8; p->pals[1] = 0; p->pals[2] = 0;
                            p->pals_time = 32;
                            s->extra -= 1+(TRAND&3);
                        }
                    }
                    break;
                case TILE4250:
                    if( rnd(32) )
                    {
                        if( p->boot_amount > 0 )
                            k = 1;
                        else
                        {
                            if(Sound[DUKE_LONGTERM_PAIN].num < 1)
                                spritesound(DUKE_LONGTERM_PAIN,pi);
                            p->pals[0] = 8; p->pals[1] = 0; p->pals[2] = 0;
                            p->pals_time = 32;
                            s->extra -= 1+(TRAND&3);
                        }
                    }
                    break;
            }
        }

        if( k )
        {
            FTA(75,p);
            p->boot_amount -= 2;
            if(p->boot_amount <= 0)
                checkavailinven(p);
        }
    }

    if ( p->posxv || p->posyv || sync[snum].fvel || sync[snum].svel )
    {
        p->crack_time = 777 + (TRAND&255);

        k = sintable[p->bobcounter&2047]>>12;

        if(truefdist < PHEIGHT+(8<<8) )
            if( k == 1 || k == 3 )
        {
            if(p->spritebridge == 0 && p->walking_snd_toggle == 0 && p->on_ground)
            {
                switch( psectlotag )
                {
                    case 0:

                        if(lz >= 0 && (lz&0xfff)==49152 )
                        {
                            if ((lz&0xfff) >= MAXSPRITES) gameexit("Sprite overflow caused by GERBIL\n");
                            j = sprite[lz&0xfff].picnum;
                        }
                        else j = sector[psect].floorpicnum;

                        switch(j)
                        {
                            case PANNEL1:
                            case PANNEL2:
                                spritesound(DUKE_WALKINDUCTS,pi);
                                p->walking_snd_toggle = 1;
                                break;
                        }
                        break;
                    case 1:
                        if((TRAND&1) == 0)
                            spritesound(DUKE_ONWATER,pi);
                        p->walking_snd_toggle = 1;
                        break;
                }
            }
        }
        else if(p->walking_snd_toggle > 0)
            p->walking_snd_toggle --;

        if(p->jetpack_on == 0 && p->steroids_amount > 0 && p->steroids_amount < 400)
            doubvel <<= 1;

        p->posxv += ((sync[snum].fvel*doubvel)<<6);
        p->posyv += ((sync[snum].svel*doubvel)<<6);

        if( ( p->curr_weapon == KNEE_WEAPON && *kb > 10 && p->on_ground ) || ( p->on_ground && (sb_snum&2) ) )
        {
            p->posxv = mulscale(p->posxv,dukefriction-0x2000,16);
            p->posyv = mulscale(p->posyv,dukefriction-0x2000,16);
        }
        else
        {
            if(psectlotag == 2)
            {
                p->posxv = mulscale(p->posxv,dukefriction-0x1400,16);
                p->posyv = mulscale(p->posyv,dukefriction-0x1400,16);
            }
            else
            {
                p->posxv = mulscale(p->posxv,dukefriction,16);
                p->posyv = mulscale(p->posyv,dukefriction,16);
            }
        }

        if( klabs2(p->posxv) < 2048 && klabs2(p->posyv) < 2048 )
            p->posxv = p->posyv = 0;

        if( shrunk )
        {
            p->posxv =
                mulscale16(p->posxv,dukefriction-(dukefriction>>1)+(dukefriction>>2));
            p->posyv =
                mulscale16(p->posyv,dukefriction-(dukefriction>>1)+(dukefriction>>2));
        }
    }

    HORIZONLY:

        if(psectlotag == 1 || p->spritebridge == 1) i = (4L<<8);
        else i = (20L<<8);

        if(sector[p->cursectnum].lotag == 2) k = 0;
        else k = 1;

        if(ud.clipping)
        {
            j = 0;
            p->posx += p->posxv>>14;
            p->posy += p->posyv>>14;
            updatesector(p->posx,p->posy,&p->cursectnum);
            changespritesect(pi,p->cursectnum);
        }
        else
            j = clipmove(&p->posx,&p->posy,
                &p->posz,&p->cursectnum,
                p->posxv,p->posyv,164L,(4L<<8),i,CLIPMASK0);

        if(p->jetpack_on == 0 && psectlotag != 2 && psectlotag != 1 && shrunk)
            p->posz += 32<<8;

        if(j)
            checkplayerhurt(p,j);

        if(p->jetpack_on == 0)
        {
            if( s->xvel > 16 )
            {
                if( psectlotag != 1 && psectlotag != 2 && p->on_ground )
                {
                    p->pycount += 52;
                    p->pycount &= 2047;
                    p->pyoff =
                        klabs2(s->xvel*sintable[p->pycount])/1596;
                }
            }
            else if( psectlotag != 2 && psectlotag != 1 )
                p->pyoff = 0;
        }

        // RBG***
        setsprite(pi,p->posx,p->posy,p->posz+PHEIGHT);

        if( psectlotag < 3 )
        {
            psect = s->sectnum;
            if( ud.clipping == 0 && sector[psect].lotag == 31)
            {
                if( sprite[sector[psect].hitag].xvel && hittype[sector[psect].hitag].temp_data[0] == 0)
                {
                    quickkill(p);
                    return;
                }
            }
        }

        if(truefdist < PHEIGHT && p->on_ground && psectlotag != 1 && shrunk == 0 && sector[p->cursectnum].lotag == 1)
            if( Sound[DUKE_ONWATER].num == 0 )
                spritesound(DUKE_ONWATER,pi);

        if (p->cursectnum != s->sectnum)
            changespritesect(pi,p->cursectnum);

        if(ud.clipping == 0)
            j = ( pushmove(&p->posx,&p->posy,&p->posz,&p->cursectnum,164L,(4L<<8),(4L<<8),CLIPMASK0) < 0 && furthestangle(pi,8) < 512 );
        else j = 0;

        if(ud.clipping == 0)
        {
            if( klabs2(hittype[pi].floorz-hittype[pi].ceilingz) < (48<<8) || j )
            {
                if ( !(sector[s->sectnum].lotag&0x8000) && ( isanunderoperator(sector[s->sectnum].lotag) ||
                    isanearoperator(sector[s->sectnum].lotag) ) )
                        activatebysector(s->sectnum,pi);
                if(j)
                {
                    quickkill(p);
                    return;
                }
            }
            else if( klabs2(fz-cz) < (32<<8) && isanunderoperator(sector[psect].lotag) )
                activatebysector(psect,pi);
        }

        if( sb_snum&(1<<18) || p->hard_landing)
            p->return_to_center = 9;

        if( sb_snum&(1<<13) )
        {
            p->return_to_center = 9;
            if( sb_snum&(1<<5) ) p->horiz += 12;
            p->horiz += 12;
        }

        else if( sb_snum&(1<<14) )
        {
            p->return_to_center = 9;
            if( sb_snum&(1<<5) ) p->horiz -= 12;
            p->horiz -= 12;
        }

        else if( sb_snum&(1<<3) )
        {
            if( sb_snum&(1<<5) ) p->horiz += 6;
            p->horiz += 6;
        }

        else if( sb_snum&(1<<4) )
        {
            if( sb_snum&(1<<5) ) p->horiz -= 6;
            p->horiz -= 6;
        }
        if(p->return_to_center > 0)
            if( (sb_snum&(1<<13)) == 0 && (sb_snum&(1<<14)) == 0 )
        {
            p->return_to_center--;
            p->horiz += 33-(p->horiz/3);
        }

        if (p->f_370 > 0)
        {
            p->f_370 -= max(1, p->f_370 / 3);
        }
        if (p->f_370 < 0)
        {
            p->f_370 -= min(-1, p->f_370 / 3);
        }

        if(p->hard_landing > 0)
        {
            p->hard_landing--;
            p->horiz -= (p->hard_landing<<4);
        }

        if(p->aim_mode)
            p->horiz += sync[snum].horz>>1;
        else
        {
             if( p->horiz > 95 && p->horiz < 105) p->horiz = 100;
             if( p->horizoff > -5 && p->horizoff < 5) p->horizoff = 0;
        }

        if(p->horiz > 228) p->horiz = 228;
        else if(p->horiz < -28) p->horiz = -28;

    //Shooting code/changes

    /*if( p->show_empty_weapon > 0)
    {
        p->show_empty_weapon--;
        if(p->show_empty_weapon == 0)
        {
            if(p->last_full_weapon == GROW_WEAPON)
                p->subweapon |= (1<<GROW_WEAPON);
            else if(p->last_full_weapon == SHRINKER_WEAPON)
                p->subweapon &= ~(1<<GROW_WEAPON);
            addweapon( p, p->last_full_weapon );
            return;
        }
    }*/

    if(p->knee_incs > 0)
    {
        p->knee_incs++;
        p->horiz -= 48;
        p->return_to_center = 9;
        if(p->knee_incs > 15)
        {
            p->knee_incs = 0;
            p->holster_weapon = 0;
            if(p->weapon_pos < 0)
                p->weapon_pos = -p->weapon_pos;
            if(p->actorsqu >= 0 && dist(&sprite[pi],&sprite[p->actorsqu]) < 1400 )
            {
                guts(&sprite[p->actorsqu],JIBS6,7,myconnectindex);
                spawn(p->actorsqu,BLOODPOOL);
                spritesound(SQUISHED,p->actorsqu);
                switch(sprite[p->actorsqu].picnum)
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
                        if(sprite[p->actorsqu].yvel)
                            operaterespawns(sprite[p->actorsqu].yvel);
                        break;
                }

                if(sprite[p->actorsqu].picnum == APLAYER)
                {
                    quickkill(&ps[sprite[p->actorsqu].yvel]);
                    ps[sprite[p->actorsqu].yvel].frag_ps = snum;
                }
                else if(badguy(&sprite[p->actorsqu]))
                {
                    deletesprite(p->actorsqu);
                    p->actors_killed++;
                }
                else deletesprite(p->actorsqu);
            }
            p->actorsqu = -1;
        }
        else if(p->actorsqu >= 0)
            p->ang += getincangle(p->ang,getangle(sprite[p->actorsqu].x-p->posx,sprite[p->actorsqu].y-p->posy))>>2;
    }

    if( doincrements(p) ) return;

    if(p->weapon_pos != 0)
    {
        if(p->weapon_pos == -9)
        {
            if(p->last_weapon >= 0)
            {
                p->weapon_pos = 10;
//                if(p->curr_weapon == KNEE_WEAPON) *kb = 1;
                p->last_weapon = -1;
            }
            else if(p->holster_weapon == 0)
                p->weapon_pos = 10;
        }
        else p->weapon_pos--;
    }

    // HACKS

    SHOOTINCODE:

    if( p->curr_weapon == SHRINKER_WEAPON || p->curr_weapon == GROW_WEAPON )
        p->random_club_frame += 64; // Glowing

    if(p->rapid_fire_hold == 1)
    {
        if( sb_snum&(1<<2) ) return;
        p->rapid_fire_hold = 0;
    }

    if(shrunk || p->tipincs || p->access_incs)
        sb_snum &= ~(1<<2);
    else if ( shrunk == 0 && (sb_snum&(1<<2)) && (*kb) == 0 && p->fist_incs == 0 &&
         p->last_weapon == -1 && ( p->weapon_pos == 0 || p->holster_weapon == 1 ) )
    {

        p->crack_time = 777+(TRAND&255);

        if(p->holster_weapon == 1)
        {
            if( p->last_pissed_time <= (26*218) && p->weapon_pos == -9)
            {
                p->holster_weapon = 0;
                p->weapon_pos = 10;
                FTA(74,p);
            }
        }
        else switch(p->curr_weapon)
        {
            case HANDBOMB_WEAPON:
                p->hbomb_hold_delay = 0;
                if( p->ammo_amount[HANDBOMB_WEAPON] > 0 )
                    (*kb)=1;
                break;
            case HANDREMOTE_WEAPON:
                p->hbomb_hold_delay = 0;
                (*kb) = 1;
                break;

            case PISTOL_WEAPON:
            case PISTOL2_WEAPON:
                if( p->ammo_amount[p->curr_weapon] > 0 )
                {
                    p->ammo_amount[p->curr_weapon]--;
                    (*kb) = 1;
                }
                break;


            case CHAINGUN_WEAPON:
                if( p->ammo_amount[CHAINGUN_WEAPON] > 0 ) // && p->random_club_frame == 0)
                    (*kb)=1;
                break;

            case SHOTGUN_WEAPON:
            case SHOTGUN2_WEAPON:
                if( p->ammo_amount[p->curr_weapon] > 0 && p->random_club_frame == 0 )
                    (*kb)=1;
                break;
#ifndef VOLUMEONE
            case TRIPBOMB_WEAPON:
                if ( p->ammo_amount[TRIPBOMB_WEAPON] > 0 )
                {
                    long sx,sy,sz;
                    short sect,hw,hitsp;

                    hitscan( p->posx, p->posy, p->posz,
                             p->cursectnum, sintable[(p->ang+512)&2047],
                             sintable[p->ang&2047], (100-p->horiz-p->horizoff)*32,
                             &sect, &hw, &hitsp, &sx, &sy, &sz,CLIPMASK1);

                    if(sect < 0 || hitsp >= 0)
                        break;

                    if( hw >= 0 && sector[sect].lotag > 2 )
                        break;

                    if(hw >= 0 && wall[hw].overpicnum >= 0)
                        if(wall[hw].overpicnum == BIGFORCE)
                            break;

                    j = headspritesect[sect];
                    while(j >= 0)
                    {
                        if( sprite[j].picnum == TRIPBOMB &&
                            klabs2(sprite[j].z-sz) < (12<<8) && ((sprite[j].x-sx)*(sprite[j].x-sx)+(sprite[j].y-sy)*(sprite[j].y-sy)) < (290*290) )
                                    break;
                        j = nextspritesect[j];
                    }

                    if(j == -1 && hw >= 0 && (wall[hw].cstat&16) == 0 )
                        if( ( wall[hw].nextsector >= 0 && sector[wall[hw].nextsector].lotag <= 2 ) || ( wall[hw].nextsector == -1 && sector[sect].lotag <= 2 ) )
                            if( ( (sx-p->posx)*(sx-p->posx) + (sy-p->posy)*(sy-p->posy) ) < (290*290) )
                    {
                        p->posz = p->oposz;
                        p->poszv = 0;
                        (*kb) = 1;
                    }
                }
                break;

            case SHRINKER_WEAPON:
            case GROW_WEAPON:
                if( p->curr_weapon == GROW_WEAPON )
                {
                    if( p->ammo_amount[GROW_WEAPON] > 0 )
                    {
                        (*kb) = 1;
                        spritesound(EXPANDERSHOOT,pi);
                    }
                }
                else if( p->ammo_amount[SHRINKER_WEAPON] > 0)
                {
                    (*kb) = 1;
                    spritesound(SHRINKER_FIRE,pi);
                }
                break;

            case FREEZE_WEAPON:
                p->hbomb_hold_delay = 0;
                if( p->ammo_amount[FREEZE_WEAPON] > 0 )
                    (*kb) = 1;
                break;
            case DEVISTATOR_WEAPON:
                if( p->ammo_amount[p->curr_weapon] > 0 )
                    (*kb) = 1;
                break;

#endif
            case RPG_WEAPON:
            case RPG2_WEAPON:
                if ( p->ammo_amount[p->curr_weapon] > 0)
                    (*kb) = 1;
                break;

            case KNEE_WEAPON:
                if(p->quick_kick == 0) (*kb) = 1;
                break;
        }
    }
    else if((*kb))
    {
        processweapon(snum, sb_snum);
    }
}
#else
#pragma GLOBAL_ASM("nonmatchings/src/player1/processinput.s")
#endif

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

int fakekrand(void)
{
	if (gamestate == 2)
		return krand();
	return 42;
}

// TODO: part of struct???

extern int D_8021D680;
extern int D_8021D6A8;
extern int D_8021D6D0;

void rotatesprite_setidshadepal(unsigned short i, short shade, short pal)
{
    shade = 28 - shade;
    shade = shade * 9;
    if (shade > 256)
        shade = 256;
    if (shade <= 0)
        shade = 0;
    D_8021D6A8 = shade;
    D_8021D680 = shade;
    D_8021D6D0 = shade;
    switch (pal)
    {
        case 1:
            D_8021D680 = D_8021D680 / 2;
            D_8021D6A8 = D_8021D6A8 / 2;
            break;
        case 2:
            D_8021D6A8 = D_8021D6A8 / 2;
            D_8021D6D0 = D_8021D6D0 / 2;
            break;
        case 6:
        case 8:
            D_8021D680 = D_8021D680 / 2;
            D_8021D6D0 = D_8021D6D0 / 2;
            break;
    }
    if (ps[i].heat_on > 0 && ps[i].newowner == -1)
    {
        D_8021D680 = (D_8021D680 * 171) >> 8;
        D_8021D6A8 = (D_8021D6A8 * 384) >> 8;
        D_8021D6D0 = (D_8021D6D0 * 171) >> 8;
        shade = (shade + 512) / 3;
    }
    D_8021D680 = (shade * D_8021D680) / 256;
    D_8021D6A8 = (shade * D_8021D6A8) / 256;
    D_8021D6D0 = (shade * D_8021D6D0) / 256;
    if (ps[i].pals_time > 0 && ps[i].loogcnt == 0)
    {
        D_8021D680 += ((ps[i].pals[0] * 4 - D_8021D680) * ps[i].pals_time) / 64;
        D_8021D6A8 += ((ps[i].pals[1] * 4 - D_8021D6A8) * ps[i].pals_time) / 64;
        D_8021D6D0 += ((ps[i].pals[2] * 4 - D_8021D6D0) * ps[i].pals_time) / 64;
    }
    else if (ps[i].loogcnt > 0)
        D_8021D6A8 += (((256 - D_8021D6A8) * ps[i].loogcnt) / 2) / 64;

    if (D_8021D680 < 0)
        D_8021D680 = 0;
    if (D_8021D680 > 255)
        D_8021D680 = 255;
    if (D_8021D6A8 < 0)
        D_8021D6A8 = 0;
    if (D_8021D6A8 > 255)
        D_8021D6A8 = 255;
    if (D_8021D6D0 < 0)
        D_8021D6D0 = 0;
    if (D_8021D6D0 > 255)
        D_8021D6D0 = 255;
    gDPSetPrimColor(pkt++, 0, 0, D_8021D680, D_8021D6A8, D_8021D6D0, totalclock & 255);
    gDPSetEnvColor(pkt++, D_8021D680, D_8021D6A8, D_8021D6D0, 255);
    gDPSetBlendColor(pkt++, D_8021D680, D_8021D6A8, D_8021D6D0, 0);
    gDPSetRenderMode(pkt++, G_RM_AA_TEX_EDGE, G_RM_AA_TEX_EDGE2);
    gDPSetCombineMode(pkt++, G_CC_BLENDIDECALA, G_CC_BLENDIDECALA);
}

void myospal(long x, long y, short tilenum, signed char shade, char orientation, char p)
{
    short sc;
    float fx;
    float fy;

    fx = (float) x;
    fy = (float) y;
    if (p != 255)
        rotatesprite_setidshadepal(splitscreenpeek, (short) shade, (short) p);
    else
        setrotatespriteflags(255, 255, 255, 666);

    if (numplayers == 2)
    {
        if (splitscreenpeek == 0)
            fy -= 52.5;
        else
            fy += 52.5;
    }
    if (numplayers > 2)
    {
        fx -= 21.0f;
        fy -= 15.0f;
        fx *= 0.5;
        fy *= 0.5;
        
        if (splitscreenpeek == 0 || splitscreenpeek == 2)
            fx += 21.0f;
        else
            fx += 160.0;

        sc = 50;
        if (splitscreenpeek == 0 || splitscreenpeek == 1)
            fy += 15.0f;
        else
            fy += 120.0;

        rotatespritesc(fx, fy, sc, sc, tilenum, orientation);
    }
    else
    {
        sc = 100;
        rotatesprite(fx, fy, sc, sc, tilenum, orientation);
    }
}

void displayflash(long x, long y, short tilenum, float sx, float sy, char orientation, short p)
{
    sx = sx * 3.0;
    sy = sy * 3.0;
    if (numplayers == 2)
    {
        if (splitscreenpeek == 0)
            y -= 52;
        else
            y += 52;
    }
    if (numplayers > 2)
    {
        x -= 21;
        y -= 15;
        x /= 2;
        y /= 2;
        if (splitscreenpeek == 0 || splitscreenpeek == 2)
            x += 21;
        else
            x += 160;

        if (splitscreenpeek == 0 || splitscreenpeek == 1)
            y += 15;
        else
            y += 120;

        sx *= 0.5;
        sy *= 0.5;
    }
    if (tilenum == TILE3841)
        setfxcolor(15, 255, (ps[splitscreenpeek].hbomb_hold_delay * 255) / 99, 115, 0, 170);
    else
        setfxcolor(colortable[p][0], colortable[p][1], colortable[p][2], colortable[p][3], colortable[p][4], colortable[p][5]);
    rendertileworld(x, y, sx, sy, gettileid(tilenum), orientation);
    unsetfxsprite();
}

void displayloogie(short snum)
{
    long i, a, x, y, z;
    float fz;

    if(ps[snum].loogcnt == 0) return;

    y = (ps[snum].loogcnt<<2);
    for(i=0;i<ps[snum].numloogs;i++)
    {
        a = klabs(sintable[((ps[snum].loogcnt+i)<<5)&2047])>>5;
        z = 4096+((ps[snum].loogcnt+i)<<9);
        x = (-sync[snum].avel)+(sintable[((ps[snum].loogcnt+i)<<6)&2047]>>10);
        fz = ((z - (i << 8)) * 100) / 65536;

        rotatesprite(
            ps[snum].loogiex[i]+x,200+ps[snum].loogiey[i]-y,fz,fz,
            LOOGIE,0);
    }
}

char animatefist(short gs,short snum)
{
    short looking_arc,fisti,fistpal;
    long fistzoom, fistz;

    fisti = ps[snum].fist_incs;
    if(fisti > 32) fisti = 32;
    if(fisti <= 0) return 0;

    looking_arc = klabs(ps[snum].look_ang)/9;

    fistzoom = 65536L - (sintable[(512+(fisti<<6))&2047]<<2);
    if(fistzoom > 90612L)
        fistzoom = 90612L;
    if(fistzoom < 40920)
        fistzoom = 40290;
    fistz = 214 + (sintable[((6+fisti)<<7)&2047]>>9);

    if(sprite[ps[snum].i].pal == 1)
        fistpal = 1;
    else
        fistpal = sector[ps[snum].cursectnum].floorpal;

    rotatesprite(
        (-fisti+222+(sync[snum].avel>>4)),
        (looking_arc+fistz),
        (fistzoom * 100) / 65536, (fistzoom * 100) / 65536,FIST,0);

    return 1;
}

char animateknee(short gs,short snum)
{
    short knee_y[] = {0,-8,-16,-32,-64,-84,-108,-108,-108,-72,-32,-8};
    short looking_arc, pal;

    if(ps[snum].knee_incs > 11 || ps[snum].knee_incs == 0 || sprite[ps[snum].i].extra <= 0) return 0;

    looking_arc = knee_y[ps[snum].knee_incs] + klabs(ps[snum].look_ang)/9;

    looking_arc -= (ps[snum].hard_landing<<3);

    if(sector[ps[snum].cursectnum].lotag == 2)
        pal = 1;
    else
        pal = sector[ps[snum].cursectnum].floorpal;

    myospal(105+(sync[snum].avel>>4)-(ps[snum].look_ang>>1)+(knee_y[ps[snum].knee_incs]>>2),looking_arc+331-((ps[snum].horiz-ps[snum].horizoff)>>4),KNEE,gs,4,pal);

    return 1;
}

char animateknuckles(short gs,short snum)
{
    short knuckle_frames[] = {0,1,2,2,3,3,3,2,2,1,0};
    short looking_arc, pal;

    if(ps[snum].knuckle_incs == 0 || sprite[ps[snum].i].extra <= 0) return 0;

    looking_arc = klabs(ps[snum].look_ang)/9;

    looking_arc -= (ps[snum].hard_landing<<3);
    
    if(sector[ps[snum].cursectnum].lotag == 2)
        pal = 1;
    else
        pal = sector[ps[snum].cursectnum].floorpal;

    myospal(160+(sync[snum].avel>>4)-(ps[snum].look_ang>>1),looking_arc+180-((ps[snum].horiz-ps[snum].horizoff)>>4),CRACKKNUCKLES+knuckle_frames[ps[snum].knuckle_incs>>1],gs,4,pal);

    return 1;
}
void displaymasks(short snum)
{
    short i, p;

    if(sector[ps[snum].cursectnum].lotag == 2)
        p = 1;
    else
        p = sector[ps[snum].cursectnum].floorpal;

     if(ps[snum].scuba_on)
	 {
        myospal(103,208,SCUBAMASK,0,0,p);
        myospal(217,208,SCUBAMASK,0,4,p);
	 }
}

char animatetip(short gs,short snum)
{
    short p,looking_arc;
    short tip_y[] = {0,-8,-16,-32,-64,-84,-108,-108,-108,-108,-108,-108,-108,-108,-108,-108,-96,-72,-64,-32,-16};

    if(ps[snum].tipincs == 0) return 0;

    looking_arc = klabs(ps[snum].look_ang)/9;
    looking_arc -= (ps[snum].hard_landing<<3);

    if(sector[ps[snum].cursectnum].lotag == 2)
        p = 1;
    else
        p = sector[ps[snum].cursectnum].floorpal;

/*    if(ps[snum].access_spritenum >= 0)
        p = sprite[ps[snum].access_spritenum].pal;
    else
        p = wall[ps[snum].access_wallnum].pal;
  */
    myospal(170+(sync[snum].avel>>4)-(ps[snum].look_ang>>1),
        (tip_y[ps[snum].tipincs]>>1)+looking_arc+240-((ps[snum].horiz-ps[snum].horizoff)>>4),TIP+((26-ps[snum].tipincs)>>4),gs,0,p);

    return 1;
}

char animateaccess(short gs,short snum)
{
    short access_y[] = {0,-8,-16,-32,-64,-84,-108,-108,-108,-108,-108,-108,-108,-108,-108,-108,-96,-72,-64,-32,-16};
    short looking_arc;
    short tile;
    char p, p2;

    if(ps[snum].access_incs == 0 || sprite[ps[snum].i].extra <= 0) return 0;

    looking_arc = access_y[ps[snum].access_incs] + klabs(ps[snum].look_ang)/9;
    looking_arc -= (ps[snum].hard_landing<<3);

    if(ps[snum].access_spritenum >= 0)
        p = sprite[ps[snum].access_spritenum].pal;
    else p = 0;
//    else
//        p = wall[ps[snum].access_wallnum].pal;
    if(sector[ps[snum].cursectnum].lotag == 2)
        p2 = 1;
    else
        p2 = sector[ps[snum].cursectnum].floorpal;

    if((ps[snum].access_incs-3) > 0 && (ps[snum].access_incs-3)>>3)
        myospal(170+(sync[snum].avel>>4)-(ps[snum].look_ang>>1)+(access_y[ps[snum].access_incs]>>2),looking_arc+279-((ps[snum].horiz-ps[snum].horizoff)>>4),HANDHOLDINGLASER+(ps[snum].access_incs>>3),gs,0,p2);
    else
    {
        switch (p)
        {
            case 0:
                tile = 0xe83;
                break;
            case 21:
                tile = 0xe81;
                break;
            case 23:
                tile = 0xe82;
                break;
        }
        myospal(170+(sync[snum].avel>>4)-(ps[snum].look_ang>>1)+(access_y[ps[snum].access_incs]>>2),looking_arc+279-((ps[snum].horiz-ps[snum].horizoff)>>4),0xe84,gs,4,p2);
        myospal(170+(sync[snum].avel>>4)-(ps[snum].look_ang>>1)+(access_y[ps[snum].access_incs]>>2),looking_arc+279-((ps[snum].horiz-ps[snum].horizoff)>>4),tile,gs,4,p2);
    }

    return 1;
}

#ifdef NON_MATCHING
extern short fistsign;

void displayweapon(short snum)
{
    long gun_pos, looking_arc, cw;
    long weapon_xoffset, i, j;
        short tile;
    short unk;
        short sh;
        short pp;
        short ox;
        short oy;
    char o,pal;
    signed char gs;
    struct player_struct *p;
    short *kb;

    p = &ps[snum];
    kb = &p->kickback_pic;

    o = 0;

    looking_arc = klabs(p->look_ang)/9;

    gs = sprite[p->i].shade;
    if(gs > 24) gs = 24;

    if(p->newowner >= 0 || ud.camerasprite >= 0 || p->over_shoulder_on > 0 || (sprite[p->i].pal != 1 && sprite[p->i].extra <= 0) || animatefist(gs,snum) || animateknuckles(gs,snum) || animatetip(gs,snum) || animateaccess(gs,snum) )
        return;

    animateknee(gs,snum);

    gun_pos = 80-(p->weapon_pos*p->weapon_pos);
    gun_pos = gun_pos - 20;

    weapon_xoffset =  (160)-90;
    weapon_xoffset -= (sintable[((p->weapon_sway>>1)+512)&2047]/(1024+512));
    weapon_xoffset -= 58 + p->weapon_ang;
    if( sprite[p->i].xrepeat < 32 )
        gun_pos -= klabs(sintable[(p->weapon_sway<<2)&2047]>>9);
    else gun_pos -= klabs(sintable[(p->weapon_sway>>1)&2047]>>11);

    gun_pos -= (p->hard_landing<<3);

    if(p->last_weapon >= 0)
        cw = p->last_weapon;
    else cw = p->curr_weapon;

    if (cw != TRIPBOMB_WEAPON)
        weapon_xoffset += (p->weapon_pos*p->weapon_pos) / 2;
    
    if(sector[p->cursectnum].lotag == 2)
        pal = 1;
    else
        pal = sector[p->cursectnum].floorpal;

    j = 14-p->quick_kick;
    if(j != 14)
    {
        if( j < 5 || j > 9 )
            myospal(weapon_xoffset+80-(p->look_ang>>1),
                looking_arc+250-gun_pos,KNEE,gs,o|4,pal);
        else myospal(weapon_xoffset+160-16-(p->look_ang>>1),
            looking_arc+214-gun_pos,KNEE+1,gs,o|4,pal);
    }

    if( sprite[p->i].xrepeat < 40 )
    {
        if(p->jetpack_on == 0 )
        {
            i = sprite[p->i].xvel;
            looking_arc += 32-(i>>1);
            fistsign += i>>1;
        }
        cw = weapon_xoffset;
        weapon_xoffset += sintable[(fistsign)&2047]>>10;
        myospal(weapon_xoffset+250-(p->look_ang>>1),
             looking_arc+258-(klabs(sintable[(fistsign)&2047]>>8)),
             FIST,gs,o,pal);
        weapon_xoffset = cw;
        weapon_xoffset -= sintable[(fistsign)&2047]>>10;
        myospal(weapon_xoffset+40-(p->look_ang>>1),
             looking_arc+200+(klabs(sintable[(fistsign)&2047]>>8)),
             FIST,gs,o|4,pal);
    }
    else switch(cw)
    {
        case KNEE_WEAPON:
            if( (*kb) > 0 )
            {
                if( (*kb) < 5 || (*kb) > 9 )
                    myospal(weapon_xoffset+260-(p->look_ang>>1),
                        looking_arc+321-gun_pos,KNEE,gs,o,pal);
                else
                    myospal(weapon_xoffset+200-(p->look_ang>>1),
                       looking_arc+285-gun_pos,KNEE+1,gs,o,pal);
            }
            break;

        case TRIPBOMB_WEAPON:
            weapon_xoffset += 8;
            gun_pos -= 10;

            if((*kb) > 6)
                looking_arc += ((*kb)<<3);
            if((*kb) < 4)
                myospal(weapon_xoffset+142-(p->look_ang>>1),
                        looking_arc+237-gun_pos,HANDHOLDINGLASER+3,gs,o,pal);

            myospal(weapon_xoffset+130-(p->look_ang>>1),
                    looking_arc+249-gun_pos,
                    HANDHOLDINGLASER+((*kb)>>2),gs,o,pal);
            myospal(weapon_xoffset+152-(p->look_ang>>1),
                    looking_arc+249-gun_pos,
                    HANDHOLDINGLASER+((*kb)>>2),gs,o|4,pal);

            break;

        case RPG_WEAPON:
        case RPG2_WEAPON:
            if(*kb < 8)
            {
                weapon_xoffset -= sintable[(1024+((*kb)<<7))&2047]>>10;
                gun_pos += sintable[(1024+((*kb)<<7)&2047)]>>11;
            }

            myospal(weapon_xoffset+249,(looking_arc<<1)+190-gun_pos,
                    RPGGUN+1,gs,o,pal);

            if(*kb > 0)
            {
                tile = cw == RPG_WEAPON ? 0xED0 : 0xECD;
                ox = 79 - *kb * 2;
                if (ox < 47)
                    ox = 47;
                oy = *kb * 3 - 60;
                if (oy < 0)
                    oy = 0;

                myospal(weapon_xoffset+ox+249,(looking_arc<<1)+oy+189-gun_pos,tile,gs,0,pal);
            }

            myospal(weapon_xoffset+249,(looking_arc<<1)+189-gun_pos,
                    RPGGUN,gs,o,pal);


            
            if(*kb == 0)
            {
                if (cw == RPG_WEAPON)
                    myospal(weapon_xoffset+246,(looking_arc<<1)+226-gun_pos,
                            0xECE,-32,o,pal);
                else
                    myospal(weapon_xoffset+255,(looking_arc<<1)+230-gun_pos,
                            0xECF,-32,o,pal);
            }
            break;

        case SHOTGUN_WEAPON:
        case SHOTGUN2_WEAPON:
            weapon_xoffset -= 8;

            if (*kb > 3 && *kb < 8)
            {
                gun_pos -= 15;
                weapon_xoffset += 15;
            }

            switch(*kb)
            {
                case 0:
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                case 8:
                    myospal(weapon_xoffset+305-(p->look_ang>>1),looking_arc+285-gun_pos,
                        SHOTGUN,gs,o,pal);
                    break;
                case 9:
                case 10:
                case 11:
                case 12:
                    myospal(weapon_xoffset+305-(p->look_ang>>1),looking_arc+285-gun_pos,
                        SHOTGUN+3,gs,o,pal);

                    break;
                case 13:
                case 14:
                case 15:
                case 28:
                case 29:
                case 30:
                    myospal(weapon_xoffset+305-(p->look_ang>>1),looking_arc+296-gun_pos,
                        SHOTGUN+4,gs,o,pal);
                    break;
                case 16:
                case 17:
                case 18:
                case 25:
                case 26:
                case 27:
                    myospal(weapon_xoffset+305-(p->look_ang>>1),looking_arc+285-gun_pos,
                        SHOTGUN+5,gs,o,pal);
                    break;
                case 19:
                case 20:
                case 21:
                case 22:
                case 23:
                case 24:
                    myospal(weapon_xoffset+305-(p->look_ang>>1),looking_arc+285-gun_pos,
                        SHOTGUN+6,gs,o,pal);
                    break;
            }
            break;



        case CHAINGUN_WEAPON:
        {
            oy = gun_pos;
            ox = weapon_xoffset;
            if(*kb > 0 && (fakekrand() & 255) < 127)
            {
        float siz;
                gun_pos -= klabs(sintable[(*kb)<<7]>>12);
                weapon_xoffset += fakekrand() & 3;

                siz = (fakekrand() & 255) * 0.00390625 + 0.7;

                displayflash(weapon_xoffset+215-(p->look_ang>>1),looking_arc+205-gun_pos,
                            0xF4F, siz, siz, 0, 12);
            }

            myospal(weapon_xoffset+235-(p->look_ang>>1),looking_arc+265-gun_pos,
                CHAINGUN,gs,0,pal);

            gun_pos = oy;
            weapon_xoffset = ox;
            
            if(*kb > 0 && (fakekrand() & 255) < 127)
            {
        float siz;
                gun_pos -= klabs(sintable[(((*kb)<<7)+512)&2047]>>12);
                weapon_xoffset -= fakekrand() & 3;

                siz = (fakekrand() & 255) * 0.00390625 + 0.7;

                displayflash(105-weapon_xoffset+(p->look_ang>>1),looking_arc+205-gun_pos,
                            0xF4F, siz, siz, 4, 12);
            }

            myospal(-weapon_xoffset+85+(p->look_ang>>1),looking_arc+265-gun_pos,
                CHAINGUN,gs,4,pal);
            
            weapon_xoffset = ox;
            gun_pos = oy;
            break;
        }
         case PISTOL_WEAPON:
         case PISTOL2_WEAPON:
            if( (*kb) < 5)
            {
                short kb_frames[] = {0,1,1,0,0},l;

                l = 165+weapon_xoffset;

                if((*kb) == 2)
                    l += 6;
                if (kb_frames[*kb])
                {
        float siz;
                    siz = (fakekrand() & 127) * 0.00390625 + 0.75;

                    displayflash(l+30-(p->look_ang>>1),looking_arc+205-gun_pos,
                                0xF4F, siz, siz, 0.f, 1);
                }
                myospal(
                    (l-(p->look_ang>>1))+115,
                    (looking_arc+285-gun_pos),
                    FIRSTGUN+kb_frames[*kb],
                    gs,0,pal);
            }
            else
            {
                if((*kb) < 10)
                    myospal(309-(p->look_ang>>1),looking_arc+291-gun_pos,FIRSTGUN+4,gs,o,pal);
                else if((*kb) < 15)
                {
                    myospal(284-((*kb)<<3)-(p->look_ang>>1),looking_arc+100-gun_pos+((*kb)<<4),FIRSTGUN+6,gs,o,pal);
                    myospal(298-(p->look_ang>>1),looking_arc+284-gun_pos,FIRSTGUN+5,gs,o,pal);
                }
                else if((*kb) < 20)
                {
                    myospal(154+((*kb)<<1)-(p->look_ang>>1),looking_arc+420-gun_pos-((*kb)<<3),FIRSTGUN+8,gs,o,pal);
                    myospal(174+((*kb)<<1)-(p->look_ang>>1),looking_arc+400-gun_pos-((*kb)<<3),FIRSTGUN+6,gs,o,pal);
                    myospal(298-(p->look_ang>>1),looking_arc+284-gun_pos,FIRSTGUN+5,gs,o,pal);
                }
                else if((*kb) < 23)
                {
                    myospal(184-(p->look_ang>>1),looking_arc+245-gun_pos,FIRSTGUN+8,gs,o,pal);
                    myospal(298-(p->look_ang>>1),looking_arc+284-gun_pos,FIRSTGUN+5,gs,o,pal);
                }
                else if((*kb) < 25)
                {
                    myospal(204-(p->look_ang>>1),looking_arc+245-gun_pos,FIRSTGUN+8,gs,o,pal);
                    myospal(298-(p->look_ang>>1),looking_arc+284-gun_pos,FIRSTGUN+5,gs,o,pal);
                }
                else if((*kb) < 27)
                {
                    myospal(204-(p->look_ang>>1),looking_arc+265-gun_pos,FIRSTGUN+8,gs,o,pal);
                    myospal(308-(p->look_ang>>1),looking_arc+299-gun_pos,FIRSTGUN+5,gs,o,pal);
                }
            }

            break;
        case HANDBOMB_WEAPON:
        {
            if((*kb))
            {
                char throw_frames[]
                    = {0,0,0,0,0,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2};

                if((*kb) < 7)
                    gun_pos -= 10*(*kb);        //D
                else if((*kb) < 12)
                    gun_pos += 20*((*kb)-10); //U
                else if((*kb) < 20)
                    gun_pos -= 9*((*kb)-14);  //D

                gun_pos = gun_pos - *kb * 2;

                myospal(weapon_xoffset+190-(p->look_ang>>1),looking_arc+250-gun_pos,HANDTHROW+throw_frames[(*kb)],gs,o,pal);
            }
            else
                myospal(weapon_xoffset+190-(p->look_ang>>1),looking_arc+240-gun_pos,HANDTHROW,gs,o,pal);
        }
        break;

        case HANDREMOTE_WEAPON:
        {
                signed char remote_frames[] = {0,0,2,2,2,2,2,2,0,0,0};
                weapon_xoffset = -48;

                if((*kb))
                    myospal(weapon_xoffset+150-(p->look_ang>>1),looking_arc+258-gun_pos,HANDREMOTE+remote_frames[(*kb)],gs,o,pal);
                else
                    myospal(weapon_xoffset+150-(p->look_ang>>1),looking_arc+258-gun_pos,HANDREMOTE,gs,o,pal);
            }
            break;
        case DEVISTATOR_WEAPON:
            if (*kb > 3 && *kb < 8)
            {
                weapon_xoffset -= sintable[(1024+((*kb)<<7))&2047]>>10;
                gun_pos += sintable[(1024+((*kb)<<7)&2047)]>>10;
            }
            if (*kb > 3 && *kb < 12)
            {
                i = (*kb - 2) / 2;
                myospal(weapon_xoffset+310-(p->look_ang>>1),looking_arc+278-gun_pos,3638+i,gs,o,pal);
            }
            else
            {
                myospal(weapon_xoffset+310-(p->look_ang>>1),looking_arc+273-gun_pos,3638,gs,o,pal);
            }
            break;

        case FREEZE_WEAPON:
            if((*kb))
            {
        float siz;
                weapon_xoffset += fakekrand()&3;
                
                looking_arc += fakekrand()&3;
                
                siz = min(1.0, p->hbomb_hold_delay / 33.0);

                displayflash(weapon_xoffset-(p->look_ang>>1)+175,looking_arc-gun_pos+215,
                             0xF01,
                             siz * (1.5 + (fakekrand() & 255)  * 0.00390625),
                             siz * (1.5 + (fakekrand() & 255)  * 0.00390625),
                             fakekrand()&255,4);
                myospal(weapon_xoffset+155-(p->look_ang>>1),looking_arc+290-gun_pos,FREEZE,gs,o,pal);
            }
            else myospal(weapon_xoffset+155-(p->look_ang>>1),looking_arc+290-gun_pos,FREEZE,gs,o,pal);

            break;

        case SHRINKER_WEAPON:
        case GROW_WEAPON:
            {
            weapon_xoffset += 28;
            looking_arc += 18;
            if((*kb) > 0)
            {
                weapon_xoffset += fakekrand()&3;
                gun_pos += (fakekrand()&3);
            }
            myospal(weapon_xoffset+206-(p->look_ang>>1),
             looking_arc+227-gun_pos,SHRINKER,gs,o,pal);
                sh = p->ammo_amount[GROW_WEAPON] > 0 ? -32 : gs+12;
            myospal(weapon_xoffset+149-(p->look_ang>>1),
             looking_arc+214-gun_pos,3884,sh,o,2);
                sh = p->ammo_amount[SHRINKER_WEAPON] > 0 ? -32 : gs+12;
            myospal(weapon_xoffset+155-(p->look_ang>>1),
             looking_arc+221-gun_pos,3886,sh,o,6);
                sh = -(sintable[p->random_club_frame&2047]>>10);
                pp = (cw == GROW_WEAPON) ? 2 : 6;
                if (*kb > 0) sh = -32;
            myospal(weapon_xoffset+171-(p->look_ang>>1),
             looking_arc+203-gun_pos,3885,sh,o,
                    pp);
            }
            break;
    }

    displayloogie(snum);

}
#else
#pragma GLOBAL_ASM("nonmatchings/src/display2/displayweapon.s")

short D_800EE888[] = { 0,1,1,0,0 };
char D_800EE894[] = { 0,0,0,0,0,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2 };
signed char D_800EE8AC[] = { 0,0,2,2,2,2,2,2,0,0,0 };
#endif

void cameratext(short i)
{
    myospal(44, 33, CAMCORNER, -32, 0, 255);
    myospal(276, 33, CAMCORNER+1, -32, 0, 255);
    myospal(44, 207, CAMCORNER+1, -32, 12, 255);
    myospal(276, 207, CAMCORNER+1, -32, 8, 255);
    if(totalclock&16)
        myospal(66, 0x20, CAMLIGHT, -32, 0, 0xFF);
}

void displayrest(long smoothratio)
{
    long a, i, j;

    struct player_struct *pp;
    long cposx,cposy,cang;
            int t;

    pp = &ps[screenpeek];

    clearscreen();

    gDPSetColorDither(pkt++, G_CD_MAGICSQ);

    if(ud.camerasprite == -1)
    {
        if( ud.overhead_on != 2 )
        {
            for (splitscreenpeek = 0; splitscreenpeek < numplayers; splitscreenpeek++)
            {
                short bx1, by1, bx2, by2;
                getviewborder(splitscreenpeek, &bx1, &by1, &bx2, &by2);

                gDPSetScissor(pkt++, G_SC_NON_INTERLACE, bx1, by1, bx2, by2);
                
                if(ps[splitscreenpeek].newowner >= 0)
                    cameratext(ps[splitscreenpeek].newowner);
                else
                {
                    displayweapon(splitscreenpeek);
                    if(ps[splitscreenpeek].over_shoulder_on == 0 )
                        displaymasks(splitscreenpeek);
                }
            }
        }

        if( ud.overhead_on > 0 )
        {
                //smoothratio = min(max(smoothratio,0),65536);
                dointerpolations(smoothratio);
                if( ud.scrollmode == 0 )
                {
                     if(pp->newowner == -1)
                     {
                         if (screenpeek == myconnectindex && numplayers > 1)
                         {
                             cposx = omyx+mulscale16((long)(myx-omyx),smoothratio);
                             cposy = omyy+mulscale16((long)(myy-omyy),smoothratio);
                             cang = omyang+mulscale16((long)(((myang+1024-omyang)&2047)-1024),smoothratio);
                         }
                         else
                         {
                              cposx = pp->oposx+mulscale16((long)(pp->posx-pp->oposx),smoothratio);
                              cposy = pp->oposy+mulscale16((long)(pp->posy-pp->oposy),smoothratio);
                              cang = pp->oang+mulscale16((long)(((pp->ang+1024-pp->oang)&2047)-1024),smoothratio);
                         }
                    }
                    else
                    {
                        cposx = pp->oposx;
                        cposy = pp->oposy;
                        cang = pp->oang;
                    }
                }
                else
                {

                     ud.fola += ud.folavel>>3;
                     ud.folx += (ud.folfvel*sintable[(512+2048-ud.fola)&2047])>>14;
                     ud.foly += (ud.folfvel*sintable[(512+1024-512-ud.fola)&2047])>>14;

                     cposx = ud.folx;
                     cposy = ud.foly;
                     cang = ud.fola;
                }

                restoreinterpolations();
        }
    }

    setrotatespriteflags(0xff,0xff,0xff,0x100);
    coolgaugetext();
    operatefta();

    if (ud.multimode > 1 && ud.coop == 0 && dukematch_mode == 2)
    {
        if (death_fade == 0)
        {
            char buf[10];
            short v2;
            t = max(0, D_800D3780);
            if (t < 300)
                setrotatespriteflags(0xFF, 0x64, 0x64, 0x100);
            else
                setrotatespriteflags(0xFF, 0xFF, 0xFF, 0x100);

            t = max(0, D_800D3780);
            t /= 30;
            v2 = (numplayers > 1 ) ? 0x74 : 0x14;

            gametext(-1, v2, ":");
            sprintf(buf, "%02d", t / 60);
            gametext(0x9E - func_80099488(buf), v2, buf);
            sprintf(buf, "%02d", t % 60);
            gametext(0xA2, v2, buf);
        }
        else
            D_800D3780 = D_800D3784;
    }
}

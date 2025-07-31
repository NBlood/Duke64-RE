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

#pragma GLOBAL_ASM("nonmatchings/src/display2/displayweapon.s")

#pragma GLOBAL_ASM("nonmatchings/src/display2/cameratext.s")

#pragma GLOBAL_ASM("nonmatchings/src/display2/displayrest.s")



short D_800EE888[] = { 0,1,1,0,0 };
char D_800EE894[] = { 0,0,0,0,0,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2 };
signed char D_800EE8AC[] = { 0,0,2,2,2,2,2,2,0,0,0 };

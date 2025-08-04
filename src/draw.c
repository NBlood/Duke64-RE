#include "common.h"
#include "duke3d.h"

short bartilescrollspeed;
short bartilescroll;
short barscrollstop;
short borderx1;
short borderx2;
short bordery1;
short bordery2;

short weapon_sprites[] = {
    BOOTS, FIRSTGUNSPRITE, SHOTGUNSPRITE, CHAINGUNSPRITE,
    DEVISTATORSPRITE, HEAVYHBOMB, SHRINKERSPRITE, GROWSPRITEICON,
    RPGSPRITE, TRIPBOMBSPRITE, FREEZESPRITE, HEAVYHBOMB,
    FIRSTGUNSPRITE, SHOTGUNSPRITE, RPGSPRITE
};

short ammo_sprites[] = {
    BOOTS, AMMO, SHOTGUNAMMO, BATTERYAMMO,
    DEVISTATORAMMO, HBOMBAMMO, CRYSTALAMMO, GROWAMMO,
    RPGAMMO, TRIPBOMBSPRITE, FREEZEAMMO, HBOMBAMMO,
    TILE34, TILE50, TILE43
};

void getviewborder(short snum, short *x1, short *y1, short *x2, short *y2)
{
    switch (numplayers)
    {
        case 1:
            *x1 = 22;
            *x2 = 298;
            *y1 = 16;
            *y2 = 224;
            break;
        case 2:
            *x1 = 22;
            *x2 = 298;
            if (snum == 0)
            {
                *y1 = 16;
                *y2 = 119;
            }
            else
            {
                *y1 = 121;
                *y2 = 224;
            }
            break;
        case 3:
        case 4:
            if (snum == 0 || snum == 2)
            {
                *x1 = 22;
                *x2 = 159;
            }
            else
            {
                *x1 = 161;
                *x2 = 298;
            }
            if (snum == 0 || snum == 1)
            {
                *y1 = 16;
                *y2 = 119;
            }
            else
            {
                *y1 = 121;
                *y2 = 224;
            }
            break;
    }
}


unsigned short invtilescale(unsigned short sc, unsigned short tile)
{
    tile = gettileid(tile);
    if (tileinfo[tile].dim2 == 0)
        return 0;
    return (sc * 100) / tileinfo[tile].dim2;
}


void printnumber(unsigned short x, unsigned short y, char* s)
{
    short tile;

    gDPSetRenderMode(pkt++, G_RM_ZB_XLU_SURF, G_RM_ZB_XLU_SURF2);
    while (*s != 0 && *s != '\n' &&  *s != '\r')
    {
        tile = *s;
        if (tile >= '0' && tile <= '9')
            tile = tile + 3689;
        if (tile == '%') 
            tile = 3736;
        rotatesprite_simple(x, y, tile);
        x = x + 4;
        s++;
    }
}

void setrotatespriteflags(short r, short g, short b, short a)
{
    short a2;
    r = max(0, min(255, r));
    g = max(0, min(255, g));
    b = max(0, min(255, b));
    a2 = max(0, min(256, a));

    gDPSetPrimColor(pkt++, 0, 0, r, g, b, a2);
    gDPSetEnvColor(pkt++, r, g, b, 0);
    gDPSetBlendColor(pkt++, r, g, b, 0);

    if (a > 255)
    {
        if (a == 666)
        {
            gDPSetRenderMode(pkt++, G_RM_TEX_EDGE, G_RM_TEX_EDGE2);
        }
        else
        {
            gDPSetRenderMode(pkt++, G_RM_ZB_XLU_SURF, G_RM_ZB_XLU_SURF2);
        }
        gDPSetCombineMode(pkt++, G_CC_BLENDIDECALA, G_CC_BLENDIDECALA);
    }
    else
    {
        gDPSetRenderMode(pkt++, G_RM_ZB_XLU_SURF, G_RM_ZB_XLU_SURF2);
        gDPSetCombineMode(pkt++, G_CC_HILITERGBA, G_CC_HILITERGBA);
    }
}

void resetsbarscroll(void)
{
    if (numplayers > 1)
    {
        barscrollstop = 1;
        bartilescroll = -99;
    }
    else
    {
        bartilescrollspeed = 12;
        bartilescroll = -24;
        barscrollstop = 0;
    }
}

void drawbarbg(short);
void drawbaroverlay(short);
void displayinvweapons(short);
void drawstatusbar(short);

void coolgaugetext(void)
{
    short i;

    for (i = 0; i < numplayers; i++)
    {
        if (ps[i].newowner < 0 && ps[i].last_extra > 0)
        {
            getviewborder(i, &borderx1, &bordery1, &borderx2, &bordery2);
            gDPSetScissor(pkt++, G_SC_NON_INTERLACE, borderx1, bordery1, borderx2, bordery2);
            displayinvweapons(i);
            drawbarbg(i);
            drawstatusbar(i);
            drawbaroverlay(i);
            if (ps[i].crosshair)
            {
                gDPSetRenderMode(pkt++, G_RM_ZB_XLU_SURF, G_RM_ZB_XLU_SURF2);
                rotatesprite_simplest((borderx1 + borderx2) / 2, (bordery1 + bordery2) / 2, 0x9DB);
            }
        }
    }
    gDPSetScissor(pkt++, G_SC_NON_INTERLACE, 21, 15, 299, 225);
}

void drawbarbg(short snum)
{
    short scroll;

    if (numplayers > 1)
    {
        barscrollstop = 1;
        return;
    }
    if (barscrollstop)
        scroll = 56;
    else
        scroll = bartilescroll;
    setrotatespriteflags(255, 255, 255, 666);
    rotatesprite(scroll + 11, bordery2 - 14, 100.0f, 100.0f, 3632, 0);
    rotatesprite(319 - scroll, bordery2 - 14, 100.0f, 100.0f, 3631, 0);
}

void drawbaroverlay(short snum)
{
    if (bartilescroll < -70)
        return;
    setrotatespriteflags(255, 255, 255, 666);
    rotatesprite(bartilescroll + 5, bordery2 - 14, 100.0f, 100.0f, 3633, 0);
    rotatesprite(314 - bartilescroll, bordery2 - 14, 100.0f, 100.0f, 3633, 4);

    if (snum + 1 == numplayers)
    {
        if (barscrollstop)
        {
            bartilescroll -= bartilescrollspeed;
            bartilescrollspeed++;
        }
        else
        {
            bartilescroll += bartilescrollspeed;
            bartilescrollspeed--;
            if (bartilescroll > 56)
            {
                bartilescroll = 56;
                barscrollstop = 1;
                bartilescrollspeed = 0;
            }
            if (bartilescrollspeed < 1)
                bartilescrollspeed = 1;
        }
    }
}

void displayinvsprite(short x, short y, short ang, unsigned short tile)
{
    int ox;
    float siz;
    float shade;

    ox = (sintable[(ang * 6) & 2047] * 60) >> 14;
    siz = sintable[(ang * 6 + 512) & 2047] / 65536.0 + 0.75;
    shade = siz * 5 - 4;
    setrotatespriteflags(255 * shade, 255 * shade, 255 * shade, ((siz * 2) - 1.0) * 256);
    rotatesprite(x + ox, y,
                 invtilescale(16, tile) * siz,
                 invtilescale(16, tile) * siz, tile, 0);
}

void displayinvweapons(short snum)
{
    short x;
    short y;
    short n1;
    short p1;
    short n2;
    short p2;
    unsigned short cw;
    
    if (ps[snum].f_378 > 0)
    {
        x = (borderx1 + borderx2) / 2;
        y = (bordery2 - ps[snum].f_378) + 0xA;
        cw = ps[snum].f_372;
        switch (cw) 
        { 
            case PISTOL2_WEAPON:
                cw = PISTOL_WEAPON;
                break;
            case SHOTGUN2_WEAPON:
                cw = SHOTGUN_WEAPON;
                break;
            case RPG2_WEAPON:
                cw = RPG_WEAPON;
                break;
        }
        p1 = getnextweapon(&ps[snum], cw, -1);
        n1 = getnextweapon(&ps[snum], cw, 1);
        p2 = getnextweapon(&ps[snum], p1, -1);
        n2 = getnextweapon(&ps[snum], n1, 1);
        displayinvsprite(x, y, (-120 - ps[snum].f_374), weapon_sprites[p2]);
        displayinvsprite(x, y, (120 - ps[snum].f_374), weapon_sprites[n2]);
        displayinvsprite(x, y, (-60 - ps[snum].f_374), weapon_sprites[p1]);
        displayinvsprite(x, y, (60 - ps[snum].f_374), weapon_sprites[n1]);
        displayinvsprite(x, y, -ps[snum].f_374, weapon_sprites[cw]);
    }
}

void drawstatusbar(short snum)
{
    unsigned short itile;
    unsigned short amount;
    char buf[8];
    char toggle;
    int x;
    int y;
    unsigned short siz;
    short red;
    short green;
    short blue;
    unsigned short atile;
    unsigned short cw;
    short alpha;
    int unk;

    if (barscrollstop == 0)
        return;

    if (ud.multimode > 1 && ud.coop == 0 && dukematch_mode != 4)
    {
        red = 200;
        green = 200;
        blue = 200;
        switch (snum)
        {
            case 0:
                red = 64;
                break;
            case 1:
                red = 64;
                blue = 64;
                break;
            case 2:
                green = 64;
                break;
            case 3:
                blue = 64;
                break;
        }
        sprintf(buf, "%d", ps[snum].frag - ps[snum].fraggedself);
        setrotatespriteflags(red, green, blue, 666);
        gametext(borderx2 - func_80099488(buf) - 4, bordery1 + 2, buf);
        setrotatespriteflags(255, 255, 255, 666);
    }
    if (ps[snum].f_372 != 0)
    {
        if (numplayers < 3)
        {
            atile = ammo_sprites[ps[snum].f_372];
            siz = invtilescale(16, atile);
            if (numplayers < 3)
                x = borderx1 + 55;
            else
                x = borderx1 + 15;
            y = bordery2 - 12;
            setrotatespriteflags(128, 128, 128, 256);
            rotatesprite(x, y - 1, siz, siz, atile, 0);
        }
        cw = ps[snum].f_372;
        if (cw == HANDREMOTE_WEAPON)
            cw = HANDBOMB_WEAPON;
        sprintf(buf, "%d\n", ps[snum].ammo_amount[cw]);
        if (cw == 12 || cw == 13 || cw == 14)
            setrotatespriteflags(255, 255, 64, 666);
        else
            setrotatespriteflags(255, 255, 255, 666);
        
        if (numplayers < 3)
            printnumber(x - 2, y - 4, buf);
        else
            gametext(borderx1 + 2, bordery2 - 10, buf);
    }
    toggle = 0;
    switch (ps[snum].inven_icon)
    {
        case 1:
            itile = FIRSTAID;
            amount = ps[snum].firstaid_amount;
            break;
        case 2:
            itile = STEROIDS;
            amount = (ps[snum].steroids_amount + 3) / 4;
            if (amount < 100)
                toggle = 1;
            break;
        case 3:
            itile = HOLODUKE;
            amount = (ps[snum].holoduke_amount + 15) / 24;
            if (ps[snum].holoduke_on > 0)
                toggle = 1;
            break;
        case 4:
            itile = JETPACK;
            amount = (ps[snum].jetpack_amount + 15) / 16;
            if (ps[snum].jetpack_on > 0)
                toggle = 1;
            break;
        case 5:
            itile = HEATSENSOR;
            amount = ps[snum].heat_amount / 12;
            if (ps[snum].heat_on > 0)
                toggle = 1;
            break;
        case 6:
            itile = AIRTANK;
            amount = (ps[snum].scuba_amount + 63) / 64;
            break;
        case 7:
            itile = BOOTS;
            amount = ps[snum].boot_amount / 2;
            break;
        case 8:
            itile = SHIELD;
            amount = ps[snum].shield_amount;
            break;
        case 0:
            itile = 0;
            amount = 0;
            break;
    }
    if (numplayers > 2 && toggle != 0 && amount < 30)
        ps[snum].invdisptime = 20;

    if (numplayers > 2 && ps[snum].invdisptime == 0)
        itile = 0;

    if (itile)
    {
        siz = invtilescale(16, itile);
        x = borderx2 - 23;
        y = bordery2 - 10;
        if (numplayers > 2)
            alpha = (ps[snum].invdisptime << 8) / 10;
        else
            alpha = 256;

        setrotatespriteflags(255, 255, 255, alpha);
        rotatesprite(x - 17, y - 1, siz, siz, itile, 0);
        sprintf(buf, "%d%%\n", amount);
        if (amount < 0x1E)
            setrotatespriteflags(255, 255, 64, alpha);
        else
            setrotatespriteflags(255, 255, 255, alpha);

        printnumber(x, y, buf);
        if (ps[snum].inven_icon > 2)
        {
            if (ps[snum].inven_icon > 5)
                rotatesprite_simple(x, y - 10, 3747);
            else if (toggle)
                rotatesprite_simple(x, y - 10, 3749);
            else
                rotatesprite_simple(x, y - 10, 3748);
        }
    }
    sprintf(buf, "%d", ps[snum].last_extra);
    if (ps[snum].last_extra < 30)
        setrotatespriteflags(255, 64, 64, 666);
    else
        setrotatespriteflags(255, 255, 255, 666);

    if (numplayers < 3)
        gametext(borderx1 - func_80099488(buf) / 2 + 15, bordery2 - 18, buf);
    else
        menutext(borderx1 + 2, bordery1 + 2, buf);

    setrotatespriteflags(255, 255, 255, 666);
    if (ud.multimode == 1 || ud.coop == 1)
    {
        if (numplayers < 2)
        {
            if (ps[snum].got_access & 1)
                rotatesprite(borderx2 - 61, bordery2 - 24, 100, 100, 3621, 0);
            if (ps[snum].got_access & 2)
                rotatesprite(borderx2 - 65, bordery2 - 18, 100, 100, 3622, 0);
            if (ps[snum].got_access & 4)
                rotatesprite(borderx2 - 65, bordery2 - 10, 100, 100, 3623, 0);
        }
        else
        {
            if (ps[snum].got_access & 1)
                rotatesprite(borderx2 - 30, bordery1 + 8, 100, 100, 3621, 0);
            if (ps[snum].got_access & 2) 
                rotatesprite(borderx2 - 20, bordery1 + 7, 100, 100, 3622, 0);
            if (ps[snum].got_access & 4) 
                rotatesprite(borderx2 - 10, bordery1 + 7, 100, 100, 3623, 0);
        }
    }
}


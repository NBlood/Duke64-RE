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

extern char everyothertime;
short which_palookup = 9;


void resetplayerstats(short snum)
{
    struct player_struct *p;
    short i;

    p = &ps[snum];

    ud.show_help        = 0;
    ud.showallmap       = 0;
    p->dead_flag        = 0;
    p->wackedbyactor    = -1;
    p->falling_counter  = 0;
    p->quick_kick       = 0;
    //p->subweapon        = 0;
    p->last_full_weapon = 0;
    p->ftq              = 0;
    p->fta              = 0;
    p->tipincs          = 0;
    p->buttonpalette    = 0;
    p->actorsqu         =-1;
    p->invdisptime      = 0;
    p->refresh_inventory= 0;
    p->last_pissed_time = 0;
    p->holster_weapon   = 0;
    p->pycount          = 0;
    p->pyoff            = 0;
    p->opyoff           = 0;
    p->loogcnt          = 0;
    p->angvel           = 0;
    p->weapon_sway      = 0;
//    p->select_dir       = 0;
    p->extra_extra8     = 0;
    p->crack_time       = 0;
    p->hbomb_hold_delay = 0;
    p->transporter_hold = 0;
    p->wantweaponfire  = -1;
    p->hurt_delay       = 0;
    p->footprintcount   = 0;
    p->footprintpal     = 0;
    p->footprintshade   = 0;
    p->jumping_toggle   = 0;
    p->ohoriz = p->horiz= 140;
    p->horizoff         = 0;
    p->bobcounter       = 0;
    p->on_ground        = 0;
    p->player_par       = 0;
    p->return_to_center = 9;
    p->airleft          = 15*26;
    p->rapid_fire_hold  = 0;
    p->toggle_key_flag  = 0;
    p->access_spritenum = -1;
    if(ud.multimode > 1 && ud.coop != 1 )
        p->got_access = 7;
    else p->got_access      = 0;
    p->random_club_frame= 0;
    pus = 1;
    p->on_warping_sector = 0;
    p->spritebridge      = 0;
    p->palette = NULL;

    if(p->steroids_amount < 400 )
    {
        p->steroids_amount = 0;
        p->inven_icon = 0;
    }
    p->heat_on =            0;
    p->jetpack_on =         0;
    p->holoduke_on =       -1;

    p->look_ang          = 512 - ((ud.level_number&1)<<10);

    p->rotscrnang        = 0;
    p->newowner          =-1;
    p->jumping_counter   = 0;
    p->hard_landing      = 0;
    p->posxv             = 0;
    p->posyv             = 0;
    p->poszv             = 0;
    p->fricxv            = 0;
    p->fricyv            = 0;
    p->somethingonplayer =-1;
    //p->one_eighty_count  = 0;
    //p->cheat_phase       = 0;

    p->on_crane          = -1;

    if(p->curr_weapon == PISTOL_WEAPON)
        p->kickback_pic  = 5;
    else p->kickback_pic = 0;

    p->weapon_pos        = 6;
    p->walking_snd_toggle= 0;
    p->weapon_ang        = 0;

    p->knuckle_incs      = 1;
    p->fist_incs = 0;
    p->knee_incs         = 0;
    p->jetpack_on        = 0;

    p->f_370 = 0;
    p->f_374 = 0;
    p->f_376 = 0;
    p->f_378 = 0;
    p->f_372 = p->curr_weapon;
}



void resetweapons(short snum)
{
    short  weapon;
    struct player_struct *p;

    p = &ps[snum];

    for ( weapon = PISTOL_WEAPON; weapon < MAX_WEAPONS; weapon++ )
        p->gotweapon[weapon] = 0;
    for ( weapon = PISTOL_WEAPON; weapon < MAX_WEAPONS; weapon++ )
        p->ammo_amount[weapon] = 0;

    p->weapon_pos = 6;
    p->kickback_pic = 5;
    p->f_372 = p->curr_weapon = PISTOL_WEAPON;
    p->gotweapon[PISTOL_WEAPON] = 1;
    p->gotweapon[PISTOL2_WEAPON] = 1;
    p->gotweapon[KNEE_WEAPON] = 1;
    p->ammo_amount[PISTOL_WEAPON] = 48;
    p->gotweapon[HANDREMOTE_WEAPON] = 1;
    p->ammo_amount[HANDREMOTE_WEAPON] = 0;
    p->last_weapon = -1;

    p->last_pissed_time = 0;
    p->holster_weapon = 0;
}

void resetinventory(short snum)
{
    struct player_struct *p;
    short i;

    p = &ps[snum];

    p->inven_icon       = 0;
    p->boot_amount = 0;
    p->scuba_on =           0;p->scuba_amount =         0;
    p->heat_on = 0;p->heat_amount        = 0;
    p->jetpack_on =         0;p->jetpack_amount =       0;
    p->shield_amount =      max_armour_amount;
    p->holoduke_on = -1;
    p->holoduke_amount =    0;
    p->firstaid_amount = 0;
    p->steroids_amount = 0;
}


void resetprestat(short snum,char g)
{
    struct player_struct *p;
    short i;

    p = &ps[snum];

    spriteqloc = 0;
    for(i=0;i<spriteqamount;i++) spriteq[i] = -1;

    p->hbomb_on          = 0;
    p->pals_time         = 0;
    p->toggle_key_flag   = 0;
    p->secret_rooms      = 0;
    p->max_secret_rooms  = 0;
    p->actors_killed     = 0;
    p->max_actors_killed = 0;
    p->f_36e = 0;
    p->f_36d = 0;
    p->lastrandomspot = 0;
    p->weapon_pos = 6;
    p->kickback_pic = 5;
    p->last_weapon = -1;
    p->weapreccnt = 0;
    p->holster_weapon = 0;
    p->last_pissed_time = 0;

    p->visibility = 0x200;

    screenpeek              = myconnectindex;
    numanimwalls            = 0;
    numcyclers              = 0;
    animatecnt              = 0;
    //parallaxtype            = 0;
    randomseed              = 17L;
    ud.pause_on             = 0;
    ud.camerasprite         =-1;
    ud.eog                  = 0;
    tempwallptr             = 0;
    camsprite               =-1;
    earthquaketime          = 0;

    numinterpolations = 0;
    startofdynamicinterpolations = 0;

    if( (g&MODE_EOL) != MODE_EOL || p->last_extra == 0 )
    {
        resetweapons(snum);
        resetinventory(snum);
    }
    if(p->curr_weapon == HANDREMOTE_WEAPON)
    {
        p->ammo_amount[HANDBOMB_WEAPON]++;
        p->curr_weapon = HANDBOMB_WEAPON;
    }

    p->timebeforeexit   = 0;
    p->customexitsound  = -1;
    ud.screen_size = 4;
    p->f_374 = 0;
    p->f_378 = 0;

}


void prelevel(char g)
{
    short i, nexti, j, startwall, endwall, lotaglist;
    short lotags[65];
    char p;

    for(p=0;p<ud.multimode;p++)
        resetprestat(p,g);

    for(i=0;i<numsectors;i++)
    {
        sector[i].extra = 256;

        switch(sector[i].lotag)
        {
            case 20:
            case 22:
                if( sector[i].floorz > sector[i].ceilingz)
                    sector[i].lotag |= 32768;
                continue;
        }

        if(sector[i].lotag == 32767) //Found a secret room
        {
            for(p=0;p<ud.multimode;p++)
                ps[p].max_secret_rooms++;
            continue;
        }

        if(sector[i].lotag == -1)
        {
            for(p=0;p<ud.multimode;p++)
            {
                ps[p].exitx = wall[sector[i].wallptr].x;
                ps[p].exity = wall[sector[i].wallptr].y;
            }
            continue;
        }
    }

    i = headspritestat[0];
    while(i >= 0)
    {
        nexti = nextspritestat[i];

        if(sprite[i].lotag == -1 && (sprite[i].cstat&16) )
        {
            for(p=0;p<ud.multimode;p++)
            {
                ps[p].exitx = SX;
                ps[p].exity = SY;
            }
        }
        else switch(PN)
        {
            case GPSPEED:
                sector[SECT].extra = SLT;
                deletesprite(i);
                break;

            case CYCLER:
                if(numcyclers >= MAXCYCLERS)
                    gameexit("\nToo many cycling sectors.");
                cyclers[numcyclers][0] = SECT;
                cyclers[numcyclers][1] = SLT;
                cyclers[numcyclers][2] = SS;
                cyclers[numcyclers][3] = sector[SECT].floorshade;
                cyclers[numcyclers][4] = SHT;
                cyclers[numcyclers][5] = (SA == 1536);
                numcyclers++;
                deletesprite(i);
                break;
        }
        i = nexti;
    }

    for(i=0;i < MAXSPRITES;i++)
    {
        if(sprite[i].statnum < MAXSTATUS)
        {
            if(PN == SECTOREFFECTOR && SLT == 14)
                continue;
            spawn(-1,i);
        }
    }

    for(i=0;i < MAXSPRITES;i++)
        if(sprite[i].statnum < MAXSTATUS)
        {
            if( PN == SECTOREFFECTOR && SLT == 14 )
                spawn(-1,i);
        }

    lotaglist = 0;

    i = headspritestat[0];
    while(i >= 0)
    {
        switch(PN)
        {
            case DIPSWITCH:
            case DIPSWITCH2:
            case ACCESSSWITCH:
            case PULLSWITCH:
            case HANDSWITCH:
            case SLOTDOOR:
            case LIGHTSWITCH:
            case SPACELIGHTSWITCH:
            case SPACEDOORSWITCH:
            case FRANKENSTINESWITCH:
            case LIGHTSWITCH2:
            case POWERSWITCH1:
            case LOCKSWITCH1:
            case POWERSWITCH2:
                break;
            case DIPSWITCH+1:
            case DIPSWITCH2+1:
            case PULLSWITCH+1:
            case HANDSWITCH+1:
            case SLOTDOOR+1:
            case LIGHTSWITCH+1:
            case SPACELIGHTSWITCH+1:
            case SPACEDOORSWITCH+1:
            case FRANKENSTINESWITCH+1:
            case LIGHTSWITCH2+1:
            case POWERSWITCH1+1:
            case LOCKSWITCH1+1:
            case POWERSWITCH2+1:
                for(j=0;j<lotaglist;j++)
                    if( SLT == lotags[j] )
                        break;

                if( j == lotaglist )
                {
                    lotags[lotaglist] = SLT;
                    lotaglist++;
                    if(lotaglist > 64)
                        gameexit("\nToo many switches (64 max).");

                    j = headspritestat[3];
                    while(j >= 0)
                    {
                        if(sprite[j].lotag == 12 && sprite[j].hitag == SLT)
                            hittype[j].temp_data[0] = 1;
                        j = nextspritestat[j];
                    }
                }
                break;
        }
        i = nextspritestat[i];
    }

    mirrorcnt = 0;

    for( i = 0; i < numwalls; i++ )
    {
        walltype *wal;
        wal = &wall[i];

        if(wal->overpicnum == MIRROR && (wal->cstat&32) != 0)
        {
            j = wal->nextsector;

            if(mirrorcnt > 63)
                gameexit("\nToo many mirrors (64 max.)");
            if ( (j >= 0) && sector[j].ceilingpicnum != MIRROR )
            {
                sector[j].ceilingpicnum = MIRROR;
                sector[j].floorpicnum = MIRROR;
                mirrorwall[mirrorcnt] = i;
                mirrorsector[mirrorcnt] = j;
                mirrorcnt++;
                continue;
            }
        }

        if(numanimwalls >= MAXANIMWALLS)
            gameexit("\nToo many 'anim' walls (max 512.)");

        animwall[numanimwalls].tag = 0;
        animwall[numanimwalls].wallnum = 0;

        switch(wal->overpicnum)
        {
            case FANSHADOW:
            case FANSPRITE:
                wall->cstat |= 65;
                animwall[numanimwalls].wallnum = i;
                numanimwalls++;
                break;

            case W_FORCEFIELD:
            case W_FORCEFIELD+1:
            case W_FORCEFIELD+2:
                if(wal->shade > 31)
                    wal->cstat = 0;
                else wal->cstat |= 85+256;


                if(wal->lotag && wal->nextwall >= 0)
                    wall[wal->nextwall].lotag =
                        wal->lotag;

            case BIGFORCE:

                animwall[numanimwalls].wallnum = i;
                numanimwalls++;

                continue;
        }

        wal->extra = -1;

        switch(wal->picnum)
        {
            case WATERTILE2:
                break;

            case TECHLIGHT2:
            case TECHLIGHT4:
                break;
            case W_TECHWALL1:
            case W_TECHWALL2:
            case W_TECHWALL3:
            case W_TECHWALL4:
                animwall[numanimwalls].wallnum = i;
//                animwall[numanimwalls].tag = -1;
                numanimwalls++;
                break;
            case SCREENBREAK6:
            case SCREENBREAK7:
            case SCREENBREAK8:
                animwall[numanimwalls].wallnum = i;
                animwall[numanimwalls].tag = -1;
                numanimwalls++;
                break;

            case FEMPIC1:
            case FEMPIC2:
            case FEMPIC3:

                wal->extra = wal->picnum;
                animwall[numanimwalls].tag = -1;

                animwall[numanimwalls].wallnum = i;
                animwall[numanimwalls].tag = wal->picnum;
                numanimwalls++;
                break;

            case SCREENBREAK1:
            case SCREENBREAK2:
            case SCREENBREAK3:
            case SCREENBREAK4:
            case SCREENBREAK5:

            case SCREENBREAK9:
            case SCREENBREAK10:
            case SCREENBREAK11:
            case SCREENBREAK12:
            case SCREENBREAK13:
            case SCREENBREAK14:
            case SCREENBREAK15:
            case SCREENBREAK16:
            case SCREENBREAK17:
            case SCREENBREAK18:
            case SCREENBREAK19:

                animwall[numanimwalls].wallnum = i;
                animwall[numanimwalls].tag = wal->picnum;
                numanimwalls++;
                break;
        }
    }

    //Invalidate textures in sector behind mirror
    for(i=0;i<mirrorcnt;i++)
    {
        startwall = sector[mirrorsector[i]].wallptr;
        endwall = startwall + sector[mirrorsector[i]].wallnum;
        for(j=startwall;j<endwall;j++)
        {
            wall[j].picnum = MIRROR;
            wall[j].overpicnum = MIRROR;
        }
    }
}


void newgame(char vn,char ln,char sk,char pi)
{
    struct player_struct *p = &ps[pi];
    short i;

    ud.secretlevel =    0;
    ud.from_bonus = 0;
    
    ud.last_level = -1;
    p->zoom            = 768;
    p->gm              = 0;
    display_mirror =        0;

    for (i = 0; i < ud.multimode; i++)
    {
        ps[i].frag = 0;
        ps[i].fraggedself = 0;
        D_801C94D8[i] = 0;
    }
    
    if (ud.multimode >= 2 && ud.m_coop == 0 && dukematch_mode == 4)
        sprintf(&fta_quotes[13][0], " ");
    else if (numplayers >= 3)
        sprintf(&fta_quotes[13][0], "OPEN TO RESTART");
    else
        sprintf(&fta_quotes[13][0], "PRESS OPEN TO RESTART LEVEL");
}


void resetpspritevars(char g)
{
    short i, j, nexti;
    long firstx,firsty;
    spritetype *s;

    EGS(ps[0].cursectnum,ps[0].posx,ps[0].posy,ps[0].posz,
        APLAYER,0,0,0,ps[0].ang,0,0,0,10);

    for(i=0;i<ud.multimode;i++)
    {
        resetplayerstats(i);
        ps[i].pals[0] = 0;
        ps[i].pals[1] = 0;
        ps[i].pals[2] = 0;
        ps[i].pals_time = 64;
        death_fade = 1;
    }

    numplayersprites = 0;

    which_palookup = 9;
    j = 0;
    i = headspritestat[10];
    while(i >= 0)
    {
        nexti = nextspritestat[i];
        s = &sprite[i];

        if( numplayersprites == 20)
            gameexit("\nToo many player sprites (max 20.)\n");

        if(numplayersprites == 0)
        {
            firstx = ps[0].posx;
            firsty = ps[0].posy;
        }

        po[numplayersprites].ox = s->x;
        po[numplayersprites].oy = s->y;
        po[numplayersprites].oz = s->z;
        po[numplayersprites].oa = s->ang;
        po[numplayersprites].os = s->sectnum;

        numplayersprites++;
        if(j < ud.multimode)
        {
            s->owner = i;
            s->shade = 0;
            s->xrepeat = 42;
            s->yrepeat = 36;
            s->cstat = 1+256;
            s->xoffset = 0;
            s->clipdist = 64;

            if( (g&MODE_EOL) != MODE_EOL || ps[j].last_extra == 0)
            {
                ps[j].last_extra = max_player_health;
                s->extra = max_player_health;
            }
            else s->extra = ps[j].last_extra;

            s->yvel = j;

            if(s->pal == 0)
            {
                s->pal = ps[j].palookup = which_palookup;
                which_palookup++;
                if( which_palookup >= 17 ) which_palookup = 9;
            }
            else ps[j].palookup = s->pal;

            ps[j].i = i;
            ps[j].frag_ps = j;
            hittype[i].owner = i;

            hittype[i].bposx = ps[j].bobposx = ps[j].oposx = ps[j].posx =        s->x;
            hittype[i].bposy = ps[j].bobposy = ps[j].oposy = ps[j].posy =        s->y;
            hittype[i].bposz = ps[j].oposz = ps[j].posz =        s->z;
            ps[j].oang  = ps[j].ang  =        s->ang;

            updatesector(s->x,s->y,&ps[j].cursectnum);

            j++;

        }
        else deletesprite(i);
        i = nexti;
    }
}

void clearfrags(void)
{
    short i, j;

    for(i = 0;i<MAXPLAYERS;i++)
    {
        ps[i].frag = ps[i].fraggedself = 0;
        for (j = 0; j < MAXPLAYERS; j++)
            frags[i][j] = 0;
    }
}

void resettimevars(void)
{
    vel = svel = angvel = horiz = 0;

    totalclock = 0L;
    ototalclock = 0L;
    lockclock = 0L;
}

void resetmys(void)
{
      myx = omyx = ps[myconnectindex].posx;
      myy = omyy = ps[myconnectindex].posy;
      myz = omyz = ps[myconnectindex].posz;
      myxvel = myyvel = myzvel = 0;
      myang = omyang = ps[myconnectindex].ang;
      myhoriz = omyhoriz = ps[myconnectindex].horiz;
      myhorizoff = omyhorizoff = ps[myconnectindex].horizoff;
      mycursectnum = ps[myconnectindex].cursectnum;
      myjumpingcounter = ps[myconnectindex].jumping_counter;
      myjumpingtoggle = ps[myconnectindex].jumping_toggle;
      myonground = ps[myconnectindex].on_ground;
      myhardlanding = ps[myconnectindex].hard_landing;
      myreturntocenter = ps[myconnectindex].return_to_center;
}

void enterlevel(char g)
{
    short i;

    if( (g&MODE_DEMO) != MODE_DEMO ) ud.recstat = ud.m_recstat;
    ud.marker = 0;
    ud.respawn_monsters = ud.m_respawn_monsters;
    ud.respawn_items    = ud.m_respawn_items;
    ud.respawn_inventory    = ud.m_respawn_inventory;
    ud.monsters_off = ud.m_monsters_off;
    ud.coop = ud.m_coop;
    ud.marker = 1;
    ud.ffire = ud.m_ffire;
    
    if (ud.multimode == 1 || ud.coop == 1)
    {
        ud.respawn_items = 0;
        ud.respawn_inventory = 0;
        ud.respawn_monsters = 0;
    }

    if (ud.player_skill == 4)
        ud.respawn_monsters = 1;

    if (ud.multimode > 1 && ud.coop == 0)
    {
        ud.monsters_off = 1;
        ud.respawn_items = 1;
        ud.respawn_inventory = 1;
        ud.respawn_monsters = 0;
    }

    if( (g&MODE_DEMO) == 0 && ud.recstat == 2)
        ud.recstat = 0;
    
    ud.screen_size = 0;
    
    if (do_restart)
    {
        if (levelnum > 0)
        {
            loadgame();
            g = MODE_EOL;
        }
        else
            g = MODE_RESTART;
        do_restart = 0;
        menu_levelnum = levelnum;
    }
    
    func_8005B5F0(levelnum);

    prelevel(g);

    allignwarpelevators();
    resetpspritevars(g);

    if( (g&MODE_GAME) || (g&MODE_EOL) )
        ps[0].gm = MODE_GAME;
    else if(g&MODE_RESTART)
    {
        if(ud.recstat == 2)
            ps[0].gm = MODE_DEMO;
        else ps[0].gm = MODE_GAME;
    }

    for(i=0;i<ud.multimode;i++)
        switch(sector[ps[i].cursectnum].floorpicnum)
        {
            case HURTRAIL:
            case FLOORSLIME:
            case FLOORPLASMA:
                resetweapons(i);
                resetinventory(i);
                ps[i].gotweapon[PISTOL_WEAPON] = 0;
                ps[i].gotweapon[PISTOL2_WEAPON] = 0;
                ps[i].ammo_amount[PISTOL_WEAPON] = 0;
                ps[i].curr_weapon = KNEE_WEAPON;
                ps[i].f_372 = KNEE_WEAPON;
                ps[i].kickback_pic = 0;
                break;
        }

      //PREMAP.C - replace near the my's at the end of the file

     resetmys();

     ps[0].palette = NULL;

     everyothertime = 0;
     global_random = 0;

     ud.last_level = ud.level_number+1;

     for(i=numinterpolations-1;i>=0;i--) bakipos[i] = *curipos[i];
    
     stopallsounds();
     func_80059398(0, -1);

     clearfrags();

     resettimevars();  // Here we go
    
     resetsbarscroll();
}

void pickrandomspot(short snum)
{
    struct player_struct *p;
    short i;

    p = &ps[snum];

    if( ud.multimode > 1 && ud.coop != 1)
        i = TRAND%numplayersprites;
    else i = snum;

    p->bobposx = p->oposx = p->posx = po[i].ox;
    p->bobposy = p->oposy = p->posy = po[i].oy;
    p->oposz = p->posz = po[i].oz;
    p->ang = po[i].oa;
    p->cursectnum = po[i].os;
}

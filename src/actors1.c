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

extern char numenvsnds,actor_tog;

void updateinterpolations()  //Stick at beginning of domovethings
{
	long i;

	for(i=numinterpolations-1;i>=0;i--) oldipos[i] = *curipos[i];
}


void setinterpolation(long *posptr)
{
	long i;

	if (numinterpolations >= MAXINTERPOLATIONS) return;
	for(i=numinterpolations-1;i>=0;i--)
		if (curipos[i] == posptr) return;
	curipos[numinterpolations] = posptr;
	oldipos[numinterpolations] = *posptr;
	numinterpolations++;
}

void stopinterpolation(long *posptr)
{
	long i;

	for(i=numinterpolations-1;i>=startofdynamicinterpolations;i--)
		if (curipos[i] == posptr)
		{
			numinterpolations--;
			oldipos[i] = oldipos[numinterpolations];
			bakipos[i] = bakipos[numinterpolations];
			curipos[i] = curipos[numinterpolations];
		}
}

void dointerpolations(long smoothratio)       //Stick at beginning of drawscreen
{
	long i, j, odelta, ndelta;

	ndelta = 0; j = 0;
	for(i=numinterpolations-1;i>=0;i--)
	{
		bakipos[i] = *curipos[i];
		odelta = ndelta; ndelta = (*curipos[i])-oldipos[i];
		if (odelta != ndelta) j = mulscale16(ndelta,smoothratio);
		*curipos[i] = oldipos[i]+j;
	}
}

void restoreinterpolations()  //Stick at end of drawscreen
{
	long i;

	for(i=numinterpolations-1;i>=0;i--) *curipos[i] = bakipos[i];
}

long ceilingspace(short sectnum)
{
    if( (sector[sectnum].ceilingstat&1) && sector[sectnum].ceilingpal == 0 )
    {
        switch(sector[sectnum].ceilingpicnum)
        {
            case MOONSKY1:
            case BIGORBIT1:
                return 1;
        }
    }
    return 0;
}

long floorspace(short sectnum)
{
    if( (sector[sectnum].floorstat&1) && sector[sectnum].ceilingpal == 0 )
    {
        switch(sector[sectnum].floorpicnum)
        {
            case MOONSKY1:
            case BIGORBIT1:
                return 1;
        }
    }
    return 0;
}

void addammo( short weapon,struct player_struct *p,short amount)
{
   p->ammo_amount[weapon] += amount;

   if( p->ammo_amount[weapon] > max_ammo_amount[weapon] )
        p->ammo_amount[weapon] = max_ammo_amount[weapon];
}

void addweapon( struct player_struct *p,short weapon)
{
    if ( p->gotweapon[weapon] == 0 )
    {
        p->gotweapon[weapon] = 1;
        if(weapon == SHRINKER_WEAPON)
            p->gotweapon[GROW_WEAPON] = 1;
        if(weapon == GROW_WEAPON)
            p->gotweapon[SHRINKER_WEAPON] = 1;
        if(weapon == PISTOL_WEAPON)
            p->gotweapon[PISTOL2_WEAPON] = 1;
        if(weapon == SHOTGUN_WEAPON)
            p->gotweapon[SHOTGUN2_WEAPON] = 1;
        if(weapon == RPG_WEAPON)
            p->gotweapon[RPG2_WEAPON] = 1;
        if(weapon == HANDBOMB_WEAPON)
            p->gotweapon[HANDREMOTE_WEAPON] = 1;
        return;
    }

    p->random_club_frame = 0;

    if(p->holster_weapon == 0)
    {
        p->weapon_pos = -1;
        p->last_weapon = p->curr_weapon;
    }
    else
    {
        p->weapon_pos = 10;
        p->holster_weapon = 0;
        p->last_weapon = -1;
    }

    p->kickback_pic = 0;
    p->curr_weapon = weapon;
    p->f_372 = weapon;

    switch(weapon)
    {
        case KNEE_WEAPON:
        case TRIPBOMB_WEAPON:
        case HANDREMOTE_WEAPON:
        case HANDBOMB_WEAPON:     break;
        case SHOTGUN_WEAPON:      spritesound(SHOTGUN_COCK,p->i);break;
        case PISTOL_WEAPON:       spritesound(INSERT_CLIP,p->i);break;
                    default:      spritesound(SELECT_WEAPON,p->i);break;
    }
}

void checkavailinven( struct player_struct *p )
{

    if(p->firstaid_amount > 0)
        p->inven_icon = 1;
    else if(p->steroids_amount > 0)
        p->inven_icon = 2;
    else if(p->holoduke_amount > 0)
        p->inven_icon = 3;
    else if(p->jetpack_amount > 0)
        p->inven_icon = 4;
    else if(p->heat_amount > 0)
        p->inven_icon = 5;
    else if(p->scuba_amount > 0)
        p->inven_icon = 6;
    else if(p->boot_amount > 0)
        p->inven_icon = 7;
    else if(p->shield_amount > 0)
        p->inven_icon = 8;
    else p->inven_icon = 0;
}

void checkavailweapon( struct player_struct *p )
{
    short i;
    short weap;

    short wchoice[14] = {
        SHOTGUN2_WEAPON, PISTOL2_WEAPON, SHOTGUN_WEAPON, CHAINGUN_WEAPON, GROW_WEAPON, DEVISTATOR_WEAPON,
        FREEZE_WEAPON, SHRINKER_WEAPON, RPG2_WEAPON, RPG_WEAPON, HANDBOMB_WEAPON, PISTOL_WEAPON,
        TRIPBOMB_WEAPON, KNEE_WEAPON
    };

    if(p->wantweaponfire >= 0)
    {
        weap = p->wantweaponfire;
        p->wantweaponfire = -1;

        if(weap == p->curr_weapon) return;
        else if( weap == KNEE_WEAPON || weap == HANDREMOTE_WEAPON
            || ( p->gotweapon[weap] && p->ammo_amount[weap] > 0 ) )
        {
            addweapon(p,weap);
            return;
        }
    }

    weap = p->curr_weapon;
    if( p->gotweapon[weap] && p->ammo_amount[weap] > 0 )
        return;

    for(i=0;i<14;i++)
    {
        weap = wchoice[i];
#ifdef VOLUMEONE
        if(weap > 6) continue;
#endif

        if( weap == 0 || ( p->gotweapon[weap] && p->ammo_amount[weap] > 0 ) )
            break;
    }

    // Found the weapon

    p->last_weapon  = p->curr_weapon;
    p->random_club_frame = 0;
    p->curr_weapon  = weap;
    p->f_372 = weap;
    p->kickback_pic = 0;
    if(p->holster_weapon == 1)
    {
        p->holster_weapon = 0;
        p->weapon_pos = 10;
    }
    else p->weapon_pos   = -1;
}

long ifsquished(short i, short p)
{
    sectortype *sc;
    char squishme;
    long floorceildist;

    if(PN == APLAYER && ud.clipping)
        return 0;

    if(PN == APLAYER && sprite[i].extra <= 0)
        return 0;

    sc = &sector[SECT];
    floorceildist = sc->floorz - sc->ceilingz;

    if(sc->lotag != 23)
    {
        if(sprite[i].pal == 1)
            squishme = floorceildist < (32<<8) && (sc->lotag&32768) == 0;
        else
            squishme = floorceildist < (12<<8); // && (sc->lotag&32768) == 0;
    }
    else squishme = 0;

    if( squishme )
    {
        FTA(10,&ps[p]);

        if(badguy(&sprite[i])) sprite[i].xvel = 0;

        if(sprite[i].pal == 1)
        {
            hittype[i].picnum = SHOTSPARK1;
            hittype[i].extra = 1;
            return 0;
        }

        return 1;
    }
    return 0;
}


int movesprite(short spritenum, long xchange, long ychange, long zchange, unsigned long cliptype)
{
    int retval;
    long daz,h, oldx, oldy;
    short dasectnum, a, cd;
    char bg;

    bg = badguy(&sprite[spritenum]);

    if(sprite[spritenum].statnum == 5 || (bg && sprite[spritenum].xrepeat < 4 ) )
    {
        sprite[spritenum].x += (xchange*TICSPERFRAME)>>2;
        sprite[spritenum].y += (ychange*TICSPERFRAME)>>2;
        sprite[spritenum].z += (zchange*TICSPERFRAME)>>2;
        if(bg)
            setsprite(spritenum,sprite[spritenum].x,sprite[spritenum].y,sprite[spritenum].z);
        return 0;
    }

    dasectnum = sprite[spritenum].sectnum;

	daz = sprite[spritenum].z;
    h = ((gettilesizy(sprite[spritenum].picnum)*sprite[spritenum].yrepeat)<<1);
    daz -= h;

    if( bg )
    {
        oldx = sprite[spritenum].x;
        oldy = sprite[spritenum].y;

        if( sprite[spritenum].xrepeat > 60 )
            retval = clipmove(&sprite[spritenum].x,&sprite[spritenum].y,&daz,&dasectnum,((xchange*TICSPERFRAME)<<11),((ychange*TICSPERFRAME)<<11),1024L,(4<<8),(4<<8),cliptype);
        else
        {
            if(sprite[spritenum].picnum == LIZMAN)
                cd = 292L;
            //else if( (actortype[sprite[spritenum].picnum]&3) )
            //    cd = sprite[spritenum].clipdist<<2;
            else
                cd = 192L;

            retval = clipmove(&sprite[spritenum].x,&sprite[spritenum].y,&daz,&dasectnum,((xchange*TICSPERFRAME)<<11),((ychange*TICSPERFRAME)<<11),cd,(4<<8),(4<<8),cliptype);
        }

        if( dasectnum < 0 || ( dasectnum >= 0 &&
            ( ( hittype[spritenum].actorstayput >= 0 && hittype[spritenum].actorstayput != dasectnum ) ||
              ( ( sprite[spritenum].picnum == BOSS2 ) && sector[dasectnum].lotag != 3 ) ||
              ( ( sprite[spritenum].picnum == BOSS1 || sprite[spritenum].picnum == BOSS2 ) && sector[dasectnum].lotag == 1 ) ||
              ( sector[dasectnum].lotag == 1 && ( sprite[spritenum].picnum == LIZMAN || ( sprite[spritenum].picnum == LIZTROOP && sprite[spritenum].zvel == 0 ) ) )
            ) )
          )
        {
                sprite[spritenum].x = oldx;
                sprite[spritenum].y = oldy;
                if(sector[dasectnum].lotag == 1 && sprite[spritenum].picnum == LIZMAN)
                    sprite[spritenum].ang = (TRAND&2047);
                else if( (hittype[spritenum].temp_data[0]&3) == 1 && sprite[spritenum].picnum != COMMANDER )
                    sprite[spritenum].ang = (TRAND&2047);
                setsprite(spritenum,oldx,oldy,sprite[spritenum].z);
                if(dasectnum < 0) dasectnum = 0;
                return (16384+dasectnum);
        }
        if( (retval&49152) >= 32768 && (hittype[spritenum].cgg==0) ) sprite[spritenum].ang += 768;
    }
    else
    {
        if(sprite[spritenum].statnum == 4)
            retval =
                clipmove(&sprite[spritenum].x,&sprite[spritenum].y,&daz,&dasectnum,((xchange*TICSPERFRAME)<<11),((ychange*TICSPERFRAME)<<11),8L,(4<<8),(4<<8),cliptype);
        else
            retval =
                clipmove(&sprite[spritenum].x,&sprite[spritenum].y,&daz,&dasectnum,((xchange*TICSPERFRAME)<<11),((ychange*TICSPERFRAME)<<11),(long)(sprite[spritenum].clipdist<<2),(4<<8),(4<<8),cliptype);
    }

    if( dasectnum >= 0)
        if ( (dasectnum != sprite[spritenum].sectnum) )
            changespritesect(spritenum,dasectnum);
    daz = sprite[spritenum].z + ((zchange*TICSPERFRAME)>>3);
    if ((daz > hittype[spritenum].ceilingz) && (daz <= hittype[spritenum].floorz))
        sprite[spritenum].z = daz;
    else
        if (retval == 0)
            return(16384+dasectnum);

	return(retval);
}


short ssp(short i,unsigned long cliptype) //The set sprite function
{
    spritetype *s;
    long movetype;

    s = &sprite[i];

    movetype = movesprite(i,
        (s->xvel*(sintable[(s->ang+512)&2047]))>>14,
        (s->xvel*(sintable[s->ang&2047]))>>14,s->zvel,
        cliptype);

    return (movetype==0);
}

void insertspriteq(short i)
{
    if(spriteqamount > 0)
    {
        if(spriteq[spriteqloc] >= 0)
            sprite[spriteq[spriteqloc]].xrepeat = 0;
        spriteq[spriteqloc] = i;
        spriteqloc = (spriteqloc+1)%spriteqamount;
    }
    else sprite[i].xrepeat = sprite[i].yrepeat = 0;
}

void lotsofmoney(spritetype *s, short n)
{
    short i ,j;
    for(i=n;i>0;i--)
    {
        j = EGS(s->sectnum,s->x,s->y,s->z-(TRAND%(47<<8)),MONEY,-32,8,8,TRAND&2047,0,0,0,5);
        sprite[j].cstat = TRAND&12;
    }
}

void lotsofmail(spritetype *s, short n)
{
    short i ,j;
    for(i=n;i>0;i--)
    {
        j = EGS(s->sectnum,s->x,s->y,s->z-(TRAND%(47<<8)),MAIL,-32,8,8,TRAND&2047,0,0,0,5);
        sprite[j].cstat = TRAND&12;
    }
}

void lotsofpaper(spritetype *s, short n)
{
    short i ,j;
    for(i=n;i>0;i--)
    {
        j = EGS(s->sectnum,s->x,s->y,s->z-(TRAND%(47<<8)),PAPER,-32,8,8,TRAND&2047,0,0,0,5);
        sprite[j].cstat = TRAND&12;
    }
}


     
void guts(spritetype *s,short gtype, short n, short p)
{
    long gutz,floorz;
    short i,a,j;
    char sx,sy;
    signed char pal;

    n = max(1, n / numplayers);

    if(badguy(s) && s->xrepeat < 16)
        sx = sy = 8;
    else sx = sy = 32;

    gutz = s->z-(8<<8);
    floorz = getflorzofslope(s->sectnum,s->x,s->y);

    if( gutz > ( floorz-(8<<8) ) )
        gutz = floorz-(8<<8);

    if(s->picnum == COMMANDER)
        gutz -= (24<<8);

    if( badguy(s) && s->pal == 6)
        pal = 6;
    else pal = 0;

    for(j=0;j<n;j++)
    {
        a = krand()&2047;
        i = EGS(s->sectnum,s->x+(krand()&255)-128,s->y+(krand()&255)-128,gutz-(krand()&8191),gtype,-32,sx,sy,a,48+(krand()&31),-512-(krand()&2047),ps[p].i,5);
        if(PN == JIBS2)
        {
            sprite[i].xrepeat >>= 2;
            sprite[i].yrepeat >>= 2;
        }
        if(pal == 6)
            sprite[i].pal = 6;
    }
}

void gutsdir(spritetype *s,short gtype, short n, short p)
{
    long gutz,floorz;
    short i,a,j;
    char sx,sy;

    if(badguy(s) && s->xrepeat < 16)
        sx = sy = 8;
    else sx = sy = 32;

    gutz = s->z-(8<<8);
    floorz = getflorzofslope(s->sectnum,s->x,s->y);

    if( gutz > ( floorz-(8<<8) ) )
        gutz = floorz-(8<<8);

    if(s->picnum == COMMANDER)
        gutz -= (24<<8);

    for(j=0;j<n;j++)
    {
        a = krand()&2047;
        i = EGS(s->sectnum,s->x,s->y,gutz,gtype,-32,sx,sy,a,256+(krand()&127),-512-(krand()&2047),ps[p].i,5);
    }
}

void setsectinterpolate(short i)
{
    long j, k, startwall,endwall;

    startwall = sector[SECT].wallptr;
    endwall = startwall+sector[SECT].wallnum;

    for(j=startwall;j<endwall;j++)
    {
        setinterpolation(&wall[j].x);
        setinterpolation(&wall[j].y);
        k = wall[j].nextwall;
        if(k >= 0)
        {
            setinterpolation(&wall[k].x);
            setinterpolation(&wall[k].y);
            k = wall[k].point2;
            setinterpolation(&wall[k].x);
            setinterpolation(&wall[k].y);
        }
    }
}

void ms(short i)
{
    //T1,T2 and T3 are used for all the sector moving stuff!!!

    short startwall,endwall,x;
    long tx,ty,j,k;
    spritetype *s;

    s = &sprite[i];

    s->x += (s->xvel*(sintable[(s->ang+512)&2047]))>>14;
    s->y += (s->xvel*(sintable[s->ang&2047]))>>14;

    j = T2;
    k = T3;

    ms_update(s->sectnum, k, s->x, s->y);

    startwall = sector[s->sectnum].wallptr;
    endwall = startwall+sector[s->sectnum].wallnum;
    for(x=startwall;x<endwall;x++)
    {
        rotatepoint(
            0,0,
            msx[j],msy[j],
            k&2047,&tx,&ty);

        dragpoint(x,s->x+tx,s->y+ty);

        j++;
    }
}

void movefta(void)
{
    long x, px, py, sx, sy;
    short i, j, p, psect, ssect, nexti;
    spritetype *s;

    i = headspritestat[2];
    while(i >= 0)
    {
        nexti = nextspritestat[i];

        s = &sprite[i];
        p = findplayer(s,&x);

        ssect = psect = s->sectnum;

        if(sprite[ps[p].i].extra > 0 )
        {
            if( x < 30000 )
            {
                hittype[i].timetosleep++;
                if( hittype[i].timetosleep >= (x>>8) )
                {
                    if(badguy(s))
                    {
                        px = ps[p].oposx+64-(TRAND&127);
                        py = ps[p].oposy+64-(TRAND&127);
                        updatesector(px,py,&psect);
                        if(psect == -1)
                        {
                            i = nexti;
                            continue;
                        }
                        sx = s->x+64-(TRAND&127);
                        sy = s->y+64-(TRAND&127);
                        updatesector(px,py,&ssect);
                        if(ssect == -1)
                        {
                            i = nexti;
                            continue;
                        }
                        j = cansee(sx,sy,s->z-(TRAND%(52<<8)),s->sectnum,px,py,ps[p].oposz-(TRAND%(32<<8)),ps[p].cursectnum);
                    }
                    else
                        j = cansee(s->x,s->y,s->z-((TRAND&31)<<8),s->sectnum,ps[p].oposx,ps[p].oposy,ps[p].oposz-((TRAND&31)<<8),ps[p].cursectnum);

       //             j = 1;

                    if(j) switch(s->picnum)
                    {
                        case RUBBERCAN:
                        case EXPLODINGBARREL:
                        case WOODENHORSE:
                        case HORSEONSIDE:
                        case CANWITHSOMETHING:
                        case CANWITHSOMETHING2:
                        case CANWITHSOMETHING3:
                        case CANWITHSOMETHING4:
                        case FIREBARREL:
                        case FIREVASE:
                        case NUKEBARREL:
                        case NUKEBARRELDENTED:
                        case NUKEBARRELLEAKED:
                        case TRIPBOMB:
                            if (sector[s->sectnum].ceilingstat&1)
                                s->shade = sector[s->sectnum].ceilingshade;
                            else s->shade = sector[s->sectnum].floorshade;

                            hittype[i].timetosleep = 0;
                            changespritestat(i,6);
                            break;
                        default:
                            hittype[i].timetosleep = 0;
                            check_fta_sounds(i);
                            changespritestat(i,1);
                            break;
                    }
                    else hittype[i].timetosleep = 0;
                }
            }
            if( badguy( s ) )
            {
                if (sector[s->sectnum].ceilingstat&1)
                    s->shade = sector[s->sectnum].ceilingshade;
                else s->shade = sector[s->sectnum].floorshade;
            }
        }
        i = nexti;
    }
}

short ifhitsectors(short sectnum)
{
    short i;

    i = headspritestat[5];
    while(i >= 0)
    {
        if( PN == EXPLOSION2 && sectnum == SECT )
            return i;
        i = nextspritestat[i];
    }
    return -1;
}

short ifhitbyweapon(short sn)
{
    short j, k, p;
    spritetype *npc;

    if( hittype[sn].extra >= 0 )
    {
        if(sprite[sn].extra >= 0 )
        {
            npc = &sprite[sn];

            if(npc->picnum == APLAYER)
            {
                if(ud.god) return -1;

                p = npc->yvel;
                j = hittype[sn].owner;

                if( j >= 0 &&
                    sprite[j].picnum == APLAYER &&
                    ud.coop == 1 &&
                    ud.ffire == 0 )
                        return -1;

                npc->extra -= hittype[sn].extra;

                if(j >= 0)
                {
                    if(npc->extra <= 0)
                    {
                        npc->extra = 0;

                        ps[p].wackedbyactor = j;

                        if( sprite[hittype[sn].owner].picnum == APLAYER && p != sprite[hittype[sn].owner].yvel )
                            ps[p].frag_ps = sprite[j].yvel;

                        hittype[sn].owner = ps[p].i;
                    }
                }

                switch(hittype[sn].picnum)
                {
                    case RADIUSEXPLOSION:
                    case RPG:
                    case HYDRENT:
                    case HEAVYHBOMB:
                    case SEENINE:
                    case OOZFILTER:
                    case EXPLODINGBARREL:
                        ps[p].posxv +=
                            hittype[sn].extra*(sintable[(hittype[sn].ang+512)&2047])<<2;
                        ps[p].posyv +=
                            hittype[sn].extra*(sintable[hittype[sn].ang&2047])<<2;
                        break;
                    default:
                        ps[p].posxv +=
                            hittype[sn].extra*(sintable[(hittype[sn].ang+512)&2047])<<1;
                        ps[p].posyv +=
                            hittype[sn].extra*(sintable[hittype[sn].ang&2047])<<1;
                        break;
                }
            }
            else
            {
                if(hittype[sn].extra == 0 )
                    if( hittype[sn].picnum == SHRINKSPARK && npc->xrepeat < 24 )
                        return -1;

                npc->extra -= hittype[sn].extra;
                if(npc->picnum != RECON && npc->owner >= 0 && sprite[npc->owner].statnum < MAXSTATUS )
                    npc->owner = hittype[sn].owner;
            }

            hittype[sn].extra = -1;
            return hittype[sn].picnum;
        }
    }

    hittype[sn].extra = -1;
    return -1;
}

void movecyclers(void)
{
    short q, j, x, t, s, *c;
    walltype *wal;
    char cshade;

    for(q=numcyclers-1;q>=0;q--)
    {

        c = &cyclers[q][0];
        s = c[0];
        
        t = c[3];
        j = t+(sintable[c[1]&2047]>>10);
        cshade = c[2];

        if( j < cshade ) j = cshade;
        else if( j > t )  j = t;

        c[1] += sector[s].extra;
        if(c[5])
        {
            wal = &wall[sector[s].wallptr];
            for(x = sector[s].wallnum;x>0;x--,wal++)
                if( wal->hitag != 1 )
            {
                wal->shade = j;

                if( (wal->cstat&2) && wal->nextwall >= 0)
                    wall[wal->nextwall].shade = j;

            }
            sector[s].floorshade = sector[s].ceilingshade = j;
        }
    }
}


short otherp;
void moveplayers(void) //Players
{
    short i , nexti;
    long otherx;
    spritetype *s;
    struct player_struct *p;

    i = headspritestat[10];
    while(i >= 0)
    {
        nexti = nextspritestat[i];

        s = &sprite[i];
        p = &ps[s->yvel];
        if(s->owner >= 0)
        {
            if(p->newowner >= 0 ) //Looking thru the camera
            {
                s->x = p->oposx;
                s->y = p->oposy;
                hittype[i].bposz = s->z = p->oposz+PHEIGHT;
                s->ang = p->oang;
                setsprite(i,s->x,s->y,s->z);
            }
            else
            {
                if(ud.multimode > 1)
                    otherp = findotherplayer(s->yvel,&otherx);
                else
                {
                    otherp = s->yvel;
                    otherx = 0;
                }

                execute(i,s->yvel,otherx);

                if(ud.multimode > 1)
                    if( sprite[ps[otherp].i].extra > 0 )
                {
                    if( s->yrepeat > 32 && sprite[ps[otherp].i].yrepeat < 32)
                    {
                        if( otherx < 1400 && p->knee_incs == 0 )
                        {
                            p->knee_incs = 1;
                            p->weapon_pos = -1;
                            p->actorsqu = ps[otherp].i;
                        }
                    }
                }
                if(ud.god)
                {
                    s->extra = max_player_health;
                    s->cstat = 257;
                    p->jetpack_amount =     1599;
                }


                if( s->extra > 0 )
                {
                    hittype[i].owner = i;

                    if(ud.god == 0)
                        if( ceilingspace(s->sectnum) || floorspace(s->sectnum) )
                            quickkill(p);
                }
                else
                {
                    
                    p->posx = s->x;
                    p->posy = s->y;
                    p->posz = s->z-(20<<8);

                    p->newowner = -1;

                }
                s->ang = p->ang;
            }
        }
        else
        {
            if(p->holoduke_on == -1)
                KILLIT(i);

            hittype[i].bposx = s->x;
            hittype[i].bposy = s->y;
            hittype[i].bposz = s->z;

            s->cstat = 0;

            if(s->xrepeat < 42)
            {
                s->xrepeat += 4;
                s->cstat |= 2;
            }
            else s->xrepeat = 42;
            if(s->yrepeat < 36)
                s->yrepeat += 4;
            else
            {
                s->yrepeat = 36;
                if(sector[s->sectnum].lotag != 2)
                    makeitfall(i);
                if(s->zvel == 0 && sector[s->sectnum].lotag == 1)
                    s->z += (32<<8);
            }

            if(s->extra < 8)
            {
                s->xvel = 128;
                s->ang = p->ang;
                s->extra++;
                IFMOVING;
            }
            else
            {
                s->ang = 2047-p->ang;
                setsprite(i,s->x,s->y,s->z);
            }
        }

        if (sector[s->sectnum].ceilingstat&1)
            s->shade += (sector[s->sectnum].ceilingshade-s->shade)>>1;
        else
            s->shade += (sector[s->sectnum].floorshade-s->shade)>>1;

        BOLT:
        i = nexti;
    }
}


void movefx(void)
{
    short i, j, nexti, p;
    long x, d, ht, p2;
    spritetype *s;

    i = headspritestat[11];
    while(i >= 0)
    {
        s = &sprite[i];

        nexti = nextspritestat[i];

        switch(s->picnum)
        {
            case RESPAWN:
                if(sprite[i].extra == 66)
                {
                    j = spawn(i,SHT);
//                    sprite[j].pal = sprite[i].pal;
                    KILLIT(i);
                }
                else if(sprite[i].extra > (66-13))
                    sprite[i].extra++;
                break;

            case MUSICANDSFX:

                ht = s->hitag;

                if(T2 != SoundToggle)
                {
                    T2 = SoundToggle;
                    T1 = 0;
                }

                if(s->lotag >= 1000 && s->lotag < 2000)
                {
                }
                else if(s->lotag < 999 && (unsigned)sector[s->sectnum].lotag < 9 && AmbienceToggle && sector[SECT].floorz != sector[SECT].ceilingz)
                {
                    if( (soundm[s->lotag]&2) )
                    {
                        x = dist(&sprite[ps[screenpeek].i],s);
                        for (p2 = 1; p2 < numplayers; p2++)
                        {
                            d = dist(&sprite[ps[p2].i],s);
                            if (d < x)
                                x = d;
                        }
                        if( x < ht && T1 == 0 )
                        {
                            if(numenvsnds == 16)
                            {
                                j = headspritestat[11];
                                while(j >= 0)
                                {
                                    if( PN == MUSICANDSFX && j != i && sprite[j].lotag < 999 && hittype[j].temp_data[0] == 1 && dist(&sprite[j],&sprite[ps[screenpeek].i]) > x )
                                    {
                                        stopenvsound(sprite[j].lotag,j);
                                        break;
                                    }
                                    j = nextspritestat[j];
                                }
                                if(j == -1) goto BOLT;
                            }
                            spritesound(s->lotag,i);
                            T1 = 1;
                        }
                        if( x >= ht && T1 == 1 )
                        {
                            T1 = 0;
                            stopenvsound(s->lotag,i);
                        }
                    }
                    if( (soundm[s->lotag]&16) )
                    {
                        if(T5 > 0) T5--;
                        else for(p=0;p<numplayers;p++)
                            if( ps[p].cursectnum == s->sectnum )
                        {
                            j = s->lotag+((unsigned)global_random%(s->hitag+1));
                            sound(j);
                            T5 =  26*40 + (global_random%(26*40));
                        }
                    }
                }
                break;
        }
        BOLT:
        i = nexti;
    }
}



void movefallers(void)
{
    short i, nexti, sect, j;
    spritetype *s;
    long x;

    i = headspritestat[12];
    while(i >= 0)
    {
        nexti = nextspritestat[i];
        s = &sprite[i];

        sect = s->sectnum;

        if( T1 == 0 )
        {
            s->z -= (16<<8);
            T2 = s->ang;
            x = s->extra;
            IFHIT
            {
                if( j == FIREEXT || j == RPG || j == RADIUSEXPLOSION || j == SEENINE || j == OOZFILTER )
                {
                    if(s->extra <= 0)
                    {
                        T1 = 1;
                        j = headspritestat[12];
                        while(j >= 0)
                        {
                            if(sprite[j].hitag == SHT)
                            {
                                hittype[j].temp_data[0] = 1;
                                sprite[j].cstat &= (65535-64);
                                if(sprite[j].picnum == CEILINGSTEAM || sprite[j].picnum == STEAM)
                                    sprite[j].cstat |= 32768;
                            }
                            j = nextspritestat[j];
                        }
                    }
                }
                else
                {
                    hittype[i].extra = 0;
                    s->extra = x;
                }
            }
            s->ang = T2;
            s->z += (16<<8);
        }
        else if(T1 == 1)
        {
            if(s->lotag > 0)
            {
                s->lotag-=3;
                if(s->lotag <= 0)
                {
                    s->xvel = (32+TRAND&63);
                    s->zvel = -(1024+(TRAND&1023));
                }
            }
            else
            {
                if( s->xvel > 0)
                {
                    s->xvel -= 8;
                    ssp(i,CLIPMASK0);
                }

                if( floorspace(s->sectnum) ) x = 0;
                else
                {
                    if(ceilingspace(s->sectnum))
                        x = gc/6;
                    else
                        x = gc;
                }

                if( s->z < (sector[sect].floorz-FOURSLEIGHT) )
                {
                    s->zvel += x;
                    if(s->zvel > 6144)
                        s->zvel = 6144;
                    s->z += s->zvel;
                }
                if( (sector[sect].floorz-s->z) < (16<<8) )
                {
                    j = 1+(TRAND&7);
                    for(x=0;x<j;x++) RANDOMSCRAP;
                    KILLIT(i);
                }
            }
        }

        BOLT:
        i = nexti;
    }
}

void movestandables(void)
{
    short i, j, k, m, nexti, nextj, nextk, p, q, sect;
    long l, x, *t, x1, y1;
    spritetype *s;

    i = headspritestat[6];
    while(i >= 0)
    {
        nexti = nextspritestat[i];

        t = &hittype[i].temp_data[0];
        s = &sprite[i];
        sect = s->sectnum;

        if( sect < 0 ) KILLIT(i);

        hittype[i].bposx = s->x;
        hittype[i].bposy = s->y;
        hittype[i].bposz = s->z;

        IFWITHIN(CRANE,CRANE+3)
        {
            //t[0] = state
            //t[1] = checking sector number

            if(s->xvel) getglobalz(i);

            if( t[0] == 0 ) //Waiting to check the sector
            {
                j = headspritesect[t[1]];
                while(j>=0)
                {
                    nextj = nextspritesect[j];
                    switch( sprite[j].statnum )
                    {
                        case 1:
                        case 2:
                        case 6:
                        case 10:
                            s->ang = getangle(msx[t[4]+1]-s->x,msy[t[4]+1]-s->y);
                            setsprite(j,msx[t[4]+1],msy[t[4]+1],sprite[j].z);
                            t[0]++;
                            goto BOLT;
                    }
                    j = nextj;
                }
            }

            else if(t[0]==1)
            {
                if( s->xvel < 184 )
                {
                    s->picnum = CRANE+1;
                    s->xvel += 8;
                }
                IFMOVING;
                if(sect == t[1])
                    t[0]++;
            }
            else if(t[0]==2 || t[0]==7)
            {
                s->z += (1024+512);

                if(t[0]==2)
                {
                    if( (sector[sect].floorz - s->z) < (64<<8) )
                        if(s->picnum > CRANE) s->picnum--;

                    if( (sector[sect].floorz - s->z) < (4096+1024))
                        t[0]++;
                }
                if(t[0]==7)
                {
                    if( (sector[sect].floorz - s->z) < (64<<8) )
                    {
                        if(s->picnum > CRANE) s->picnum--;
                        else
                        {
                            if(s->owner==-2)
                            {
                                spritesound(DUKE_GRUNT,ps[p].i);
                                p = findplayer(s,&x);
                                if(ps[p].on_crane == i)
                                    ps[p].on_crane = -1;
                            }
                            t[0]++;
                            s->owner = -1;
                        }
                    }
                }
            }
            else if(t[0]==3)
            {
                s->picnum++;
                if( s->picnum == (CRANE+2) )
                {
                    p = checkcursectnums(t[1]);
                    if(p >= 0 && ps[p].on_ground)
                    {
                        s->owner = -2;
                        ps[p].on_crane = i;
                        spritesound(DUKE_GRUNT,ps[p].i);
                        ps[p].ang = s->ang+1024;
                    }
                    else
                    {
                        j = headspritesect[t[1]];
                        while(j>=0)
                        {
                            switch( sprite[j].statnum )
                            {
                                case 1:
                                case 6:
                                    s->owner = j;
                                    break;
                            }
                            j = nextspritesect[j];
                        }
                    }

                    t[0]++;//Grabbed the sprite
                    t[2]=0;
                    goto BOLT;
                }
            }
            else if(t[0]==4) //Delay before going up
            {
                t[2]++;
                if(t[2] > 10)
                    t[0]++;
            }
            else if(t[0]==5 || t[0] == 8)
            {
                if(t[0]==8 && s->picnum < (CRANE+2))
                    if( (sector[sect].floorz-s->z) > 8192)
                        s->picnum++;

                if(s->z < msx[t[4]+2])
                {
                    t[0]++;
                    s->xvel = 0;
                }
                else
                    s->z -= (1024+512);
            }
            else if(t[0]==6)
            {
                if( s->xvel < 192 )
                    s->xvel += 8;
                s->ang = getangle(msx[t[4]]-s->x,msy[t[4]]-s->y);
                IFMOVING;
                if( ((s->x-msx[t[4]])*(s->x-msx[t[4]])+(s->y-msy[t[4]])*(s->y-msy[t[4]]) ) < (128*128) )
                    t[0]++;
            }

            else if(t[0]==9)
                t[0] = 0;

            setsprite(msy[t[4]+2],s->x,s->y,s->z-(34<<8));

            if(s->owner != -1)
            {
                p = findplayer(s,&x);

                IFHIT
                {
                    if(s->owner == -2)
                        if(ps[p].on_crane == i)
                            ps[p].on_crane = -1;
                    s->owner = -1;
                    s->picnum = CRANE;
                    goto BOLT;
                }

                if(s->owner >= 0)
                {
                    setsprite(s->owner,s->x,s->y,s->z);

                    hittype[s->owner].bposx = s->x;
                    hittype[s->owner].bposy = s->y;
                    hittype[s->owner].bposz = s->z;

                    s->zvel = 0;
                }
                else if(s->owner == -2)
                {
                    ps[p].oposx = ps[p].posx = s->x-(sintable[(ps[p].ang+512)&2047]>>6);
                    ps[p].oposy = ps[p].posy = s->y-(sintable[ps[p].ang&2047]>>6);
                    ps[p].oposz = ps[p].posz = s->z+(2<<8);
                    setsprite(ps[p].i,ps[p].posx,ps[p].posy,ps[p].posz);
                    ps[p].cursectnum = sprite[ps[p].i].sectnum;
                }
            }

            goto BOLT;
        }

        IFWITHIN(WATERFOUNTAIN,WATERFOUNTAIN+3)
        {
            if(t[0] > 0)
            {
                if( t[0] < 20 )
                {
                    t[0]++;

                    s->picnum++;

                    if( s->picnum == ( WATERFOUNTAIN+3 ) )
                        s->picnum = WATERFOUNTAIN+1;
                }
                else
                {
                    p = findplayer(s,&x);

                    if(x > 512)
                    {
                        t[0] = 0;
                        s->picnum = WATERFOUNTAIN;
                    }
                    else t[0] = 1;
                }
            }
            goto BOLT;
        }

        if( AFLAMABLE(s->picnum) )
        {
            if(T1 == 1)
            {
                T2++;
                if( (T2&3) > 0) goto BOLT;

                if( s->picnum == TIRE && T2 == 32 )
                {
                    s->cstat = 0;
                    j = spawn(i,BLOODPOOL);
                    sprite[j].shade = 127;
                }
                else
                {
                    if(s->shade < 64) s->shade++;
                    else KILLIT(i);
                }

                j = s->xrepeat-(TRAND&7);
                if(j < 10)
                {
                    KILLIT(i);
                }

                s->xrepeat = j;

                j = s->yrepeat-(TRAND&7);
                if(j < 4) { KILLIT(i); }
                s->yrepeat = j;
            }
            if(s->picnum == BOX)
            {
                makeitfall(i);
                hittype[i].ceilingz = sector[s->sectnum].ceilingz;
            }
            goto BOLT;
        }

        if(s->picnum == TRIPBOMB)
        {
            if(T3 > 0)
            {
                T3--;
                if(T3 == 8)
                {
                    spritesound(LASERTRIP_EXPLODE,i);
                    for(j=0;j<5;j++) RANDOMSCRAP;
                    x = s->extra;
                    hitradius( i, tripbombblastradius, x>>2,x>>1,x-(x>>2),x);

                    j = spawn(i,EXPLOSION2);
                    sprite[j].ang = s->ang;
                    sprite[j].xvel = 348;
                    ssp(j,CLIPMASK0);

                    j = headspritestat[5];
                    while(j >= 0)
                    {
                        if(sprite[j].picnum == LASERLINE && s->hitag == sprite[j].hitag)
                            sprite[j].xrepeat = sprite[j].yrepeat = 0;
                        j = nextspritestat[j];
                    }
                    KILLIT(i);
                }
                goto BOLT;
            }
            else
            {
                x = s->extra;
                s->extra = 1;
                l = s->ang;
                IFHIT { T3 = 16; }
                s->extra = x;
                s->ang = l;
            }

            if( T1 < 32 )
            {
                p = findplayer(s,&x);
                if( x > 768 ) T1++;
                else if(T1 > 16) T1++;
            }
            if( T1 == 32 )
            {
                l = s->ang;
                s->ang = T6;

                T4 = s->x;T5 = s->y;
                s->x += sintable[(T6+512)&2047]>>9;
                s->y += sintable[(T6)&2047]>>9;
                s->z -= (3<<8);
                setsprite(i,s->x,s->y,s->z);

                x = hitasprite(i,&m);

                hittype[i].lastvx = x;

                s->ang = l;

                k = 0;
                
                s->x += sintable[(T6+512)&2047]>>5;
                s->y += sintable[(T6)&2047]>>5;

                while(x > 0)
                {
                    j = spawn(i,LASERLINE);
                    setsprite(j,sprite[j].x,sprite[j].y,sprite[j].z);
                    sprite[j].hitag = s->hitag;
                    hittype[j].temp_data[1] = sprite[j].z;

                    if (x >= 2048)
                    {
                        s->x += sintable[(T6+512)&2047]>>4;
                        s->y += sintable[(T6)&2047]>>4;
                    }
                    else
                    {
                        s->x += (sintable[(T6+512)&2047]*x)>>15;
                        s->y += (sintable[(T6)&2047]*x)>>15;
                    }

                    if( x < 1024 )
                    {
                        sprite[j].xrepeat = (x+31)>>5;
                        break;
                    }
                    x -= 1024;
                }

                T1++;
                s->x = T4;s->y = T5;
                s->z += (3<<8);
                setsprite(i,s->x,s->y,s->z);
                T4 = 0;
                if( m >= 0 )
                {
                    T3 = 13;
                    spritesound(LASERTRIP_ARMING,i);
                }
                else T3 = 0;
            }
            if(T1 == 33)
            {
                T2++;


                T4 = s->x;T5 = s->y;
                s->x += sintable[(T6+512)&2047]>>9;
                s->y += sintable[(T6)&2047]>>9;
                s->z -= (3<<8);
                setsprite(i,s->x,s->y,s->z);

                x = hitasprite(i,&m);

                s->x = T4;s->y = T5;
                s->z += (3<<8);
                setsprite(i,s->x,s->y,s->z);

                if( hittype[i].lastvx != x )
                {
                    T3 = 13;
                    spritesound(LASERTRIP_ARMING,i);
                }
            }
            goto BOLT;
        }


        if( s->picnum >= CRACK1 && s->picnum <= CRACK4 )
        {
            if(s->hitag > 0)
            {
                t[0] = s->cstat;
                t[1] = s->ang;
                j = ifhitbyweapon(i);
                if(j == TILE3634 || j == FIREEXT || j == RPG || j == RADIUSEXPLOSION || j == SEENINE || j == OOZFILTER || j == TILE3841 )
                {
                    j = headspritestat[6];
                    while(j >= 0)
                    {
                        if(s->hitag == sprite[j].hitag && ( sprite[j].picnum == OOZFILTER || sprite[j].picnum == SEENINE ) )
                            if(sprite[j].shade != -32)
                                sprite[j].shade = -32;
                        j = nextspritestat[j];
                    }

                    goto DETONATE;
                }
                else
                {
                    s->cstat = t[0];
                    s->ang = t[1];
                    s->extra = 0;
                }
            }
            goto BOLT;
        }

        if( s->picnum == FIREEXT )
        {
            j = ifhitbyweapon(i);
            if( j == -1 ) goto BOLT;

            for(k=0;k<16;k++)
            {
                j = EGS(SECT,SX,SY,SZ-(TRAND%(48<<8)),SCRAP3+(TRAND&3),-8,48,48,TRAND&2047,(TRAND&63)+64,-(TRAND&4095)-(sprite[i].zvel>>2),i,5);
                sprite[j].pal = 2;
            }

            spawn(i,EXPLOSION2);
            spritesound(PIPEBOMB_EXPLODE,i);
            spritesound(GLASS_HEAVYBREAK,i);

            if(s->hitag > 0)
            {
                j = headspritestat[6];
                while(j >= 0)
                {
                    if(s->hitag == sprite[j].hitag && ( sprite[j].picnum == OOZFILTER || sprite[j].picnum == SEENINE ) )
                        if(sprite[j].shade != -32)
                            sprite[j].shade = -32;
                    j = nextspritestat[j];
                }

                x = s->extra;
                spawn(i,EXPLOSION2);
                hitradius( i, pipebombblastradius,x>>2, x-(x>>1),x-(x>>2), x);
                spritesound(PIPEBOMB_EXPLODE,i);

                goto DETONATE;
            }
            else
            {
                hitradius(i,seenineblastradius,10,15,20,25);
                KILLIT(i);
            }
            goto BOLT;
        }

        if(s->picnum == OOZFILTER || s->picnum == SEENINE || s->picnum == SEENINEDEAD || s->picnum == (SEENINEDEAD+1) )
        {
            if(s->shade != -32 && s->shade != -33)
            {
                if(s->xrepeat)
                    j = (ifhitbyweapon(i) >= 0);
                else
                    j = 0;

                if( j || s->shade == -31 )
                {
                    if(j) s->lotag = 0;

                    t[3] = 1;

                    j = headspritestat[6];
                    while(j >= 0)
                    {
                        if(s->hitag == sprite[j].hitag && ( sprite[j].picnum == SEENINE || sprite[j].picnum == OOZFILTER ) )
                            sprite[j].shade = -32;
                        j = nextspritestat[j];
                    }
                }
            }
            else
            {
                if(s->shade == -32)
                {
                    if(s->lotag > 0)
                    {
                        s->lotag-=3;
                        if(s->lotag <= 0) s->lotag = -99;
                    }
                    else
                        s->shade = -33;
                }
                else
                {
                    if( s->xrepeat > 0 )
                    {
                        T3++;
                        if(T3 == 3)
                        {
                            if( s->picnum == OOZFILTER )
                            {
                                T3 = 0;
                                goto DETONATE;
                            }
                            if( s->picnum != (SEENINEDEAD+1) )
                            {
                                T3 = 0;

                                if(s->picnum == SEENINEDEAD) s->picnum++;
                                else if(s->picnum == SEENINE)
                                    s->picnum = SEENINEDEAD;
                            }
                            else goto DETONATE;
                        }
                        goto BOLT;
                    }

                    DETONATE:

                    earthquaketime = 16;

                    j = headspritestat[3];
                    while(j >= 0)
                    {
                        if( s->hitag == sprite[j].hitag )
                        {
                            if(sprite[j].lotag == 13)
                            {
                                if( hittype[j].temp_data[2] == 0 )
                                    hittype[j].temp_data[2] = 1;
                            }
                            else if(sprite[j].lotag == 8)
                                hittype[j].temp_data[4] = 1;
                            else if(sprite[j].lotag == 18)
                            {
                                if(hittype[j].temp_data[0] == 0)
                                    hittype[j].temp_data[0] = 1;
                            }
                            else if(sprite[j].lotag == 21)
                                hittype[j].temp_data[0] = 1;
                        }
                        j = nextspritestat[j];
                    }

                    s->z -= (32<<8);

                    if( ( t[3] == 1 && s->xrepeat ) || s->lotag == -99 )
                    {
                        x = s->extra;
                        spawn(i,EXPLOSION2);
                        if (s->xrepeat != 0 || s->pal != 0)
                            hitradius( i,seenineblastradius,x>>2, x-(x>>1),x-(x>>2), x);
                        spritesound(PIPEBOMB_EXPLODE,i);
                    }

                    if(s->xrepeat)
                        for(x=0;x<8;x++) RANDOMSCRAP;

                    KILLIT(i);
                }
            }
            goto BOLT;
        }

        if(s->picnum == MASTERSWITCH)
        {
            if(s->yvel == 1)
                {
                    s->hitag--;
                    if(s->hitag <= 0)
                    {
                        operatesectors(sect,i);

                        j = headspritesect[sect];
                        while(j >= 0)
                        {
                            if(sprite[j].statnum == 3)
                            {
                                switch(sprite[j].lotag)
                                {
                                    case 2:
                                    case 21:
                                    case 31:
                                    case 32:
                                    case 36:
                                        hittype[j].temp_data[0] = 1;
                                        break;
                                    case 3:
                                        hittype[j].temp_data[4] = 1;
                                        break;
                                }
                            }
                            else if(sprite[j].statnum == 6)
                            {
                                switch(sprite[j].picnum)
                                {
                                    case SEENINE:
                                    case OOZFILTER:
                                        sprite[j].shade = -31;
                                        break;
                                }
                            }
                            j = nextspritesect[j];
                        }
                        KILLIT(i);
                    }
                }
                goto BOLT;
        }

        switch(s->picnum)
        {
            case VIEWSCREEN:
            case VIEWSCREEN2:

                if(s->xrepeat == 0) KILLIT(i);

                p = findplayer(s, &x);

                if( x < 2048 )
                {
                    if( SP == 1 )
                        camsprite = i;
                }
                else if( camsprite != -1 && T1 == 1)
                {
                    camsprite = -1;
                    T1 = 0;
                    //loadtile(s->picnum);
                }

                goto BOLT;

            case TRASH:

                if(s->xvel == 0) s->xvel = 1;
                IFMOVING
                {
                    makeitfall(i);
                    if(TRAND&1) s->zvel -= 256;
                    if( klabs2(s->xvel) < 48 )
                        s->xvel += (TRAND&3);
                }
                else KILLIT(i);
                break;

            case SIDEBOLT1:
            case SIDEBOLT1+1:
            case SIDEBOLT1+2:
            case SIDEBOLT1+3:
                p = findplayer(s, &x);
                if( x > 20480 ) goto BOLT;

                CLEAR_THE_BOLT2:
                if(t[2])
                {
                    t[2]--;
                    goto BOLT;
                }
                if( (s->xrepeat|s->yrepeat) == 0 )
                {
                    s->xrepeat=t[0];
                    s->yrepeat=t[1];
                }
                if( (TRAND&8) == 0 )
                {
                    t[0]=s->xrepeat;
                    t[1]=s->yrepeat;
                    t[2] = global_random&4;
                    s->xrepeat=s->yrepeat=0;
                    goto CLEAR_THE_BOLT2;
                }
                s->picnum++;

                if(l&1) s->cstat ^= 2;

                if( (TRAND&1) && sector[sect].floorpicnum == HURTRAIL )
                    spritesound(SHORT_CIRCUIT,i);

                if(s->picnum == SIDEBOLT1+4) s->picnum = SIDEBOLT1;

                goto BOLT;

            case BOLT1:
            case BOLT1+1:
            case BOLT1+2:
            case BOLT1+3:
                p = findplayer(s, &x);
                if( x > 20480 ) goto BOLT;

                if( t[3] == 0 )
                    t[3]=sector[sect].floorshade;

                CLEAR_THE_BOLT:
                if(t[2])
                {
                    t[2]--;
                    sector[sect].floorshade = 20;
                    sector[sect].ceilingshade = 20;
                    goto BOLT;
                }
                if( (s->xrepeat|s->yrepeat) == 0 )
                {
                    s->xrepeat=t[0];
                    s->yrepeat=t[1];
                }
                else if( (TRAND&8) == 0 )
                {
                    t[0]=s->xrepeat;
                    t[1]=s->yrepeat;
                    t[2] = global_random&4;
                    s->xrepeat=s->yrepeat=0;
                    goto CLEAR_THE_BOLT;
                }
                s->picnum++;

                l = global_random&7;
                s->xrepeat=l+8;

                if(l&1) s->cstat ^= 2;

                if( s->picnum == (BOLT1+1) && (TRAND&7) == 0 && sector[sect].floorpicnum == HURTRAIL )
                    spritesound(SHORT_CIRCUIT,i);

                if(s->picnum==BOLT1+4) s->picnum=BOLT1;

                if(s->picnum&1)
                {
                    sector[sect].floorshade = 0;
                    sector[sect].ceilingshade = 0;
                }
                else
                {
                    sector[sect].floorshade = 20;
                    sector[sect].ceilingshade = 20;
                }
                goto BOLT;
                
            case WATERDRIP:

                if( t[1] )
                {
                    t[1]--;
                    if(t[1] == 0)
                        s->cstat &= 32767;
                }
                else
                {
                    makeitfall(i);
                    ssp(i,CLIPMASK0);
                    if(s->xvel > 0) s->xvel -= 2;

                    if(s->zvel == 0)
                    {
                        s->cstat |= 32768;

                        if(s->pal != 2 && s->hitag == 0)
                            spritesound(SOMETHING_DRIPPING,i);

                        if(sprite[s->owner].picnum != WATERDRIP)
                        {
                            KILLIT(i);
                        }
                        else
                        {
                            hittype[i].bposz = s->z = t[0];
                            t[1] = 48+(TRAND&31);
                        }
                    }
                }


                goto BOLT;

            case DOORSHOCK:
                j = klabs2(sector[sect].ceilingz-sector[sect].floorz)>>9;
                s->yrepeat = j+4;
                s->xrepeat = 16;
                s->z = sector[sect].floorz;
                goto BOLT;

            case TOUCHPLATE:
                if( t[1] == 1 && s->hitag >= 0) //Move the sector floor
                {
                    x = sector[sect].floorz;

                    if(t[3] == 1)
                    {
                        if(x >= t[2])
                        {
                            sector[sect].floorz = x;
                            t[1] = 0;
                        }
                        else
                        {
                            sector[sect].floorz += sector[sect].extra;
                            p = checkcursectnums(sect);
                            if(p >= 0) ps[p].posz += sector[sect].extra;
                        }
                    }
                    else
                    {
                        if(x <= s->z)
                        {
                            sector[sect].floorz = s->z;
                            t[1] = 0;
                        }
                        else
                        {
                            sector[sect].floorz -= sector[sect].extra;
                            p = checkcursectnums(sect);
                            if(p >= 0)
                                ps[p].posz -= sector[sect].extra;
                        }
                    }
                    goto BOLT;
                }

                if(t[5] == 1) goto BOLT;

                p = checkcursectnums(sect);
                if( p >= 0 && ( ps[p].on_ground || s->ang == 512) )
                {
                    if( t[0] == 0 && !check_activator_motion(s->lotag) )
                    {
                        t[0] = 1;
                        t[1] = 1;
                        t[3] = !t[3];
                        operatemasterswitches(s->lotag);
                        operateactivators(s->lotag,p);
                        if(s->hitag > 0)
                        {
                            s->hitag--;
                            if(s->hitag == 0) t[5] = 1;
                        }
                    }
                }
                else t[0] = 0;

                if(t[1] == 1)
                {
                    j = headspritestat[6];
                    while(j >= 0)
                    {
                        if(j != i && sprite[j].picnum == TOUCHPLATE && sprite[j].lotag == s->lotag)
                        {
                            hittype[j].temp_data[1] = 1;
                            hittype[j].temp_data[3] = t[3];
                        }
                        j = nextspritestat[j];
                    }
                }
                goto BOLT;

            case CANWITHSOMETHING:
            case CANWITHSOMETHING2:
            case CANWITHSOMETHING3:
            case CANWITHSOMETHING4:
                makeitfall(i);
                IFHIT
                {
                    spritesound(VENT_BUST,i);
                    for(j=0;j<10;j++)
                        RANDOMSCRAP;

                    if(s->lotag) spawn(i,s->lotag);

                    KILLIT(i);
                }
                goto BOLT;

            case EXPLODINGBARREL:
            case WOODENHORSE:
            case HORSEONSIDE:
            case FLOORFLAME:
            case FIREBARREL:
            case FIREVASE:
            case NUKEBARREL:
            case NUKEBARRELDENTED:
            case NUKEBARRELLEAKED:
            case TOILETWATER:
            case RUBBERCAN:
            case STEAM:
            case CEILINGSTEAM:
                p = findplayer(s, &x);
                execute(i,p,x);
                goto BOLT;
            case WATERBUBBLEMAKER:
                p = findplayer(s, &x);
                execute(i,p,x);
                goto BOLT;
        }

        BOLT:
        i = nexti;
    }
}



void movetransports(void)
{
    char warpspriteto;
    short i, j, k, l, p, sect, sectlotag, nexti, nextj, nextk;
    long ll,onfloorz,q;

    i = headspritestat[9]; //Transporters

    while(i >= 0)
    {
        sect = SECT;
        sectlotag = sector[sect].lotag;

        nexti = nextspritestat[i];

        if(OW == i)
        {
            i = nexti;
            continue;
        }

        onfloorz = T5;

        if(T1 > 0) T1--;

        j = headspritesect[sect];
        while(j >= 0)
        {
            nextj = nextspritesect[j];

            switch(sprite[j].statnum)
            {
                case 10:    // Player

                    if( sprite[j].owner != -1 )
                    {
                        p = sprite[j].yvel;

                        ps[p].on_warping_sector = 1;

                        if( ps[p].transporter_hold == 0 && ps[p].jumping_counter == 0 )
                        {
                            if(ps[p].on_ground && sectlotag == 0 && onfloorz && ps[p].jetpack_on == 0 )
                            {
                                if(sprite[i].pal == 0)
                                {
                                    spawn(i,TRANSPORTERBEAM);
                                    spritesound(TELEPORTER,i);
                                }

                                for(k=0;k<ud.multimode;k++)
                                    if(ps[k].cursectnum == sprite[OW].sectnum)
                                {
                                    ps[k].frag_ps = p;
                                    sprite[ps[k].i].extra = 0;
                                }

                                ps[p].ang = sprite[OW].ang;

                                if(sprite[OW].owner != OW)
                                {
                                    T1 = 13;
                                    hittype[OW].temp_data[0] = 13;
                                    ps[p].transporter_hold = 13;
                                }

                                ps[p].bobposx = ps[p].oposx = ps[p].posx = sprite[OW].x;
                                ps[p].bobposy = ps[p].oposy = ps[p].posy = sprite[OW].y;
                                ps[p].oposz = ps[p].posz = sprite[OW].z-PHEIGHT;

                                changespritesect(j,sprite[OW].sectnum);
                                ps[p].cursectnum = sprite[j].sectnum;

                                if(sprite[i].pal == 0)
                                {
                                    k = spawn(OW,TRANSPORTERBEAM);
                                    spritesound(TELEPORTER,k);
                                }

                                break;
                            }
                        }
                        else if( !(sectlotag == 1 && ps[p].on_ground == 1)  ) break;

                        if(onfloorz == 0 && klabs2(SZ-ps[p].posz) < 6144 )
                            if( (ps[p].jetpack_on == 0 ) || (ps[p].jetpack_on && (sync[p].bits&1) ) ||
                                (ps[p].jetpack_on && (sync[p].bits&2) ) )
                        {
                            ps[p].oposx = ps[p].posx += sprite[OW].x-SX;
                            ps[p].oposy = ps[p].posy += sprite[OW].y-SY;

                            if( ps[p].jetpack_on && ( (sync[p].bits&1) || ps[p].jetpack_on < 11 ) )
                                ps[p].posz = sprite[OW].z-6144;
                            else ps[p].posz = sprite[OW].z+6144;
                            ps[p].oposz = ps[p].posz;

                            hittype[ps[p].i].bposx = ps[p].posx;
                            hittype[ps[p].i].bposy = ps[p].posy;
                            hittype[ps[p].i].bposz = ps[p].posz;

                            changespritesect(j,sprite[OW].sectnum);
                            ps[p].cursectnum = sprite[OW].sectnum;

                            break;
                        }

                        k = 0;

                        if( onfloorz && sectlotag == 1 && ps[p].on_ground && ps[p].posz > (sector[sect].floorz-(16<<8)) && ( (sync[p].bits&2) || ps[p].poszv > 2048 ) )
//                        if( onfloorz && sectlotag == 1 && ps[p].posz > (sector[sect].floorz-(6<<8)) )
                        {
                            k = 1;
                            /*if(screenpeek == p)
                            {
                                FX_StopAllSounds();
                                clearsoundlocks();
                            }*/
                            if(sprite[ps[p].i].extra > 0)
                                spritesound(DUKE_UNDERWATER,j);
                            ps[p].oposz = ps[p].posz =
                                sector[sprite[OW].sectnum].ceilingz+(7<<8);

                            ps[p].posxv = 4096-(TRAND&8192);
                            ps[p].posyv = 4096-(TRAND&8192);

                        }

                        if( onfloorz && sectlotag == 2 && ps[p].posz < (sector[sect].ceilingz+(6<<8)) )
                        {
                            k = 1;
//                            if( sprite[j].extra <= 0) break;
                            /*if(screenpeek == p)
                            {
                                FX_StopAllSounds();
                                clearsoundlocks();
                            }*/
                            spritesound(DUKE_GASP,j);

                            ps[p].oposz = ps[p].posz =
                                sector[sprite[OW].sectnum].floorz-(7<<8);

                            ps[p].jumping_toggle = 1;
                            ps[p].jumping_counter = 0;
                        }

                        if(k == 1)
                        {
                            ps[p].oposx = ps[p].posx += sprite[OW].x-SX;
                            ps[p].oposy = ps[p].posy += sprite[OW].y-SY;

                            if(sprite[OW].owner != OW)
                                ps[p].transporter_hold = -2;
                            ps[p].cursectnum = sprite[OW].sectnum;

                            changespritesect(j,sprite[OW].sectnum);
                            setsprite(ps[p].i,ps[p].posx,ps[p].posy,ps[p].posz+PHEIGHT);

                            //setpal(&ps[p]);

                            if( (TRAND&255) < 32 )
                                spawn(j,WATERSPLASH2);

                            if(sectlotag == 1)
                                for(l = 0;l < 9;l++)
                            {
                                q = spawn(ps[p].i,WATERBUBBLE);
                                sprite[q].z += TRAND&16383;
                            }
                        }
                    }
                    break;

                case 1:
                    switch(sprite[j].picnum)
                    {
                        case SHARK:
                        case COMMANDER:
                        case OCTABRAIN:
                        case GREENSLIME:
                        case GREENSLIME+1:
                        case GREENSLIME+2:
                        case GREENSLIME+3:
                        case GREENSLIME+4:
                        case GREENSLIME+5:
                        case GREENSLIME+6:
                        case GREENSLIME+7:
                            if(sprite[j].extra > 0)
                                goto JBOLT;
                    }
                case 4:
                case 5:
                case 12:
                case 13:

                    ll = klabs2(sprite[j].zvel);

                    {
                        warpspriteto = 0;
                        if( ll && sectlotag == 2 && sprite[j].z < (sector[sect].ceilingz+ll) )
                            warpspriteto = 1;

                        if( ll && sectlotag == 1 && sprite[j].z > (sector[sect].floorz-ll) )
                            warpspriteto = 1;

                        if( sectlotag == 0 && ( onfloorz || klabs2(sprite[j].z-SZ) < 4096) )
                        {
                            if( sprite[OW].owner != OW && onfloorz && T1 > 0 && sprite[j].statnum != 5 )
                            {
                                T1++;
                                goto BOLT;
                            }
                            warpspriteto = 1;
                        }

                        if( warpspriteto ) switch(sprite[j].picnum)
                        {
                            case TRANSPORTERSTAR:
                            case TRANSPORTERBEAM:
                            case TRIPBOMB:
                            case BULLETHOLE:
                            case WATERSPLASH2:
                            case BURNING:
                            case BURNING2:
                            case FIRE:
                            case FIRE2:
                            case TOILETWATER:
                            case LASERLINE:
                                goto JBOLT;
                            case PLAYERONWATER:
                                if(sectlotag == 2)
                                {
                                    sprite[j].cstat &= 32767;
                                    break;
                                }
                            default:
                                if(sprite[j].statnum == 5 && !(sectlotag == 1 || sectlotag == 2) )
                                    break;

                            case WATERBUBBLE:
//                                if( rnd(192) && sprite[j].picnum == WATERBUBBLE)
  //                                 break;

                                if(sectlotag > 0)
                                {
                                    k = spawn(j,WATERSPLASH2);
                                    if( sectlotag == 1 && sprite[j].statnum == 4 )
                                    {
                                        sprite[k].xvel = sprite[j].xvel>>1;
                                        sprite[k].ang = sprite[j].ang;
                                        ssp(k,CLIPMASK0);
                                    }
                                }

                                switch(sectlotag)
                                {
                                    case 0:
                                        if(onfloorz)
                                        {
                                            if( sprite[j].statnum == 4 || ( checkcursectnums(sect) == -1 && checkcursectnums(sprite[OW].sectnum)  == -1 ) )
                                            {
                                                sprite[j].x += (sprite[OW].x-SX);
                                                sprite[j].y += (sprite[OW].y-SY);
                                                sprite[j].z -= SZ - sector[sprite[OW].sectnum].floorz;
                                                sprite[j].ang = sprite[OW].ang;

                                                hittype[j].bposx = sprite[j].x;
                                                hittype[j].bposy = sprite[j].y;
                                                hittype[j].bposz = sprite[j].z;

                                                if(sprite[i].pal == 0)
                                                {
                                                    k = spawn(i,TRANSPORTERBEAM);
                                                    spritesound(TELEPORTER,k);

                                                    k = spawn(OW,TRANSPORTERBEAM);
                                                    spritesound(TELEPORTER,k);
                                                }

                                                if( sprite[OW].owner != OW )
                                                {
                                                    T1 = 13;
                                                    hittype[OW].temp_data[0] = 13;
                                                }

                                                changespritesect(j,sprite[OW].sectnum);
                                            }
                                        }
                                        else
                                        {
                                            sprite[j].x += (sprite[OW].x-SX);
                                            sprite[j].y += (sprite[OW].y-SY);
                                            sprite[j].z = sprite[OW].z+4096;

                                            hittype[j].bposx = sprite[j].x;
                                            hittype[j].bposy = sprite[j].y;
                                            hittype[j].bposz = sprite[j].z;

                                            changespritesect(j,sprite[OW].sectnum);
                                        }
                                        break;
                                    case 1:
                                        sprite[j].x += (sprite[OW].x-SX);
                                        sprite[j].y += (sprite[OW].y-SY);
                                        sprite[j].z = sector[sprite[OW].sectnum].ceilingz+ll;

                                        hittype[j].bposx = sprite[j].x;
                                        hittype[j].bposy = sprite[j].y;
                                        hittype[j].bposz = sprite[j].z;

                                        changespritesect(j,sprite[OW].sectnum);

                                        break;
                                    case 2:
                                        sprite[j].x += (sprite[OW].x-SX);
                                        sprite[j].y += (sprite[OW].y-SY);
                                        sprite[j].z = sector[sprite[OW].sectnum].floorz-ll;

                                        hittype[j].bposx = sprite[j].x;
                                        hittype[j].bposy = sprite[j].y;
                                        hittype[j].bposz = sprite[j].z;

                                        changespritesect(j,sprite[OW].sectnum);

                                        break;
                                }

                                break;
                        }
                }
                break;

            }
            JBOLT:
            j = nextj;
        }
        BOLT:
        i = nexti;
    }
}


void moveexplosions(void)  // STATNUM 5
{
    short i, j, k, nexti, sect, p;
    long l, x, *t;
    spritetype *s;

    i = headspritestat[5];
    while(i >= 0)
    {
        nexti = nextspritestat[i];

        t = &hittype[i].temp_data[0];
        s = &sprite[i];
        sect = s->sectnum;

        if( sect < 0 || s->xrepeat == 0 ) KILLIT(i);

        hittype[i].bposx = s->x;
        hittype[i].bposy = s->y;
        hittype[i].bposz = s->z;

        switch(s->picnum)
        {
            case NEON1:
            case NEON2:
            case NEON3:
            case NEON4:
            case NEON5:
            case NEON6:

                if( (global_random/(s->lotag+1)&31) > 4) s->shade = -127;
                else s->shade = 127;
                goto BOLT;

            case BLOODSPLAT1:
            case BLOODSPLAT2:
            case BLOODSPLAT3:
            case BLOODSPLAT4:

                if( t[0] == 7*26 ) goto BOLT;
                s->z += 16+(TRAND&15);
                t[0]++;
                if( (t[0]%9) == 0 ) s->yrepeat++;
                goto BOLT;

            case NUKEBUTTON:
            case NUKEBUTTON+1:
            case NUKEBUTTON+2:
            case NUKEBUTTON+3:

                if (ud.multimode > 1 && ud.coop == 0 && dukematch_mode != 1)
                    goto BOLT;

                if(t[0])
                {
                    t[0]++;
                    if(t[0] == 8) s->picnum = NUKEBUTTON+1;
                    else if(t[0] == 16)
                    {
                        s->picnum = NUKEBUTTON+2;
                        ps[sprite[s->owner].yvel].fist_incs = 1;
                    }
                    if( ps[sprite[s->owner].yvel].fist_incs == 26 )
                        s->picnum = NUKEBUTTON+3;
                }
                goto BOLT;

            case FORCESPHERE:

                l = s->xrepeat;
                if(t[1] > 0)
                {
                    t[1]--;
                    if(t[1] == 0)
                    {
                        KILLIT(i);
                    }
                }
                if(hittype[s->owner].temp_data[1] == 0)
                {
                    if(t[0] < 64)
                    {
                        t[0]++;
                        l += 3;
                    }
                }
                else
                    if(t[0] > 64)
                    {
                        t[0]--;
                        l -= 3;
                    }

                s->x = sprite[s->owner].x;
                s->y = sprite[s->owner].y;
                s->z = sprite[s->owner].z;
                s->ang += hittype[s->owner].temp_data[0];

                if(l > 64) l = 64;
                else if(l < 1) l = 1;

                s->xrepeat = l;
                s->yrepeat = l;
                s->shade = (l>>1)-48;

                for(j=t[0];j > 0;j--)
                    ssp(i,CLIPMASK0);
                goto BOLT;
            case WATERSPLASH2:

                t[0]++;
                if(t[0] == 1 )
                {
                    if(sector[sect].lotag != 1 && sector[sect].lotag != 2)
                        KILLIT(i);
/*                    else
                    {
                        l = getflorzofslope(sect,s->x,s->y)-s->z;
                        if( l > (16<<8) ) KILLIT(i);
                    }
                    else */ if(Sound[ITEM_SPLASH].num == 0)
                        spritesound(ITEM_SPLASH,i);
                }
                if(t[0] == 3)
                {
                    t[0] = 0;
                    t[1]++;
                }
                if(t[1] == 5)
                    deletesprite(i);
                goto BOLT;

            case FRAMEEFFECT1:

                if(s->owner >= 0)
                {
                    t[0]++;

                    if( t[0] > 7 )
                    {
                        KILLIT(i);
                    }
                    else if( t[0] > 4 )
                        s->cstat |= 512+2;
                    else if( t[0] > 2 )
                        s->cstat |= 2;
                    s->xoffset = sprite[s->owner].xoffset;
                    s->yoffset = sprite[s->owner].yoffset;
                }
                goto BOLT;
            case INNERJAW:
            case INNERJAW+1:

                p = findplayer(s,&x);
                if(x < 512)
                {
                    ps[p].pals_time = 32;
                    ps[p].pals[0] = 32;
                    ps[p].pals[1] = 0;
                    ps[p].pals[2] = 0;
                    sprite[ps[p].i].extra -= 4;
                }

            case FIRELASER:
            case TILE3845:
                if(s->extra != 999)
                    s->extra = 999;
                else KILLIT(i);
                break;
            case TONGUE:
                KILLIT(i);
            case MONEY+1:
            case MAIL+1:
            case PAPER+1:
                hittype[i].floorz = s->z = getflorzofslope(s->sectnum,s->x,s->y);
                break;
            case MONEY:
            case MAIL:
            case PAPER:

                s->xvel = (TRAND&7)+(sintable[T1&2047]>>9);
                T1 += (TRAND&63);
                if( (T1&2047) > 512 && (T1&2047) < 1596)
                {
                    if(sector[sect].lotag == 2)
                    {
                        if(s->zvel < 64)
                            s->zvel += (gc>>5)+(TRAND&7);
                    }
                    else
                        if(s->zvel < 144)
                            s->zvel += (gc>>5)+(TRAND&7);
                }

                ssp(i,CLIPMASK0);

                if( (TRAND&3) == 0 )
                    setsprite(i,s->x,s->y,s->z);

                if(s->sectnum == -1) KILLIT(i);
                l = getflorzofslope(s->sectnum,s->x,s->y);

                if( s->z > l )
                {
                    s->z = l;

                    insertspriteq(i);
                    PN ++;

                    j = headspritestat[5];
                    while(j >= 0)
                    {
                        if(sprite[j].picnum == BLOODPOOL)
                            if(ldist(s,&sprite[j]) < 348)
                        {
                            s->pal = 2;
                            break;
                        }
                        j = nextspritestat[j];
                    }
                }

                break;

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

                if(s->xvel > 0) s->xvel--;
                else s->xvel = 0;

                if( t[5] < 30*100 )
                    t[5]++;
                //else { KILLIT(i); }


                if(s->zvel > 1024 && s->zvel < 1280)
                {
                    setsprite(i,s->x,s->y,s->z);
                    sect = s->sectnum;
                }

                l = getflorzofslope(sect,s->x,s->y);
                x = getceilzofslope(sect,s->x,s->y);
                if(x == l || sect < 0 || sect >= MAXSECTORS) KILLIT(i);

                if( s->z < l-(2<<8) )
                {
                    if(t[1] < 2) t[1]++;
                    else if(sector[sect].lotag != 2)
                    {
                        t[1] = 0;
                        if( s->picnum == DUKELEG || s->picnum == DUKETORSO || s->picnum == DUKEGUN )
                        {
                            if(t[0] > 6) t[0] = 0;
                            else t[0]++;
                        }
                        else
                        {
                            if(t[0] > 2)
                                t[0] = 0;
                            else t[0]++;
                        }
                    }

                    if(s->zvel < 6144)
                    {
                        if(sector[sect].lotag == 2)
                        {
                            if(s->zvel < 1024)
                                s->zvel += 48;
                            else s->zvel = 1024;
                        }
                        else s->zvel += gc-50;
                    }

                    s->x += (s->xvel*sintable[(s->ang+512)&2047])>>14;
                    s->y += (s->xvel*sintable[s->ang&2047])>>14;
                    s->z += s->zvel;

                }
                else
                {
                    if(t[2] == 0)
                    {
                        if( s->sectnum == -1) { KILLIT(i); }
                        if( (sector[s->sectnum].floorstat&2) ) { KILLIT(i); }
                        t[2]++;
                    }
                    l = getflorzofslope(s->sectnum,s->x,s->y);

                    s->z = l-(2<<8);
                    s->xvel = 0;

                    if(s->picnum == JIBS6)
                    {
                        t[1]++;
                        if( (t[1]&3) == 0 && t[0] < 7)
                            t[0]++;
                        if(t[1] > 20) KILLIT(i);
                    }
                    else { s->picnum = JIBS6; t[0] = 0; t[1] = 0; }
                }
                goto BOLT;

            case BLOODPOOL:
            case PUKE:

                if(t[0] == 0)
                {
                    t[0] = 1;
                    if(sector[sect].floorstat&2) { KILLIT(i); }
                    else insertspriteq(i);
                }

                makeitfall(i);

                p = findplayer(s,&x);

                s->z = hittype[i].floorz-(FOURSLEIGHT);

                if(t[2] < 32)
                {
                    t[2]++;
                    if(hittype[i].picnum == TIRE)
                    {
                        if(s->xrepeat < 64 && s->yrepeat < 64)
                        {
                            s->xrepeat += TRAND&3;
                            s->yrepeat += TRAND&3;
                        }
                    }
                    else
                    {
                        if(s->xrepeat < 32 && s->yrepeat < 32)
                        {
                            s->xrepeat += TRAND&3;
                            s->yrepeat += TRAND&3;
                        }
                    }
                }

                if(x < 844 && s->xrepeat > 6 && s->yrepeat > 6)
                {
                    if( s->pal == 0 && (TRAND&255) < 16 && s->picnum != PUKE)
                    {
                        if(ps[p].boot_amount > 0)
                            ps[p].boot_amount--;
                        else
                        {
                            if(Sound[DUKE_LONGTERM_PAIN].num < 1)
                                spritesound(DUKE_LONGTERM_PAIN,ps[p].i);
                            sprite[ps[p].i].extra --;
                            ps[p].pals_time = 32;
                            ps[p].pals[0] = 16;
                            ps[p].pals[1] = 0;
                            ps[p].pals[2] = 0;
                        }
                    }

                    if(t[1] == 1) goto BOLT;
                    t[1] = 1;

                    if(hittype[i].picnum == TIRE)
                        ps[p].footprintcount = 10;
                    else ps[p].footprintcount = 3;

                    if (s->picnum == BLOODPOOL && s->xrepeat >= 30)
                    {
                        if ((TRAND & 255) < 16)
                            spritesound(SND260, ps[p].i);
                    }

                    ps[p].footprintpal = s->pal;
                    ps[p].footprintshade = s->shade;

                    if(t[2] == 32)
                    {
                        s->xrepeat -= 6;
                        s->yrepeat -= 6;
                    }
                }
                else t[1] = 0;
                goto BOLT;

            case BURNING:
            case BURNING2:
            case FECES:
            case WATERBUBBLE:
            case SMALLSMOKE:
            case EXPLOSION2:
            case SHRINKEREXPLOSION:
            //case EXPLOSION2BOT:
            case BLOOD:
            case LASERSITE:
            case FORCERIPPLE:
            case TRANSPORTERSTAR:
            case TRANSPORTERBEAM:
            case TILE3953:
                p = findplayer(s,&x);
                execute(i,p,x);
                goto BOLT;

            case SHELL:
            case SHOTGUNSHELL:

                ssp(i,CLIPMASK0);

                if(sect < 0 || ( sector[sect].floorz+(24<<8) ) < s->z ) KILLIT(i);

                if(sector[sect].lotag == 2)
                {
                    t[1]++;
                    if(t[1] > 8)
                    {
                        t[1] = 0;
                        t[0]++;
                        t[0] &= 3;
                    }
                    if(s->zvel < 128) s->zvel += (gc/13); // 8
                    else s->zvel -= 64;
                    if(s->xvel > 0)
                        s->xvel -= 4;
                    else s->xvel = 0;
                }
                else
                {
                    t[1]++;
                    if(t[1] > 3)
                    {
                        t[1] = 0;
                        t[0]++;
                        t[0] &= 3;
                    }
                    if(s->zvel < 512) s->zvel += (gc/3); // 52;
                    /*if(s->xvel > 0)
                        s->xvel --;
                    else KILLIT(i);*/
                }

                goto BOLT;

            case GLASSPIECES:
            case GLASSPIECES+1:
            case GLASSPIECES+2:

                makeitfall(i);

                if(s->zvel > 4096) s->zvel = 4096;
                if(sect < 0) KILLIT(i);

                if( s->z == hittype[i].floorz-(FOURSLEIGHT) && t[0] < 3)
                {
                    s->zvel = -((3-t[0])<<8)-(TRAND&511);
                    if(sector[sect].lotag == 2)
                        s->zvel >>= 1;
                    s->xrepeat >>= 1;
                    s->yrepeat >>= 1;
                    if( rnd(96) )
                      setsprite(i,s->x,s->y,s->z);
                    t[0]++;//Number of bounces
                }
                else if( t[0] == 3 ) KILLIT(i);

                if(s->xvel > 0)
                {
                    s->xvel -= 2;
                    s->cstat = ((s->xvel&3)<<2);
                }
                else s->xvel = 0;

                ssp(i,CLIPMASK0);

                goto BOLT;
        }

        IFWITHIN(SCRAP6,SCRAP5+3)
        {
                if(s->xvel > 0)
                    s->xvel--;
                else s->xvel = 0;

                if(s->zvel > 1024 && s->zvel < 1280)
                {
                    setsprite(i,s->x,s->y,s->z);
                    sect = s->sectnum;
                }

                if( s->z < sector[sect].floorz-(2<<8) )
                {
                    if(t[1] < 1) t[1]++;
                    else
                    {
                        t[1] = 0;

                        if(s->picnum < SCRAP6+8)
                        {
                            if(t[0] > 6)
                                t[0] = 0;
                            else t[0]++;
                        }
                        else
                        {
                            if(t[0] > 2)
                                t[0] = 0;
                            else t[0]++;
                        }
                    }
                    if(s->zvel < 4096) s->zvel += gc-50;
                    s->x += (s->xvel*sintable[(s->ang+512)&2047])>>14;
                    s->y += (s->xvel*sintable[s->ang&2047])>>14;
                    s->z += s->zvel;
                }
                else
                {
                    if(s->picnum == SCRAP1 && s->yvel > 0)
                    {
                        j = spawn(i,s->yvel);
                        setsprite(j,s->x,s->y,s->z);
                        getglobalz(j);
                        sprite[j].hitag = sprite[j].lotag = 0;
                    }
                    KILLIT(i);
                }
                goto BOLT;
        }

        BOLT:
        i = nexti;
    }
}

void moveeffectors(void)   //STATNUM 3
{
    long q, l, m, x, st, j, *t;
    short i, k, nexti, nextk, p, sh, nextj;
    spritetype *s;
    sectortype *sc;
    walltype *wal;

    for (p = 0; p < ud.multimode; p++)
    {
        ps[p].fricxv = ps[p].fricyv = 0;
    }

    i = headspritestat[3];
    while(i >= 0)
    {
        nexti = nextspritestat[i];
        s = &sprite[i];

        sc = &sector[s->sectnum];
        st = s->lotag;
        sh = s->hitag;

        t = &hittype[i].temp_data[0];

        switch(st)
        {
            case 0:
            {
                long zchange = 0;

                zchange = 0;

                j = s->owner;

                if( sprite[j].lotag == (short) 65535 )
                    KILLIT(i);

                q = sc->extra>>3;
                l = 0;

                if(sc->lotag == 30)
                {
                    q >>= 2;

                    if( sprite[i].extra == 1 )
                    {
                        if(hittype[i].tempang < 256)
                        {
                            hittype[i].tempang += 4;
                            if(hittype[i].tempang >= 256)
                                callsound(s->sectnum,i);
                            if(s->clipdist) l = 1;
                            else l = -1;
                        }
                        else hittype[i].tempang = 256;

                        if( sc->floorz > s->z ) //z's are touching
                        {
                            sc->floorz -= 512;
                            zchange = -512;
                            if( sc->floorz < s->z )
                                sc->floorz = s->z;
                        }

                        else if( sc->floorz < s->z ) //z's are touching
                        {
                            sc->floorz += 512;
                            zchange = 512;
                            if( sc->floorz > s->z )
                                sc->floorz = s->z;
                        }
                    }
                    else if(sprite[i].extra == 3)
                    {
                        if(hittype[i].tempang > 0)
                        {
                            hittype[i].tempang -= 4;
                            if(hittype[i].tempang <= 0)
                                callsound(s->sectnum,i);
                            if( s->clipdist ) l = -1;
                            else l = 1;
                        }
                        else hittype[i].tempang = 0;

                        if( sc->floorz > T4 ) //z's are touching
                        {
                            sc->floorz -= 512;
                            zchange = -512;
                            if( sc->floorz < T4 )
                                sc->floorz = T4;
                        }

                        else if( sc->floorz < T4 ) //z's are touching
                        {
                            sc->floorz += 512;
                            zchange = 512;
                            if( sc->floorz > T4 )
                                sc->floorz = T4;
                        }
                    }

                    s->ang += (l*q);
                    t[2] += (l*q);
                }
                else
                {
                    if( hittype[j].temp_data[0] == 0 ) break;
                    if( hittype[j].temp_data[0] == 2 ) KILLIT(i);

                    if( sprite[j].ang > 1024 )
                        l = -1;
                    else l = 1;
                    if( t[3] == 0 )
                        t[3] = ldist(s,&sprite[j]);
                    s->xvel = t[3];
                    s->x = sprite[j].x;
                    s->y = sprite[j].y;
                    s->ang += (l*q);
                    t[2] += (l*q);
                }

                if( l && (sc->floorstat&64) )
                {
                    for(p=0;p<ud.multimode;p++)
                    {
                        if( ps[p].cursectnum == s->sectnum && ps[p].on_ground == 1)
                        {

                            ps[p].ang += (l*q);
                            ps[p].ang &= 2047;

                            ps[p].posz += zchange;

                            rotatepoint( sprite[j].x,sprite[j].y,
                                ps[p].posx,ps[p].posy,(q*l),
                                &m,&x);

                            ps[p].bobposx += m-ps[p].posx;
                            ps[p].bobposy += x-ps[p].posy;

                            ps[p].posx = m;
                            ps[p].posy = x;

                            if(sprite[ps[p].i].extra <= 0)
                            {
                                sprite[ps[p].i].x = m;
                                sprite[ps[p].i].y = x;
                            }
                        }
                    }

                    p = headspritesect[s->sectnum];
                    while(p >= 0)
                    {
                        if(sprite[p].statnum != 3 && sprite[p].statnum != 4)
                            if( sprite[p].picnum != LASERLINE )
                        {
                            if(sprite[p].picnum == APLAYER && sprite[p].owner >= 0)
                            {
                                p = nextspritesect[p];
                                continue;
                            }

                            sprite[p].ang += (l*q);
                            sprite[p].ang &= 2047;

                            sprite[p].z += zchange;

                            rotatepoint(sprite[j].x,sprite[j].y,
                                sprite[p].x,sprite[p].y,(q*l),
                                &sprite[p].x,&sprite[p].y);

                        }
                        p = nextspritesect[p];
                    }

                }

                ms(i);
            }

            break;
            case 1: //Nothing for now used as the pivot
                if(s->owner == -1) //Init
                {
                    s->owner = i;

                    j = headspritestat[3];
                    while(j >= 0)
                    {
                        if( sprite[j].lotag == 19 && sprite[j].hitag == sh )
                        {
                            t[0] = 0;
                            break;
                        }
                        j = nextspritestat[j];
                    }
                }

                break;
            case 6:
                k = sc->extra;

                if(t[4] > 0)
                {
                    t[4]--;
                    if( t[4] >= (k-(k>>3)) )
                        s->xvel -= (k>>5);
                    if( t[4] > ((k>>1)-1) && t[4] < (k-(k>>3)) )
                        s->xvel = 0;
                    if( t[4] < (k>>1) )
                        s->xvel += (k>>5);
                    if( t[4] < ((k>>1)-(k>>3)) )
                    {
                        t[4] = 0;
                        s->xvel = k;
                    }
                }
                else s->xvel = k;

                j = headspritestat[3];
                while( j >= 0)
                {
                    if( (sprite[j].lotag == 14) && (sh == sprite[j].hitag) && (hittype[j].temp_data[0] == t[0]) )
                    {
                        sprite[j].xvel = s->xvel;
//                        if( t[4] == 1 )
                        {
                            if(hittype[j].temp_data[5] == 0)
                                hittype[j].temp_data[5] = dist(&sprite[j],s);
                            x = ksgn( dist(&sprite[j],s)-hittype[j].temp_data[5] );
                            if(sprite[j].extra)
                                x = -x;
                            s->xvel += x;
                        }
                        hittype[j].temp_data[4] = t[4];
                    }
                    j = nextspritestat[j];
                }
                x = 0;


            case 14:
                if(s->owner==-1)
                    s->owner = LocateTheLocator((short)t[3],(short)t[0]);

                if(s->owner == -1)
                {
                    sprintf(tempbuf,"Could not find any locators for SE# 6 and 14 with a hitag of %ld.\n",t[3]);
                    gameexit(tempbuf);
                }

                j = ldist(&sprite[s->owner],s);

                if( j < 1024L )
                {
                    if(st==6)
                        if(sprite[s->owner].hitag&1)
                            t[4]=sc->extra; //Slow it down
                    t[3]++;
                    s->owner = LocateTheLocator(t[3],t[0]);
                    if(s->owner==-1)
                    {
                        t[3]=0;
                        s->owner = LocateTheLocator(0,t[0]);
                    }
                }

                if(s->xvel)
                {
                    x = getangle(sprite[s->owner].x-s->x,sprite[s->owner].y-s->y);
                    q = getincangle(s->ang,x)>>3;

                    t[2] += q;
                    s->ang += q;

                    if(s->xvel == sc->extra )
                    {
                        if( (sc->floorstat&1) == 0 && (sc->ceilingstat&1) == 0 )
                        {
                            if( Sound[hittype[i].lastvx].num == 0 )
                                spritesound(hittype[i].lastvx,i);
                        }
                        else if( ud.monsters_off == 0 && sc->floorpal == 0 && (sc->floorstat&1) && rnd(8) )
                        {
                            p = findplayer(s,&x);
                            if(x < 20480)
                            {
                                j = s->ang;
                                s->ang = getangle(s->x-ps[p].posx,s->y-ps[p].posy);
                                shoot(i,RPG);
                                s->ang = j;
                            }
                        }
                    }

                    if(s->xvel <= 64 && (sc->floorstat&1) == 0 && (sc->ceilingstat&1) == 0 )
                        stopsound(hittype[i].lastvx);

                    if( (sc->floorz-sc->ceilingz) < (108<<8) )
                    {
                        if(ud.clipping == 0 && s->xvel >= 192)
                            for(p=0;p<ud.multimode;p++)
                                if(sprite[ps[p].i].extra > 0)
                        {
                            k = ps[p].cursectnum;
                            updatesector(ps[p].posx,ps[p].posy,&k);
                            if( ( k == -1 && ud.clipping == 0 ) || ( k == s->sectnum && ps[p].cursectnum != s->sectnum ) )
                            {
                                ps[p].posx = s->x;
                                ps[p].posy = s->y;
                                ps[p].cursectnum = s->sectnum;

                                setsprite(ps[p].i,s->x,s->y,s->z);
                                quickkill(&ps[p]);
                            }
                        }
                    }

                    m = (s->xvel*sintable[(s->ang+512)&2047])>>14;
                    x = (s->xvel*sintable[s->ang&2047])>>14;

                    for(p=0;p<ud.multimode;p++)
                       if(sector[ps[p].cursectnum].lotag != 2)
                    {
                        if(po[p].os == s->sectnum)
                        {
                            po[p].ox += m;
                            po[p].oy += x;
                        }

                        if(s->sectnum == sprite[ps[p].i].sectnum)
                        {
                            rotatepoint(s->x,s->y,ps[p].posx,ps[p].posy,q,&ps[p].posx,&ps[p].posy);

                            ps[p].posx += m;
                            ps[p].posy += x;

                            ps[p].bobposx += m;
                            ps[p].bobposy += x;

                            ps[p].ang += q;

                            if(numplayers > 1)
                            {
                                ps[p].oposx = ps[p].posx;
                                ps[p].oposy = ps[p].posy;
                            }
                            if( sprite[ps[p].i].extra <= 0 )
                            {
                                sprite[ps[p].i].x = ps[p].posx;
                                sprite[ps[p].i].y = ps[p].posy;
                            }
                        }
                    }
                    j = headspritesect[s->sectnum];
                    while(j >= 0)
                    {
                        if (sprite[j].statnum != 10 && sector[sprite[j].sectnum].lotag != 2 && sprite[j].picnum != SECTOREFFECTOR && sprite[j].picnum != LOCATORS )
                        {
                            rotatepoint(s->x,s->y,
                                sprite[j].x,sprite[j].y,q,
                                &sprite[j].x,&sprite[j].y);

                            sprite[j].x+= m;
                            sprite[j].y+= x;

                            sprite[j].ang+=q;

                            if(numplayers > 1)
                            {
                                hittype[j].bposx = sprite[j].x;
                                hittype[j].bposy = sprite[j].y;
                            }
                        }
                        j = nextspritesect[j];
                    }

                    ms(i);
                    setsprite(i,s->x,s->y,s->z);

                    if( (sc->floorz-sc->ceilingz) < (108<<8) )
                    {
                        if(ud.clipping == 0 && s->xvel >= 192)
                            for(p=0;p<ud.multimode;p++)
                                if(sprite[ps[p].i].extra > 0)
                        {
                            k = ps[p].cursectnum;
                            updatesector(ps[p].posx,ps[p].posy,&k);
                            if( ( k == -1 && ud.clipping == 0 ) || ( k == s->sectnum && ps[p].cursectnum != s->sectnum ) )
                            {
                                ps[p].oposx = ps[p].posx = s->x;
                                ps[p].oposy = ps[p].posy = s->y;
                                ps[p].cursectnum = s->sectnum;

                                setsprite(ps[p].i,s->x,s->y,s->z);
                                quickkill(&ps[p]);
                            }
                        }

                        j = headspritesect[sprite[OW].sectnum];
                        while(j >= 0)
                        {
                            l = nextspritesect[j];
                            if (sprite[j].statnum == 1 && badguy(&sprite[j]) && sprite[j].picnum != SECTOREFFECTOR && sprite[j].picnum != LOCATORS )
                            {
                                k = sprite[j].sectnum;
                                updatesector(sprite[j].x,sprite[j].y,&k);
                                if( sprite[j].extra >= 0 && k == s->sectnum )
                                {
                                    gutsdir(&sprite[j],JIBS6,72,myconnectindex);
                                    spritesound(SQUISHED,i);
                                    deletesprite(j);
                                }
                            }
                            j = l;
                        }
                    }
                }

                break;

            case 30:
                if (s->xvel && s->pal)
                    if (Sound[hittype[i].lastvx].num == 0)
                        spritesound(hittype[i].lastvx,i);

                if (t[4] == 0 && s->pal)
                    t[4] = 300;
                if(s->owner == -1)
                {
                    t[3] = !t[3];
                    s->owner = LocateTheLocator(t[3],t[0]);
                }
                else
                {

                    if(t[4] == 1) // Starting to go
                    {
                        if( ldist( &sprite[s->owner],s ) < (2048-128) )
                            t[4] = 2;
                        else
                        {
                            if(s->xvel == 0 && s->pal == 0)
                                operateactivators(s->hitag+(!t[3]),-1);
                            if(s->xvel < 256)
                                s->xvel += 16;
                        }
                    }
                    if(t[4] == 2)
                    {
                        l = FindDistance2D(sprite[s->owner].x-s->x,sprite[s->owner].y-s->y);

                        if(l <= 128)
                            s->xvel = 0;

                        if( s->xvel > 0 )
                            s->xvel -= 16;
                        else
                        {
                            s->xvel = 0;
                            if (s->pal == 0)
                                operateactivators(s->hitag+(short)t[3],-1);
                            s->owner = -1;
                            s->ang += 1024;
                            t[4] = 0;
                            if (s->pal == 0)
                                operateforcefields(i,s->hitag);

                            j = headspritesect[s->sectnum];
                            while(j >= 0)
                            {
                                if(sprite[j].picnum != SECTOREFFECTOR && sprite[j].picnum != LOCATORS )
                                {
                                    hittype[j].bposx = sprite[j].x;
                                    hittype[j].bposy = sprite[j].y;
                                }
                                j = nextspritesect[j];
                            }

                        }
                    }
                    if(t[4] > 10)
                    {
                        if (--t[4] == 10)
                            t[4] = 1;
                    }
                }

                if(s->xvel)
                {
                    l = (s->xvel*sintable[(s->ang+512)&2047])>>14;
                    x = (s->xvel*sintable[s->ang&2047])>>14;

                    if( (sc->floorz-sc->ceilingz) < (108<<8) )
                        if(ud.clipping == 0)
                            for(p=0;p<ud.multimode;p++)
                                if(sprite[ps[p].i].extra > 0)
                    {
                        k = ps[p].cursectnum;
                        updatesector(ps[p].posx,ps[p].posy,&k);
                        if( ( k == -1 && ud.clipping == 0 ) || ( k == s->sectnum && ps[p].cursectnum != s->sectnum ) )
                        {
                            ps[p].posx = s->x;
                            ps[p].posy = s->y;
                            ps[p].cursectnum = s->sectnum;

                            setsprite(ps[p].i,s->x,s->y,s->z);
                            quickkill(&ps[p]);
                        }
                    }

                    for(p=0;p<ud.multimode;p++)
                    {
                        if( sprite[ps[p].i].sectnum == s->sectnum )
                        {
                            ps[p].posx += l;
                            ps[p].posy += x;

                            if(numplayers > 1)
                            {
                                ps[p].oposx = ps[p].posx;
                                ps[p].oposy = ps[p].posy;
                            }

                            ps[p].bobposx += l;
                            ps[p].bobposy += x;
                        }

                        if( po[p].os == s->sectnum )
                        {
                            po[p].ox += l;
                            po[p].oy += x;
                        }
                    }

                    j = headspritesect[s->sectnum];
                    while(j >= 0)
                    {
                        if(sprite[j].picnum != SECTOREFFECTOR && sprite[j].picnum != LOCATORS )
                        {
                            if(numplayers < 2)
                            {
                                hittype[j].bposx = sprite[j].x;
                                hittype[j].bposy = sprite[j].y;
                            }

                            sprite[j].x += l;
                            sprite[j].y += x;

                            if(numplayers > 1)
                            {
                                hittype[j].bposx = sprite[j].x;
                                hittype[j].bposy = sprite[j].y;
                            }
                        }
                        j = nextspritesect[j];
                    }

                    ms(i);
                    setsprite(i,s->x,s->y,s->z);

                    if( (sc->floorz-sc->ceilingz) < (108<<8) )
                    {
                        if(ud.clipping == 0)
                            for(p=0;p<ud.multimode;p++)
                                if(sprite[ps[p].i].extra > 0)
                        {
                            k = ps[p].cursectnum;
                            updatesector(ps[p].posx,ps[p].posy,&k);
                            if( ( k == -1 && ud.clipping == 0 ) || ( k == s->sectnum && ps[p].cursectnum != s->sectnum ) )
                            {
                                ps[p].posx = s->x;
                                ps[p].posy = s->y;

                                ps[p].oposx = ps[p].posx;
                                ps[p].oposy = ps[p].posy;

                                ps[p].cursectnum = s->sectnum;

                                setsprite(ps[p].i,s->x,s->y,s->z);
                                quickkill(&ps[p]);
                            }
                        }

                        j = headspritesect[sprite[OW].sectnum];
                        while(j >= 0)
                        {
                            l = nextspritesect[j];
                            if (sprite[j].statnum == 1 && badguy(&sprite[j]) && sprite[j].picnum != SECTOREFFECTOR && sprite[j].picnum != LOCATORS )
                            {
            //                    if(sprite[j].sectnum != s->sectnum)
                                {
                                    k = sprite[j].sectnum;
                                    updatesector(sprite[j].x,sprite[j].y,&k);
                                    if( sprite[j].extra >= 0 && k == s->sectnum )
                                    {
                                        gutsdir(&sprite[j],JIBS6,24,myconnectindex);
                                        spritesound(SQUISHED,j);
                                        deletesprite(j);
                                    }
                                }

                            }
                            j = l;
                        }
                    }
                }

                break;


            case 2://Quakes
                if(t[4] > 0 && t[0] == 0 )
                {
                    if( t[4] < sh )
                        t[4]++;
                    else t[0] = 1;
                }

                if(t[0] > 0)
                {
                    t[0]++;

                    s->xvel = 3;

                    if(t[0] > 96)
                    {
                        t[0] = -1; //Stop the quake
                        t[4] = -1;
                        KILLIT(i);
                    }
                    else
                    {
                        if( (t[0]&31) ==  8 )
                        {
                            earthquaketime = 48;
                            spritesound(EARTHQUAKE,ps[screenpeek].i);
                        }

                        if( klabs2( sc->floorheinum-t[5] ) < 8 )
                            sc->floorheinum = t[5];
                        else sc->floorheinum += ( ksgn(t[5]-sc->floorheinum)<<4 );
                    }

                    /*m = (s->xvel*sintable[(s->ang+512)&2047])>>14;
                    x = (s->xvel*sintable[s->ang&2047])>>14;


                    for(p=0;p<ud.multimode;p++)
                        if(ps[p].cursectnum == s->sectnum && ps[p].on_ground)
                        {
                            ps[p].posx += m;
                            ps[p].posy += x;

                            ps[p].bobposx += m;
                            ps[p].bobposy += x;
                        }

                    j = headspritesect[s->sectnum];
                    while(j >= 0)
                    {
                        nextj = nextspritesect[j];

                        if (sprite[j].picnum != SECTOREFFECTOR)
                        {
                            sprite[j].x+=m;
                            sprite[j].y+=x;
                            setsprite(j,sprite[j].x,sprite[j].y,sprite[j].z);
                        }
                        j = nextj;
                    }
                    ms(i);
                    setsprite(i,s->x,s->y,s->z);*/
                }
                break;

            //Flashing sector lights after reactor EXPLOSION2

            case 3:

                if( t[4] == 0 ) break;
                p = findplayer(s,&x);

            //    if(t[5] > 0) { t[5]--; break; }

                if( (global_random/(sh+1)&31) < 4 && !t[2])
                {
             //       t[5] = 4+(global_random&7);
                    sc->ceilingpal = s->owner>>8;
                    sc->floorpal = s->owner&0xff;
                    t[0] = s->shade + (global_random&15);
                }
                else
                {
             //       t[5] = 4+(global_random&3);
                    sc->ceilingpal = s->pal;
                    sc->floorpal = s->pal;
                    t[0] = t[3];
                }

                sc->ceilingshade = t[0];
                sc->floorshade = t[0];

                wal = &wall[sc->wallptr];

                for(x=sc->wallnum;x > 0;x--,wal++)
                {
                    if( wal->hitag != 1 )
                    {
                        wal->shade = t[0];
                        if((wal->cstat&2) && wal->nextwall >= 0)
                        {
                            wall[wal->nextwall].shade = wal->shade;
                        }
                    }
                }

                break;

            case 4:

                if((global_random/(sh+1)&31) < 4 )
                {
                    t[1] = s->shade + (global_random&15);//Got really bright
                    t[0] = s->shade + (global_random&15);
                    sc->ceilingpal = s->owner>>8;
                    sc->floorpal = s->owner&0xff;
                    j = 1;
                }
                else
                {
                    t[1] = t[2];
                    t[0] = t[3];

                    sc->ceilingpal = s->pal;
                    sc->floorpal = s->pal;

                    j = 0;
                }

                sc->floorshade = t[1];
                sc->ceilingshade = t[1];

                wal = &wall[sc->wallptr];

                for(x=sc->wallnum;x > 0; x--,wal++)
                {
                    if(j) wal->pal = (s->owner&0xff);
                    else wal->pal = s->pal;

                    if( wal->hitag != 1 )
                    {
                        wal->shade = t[0];
                        if((wal->cstat&2) && wal->nextwall >= 0)
                            wall[wal->nextwall].shade = wal->shade;
                    }
                }

                j = headspritesect[SECT];
                while(j >= 0)
                {
                    if(sprite[j].cstat&16)
                    {
                        if (sc->ceilingstat&1)
                            sprite[j].shade = sc->ceilingshade;
                        else sprite[j].shade = sc->floorshade;
                    }

                    j = nextspritesect[j];
                }

                if(t[4]) KILLIT(i);

                break;

            //BOSS
            /*case 5:
                p = findplayer(s,&x);
                if(x < 8192)
                {
                    j = s->ang;
                    s->ang = getangle(s->x-ps[p].posx,s->y-ps[p].posy);
                    shoot(i,FIRELASER);
                    s->ang = j;
                }

                if(s->owner==-1) //Start search
                {
                    t[4]=0;
                    l = 0x7fffffff;
                    while(1) //Find the shortest dist
                    {
                        s->owner = LocateTheLocator((short)t[4],-1); //t[0] hold sectnum

                        if(s->owner==-1) break;

                        m = ldist(&sprite[ps[p].i],&sprite[s->owner]);

                        if(l > m)
                        {
                            q = s->owner;
                            l = m;
                        }

                        t[4]++;
                    }

                    s->owner = q;
                    s->zvel = ksgn(sprite[q].z-s->z)<<4;
                }

                if(ldist(&sprite[s->owner],s) < 1024)
                {
                    short ta;
                    ta = s->ang;
                    s->ang = getangle(ps[p].posx-s->x,ps[p].posy-s->y);
                    s->ang = ta;
                    s->owner = -1;
                    goto BOLT;

                }
                else s->xvel=256;

                x = getangle(sprite[s->owner].x-s->x,sprite[s->owner].y-s->y);
                q = getincangle(s->ang,x)>>3;
                s->ang += q;
                
                if(rnd(32))
                {
                    t[2]+=q;
                    sc->ceilingshade = 127;
                }
                else
                {
                    t[2] +=
                        getincangle(t[2]+512,getangle(ps[p].posx-s->x,ps[p].posy-s->y))>>2;
                    sc->ceilingshade = 0;
                }
                IFHIT
                {
                    t[3]++;
                    if(t[3] == 5)
                    {
                        s->zvel += 1024;
                        FTA(7,&ps[myconnectindex]);
                    }
                }

                s->z += s->zvel;
                sc->ceilingz += s->zvel;
                sector[t[0]].ceilingz += s->zvel;
                ms(i);
                setsprite(i,s->x,s->y,s->z);
                break;*/

            
            case 8:
            case 9:

                // work only if its moving

                j = -1;

                if(hittype[i].temp_data[4])
                {
                    hittype[i].temp_data[4]++;
                    if( hittype[i].temp_data[4] > 8 ) KILLIT(i);
                    j = 1;
                }
                else j = getanimationgoal(&sc->ceilingz);

                if( j >= 0 )
                {
                    short sn;

                    if( (sc->lotag&0x8000) || hittype[i].temp_data[4] )
                        x = -t[3];
                    else
                        x = t[3];

                    if ( st == 9 ) x = -x;

                    j = headspritestat[3];
                    while(j >= 0)
                    {
                        if( ((sprite[j].lotag) == st ) && (sprite[j].hitag) == sh )
                        {
                            sn = sprite[j].sectnum;
                            m = sprite[j].shade;

                            wal = &wall[sector[sn].wallptr];

                            for(l=sector[sn].wallnum;l>0;l--,wal++)
                            {
                                if( wal->hitag != 1 )
                                {
                                    wal->shade+=x;

                                    if(wal->shade < m)
                                        wal->shade = m;
                                    else if(wal->shade > hittype[j].temp_data[2])
                                        wal->shade = hittype[j].temp_data[2];

                                    if(wal->nextwall >= 0)
                                        if(wall[wal->nextwall].hitag != 1)
                                            wall[wal->nextwall].shade = wal->shade;
                                }
                            }

                            sector[sn].floorshade   += x;
                            sector[sn].ceilingshade += x;

                            if(sector[sn].floorshade < m)
                                sector[sn].floorshade = m;
                            else if(sector[sn].floorshade > hittype[j].temp_data[0])
                                sector[sn].floorshade = hittype[j].temp_data[0];

                            if(sector[sn].ceilingshade < m)
                                sector[sn].ceilingshade = m;
                            else if(sector[sn].ceilingshade > hittype[j].temp_data[1])
                                sector[sn].ceilingshade = hittype[j].temp_data[1];

                        }
                        j = nextspritestat[j];
                    }
                }
                break;
            case 10:

                if( (sc->lotag&0xff) == 27 || ( sc->floorz > sc->ceilingz && (sc->lotag&0xff) != 23 ) || sc->lotag == (short) 32791 )
                {
                    j = 1;

                    if( (sc->lotag&0xff) != 27)
                        for(p=0;p<ud.multimode;p++)
                            if( sc->lotag != 30 && sc->lotag != 31 && sc->lotag != 0 )
                                if(s->sectnum == sprite[ps[p].i].sectnum)
                                    j = 0;

                    if(j == 1)
                    {
                        if(t[0] > sh )
                            switch(sector[s->sectnum].lotag)
                            {
                                case 20:
                                case 21:
                                case 22:
                                case 26:
                                  if( getanimationgoal(&sector[s->sectnum].ceilingz) >= 0 )
                                      break;
                                default:
                                  activatebysector(s->sectnum,i);
                                  t[0] = 0;
                                  break;
                            }
                        else t[0]++;
                    }
                }
                else t[0]=0;
                break;
            case 11: //Swingdoor

                if( t[5] > 0)
                {
                    t[5]--;
                    break;
                }

                if( t[4] )
                {
                    short startwall,endwall;

                    startwall = sc->wallptr;
                    endwall = startwall+sc->wallnum;

                    for(j=startwall;j<endwall;j++)
                    {
                        k = headspritestat[1];
                        while(k >= 0)
                        {
                            if( sprite[k].extra > 0 && badguy(&sprite[k]) && clipinsidebox(sprite[k].x,sprite[k].y,j,256L) == 1 )
                                goto BOLT;
                            k = nextspritestat[k];
                        }

                        k = headspritestat[10];
                        while(k >= 0)
                        {
                            if( sprite[k].owner >= 0 && clipinsidebox(sprite[k].x,sprite[k].y,j,144L) == 1 )
                            {
                                t[5] = 8; // Delay
                                k = (SP>>3)*t[3];
                                t[2]-=k;
                                t[4]-=k;
                                ms(i);
                                setsprite(i,s->x,s->y,s->z);
                                goto BOLT;
                            }
                            k = nextspritestat[k];
                        }
                    }

                    k = (SP>>3)*t[3];
                    t[2]+=k;
                    t[4]+=k;
                    ms(i);
                    setsprite(i,s->x,s->y,s->z);

                    if(t[4] <= -511 || t[4] >= 512)
                    {
                        t[4] = 0;
                        t[2] &= 0xffffff00;
                        ms(i);
                        setsprite(i,s->x,s->y,s->z);
                        break;
                    }
                }
                break;
            case 12:
                if( t[0] == 3 || t[3] == 1 ) //Lights going off
                {
                    sc->floorpal = 0;
                    sc->ceilingpal = 0;

                    wal = &wall[sc->wallptr];
                    for(j = sc->wallnum;j > 0; j--, wal++)
                        if(wal->hitag != 1)
                        {
                            wal->shade = t[1];
                            wal->pal = 0;
                        }

                    sc->floorshade = t[1];
                    sc->ceilingshade = t[2];
                    t[0]=0;

                    j = headspritesect[SECT];
                    while(j >= 0)
                    {
                        if(sprite[j].cstat&16)
                        {
                            if (sc->ceilingstat&1)
                                sprite[j].shade = sc->ceilingshade;
                            else sprite[j].shade = sc->floorshade;
                        }
                        j = nextspritesect[j];

                    }

                    if(t[3] == 1) KILLIT(i);
                }
                if( t[0] == 1 ) //Lights flickering on
                {
                    if( sc->floorshade > s->shade )
                    {
                        sc->floorpal = s->pal;
                        sc->ceilingpal = s->pal;

                        sc->floorshade -= 2;
                        sc->ceilingshade -= 2;

                        wal = &wall[sc->wallptr];
                        for(j=sc->wallnum;j>0;j--,wal++)
                            if(wal->hitag != 1)
                            {
                                wal->pal = s->pal;
                                wal->shade -= 2;
                            }
                    }
                    else t[0] = 2;

                    j = headspritesect[SECT];
                    while(j >= 0)
                    {
                        if(sprite[j].cstat&16)
                        {
                            if (sc->ceilingstat&1)
                                sprite[j].shade = sc->ceilingshade;
                            else sprite[j].shade = sc->floorshade;
                        }
                        j = nextspritesect[j];
                    }
                }
                break;


            case 13:
                if( t[2] )
                {
                    j = (SP<<5)|1;

                    if( s->ang == 512 )
                    {
                        if( s->owner )
                        {
                            if( klabs2(t[0]-sc->ceilingz) >= j )
                                sc->ceilingz += ksgn(t[0]-sc->ceilingz)*j;
                            else sc->ceilingz = t[0];
                        }
                        else
                        {
                            if( klabs2(t[1]-sc->floorz) >= j )
                                sc->floorz += ksgn(t[1]-sc->floorz)*j;
                            else sc->floorz = t[1];
                        }
                    }
                    else
                    {
                        if( klabs2(t[1]-sc->floorz) >= j )
                            sc->floorz += ksgn(t[1]-sc->floorz)*j;
                        else sc->floorz = t[1];
                        if( klabs2(t[0]-sc->ceilingz) >= j )
                            sc->ceilingz += ksgn(t[0]-sc->ceilingz)*j;
                        sc->ceilingz = t[0];
                    }

                    if( t[3] == 1 )
                    {
                        //Change the shades

                        t[3]++;
                        sc->ceilingstat ^= 1;

                        if(s->ang == 512)
                        {
                            wal = &wall[sc->wallptr];
                            for(j=sc->wallnum;j>0;j--,wal++)
                                wal->shade = s->shade;

                            sc->floorshade = s->shade;

                            /*if(ps[0].one_parallax_sectnum >= 0)
                            {
                                sc->ceilingpicnum =
                                    sector[ps[0].one_parallax_sectnum].ceilingpicnum;
                                sc->ceilingshade  =
                                    sector[ps[0].one_parallax_sectnum].ceilingshade;
                            }*/
                        }
                    }
                    t[2]++;
                    if(t[2] > 256)
                        KILLIT(i);
                }


                if( t[2] == 4 && s->ang != 512)
                    for(x=0;x<7;x++) RANDOMSCRAP;
                break;


            case 15:

                if(t[4])
                {
                    s->xvel = 16;

                    if(t[4] == 1) //Opening
                    {
                        if( t[3] >= (SP>>3) )
                        {
                            t[4] = 0; //Turn off the sliders
                            callsound(s->sectnum,i);
                            break;
                        }
                        t[3]++;
                    }
                    else if(t[4] == 2)
                    {
                        if(t[3]<1)
                        {
                            t[4] = 0;
                            callsound(s->sectnum,i);
                            break;
                        }
                        t[3]--;
                    }

                    ms(i);
                    setsprite(i,s->x,s->y,s->z);
                }
                break;

            case 16: //Reactor

                t[2]+=32;
                if(sc->floorz<sc->ceilingz) s->shade=0;

                else if( sc->ceilingz < t[3] )
                {

                    //The following code check to see if
                    //there is any other sprites in the sector.
                    //If there isn't, then kill this sectoreffector
                    //itself.....

                    j = headspritesect[s->sectnum];
                    while(j >= 0)
                    {
                        if(sprite[j].picnum == REACTOR || sprite[j].picnum == REACTOR2)
                            break;
                        j = nextspritesect[j];
                    }
                    if(j == -1) { KILLIT(i); }
                    else s->shade=1;
                }

                if(s->shade) sc->ceilingz+=1024;
                else sc->ceilingz-=512;

                ms(i);
                setsprite(i,s->x,s->y,s->z);

                break;

            case 17:

                q = t[0]*(SP<<2);

                sc->ceilingz += q;
                sc->floorz += q;

                j = headspritesect[s->sectnum];
                while(j >= 0)
                {
                    if(sprite[j].statnum == 10 && sprite[j].owner >= 0)
                    {
                        p = sprite[j].yvel;
                        if(numplayers < 2)
                            ps[p].oposz = ps[p].posz;
                        ps[p].posz += q;
                        ps[p].truefz += q;
                        ps[p].truecz += q;
                        if(numplayers > 1)
                            ps[p].oposz = ps[p].posz;
                    }
                    if( sprite[j].statnum != 3 )
                    {
                        hittype[j].bposz = sprite[j].z;
                        sprite[j].z += q;
                    }

                    hittype[j].floorz = sc->floorz;
                    hittype[j].ceilingz = sc->ceilingz;

                    j = nextspritesect[j];
                }

                if( t[0] ) //If in motion
                {
                    if( klabs2(sc->floorz-t[2]) <= SP)
                    {
                        activatewarpelevators(i,0);
                        break;
                    }

                    if(t[0]==-1)
                    {
                        if( sc->floorz > t[3] )
                            break;
                    }
                    else if( sc->ceilingz < t[4] ) break;

                    if( t[1] == 0 ) break;
                    t[1] = 0;

                    j = headspritestat[3];
                    while(j >= 0)
                    {
                                if( i != j && (sprite[j].lotag) == 17)
                                    if( (sc->hitag-t[0]) ==
                                        (sector[sprite[j].sectnum].hitag)
                                        && sh == (sprite[j].hitag))
                                            break;
                                j = nextspritestat[j];
                    }

                    if(j == -1) break;

                    k = headspritesect[s->sectnum];
                    while(k >= 0)
                    {
                        nextk = nextspritesect[k];

                        if(sprite[k].statnum == 10 && sprite[k].owner >= 0)
                        {
                            p = sprite[k].yvel;

                            ps[p].posx += sprite[j].x-s->x;
                            ps[p].posy += sprite[j].y-s->y;
                            ps[p].posz = sector[sprite[j].sectnum].floorz-(sc->floorz-ps[p].posz);

                            hittype[k].floorz = sector[sprite[j].sectnum].floorz;
                            hittype[k].ceilingz = sector[sprite[j].sectnum].ceilingz;

                            ps[p].bobposx = ps[p].oposx = ps[p].posx;
                            ps[p].bobposy = ps[p].oposy = ps[p].posy;
                            ps[p].oposz = ps[p].posz;

                            ps[p].truefz = hittype[k].floorz;
                            ps[p].truecz = hittype[k].ceilingz;
                            ps[p].bobcounter = 0;

                            changespritesect(k,sprite[j].sectnum);
                            ps[p].cursectnum = sprite[j].sectnum;
                        }
                        else if( sprite[k].statnum != 3 )
                        {
                            sprite[k].x +=
                                sprite[j].x-s->x;
                            sprite[k].y +=
                                sprite[j].y-s->y;
                            sprite[k].z = sector[sprite[j].sectnum].floorz-
                                (sc->floorz-sprite[k].z);

                            hittype[k].bposx = sprite[k].x;
                            hittype[k].bposy = sprite[k].y;
                            hittype[k].bposz = sprite[k].z;

                            changespritesect(k,sprite[j].sectnum);
                            setsprite(k,sprite[k].x,sprite[k].y,sprite[k].z);

                            hittype[k].floorz = sector[sprite[j].sectnum].floorz;
                            hittype[k].ceilingz = sector[sprite[j].sectnum].ceilingz;

                        }
                        k = nextk;
                    }
                }
                break;

            case 18:
                if(t[0])
                {
                    if(s->pal)
                    {
                        if(s->ang == 512)
                        {
                            sc->ceilingz -= sc->extra;
                            if(sc->ceilingz <= t[1])
                            {
                                sc->ceilingz = t[1];
                                KILLIT(i);
                            }
                        }
                        else
                        {
                            sc->floorz += sc->extra;
                                j = headspritesect[s->sectnum];
                                while(j >= 0)
                                {
                                    if(sprite[j].picnum == APLAYER && sprite[j].owner >= 0)
                                        if( ps[sprite[j].yvel].on_ground == 1 )
                                            ps[sprite[j].yvel].posz += sc->extra;
                                    if( sprite[j].zvel == 0 && sprite[j].statnum != 3 && sprite[j].statnum != 4)
                                    {
                                        hittype[j].bposz = sprite[j].z += sc->extra;
                                        hittype[j].floorz = sc->floorz;
                                    }
                                    j = nextspritesect[j];
                                }
                            if(sc->floorz >= t[1])
                            {
                                sc->floorz = t[1];
                                KILLIT(i);
                            }
                        }
                    }
                    else
                    {
                        if(s->ang == 512)
                        {
                            sc->ceilingz += sc->extra;
                            if(sc->ceilingz >= s->z)
                            {
                                sc->ceilingz = s->z;
                                KILLIT(i);
                            }
                        }
                        else
                        {
                            sc->floorz -= sc->extra;
                                j = headspritesect[s->sectnum];
                                while(j >= 0)
                                {
                                    if(sprite[j].picnum == APLAYER && sprite[j].owner >= 0)
                                        if( ps[sprite[j].yvel].on_ground == 1 )
                                            ps[sprite[j].yvel].posz -= sc->extra;
                                    if( sprite[j].zvel == 0 && sprite[j].statnum != 3 && sprite[j].statnum != 4)
                                    {
                                        hittype[j].bposz = sprite[j].z -= sc->extra;
                                        hittype[j].floorz = sc->floorz;
                                    }
                                    j = nextspritesect[j];
                                }
                            if(sc->floorz <= s->z)
                            {
                                sc->floorz = s->z;
                                KILLIT(i);
                            }
                        }
                    }

                    t[2]++;
                    if(t[2] >= s->hitag)
                    {
                        t[2] = 0;
                        t[0] = 0;
                    }
                }
                break;

            case 19: //Battlestar galactia shields

                if(t[0])
                {
                    if(t[0] == 1)
                    {
                        t[0]++;
                        x = sc->wallptr;
                        q = x+sc->wallnum;
                        for(j=x;j<q;j++)
                            if(wall[j].overpicnum == BIGFORCE)
                            {
                                wall[j].cstat &= (128+32+8+4+2);
                                wall[j].overpicnum = 0;
                                if(wall[j].nextwall >= 0)
                                {
                                    wall[wall[j].nextwall].overpicnum = 0;
                                    wall[wall[j].nextwall].cstat &= (128+32+8+4+2);
                                }
                            }
                    }

                    if(sc->ceilingz < sc->floorz)
                        sc->ceilingz += SP;
                    else
                    {
                        sc->ceilingz = sc->floorz;

                        j = headspritestat[3];
                        while(j >= 0)
                        {
                            if(sprite[j].lotag == 0 && sprite[j].hitag==sh)
                            {
                                q = sprite[sprite[j].owner].sectnum;
                                sector[sprite[j].sectnum].floorpal = sector[sprite[j].sectnum].ceilingpal =
                                        sector[q].floorpal;
                                sector[sprite[j].sectnum].floorshade = sector[sprite[j].sectnum].ceilingshade =
                                    sector[q].floorshade;

                                hittype[sprite[j].owner].temp_data[0] = 2;
                            }
                            j = nextspritestat[j];
                        }
                        KILLIT(i);
                    }
                }
                else //Not hit yet
                {
                    IFHITSECT
                    {
                        FTA(8,&ps[myconnectindex]);

                        l = headspritestat[3];
                        while(l >= 0)
                        {
                            x = sprite[l].lotag&0x7fff;
                            switch( x )
                            {
                                case 0:
                                    if(sprite[l].hitag == sh)
                                    {
                                        q = sprite[l].sectnum;
                                        sector[q].floorshade =
                                            sector[q].ceilingshade =
                                                sprite[sprite[l].owner].shade;
                                        sector[q].floorpal =
                                            sector[q].ceilingpal =
                                                sprite[sprite[l].owner].pal;
                                    }
                                    break;

                                case 1:
                                case 12:
//                                case 18:
                                case 19:

                                    if( sh == sprite[l].hitag )
                                        if( hittype[l].temp_data[0] == 0 )
                                        {
                                            hittype[l].temp_data[0] = 1; //Shut them all on
                                            sprite[l].owner = i;
                                        }

                                    break;
                            }
                            l = nextspritestat[l];
                        }
                    }
                }

                break;

            case 20: //Extend-o-bridge

                if( t[0] == 0 ) break;
                if( t[0] == 1 ) s->xvel = 8;
                else s->xvel = -8;

                if( s->xvel ) //Moving
                {
                    x = (s->xvel*sintable[(s->ang+512)&2047])>>14;
                    l = (s->xvel*sintable[s->ang&2047])>>14;

                    t[3] += s->xvel;

                    s->x += x;
                    s->y += l;

                    if( t[3] <= 0 || (t[3]>>6) >= (SP>>6) )
                    {
                        s->x -= x;
                        s->y -= l;
                        t[0] = 0;
                        callsound(s->sectnum,i);
                        break;
                    }

                    j = headspritesect[s->sectnum];
                    while(j >= 0)
                    {
                        nextj = nextspritesect[j];

                        if( sprite[j].statnum != 3 && sprite[j].zvel == 0)
                        {
                            sprite[j].x += x;
                            sprite[j].y += l;
                            setsprite(j,sprite[j].x,sprite[j].y,sprite[j].z);
                            if( sector[sprite[j].sectnum].floorstat&2 )
                                if(sprite[j].statnum == 2)
                                    makeitfall(j);
                        }
                        j = nextj;
                    }

                    dragpoint((short)t[1],wall[t[1]].x+x,wall[t[1]].y+l);
                    dragpoint((short)t[2],wall[t[2]].x+x,wall[t[2]].y+l);

                    for(p=0;p<ud.multimode;p++)
                        if(ps[p].cursectnum == s->sectnum && ps[p].on_ground)
                        {
                            ps[p].posx += x;
                            ps[p].posy += l;

                            ps[p].oposx = ps[p].posx;
                            ps[p].oposy = ps[p].posy;

                            setsprite(ps[p].i,ps[p].posx,ps[p].posy,ps[p].posz+PHEIGHT);
                        }

                    sc->floorxpanning-=x>>3;
                    sc->floorypanning-=l>>3;

                    adjustfloorpanning(s->sectnum, -x>>3, -l>>3);

                    sc->ceilingxpanning-=x>>3;
                    sc->ceilingypanning-=l>>3;

                    adjustceilingpanning(s->sectnum, -x>>3, -l>>3);
                }

                break;

            case 21: // Cascading effect

                if( t[0] == 0 ) break;

                if( s->ang == 1536 )
                    l = (long) &sc->ceilingz;
                else
                    l = (long) &sc->floorz;

                if( t[0] == 1 ) //Decide if the s->sectnum should go up or down
                {
                    s->zvel = ksgn(s->z-*(long *)l) * (SP<<4);
                    t[0]++;
                }

                if( sc->extra == 0 )
                {
                    *(long *)l += s->zvel;

                    if(klabs2(*(long *)l-s->z) < 1024)
                    {
                        *(long *)l = s->z;
                        KILLIT(i); //All done
                    }
                }
                else sc->extra--;
                break;

            case 22:

                if( t[1] )
                {
                    if(getanimationgoal(&sector[t[0]].ceilingz) >= 0)
                        sc->ceilingz += sc->extra*9;
                    else t[1] = 0;
                }
                break;

            case 24:
            case 34:

                if(t[4]) break;

                x = (SP*sintable[(s->ang+512)&2047])>>18;
                l = (SP*sintable[s->ang&2047])>>18;

                k = 0;

                j = headspritesect[s->sectnum];
                while(j >= 0)
                {
                    nextj = nextspritesect[j];
                    if(sprite[j].zvel >= 0)
                        switch(sprite[j].statnum)
                    {
                        case 5:
                            switch(sprite[j].picnum)
                            {
                                case BLOODPOOL:
                                case PUKE:
                                case FOOTPRINTS:
                                case FOOTPRINTS2:
                                case FOOTPRINTS3:
                                case FOOTPRINTS4:
                                case BULLETHOLE:
                                case BLOODSPLAT1:
                                case BLOODSPLAT2:
                                case BLOODSPLAT3:
                                case BLOODSPLAT4:
                                    sprite[j].xrepeat = sprite[j].yrepeat = 0;
                                    j = nextj;
                                    continue;
                                case LASERLINE:
                                    j = nextj;
                                    continue;
                            }
                        case 6:
                            if(sprite[j].picnum == TRIPBOMB) break;
                        case 1:
                        case 0:
                            if(
                                sprite[j].picnum == BOLT1 ||
                                sprite[j].picnum == BOLT1+1 ||
                                sprite[j].picnum == BOLT1+2 ||
                                sprite[j].picnum == BOLT1+3 ||
                                sprite[j].picnum == SIDEBOLT1 ||
                                sprite[j].picnum == SIDEBOLT1+1 ||
                                sprite[j].picnum == SIDEBOLT1+2 ||
                                sprite[j].picnum == SIDEBOLT1+3 ||
                                wallswitchcheck(j)
                              )
                              break;

                            if( !(sprite[j].picnum >= CRANE && sprite[j].picnum <= (CRANE+3)))
                            {
                                if( sprite[j].z > (hittype[j].floorz-(16<<8)) )
                                {
                                    hittype[j].bposx = sprite[j].x;
                                    hittype[j].bposy = sprite[j].y;

                                    sprite[j].x += x>>2;
                                    sprite[j].y += l>>2;

                                    setsprite(j,sprite[j].x,sprite[j].y,sprite[j].z);

                                    if( sector[sprite[j].sectnum].floorstat&2 )
                                        if(sprite[j].statnum == 2)
                                            makeitfall(j);
                                }
                            }
                            break;
                    }
                    j = nextj;
                }

                for (p = 0; p < ud.multimode; p++)
                {
                    if(ps[p].cursectnum == s->sectnum && ps[p].on_ground)
                        if( klabs2(ps[p].posz-ps[p].truefz) < PHEIGHT+(9<<8) )
                    {
                        ps[p].fricxv += x<<3;
                        ps[p].fricyv += l<<3;
                    }
                }

                sc->floorxpanning += SP>>7;
                adjustfloorpanning(s->sectnum, SP>>7, 0);

            break;

            case 35:
                if(sc->ceilingz > s->z)
                    for(j = 0;j < 8;j++)
                {
                    s->ang += TRAND&511;
                    k = spawn(i,SMALLSMOKE);
                    sprite[k].xvel = 96+(TRAND&127);
                    ssp(k,CLIPMASK0);
                    setsprite(k,sprite[k].x,sprite[k].y,sprite[k].z);
                    if( rnd(16) )
                        k = spawn(i,EXPLOSION2);
                }

                switch(t[0])
                {
                    case 0:
                        sc->ceilingz += s->yvel;
                        if(sc->ceilingz > sc->floorz)
                            sc->floorz = sc->ceilingz;
                        if(sc->ceilingz > s->z+(32<<8))
                            t[0]++;
                        break;
                    case 1:
                        sc->ceilingz-=(s->yvel<<2);
                        if(sc->ceilingz < t[4])
                        {
                            sc->ceilingz = t[4];
                            t[0] = 0;
                        }
                        break;
                }
                break;

            case 25: //PISTONS

                if( t[4] == 0 ) break;

                if(sc->floorz <= sc->ceilingz)
                    s->shade = 0;
                else if( sc->ceilingz <= t[3])
                    s->shade = 1;

                if(s->shade)
                {
                    sc->ceilingz += SP<<4;
                    if(sc->ceilingz > sc->floorz)
                        sc->ceilingz = sc->floorz;
                }
                else
                {
                    sc->ceilingz   -= SP<<4;
                    if(sc->ceilingz < t[3])
                        sc->ceilingz = t[3];
                }

                break;

            case 26:

                s->xvel = 32;
                l = (s->xvel*sintable[(s->ang+512)&2047])>>14;
                x = (s->xvel*sintable[s->ang&2047])>>14;

                s->shade++;
                if( s->shade > 7 )
                {
                    s->x = t[3];
                    s->y = t[4];
                    sc->floorz -= ((s->zvel*s->shade)-s->zvel);
                    s->shade = 0;
                }
                else
                    sc->floorz += s->zvel;

                j = headspritesect[s->sectnum];
                while( j >= 0 )
                {
                    nextj = nextspritesect[j];
                    if(sprite[j].statnum != 3 && sprite[j].statnum != 10)
                    {
                        hittype[j].bposx = sprite[j].x;
                        hittype[j].bposy = sprite[j].y;

                        sprite[j].x += l;
                        sprite[j].y += x;

                        sprite[j].z += s->zvel;
                        setsprite(j,sprite[j].x,sprite[j].y,sprite[j].z);
                    }
                    j = nextj;
                }

                for(p = 0;p < ud.multimode;p++)
                    if(sprite[ps[p].i].sectnum == s->sectnum && ps[p].on_ground)
                    {
                        ps[p].posz += s->zvel;
                        ps[p].fricxv += l<<5;
                        ps[p].fricyv += x<<5;
                    }

                ms(i);
                setsprite(i,s->x,s->y,s->z);

                break;


            case 27:

                if(ud.recstat == 0) break;

                hittype[i].tempang = s->ang;

                p = findplayer(s,&x);
                if( sprite[ps[p].i].extra > 0 && myconnectindex == screenpeek)
                {
                    if( t[0] < 0 )
                    {
                        ud.camerasprite = i;
                        t[0]++;
                    }
                    else if(ud.recstat == 2 && ps[p].newowner == -1)
                    {
                        if(cansee(s->x,s->y,s->z,SECT,ps[p].posx,ps[p].posy,ps[p].posz,ps[p].cursectnum))
                        {
                            if(x < (unsigned)sh)
                            {
                                ud.camerasprite = i;
                                t[0] = 999;
                                s->ang += getincangle(s->ang,getangle(ps[p].posx-s->x,ps[p].posy-s->y))>>3;
                                SP = 100+((s->z-ps[p].posz)/257);

                            }
                            else if(t[0] == 999)
                            {
                                if(ud.camerasprite == i)
                                    t[0] = 0;
                                else t[0] = -10;
                                ud.camerasprite = i;

                            }
                        }
                        else
                        {
                            s->ang = getangle(ps[p].posx-s->x,ps[p].posy-s->y);

                            if(t[0] == 999)
                            {
                                if(ud.camerasprite == i)
                                    t[0] = 0;
                                else t[0] = -20;
                                ud.camerasprite = i;
                            }
                        }
                    }
                }
                break;
            case 28:
                if(t[5] > 0)
                {
                    t[5]--;
                    break;
                }

                if(T1 == 0)
                {
                    p = findplayer(s,&x);
                    if( x > 15500 )
                        break;
                    T1 = 1;
                    T2 = 64 + (TRAND&511);
                    T3 = 0;
                }
                else
                {
                    T3++;
                    if(T3 > T2)
                    {
                        T1 = 0;
                        ps[screenpeek].visibility = 512;
                        break;
                    }
                    else if( T3 == (T2>>1) )
                        spritesound(THUNDER,i);
                    else if(T3 == (T2>>3) )
                        spritesound(LIGHTNING_SLAP,i);
                    else if( T3 == (T2>>2) )
                    {
                        j = headspritestat[0];
                        while(j >= 0)
                        {
                            if( sprite[j].picnum == NATURALLIGHTNING && sprite[j].hitag == s->hitag)
                                sprite[j].cstat |= 32768;
                            j = nextspritestat[j];
                        }
                    }
                    else if(T3 > (T2>>3) && T3 < (T2>>2) )
                    {
                        if( cansee(s->x,s->y,s->z,s->sectnum,ps[screenpeek].posx,ps[screenpeek].posy,ps[screenpeek].posz,ps[screenpeek].cursectnum ) )
                            j = 1;
                        else j = 0;

                        if( rnd(192) && (T3&1) )
                        {
                            if(j)
                                ps[screenpeek].visibility = 0;
                        }
                        else if(j)
                            ps[screenpeek].visibility = 512;

                        j = headspritestat[0];
                        while(j >= 0)
                        {
                            if( sprite[j].picnum == NATURALLIGHTNING && sprite[j].hitag == s->hitag)
                            {
                                if ( rnd(32) && (T3&1) )
                                {
                                    sprite[j].cstat &= 32767;
                                    spawn(j,SMALLSMOKE);

                                    p = findplayer(s,&x);
                                    x = ldist(&sprite[ps[p].i], &sprite[j]);
                                    if( x < 768 )
                                    {
                                        if(Sound[DUKE_LONGTERM_PAIN].num < 1)
                                            spritesound(DUKE_LONGTERM_PAIN,ps[p].i);
                                        spritesound(SHORT_CIRCUIT,ps[p].i);
                                        sprite[ps[p].i].extra -= 8+(TRAND&7);
                                        ps[p].pals_time = 32;
                                        ps[p].pals[0] = 16;
                                        ps[p].pals[1] = 0;
                                        ps[p].pals[2] = 0;
                                    }
                                    break;
                                }
                                else sprite[j].cstat |= 32768;
                            }

                            j = nextspritestat[j];
                        }
                    }
                }
                break;
            case 29:
                s->hitag += 64;
                l = mulscale12((long)s->yvel,sintable[s->hitag&2047]);
                sc->floorz = s->z + l;
                break;
            case 31: // True Drop Floor
                if(t[0] == 1)
                {
                    // Choose dir

                    if(t[3] > 0)
                    {
                        t[3]--;
                        break;
                    }

                    if(t[2] == 1) // Retract
                    {
                        if(SA != 1536)
                        {
                            if( klabs2( sc->floorz - s->z ) < SP )
                            {
                                sc->floorz = s->z;
                                t[2] = 0;
                                t[0] = 0;
                                t[3] = s->hitag;
                                callsound(s->sectnum,i);
                            }
                            else
                            {
                                l = ksgn(s->z-sc->floorz)*SP;
                                sc->floorz += l;

                                j = headspritesect[s->sectnum];
                                while(j >= 0)
                                {
                                    if(sprite[j].picnum == APLAYER && sprite[j].owner >= 0)
                                        if( ps[sprite[j].yvel].on_ground == 1 )
                                            ps[sprite[j].yvel].posz += l;
                                    if( sprite[j].zvel == 0 && sprite[j].statnum != 3 && sprite[j].statnum != 4)
                                    {
                                        hittype[j].bposz = sprite[j].z += l;
                                        hittype[j].floorz = sc->floorz;
                                    }
                                    j = nextspritesect[j];
                                }
                            }
                        }
                        else
                        {
                            if( klabs2( sc->floorz - t[1] ) < SP )
                            {
                                sc->floorz = t[1];
                                callsound(s->sectnum,i);
                                t[2] = 0;
                                t[0] = 0;
                                t[3] = s->hitag;
                            }
                            else
                            {
                                l = ksgn(t[1]-sc->floorz)*SP;
                                sc->floorz += l;

                                j = headspritesect[s->sectnum];
                                while(j >= 0)
                                {
                                    if(sprite[j].picnum == APLAYER && sprite[j].owner >= 0)
                                        if( ps[sprite[j].yvel].on_ground == 1 )
                                            ps[sprite[j].yvel].posz += l;
                                    if( sprite[j].zvel == 0 && sprite[j].statnum != 3 && sprite[j].statnum != 4 )
                                    {
                                        hittype[j].bposz = sprite[j].z += l;
                                        hittype[j].floorz = sc->floorz;
                                    }
                                    j = nextspritesect[j];
                                }
                            }
                        }
                        break;
                    }

                    if( (s->ang&2047) == 1536)
                    {
                        if( klabs2( s->z-sc->floorz ) < SP )
                        {
                            callsound(s->sectnum,i);
                            t[0] = 0;
                            t[2] = 1;
                            t[3] = s->hitag;
                        }
                        else
                        {
                            l = ksgn(s->z-sc->floorz)*SP;
                            sc->floorz += l;

                            j = headspritesect[s->sectnum];
                            while(j >= 0)
                            {
                                if(sprite[j].picnum == APLAYER && sprite[j].owner >= 0)
                                    if( ps[sprite[j].yvel].on_ground == 1 )
                                        ps[sprite[j].yvel].posz += l;
                                if( sprite[j].zvel == 0 && sprite[j].statnum != 3 && sprite[j].statnum != 4 )
                                {
                                    hittype[j].bposz = sprite[j].z += l;
                                    hittype[j].floorz = sc->floorz;
                                }
                                j = nextspritesect[j];
                            }
                        }
                    }
                    else
                    {
                        if( klabs2( sc->floorz-t[1] ) < SP )
                        {
                            t[0] = 0;
                            callsound(s->sectnum,i);
                            t[2] = 1;
                            t[3] = s->hitag;
                        }
                        else
                        {
                            l = ksgn(s->z-t[1])*SP;
                            sc->floorz -= l;

                            j = headspritesect[s->sectnum];
                            while(j >= 0)
                            {
                                if(sprite[j].picnum == APLAYER && sprite[j].owner >= 0)
                                    if( ps[sprite[j].yvel].on_ground == 1 )
                                        ps[sprite[j].yvel].posz -= l;
                                if(sprite[j].zvel == 0 && sprite[j].statnum != 3 && sprite[j].statnum != 4 )
                                {
                                    hittype[j].bposz = sprite[j].z -= l;
                                    hittype[j].floorz = sc->floorz;
                                }
                                j = nextspritesect[j];
                            }
                        }
                    }
                }
                break;

           case 32: // True Drop Ceiling
                if(t[0] == 1)
                {
                    // Choose dir

                    if(t[2] == 1) // Retract
                    {
                        if(SA != 1536)
                        {
                            if( klabs2( sc->ceilingz - s->z ) <
                                (SP<<1) )
                            {
                                sc->ceilingz = s->z;
                                callsound(s->sectnum,i);
                                t[2] = 0;
                                t[0] = 0;
                            }
                            else sc->ceilingz +=
                                ksgn(s->z-sc->ceilingz)*SP;
                        }
                        else
                        {
                            if( klabs2( sc->ceilingz - t[1] ) <
                                (SP<<1) )
                            {
                                sc->ceilingz = t[1];
                                callsound(s->sectnum,i);
                                t[2] = 0;
                                t[0] = 0;
                            }
                            else sc->ceilingz +=
                                ksgn(t[1]-sc->ceilingz)*SP;
                        }
                        break;
                    }

                    if( (s->ang&2047) == 1536)
                    {
                        if( klabs2(sc->ceilingz-s->z ) <
                            (SP<<1) )
                        {
                            t[0] = 0;
                            t[2] = !t[2];
                            callsound(s->sectnum,i);
                            sc->ceilingz = s->z;
                        }
                        else sc->ceilingz +=
                            ksgn(s->z-sc->ceilingz)*SP;
                    }
                    else
                    {
                        if( klabs2(sc->ceilingz-t[1] ) < (SP<<1) )
                        {
                            t[0] = 0;
                            t[2] = !t[2];
                            callsound(s->sectnum,i);
                        }
                        else sc->ceilingz -= ksgn(s->z-t[1])*SP;
                    }
                }
                break;

            case 33:
                if( earthquaketime > 0 && (TRAND&7) == 0 )
                    RANDOMSCRAP;
                break;
            case 36:

                if( t[0] )
                {
                    if( t[0] == 1 )
                        shoot(i,sc->extra);
                    else if( t[0] == 26*5 )
                        t[0] = 0;
                    t[0]++;
                }
                break;

            case 128: //SE to control glass breakage

                wal = &wall[t[2]];

                if(wal->cstat|32)
                {
                    wal->cstat &= (255-32);
                    wal->cstat |= 16;
                    if(wal->nextwall >= 0)
                    {
                        wall[wal->nextwall].cstat &= (255-32);
                        wall[wal->nextwall].cstat |= 16;
                    }
                }
                else break;

                wal->overpicnum++;
                if(wal->nextwall >= 0)
                    wall[wal->nextwall].overpicnum++;

                if(t[0] < t[1]) t[0]++;
                else
                {
                    wal->cstat &= (128+32+8+4+2);
                    if(wal->nextwall >= 0)
                        wall[wal->nextwall].cstat &= (128+32+8+4+2);
                    KILLIT(i);
                }
                break;

            case 130:
                if(t[0] > 80) { KILLIT(i); }
                else t[0]++;

                x = sc->floorz-sc->ceilingz;

                if( rnd(64) )
                {
                    k = spawn(i,EXPLOSION2);
                    sprite[k].xrepeat = sprite[k].yrepeat = 2+(TRAND&7);
                    sprite[k].z = sc->floorz-(TRAND%x);
                    sprite[k].ang += 256-(TRAND%511);
                    sprite[k].xvel = TRAND&127;
                    ssp(k,CLIPMASK0);
                }
                break;
            case 131:
                if(t[0] > 40) { KILLIT(i); }
                else t[0]++;

                x = sc->floorz-sc->ceilingz;

                if( rnd(32) )
                {
                    k = spawn(i,EXPLOSION2);
                    sprite[k].xrepeat = sprite[k].yrepeat = 2+(TRAND&3);
                    sprite[k].z = sc->floorz-(TRAND%x);
                    sprite[k].ang += 256-(TRAND%511);
                    sprite[k].xvel = TRAND&127;
                    ssp(k,CLIPMASK0);
                }
                break;
            case 77:
                if (ud.multimode > 1 && ud.coop == 0 && dukematch_mode != 1)
                    break;
                
                for (p = 0; p < ud.multimode; p++)
                    if (ps[p].cursectnum == s->sectnum && klabs(ps[p].posz - s->z) <= (24<<8)
                        && ps[p].timebeforeexit == 0)
                    {
                        ps[p].timebeforeexit = 17;
                    }
                break;
        }
        BOLT:
        i = nexti;
    }

         //Sloped sin-wave floors!
     for(i=headspritestat[3];i>=0;i=nextspritestat[i])
     {
          s = &sprite[i];
          if (s->lotag != 29) continue;
          sc = &sector[s->sectnum];
          if (sc->wallnum != 4) continue;
          wal = &wall[sc->wallptr+2];
          alignflorslope(s->sectnum,wal->x,wal->y,sector[wal->nextsector].floorz);
     }
}

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

#include "pragmas.h"
#include "build.h"
#include "names.h"
#include "soundefs.h"

#ifndef NULL
#define NULL 0
#endif

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

typedef int int32;

#define VOLUMEALL
#define PLUTOPAK

// #define VOLUMEONE
// #define ONELEVELDEMO

// #define TEN
// #define BETA

// #define AUSTRALIA
// #define UK


#define MAXSLEEPDIST  16384
#define SLEEPTIME 24*64

#ifdef VOLUMEONE
    #define BYTEVERSION 28
#else
    #define BYTEVERSION 117
#endif

#define NUMPAGES 1

#define AUTO_AIM_ANGLE          48
#define RECSYNCBUFSIZ 2520   //2520 is the (LCM of 1-8)*3
#define MOVEFIFOSIZ 256

#define FOURSLEIGHT (1<<8)


#define TICRATE (120)
#define TICSPERFRAME (TICRATE/26)

// #define GC (TICSPERFRAME*44)

#define NUM_SOUNDS 300

#define    RANDOMSCRAP EGS(s->sectnum,s->x+(TRAND&255)-128,s->y+(TRAND&255)-128,s->z-(8<<8)-(TRAND&8191),SCRAP6+(TRAND&15),-8,48,48,TRAND&2047,(TRAND&63)+64,-512-(TRAND&2047),i,5)

#define    BLACK 0
#define    DARKBLUE 1
#define    DARKGREEN 2
#define    DARKCYAN 3
#define    DARKRED 4
#define    DARKPURPLE 5
#define    BROWN 6
#define    LIGHTGRAY 7

#define    DARKGRAY 8
#define    BLUE 9
#define    GREEN 10
#define    CYAN 11
#define    RED 12
#define    PURPLE 13
#define    YELLOW 14
#define    WHITE 15

#define    PHEIGHT (38<<8)

// #define P(X) printf("%ld\n",X);

#define WAIT(X) ototalclock=totalclock+(X);while(totalclock<ototalclock)


#define MODE_MENU       1
#define MODE_DEMO       2
#define MODE_GAME       4
#define MODE_EOL        8
#define MODE_TYPE       16
#define MODE_RESTART    32
#define MODE_SENDTOWHOM 64
#define MODE_END        128


#define MAXANIMWALLS 512
#define MAXINTERPOLATIONS 1024
#define NUMOFFIRSTTIMEACTIVE 131

#define MAXCYCLERS 256
#define MAXSCRIPTSIZE 13473
#define MAXANIMATES 64

#define SP  sprite[i].yvel
#define SX  sprite[i].x
#define SY  sprite[i].y
#define SZ  sprite[i].z
#define SS  sprite[i].shade
#define PN  sprite[i].picnum
#define SA  sprite[i].ang
#define SV  sprite[i].xvel
#define ZV  sprite[i].zvel
#define RX  sprite[i].xrepeat
#define RY  sprite[i].yrepeat
#define OW  sprite[i].owner
#define CS  sprite[i].cstat
#define SH  sprite[i].extra
#define CX  sprite[i].xoffset
#define CY  sprite[i].yoffset
#define CD  sprite[i].clipdist
#define PL  sprite[i].pal
#define SLT  sprite[i].lotag
#define SHT  sprite[i].hitag
#define SECT sprite[i].sectnum

#define face_player 1
#define geth 2
#define getv 4
#define random_angle 8
#define face_player_slow 16
#define spin 32
#define face_player_smart 64
#define fleeenemy 128
#define jumptoplayer 257
#define seekplayer 512
#define furthestdir 1024
#define dodgebullet 4096

#define TRAND krand2()


#define MAX_WEAPONS  15

#define KNEE_WEAPON          0
#define PISTOL_WEAPON        1
#define SHOTGUN_WEAPON       2
#define CHAINGUN_WEAPON      3
#define DEVISTATOR_WEAPON    4
#define HANDBOMB_WEAPON      5
#define SHRINKER_WEAPON      6
#define GROW_WEAPON          7
#define RPG_WEAPON           8
#define TRIPBOMB_WEAPON      9
#define FREEZE_WEAPON        10
#define HANDREMOTE_WEAPON    11
#define PISTOL2_WEAPON       12
#define SHOTGUN2_WEAPON      13
#define RPG2_WEAPON          14

#define T1  hittype[i].temp_data[0]
#define T2  hittype[i].temp_data[1]
#define T3  hittype[i].temp_data[2]
#define T4  hittype[i].temp_data[3]
#define T5  hittype[i].temp_data[4]
#define T6  hittype[i].temp_data[5]

#define IFWITHIN(B,E) if((PN)>=(B) && (PN)<=(E))
#define KILLIT(KX) {deletesprite(KX);goto BOLT;}


#define IFMOVING if(ssp(i,CLIPMASK0))
#define IFHIT j=ifhitbyweapon(i);if(j >= 0)
#define IFHITSECT j=ifhitsectors(s->sectnum);if(j >= 0)

#define AFLAMABLE(X) (X==BOX||X==TREE1||X==TREE2||X==TIRE||X==CONE)


#define IFSKILL1 if(player_skill<1)
#define IFSKILL2 if(player_skill<2)
#define IFSKILL3 if(player_skill<3)
#define IFSKILL4 if(player_skill<4)

#define rnd(X) ((TRAND>>8)>=(255-(X)))

typedef struct
{
    short i;
    int voice;
    int f_8;
} SOUNDOWNER;

typedef struct
{
    signed char avel, horz;
    short fvel, svel;
    unsigned long bits;
} input;

extern input sync[MAXPLAYERS];

typedef struct
{
    char *ptr;
    volatile char lock;
    int  length, num;
} SAMPLE;

static struct animwalltype
{
        short wallnum;
        long tag;
};
extern struct animwalltype animwall[MAXANIMWALLS];
extern short numanimwalls,probey,lastprobey;

extern long msx[2048],msy[2048];
extern short cyclers[MAXCYCLERS][6],numcyclers;

struct user_defs
{
    char god,warp_on,cashman,eog,showallmap;
    char show_help,scrollmode,clipping;
    char savegame[10][22];
    char overhead_on,last_overhead,showweapons;

    short pause_on,from_bonus;
    short camerasprite,last_camsprite;
    short last_level,secretlevel;

    long const_visibility,uw_framerate;
    long camera_time,folfvel,folavel,folx,foly,fola;
    long reccnt;

    int32 entered_name,screen_tilting,shadows,fta_on,executions,auto_run;
    int32 coords,tickrate,m_coop,coop,screen_size,lockout;

    int32 respawn_monsters,respawn_items,respawn_inventory,recstat,monsters_off,brightness;
    int32 m_respawn_items,m_respawn_monsters,m_respawn_inventory,m_recstat,m_monsters_off,detail;
    int32 m_ffire,ffire,m_player_skill,m_level_number,m_volume_number,multimode;
    int32 player_skill,level_number,volume_number,m_marker,marker,mouseflip;

};

struct player_orig
{
    long ox,oy,oz;
    short oa,os;
};


extern char numplayersprites;

void add_ammo( short, short, short, short );


extern long fricxv,fricyv;

struct player_struct
{
    long zoom,exitx,exity,loogiex[64],loogiey[64],numloogs,loogcnt;
    long posx, posy, posz, horiz, ohoriz, ohorizoff, invdisptime;
    long bobposx,bobposy,oposx,oposy,oposz,pyoff,opyoff;
    long posxv,posyv,poszv,last_pissed_time,truefz,truecz;
    long player_par,visibility;
    long bobcounter,weapon_sway;
    long pals_time,randomflamex,crack_time;

    char aim_mode;

    short ang,oang,angvel,cursectnum,look_ang,last_extra;
    short ammo_amount[MAX_WEAPONS],wackedbyactor,frag,fraggedself;

    short curr_weapon, last_weapon, tipincs, horizoff, wantweaponfire;
    short holoduke_amount,newowner,hurt_delay,hbomb_hold_delay;
    short jumping_counter,airleft,knee_incs,access_incs;
    short fta,ftq,access_wallnum,access_spritenum;
    short kickback_pic,got_access,weapon_ang,firstaid_amount;
    short somethingonplayer,on_crane,i;
    short over_shoulder_on,random_club_frame,fist_incs;
    short extra_extra8,quick_kick;
    short heat_amount,actorsqu,timebeforeexit,customexitsound;

    short weaprecs[16],weapreccnt,interface_toggle_flag;

    short rotscrnang,dead_flag;
    short scuba_amount,jetpack_amount,steroids_amount,shield_amount;
    short holoduke_on,pycount,weapon_pos,frag_ps;
    short transporter_hold,last_full_weapon,footprintshade,boot_amount;

    int scream_voice;

    char gm,on_warping_sector,footprintcount;
    char hbomb_on,jumping_toggle,rapid_fire_hold,on_ground;
    char inven_icon,buttonpalette;

    char jetpack_on,spritebridge,lastrandomspot;
    char scuba_on,footprintpal,heat_on;

    char  holster_weapon,falling_counter;
    char  gotweapon[MAX_WEAPONS],refresh_inventory,*palette;

    char toggle_key_flag,knuckle_incs; // ,select_dir;
    char walking_snd_toggle, palookup, hard_landing;
    char max_secret_rooms,secret_rooms,/*fire_flag,*/pals[3];
    char max_actors_killed,actors_killed,return_to_center;

    char f_36d;
    char f_36e;
    short f_370, f_372, f_374, f_376, f_378;

    long fricxv, fricyv;
    char crosshair, autoaim, left_right_handed;
    long f_388;
};

extern unsigned char tempbuf[2048], packbuf[576];

#define impact_damage 10
#define max_player_health 100
#define max_armour_amount 0
#define respawnactortime 768
#define respawnitemtime 768
#define dukefriction 53200
#define gc 176
#define rpgblastradius 1780
#define pipebombblastradius 2500
#define shrinkerblastradius 680
#define tripbombblastradius 3880
#define morterblastradius 2500
#define bouncemineblastradius 2500
#define seenineblastradius 2048
#define camerashitable 0
#define numfreezebounces 3
#define freezerhurtowner 1
#define spriteqamount 64
#define lasermode 0

extern long max_ammo_amount[MAX_WEAPONS];

#define MOVFIFOSIZ 256

extern short spriteq[1024],spriteqloc;
extern struct player_struct ps[MAXPLAYERS];
extern struct player_orig po[20];
extern struct user_defs ud;
extern short int global_random;
extern long scaredfallz;
extern char buf[80]; //My own generic input buffer

extern char *fta_quotes[NUMOFFIRSTTIMEACTIVE];

extern SAMPLE Sound[ NUM_SOUNDS ];
extern int32 VoiceToggle,AmbienceToggle;
extern SOUNDOWNER SoundOwner[NUM_SOUNDS][4];

extern char playerreadyflag[MAXPLAYERS],playerquitflag[MAXPLAYERS];
extern char sounds[NUM_SOUNDS][14];

extern long script[MAXSCRIPTSIZE],*scriptptr,*insptr,*labelcode,labelcnt;
extern char *label,*textptr,error,warning,killit_flag;
extern short actorscrptr[MAXTILES];
extern long *parsing_actor;
extern char actortype[MAXTILES];

extern char ipath[80],opath[80];

extern short camsprite;

// extern char gotz;
extern char inspace(short sectnum);


struct weaponhit
{
    char cgg;
    short picnum,ang,extra,owner,movflag;
    short tempang,actorstayput,dispicnum;
    short timetosleep;
    long floorz,ceilingz,lastvx,lastvy,bposx,bposy,bposz;
    long temp_data[6];
};

extern struct weaponhit hittype[MAXSPRITES];

extern input loc;

extern short numplayers, myconnectindex;
extern short connecthead, connectpoint2[MAXPLAYERS];   //Player linked list variables (indeces, not connection numbers)
extern short screenpeek;

extern long tempwallptr,animatecnt;
extern long lockclock;
extern char display_mirror;

extern long ototalclock;

extern long *animateptr[MAXANIMATES], animategoal[MAXANIMATES];
extern long animatevel[MAXANIMATES];
// extern long oanimateval[MAXANIMATES];
extern short neartagsector, neartagwall, neartagsprite;
extern long neartaghitdist;
extern short animatesect[MAXANIMATES];
extern long vel,svel,angvel,horiz;

extern short mirrorwall[64], mirrorsector[64], mirrorcnt;

extern short soundps[NUM_SOUNDS],soundpe[NUM_SOUNDS],soundvo[NUM_SOUNDS];
extern char soundpr[NUM_SOUNDS],soundm[NUM_SOUNDS];

extern int32 SoundToggle;

extern long cachecount;
extern char earthquaketime;

    //DUKE3D.H - replace the end "my's" with this
extern long myx, omyx, myxvel, myy, omyy, myyvel, myz, omyz, myzvel;
extern short myhoriz, omyhoriz, myhorizoff, omyhorizoff, globalskillsound;
extern short myang, omyang, mycursectnum, myjumpingcounter;
extern char myjumpingtoggle, myonground, myhardlanding,myreturntocenter;

extern short weaponsandammosprites[15];

extern short frags[MAXPLAYERS][MAXPLAYERS];
extern long cameradist, cameraclock;

extern char pus,pub;
extern char everyothertime;

extern long numinterpolations, startofdynamicinterpolations;
extern long oldipos[MAXINTERPOLATIONS];
extern long bakipos[MAXINTERPOLATIONS];
extern long *curipos[MAXINTERPOLATIONS];

extern char dukematch_mode;
extern char death_fade;

extern float viewhorizang;

extern unsigned short buttonheld, button_mode;

typedef struct {
    int jump, crouch, fire, aim_up, aim_down, prev_weapon, next_weapon;
    int inv_left, inv_right, open, inventory, forward, backward;
    int strafe_left, strafe_right, buttonmode;
} controlmap_t;

extern volatile unsigned short control_button[4];
extern volatile short control_stickx[4];
extern volatile short control_sticky[4];
extern controlmap_t controlmap[4];
extern unsigned short demo_button;
extern short demo_stickx;
extern short demo_sticky;
extern short stickx;
extern short sticky;
extern signed char game_inactive;

extern short D_801C94D8[4];

extern char do_restart;
extern short levelnum;
extern short menu_levelnum;

#include "funct.h"

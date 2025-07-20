
//#line "sounds.c" 407
extern void sound(short num);
//#line "sounds.c" 463
extern int spritesound(unsigned short num,short i);
//#line "sounds.c" 469
extern void stopsound(short num);
//#line "sounds.c" 478
extern void stopenvsound(short num,short i);

//#line "sector.c" 9
extern short callsound(short sn,short whatsprite);
//#line "sector.c" 56
extern short check_activator_motion(short lotag);
//#line "sector.c" 161
extern short checkcursectnums(short sect);
//#line "sector.c" 169
extern long ldist(spritetype *s1,spritetype *s2);
//#line "sector.c" 177
extern long dist(spritetype *s1,spritetype *s2);
//#line "sector.c" 186
extern short findplayer(spritetype *s,long *d);
//#line "sector.c" 214
extern short findotherplayer(short p,long *d);
//#line "sector.c" 301
extern int getanimationgoal(long *animptr);
//#line "sector.c" 457
extern char activatewarpelevators(short s,short d);
//#line "sector.c" 504
extern void operatesectors(short sn,short ii);
//#line "sector.c" 1020
extern void operateactivators(short low,short snum);
//#line "sector.c" 1089
extern void operatemasterswitches(short low);
//#line "sector.c" 1104
extern void operateforcefields(short s,short low);
//#line "sector.c" 1515
extern void activatebysector(short sect,short j);

//#line "player.c" 59
extern void quickkill(struct player_struct *p);
//#line "player.c" 131
extern long hitasprite(short i,short *hitsp);
//#line "player.c" 234
extern void shoot(short i,short atwith);

//#line "gamedef.c" 122
extern short getincangle(short a,short na);
//#line "gamedef.c" 159
extern void getglobalz(short i);
//#line "gamedef.c" 203
extern void makeitfall(short i);
//#line "gamedef.c" 2699
extern void execute(short i,short p,long x);

//#line "game.c" 815
extern void check_fta_sounds(short i);
//#line "game.c" 886
extern short badguy(spritetype *s);
//#line "game.c" 1654
extern void FTA(short q, struct player_struct* p);
//#line "game.c" 1705
extern void gameexit(char *t);
//#line "game.c" 2445
extern short LocateTheLocator(short n,short sn);
//#line "game.c" 2459
extern short EGS(short whatsect,long s_x,long s_y,long s_z,short s_pn,signed char s_s,signed char s_xr,signed char s_yr,short s_a,short s_ve,long s_zv,short s_ow,signed char s_ss);
//#line "game.c" 2537
extern char wallswitchcheck(short i);
//#line "game.c" 2588
extern short spawn(short j,short pn);

//#line "actors.c" 7
extern void updateinterpolations(void);
//#line "actors.c" 15
extern void setinterpolation(long *posptr);
//#line "actors.c" 27
extern void stopinterpolation(long *posptr);
//#line "actors.c" 41
extern void dointerpolations(long smoothratio);
//#line "actors.c" 55
extern void restoreinterpolations(void);
//#line "actors.c" 63
extern long ceilingspace(short sectnum);
//#line "actors.c" 77
extern long floorspace(short sectnum);
//#line "actors.c" 91
extern void addammo(short weapon,struct player_struct *p,short amount);
//#line "actors.c" 99
extern void addweapon(struct player_struct *p,short weapon);
//#line "actors.c" 132
extern void checkavailinven(struct player_struct *p);
//#line "actors.c" 152
extern void checkavailweapon(struct player_struct *p);
//#line "actors.c" 301
extern long ifsquished(short i,short p);
//#line "actors.c" 326
extern void hitradius(short i,long r,long hp1,long hp2,long hp3,long hp4);
//#line "actors.c" 556
extern int movesprite(short spritenum,long xchange,long ychange,long zchange,unsigned long cliptype);
//#line "actors.c" 643
extern short ssp(short i,unsigned long cliptype);
//#line "actors.c" 658
extern void insertspriteq(short i);
//#line "actors.c" 666
extern void lotsofmoney(spritetype *s,short n);
//#line "actors.c" 676
extern void guts(spritetype *s,short gtype,short n,short p);
//#line "actors.c" 718
extern void setsectinterpolate(short i);
//#line "actors.c" 742
extern void clearsectinterpolate(short i);
//#line "actors.c" 760
extern void ms(short i);
//#line "actors.c" 791
extern void movefta(void );
//#line "actors.c" 882
extern short ifhitsectors(short sectnum);
//#line "actors.c" 896
extern short ifhitbyweapon(short sn);
//#line "actors.c" 970
extern void movecyclers(void );
//#line "actors.c" 1053
extern void moveplayers(void );
//#line "actors.c" 1192
extern void movefx(void );
//#line "actors.c" 1294
extern void movefallers(void );
//#line "actors.c" 1389
extern void movestandables(void );
//#line "actors.c" 2234
extern void bounce(short i);
//#line "actors.c" 2273
extern void moveweapons(void );
//#line "actors.c" 2613
extern void movetransports(void );
//#line "actors.c" 2887
extern void moveeffectors(void );
//#line "actors.c" 4840
extern void moveactors(void );
//#line "actors.c" 6005
extern void moveexplosions(void );


void ms_update(unsigned short, unsigned short, int, int);


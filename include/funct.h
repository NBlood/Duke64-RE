
//#line "sounds.c" 277
extern int xyzsound(short num,short i,long x,long y,long z);
//#line "sounds.c" 407
extern void sound(short num);
//#line "sounds.c" 463
extern int spritesound(unsigned short num,short i);
//#line "sounds.c" 469
extern void stopsound(short num);
//#line "sounds.c" 478
extern void stopenvsound(short num,short i);
//#line "sounds.c" 571
extern void testcallback(long num);

//#line "sector.c" 9
extern short callsound(short sn,short whatsprite);
//#line "sector.c" 56
extern short check_activator_motion(short lotag);
//#line "sector.c" 93
extern char isadoorwall(short dapic);
//#line "sector.c" 124
extern char isanunderoperator(short lotag);
//#line "sector.c" 139
extern char isanearoperator(short lotag);
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
//#line "sector.c" 240
extern void doanimations(void );
//#line "sector.c" 301
extern int getanimationgoal(long *animptr);
//#line "sector.c" 315
extern int setanimation(short animsect,long *animptr,long thegoal,long thevel);
//#line "sector.c" 369
extern void animatewalls(void );
//#line "sector.c" 457
extern char activatewarpelevators(short s,short d);
//#line "sector.c" 504
extern void operatesectors(short sn,short ii);
//#line "sector.c" 997
extern void operaterespawns(short low);
//#line "sector.c" 1020
extern void operateactivators(short low,short snum);
//#line "sector.c" 1089
extern void operatemasterswitches(short low);
//#line "sector.c" 1104
extern void operateforcefields(short s,short low);
//#line "sector.c" 1140
extern char checkhitswitch(short snum,long w,char switchtype);
//#line "sector.c" 1515
extern void activatebysector(short sect,short j);
//#line "sector.c" 1532
extern void checkhitwall(short spr,short dawallnum,long x,long y,long z,short atwith);
//#line "sector.c" 1746
extern void checkplayerhurt(struct player_struct* p, short j);
//#line "sector.c" 1816
extern char checkhitceiling(short sn);
//#line "sector.c" 1891
extern void checkhitsprite(short i,short sn);
//#line "sector.c" 2326
extern void allignwarpelevators(void );
//#line "sector.c" 2357
extern void cheatkeys(short snum);
//#line "sector.c" 2766
extern void checksectors(short snum);

//#line "premap.c" 58
extern void pickrandomspot(short snum);
//#line "premap.c" 80
extern void resetplayerstats(short snum);
//#line "premap.c" 187
extern void resetweapons(short snum);
//#line "premap.c" 212
extern void resetinventory(short snum);
//#line "premap.c" 232
extern void resetprestat(short snum,char g);
//#line "premap.c" 516
extern void prelevel(char g);
//#line "premap.c" 823
extern void newgame(char vn,char ln,char sk,char pi);
//#line "premap.c" 874
extern void resetpspritevars(char g);
//#line "premap.c" 1012
extern void resettimevars(void);
//#line "premap.c" 1285
extern void enterlevel(char g);

//#line "player.c" 28
extern void incur_damage(struct player_struct *p);
//#line "player.c" 59
extern void quickkill(struct player_struct *p);
//#line "player.c" 73
extern void forceplayerangle(struct player_struct *p);
//#line "player.c" 85
extern void tracers(long x1,long y1,long z1,long x2,long y2,long z2,long p);
//#line "player.c" 114
extern long hits(short i);
//#line "player.c" 131
extern long hitasprite(short i,short *hitsp);
//#line "player.c" 152
extern long hitawall(struct player_struct *p,short *hitw);
//#line "player.c" 163
extern short aim(spritetype *s,short aang);
//#line "player.c" 234
extern void shoot(short i,short atwith);
//#line "player.c" 1501
extern void getinput(short snum);
//#line "player.c" 1742
extern char doincrements(struct player_struct *p);
//#line "player.c" 1935
extern void checkweapons(struct player_struct *p);
//#line "player.c" 1951
extern void processinput(short snum);

//#line "gamedef.c" 122
extern short getincangle(short a,short na);
//#line "gamedef.c" 159
extern void getglobalz(short i);
//#line "gamedef.c" 203
extern void makeitfall(short i);
//#line "gamedef.c" 1374
extern short furthestangle(short i,short angs);
//#line "gamedef.c" 2699
extern void execute(short i,short p,long x);

//#line "game.c" 815
extern void check_fta_sounds(short i);
//#line "game.c" 886
extern short badguy(spritetype *s);
//#line "game.c" 924
extern short badguypic(short pn);
//#line "game.c" 869
extern short inventory(spritetype *s);
//#line "game.c" 976
extern void myospal(long x,long y,short tilenum,signed char shade,char orientation,char p);
//#line "game.c" 1607
extern void operatefta(void);
//#line "game.c" 1654
extern void FTA(short q,struct player_struct *p);
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
//#line "game.c" 4181
extern void animatesprites(long x,long y,short a,long smoothratio,int spr,spritetype *t);
//#line "game.c" 7247
extern char domovethings(void );
//#line "game.c" 7882
extern void lotsofglass(short i,short wallnum,short n);
//#line "game.c" 7924
extern void spriteglass(short i, short n);
//#line "game.c" 7937
extern void ceilingglass(short i,short sectnum,short n);
//#line "game.c" 7966
extern void lotsofcolourglass(short i,short wallnum,short n);

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
void lotsofmail(spritetype *s, short n);
void lotsofpaper(spritetype *s, short n);
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


float sinf(float);
float cosf(float);
float getanglef(float dy, float dx);
void controllersetvib(char, unsigned short, char);
void demoinput(void);
void FX_StopSound(int);
void processweapon(short, unsigned long);
void stopallsounds(void);
void loadgame(void);
void func_8005B5F0(int);
void func_80059398(int, signed char);
void resetsbarscroll(void);
void clearbufbyte(unsigned int, unsigned int, unsigned int);

void gametext(short, short, char *);
void setrotatespriteflags(short, short, short, short);
void computerinput(long snum, input *syn);
void fastmemcpy(void*, void*, unsigned long);
int func_80099488(char* s);
void ms_add(unsigned short, int, int);
void func_8005B728();
void rotatesprite(float, float, float, float, int, int);
void rotatespritesc(float, float, float, float, int, int);
void rendertileworld(float, float, float, float, int, int);
void setfxcolor(char, char, char, char, char, char);
void unsetfxsprite(void);

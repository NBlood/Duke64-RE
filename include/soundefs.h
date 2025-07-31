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


// These tile positions are reserved!
#define RESERVEDSLOT1 6132
#define RESERVEDSLOT2 6133
#define RESERVEDSLOT3 6134
#define RESERVEDSLOT4 6135
#define RESERVEDSLOT5 6136
#define RESERVEDSLOT6 6137
#define RESERVEDSLOT7 6138
#define RESERVEDSLOT8 6139
#define RESERVEDSLOT9 6140
#define RESERVEDSLOT10 6141
#define RESERVEDSLOT11 6142
#define RESERVEDSLOT12 6143

// Defines weapon, not to be used with the 'shoot' keyword.

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

// Defines the motion characteristics of an actor
#define faceplayer 1
#define geth 2
#define getv 4
#define randomangle 8
#define faceplayerslow 16
#define spin 32
#define faceplayersmart 64
#define fleeenemy 128
#define jumptoplayer 257
#define seekplayer 512
#define furthestdir 1024
#define dodgebullet 4096

// Some misc #defines
#define NO       0
#define YES      1

// Defines for 'useractor' keyword
#define notenemy       0
#define enemy          1
#define enemystayput   2

// Player Actions.
#define pstanding 1
#define pwalking 2
#define prunning 4
#define pducking 8
#define pfalling 16
#define pjumping 32
#define phigher 64
#define pwalkingback 128
#define prunningback 256
#define pkicking 512
#define pshrunk 1024
#define pjetpack 2048
#define ponsteroids 4096
#define ponground 8192
#define palive 16384
#define pdead 32768
#define pfacing 65536


#define GET_STEROIDS     0
#define GET_SHIELD       1
#define GET_SCUBA        2
#define GET_HOLODUKE     3
#define GET_JETPACK      4
#define GET_ACCESS       6
#define GET_HEATS        7
#define GET_FIRSTAID     9
#define GET_BOOTS       10


#define KICK_HIT                         0
//#define PISTOL_RICOCHET                  1
#define PISTOL_BODYHIT                   1
#define PISTOL_FIRE                      2
#define EJECT_CLIP                       3
#define INSERT_CLIP                      4
#define CHAINGUN_FIRE                    5
#define RPG_SHOOT                        6
#define POOLBALLHIT                      7
#define RPG_EXPLODE                      8
//#define CAT_FIRE                        10
#define SHRINKER_FIRE                    9
#define ACTOR_SHRINKING                 10
#define PIPEBOMB_BOUNCE                 11
#define PIPEBOMB_EXPLODE                12
#define LASERTRIP_ONWALL                13
#define LASERTRIP_ARMING                14
#define LASERTRIP_EXPLODE               15
#define VENT_BUST                       16
#define GLASS_BREAKING                  17
#define GLASS_HEAVYBREAK                18
#define SHORT_CIRCUIT                   19
#define ITEM_SPLASH                     20
// #define DUKE_BREATHING                  23
// #define DUKE_EXHALING                   24
#define DUKE_GASP                       21
#define SLIM_RECOG                      22
// #define ENDSEQVOL3SND1                  27
#define DUKE_URINATE                    23
#define ENDSEQVOL3SND2                  24
#define ENDSEQVOL3SND3                  25
// #define DUKE_PASSWIND                   32
#define DUKE_CRACK                      26
#define SLIM_ATTACK                     27
#define SOMETHINGHITFORCE               28
#define DUKE_DRINKING                   29
#define DUKE_KILLED1                    30
#define DUKE_GRUNT                      31
#define DUKE_HARTBEAT                   32
#define DUKE_ONWATER                    33
#define DUKE_DEAD                       34
#define DUKE_LAND                       35
#define DUKE_WALKINDUCTS                36
//#define DUKE_GLAD                       44
//#define DUKE_YES                        45
//#define DUKE_HEHE                       46
//#define DUKE_SHUCKS                     47
#define DUKE_UNDERWATER                 37
#define DUKE_JETPACK_ON                 38
#define DUKE_JETPACK_IDLE               39
#define DUKE_JETPACK_OFF                40
// #define LIZTROOP_GROWL                  52
// #define LIZTROOP_TALK1                  53
// #define LIZTROOP_TALK2                  54
// #define LIZTROOP_TALK3                  55
// #define DUKETALKTOBOSS                  56
// #define LIZCAPT_GROWL                   57
// #define LIZCAPT_TALK1                   58
// #define LIZCAPT_TALK2                   59
// #define LIZCAPT_TALK3                   60
#define LIZARD_BEG                      41
// #define LIZARD_PAIN                     62
// #define LIZARD_DEATH                    63
#define LIZARD_SPIT                     42
// #define DRONE1_HISSRATTLE               65
// #define DRONE1_HISSSCREECH              66
// #define DUKE_TIP2                       67
#define FLESH_BURNING                   43
#define SQUISHED                        44
#define TELEPORTER                      45
#define ELEVATOR_ON                     46
#define DUKE_KILLED3                    47
#define ELEVATOR_OFF                    48
#define DOOR_OPERATE1                   49
#define SUBWAY                          50
#define SWITCH_ON                       51
// #define FAN                             77
#define DUKE_GETWEAPON3                 52
#define FLUSH_TOILET                    53
//#define HOVER_CRAFT                     80
#define EARTHQUAKE                      54
#define INTRUDER_ALERT                  55
#define END_OF_LEVEL_WARN               56
#define ENGINE_OPERATING                57
#define REACTOR_ON                      58
#define COMPUTER_AMBIENCE               59
#define GEARS_GRINDING                  60
#define BUBBLE_AMBIENCE                 61
#define MACHINE_AMBIENCE                62
#define SEWER_AMBIENCE                  63
#define WIND_AMBIENCE                   64
#define SOMETHING_DRIPPING              65
#define STEAM_HISSING                   66
// #define THEATER_BREATH                  94
// #define BAR_MUSIC                       95
#define BOS1_ROAM                       67
#define BOS1_RECOG                      68
#define BOS1_ATTACK1                    69
#define BOS1_PAIN                       70
#define BOS1_DYING                      71
#define BOS2_ROAM                       72
#define BOS2_RECOG                      73
#define BOS2_ATTACK                     74
#define BOS2_PAIN                       75
#define BOS2_DYING                      76
#define GETATOMICHEALTH                 77
#define DUKE_GETWEAPON2                 78
#define BOS3_DYING                      79
#define SHOTGUN_FIRE                    80
#define PRED_ROAM                       81
#define PRED_RECOG                      82
#define PRED_ATTACK                     83
#define PRED_PAIN                       84
#define PRED_DYING                      85
#define CAPT_ROAM                       86
#define CAPT_ATTACK                     87
#define CAPT_RECOG                      88
#define CAPT_PAIN                       89
#define CAPT_DYING                      90
#define PIG_ROAM                        91
#define PIG_RECOG                       92
#define PIG_ATTACK                      93
#define PIG_PAIN                        94
#define PIG_DYING                       95
#define RECO_ROAM                       96
#define RECO_RECOG                      97
#define RECO_ATTACK                     98
#define RECO_PAIN                       99
#define RECO_DYING                     100
// #define DRON_ROAM                      130
#define DRON_RECOG                     101
#define DRON_ATTACK1                   102
#define DRON_PAIN                      103
#define DRON_DYING                     104
#define COMM_ROAM                      105
#define COMM_RECOG                     106
#define COMM_ATTACK                    107
#define COMM_PAIN                      108
#define COMM_DYING                     109
#define OCTA_ROAM                      110
#define OCTA_RECOG                     111
#define OCTA_ATTACK1                   112
#define OCTA_PAIN                      113
#define OCTA_DYING                     114
// #define TURR_ROAM                      145
// #define TURR_RECOG                     146
// #define TURR_ATTACK                    147
#define DUMPSTER_MOVE                  115
#define SLIM_DYING                     116
#define BOS3_ROAM                      117
#define BOS3_RECOG                     118
// #define BOS3_ATTACK1                   152
#define BOS3_PAIN                      119
#define BOS1_ATTACK2                   120
#define COMM_SPIN                      121
#define BOS1_WALK                      122
#define DRON_ATTACK2                   123
#define THUD                           124
#define OCTA_ATTACK2                   125
#define WIERDSHOT_FLY                  126
// #define TURR_PAIN                      161
// #define TURR_DYING                     162
#define SLIM_ROAM                      127
// #define LADY_SCREAM                    164
#define DOOR_OPERATE2                  128
#define DOOR_OPERATE3                  129
#define DOOR_OPERATE4                  130
// #define BORNTOBEWILDSND                168
#define SHOTGUN_COCK                   131
#define GENERIC_AMBIENCE1              132
#define GENERIC_AMBIENCE2              133
#define GENERIC_AMBIENCE3              134
#define GENERIC_AMBIENCE4              135
#define GENERIC_AMBIENCE5              136
#define GENERIC_AMBIENCE6              137
// #define BOS3_ATTACK2                   176
#define GENERIC_AMBIENCE17             138
#define GENERIC_AMBIENCE18             139
#define GENERIC_AMBIENCE19             140
#define GENERIC_AMBIENCE20             141
#define GENERIC_AMBIENCE21             142
// #define GENERIC_AMBIENCE22             182
#define SECRETLEVELSND                 143
#define GENERIC_AMBIENCE8              144
#define GENERIC_AMBIENCE9              145
#define GENERIC_AMBIENCE10             146
#define GENERIC_AMBIENCE11             147
#define GENERIC_AMBIENCE12             148
#define GENERIC_AMBIENCE13             149
// #define GENERIC_AMBIENCE14             190
#define GENERIC_AMBIENCE15             150
#define GENERIC_AMBIENCE16             151
#define FIRE_CRACKLE                   152
#define BONUS_SPEECH1                  153
#define BONUS_SPEECH2                  154
#define BONUS_SPEECH3                  155
#define PIG_CAPTURE_DUKE               156
#define BONUS_SPEECH4                  157
#define DUKE_LAND_HURT                 158
#define DUKE_HIT_STRIPPER1             159
// #define DUKE_TIP1                      202
#define DUKE_KILLED2                   160
#define PRED_ROAM2                     161
#define PIG_ROAM2                      162
#define DUKE_GETWEAPON1                163
#define DUKE_SEARCH2                   164
#define DUKE_CRACK2                    165
#define DUKE_SEARCH                    166
#define DUKE_GET                       167
#define DUKE_LONGTERM_PAIN             168
#define MONITOR_ACTIVE                 169
#define NITEVISION_ONOFF               170
#define DUKE_HIT_STRIPPER2             171
#define DUKE_CRACK_FIRST               172
#define DUKE_USEMEDKIT                 173
#define DUKE_TAKEPILLS                 174
#define DUKE_PISSRELIEF                175
#define SELECT_WEAPON                  176
#define WATER_GURGLE                   177
#define DUKE_GETWEAPON4                178
#define JIBBED_ACTOR1                  179
// #define JIBBED_ACTOR2                  223
#define JIBBED_ACTOR3                  180
// #define JIBBED_ACTOR4                  225
#define JIBBED_ACTOR5                  181
#define JIBBED_ACTOR6                  182
#define JIBBED_ACTOR7                  183
#define DUKE_GOTHEALTHATLOW            184
// #define BOSSTALKTODUKE                 230
#define WAR_AMBIENCE1                  185
#define WAR_AMBIENCE2                  186
#define WAR_AMBIENCE3                  187
#define WAR_AMBIENCE4                  188
#define WAR_AMBIENCE5                  189
#define WAR_AMBIENCE6                  190
#define WAR_AMBIENCE7                  191
// #define WAR_AMBIENCE8                  238
#define WAR_AMBIENCE9                  192
#define WAR_AMBIENCE10                 193
// #define ALIEN_TALK1                    241
// #define ALIEN_TALK2                    242
#define EXITMENUSOUND                  194
#define FLY_BY                         195
#define DUKE_SCREAM                    196
#define SHRINKER_HIT                   197
#define RATTY                          198
#define INTO_MENU                      199
// #define BONUSMUSIC                     249
#define DUKE_BOOBY                     200
// #define DUKE_TALKTOBOSSFALL            251
// #define DUKE_LOOKINTOMIRROR            252
#define PIG_ROAM3                      201
// #define KILLME                         254
#define DRON_JETSND                    202
#define SPACE_DOOR1                    203
#define SPACE_DOOR2                    204
#define SPACE_DOOR3                    205
#define SPACE_DOOR4                    206
#define SPACE_DOOR5                    207
#define ALIEN_ELEVATOR1                208
#define VAULT_DOOR                     209
// #define JIBBED_ACTOR13                 263
#define DUKE_GETWEAPON6                210
// #define JIBBED_ACTOR8                  265
// #define JIBBED_ACTOR9                  266
// #define JIBBED_ACTOR10                 267
#define JIBBED_ACTOR11                 211
#define JIBBED_ACTOR12                 212
#define DUKE_KILLED4                   213
//#define DUKE_KILLED5                   271
#define ALIEN_SWITCH1                  214
//#define DUKE_STEPONFECES               273
#define DUKE_LONGTERM_PAIN2            215
#define DUKE_LONGTERM_PAIN3            216
#define DUKE_LONGTERM_PAIN4            217
#define COMPANB2                       218
// #define KTIT                           278
// #define HELICOP_IDLE                   279
#define STEPNIT                        219
#define SPACE_AMBIENCE1                220
#define SPACE_AMBIENCE2                221
#define SLIM_HATCH                     222
// #define RIPHEADNECK                    284
#define FOUNDJONES                     223
#define ALIEN_DOOR1                    224
#define ALIEN_DOOR2                    225
// #define ENDSEQVOL3SND4                 288
// #define ENDSEQVOL3SND5                 289
// #define ENDSEQVOL3SND6                 290
// #define ENDSEQVOL3SND7                 291
// #define ENDSEQVOL3SND8                 292
// #define ENDSEQVOL3SND9                 293
#define WHIPYOURASS                    226
#define ENDSEQVOL2SND1                 227
// #define ENDSEQVOL2SND2                 296
// #define ENDSEQVOL2SND3                 297
#define ENDSEQVOL2SND4                 228
// #define ENDSEQVOL2SND5                 299
// #define ENDSEQVOL2SND6                 300
// #define ENDSEQVOL2SND7                 301
#define GENERIC_AMBIENCE23             229
// #define SOMETHINGFROZE                 303
#define DUKE_LONGTERM_PAIN5            230
#define DUKE_LONGTERM_PAIN6            231
#define DUKE_LONGTERM_PAIN7            232
#define DUKE_LONGTERM_PAIN8            233
#define WIND_REPEAT                    234
// #define MYENEMY_ROAM                   309
// #define MYENEMY_HURT                   310
// #define MYENEMY_DEAD                   311
// #define MYENEMY_SHOOT                  312
// #define STORE_MUSIC                    313
// #define STORE_MUSIC_BROKE              314
#define ACTOR_GROWING                  235
#define NEWBEAST_ROAM                  236
#define NEWBEAST_RECOG                 237
#define NEWBEAST_ATTACK                238
#define NEWBEAST_PAIN                  239
#define NEWBEAST_DYING                 240
#define NEWBEAST_SPIT                  241
// #define VOL4_1                         322
// #define SUPERMARKET                    323
// #define MOUSEANNOY                     324
// #define BOOKEM                         325
// #define SUPERMARKETCRY                 326
// #define DESTRUCT                       327
// #define EATFOOD                        328
#define MAKEMYDAY                      242
// #define WITNESSSTAND                   330
// #define VACATIONSPEECH                 331
// #define YIPPEE1                        332
// #define YOHOO1                         333
// #define YOHOO2                         334
// #define DOLPHINSND                     335
// #define TOUGHGALSND1                   336
// #define TOUGHGALSND2                   337
// #define TOUGHGALSND3                   338
// #define TOUGHGALSND4                   339
// #define TANK_ROAM                      340
// #define BOS4_ROAM                      341
// #define BOS4_RECOG                     342
// #define BOS4_ATTACK                    343
// #define BOS4_PAIN                      344
// #define BOS4_DYING                     345
#define NEWBEAST_ATTACKMISS            243
#define VOL4_2                         244
#define COOKINGDEEPFRIER               245
// #define WHINING_DOG                    349
// #define DEAD_DOG                       350
#define LIGHTNING_SLAP                 246
#define THUNDER                        247
// #define HAPPYMOUSESND1                 353
// #define HAPPYMOUSESND2                 354
// #define HAPPYMOUSESND3                 355
// #define HAPPYMOUSESND4                 356
// #define ALARM                          357
#define RAIN                           248
// #define DTAG_GREENRUN                  359
// #define DTAG_BROWNRUN                  360
// #define DTAG_GREENSCORE                361
// #define DTAG_BROWNSCORE                362
// #define INTRO4_1                       363
#define INTRO4_2                       249
// #define INTRO4_3                       365
// #define INTRO4_4                       366
#define INTRO4_5                       250
// #define INTRO4_6                       368
// #define SCREECH                        369
// #define BOSS4_DEADSPEECH               370
// #define BOSS4_FIRSTSEE                 371
// #define PARTY_SPEECH                   372
// #define POSTAL_SPEECH                  373
// #define TGSPEECH                       374
// #define DOGROOMSPEECH                  375
// #define SMACKED                        376
// #define MDEVSPEECH                     377
// #define AREA51SPEECH                   378
// #define JEEPSOUND                      379
// #define BIGDOORSLAM                    380
// #define BOS4_LAY                       381
// #define WAVESOUND                      382
// #define ILLBEBACK                      383
// #define VOL4ENDSND1                    384
// #define VOL4ENDSND2                    385
#define EXPANDERHIT                    251
#define SNAKESPEECH                    252
#define EXPANDERSHOOT                  253
// #define GETBACKTOWORK                  389
// #define JIBBED_ACTOR14                 390
// #define JIBBED_ACTOR15                 391
// #define INTRO4_B                       392
// #define BIGBANG                        393
// #define SMACKIT                        394
// #define BELLSND                        395
#define SND254                         254
#define GOAWAY                         255
#define SND256                         256
#define SND257                         257
#define SND258                         258
#define SND259                         259
#define SND260                         260
#define SND261                         261
#define SND262                         262
#define SND263                         263
#define SND264                         264
#define SND265                         265
#define SND266                         266
#define SND267                         267
#define SND268                         268
#define SND269                         269
#define SND270                         270
#define SND271                         271
#define SND272                         272
#define SND273                         273
#define SND274                         274
#define SND275                         275
#define SND276                         276
// MAXIMUM NUMBER OF SOUNDS: 450 ( 0-449 )

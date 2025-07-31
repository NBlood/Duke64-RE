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

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAXSCRIPTSIZE 20460
#define MAXTILES 6144
#define NUMOFFIRSTTIMEACTIVE 192
#define NUM_SOUNDS 300

const int baseoffset = 50000000;

unsigned char tempbuf[2048];

char fta_quotes[NUMOFFIRSTTIMEACTIVE][64];
short soundps[NUM_SOUNDS],soundpe[NUM_SOUNDS],soundvo[NUM_SOUNDS];
char soundpr[NUM_SOUNDS],soundm[NUM_SOUNDS];
char sounds[NUM_SOUNDS][14];

long script[MAXSCRIPTSIZE],*scriptptr,*insptr,*labelcode,labelcnt;
char *label,*textptr,error,warning,killit_flag;
long *actorscrptr[MAXTILES],*parsing_actor;
char actortype[MAXTILES];
char *music_pointer;

static short total_lines,line_number;
static char checking_ifelse,parsing_state,*last_used_text;
static short num_squigilly_brackets;
static long last_used_size;

long ptr_to_offset(long *ptr)
{
    int offset = (char*)ptr - (char*)script;
    return baseoffset + offset;
}

#define NUMKEYWORDS     112

const char *keyw[NUMKEYWORDS] =
{
    "definelevelname",  // 0
    "actor",            // 1    [#]
    "addammo",   // 2    [#]
    "ifrnd",            // 3    [C]
    "enda",             // 4    [:]
    "ifcansee",         // 5    [C]
    "ifhitweapon",      // 6    [#]
    "action",           // 7    [#]
    "ifpdistl",         // 8    [#]
    "ifpdistg",         // 9    [#]
    "else",             // 10   [#]
    "strength",         // 11   [#]
    "break",            // 12   [#]
    "shoot",            // 13   [#]
    "palfrom",          // 14   [#]
    "sound",            // 15   [filename.voc]
    "fall",             // 16   []
    "state",            // 17
    "ends",             // 18
    "define",           // 19
    "//",               // 20
    "ifai",             // 21
    "killit",           // 22
    "addweapon",        // 23
    "ai",               // 24
    "addphealth",       // 25
    "ifdead",           // 26
    "ifsquished",       // 27
    "sizeto",           // 28
    "{",                // 29
    "}",                // 30
    "spawn",            // 31
    "move",             // 32
    "ifwasweapon",      // 33
    "ifaction",         // 34
    "ifactioncount",    // 35
    "resetactioncount", // 36
    "debris",           // 37
    "pstomp",           // 38
    "/*",               // 39
    "cstat",            // 40
    "ifmove",           // 41
    "resetplayer",      // 42
    "ifonwater",        // 43
    "ifinwater",        // 44
    "ifcanshoottarget", // 45
    "ifcount",          // 46
    "resetcount",       // 47
    "addinventory",     // 48
    "ifactornotstayput",// 49
    "hitradius",        // 50
    "ifp",              // 51
    "count",            // 52
    "ifactor",          // 53
    "music",            // 54
    "include",          // 55
    "ifstrength",       // 56
    "definesound",      // 57
    "guts",             // 58
    "ifspawnedby",      // 59
    "gamestartup",      // 60
    "wackplayer",       // 61
    "ifgapzl",          // 62
    "ifhitspace",       // 63
    "ifoutside",        // 64
    "ifmultiplayer",    // 65
    "operate",          // 66
    "ifinspace",        // 67
    "debug",            // 68
    "endofgame",        // 69
    "ifbulletnear",     // 70
    "ifrespawn",        // 71
    "iffloordistl",     // 72
    "ifceilingdistl",   // 73
    "spritepal",        // 74
    "ifpinventory",     // 75
    "betaname",         // 76
    "cactor",           // 77
    "ifphealthl",       // 78
    "definequote",      // 79
    "quote",            // 80
    "ifinouterspace",   // 81
    "ifnotmoving",      // 82
    "respawnhitag",        // 83
    "tip",             // 84
    "ifspritepal",      // 85
    "money",         // 86
    "soundonce",         // 87
    "addkills",         // 88
    "stopsound",        // 89
    "ifawayfromwall",       // 90
    "ifcanseetarget",   // 91
    "globalsound",  // 92
    "lotsofglass", // 93
    "ifgotweaponce", // 94
    "getlastpal", // 95
    "pkick",  // 96
    "mikesnd", // 97
    "useractor",  // 98
    "sizeat",  // 99
    "addstrength", // 100   [#]
    "cstator", // 101
    "mail", // 102
    "paper", // 103
    "tossweapon", // 104
    "sleeptime", // 105
    "nullop", // 106
    "definevolumename", // 107
    "defineskillname", // 108
    "ifnosounds", // 109
    "clipdist", // 110
    "ifangdiffl" // 111
};

char ispecial(char c)
{
    if(c == 0x0a)
    {
        line_number++;
        return 1;
    }

    if(c == ' ' || c == 0x0d)
        return 1;

    return 0;
}

char isaltok(char c)
{
    return ( isalnum(c) || c == '{' || c == '}' || c == '/' || c == '*' || c == '-' || c == '_' || c == '.');
}


void getlabel(void)
{
    long i;

    while( isalnum(*textptr) == 0 )
    {
        if(*textptr == 0x0a) line_number++;
        textptr++;
        if( *textptr == 0)
            return;
    }

    i = 0;
    while( ispecial(*textptr) == 0 )
        label[(labelcnt<<6)+i++] = *(textptr++);

    label[(labelcnt<<6)+i] = 0;
}

long keyword(void)
{
    long i;
    char *temptextptr;

    temptextptr = textptr;

    while( isaltok(*temptextptr) == 0 )
    {
        temptextptr++;
        if( *temptextptr == 0 )
            return 0;
    }

    i = 0;
    while( isaltok(*temptextptr) )
    {
        tempbuf[i] = *(temptextptr++);
        i++;
    }
    tempbuf[i] = 0;

    for(i=0;i<NUMKEYWORDS;i++)
        if( strcmp( (char*)tempbuf,keyw[i]) == 0 )
            return i;

    return -1;
}

long transword(void) //Returns its code #
{
    long i, l;

    while( isaltok(*textptr) == 0 )
    {
        if(*textptr == 0x0a) line_number++;
        if( *textptr == 0 )
            return -1;
        textptr++;
    }

    l = 0;
    while( isaltok(*(textptr+l)) )
    {
        tempbuf[l] = textptr[l];
        l++;
    }
    tempbuf[l] = 0;

    for(i=0;i<NUMKEYWORDS;i++)
    {
        if( strcmp( (char*)tempbuf,keyw[i]) == 0 )
        {
            *scriptptr = i;
            textptr += l;
            scriptptr++;
            return i;
        }
    }

    textptr += l;

    if( tempbuf[0] == '{' && tempbuf[1] != 0)
        printf("  * ERROR!(L%ld) Expecting a SPACE or CR between '{' and '%s'.\n",line_number,tempbuf+1);
    else if( tempbuf[0] == '}' && tempbuf[1] != 0)
        printf("  * ERROR!(L%ld) Expecting a SPACE or CR between '}' and '%s'.\n",line_number,tempbuf+1);
    else if( tempbuf[0] == '/' && tempbuf[1] == '/' && tempbuf[2] != 0 )
        printf("  * ERROR!(L%ld) Expecting a SPACE between '//' and '%s'.\n",line_number,tempbuf+2);
    else if( tempbuf[0] == '/' && tempbuf[1] == '*' && tempbuf[2] != 0 )
        printf("  * ERROR!(L%ld) Expecting a SPACE between '/*' and '%s'.\n",line_number,tempbuf+2);
    else if( tempbuf[0] == '*' && tempbuf[1] == '/' && tempbuf[2] != 0 )
        printf("  * ERROR!(L%ld) Expecting a SPACE between '*/' and '%s'.\n",line_number,tempbuf+2);
    else printf("  * ERROR!(L%ld) Expecting key word, but found '%s'.\n",line_number,tempbuf);

    error++;
    return -1;
}

void transnum(void)
{
    long i, l;

    while( isaltok(*textptr) == 0 )
    {
        if(*textptr == 0x0a) line_number++;
        textptr++;
        if( *textptr == 0 )
            return;
    }


    l = 0;
    while( isaltok(*(textptr+l)) )
    {
        tempbuf[l] = textptr[l];
        l++;
    }
    tempbuf[l] = 0;

    for(i=0;i<NUMKEYWORDS;i++)
        if( strcmp( label+(labelcnt<<6),keyw[i]) == 0 )
    {
        error++;
        printf("  * ERROR!(L%ld) Symbol '%s' is a key word.\n",line_number,label+(labelcnt<<6));
        textptr+=l;
    }


    for(i=0;i<labelcnt;i++)
    {
        if( strcmp((char*)tempbuf,label+(i<<6)) == 0 )
        {
            *scriptptr = labelcode[i];
            scriptptr++;
            textptr += l;
            return;
        }
    }

    if( isdigit(*textptr) == 0 && *textptr != '-')
    {
        printf("  * ERROR!(L%ld) Parameter '%s' is undefined.\n",line_number,tempbuf);
        error++;
        textptr+=l;
        return;
    }

    *scriptptr = atol(textptr);
    scriptptr++;

    textptr += l;
}


char parsecommand(void)
{
    long i, j, k, *tempscrptr;
    char done, *origtptr, temp_ifelse_check, tw;
    short temp_line_number;
    FILE *fp;

    if( error > 12 || ( *textptr == '\0' ) || ( *(textptr+1) == '\0' ) ) return 1;

    tw = transword();

    switch(tw)
    {
        default:
        case -1:
            return 0; //End
        case 39:      //Rem endrem
            scriptptr--;
            j = line_number;
            do
            {
                textptr++;
                if(*textptr == 0x0a) line_number++;
                if( *textptr == 0 )
                {
                    printf("  * ERROR!(L%ld) Found '/*' with no '*/'.\n",j,label+(labelcnt<<6));
                    error++;
                    return 0;
                }
            }
            while( *textptr != '*' || *(textptr+1) != '/' );
            textptr+=2;
            return 0;
        case 17:
            if( parsing_actor == 0 && parsing_state == 0 )
            {
                getlabel();
                scriptptr--;
                labelcode[labelcnt] = ptr_to_offset(scriptptr);
                labelcnt++;

                parsing_state = 1;

                return 0;
            }

            getlabel();

            for(i=0;i<NUMKEYWORDS;i++)
                if( strcmp( label+(labelcnt<<6),keyw[i]) == 0 )
                {
                    error++;
                    printf("  * ERROR!(L%ld) Symbol '%s' is a key word.\n",line_number,label+(labelcnt<<6));
                    return 0;
                }

            for(j=0;j<labelcnt;j++)
            {
                if( strcmp(label+(j<<6),label+(labelcnt<<6)) == 0 )
                {
                    *scriptptr = labelcode[j];
                    break;
                }
            }

            if(j==labelcnt)
            {
                printf("  * ERROR!(L%ld) State '%s' not found.\n",line_number,label+(labelcnt<<6));
                error++;
            }
            scriptptr++;
            return 0;

        case 15:
        case 92:
        case 87:
        case 89:
        case 93:
            transnum();
            return 0;

        case 18:
            if( parsing_state == 0 )
            {
                printf("  * ERROR!(L%ld) Found 'ends' with no 'state'.\n",line_number);
                error++;
            }
//            else
            {
                if( num_squigilly_brackets > 0 )
                {
                    printf("  * ERROR!(L%ld) Found more '{' than '}' before 'ends'.\n",line_number);
                    error++;
                }
                if( num_squigilly_brackets < 0 )
                {
                    printf("  * ERROR!(L%ld) Found more '}' than '{' before 'ends'.\n",line_number);
                    error++;
                }
                parsing_state = 0;
            }
            return 0;
        case 19:
            getlabel();
            // Check to see it's already defined

            for(i=0;i<NUMKEYWORDS;i++)
                if( strcmp( label+(labelcnt<<6),keyw[i]) == 0 )
                {
                    error++;
                    printf("  * ERROR!(L%ld) Symbol '%s' is a key word.\n",line_number,label+(labelcnt<<6));
                    return 0;
                }

            for(i=0;i<labelcnt;i++)
            {
                if( strcmp(label+(labelcnt<<6),label+(i<<6)) == 0 )
                {
                    warning++;
                    printf("  * WARNING.(L%ld) Duplicate definition '%s' ignored.\n",line_number,label+(labelcnt<<6));
                    break;
                }
            }

            transnum();
            if(i == labelcnt)
                labelcode[labelcnt++] = *(scriptptr-1);
            scriptptr -= 2;
            return 0;
        case 14:

            for(j = 0;j < 4;j++)
            {
                if( keyword() == -1 )
                    transnum();
                else break;
            }

            while(j < 4)
            {
                *scriptptr = 0;
                scriptptr++;
                j++;
            }
            return 0;

        case 32:
            if( parsing_actor || parsing_state )
            {
                transnum();

                j = 0;
                while(keyword() == -1)
                {
                    transnum();
                    scriptptr--;
                    j |= *scriptptr;
                }
                *scriptptr = j;
                scriptptr++;
            }
            else
            {
                scriptptr--;
                getlabel();
                // Check to see it's already defined

                for(i=0;i<NUMKEYWORDS;i++)
                    if( strcmp( label+(labelcnt<<6),keyw[i]) == 0 )
                {
                    error++;
                    printf("  * ERROR!(L%ld) Symbol '%s' is a key word.\n",line_number,label+(labelcnt<<6));
                    return 0;
                }

                for(i=0;i<labelcnt;i++)
                    if( strcmp(label+(labelcnt<<6),label+(i<<6)) == 0 )
                    {
                        warning++;
                        printf("  * WARNING.(L%ld) Duplicate move '%s' ignored.\n",line_number,label+(labelcnt<<6));
                        break;
                    }
                if(i == labelcnt)
                    labelcode[labelcnt++] = ptr_to_offset(scriptptr);
                for(j=0;j<2;j++)
                {
                    if(keyword() >= 0) break;
                    transnum();
                }
                for(k=j;k<2;k++)
                {
                    *scriptptr = 0;
                    scriptptr++;
                }
            }
            return 0;

        case 54:
            {
                scriptptr--;
                transnum(); // Volume Number (0/4)
                scriptptr--;

                k = *scriptptr-1;

                if(k >= 0) // if it's background music
                {
                    i = 0;
                    while(keyword() == -1)
                    {
                        while( isaltok(*textptr) == 0 )
                        {
                            if(*textptr == 0x0a) line_number++;
                            textptr++;
                            if( *textptr == 0 ) break;
                        }
                        j = 0;
                        while( isaltok(*(textptr+j)) )
                        {
                            //music_fn[k][i][j] = textptr[j];
                            j++;
                        }
                        //music_fn[k][i][j] = '\0';
                        textptr += j;
                        if(i > 9) break;
                        i++;
                    }
                }
                else
                {
                    i = 0;
                    while(keyword() == -1)
                    {
                        while( isaltok(*textptr) == 0 )
                        {
                            if(*textptr == 0x0a) line_number++;
                            textptr++;
                            if( *textptr == 0 ) break;
                        }
                        j = 0;
                        while( isaltok(*(textptr+j)) )
                        {
                            //env_music_fn[i][j] = textptr[j];
                            j++;
                        }
                        //env_music_fn[i][j] = '\0';

                        textptr += j;
                        if(i > 9) break;
                        i++;
                    }
                }
            }
            return 0;
        case 55:
            scriptptr--;
            while( isaltok(*textptr) == 0 )
            {
                if(*textptr == 0x0a) line_number++;
                textptr++;
                if( *textptr == 0 ) break;
            }
            j = 0;
            while( isaltok(*textptr) )
            {
                tempbuf[j] = *(textptr++);
                j++;
            }
            tempbuf[j] = '\0';

            fp = fopen((char*)tempbuf,"rb");
            if(fp == 0)
            {
                error++;
                printf("  * ERROR!(L%ld) Could not find '%s'.\n",line_number,label+(labelcnt<<6));
                return 0;
            }

            fseek(fp, 0, SEEK_END);
            j = ftell(fp);
            rewind(fp);

            printf("Including: '%s'.\n",tempbuf);

            temp_line_number = line_number;
            line_number = 1;
            temp_ifelse_check = checking_ifelse;
            checking_ifelse = 0;
            origtptr = textptr;
            textptr = last_used_text+last_used_size;

            *(textptr+j) = 0;

            fread((char *)textptr,1,j,fp);
            fclose(fp);

            do
                done = parsecommand();
            while( done == 0 );

            textptr = origtptr;
            total_lines += line_number;
            line_number = temp_line_number;
            checking_ifelse = temp_ifelse_check;

            return 0;
        case 24:
            if( parsing_actor || parsing_state )
                transnum();
            else
            {
                scriptptr--;
                getlabel();

                for(i=0;i<NUMKEYWORDS;i++)
                    if( strcmp( label+(labelcnt<<6),keyw[i]) == 0 )
                    {
                        error++;
                        printf("  * ERROR!(L%ld) Symbol '%s' is a key word.\n",line_number,label+(labelcnt<<6));
                        return 0;
                    }

                for(i=0;i<labelcnt;i++)
                    if( strcmp(label+(labelcnt<<6),label+(i<<6)) == 0 )
                    {
                        warning++;
                        printf("  * WARNING.(L%ld) Duplicate ai '%s' ignored.\n",line_number,label+(labelcnt<<6));
                        break;
                    }

                if(i == labelcnt)
                    labelcode[labelcnt++] = ptr_to_offset(scriptptr);

                for(j=0;j<3;j++)
                {
                    if(keyword() >= 0) break;
                    if(j == 2)
                    {
                        k = 0;
                        while(keyword() == -1)
                        {
                            transnum();
                            scriptptr--;
                            k |= *scriptptr;
                        }
                        *scriptptr = k;
                        scriptptr++;
                        return 0;
                    }
                    else transnum();
                }
                for(k=j;k<3;k++)
                {
                    *scriptptr = 0;
                    scriptptr++;
                }
            }
            return 0;

        case 7:
            if( parsing_actor || parsing_state )
                transnum();
            else
            {
                scriptptr--;
                getlabel();
                // Check to see it's already defined

                for(i=0;i<NUMKEYWORDS;i++)
                    if( strcmp( label+(labelcnt<<6),keyw[i]) == 0 )
                    {
                        error++;
                        printf("  * ERROR!(L%ld) Symbol '%s' is a key word.\n",line_number,label+(labelcnt<<6));
                        return 0;
                    }

                for(i=0;i<labelcnt;i++)
                    if( strcmp(label+(labelcnt<<6),label+(i<<6)) == 0 )
                    {
                        warning++;
                        printf("  * WARNING.(L%ld) Duplicate action '%s' ignored.\n",line_number,label+(labelcnt<<6));
                        break;
                    }

                if(i == labelcnt)
                    labelcode[labelcnt++] = ptr_to_offset(scriptptr);

                for(j=0;j<5;j++)
                {
                    if(keyword() >= 0) break;
                    transnum();
                }
                for(k=j;k<5;k++)
                {
                    *scriptptr = 0;
                    scriptptr++;
                }
            }
            return 0;

        case 1:
            if( parsing_state )
            {
                printf("  * ERROR!(L%ld) Found 'actor' within 'state'.\n",line_number);
                error++;
            }

            if( parsing_actor )
            {
                printf("  * ERROR!(L%ld) Found 'actor' within 'actor'.\n",line_number);
                error++;
            }

            num_squigilly_brackets = 0;
            scriptptr--;
            parsing_actor = scriptptr;

            transnum();
            scriptptr--;
            actorscrptr[*scriptptr] = parsing_actor;

            for(j=0;j<4;j++)
            {
                *(parsing_actor+j) = 0;
                if(j == 3)
                {
                    j = 0;
                    while(keyword() == -1)
                    {
                        transnum();
                        scriptptr--;
                        j |= *scriptptr;
                    }
                    *scriptptr = j;
                    scriptptr++;
                    break;
                }
                else
                {
                    if(keyword() >= 0)
                    {
                        scriptptr += (4-j);
                        break;
                    }
                    transnum();

                    *(parsing_actor+j) = *(scriptptr-1);
                }
            }

            checking_ifelse = 0;

            return 0;

        case 98:

            if( parsing_state )
            {
                printf("  * ERROR!(L%ld) Found 'useritem' within 'state'.\n",line_number);
                error++;
            }

            if( parsing_actor )
            {
                printf("  * ERROR!(L%ld) Found 'useritem' within 'actor'.\n",line_number);
                error++;
            }

            num_squigilly_brackets = 0;
            scriptptr--;
            parsing_actor = scriptptr;

            transnum();
            scriptptr--;
            j = *scriptptr;

            transnum();
            scriptptr--;
            actorscrptr[*scriptptr] = parsing_actor;
            actortype[*scriptptr] = j;

            for(j=0;j<4;j++)
            {
                *(parsing_actor+j) = 0;
                if(j == 3)
                {
                    j = 0;
                    while(keyword() == -1)
                    {
                        transnum();
                        scriptptr--;
                        j |= *scriptptr;
                    }
                    *scriptptr = j;
                    scriptptr++;
                    break;
                }
                else
                {
                    if(keyword() >= 0)
                    {
                        scriptptr += (4-j);
                        break;
                    }
                    transnum();

                    *(parsing_actor+j) = *(scriptptr-1);
                }
            }

            checking_ifelse = 0;

            return 0;



        case 11:
        case 13:
        case 25:
        case 31:
        case 40:
        case 52:
        case 69:
        case 74:
        case 77:
        case 80:
        case 86:
        case 88:
        case 68:
        case 100:
        case 101:
        case 102:
        case 103:
        case 105:
        case 110:
            transnum();
            return 0;

        case 2:
        case 23:
        case 28:
        case 99:
        case 37:
        case 48:
        case 58:
            transnum();
            transnum();
            break;
        case 50:
            transnum();
            transnum();
            transnum();
            transnum();
            transnum();
            break;
        case 10:
            if( checking_ifelse )
            {
                checking_ifelse--;
                tempscrptr = scriptptr;
                scriptptr++; //Leave a spot for the fail location
                parsecommand();
                *tempscrptr = ptr_to_offset(scriptptr);
            }
            else
            {
                scriptptr--;
                error++;
                printf("  * ERROR!(L%ld) Found 'else' with no 'if'.\n",line_number);
            }

            return 0;

        case 75:
            transnum();
        case 3:
        case 8:
        case 9:
        case 21:
        case 33:
        case 34:
        case 35:
        case 41:
        case 46:
        case 53:
        case 56:
        case 59:
        case 62:
        case 72:
        case 73:
//        case 74:
        case 78:
        case 85:
        case 94:
        case 111:
            transnum();
        case 43:
        case 44:
        case 49:
        case 5:
        case 6:
        case 27:
        case 26:
        case 45:
        case 51:
        case 63:
        case 64:
        case 65:
        case 67:
        case 70:
        case 71:
        case 81:
        case 82:
        case 90:
        case 91:
        case 109:

            if(tw == 51)
            {
                j = 0;
                do
                {
                    transnum();
                    scriptptr--;
                    j |= *scriptptr;
                }
                while(keyword() == -1);
                *scriptptr = j;
                scriptptr++;
            }

            tempscrptr = scriptptr;
            scriptptr++; //Leave a spot for the fail location

            do
            {
                j = keyword();
                if(j == 20 || j == 39)
                    parsecommand();
            } while(j == 20 || j == 39);

            parsecommand();

            *tempscrptr = ptr_to_offset(scriptptr);

            checking_ifelse++;
            return 0;
        case 29:
            num_squigilly_brackets++;
            do
                done = parsecommand();
            while( done == 0 );
            return 0;
        case 30:
            num_squigilly_brackets--;
            if( num_squigilly_brackets < 0 )
            {
                printf("  * ERROR!(L%ld) Found more '}' than '{'.\n",line_number);
                error++;
            }
            return 1;
        case 76:
            scriptptr--;
            j = 0;
            while( *textptr != 0x0a )
            {
                //betaname[j] = *textptr;
                j++; textptr++;
            }
            //betaname[j] = 0;
            return 0;
        case 20:
            scriptptr--; //Negate the rem
            while( *textptr != 0x0a )
                textptr++;

            // line_number++;
            return 0;

        case 107:
            scriptptr--;
            transnum();
            scriptptr--;
            j = *scriptptr;
            while( *textptr == ' ' ) textptr++;

            i = 0;

            while( *textptr != 0x0a )
            {
                //volume_names[j][i] = toupper(*textptr);
                textptr++,i++;
                if(i >= 32)
                {
                    printf("  * ERROR!(L%ld) Volume name exceeds character size limit of 32.\n",line_number);
                    error++;
                    while( *textptr != 0x0a ) textptr++;
                    break;
                }
            }
            //volume_names[j][i-1] = '\0';
            return 0;
        case 108:
            scriptptr--;
            transnum();
            scriptptr--;
            j = *scriptptr;
            while( *textptr == ' ' ) textptr++;

            i = 0;

            while( *textptr != 0x0a )
            {
                //skill_names[j][i] = toupper(*textptr);
                textptr++,i++;
                if(i >= 32)
                {
                    printf("  * ERROR!(L%ld) Skill name exceeds character size limit of 32.\n",line_number);
                    error++;
                    while( *textptr != 0x0a ) textptr++;
                    break;
                }
            }
            //skill_names[j][i-1] = '\0';
            return 0;

        case 0:
            scriptptr--;
            transnum();
            scriptptr--;
            j = *scriptptr;
            transnum();
            scriptptr--;
            k = *scriptptr;
            while( *textptr == ' ' ) textptr++;

            i = 0;
            while( *textptr != ' ' && *textptr != 0x0a )
            {
                //level_file_names[j*11+k][i] = *textptr;
                textptr++,i++;
                if(i > 127)
                {
                    printf("  * ERROR!(L%ld) Level file name exceeds character size limit of 128.\n",line_number);
                    error++;
                    while( *textptr != ' ') textptr++;
                    break;
                }
            }
            //level_names[j*11+k][i-1] = '\0';

            while( *textptr == ' ' ) textptr++;

            //partime[j*11+k] =
            //    (((*(textptr+0)-'0')*10+(*(textptr+1)-'0'))*26*60)+
            //    (((*(textptr+3)-'0')*10+(*(textptr+4)-'0'))*26);

            textptr += 5;
            while( *textptr == ' ' ) textptr++;

            //designertime[j*11+k] =
            //    (((*(textptr+0)-'0')*10+(*(textptr+1)-'0'))*26*60)+
            //    (((*(textptr+3)-'0')*10+(*(textptr+4)-'0'))*26);

            textptr += 5;
            while( *textptr == ' ' ) textptr++;

            i = 0;

            while( *textptr != 0x0a )
            {
                //level_names[j*11+k][i] = toupper(*textptr);
                textptr++,i++;
                if(i >= 32)
                {
                    printf("  * ERROR!(L%ld) Level name exceeds character size limit of 32.\n",line_number);
                    error++;
                    while( *textptr != 0x0a ) textptr++;
                    break;
                }
            }
            //level_names[j*11+k][i-1] = '\0';
            return 0;

        case 79:
            scriptptr--;
            transnum();
            k = *(scriptptr-1);
            if(k >= NUMOFFIRSTTIMEACTIVE)
            {
                printf("  * ERROR!(L%ld) Quote amount exceeds limit of %ld characters.\n",line_number,NUMOFFIRSTTIMEACTIVE);
                error++;
            }
            scriptptr--;
            i = 0;
            while( *textptr == ' ' )
                textptr++;

            while( *textptr != 0x0a )
            {
                fta_quotes[k][i] = *textptr;
                textptr++,i++;
                if(i >= 64)
                {
                    printf("  * ERROR!(L%ld) Quote exceeds character size limit of 64.\n",line_number);
                    error++;
                    while( *textptr != 0x0a ) textptr++;
                    break;
                }
            }
            fta_quotes[k][i] = '\0';
            return 0;
        case 57:
            scriptptr--;
            transnum();
            k = *(scriptptr-1);
            if(k >= NUM_SOUNDS)
            {
                printf("  * ERROR!(L%ld) Exceeded sound limit of %ld.\n",line_number,NUM_SOUNDS);
                error++;
            }
            scriptptr--;
            i = 0;
            while( *textptr == ' ')
                textptr++;

            while( *textptr != ' ' )
            {
                sounds[k][i] = *textptr;
                textptr++,i++;
                if(i >= 13)
                {
                    puts(sounds[k]);
                    printf("  * ERROR!(L%ld) Sound filename exceeded limit of 13 characters.\n",line_number);
                    error++;
                    while( *textptr != ' ' ) textptr++;
                    break;
                }
            }
            sounds[k][i] = '\0';

            transnum();
            soundps[k] = *(scriptptr-1);
            scriptptr--;
            transnum();
            soundpe[k] = *(scriptptr-1);
            scriptptr--;
            transnum();
            soundpr[k] = *(scriptptr-1);
            scriptptr--;
            transnum();
            soundm[k] = *(scriptptr-1);
            scriptptr--;
            transnum();
            soundvo[k] = *(scriptptr-1);
            scriptptr--;
            return 0;
        
        case 4:
            if( parsing_actor == 0 )
            {
                printf("  * ERROR!(L%ld) Found 'enda' without defining 'actor'.\n",line_number);
                error++;
            }
//            else
            {
                if( num_squigilly_brackets > 0 )
                {
                    printf("  * ERROR!(L%ld) Found more '{' than '}' before 'enda'.\n",line_number);
                    error++;
                }
                parsing_actor = 0;
            }

            return 0;
        case 12:
        case 16:
        case 84:
//        case 21:
        case 22:    //KILLIT
        case 36:
        case 38:
        case 42:
        case 47:
        case 61:
        case 66:
        case 83:
        case 95:
        case 96:
        case 97:
        case 104:
        case 106:
            return 0;
        case 60:
            j = 0;
            while(j < 30)
            {
                transnum();
                scriptptr--;

#if 0
                switch(j)
                {
                    case 0:
                        ud.const_visibility = *scriptptr;
                        break;
                    case 1:
                        impact_damage = *scriptptr;
                        break;
                    case 2:
                        max_player_health = *scriptptr;
                        break;
                    case 3:
                        max_armour_amount = *scriptptr;
                        break;
                    case 4:
                        respawnactortime = *scriptptr;break;
                    case 5:
                        respawnitemtime = *scriptptr;break;
                    case 6:
                        dukefriction = *scriptptr;break;
                    case 7:
                        gc = *scriptptr;break;
                    case 8:rpgblastradius = *scriptptr;break;
                    case 9:pipebombblastradius = *scriptptr;break;
                    case 10:shrinkerblastradius = *scriptptr; break;
                    case 11:tripbombblastradius = *scriptptr; break;
                    case 12:morterblastradius = *scriptptr;break;
                    case 13:bouncemineblastradius = *scriptptr;break;
                    case 14:seenineblastradius = *scriptptr;break;

                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                        if(j == 24)
                            max_ammo_amount[11] = *scriptptr;
                        else max_ammo_amount[j-14] = *scriptptr;
                        break;
                    case 25:
                        camerashitable = *scriptptr;
                        break;
                    case 26:
                        numfreezebounces = *scriptptr;
                        break;
                    case 27:
                        freezerhurtowner = *scriptptr;
                        break;
                    case 28:
                        spriteqamount = *scriptptr;
                        if(spriteqamount > 1024) spriteqamount = 1024;
                        else if(spriteqamount < 0) spriteqamount = 0;
                        break;
                    case 29:
                        lasermode = *scriptptr;
                        break;
                }
#endif
                j++;
            }
            scriptptr++;
            return 0;
    }
    return 0;
}


void passone(void)
{

    while( parsecommand() == 0 );

    if( (error+warning) > 12)
        puts(  "  * ERROR! Too many warnings or errors.");

}

void loadefs(const char *filenam,char *mptr)
{
    int i;
    long fs;
    FILE *fp;

    fp = fopen(filenam,"rb");
    if( fp == 0 )
    {
        exit(0);
        return; //Not there
    }
    else
    {
        printf("Compiling: '%s'.\n",filenam);

        fseek(fp, 0, SEEK_END);
        fs = ftell(fp);
        rewind(fp);

        last_used_text = textptr = (char *) mptr;
        last_used_size = fs;

        fread((char *)textptr,1,fs,fp);
        fclose(fp);
    }

    textptr[fs - 2] = 0;

    memset(actorscrptr,0,sizeof(actorscrptr));
    memset(actortype,0,sizeof(actortype));

    labelcnt = 0;
    scriptptr = script+1;
    warning = 0;
    error = 0;
    line_number = 1;
    total_lines = 0;

    passone(); //Tokenize
    *script = ptr_to_offset(scriptptr);

    if(warning|error)
        printf("Found %ld warning(s), %ld error(s).\n",warning,error);

    if(error)
    {
        printf("\nError in %s.",filenam);
        exit(0);
    }
    else
    {
        total_lines += line_number;
        printf("Code Size:%ld bytes(%ld labels).\n",(long)((scriptptr-script)<<2)-4,labelcnt);
    }
}

char membuf[10000000];
char membuf2[10000000];
char membuf3[10000000];


int main()
{
    labelcode = (long*)membuf2;
    label = membuf3;
    loadefs("game.con", membuf);

    int size = scriptptr - script;

#if 0
    FILE* fs = fopen("script.bin", "wb");
    if (fs)
    {
        for (int i = 0; i < size; i++)
        {
            char opcode[4];
            opcode[0] = (script[i] >> 24) & 255;
            opcode[1] = (script[i] >> 16) & 255;
            opcode[2] = (script[i] >> 8) & 255;
            opcode[3] = (script[i] >> 0) & 255;
            fwrite(opcode, 1, 4, fs);
        }
        fclose(fs);
    }

    fs = fopen("scrptr.bin", "wb");
    if (fs)
    {
        for (int i = 0; i < MAXTILES; i++)
        {
            char offset[2] = {};
            if (actorscrptr[i])
            {
                int of = actorscrptr[i] - script;
                offset[0] = (of >> 8) & 255;
                offset[1] = of & 255;
            }
            fwrite(offset, 1, 2, fs);
        }
        fclose(fs);
    }
#endif

    FILE* cf = fopen("script.c", "wb");
    if (!cf)
        return 0;

    fprintf(cf, "#include \"common.h\"\n#include \"duke3d.h\"\n\n");

    fprintf(cf, "long script[] = {\n");
    for (int i = 0; i < size; i++)
    {
        if ((i & 7) == 0)
            fprintf(cf, "    ");
        fprintf(cf, "%i,", script[i]);
        if ((i & 7) == 7)
            fprintf(cf, "\n");
        else
            fprintf(cf, " ");
    }

    fprintf(cf, "\n};\n\n");

    fprintf(cf, "short actorscrptr[] = {\n");
    for (int i = 0; i < MAXTILES; i++)
    {
        int of = actorscrptr[i] ? actorscrptr[i] - script : 0;
        if ((i & 7) == 0)
            fprintf(cf, "    ");
        fprintf(cf, "%i,", of);
        if ((i & 7) == 7)
            fprintf(cf, "\n");
        else
            fprintf(cf, " ");
    }

    fprintf(cf, "\n};\n\n");

    int maxquote = 0;
    for (int i = 0; i < NUMOFFIRSTTIMEACTIVE; i++)
    {
        if (fta_quotes[i][0])
            maxquote = i + 1;
    }

    fprintf(cf, "char *fta_quotes[] = {\n");

    for (int i = 0; i < maxquote; i++)
    {
        fprintf(cf, "    \"");
        for (int j = 0; j < 64; j++)
        {
            if (fta_quotes[i][j] == '\0' || fta_quotes[i][j] == '\r' || fta_quotes[i][j] == '\n')
                break;

            if (fta_quotes[i][j] == '"')
                fprintf(cf, "\\\"");
            else
                fprintf(cf, "%c", fta_quotes[i][j]);
        }
        fprintf(cf, "\",\n");
    }

    fprintf(cf, "\n};\n\n");

    fprintf(cf, "short soundvo[] = {\n");

    for (int i = 0; i < NUM_SOUNDS; i++)
    {
        if ((i & 7) == 0)
            fprintf(cf, "    ");
        fprintf(cf, "%i,", soundvo[i]);
        if ((i & 7) == 7)
            fprintf(cf, "\n");
        else
            fprintf(cf, " ");
    }

    fprintf(cf, "\n};\n\n");

    fprintf(cf, "short soundpe[] = {\n");

    for (int i = 0; i < NUM_SOUNDS; i++)
    {
        if ((i & 7) == 0)
            fprintf(cf, "    ");
        fprintf(cf, "%i,", soundpe[i]);
        if ((i & 7) == 7)
            fprintf(cf, "\n");
        else
            fprintf(cf, " ");
    }

    fprintf(cf, "\n};\n\n");

    fprintf(cf, "short soundps[] = {\n");

    for (int i = 0; i < NUM_SOUNDS; i++)
    {
        if ((i & 7) == 0)
            fprintf(cf, "    ");
        fprintf(cf, "%i,", soundps[i]);
        if ((i & 7) == 7)
            fprintf(cf, "\n");
        else
            fprintf(cf, " ");
    }

    fprintf(cf, "\n};\n\n");

    fprintf(cf, "char soundm[] = {\n");

    for (int i = 0; i < NUM_SOUNDS; i++)
    {
        if ((i & 7) == 0)
            fprintf(cf, "    ");
        fprintf(cf, "%i,", soundm[i]);
        if ((i & 7) == 7)
            fprintf(cf, "\n");
        else
            fprintf(cf, " ");
    }

    fprintf(cf, "\n};\n");

    fclose(cf);

}

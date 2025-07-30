#include "common.h"
#include "duke3d.h"

void func_800B4C80(void)
{
    long i;

    initspritelists();
    func_8005B728();

    for (i = 0; i < numsprites; i++)
    {
        if ((unsigned)i != (unsigned)insertsprite(sprite[i].sectnum, sprite[i].statnum))
        {
            gameexit("Sprite loading mismatch\n");
        }
    }
    updatesector(ps[0].posx, ps[0].posy, &ps[0].cursectnum);
}

void loadefs(void)
{
    long autoaim;
    long i;

    initengine();
    autoaim = 1;

    for (i = 0; i < MAXSCRIPTSIZE; i++)
    {
        if ((script[i]) > 50000000)
        {
            script[i] -= 50000000;
            script[i] += (unsigned long)script;
        }
    }

    ud.fta_on = 1;

    /*FAKEMATCH*/
    i = 0;
    do
    {
        ps[0].autoaim = autoaim;
        ps[1].autoaim = autoaim;
        ps[2].autoaim = autoaim;
        ps[3].autoaim = autoaim;
    } while (0);

    max_ammo_amount[PISTOL_WEAPON] = 192;
    max_ammo_amount[SHOTGUN_WEAPON] = 50;
    max_ammo_amount[CHAINGUN_WEAPON] = 400;
    max_ammo_amount[RPG_WEAPON] = 50;
    max_ammo_amount[HANDBOMB_WEAPON] = 50;
    max_ammo_amount[SHRINKER_WEAPON] = 66;
    max_ammo_amount[DEVISTATOR_WEAPON] = 48;
    max_ammo_amount[TRIPBOMB_WEAPON] = 10;
    max_ammo_amount[FREEZE_WEAPON] = 99;
    max_ammo_amount[GROW_WEAPON] = 99;
    max_ammo_amount[HANDREMOTE_WEAPON] = 0;
    max_ammo_amount[PISTOL2_WEAPON] = 36;
    max_ammo_amount[SHOTGUN2_WEAPON] = 20;
    max_ammo_amount[RPG2_WEAPON] = 25;
}
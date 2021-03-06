#ifndef DND_WEAPONDEFS_IN
#define DND_WEAPONDEFS_IN

#include "DnD_Common.h"

#define DND_BASE_CRITMODIFIER 200

enum {
	DND_WEAPON_FIST,
	DND_WEAPON_CHAINSAW,
	DND_WEAPON_DOUBLECHAINSAW,
	DND_WEAPON_KATANA,
	DND_WEAPON_EXCALIBAT,
	DND_WEAPON_INFERNOSWORD,
	DND_WEAPON_DUSKBLADE,
	DND_WEAPON_SICKLE,
	
	DND_WEAPON_PISTOL,
	DND_WEAPON_AKIMBOPISTOL,
	DND_WEAPON_MAGNUM,
	DND_WEAPON_LASERPISTOL,
	DND_WEAPON_ASSAULTRIFLE,
	DND_WEAPON_VIPERSTAFF,
	DND_WEAPON_RUBYWAND,
	DND_WEAPON_SCATTERGUN,
	
	DND_WEAPON_SHOTGUN,
	DND_WEAPON_PURIFIER,
	DND_WEAPON_KILLSTORM,
	DND_WEAPON_EMERALDWAND,
	DND_WEAPON_DEADLOCK,
	DND_WEAPON_NITROGENCROSSBOW,
	DND_WEAPON_WHEELOFTORMENT,
	
	DND_WEAPON_SUPERSHOTGUN,
	DND_WEAPON_HEAVYSUPERSHOTGUN,
	DND_WEAPON_ERASUS,
	DND_WEAPON_HELLSMAW,
	DND_WEAPON_PLASMACANNON,
	DND_WEAPON_SHOCKER,
	DND_WEAPON_HADES,
	
	DND_WEAPON_SILVERGUN,
	DND_WEAPON_SLAYER,
	
	DND_WEAPON_MACHINEGUN,
	DND_WEAPON_HEAVYMACHINEGUN,
	DND_WEAPON_LEADSPITTER,
	DND_WEAPON_DEMONSEALER,
	DND_WEAPON_TEMPLARMG,
	DND_WEAPON_RIOTCANNON,
	DND_WEAPON_ACIDRIFLE,
	DND_WEAPON_FUSIONBLASTER,
	
	DND_WEAPON_DESOLATOR,
	DND_WEAPON_MINIGUN,
	DND_WEAPON_EBONYCANNON,
	
	DND_WEAPON_ROCKETLAUNCHER,
	DND_WEAPON_TORPEDOLAUNCHER,
	DND_WEAPON_MERCURYLAUNCHER,
	DND_WEAPON_VINDICATOR,
	DND_WEAPON_METEORLAUNCHER,
	DND_WEAPON_HEAVYGL,
	DND_WEAPON_FREEZER,
	DND_WEAPON_GRENADELAUNCHER,
	DND_WEAPON_ROTARYGRENADELAUNCHER,
	DND_WEAPON_HEAVYMISSILELAUNCHER,
	
	DND_WEAPON_PLASMARIFLE,
	DND_WEAPON_NUCLEARPLASMARIFLE,
	DND_WEAPON_TURELCANNON,
	DND_WEAPON_FROSTFANG,
	DND_WEAPON_FLAMETHROWER,
	DND_WEAPON_LIGHTNINGGUN,
	DND_WEAPON_REBOUNDER,
	
	DND_WEAPON_RHINORIFLE,
	DND_WEAPON_NAILGUN,
	DND_WEAPON_BASILISK,
	
	DND_WEAPON_BFG6000,
	DND_WEAPON_BFG32768,
	DND_WEAPON_DEVASTATOR,
	DND_WEAPON_MFG,
	DND_WEAPON_IONCANNON,
	DND_WEAPON_THUNDERSTAFF,
	
	DND_WEAPON_GAUSSRIFLE,
	DND_WEAPON_RAILGUN,
	DND_WEAPON_DEATHRAY,
	
	DND_WEAPON_DEATHSTAFF,
	DND_WEAPON_RAZORFANG,
	DND_WEAPON_SUNSTAFF,
	DND_WEAPON_SOULREAVER,
	
	DND_WEAPON_SAWEDOFF,
	DND_WEAPON_SOULRENDER,
	DND_WEAPON_SMG,
	DND_WEAPON_HELLFORGECANNON,
	DND_WEAPON_BLOODFIENDSPINE,
	DND_WEAPON_ENFORCERRIFLE,
	DND_WEAPON_VENOM,
	DND_WEAPON_DEMONHEART,
	DND_WEAPON_DARKGLOVES,
	DND_WEAPON_HEAVYNAILGUN,
	DND_WEAPON_BERETTAS,
	DND_WEAPON_PLASMABOLTER,
	DND_WEAPON_RIPPERCANNON
};

#define DND_LIGHTNINGGUN_DMGPERSTACK 4
#define DND_DUSKBLADE_DMGPERSTACK 3
#define MAXWEPS DND_WEAPON_RIPPERCANNON + 1

enum {  
	WEAPON_NAME,
	WEAPON_TAG,
	WEAPON_SLOT,
	WEAPON_AMMO1,
	WEAPON_AMMO2,
	WEAPON_GOT,
	WEAPON_ICON,
	WEAPON_ICON64
};

// weapon mod data, mod_id contains the modifier, low and high are the rolled values
typedef struct {
	int mod_id;
	int tier;
	int low;
	int high;
} wep_mod_T;

enum {
	WEP_BONUS_CRIT,
	WEP_BONUS_CRITDMG,
	WEP_BONUS_CRITPERCENT,
	WEP_BONUS_DMG
};

typedef struct {
	int amt;
} wep_bonus_T;

#define MAX_WEP_MODS 4 // max allowed mods
#define MAX_WEP_BONUSES (WEP_BONUS_DMG + 1)
// store players mods etc.
typedef struct {
	int enchants;
	wep_bonus_T wep_bonuses[MAX_WEP_BONUSES];
	wep_mod_T wep_mods[MAX_WEP_MODS];
} wep_info_T;

global wep_info_T 2: Player_Weapon_Infos[MAXPLAYERS][MAXWEPS];

#endif
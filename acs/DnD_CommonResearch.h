#ifndef DND_COMMONRESEARCH_IN
#define DND_COMMONRESEARCH_IN

enum {
	RES_RAREARMOR,
	RES_SUPERARMOR,
	RES_MEDKITSTORE,
	RES_ACCESSORY,
	
	RES_FLECHETTE,
	RES_PIERCING,
	RES_ELECTRIC,
    RES_NITRO,
	RES_SLUGSHELL,
	
	RES_SONICGRENADE,
	RES_HEGRENADE,
	
	RES_DOUBLESPECIALCAP,
	
	RES_SLOT1OCCULT,
	RES_SLOT2LUXURY,
	RES_SLOT3LUXURY,
	RES_SLOT4LUXURY,
	RES_SLOT5LUXURY,
	RES_SLOT6LUXURY,
	RES_SLOT7LUXURY,
	
	RES_SLOT1UPG1,
	RES_SLOT1UPG2,
	
	RES_SLOT2UPG1,
	RES_SLOT2UPG2,
	
	RES_SLOT3UPG1,
    RES_SLOT3UPG2,
	RES_SLOT3UPG3,
	
	RES_SLOT3SSGUPG1,
	RES_SLOT3SSGUPG2,
	RES_SLOT3SSGUPG3,
	
	RES_SLOT4UPG1,
    RES_SLOT4UPG2,
	RES_SLOT4UPG3,
	RES_SLOT4UPG4,
	
	RES_SLOT5UPG1,
	RES_SLOT5GL,
	RES_SLOT5UPG2,
	RES_SLOT5UPG3,
	
	RES_SLOT6UPG1,
    RES_SLOT6UPG2,
	RES_SLOT6UPG3,
	
	RES_SLOT7UPG1,
	RES_SLOT7UPG2,
	
	RES_OCCULTABILITY,
	RES_SLOT8REVEAL,
	RES_NANOTECH,
	RES_OCCULTARTIFACT,
	
	RES_BIO1,
	RES_BIO2,
	RES_BIO3,
	RES_EXO1,
	RES_EXO2,
	RES_EXO3,
	RES_IMP1,
	RES_IMP2,
	RES_IMP3,
	
	RES_STASHTAB
};

#define DND_RESEARCH_BEGIN RES_RAREARMOR
#define DND_RESEARCH_END RES_STASHTAB
#define MAX_RESEARCHES (DND_RESEARCH_END + 1)
#define RESEARCH_BITSETS ((MAX_RESEARCHES / 31) + 1) //Remember: use 31 bits (zand uses signed numbers only, and min inv amount min is 0)

enum {
	RES_NA = 0,
	RES_KNOWN,
	RES_DONE
};

// 0 for NA, 1 for found, 2 for researched
int CheckResearchStatus(int res_id) {
	if(res_id > 30) {
		res_id %= 31;
		if(IsSet(CheckInventory("Research_Done_2"), res_id))
			return RES_DONE;
		if(IsSet(CheckInventory("Research_Discovered_2"), res_id))
			return RES_KNOWN;
	}
	else {
		if(IsSet(CheckInventory("Research_Done_1"), res_id))
			return RES_DONE;
		if(IsSet(CheckInventory("Research_Discovered_1"), res_id))
			return RES_KNOWN;
	}
	return RES_NA;
}

int CheckActorResearchStatus(int tid, int res_id) {
	// if we go over 62, we need another check, refrain from using Strparam here as these are potentially called often
	// Remember: Actor inventory max value is 31 bits (signed, and min is 0).
	if(res_id > 30) {
		res_id %= 31;
		if(IsSet(CheckActorInventory(tid, "Research_Done_2"), res_id))
			return RES_DONE;
		if(IsSet(CheckActorInventory(tid, "Research_Discovered_2"), res_id))
			return RES_KNOWN;
	}
	else {
		if(IsSet(CheckActorInventory(tid, "Research_Done_1"), res_id))
			return RES_DONE;
		if(IsSet(CheckActorInventory(tid, "Research_Discovered_1"), res_id))
			return RES_KNOWN;
	}
	return RES_NA;
}

#endif
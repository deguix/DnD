#ifndef DND_INVENTORY_IN
#define DND_INVENTORY_IN

#define MAXINVENTORYBLOCKS_HORIZ 5
#define MAXINVENTORYBLOCKS_VERT 9

#define MAX_INVENTORY_BOXES MAXINVENTORYBLOCKS_HORIZ * MAXINVENTORYBLOCKS_VERT

#define MAX_ITEM_LEVEL 100
#define MAX_ITEM_AFFIXTIERS 4
#define ITEM_TIER_SEPERATOR 100 / MAX_ITEM_AFFIXTIERS

#define MAX_ITEM_ATTRIBUTES 9

#define DND_SYNC_ITEMBEGIN DND_SYNC_ITEMTOPLEFTBOX
#define DND_SYNC_ITEMEND DND_SYNC_ITEMATTRIBUTES_VAL

#define INVENTORY_INFO_ALPHA 0.666
#define INVENTORY_HOLDTIME 0.5
#define INVENTORY_FADETIME 0.5

#define MAX_CHARM_AFFIXTIERS 4
#define CHARM_TIER_SEPERATOR 100 / MAX_ITEM_AFFIXTIERS
#define CHARM_ATTRIBLEVEL_SEPERATOR MAX_ITEM_LEVEL / MAX_CHARM_AFFIXTIERS

// MENU IDS
// Moved here because of dependencies
enum {
	RPGMENUINVENTORYID = 699,
};

enum {
	DND_ITEM_NULL,
	DND_ITEM_CHARM,
	DND_ITEM_HELM,
	DND_ITEM_BOOT,
	DND_ITEM_GLOVE,
	DND_ITEM_NECKLACE,
	DND_ITEM_RING
};

typedef struct {
	int attrib_val;
	int attrib_id;
} attr_inf_T;

typedef struct it {
	int width;										// width in inventory space
	int height;										// height in inventory space
	int item_image;									// image of item from image list
	int item_type;									// what type of item it is
	int item_subtype;								// subtype for items that have it (charms etc)
	int item_level;									// what level this item is
	int topleftboxid;								// used to determine the owning pointer (-1 of this is the pointer)
	int attrib_count;								// count of attributes
	attr_inf_T attributes[MAX_ITEM_ATTRIBUTES];		// attribute list
} inventory_T;

// so far only charms count here
enum {
	IM_SCHRM1,
	IM_SCHRM2,
	IM_SCHRM3,
	
	IM_MCHRM1,
	IM_MCHRM2,
	IM_MCHRM3,
	
	IM_LCHRM1,
	IM_LCHRM2,
	IM_LCHRM3,
};

#define DND_SMALLCHARM_IMAGEBEGIN IM_SCHRM1
#define DND_SMALLCHARM_IMAGEEND IM_SCHRM3
#define DND_MEDIUMCHARM_IMAGEBEGIN IM_MCHRM1
#define DND_MEDIUMCHARM_IMAGEEND IM_MCHRM3
#define DND_LARGECHARM_IMAGEBEGIN IM_LCHRM1
#define DND_LARGECHARM_IMAGEEND IM_LCHRM3

#define MAX_ITEM_IMAGES IM_LCHRM3 + 1
str Item_Images[MAX_ITEM_IMAGES] = {
	"SCHRM1",
	"SCHRM2",
	"SCHRM3",
	
	"MCHRM1",
	"MCHRM2",
	"MCHRM3",
	
	"LCHRM1",
	"LCHRM2",
	"LCHRM3"
};

enum {
	DND_INVDROP_CHARM,
};
#define MAX_DND_INVDROPACTORS DND_INVDROP_CHARM + 1
str InventoryDropActors[MAX_DND_INVDROPACTORS] = {
	"CharmDrop"
};

#define ITEMLEVEL_VARIANCE_LOWER 15
#define ITEMLEVEL_VARIANCE_HIGHER 7


// holds inventories of all players
global inventory_T 11: PlayerInventoryList[MAXPLAYERS][MAX_INVENTORY_BOXES];
#define MAX_INVENTORIES_ON_FIELD 256
global inventory_T 13: Inventories_On_Field[MAX_INVENTORIES_ON_FIELD];
global inventory_T 14: TradeViewList[MAXPLAYERS][MAX_INVENTORY_BOXES];

#define INVSOURCE_PLAYER PlayerInventoryList
#define INVSOURCE_CHARMUSED Charms_Used

// Creates an item on the game field
int CreateItemSpot() {
	int pos = -1, i;
	for(i = 0; i < MAX_INVENTORIES_ON_FIELD; ++i)
		if(Inventories_On_Field[i].item_type == DND_ITEM_NULL) {
			pos = i;
			break;
		}
	return pos;
}

void FreeItem(int pnum, int item_index, int source, bool dontSync) {
	int i, j;
	int temp;
	int wtemp = GetPlayerItemSyncValue(DND_SYNC_ITEMWIDTH, item_index, -1, source);
	int htemp = GetPlayerItemSyncValue(DND_SYNC_ITEMHEIGHT, item_index, -1, source);
	SetItemSyncValue(DND_SYNC_ITEMSUBTYPE, item_index, -1, 0, source);
	SetItemSyncValue(DND_SYNC_ITEMIMAGE, item_index, -1, 0, source);
	SetItemSyncValue(DND_SYNC_ITEMLEVEL, item_index, -1, 0, source);
	temp = GetPlayerItemSyncValue(DND_SYNC_ITEMSATTRIBCOUNT, item_index, -1, source);
	for(j = 0; j < temp; ++j) {
		SetItemSyncValue(DND_SYNC_ITEMATTRIBUTES_ID, item_index, i, 0, source);
		SetItemSyncValue(DND_SYNC_ITEMATTRIBUTES_VAL, item_index, i, 0, source);
	}
	SetItemSyncValue(DND_SYNC_ITEMSATTRIBCOUNT, item_index, -1, 0, source);
	for(j = 0; j < htemp; ++j)
		for(i = 0; i < wtemp; ++i) {
			SetItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, item_index + j * MAXINVENTORYBLOCKS_VERT + i, -1, 0, source);
			SetItemSyncValue(DND_SYNC_ITEMTYPE, item_index + j * MAXINVENTORYBLOCKS_VERT + i, -1, DND_ITEM_NULL, source);
		}
	SetItemSyncValue(DND_SYNC_ITEMWIDTH, item_index, -1, 0, source);
	SetItemSyncValue(DND_SYNC_ITEMHEIGHT, item_index, -1, 0, source);
	if(!dontSync)
		SyncItemData_Null(item_index, source, wtemp, htemp);
}

void FreeSpot(int pnum, int item_index, int source) {
	int j, temp;
	SetItemSyncValue(DND_SYNC_ITEMSUBTYPE, item_index, -1, 0, source);
	SetItemSyncValue(DND_SYNC_ITEMIMAGE, item_index, -1, 0, source);
	SetItemSyncValue(DND_SYNC_ITEMLEVEL, item_index, -1, 0, source);
	
	temp = GetPlayerItemSyncValue(DND_SYNC_ITEMSATTRIBCOUNT, item_index, -1, source);
	for(j = 0; j < temp; ++j) {
		SetItemSyncValue(DND_SYNC_ITEMATTRIBUTES_ID, item_index, j, 0, source);
		SetItemSyncValue(DND_SYNC_ITEMATTRIBUTES_VAL, item_index, j, 0, source);
	}
	SetItemSyncValue(DND_SYNC_ITEMSATTRIBCOUNT, item_index, -1, 0, source);
	SetItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, item_index, -1, 0, source);
	SetItemSyncValue(DND_SYNC_ITEMTYPE, item_index, -1, DND_ITEM_NULL, source);
	SetItemSyncValue(DND_SYNC_ITEMWIDTH, item_index, -1, 0, source);
	SetItemSyncValue(DND_SYNC_ITEMHEIGHT, item_index, -1, 0, source);
}

// note to self: height is => horizontal, moving heights => x * MAXINVENTORYBLOCKS_VERT, width is vertical, just + x
int GetFreeSpotForItem(int item_index, int player_index, int source) {
	int i = 0, j = 0;
	int bid = 0, wcheck = 0, hcheck = 0;
	int w = GetPlayerItemSyncValue(DND_SYNC_ITEMWIDTH, item_index, -1, source);
	int h = GetPlayerItemSyncValue(DND_SYNC_ITEMHEIGHT, item_index, -1, source);
	bool unfit = false;

	// try every line
	for(i = 0; i < MAXINVENTORYBLOCKS_VERT; ++i) {
		for(j = 0; j < MAXINVENTORYBLOCKS_HORIZ; ++j) {
			// if width matches, try height from here on then and if unfit, restart at a new coordinate
			unfit = false;
			for(hcheck = 0; !unfit && hcheck < h && hcheck + j < MAXINVENTORYBLOCKS_HORIZ; ++hcheck) {
				for(wcheck = 0; !unfit && wcheck < w && wcheck + i < MAXINVENTORYBLOCKS_VERT; ++wcheck) {
					bid = (j + hcheck) * MAXINVENTORYBLOCKS_VERT + i + wcheck;
					if(PlayerInventoryList[player_index][bid].item_type != DND_ITEM_NULL || bid >= MAX_INVENTORY_BOXES)
						unfit = true;
				}
			}
			// we return top left corner box id
			if(wcheck == w && hcheck == h && !unfit)
				return j * MAXINVENTORYBLOCKS_VERT + i;
		}
	}
	return -1;
}

// based on average player level
int RollItemLevel() {
	int res = 0;
	// return average player level, +- some value
	int pavg = total_level / PlayerCount();
	if(pavg > 2 * ITEMLEVEL_VARIANCE_LOWER) {
		res = pavg + random(-ITEMLEVEL_VARIANCE_LOWER, ITEMLEVEL_VARIANCE_HIGHER);
		if(res > MAX_ITEM_LEVEL)
			res = MAX_ITEM_LEVEL;
		return res;
	}
	return pavg + random((-pavg + 1) / 2, ITEMLEVEL_VARIANCE_HIGHER);
}

bool CheckItemAttribute(int item_pos, int attrib_index, bool onField, int pnum, int count) {
	int i;
	if(onField) {
		for(i = 0; i < count; ++i)
			if(Inventories_On_Field[item_pos].attributes[i].attrib_id == attrib_index)
				return true;
		return false;
	}
	else {
		for(i = 0; i < count; ++i)
			if(PlayerInventoryList[pnum][item_pos].attributes[i].attrib_id == attrib_index)
				return true;
		return false;
	}
	return false;
}

void CopyItem(bool fieldToPlayer, int fieldpos, int player_index, int item_index) {
	int i, j, k, wtemp, htemp;
	if(!fieldToPlayer) {
		wtemp = PlayerInventoryList[player_index][item_index].topleftboxid - 1;
		Inventories_On_Field[fieldpos].width = PlayerInventoryList[player_index][wtemp].width;
		Inventories_On_Field[fieldpos].height = PlayerInventoryList[player_index][wtemp].height;
		Inventories_On_Field[fieldpos].item_type = PlayerInventoryList[player_index][wtemp].item_type;
		Inventories_On_Field[fieldpos].item_subtype = PlayerInventoryList[player_index][wtemp].item_subtype;
		Inventories_On_Field[fieldpos].item_image = PlayerInventoryList[player_index][wtemp].item_image;
		Inventories_On_Field[fieldpos].item_level = PlayerInventoryList[player_index][wtemp].item_level;
		Inventories_On_Field[fieldpos].attrib_count = PlayerInventoryList[player_index][wtemp].attrib_count;
		Inventories_On_Field[fieldpos].topleftboxid = 0;
		for(i = 0; i < Inventories_On_Field[fieldpos].attrib_count; ++i) {
			Inventories_On_Field[fieldpos].attributes[i].attrib_id = PlayerInventoryList[player_index][wtemp].attributes[i].attrib_id;
			Inventories_On_Field[fieldpos].attributes[i].attrib_val = PlayerInventoryList[player_index][wtemp].attributes[i].attrib_val;
		}

		// the leftover spot is a null charm
		FreeItem(player_index, wtemp, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, false);
	}
	else {
		// handle the box management
		wtemp = Inventories_On_Field[fieldpos].width;
		htemp = Inventories_On_Field[fieldpos].height;
		PlayerInventoryList[player_index][item_index].width = wtemp;
		PlayerInventoryList[player_index][item_index].height = htemp;
		PlayerInventoryList[player_index][item_index].item_subtype = Inventories_On_Field[fieldpos].item_subtype;
		PlayerInventoryList[player_index][item_index].item_image = Inventories_On_Field[fieldpos].item_image;
		PlayerInventoryList[player_index][item_index].item_level = Inventories_On_Field[fieldpos].item_level;
		PlayerInventoryList[player_index][item_index].attrib_count = Inventories_On_Field[fieldpos].attrib_count;
		for(k = 0; k < PlayerInventoryList[player_index][item_index].attrib_count; ++k) {
			PlayerInventoryList[player_index][item_index].attributes[k].attrib_id = Inventories_On_Field[fieldpos].attributes[k].attrib_id;
			PlayerInventoryList[player_index][item_index].attributes[k].attrib_val = Inventories_On_Field[fieldpos].attributes[k].attrib_val;
		}
		for(i = 0; i < htemp; ++i)
			for(j = 0; j < wtemp; ++j) {
				PlayerInventoryList[player_index][item_index + i * MAXINVENTORYBLOCKS_VERT + j].item_type = Inventories_On_Field[fieldpos].item_type;
				PlayerInventoryList[player_index][item_index + i * MAXINVENTORYBLOCKS_VERT + j].topleftboxid = item_index + 1;
			}
		// the leftover spot is a null charm
		Inventories_On_Field[fieldpos].width = 1;
		Inventories_On_Field[fieldpos].height = 1;
		Inventories_On_Field[fieldpos].item_type = DND_ITEM_NULL;
		Inventories_On_Field[fieldpos].item_subtype = 0;
		Inventories_On_Field[fieldpos].item_image = 0;
		Inventories_On_Field[fieldpos].item_level = 0;
		for(k = 0; k < Inventories_On_Field[fieldpos].attrib_count; ++k) {
			Inventories_On_Field[fieldpos].attributes[k].attrib_id = 0;
			Inventories_On_Field[fieldpos].attributes[k].attrib_val = 0;
		}
		Inventories_On_Field[fieldpos].attrib_count = 0;
		SyncItemData(item_index, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, wtemp, htemp);
	}
}

// check if clicked spot is free for the item we want to put
bool IsFreeSpot(int itempos, int emptypos) {
	int pnum = PlayerNumber();
	int temp = PlayerInventoryList[pnum][itempos].topleftboxid - 1;
	int w = PlayerInventoryList[pnum][temp].width;
	int h = PlayerInventoryList[pnum][temp].height;
	int offset = temp - itempos;
	int bid = 0;
	
	for(int i = 0; i < h; ++i) {
		for(int j = 0; j < w; ++j) {
			bid = emptypos + offset + j + i * MAXINVENTORYBLOCKS_VERT;
			if(bid >= MAX_INVENTORY_BOXES || bid < 0)
				return false;
			// if not empty and it's not us
			if(PlayerInventoryList[pnum][bid].topleftboxid && PlayerInventoryList[pnum][bid].topleftboxid - 1 != temp)
				return false;
		}
	}
	return true;
}

bool IsFreeSpot_Offset(int itempos, int emptypos, int itemsource, int emptysource) {
	int temp = GetPlayerItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, itempos, -1, itemsource) - 1;
	int w = GetPlayerItemSyncValue(DND_SYNC_ITEMWIDTH, temp, -1, itemsource);
	int h = GetPlayerItemSyncValue(DND_SYNC_ITEMHEIGHT, temp, -1, itemsource);
	int offset = temp - itempos;
	int bid = 0;
	int tb;
	
	for(int i = 0; i < h; ++i) {
		for(int j = 0; j < w; ++j) {
			bid = emptypos + offset + j + i * MAXINVENTORYBLOCKS_VERT;
			if(bid >= MAX_INVENTORY_BOXES || bid < 0)
				return false;
			// if not empty and it's not us
			tb = GetPlayerItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, bid, -1, emptysource);
			if(tb && tb - 1 != temp)
				return false;
		}
	}
	return true;
}

bool IsSourceInventoryView(int source) {
	return source == DND_SYNC_ITEMSOURCE_PLAYERINVENTORY || source == DND_SYNC_ITEMSOURCE_TRADEVIEW;
}

// assumes both spots have an item
// if ipos1 has enough space to acommodate ipos2
bool CanSwapItems(int ipos1, int ipos2, int offset1, int offset2, int source1, int source2) {
	int pnum = PlayerNumber();
	int i, j;
	int w1, w2, h1, h2;
	int bid = 0, tb1, tb2;
	// from ipos2 to ipos1
	tb1 = GetPlayerItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, ipos1, -1, source1) - 1;
	tb2 = GetPlayerItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, ipos2, -1, source2) - 1;
	w2 = GetPlayerItemSyncValue(DND_SYNC_ITEMWIDTH, tb2, -1, source2);
	h2 = GetPlayerItemSyncValue(DND_SYNC_ITEMHEIGHT, tb2, -1, source2);
	for(i = 0; i < h2; ++i) {
		for(j = 0; j < w2; ++j) {
			bid = ipos1 + offset2 + j + i * MAXINVENTORYBLOCKS_VERT;
			if(bid >= MAX_INVENTORY_BOXES || bid < 0)
				return false;
			if(GetPlayerItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, bid, -1, source1) - 1 != tb1 && GetPlayerItemSyncValue(DND_SYNC_ITEMTYPE, bid, -1, source1))
				return false;
		}
	}
	// from ipos1 to ipos2
	w1 = GetPlayerItemSyncValue(DND_SYNC_ITEMWIDTH, tb1, -1, source1);
	h1 = GetPlayerItemSyncValue(DND_SYNC_ITEMHEIGHT, tb1, -1, source1);
	for(i = 0; i < h1; ++i) {
		for(j = 0; j < w1; ++j) {
			bid = ipos2 + offset1 + j + i * MAXINVENTORYBLOCKS_VERT;
			if(bid >= MAX_INVENTORY_BOXES || bid < 0)
				return false;
			if(GetPlayerItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, bid, -1, source2) - 1 != tb2 && GetPlayerItemSyncValue(DND_SYNC_ITEMTYPE, bid, -1, source2) != DND_ITEM_NULL)
				return false;
		}
	}
	return true;
}

void SwapItemProperty(int ipos1, int ipos2, int property, int source1, int source2, int offset1, int offset2) {
	int pnum = PlayerNumber();
	int temp;
	int i;
	int ht, wt;
	
	int h1, h2, w1, w2;
	int itype1, itype2;
	
	int tb1, tb2;
	
	switch(property) {
		case DND_SYNC_ITEMTOPLEFTBOX:
			tb1 = GetPlayerItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, ipos1, -1, source1) - 1;
			tb2 = GetPlayerItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, ipos2, -1, source2) - 1;
			h1 = GetPlayerItemSyncValue(DND_SYNC_ITEMHEIGHT, tb1, -1, source1);
			h2 = GetPlayerItemSyncValue(DND_SYNC_ITEMHEIGHT, tb2, -1, source2);
			w1 = GetPlayerItemSyncValue(DND_SYNC_ITEMWIDTH, tb1, -1, source1);
			w2 = GetPlayerItemSyncValue(DND_SYNC_ITEMWIDTH, tb2, -1, source2);
			// reset pointers
			for(ht = 0; ht < h1; ++ht)
				for(wt = 0; wt < w1; ++wt)
					SetItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, tb1 + wt + ht * MAXINVENTORYBLOCKS_VERT, -1, 0, source1);
			
			for(ht = 0; ht < h2; ++ht)
				for(wt = 0; wt < w2; ++wt)
					SetItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, tb2 + wt + ht * MAXINVENTORYBLOCKS_VERT, -1, 0, source2);
					
			// handle the leftover pointers
			// update pointers for new scale
			// ipos + 1 because -1 of this is used as pointer
			if(IsSourceInventoryView(source1)) {
				for(ht = 0; ht < h2; ++ht) {
					for(wt = 0; wt < w2; ++wt)
						SetItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, ipos1 + offset2 + wt + ht * MAXINVENTORYBLOCKS_VERT, -1, ipos1 + offset2 + 1, source1);
				}
			}
			else
				SetItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, ipos1, -1, ipos1 + 1, source1);
				
			if(IsSourceInventoryView(source2)) {
				for(ht = 0; ht < h1; ++ht) {
					for(wt = 0; wt < w1; ++wt)
						SetItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, ipos2 + offset1 + wt + ht * MAXINVENTORYBLOCKS_VERT, -1, ipos2 + offset1 + 1, source2);
				}
			}
			else
				SetItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, ipos2, -1, ipos2 + 1, source2);
		break;
		case DND_SYNC_ITEMTYPE:
			tb1 = ipos1 + offset1;
			tb2 = ipos2 + offset2;
			h1 = GetPlayerItemSyncValue(DND_SYNC_ITEMHEIGHT, tb1, -1, source1);
			h2 = GetPlayerItemSyncValue(DND_SYNC_ITEMHEIGHT, tb2, -1, source2);
			w1 = GetPlayerItemSyncValue(DND_SYNC_ITEMWIDTH, tb1, -1, source1);
			w2 = GetPlayerItemSyncValue(DND_SYNC_ITEMWIDTH, tb2, -1, source2);
			itype1 = GetPlayerItemSyncValue(DND_SYNC_ITEMTYPE, tb1, -1, source1);
			itype2 = GetPlayerItemSyncValue(DND_SYNC_ITEMTYPE, tb2, -1, source2);
			// reset pointers
			for(ht = 0; ht < h1; ++ht)
				for(wt = 0; wt < w1; ++wt)
					SetItemSyncValue(DND_SYNC_ITEMTYPE, tb1 + wt + ht * MAXINVENTORYBLOCKS_VERT, -1, DND_ITEM_NULL, source1);
			
			for(ht = 0; ht < h2; ++ht)
				for(wt = 0; wt < w2; ++wt)
					SetItemSyncValue(DND_SYNC_ITEMTYPE, tb2 + wt + ht * MAXINVENTORYBLOCKS_VERT, -1, DND_ITEM_NULL, source2);
			
			// handle the leftover pointers
			// update pointers for new scale
			if(IsSourceInventoryView(source1)) {
				for(ht = 0; ht < h2; ++ht) {
					for(wt = 0; wt < w2; ++wt)
						SetItemSyncValue(DND_SYNC_ITEMTYPE, ipos1 + offset2 + wt + ht * MAXINVENTORYBLOCKS_VERT, -1, itype2, source1);
				}
			}
			else
				SetItemSyncValue(DND_SYNC_ITEMTYPE, ipos1, -1, itype2, source1);
				
			if(IsSourceInventoryView(source2)) {
				for(ht = 0; ht < h1; ++ht) {
					for(wt = 0; wt < w1; ++wt)
						SetItemSyncValue(DND_SYNC_ITEMTYPE, ipos2 + offset1 + wt + ht * MAXINVENTORYBLOCKS_VERT, -1, itype1, source2);
				}
			}
			else
				SetItemSyncValue(DND_SYNC_ITEMTYPE, ipos2, -1, itype1, source2);
		break;
		case DND_SYNC_ITEMWIDTH:
		case DND_SYNC_ITEMHEIGHT:
		case DND_SYNC_ITEMIMAGE:
		case DND_SYNC_ITEMSUBTYPE:
		case DND_SYNC_ITEMLEVEL:
		case DND_SYNC_ITEMSATTRIBCOUNT:
			tb1 = ipos1 + offset1;
			tb2 = ipos2 + offset2;
			temp = GetPlayerItemSyncValue(property, tb1, -1, source1);
			SetItemSyncValue(property, ipos1 + offset2, -1, GetPlayerItemSyncValue(property, tb2, -1, source2), source1);
			SetItemSyncValue(property, ipos2 + offset1, -1, temp, source2);
		break;
		case DND_SYNC_ITEMATTRIBUTES_ID:
		case DND_SYNC_ITEMATTRIBUTES_VAL:
			tb1 = ipos1 + offset1;
			tb2 = ipos2 + offset2;
			wt = GetPlayerItemSyncValue(DND_SYNC_ITEMSATTRIBCOUNT, tb1, -1, source1);
			ht = GetPlayerItemSyncValue(DND_SYNC_ITEMSATTRIBCOUNT, tb2, -1, source2);
			// if my attrib count now is higher, I need to take from whoever had the higher one before
			if(wt > ht) {
				for(i = 0; i < wt; ++i) {
					// ipos2 had more attributes, it covers a larger space, take them all
					temp = GetPlayerItemSyncValue(property, tb2, i, source2);
					SetItemSyncValue(property, ipos2 + offset1, i, GetPlayerItemSyncValue(property, tb1, i, source1), source2);
					SetItemSyncValue(property, ipos1 + offset2, i, temp, source1);
				}
			}
			else {
				for(i = 0; i < ht; ++i) {
					// ipos1 had more attributes, it covers a larger space, take them all
					temp = GetPlayerItemSyncValue(property, tb1, i, source1);
					SetItemSyncValue(property, ipos1 + offset2, i, GetPlayerItemSyncValue(property, tb2, i, source2), source1);
					SetItemSyncValue(property, ipos2 + offset1, i, temp, source2);
				}
			}
		break;
	}
}

// for swapping items -- assumes neither are null
void SwapItems(int ipos1, int ipos2, int source1, int source2, bool dontSync) {
	// check if there's enough space
	// find if there is some sort of offset we must take care of, this matters only if we are swapping in inventory
	int offset1 = 0, offset2 = 0;
	if(IsSourceInventoryView(source1))
		offset1 = GetPlayerItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, ipos1, -1, source1) - 1 - ipos1;
	if(IsSourceInventoryView(source2))
		offset2 = GetPlayerItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, ipos2, -1, source2) - 1 - ipos2;
	if(CanSwapItems(ipos1, ipos2, offset1, offset2, source1, source2)) {
		// we need difference in height and width to the topboxid of this item, then we can anchor it properly
		// divide by max_vert to get height
		int i, j;
		int h1p = GetPlayerItemSyncValue(DND_SYNC_ITEMHEIGHT, ipos1 + offset1, -1, source1);
		int h2p = GetPlayerItemSyncValue(DND_SYNC_ITEMHEIGHT, ipos2 + offset2, -1, source2);
		int w1p = GetPlayerItemSyncValue(DND_SYNC_ITEMWIDTH, ipos1 + offset1, -1, source1); 
		int w2p = GetPlayerItemSyncValue(DND_SYNC_ITEMWIDTH, ipos2 + offset2, -1, source2);
		
		for(i = DND_SYNC_ITEMBEGIN; i <= DND_SYNC_ITEMEND; ++i)
			SwapItemProperty(ipos1, ipos2, i, source1, source2, offset1, offset2);
		
		// for large swaps, a big sync at the end is way more favorable
		if(!dontSync) {
			if(GetPlayerItemSyncValue(DND_SYNC_ITEMTYPE, ipos1, -1, source1) == DND_ITEM_NULL) {
				SyncItemData_Null(ipos1 + offset1, source1, w1p, h1p);
			}
			else {
				SyncItemPointers(ipos1 + offset1, source1, w1p, h1p);
				SyncItemData(ipos1 + offset2, source1, -1, -1);
			}
				
			if(GetPlayerItemSyncValue(DND_SYNC_ITEMTYPE, ipos2, -1, source2) == DND_ITEM_NULL) {
				SyncItemData_Null(ipos2 + offset2, source2, w2p, h2p);
			}
			else {
				SyncItemPointers(ipos2 + offset2, source2, w2p, h2p);
				SyncItemData(ipos2 + offset1, source2, -1, -1);
			}
		}
	}
}

/*
	0  1  2  3  4
	9 10 11 12 13
   18 19 20 21 22
   27 28 29 30 31
*/

bool InventoryBoxContainsPoint(int point, int boxleft, int w, int h) {
	// for every line, check if point falls into a linear range
	for(int i = 0; i < h; ++i)
		if(boxleft + i * MAXINVENTORYBLOCKS_VERT <= point && boxleft + i * MAXINVENTORYBLOCKS_VERT + w - 1 >= point)
			return true;
	return false;
}

void MoveItem(int itempos, int emptypos) {
	int pnum = PlayerNumber();
	int tb = PlayerInventoryList[pnum][itempos].topleftboxid - 1;
	int offset = tb - itempos;
	
	int i, j, bid;
	
	int w = PlayerInventoryList[pnum][tb].width;
	int h = PlayerInventoryList[pnum][tb].height;

	// these two mark box ids that have been modified, ie. need updates
	// if the boxes aren't in range of itempos, they will need to be nulled
	bool set1 = false;
	bool set2 = false;
	
	int temp = emptypos + offset;
	
	for(i = 0; i < h; ++i) 
		for(j = 0; j < w; ++j) {
			bid = temp + j + i * MAXINVENTORYBLOCKS_VERT;
			if(!InventoryBoxContainsPoint(tb + j + i * MAXINVENTORYBLOCKS_VERT, temp, w, h)) {
				if(tb + j + i * MAXINVENTORYBLOCKS_VERT < 32)
					set1 |= 1 << (tb + j + i * MAXINVENTORYBLOCKS_VERT);
				else
					set2 |= 1 << (tb + j + i * MAXINVENTORYBLOCKS_VERT);
			}
		}
	
	PlayerInventoryList[pnum][temp].width = w;
	PlayerInventoryList[pnum][temp].height = h;
	PlayerInventoryList[pnum][temp].item_subtype = PlayerInventoryList[pnum][tb].item_subtype;
	PlayerInventoryList[pnum][temp].item_image = PlayerInventoryList[pnum][tb].item_image;
	PlayerInventoryList[pnum][temp].item_level = PlayerInventoryList[pnum][tb].item_level;
	PlayerInventoryList[pnum][temp].attrib_count = PlayerInventoryList[pnum][tb].attrib_count;
	for(i = 0; i < PlayerInventoryList[pnum][temp].attrib_count; ++i) {
		PlayerInventoryList[pnum][temp].attributes[i].attrib_id = PlayerInventoryList[pnum][tb].attributes[i].attrib_id;
		PlayerInventoryList[pnum][temp].attributes[i].attrib_val = PlayerInventoryList[pnum][tb].attributes[i].attrib_val;
	}
	for(i = 0; i < h; ++i)
		for(j = 0; j < w; ++j) {
			PlayerInventoryList[pnum][temp + i * MAXINVENTORYBLOCKS_VERT + j].item_type = PlayerInventoryList[pnum][tb].item_type;
			PlayerInventoryList[pnum][temp + i * MAXINVENTORYBLOCKS_VERT + j].topleftboxid = temp + 1;
		}

	if(set1 || set2) {
		for(i = 0; i < 32; ++i)
			if(IsSet(set1, i))
				FreeSpot(pnum, i, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
			
		for(i = 0; i < MAX_INVENTORY_BOXES - 32; ++i)
			if(IsSet(set2, i))
				FreeSpot(pnum, i + 32, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
		SyncItemPointers(tb, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, w, h);
	}
	else {
		// Simply null the leftover spot, no collision happened
		FreeItem(pnum, tb, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, false);
	}
	
	SyncItemData(emptypos + offset, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, -1, -1);
}

// this is made specifically for trade view, the one above is optimized for normal inventory
void MoveItemTrade(int itempos, int emptypos, int itemsource, int emptysource) {
	int pnum = PlayerNumber();
	int tb = GetPlayerItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, itempos, -1, itemsource) - 1;
	int offset = tb - itempos;
	
	int i, j, bid;
	
	int w = GetPlayerItemSyncValue(DND_SYNC_ITEMWIDTH, tb, -1, itemsource);
	int h = GetPlayerItemSyncValue(DND_SYNC_ITEMHEIGHT, tb, -1, itemsource);

	// these two mark box ids that have been modified, ie. need updates
	// if the boxes aren't in range of itempos, they will need to be nulled
	bool set1 = false;
	bool set2 = false;
	
	int temp = emptypos + offset;
	
	for(i = 0; i < h; ++i) 
		for(j = 0; j < w; ++j) {
			bid = temp + j + i * MAXINVENTORYBLOCKS_VERT;
			if(!InventoryBoxContainsPoint(tb + j + i * MAXINVENTORYBLOCKS_VERT, temp, w, h)) {
				if(tb + j + i * MAXINVENTORYBLOCKS_VERT < 32)
					set1 |= 1 << (tb + j + i * MAXINVENTORYBLOCKS_VERT);
				else
					set2 |= 1 << (tb + j + i * MAXINVENTORYBLOCKS_VERT);
			}
		}
	
	SetItemSyncValue(DND_SYNC_ITEMWIDTH, temp, -1, w, emptysource);
	SetItemSyncValue(DND_SYNC_ITEMHEIGHT, temp, -1, h, emptysource);
	SetItemSyncValue(DND_SYNC_ITEMSUBTYPE, temp, -1, GetPlayerItemSyncValue(DND_SYNC_ITEMSUBTYPE, tb, -1, itemsource), emptysource);
	SetItemSyncValue(DND_SYNC_ITEMIMAGE, temp, -1, GetPlayerItemSyncValue(DND_SYNC_ITEMIMAGE, tb, -1, itemsource), emptysource);
	SetItemSyncValue(DND_SYNC_ITEMLEVEL, temp, -1, GetPlayerItemSyncValue(DND_SYNC_ITEMLEVEL, tb, -1, itemsource), emptysource);
	SetItemSyncValue(DND_SYNC_ITEMSATTRIBCOUNT, temp, -1, GetPlayerItemSyncValue(DND_SYNC_ITEMSATTRIBCOUNT, tb, -1, itemsource), emptysource);
	bid = GetPlayerItemSyncValue(DND_SYNC_ITEMSATTRIBCOUNT, temp, -1, emptysource);
	for(i = 0; i < bid; ++i) {
		SetItemSyncValue(DND_SYNC_ITEMATTRIBUTES_ID, temp, i, GetPlayerItemSyncValue(DND_SYNC_ITEMATTRIBUTES_ID, tb, i, itemsource), emptysource);
		SetItemSyncValue(DND_SYNC_ITEMATTRIBUTES_VAL, temp, i, GetPlayerItemSyncValue(DND_SYNC_ITEMATTRIBUTES_VAL, tb, i, itemsource), emptysource);
	}
	bid = GetPlayerItemSyncValue(DND_SYNC_ITEMTYPE, tb, -1, itemsource);
	for(i = 0; i < h; ++i)
		for(j = 0; j < w; ++j) {
			SetItemSyncValue(DND_SYNC_ITEMTYPE, temp + i * MAXINVENTORYBLOCKS_VERT + j, -1, bid, emptysource);
			SetItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, temp + i * MAXINVENTORYBLOCKS_VERT + j, -1, temp + 1, emptysource);
		}

	if(set1 || set2) {
		for(i = 0; i < 32; ++i)
			if(IsSet(set1, i))
				FreeSpot(pnum, i, itemsource);
			
		for(i = 0; i < MAX_INVENTORY_BOXES - 32; ++i)
			if(IsSet(set2, i))
				FreeSpot(pnum, i + 32, itemsource);
		SyncItemPointers(tb, itemsource, w, h);
	}
	else {
		// Simply null the leftover spot, no collision happened
		FreeItem(pnum, tb, itemsource, false);
	}
	
	SyncItemData(temp, emptysource, -1, -1);
}

void DrawInventoryInfo_Field(int topboxid, int source, int bx, int by) {
	int pnum = PlayerNumber();
	int i = 0, j, temp, val;
	DeleteTextRange(RPGMENUINVENTORYID - 3 * MAX_INVENTORY_BOXES - 13, RPGMENUINVENTORYID - 3 * MAX_INVENTORY_BOXES);
	if(topboxid != -1 && GetPlayerItemSyncValue(DND_SYNC_ITEMTYPE, topboxid, -1, source) != DND_ITEM_NULL) {
		SetHudSize(480, 320, 1);
		SetFont("LDTITINF");
		HudMessage(s:"A"; HUDMSG_PLAIN | HUDMSG_ALPHA | HUDMSG_FADEOUT, RPGMENUINVENTORYID - 3 * MAX_INVENTORY_BOXES, CR_WHITE, bx, by, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA);
		by += 10.0;
		// show item details
		SetFont(Item_Images[GetPlayerItemSyncValue(DND_SYNC_ITEMIMAGE, topboxid, -1, source)]);
		HudMessage(s:"A"; HUDMSG_PLAIN | HUDMSG_FADEOUT, RPGMENUINVENTORYID - 3 * MAX_INVENTORY_BOXES - 1, CR_WHITE, bx, by, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA);
		SetHudSize(720, 480, 1);
		bx *= 3; by *= 3;
		bx /= 2; by /= 2;
		bx &= 0xFFFF0000;
		by &= 0xFFFF0000;
		bx += 0.4;
		by += 48.1;
		SetHudClipRect(-96, 80, 256, 224, 256, 1);
		SetFont("SMALLFONT");
		if(GetPlayerItemSyncValue(DND_SYNC_ITEMTYPE, topboxid, -1, source) == DND_ITEM_CHARM) {
			HudMessage(s:Charm_Tiers[GetPlayerItemSyncValue(DND_SYNC_ITEMLEVEL, topboxid, -1, source) / CHARM_ATTRIBLEVEL_SEPERATOR], s: " ", s:Charm_TypeName[GetPlayerItemSyncValue(DND_SYNC_ITEMSUBTYPE, topboxid, -1, source)], s:" Charm"; 
				HUDMSG_PLAIN | HUDMSG_FADEOUT, RPGMENUINVENTORYID - 3 * MAX_INVENTORY_BOXES - 2, CR_WHITE, bx, by - 40.0, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA
			);
		}
		i = GetPlayerItemSyncValue(DND_SYNC_ITEMSATTRIBCOUNT, topboxid, -1, source);
		for(j = 0; j < i; ++j) {
			temp = GetPlayerItemSyncValue(DND_SYNC_ITEMATTRIBUTES_ID, topboxid, j, source);
			val = GetPlayerItemSyncValue(DND_SYNC_ITEMATTRIBUTES_VAL, topboxid, j, source);
			if(val > 0)
				HudMessage(s:"+ ", d:val, s:Inv_Attribute_Names[temp]; HUDMSG_PLAIN | HUDMSG_FADEOUT, RPGMENUINVENTORYID - 3 * MAX_INVENTORY_BOXES - 3 - j, CR_WHITE, bx, by + 24.0 * j, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA);
			else
				HudMessage(s:"- ", d:val, s:Inv_Attribute_Names[temp]; HUDMSG_PLAIN | HUDMSG_FADEOUT, RPGMENUINVENTORYID - 3 * MAX_INVENTORY_BOXES - 3 -  j, CR_WHITE, bx, by + 24.0 * j, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA);
		}
		SetHudClipRect(0, 0, 0, 0, 0);
	}
}

void ResetPlayerInventory(int pnum) {
	for(int i = 0; i < MAX_INVENTORY_BOXES; ++i) {
		PlayerInventoryList[pnum][i].item_type = DND_ITEM_NULL;
		PlayerInventoryList[pnum][i].width = 0;
		PlayerInventoryList[pnum][i].height = 0;
		PlayerInventoryList[pnum][i].item_image = 0;
		PlayerInventoryList[pnum][i].item_type = 0;
		PlayerInventoryList[pnum][i].item_subtype = 0;
		PlayerInventoryList[pnum][i].item_level = 0;
		PlayerInventoryList[pnum][i].topleftboxid = 0;
		for(int j = 0; j < PlayerInventoryList[pnum][i].attrib_count; ++j) {
			PlayerInventoryList[pnum][i].attributes[j].attrib_id = 0;
			PlayerInventoryList[pnum][i].attributes[j].attrib_val = 0;
		}
		PlayerInventoryList[pnum][i].attrib_count = 0;
	}
}

void ResetTradeViewList(int pnum) {
	for(int i = 0; i < MAX_INVENTORY_BOXES; ++i) {
		TradeViewList[pnum][i].item_type = DND_ITEM_NULL;
		TradeViewList[pnum][i].width = 0;
		TradeViewList[pnum][i].height = 0;
		TradeViewList[pnum][i].item_image = 0;
		TradeViewList[pnum][i].item_type = 0;
		TradeViewList[pnum][i].item_subtype = 0;
		TradeViewList[pnum][i].item_level = 0;
		TradeViewList[pnum][i].topleftboxid = 0;
		for(int j = 0; j < TradeViewList[pnum][i].attrib_count; ++j) {
			TradeViewList[pnum][i].attributes[j].attrib_id = 0;
			TradeViewList[pnum][i].attributes[j].attrib_val = 0;
		}
		TradeViewList[pnum][i].attrib_count = 0;
	}
}

void ResetFieldInventory() {
	for(int i = 0; i < MAX_INVENTORIES_ON_FIELD; ++i) {
		Inventories_On_Field[i].item_type = DND_ITEM_NULL;
		Inventories_On_Field[i].width = 0;
		Inventories_On_Field[i].height = 0;
		Inventories_On_Field[i].item_image = 0;
		Inventories_On_Field[i].item_type = 0;
		Inventories_On_Field[i].item_subtype = 0;
		Inventories_On_Field[i].item_level = 0;
		Inventories_On_Field[i].topleftboxid = 0;
		for(int j = 0; j < Inventories_On_Field[i].attrib_count; ++j) {
			Inventories_On_Field[i].attributes[j].attrib_id = 0;
			Inventories_On_Field[i].attributes[j].attrib_val = 0;
		}
		Inventories_On_Field[i].attrib_count = 0;
	}
}

#endif
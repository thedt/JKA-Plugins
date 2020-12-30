#pragma once
#include "jka/game/q_shared.h"
#include "jka/game/g_local.h"

typedef struct p_playerInfo_s {
	// client data
	int bounty;
	int bountyOn;
	int bountyCr;
	int killCount;
	int lastKill;
	int conKillCount;
	int isPro;
	int powerupCd;
	int gotoClientCd;
	int lastCmd;
	int msgCount;
	int lastMsg;
	int msgStatus;
	int chatCmds;
	int oddball;
	int check;
	int regenSlowed;
	int pwShield;
	// skill data
	int fastRegen;
	int * cds;
	char* pinkey;
	int Ad_delay;
	int rapid_fire;
	int rapid_delay;
	qboolean melee;
	qboolean skybreaker;
	qboolean na1;
	qboolean na2;
	qboolean na3;
	int na1delay;
	int na2delay;
	int na3delay;

	//qboolean rapid_fire;
	qboolean force_reg;
	int builder;
	int builder_lvl;
	int frozen;
	int slap;
	int lockname;
	char* oldname;
	int blimit;
	char lastphase[MAX_STRING_CHARS];
	qboolean blocked;
	int chattimer;
	int chattimes;
	int otakutimer;

	int tagged;
	int tagged_delay;
	int tagcount;
	int tagged_before;

	int laser_tagged;
	int laser_tagged_delay;
	int laser_tagcount;
	int laser_tagged_before;

	bool buylevel;

	int blocknum[MAX_STRING_CHARS];
	char* blockaccount;
} p_playerInfo_t;

//typedef struct plugin_s
//{
//	manager_t* manager;
//	store_t* store;
//}plugin_t;
//
//typedef struct manager_s
//{
//
//}manager_t;
//
//typedef struct store_s
//{
//
//}store_t;
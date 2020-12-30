#include "XG-Header.h"
//#include "XG-Function.h"
#include <cstdint>

int clientCheck = 0; // global variable to determine if cliCheck is being used or not
Auths_GetPlayer* Auths_GetPlayerRank = (Auths_GetPlayer*) AUTHSGETPLAYERRANK;
Auths_Get* Auths_GetRank = (Auths_Get*) AUTHSGETRANK;
G_UseTargets2define* G_UseTargets3 = (G_UseTargets2define*) USETARGETS2;
LmdEntFromNum* GetEnt = (LmdEntFromNum*) LMDENTFROMNUM;
G_Findz* Lmd_Find = (G_Findz*) G_FIND_ENT;
// void (*Lmd_SetAnim)(gentity_t* ent, int setAnimParts, int anim, int setAnimFlags, int blendTime) = (void(*)(gentity_t*, int, int, int, int))LMDSETANIM;
level_locals_t* pLevel = (level_locals_t*) LEVELSTRUCT;
// spawn_t* Lmd_Spawns = (spawn_t*) SPAWNS;
// Account_t* Lmd_Accounts = (Account_t*) 0x20E03D70;
void (*BG_SetAnim)(gentity_t* ent, int setAnimParts, int anim, int setAnimFlags, int blendTime) = (void(*)(gentity_t*, int, int, int, int))LMDSETANIM;
gentity_t* (*Lmd_TempEntity)(vec3_t origin,int event) = (gentity_t*(*)(vec3_t,int))LMDTEMPENT;
Account_t* (*Accounts_GetByusername)(const char* name) = (Account_t*(*)(const char*))LMDACCFROMNAME;
void (*Lmd_TestLine)(vec3_t start, vec3_t end, int color, int time) = (void(*)(vec3_t, vec3_t, int, int)) LMDTESTLINE;
void (*Lmd_TeleportPlayer)( gentity_t *player, vec3_t origin, vec3_t angles ) = (void(*)(gentity_t*, vec3_t, vec3_t)) TELEPORT;
gentity_t* (*Lmd_PlayEffect)(int fxID, vec3_t org, vec3_t ang) = (gentity_t*(*)(int, vec3_t, vec3_t)) PLAYEFFECT;
int (*Lmd_NumFromName)(gentity_t* ent, const char* name) = (int(*)(gentity_t*, const char*))NUMFROMNAME;
void (*Lmd_Edit)(SpawnData_t* ent, const char* key, const char* value) = (void(*)(SpawnData_t*, const char*, const char*))EDIT;
void (*Lmd_PlayerDie)( gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int meansOfDeath ) = (void(*)(gentity_t*, gentity_t*, gentity_t*, int, int))PLAYERDIE;
int (*Lmd_SetSD)(gentity_t* ent, SpawnData_t* spawnData) = (int(*)(gentity_t*, SpawnData_t*)) RESETENT;
void (*Lmd_Damage)( gentity_t *targ, gentity_t *inflictor, gentity_t *attacker,
				   vec3_t dir, vec3_t point, int damage, int dflags, int mod ) = (void(*)(gentity_t*, gentity_t*, gentity_t*, vec3_t, vec3_t, int, int, int)) DAMAGE;
qboolean (*Lmd_RadiusDamage)( vec3_t origin, gentity_t *attacker, float damage, float radius,
							 gentity_t *ignore, gentity_t *missile, int mod) = (qboolean(*)(vec3_t, gentity_t*, float, float, gentity_t*, gentity_t*, int)) RADIUSDAMAGE;
int (*Lmd_GetProf)( Account_t* acc ) = (int(*)(Account_t*)) GETPROF; // 1 for god, 2 for bot, 3 for force user (call jedisith), 4 for merc
int (*Lmd_JediSith)( Account_t* acc ) = (int(*)(Account_t*)) JEDISITH; // 1 for jedi, 2 for sith
void (*Lmd_SecCode)( Account_t* acc, int value ) = (void(*)(Account_t*, int)) SECCODE;
void (*Lmd_SetLevel)( Account_t* acc, int value ) = (void(*)(Account_t*, int)) SETLEVEL;
void (*Lmd_SetValueForKey)( char *s, const char *key, const char *value ) = (void(*)(char*, const char*, const char*)) SETVALUEFORKEY;
void (*Lmd_RemoveKey)( char *s, const char *key ) = (void(*)(char*, const char*)) REMOVEKEY;
qboolean (*Lmd_SpawnString)( const char *key, const char *defaultString, char **out ) = (qboolean(*)(const char*, const char*, char**)) SPAWNSTRING;
char* (*Lmd_NewString)( const char *string ) = (char*(*)(const char*)) NEWSTRING;
SpawnData_t* (*Lmd_SDFromString)( const char *str ) = (SpawnData_t*(*)(const char*)) SDFROMSTRING;
gentity_t* (*Lmd_EntFromSD)( SpawnData_t* sd ) = (gentity_t*(*)(SpawnData_t*)) ENTFROMSD;
void (*Lmd_FreeEntity)(gentity_t* ed) = (void(*)(gentity_t*)) FREEENTITY;
char * (*Lmd_ValueForKey)( const char *s, const char *key ) = (char*(*)(const char*, const char*)) VALUEFORKEY;
//char* (*Lmd_SkillValue)( Account_t* acc, const char *skill ) = (char*(*)(Account_t*, const char*)) SKILLVALUE;
//void (*Lmd_SetSkill)( Account_t* acc, const char *skill, const char *val ) = (void(*)(Account_t*, const char*, const char*)) SETSKILL;
//int (*Lmd_GetSkill)( gentity_t* ent, int prof, int skillID ) = (int(*)(gentity_t*, int, int)) GETSKILL;
//void (*Lmd_EditSkill)( Account_t* acc, int prof, int skillID, int level ) = (void(*)(Account_t*, int, int, int)) EDITSKILL;

int(*G2API_AddBolt)( void *ghoul2, int modelIndex, const char *boneName ) = (int(*)(void*,int,const char*)) LMDADDBOLT;
qboolean(*G2API_GetBoltMatrix)( void *ghoul2, const int modelIndex, const int boltIndex, mdxaBone_t *matrix, const vec3_t angles, const vec3_t position, const int frameNum, qhandle_t *modelList, vec3_t scale ) = (qboolean(*) ( void*, const int, const int, mdxaBone_t*, const vec3_t, const vec3_t, const int, qhandle_t*, vec3_t)) LMDGETBOLT;
void(*LinkEntity)( sharedEntity_t *ent ) = (void(*)(sharedEntity_t*)) LMDLINKENT;

void (*WP_ForcePowerStart)( gentity_t *self, forcePowers_t forcePower, int overrideAmt ) = (void(*)(gentity_t*,forcePowers_t,int)) STARTFORCE;

int (*Lmd_deaths)(gentity_t *self) = (int(*)(gentity_t*)) DEATHS;
gentity_t *(*Lmd_Spawn)() = (gentity_t*(*)(void)) GSPAWN;
void (*RegisterSpawnableEntity)(char *name, void(*spawn)(gentity_s *), int* logical, entityInfo_t *info) = (void(*)(char*, void(*)(gentity_s*), int*, entityInfo_t*)) REGENT;
int(*Accounts_Prof_GetLevel)(Account_s *acc) = (int(*)(Account_s *)) ACCPROLEVEL;

//void (*Lmd_EntitySound)( gentity_t *ent, int channel, int soundIndex ) = (void(*)(gentity_t*,int,int)) LMDGSOUND;
/* skill ids:
jedi				Merc
---------------------------------
heal -> 0			weapons -> 0
jump -> 1			fuel -> 1
speed -> 2			weapongrip -> 2
push -> 3			ysalamiri -> 3
pull -> 4			ammo -> 4
mindtrick -> 5		stashrange -> 5
grip -> 6			melee -> 6
lightning -> 7		sheild -> 7
rage -> 8			flameburst -> 8
protect -> 9
absorb -> 10
teamheal -> 11
energize -> 12
drain -> 13
seeing -> 14
saber attack -> 15
saber defend -> 16
saber throw -> 17 */

string StringPadding(string original, size_t charCount )
{
	original.resize( charCount, ' ' );
	return original;
}

void Manager_list()
{
	g_syscall(G_PRINT,JASS_VARARGS("^5Usage^7: /manager\n"));
	g_syscall(G_PRINT,JASS_VARARGS("^6[^7<^2list^7> list all commands\n"));
	g_syscall(G_PRINT,JASS_VARARGS("<^3Dlist^7> list all commands and values\n"));
	g_syscall(G_PRINT,JASS_VARARGS("<^5command^7>^6]^7 command to select\n"));
	g_syscall(G_PRINT,JASS_VARARGS("^5[^7<^5info^7>  info on the command\n"));
	g_syscall(G_PRINT,JASS_VARARGS("<^5remove^7> remove command from list\n"));
	g_syscall(G_PRINT,JASS_VARARGS("<^5file^7> what .qq file to open\n"));
	g_syscall(G_PRINT,JASS_VARARGS("<^5script^7> what .cfg file to run\n"));
	g_syscall(G_PRINT,JASS_VARARGS("<^5anim^7> set an animation to the command\n"));

	g_syscall(G_PRINT,JASS_VARARGS("<^5animtime^7> set an animation timer to the command\n"));
	g_syscall(G_PRINT,JASS_VARARGS("<^5custom^7> if this is a custom command set this flag\n"));
	g_syscall(G_PRINT,JASS_VARARGS("<^5setlevel^7> only one admin level can use it\n"));
	g_syscall(G_PRINT,JASS_VARARGS("<^5adminlevel^7> sets the minimum admin level required\n"));
	g_syscall(G_PRINT,JASS_VARARGS("<^5map^7> only on this map is it disabled\n"));
	g_syscall(G_PRINT,JASS_VARARGS("<^5noduel^7> if set this command is not allowed in duels\n"));
	g_syscall(G_PRINT,JASS_VARARGS("<^5gametype^7> only on this gametype is it disabled\n"));
	g_syscall(G_PRINT,JASS_VARARGS("<^5errormsg^7>^5]^7 if info is invalid set a error message\n"));
	g_syscall(G_PRINT,JASS_VARARGS("<^5message^7>^5]^7 sets a message\n"));
	g_syscall(G_PRINT,JASS_VARARGS("^4[^7<^1value^7>^4]^7 set the value for the flag\n"));
}

void Manager_list(int arg0)
{
	g_syscall(G_SEND_SERVER_COMMAND,arg0,JASS_VARARGS("print \"^5Usage^7: /manager\n\""));
	g_syscall(G_SEND_SERVER_COMMAND,arg0,JASS_VARARGS("print \"^6[^7<^2list^7> list all commands\n\""));
	g_syscall(G_SEND_SERVER_COMMAND,arg0,JASS_VARARGS("print \"<^5command^7>^6]^7 command to select\n\""));
	g_syscall(G_SEND_SERVER_COMMAND,arg0,JASS_VARARGS("print \"^5[^7<^5info^7>  info on the command\n\""));
	g_syscall(G_SEND_SERVER_COMMAND,arg0,JASS_VARARGS("print \"<^5remove^7> remove command from list\n\""));

	g_syscall(G_SEND_SERVER_COMMAND,arg0,JASS_VARARGS("print \"<^5animtime^7> set an animation timer to the command\n\""));
	g_syscall(G_SEND_SERVER_COMMAND,arg0,JASS_VARARGS("print \"<^5setanim^7> set an animation to the command permanently\n\""));
	g_syscall(G_SEND_SERVER_COMMAND,arg0,JASS_VARARGS("print \"<^5anim^7> set an animation to the command\n\""));
	g_syscall(G_SEND_SERVER_COMMAND,arg0,JASS_VARARGS("print \"<^5custom^7> if this is a custom command set this flag\n\""));
	g_syscall(G_SEND_SERVER_COMMAND,arg0,JASS_VARARGS("print \"<^5setlevel^7> only one admin level can use it\n\""));
	g_syscall(G_SEND_SERVER_COMMAND,arg0,JASS_VARARGS("print \"<^5adminlevel^7> sets the minimum admin level required\n\""));
	g_syscall(G_SEND_SERVER_COMMAND,arg0,JASS_VARARGS("print \"<^5accountlevel^7> sets the minimum account level required\n\""));
	g_syscall(G_SEND_SERVER_COMMAND,arg0,JASS_VARARGS("print \"<^5profession^7> if this is set only the profession targeted can use this\n\""));
	g_syscall(G_SEND_SERVER_COMMAND,arg0,JASS_VARARGS("print \"<^5fx^7> Effect to play\n\""));
	g_syscall(G_SEND_SERVER_COMMAND,arg0,JASS_VARARGS("print \"<^5map^7> only on this map is it disabled\n\""));
	g_syscall(G_SEND_SERVER_COMMAND,arg0,JASS_VARARGS("print \"<^5scale^7> scale a player\n\""));
	g_syscall(G_SEND_SERVER_COMMAND,arg0,JASS_VARARGS("print \"<^5scaletimer^7> set timer for the time scaled\n\""));
	g_syscall(G_SEND_SERVER_COMMAND,arg0,JASS_VARARGS("print \"<^5noduel^7> if set this command is not allowed in duels\n\""));
	g_syscall(G_SEND_SERVER_COMMAND,arg0,JASS_VARARGS("print \"<^5gametype^7> only on this gametype is it disabled\n\""));
	g_syscall(G_SEND_SERVER_COMMAND,arg0,JASS_VARARGS("print \"<^5errormsg^7>^5]^7 if info is invalid set an error message\n\""));
	g_syscall(G_SEND_SERVER_COMMAND,arg0,JASS_VARARGS("print \"<^5message^7>^5]^7 sets a message\n\""));
	g_syscall(G_SEND_SERVER_COMMAND,arg0,JASS_VARARGS("print \"^4[^7<^1value^7>^4]^7 set the value for the flag\n\""));
}

char* rank_ratio(gentity_t* ent)
{
	int deaths = ent->client->pers.Lmd.account->Accounts_StatData->dealths;
	int kills = ent->client->pers.Lmd.account->Accounts_StatData->kills;

	if(deaths <= 0)
	{
		deaths += 1;
	}

	if(kills <= 0)
	{
		kills += 1;
	}

	double ratio = kills/deaths;

	if(ratio > 30)
		return "God Tier";

	else if(ratio > 25)
		return "Pro Tier";

	else if(ratio > 15)
		return "Good Tier";

	else if(ratio > 10)
		return "Okay Tier";

	else if(ratio > 5)
		return "Dirt Tier";

	else
		return "Shit Tier";
}

int rank_credits(gentity_t*ent)
{
	int deaths = ent->client->pers.Lmd.account->Accounts_StatData->dealths;
	int kills = ent->client->pers.Lmd.account->Accounts_StatData->kills;

	if(deaths <= 0)
	{
		deaths += 1;
	}

	if(kills <= 0)
	{
		kills += 1;
	}

	double ratio = kills/deaths;

	if(ratio > 30)
		return 30;

	else if(ratio > 25)
		return 25;

	else if(ratio > 15)
		return 20;

	else if(ratio > 10)
		return 15;

	else if(ratio > 5)
		return 10;

	else
		return PlayersInGame();
}

void Add_Ammo (gentity_t *ent, int weapon, int count)
{
	ammoData_t ammoData[AMMO_MAX];
	int max = ammoData[weapon].max;

	max = count;

	if ( ent->client->ps.ammo[weapon] < max )
	{
		ent->client->ps.ammo[weapon] += count;
		if ( ent->client->ps.ammo[weapon] > max )
		{
			ent->client->ps.ammo[weapon] = max;
		}
	}
}

void bio_weapons(gentity_t* ent,int weapon)
{
	int cost = ent->client->ps.ammo[weaponData[weapon].energyPerShot];
	int AltCost = ent->client->ps.ammo[weaponData[weapon].altEnergyPerShot];
	ent->client->ps.ammo[weaponData[weapon].energyPerShot] = 0;
	ent->client->ps.ammo[weaponData[weapon].altEnergyPerShot] = 0;

	if(ent->client->ps.stats[STAT_HEALTH] != ent->client->ps.ammo[weaponData[weapon].ammoIndex])
	{
		ent->client->ps.stats[STAT_HEALTH] = ent->client->ps.ammo[weaponData[weapon].ammoIndex];
	}
}

#define	MAX_VA_STRING	32000
#define MAX_VA_BUFFERS 4

int Q_vsnprintf(char *str, size_t size, const char *format, va_list ap)
{
	int retval;

	retval = _vsnprintf(str, size, format, ap);

	if(retval < 0 || retval == size)
	{
		str[size - 1] = '\0';
		return size;
	}

	return retval;
}

int om_sprintf( char *dest, int size, const char *fmt, ...) {
	int		len;
	va_list		argptr;

	va_start (argptr,fmt);
	len = Q_vsnprintf(dest, size, fmt, argptr);
	va_end (argptr);

	if(len >= size)
		Com_Printf("Com_sprintf: Output length %d too short, require %d bytes.\n", size, len + 1);

	return len;
}

char	*vtos( const vec3_t v ) {
	static	int		index;
	static	char	str[8][32];
	char	*s;

	// use an array so that multiple vtos won't collide
	s = str[index];
	index = (index + 1)&7;

	om_sprintf (s, 32, "%i %i %i", (int)v[0], (int)v[1], (int)v[2]);

	return s;
}

void G_SetOrigin( gentity_t *ent, vec3_t origin ) {
	VectorCopy( origin, ent->s.pos.trBase );
	ent->s.pos.trType = TR_STATIONARY;
	ent->s.pos.trTime = 0;
	ent->s.pos.trDuration = 0;
	VectorClear( ent->s.pos.trDelta );

	VectorCopy( origin, ent->r.currentOrigin );
}

void G_SetAngles( gentity_t *ent, vec3_t angles )
{
	VectorCopy( angles, ent->r.currentAngles );
	VectorCopy( angles, ent->s.angles );
	VectorCopy( angles, ent->s.apos.trBase );
}

char* colorfilter( const char* in, char *out )
{
	int i = 0;
	int r = 0;

	while (in[i])
	{
		if (i >= 1024-1)
		{ //the ui truncates the name here..
			break;
		}

		if (in[i] == '^')
		{
			if (in[i+1] >= 48 && //'0'
				in[i+1] <= 57) //'9'
			{ //only skip it if there's a number after it for the color
				i += 2;
				continue;
			}
			else
			{ //just skip the ^
				i++;
				continue;
			}
		}

		out[r] = in[i];
		r++;
		i++;
	}
	out[r] = 0;
	return out;
}

// checks if a given player can fire a playercheck or not
int cliCheck(gentity_t* ent, const char* checkString)
{
	if (!ent) return 0;
	if (!ent->client) return 0;
	if (!ent->client->pers.connected) return 0;
	if (ent->s.number >= 32) return 0;
	if (strlen(checkString) < 1) return 1;
	gentity_t* chk = sp(JASS_VARARGS("classname,lmd_playercheck,origin,0 0 0,%s", checkString));
	gentity_t* ass = sp("classname,lmd_plugin,origin,0 0 0,mode,check,param,1,targetname,aee56f8e1a95a20a35444d4ce6f9598a");
	Lmd_Edit(chk->spawnData, "target", "aee56f8e1a95a20a35444d4ce6f9598a");
	Lmd_SetSD(chk, chk->spawnData);
	clientCheck = 1;
	chk->use(chk, ent, ent);
	clientCheck = 0;
	Lmd_FreeEntity(chk);
	Lmd_FreeEntity(ass);
	if (p_playerInfo[ent->s.number].check)
	{
		p_playerInfo[ent->s.number].check = 0;
		return 1;
	}
	else
	{
		return 0;
	}
}

void Lmd_playfx(gentity_t* ent, char* fx)
{
	vec3_t angles, storage;
	VectorCopy(ent->client->ps.origin, storage);
	storage[2] += 25;
	Lmd_PlayEffect(G_EffectIndex(fx), storage, ent->client->ps.viewangles);
}

char* Lmd_KeyValue(SpawnData_t* spawnData, const char* key)
{
	unsigned int i = 0;
	while (i < spawnData->keys.count)
	{
		if (!_stricmp((const char*)spawnData->keys.pairs[i].key, key))
		{
			return spawnData->keys.pairs[i].value;
		}
		i++;
	}
	return "";
}

/*void G_SetOrigin( gentity_t *ent, vec3_t origin ) {
VectorCopy( origin, ent->s.pos.trBase );
//ent->s.pos.trTime = 0;
//ent->s.pos.trType = TR_STATIONARY;
//ent->s.pos.trDuration = 0;
//ent->r.contents = 1;
//VectorClear( ent->s.pos.trDelta );

VectorCopy( origin, ent->r.currentOrigin);
//VectorCopy( origin, ent->s.origin );
}*/

void Lmd_EntitySound( gentity_t *ent, int channel, int soundIndex ) {
	gentity_t	*te;

	te = Lmd_TempEntity( ent->r.currentOrigin, EV_ENTITY_SOUND );
	te->s.eventParm = soundIndex;
	te->s.clientNum = ent->s.number;
	te->s.trickedentindex = channel;
}

void Jetpack_Off(gentity_t *ent)
{ //create effects?
	assert(ent && ent->client);

	if (!ent->client->jetPackOn)
	{ //aready off
		return;
	}

	ent->client->jetPackOn = qfalse;
}

void Lmd_SetOrigin( gentity_t *ent, vec3_t origin ) {
	VectorCopy( origin, ent->s.pos.trBase );
	ent->s.pos.trType = TR_STATIONARY;
	ent->s.pos.trTime = 0;
	ent->s.pos.trDuration = 0;
	VectorClear( ent->s.pos.trDelta );

	VectorCopy( origin, ent->r.currentOrigin );
}

vec_t VectorNormalize( vec3_t v ) {
	float	length, ilength;

	length = v[0]*v[0] + v[1]*v[1] + v[2]*v[2];
	length = sqrt (length);
	if ( length ) {
		ilength = 1/length;
		v[0] *= ilength;
		v[1] *= ilength;
		v[2] *= ilength;
	}

	return length;
}

void AngleVectors( const vec3_t angles, vec3_t forward, vec3_t right, vec3_t up) {
	float		angle;
	static float		sr, sp, sy, cr, cp, cy;
	// static to help MS compiler fp bugs

	angle = angles[YAW] * (M_PI*2 / 360);
	sy = sin(angle);
	cy = cos(angle);
	angle = angles[PITCH] * (M_PI*2 / 360);
	sp = sin(angle);
	cp = cos(angle);
	angle = angles[ROLL] * (M_PI*2 / 360);
	sr = sin(angle);
	cr = cos(angle);

	if (forward)
	{
		forward[0] = cp*cy;
		forward[1] = cp*sy;
		forward[2] = -sp;
	}
	if (right)
	{
		right[0] = (-1*sr*sp*cy+-1*cr*-sy);
		right[1] = (-1*sr*sp*sy+-1*cr*cy);
		right[2] = -1*sr*cp;
	}
	if (up)
	{
		up[0] = (cr*sp*cy+-sr*-sy);
		up[1] = (cr*sp*sy+-sr*cy);
		up[2] = cr*cp;
	}
}

vec_t S_VectorLength( const vec3_t v ) {
	return (vec_t)sqrt (v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
}

vec_t S_Distance( const vec3_t p1, const vec3_t p2 ) {
	vec3_t	v;

	VectorSubtract (p2, p1, v);
	return S_VectorLength( v );
}

void trap_SetConfigstring( int num, const char *string ) {
	g_syscall( G_SET_CONFIGSTRING, num, string );
}

void trap_GetConfigstring( int num, char *buffer, int bufferSize ) {
	g_syscall( G_GET_CONFIGSTRING, num, buffer, bufferSize );
}

void ForceCvar( int clientNum, const char *cvar, const char *value )
{
	bool wipe = true;
	char buf[1024];
	trap_GetConfigstring(1, buf, 1024);
	if (strstr(buf, JASS_VARARGS("\\%s\\", cvar))) { wipe = false; }
	Lmd_SetValueForKey(buf, cvar, value);
	g_syscall(G_SEND_SERVER_COMMAND, clientNum, JASS_VARARGS("cs 1 \"%s\"", buf));
	if (wipe)
	{
		Lmd_RemoveKey(buf, cvar);
		g_syscall(G_SEND_SERVER_COMMAND, clientNum, JASS_VARARGS("cs 1 \"%s\"", buf));
	}
	return;
}

static int G_FindConfigstringIndex( const char *name, int start, int max, qboolean create ) {
	int		i;
	char	s[MAX_STRING_CHARS];

	if ( !name || !name[0] ) {
		return 0;
	}

	for ( i=1 ; i<max ; i++ ) {
		trap_GetConfigstring( start + i, s, sizeof( s ) );
		if ( !s[0] ) {
			break;
		}
		if ( !strcmp( s, name ) ) {
			return i;
		}
	}

	if ( !create ) {
		return 0;
	}

	trap_SetConfigstring( start + i, name );

	return i;
}

int G_ModelIndex( const char *name ) {
	return G_FindConfigstringIndex (name, CS_MODELS, MAX_MODELS, qtrue);
}

int G_EffectIndex( const char *name )
{
	return G_FindConfigstringIndex (name, CS_EFFECTS, MAX_FX, qtrue);
}

int G_SoundIndex( const char *name ) {
	assert(name && name[0]);
	return G_FindConfigstringIndex (name, CS_SOUNDS, MAX_SOUNDS, qtrue);
}

int PIReset(int clientNum)
{
	int i = 0;
	if (clientNum >= 32) return 0;
	// reset the playerInfo struct for the client slot
	p_playerInfo[clientNum].bounty = 0;
	p_playerInfo[clientNum].bountyOn = 0;
	p_playerInfo[clientNum].bountyCr = 0;
	p_playerInfo[clientNum].fastRegen = 0;
	p_playerInfo[clientNum].gotoClientCd = 0;
	p_playerInfo[clientNum].isPro = 0;
	p_playerInfo[clientNum].killCount = 0;
	p_playerInfo[clientNum].lastKill = 0;
	p_playerInfo[clientNum].conKillCount = 0;
	p_playerInfo[clientNum].powerupCd = 0;
	p_playerInfo[clientNum].lastCmd = 0;
	p_playerInfo[clientNum].lastMsg = 0;
	p_playerInfo[clientNum].msgCount = 0;
	p_playerInfo[clientNum].msgStatus = 0;
	p_playerInfo[clientNum].oddball = 0;
	p_playerInfo[clientNum].chatCmds = 0;
	p_playerInfo[clientNum].regenSlowed = 0;
	p_playerInfo[clientNum].pwShield = 0;
	p_playerInfo[clientNum].check = 0;
	if (p_playerInfo[clientNum].cds)
		delete[] p_playerInfo[clientNum].cds;
	p_playerInfo[clientNum].cds = new int[NUM_SKILLS];
	while (i < NUM_SKILLS)
	{
		p_playerInfo[clientNum].cds[i] = 0;
		i++;
	}
	return 1;
}

char *str_replace( char *string, char *substr, char *replacement ){
	char *tok = NULL;
	char *newstr = NULL;
	if (!strstr(string,substr)) return string;
	while (strstr( string, substr ))
	{
		tok = NULL;
		newstr = NULL;
		tok = strstr( string, substr );
		if( tok == NULL ) return _strdup( string );
		newstr = (char *) malloc( strlen( string ) - strlen( substr ) + strlen( replacement ) + 1 );
		if( newstr == NULL ) return NULL;
		memcpy( newstr, string, tok - string );
		memcpy( newstr + (tok - string), replacement, strlen( replacement ) );
		memcpy( newstr + (tok - string) + strlen( replacement ), tok + strlen( substr ), strlen( string ) - strlen( substr ) - ( tok - string ) );
		memset( newstr + strlen( string ) - strlen( substr ) + strlen( replacement ), 0, 1 );
		strcpy(string,newstr);
	}
	return newstr;
}

//test this @@ex
qboolean Activity_Check( gentity_t *ent,int timer ) {
	ent->client->inactivityTime = pLevel->time + 60 * 1000;
	ent->client->inactivityWarning = qfalse;
	if ( ent->client->pers.cmd.forwardmove || ent->client->pers.cmd.rightmove || ent->client->pers.cmd.upmove ||(ent->client->pers.cmd.buttons & (BUTTON_ATTACK|BUTTON_ALT_ATTACK)) ) {
		ent->client->inactivityTime = pLevel->time + timer * 1000;
		ent->client->inactivityWarning = qfalse;
	} else if ( !ent->client->pers.localClient ) {
		if ( pLevel->time > ent->client->inactivityTime ) {
			//trap_DropClient( client - level.clients, "Dropped due to inactivity" );
			return qtrue;
		}
		if ( pLevel->time > ent->client->inactivityTime - 10000 && !ent->client->inactivityWarning ) {
			ent->client->inactivityWarning = qtrue;
			//trap_SendServerCommand( client - level.clients, "cp \"Ten seconds until inactivity drop!\n\"" );
		}
	}
	return qfalse;
}

gentity_t *G_PlayEffect(int fxID, vec3_t org, vec3_t ang)
{
	gentity_t	*te;

	te = Lmd_TempEntity( org, EV_PLAY_EFFECT );
	VectorCopy(ang, te->s.angles);
	VectorCopy(org, te->s.origin);
	te->s.eventParm = fxID;

	return te;
}

gentity_t *G_PlayEffectID(const int fxID, vec3_t org, vec3_t ang)
{ //play an effect by the G_EffectIndex'd ID instead of a predefined effect ID
	gentity_t	*te;

	te = Lmd_TempEntity( org, EV_PLAY_EFFECT_ID );
	VectorCopy(ang, te->s.angles);
	VectorCopy(org, te->s.origin);
	te->s.eventParm = fxID;

	if (!te->s.angles[0] &&
		!te->s.angles[1] &&
		!te->s.angles[2])
	{ //play off this dir by default then.
		te->s.angles[1] = 1;
	}

	return te;
}

char* ConcatArgs( int start ) {
	int		i, c, tlen;
	static char	line[MAX_STRING_CHARS];
	int		len;
	char	arg[MAX_STRING_CHARS];

	len = 0;
	c = g_syscall(G_ARGC);
	for ( i = start ; i < c ; i++ ) {
		g_syscall(G_ARGV, i, arg, sizeof( arg ) );
		tlen = strlen( arg );
		if ( len + tlen >= MAX_STRING_CHARS - 1 ) {
			break;
		}
		memcpy( line + len, arg, tlen );
		len += tlen;
		if ( i != c - 1 ) {
			line[len] = ' ';
			len++;
		}
	}

	line[len] = 0;

	return line;
}

int PlayersInGame(void)
{
	int i = 0;
	gentity_t *ent;
	int pl = 0;

	while (i < MAX_CLIENTS)
	{
		ent = ENT_FROM_NUM(i);

		if (ent && ent->client && ent->client->pers.connected == CON_CONNECTED)
		{
			pl++;
		}

		i++;
	}

	return pl;
}

float PM_GroundDistance(gentity_t* ent)
{
	trace_t tr;
	vec3_t down;
	pmove_t	*pm;
	VectorCopy(ent->client->ps.origin, down);

	down[2] -= 4096;
	//pm->trace(&tr, ent->client->ps.origin, pm->mins, pm->maxs, down, ent->client->ps.clientNum, MASK_SOLID);

	VectorSubtract(ent->client->ps.origin, tr.endpos, down);

	return VectorLength(down);
}

void WP_ForcePowerRegenerate( gentity_t *self, int overrideAmt )
{ //called on a regular interval to regenerate force power.
	if ( !self->client )
	{
		return;
	}

	if ( overrideAmt )
	{ //custom regen amount
		self->client->ps.fd.forcePower += overrideAmt;
	}
	else
	{ //otherwise, just 1
		self->client->ps.fd.forcePower++;
	}

	if ( self->client->ps.fd.forcePower > self->client->ps.fd.forcePowerMax )
	{ //cap it off at the max (default 100)
		self->client->ps.fd.forcePower = self->client->ps.fd.forcePowerMax;
	}
}

void Powerup(gentity_t* ent,int powerup,int time)
{
	if (ent->client->ps.powerups[powerup] <= pLevel->time) {
		ent->client->ps.powerups[powerup] = pLevel->time + 1000*time;
	}else{
		ent->client->ps.powerups[powerup] += 1000*time;
	}
}

// just shorthands for credit modification
int GetCredits(gentity_t *to)
{
	return to->client->pers.Lmd.account->credits;
}

void SetCredits(gentity_t *to, int newCr)
{
	to->client->pers.Lmd.account->credits = newCr;
	to->client->pers.Lmd.account->modifiedTime = pLevel->time;
}

int numLines(FILE * pFile)
{
	int number_of_lines = 0;
	int ch = 0;
	do
	{
		ch = fgetc(pFile);
		if(ch == '\n')
			number_of_lines++;
	} while (ch != EOF);

	// last line doesn't end with a new line!
	// but there has to be a line at least before the last line
	// if(ch != '\n' && number_of_lines != 0)
	// number_of_lines++;
	rewind(pFile);
	return number_of_lines;
}

void bbox_f(int clientNum)
{
	char arg1[16];
	int entNum;
	trace_t tr;
	gentity_t* ent;
	vec3_t point1, point2, point3, point4, point5, point6, point7, point8;
	if (g_syscall(G_ARGC) < 2)
	{
		cliAimTrace(&tr, ENT_FROM_NUM(clientNum));
		if (tr.fraction < 1.0 && tr.entityNum != 1022) entNum = tr.entityNum;
		else return;
	}
	else
	{
		g_syscall(G_ARGV, 1, arg1, sizeof(arg1));
		entNum = atoi(arg1);
	}
	ent = ENT_FROM_NUM(entNum);

	if (ent == NULL) return;

	if (!ent->inuse)
		return;

	VectorCopy(ent->r.absmax, point1);
	VectorCopy(ent->r.absmin, point8);
	VectorSet(point2, point1[0], point8[1], point1[2]);
	VectorSet(point3, point8[0], point8[1], point1[2]);
	VectorSet(point4, point8[0], point1[1], point1[2]);
	VectorSet(point5, point8[0], point1[1], point8[2]);
	VectorSet(point6, point1[0], point1[1], point8[2]);
	VectorSet(point7, point1[0], point8[1], point8[2]);

	//top
	Lmd_TestLine(point1, point2, 0x00000ff, 5000);
	Lmd_TestLine(point2, point3, 0x00000ff, 5000);
	Lmd_TestLine(point3, point4, 0x00000ff, 5000);
	Lmd_TestLine(point4, point1, 0x00000ff, 5000);
	//botton
	Lmd_TestLine(point8, point5, 0x00000ff, 5000);
	Lmd_TestLine(point5, point6, 0x00000ff, 5000);
	Lmd_TestLine(point6, point7, 0x00000ff, 5000);
	Lmd_TestLine(point7, point8, 0x00000ff, 5000);
	//sides
	Lmd_TestLine(point3, point8, 0x00000ff, 5000);
	Lmd_TestLine(point6, point1, 0x00000ff, 5000);
	Lmd_TestLine(point2, point7, 0x00000ff, 5000);
	Lmd_TestLine(point4, point5, 0x00000ff, 5000);
}

static uint32_t	holdrand = 0x89abcdef; // 64 bit support for iojamp

int irand(int min, int max)
{
	int		result;

	assert((max - min) < MAX_GAMESTATE_CHARS);

	max++;
	holdrand = (holdrand * 214013L) + 2531011L;
	result = holdrand >> 17;
	result = ((result * (max - min)) >> 15) + min;
	return(result);
}

/*inline string to_string(int _Val)
	{	// convert int to string
	char _Buf[2 * _MAX_INT_DIG];

	_CSTD _TOSTRING(_Buf, "%d", _Val);
	return (string(_Buf));
	}*/

void ScalePlayer( gentity_t *self, int scale )
{
	float fc = scale / 100.0f;

	if (!self)
		return;

	if (!self->client)
		return;

	if ( !scale )
		fc = 1.0f;

	//if ( self->r.svFlags & SVF_BOT )
	self->client->ps.iModelScale = scale;
	self->modelScale[0] = self->modelScale[1] = self->modelScale[2] = fc;
}

int traceTarg(int clientNum)
{
	trace_t tr;
	vec3_t end, start, forward, maxs = {4, 4, 4}, mins = {-4, -4, -4};
	gclient_t* cl;
	cl = CLIENT_FROM_NUM( clientNum );
	AngleVectors( cl->ps.viewangles, forward, NULL, NULL );
	VectorSet(start, cl->ps.origin[0], cl->ps.origin[1], cl->ps.origin[2]+cl->ps.viewheight);
	VectorMA(start, Q3_INFINITE,forward,end);

	g_syscall(G_TRACE, &tr, start, mins, maxs, end, clientNum, MASK_SHOT);
	if( tr.fraction != 1 && tr.entityNum < ENTITYNUM_WORLD )
		return tr.entityNum;
	g_syscall(G_SEND_SERVER_COMMAND, clientNum, "print \"^1Target not found\n\"");
	return -1;
}

void cliAimTrace(trace_t* tr, gentity_t* ent, int distance, vec3_t mins, vec3_t maxs)
{
	vec3_t location, forward, storage;
	AngleVectors(ent->client->ps.viewangles, forward, NULL, NULL);
	VectorNormalize(forward);
	VectorCopy(ent->client->ps.origin, location);
	VectorCopy(ent->client->ps.origin, storage);
	storage[2] += 25;
	VectorMA(ent->client->ps.origin, distance, forward, location);
	g_syscall(G_TRACE, tr, storage, mins, maxs, location, ent->s.number, ent->clipmask);
}

void InitStructs()
{
	int i = 0;
	// initialize p_playerInfo array, required for unloading support
	while (i < 32)
	{
		PIReset(i);
		i++;
	}

	g_syscall(G_PRINT, "Structs initialized.\n");
}

void vectoangles( const vec3_t value1, vec3_t angles ) {
	float	forward;
	float	yaw, pitch;

	if ( value1[1] == 0 && value1[0] == 0 ) {
		yaw = 0;
		if ( value1[2] > 0 ) {
			pitch = 90;
		}
		else {
			pitch = 270;
		}
	}
	else {
		if ( value1[0] ) {
			yaw = ( atan2 ( value1[1], value1[0] ) * 180 / M_PI );
		}
		else if ( value1[1] > 0 ) {
			yaw = 90;
		}
		else {
			yaw = 270;
		}
		if ( yaw < 0 ) {
			yaw += 360;
		}

		forward = sqrt ( value1[0]*value1[0] + value1[1]*value1[1] );
		pitch = ( atan2(value1[2], forward) * 180 / M_PI );
		if ( pitch < 0 ) {
			pitch += 360;
		}
	}

	angles[PITCH] = -pitch;
	angles[YAW] = yaw;
	angles[ROLL] = 0;
}

void SetClientViewAngle( gentity_t *ent, vec3_t angle ) {
	int			i;

	// set the delta angle
	for (i=0 ; i<3 ; i++) {
		int		cmdAngle;

		cmdAngle = ANGLE2SHORT(angle[i]);
		ent->client->ps.delta_angles[i] = cmdAngle - ent->client->pers.cmd.angles[i];
	}
	VectorCopy( angle, ent->s.angles );
	VectorCopy (ent->s.angles, ent->client->ps.viewangles);
}


void target_heal_use(gentity_t* self, gentity_t* other, gentity_t* activator)
{
	activator->client->ps.stats[STAT_ARMOR] += atoi(self->target3);
	if (activator->client->ps.stats[STAT_MAX_HEALTH] < activator->client->ps.stats[STAT_ARMOR])
	{
		activator->client->ps.stats[STAT_ARMOR] = activator->client->ps.stats[STAT_MAX_HEALTH];
	}
	if (activator->client->ps.stats[STAT_ARMOR] < 0)
	{
		if (self->spawnflags & 1)
		{
			activator->health += activator->client->ps.stats[STAT_ARMOR];
			activator->client->ps.stats[STAT_HEALTH] += activator->client->ps.stats[STAT_ARMOR];
		}
		activator->client->ps.stats[STAT_ARMOR] = 0;
	}
	activator->health += atoi(self->target2);
	activator->client->ps.stats[STAT_HEALTH] += atoi(self->target2);
	if (activator->client->ps.stats[STAT_MAX_HEALTH] < activator->client->ps.stats[STAT_HEALTH])
	{
		activator->health = activator->client->ps.stats[STAT_MAX_HEALTH];
		activator->client->ps.stats[STAT_HEALTH] = activator->client->ps.stats[STAT_MAX_HEALTH];
	}
	if (activator->client->ps.stats[STAT_HEALTH] <= 0)
	{
		activator->flags &= ~FL_GODMODE;
		Lmd_PlayerDie(activator, activator, activator, 100000, MOD_SUICIDE);
	}
	if (atoi(self->target4) > 0)
	{
		int num = atoi(self->target4);
		for (int i = AMMO_BLASTER; i < AMMO_ROCKETS; i++)
			if (activator->client->ps.ammo[i] + num <= 600)
				activator->client->ps.ammo[i] += num;
	}

	return;
}

void sp_target_heal(gentity_t* self)
{
	Lmd_SpawnString("health", "", &self->target2);
	Lmd_SpawnString("armor", "", &self->target3);
	Lmd_SpawnString("ammo", "", &self->target4);
	self->use = target_heal_use;
}

#pragma once
#include "Jass/jassapi.h"
#include "XG-Structure.h"

#define NUM_SKILLS 7
#define NUM_BUILDABLES 2

#define sp(a) Lmd_EntFromSD(Lmd_SDFromString(a))
//extern sub_struct_t sub_struct; //a server struct array holding server side plugin information

extern gentity_t* g_gents;
extern int g_gentsize; 
extern gclient_t* g_clients;
extern int g_clientsize;

typedef void G_UseTargets2define(gentity_t *entity, gentity_t *activator, const char *string );
typedef gentity_t * LmdEntFromNum(int entNum);
typedef gentity_t * G_Findz(gentity_t *from, int fieldofs, const char *match);
typedef int Auths_GetPlayer(gentity_t *player);
typedef int Auths_Get(Account_t *acc);
typedef vec3_t vec3pair_t[2], matrix3_t[3];

extern p_playerInfo_t p_playerInfo[32];
extern int clientCheck;

extern eng_syscall_t g_syscall;
extern plugininfo_t g_plugininfo;
extern Auths_GetPlayer* Auths_GetPlayerRank;
extern Auths_Get* Auths_GetRank;
extern G_UseTargets2define* G_UseTargets3;
extern LmdEntFromNum* GetEnt;
extern G_Findz* Lmd_Find;
// void (*Lmd_SetAnim)(gentity_t* ent, int setAnimParts, int anim, int setAnimFlags, int blendTime) = (void(*)(gentity_t*, int, int, int, int))LMDSETANIM;
extern level_locals_t* pLevel;
// spawn_t* Lmd_Spawns = (spawn_t*) SPAWNS;
extern gentity_t* (*Lmd_Spawn)(void);
// Account_t* Lmd_Accounts = (Account_t*) 0x20E03D70;
extern Account_t* (*Accounts_GetByusername)(const char* name);
extern void (*Lmd_TestLine)(vec3_t start, vec3_t end, int color, int time);

extern int(*G2API_AddBolt)( void *ghoul2, int modelIndex, const char *boneName );
extern qboolean(*G2API_GetBoltMatrix)( void *ghoul2, const int modelIndex, const int boltIndex, mdxaBone_t *matrix, const vec3_t angles, const vec3_t position, const int frameNum, qhandle_t *modelList, vec3_t scale );

extern void (*Lmd_TeleportPlayer)( gentity_t *player, vec3_t origin, vec3_t angles );
extern gentity_t* (*Lmd_PlayEffect)(int fxID, vec3_t org, vec3_t ang);
extern int (*Lmd_NumFromName)(gentity_t* ent, const char* name);
extern void (*Lmd_Edit)(SpawnData_t* ent, const char* key, const char* value);
extern void (*Lmd_PlayerDie)( gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int meansOfDeath );
extern int (*Lmd_SetSD)(gentity_t* ent, SpawnData_t* spawnData);
extern void (*Lmd_Damage)( gentity_t *targ, gentity_t *inflictor, gentity_t *attacker,
			   vec3_t dir, vec3_t point, int damage, int dflags, int mod );
extern qboolean (*Lmd_RadiusDamage)( vec3_t origin, gentity_t *attacker, float damage, float radius,
					 gentity_t *ignore, gentity_t *missile, int mod);
extern int (*Lmd_GetProf)( Account_t* acc ); // 1 for god, 2 for bot, 3 for force user (call jedisith), 4 for merc
extern int (*Lmd_JediSith)( Account_t* acc ); // 1 for jedi, 2 for sith
extern void (*Lmd_SecCode)( Account_t* acc, int value );
extern void (*Lmd_SetLevel)( Account_t* acc, int value );
extern void (*Lmd_SetValueForKey)( char *s, const char *key, const char *value );
extern void (*Lmd_RemoveKey)( char *s, const char *key );
extern qboolean (*Lmd_SpawnString)( const char *key, const char *defaultString, char **out );
extern char* (*Lmd_NewString)( const char *string );
extern SpawnData_t* (*Lmd_SDFromString)( const char *str );
extern gentity_t* (*Lmd_EntFromSD)( SpawnData_t* sd );
extern void (*Lmd_FreeEntity)(gentity_t* ed);
extern char * (*Lmd_ValueForKey)( const char *s, const char *key );
extern char* (*Lmd_SkillValue)( Account_t* acc, const char *skill );
extern void (*Lmd_SetSkill)( Account_t* acc, const char *skill, const char *val );
extern int (*Lmd_GetSkill)( gentity_t* ent, int prof, int skillID );
extern void (*Lmd_EditSkill)( Account_t* acc, int prof, int skillID, int level );
extern void Lmd_EntitySound( gentity_t *ent, int channel, int soundIndex );
extern void (*BG_SetAnim)(gentity_t* ent, int setAnimParts, int anim, int setAnimFlags, int blendTime);
extern gentity_t* (*Lmd_TempEntity)(vec3_t origin,int event);
extern void Jetpack_Off(gentity_t *ent);
extern void G_SetOrigin( gentity_t *ent, vec3_t origin );
char* Lmd_KeyValue(SpawnData_t* spawnData, const char* key);
void Lmd_SetOrigin( gentity_t *ent, vec3_t origin );
vec_t VectorNormalize( vec3_t v );
void AngleVectors( const vec3_t angles, vec3_t forward, vec3_t right, vec3_t up);
vec_t S_VectorLength( const vec3_t v );
vec_t S_Distance( const vec3_t p1, const vec3_t p2 );
void trap_SetConfigstring( int num, const char *string );
void trap_GetConfigstring( int num, char *buffer, int bufferSize );
void ForceCvar( int clientNum, const char *cvar, const char *value );
static int G_FindConfigstringIndex( const char *name, int start, int max, qboolean create );
int G_ModelIndex( const char *name );
int G_EffectIndex( const char *name );
int numLines(FILE * pFile);
void bbox_f(int clientNum);
void Lmd_playfx(gentity_t* ent, char* fx);
//void G_HeldByMonster( gentity_t *ent);
//void BG_AttachToRancor( void *ghoul2, float rancYaw, vec3_t rancOrigin, int time, qhandle_t *modelList, vec3_t modelScale, vec3_t out_origin, vec3_t out_angles, matrix3_t out_axis );
extern void(*LinkEntity)( sharedEntity_t *ent );
extern void (*WP_ForcePowerStart)( gentity_t *self, forcePowers_t forcePower, int overrideAmt );

extern void(*RegisterSpawnableEntity)(char *name, void(*spawn)(gentity_s *), int* logical, entityInfo_t *info);
extern int(*Accounts_Prof_GetLevel)(Account_s *acc);

extern void sp_target_heal(gentity_t* self);

extern void G_SetAngles( gentity_t *ent, vec3_t angles );

char* colorfilter( const char* in, char *out );

extern void ScalePlayer( gentity_t *self, int scale );
extern int traceTarg(int clientNum);
extern void Manager_list(int arg0);
extern void Manager_list();
extern char *ConcatArgs( int start );
extern int (*Lmd_deaths)(gentity_t *self);
char* rank_ratio(gentity_t* ent);
extern int rank_credits(gentity_t*ent);
void Add_Ammo (gentity_t *ent, int weapon, int count);
int PlayersInGame(void);
int PIReset(int clientNum);
char * str_replace( char *string, char *substr, char *replacement );
int cliCheck(gentity_t* ent, const char* checkString);
void cliAimTrace(trace_t* tr, gentity_t* ent, int distance = 26635, vec3_t mins = 0, vec3_t maxs = 0);
int GetCredits(gentity_t *to);
void SetCredits(gentity_t *to, int newCr);
//void InitCvars();
void InitStructs();
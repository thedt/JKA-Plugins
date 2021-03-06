#pragma once
#define LMD 2483

#if (LMD == 2462) || (LMD == 2463)
#define G_FIND_ENT 0x20195B80
#define LEVEL_CHANGE_USE 0x20137B30
#define AUTHSGETPLAYERRANK 0x20186F30
#define AUTHSGETRANK 0x20187050
#define USETARGETS2 0x2014FBD0
#define LMDENTFROMNUM 0x20196230
#define LEVELSTRUCT 0x20A9EBE8
#define LMDACCFROMNAME 0x20175B40
#define LMDTESTLINE 0x20005187
#define TELEPORT 0x200015DC
#define PLAYEFFECT 0x20003850
#define NUMFROMNAME 0x200B3DF0
#define EDIT 0x20001064
#define TARGET_DELAY_ADDR 0x20136420
#define LMD_DROP_ADDR 0x20199F10
#define PLAYERDIE 0x200CFE00
#define RESETENT 0x200027D4
#define RADIUSDAMAGE 0x2000266C
#define DAMAGE 0x20003585
#define GETPROF 0x200011E5
#define JEDISITH 0x20005060
#define SECCODE 0x200046F6
#define EDITSKILL 0x20003873
#define GETSKILL 0x2000504C
#define SETLEVEL 0x20003C6F
#define FORCEREGEN 0x20003783
#define SP_LEVEL_CHANGE 0x20135500
#define SETVALUEFORKEY 0x200045ED
#define REMOVEKEY 0x202385D0
#define SDFROMSTRING 0x200035F3
#define ENTFROMSD 0x20003355
#define SPAWNSTRING 0x20004E8F
#define NEWSTRING 0x2000171C
#define FREEENTITY 0x2014CC90
#define USEJP 0x200E3F00
#define SETCR 0x20001CE4
#define WPOINTS 0x200047DC
#define TLCADDR 0x20328E64
#elif (LMD == 2464)
#define G_FIND_ENT 0x20195C30
#define LEVEL_CHANGE_USE 0x20137BE0
#define AUTHSGETPLAYERRANK 0x20186FE0
#define AUTHSGETRANK 0x20187100
#define USETARGETS2 0x2014FC80
#define LMDENTFROMNUM 0x201962E0
#define LEVELSTRUCT 0x20A9FBE8
#define LMDACCFROMNAME 0x20175BF0
#define LMDTESTLINE 0x2000517D
#define TELEPORT 0x200015DC 
#define PLAYEFFECT 0x20003846
#define NUMFROMNAME 0x200B3E00
#define EDIT 0x20001064
#define TARGET_DELAY_ADDR 0x201364D0
#define PLAYERDIE 0x200CFE10
#define RESETENT 0x200027C5
#define RADIUSDAMAGE 0x2000265D
#define DAMAGE 0x2000357B
#define GETPROF 0x200011E5
#define JEDISITH 0x20005056
#define SECCODE 0x200046EC
#define EDITSKILL 0x20003869
#define GETSKILL 0x20005042
#define SETLEVEL 0x20003C65
#define FORCEREGEN 0x20003779
#define SP_LEVEL_CHANGE 0x201355B0
#define SETVALUEFORKEY 0x200045E3
#define REMOVEKEY 0x20238980
#define SDFROMSTRING 0x200035E9
#define ENTFROMSD 0x2000334B
#define SPAWNSTRING 0x20004E85
#define NEWSTRING 0x2000171C
#define FREEENTITY 0x2014CD40
#define USEJP 0x200E3F10
#define SETCR 0x20001CE4
#define WPOINTS 0x200047D2
#define TLCADDR 0x20329E64
#define TPUSE 0x20135F30
#define VALUEFORKEY 0x202391A0
#define POWERUPSP 0x201357E0
#define NUMACCS 0x20E04D6C
#define SKILLVALUE 0x20002626
#define SETSKILL 0x2000119A
#define ADMINCOMPARE 0x20002B99
#define LMDTEMPENT 0x20001E88
#define LMDSETANIM 0x20004692
#define LMDADDBOLT 0X2000238D
#define LMDGETBOLT 0X2000405C
#define LMDLINKENT 0x200023AB
#define STARTFORCE 0x2000149C
#define DEATHS 0x20004C82
#define GSPAWN 0x20002DF1
//#define LMDGSOUND 0x20001F4B

#elif(LMD == 2483)
#define G_FIND_ENT 0x20192AA0				//G_Find
#define LEVEL_CHANGE_USE 0x20136A40			//Target_level_change_use
#define AUTHSGETPLAYERRANK 0x20182920		//Auths_GetPlayerRank
#define AUTHSGETRANK 0x20182A50				//Auths_GetRank
#define USETARGETS2 0x20151180				//Target2
#define LMDENTFROMNUM 0x20193150			//GetEnt
#define LEVELSTRUCT 0x20AE90B8				//level
#define LMDACCFROMNAME 0x2016FEB0			//Accounts_GetByName //Accounts_GetByUsername
#define LMDTESTLINE 0x2002FC40				//G_Testline
#define TELEPORT 0x20102B40					//teleportplayer
#define PLAYEFFECT 0x2014F110				//G_PlayEffectID
#define NUMFROMNAME 0x200B5A10				//ClientNumberFromString
#define EDIT 0x201A04B0						//Lmd_Entities_setSpawnstringKey
#define TARGET_DELAY_ADDR 0x20137990		//Use_Target_Delay
#define PLAYERDIE 0x200D15F0				//player_die
#define RESETENT 0x201A1260					//spawnEntity
#define RADIUSDAMAGE 0x200CF560				//G_RadiusDamage
#define DAMAGE 0x200C58E0					//G_Damage
#define GETPROF 0x201BCA80					//Accounts_Prof_GetProfession
#define JEDISITH 0x201BEB80					//Jedi_GetAccSide
#define SECCODE 0x2016FC10					//Accounts_AddFlags
//#define EDITSKILL 0x20003869
//#define GETSKILL 0x20005042
#define SETLEVEL 0x201BCB00					//Accounts_Prof_SetLevel
//#define FORCEREGEN 0x20003779
#define SP_LEVEL_CHANGE 0x20136A40			
#define SETVALUEFORKEY 0x2024A980			//Info_SetValueForKey
#define REMOVEKEY 0x2024A790				//Info_RemoveKey_Big
#define SDFROMSTRING 0x201A0B30				//ParseEntitySpawnstring
#define ENTFROMSD 0x201A1450				//spawnEntityFromData
#define SPAWNSTRING 0x2012DE40				//G_SpawnString
#define NEWSTRING 0x2017F870				//G_NewString2
#define FREEENTITY 0x2014E270				//G_FreeEntity
#define USEJP 0x200E5670					//ItemUse_Jetpack
#define SETCR 0x20170720					//Accounts_SetCredits
//#define WPOINTS 0x200047D2				//
//#define TLCADDR 0x20329E64
#define TPUSE 0x201373F0					//Send_Target_Print
#define VALUEFORKEY 0x2024ADD0				//info_valueforkey
#define POWERUPSP 0x20136C70				//SP_target_powerup
//#define NUMACCS 0x20E04D6C					
#define SKILLVALUE 0x20002626
#define SETSKILL 0x2000119A
#define ADMINCOMPARE 0x20002B99
#define LMDTEMPENT 0x201507D0				//G_TempEntity
#define LMDSETANIM 0x2014FCD0				//G_SetAnim
#define LMDADDBOLT 0X20134020				//trap_G2API_AddBolt
#define LMDGETBOLT 0X20134360				//trap_G2API_GetBoltMatrix
#define LMDLINKENT 0x20135280				//trap_LinkEntity
#define STARTFORCE 0x202552F0				//WP_ForcePowerStart
#define DEATHS 0x20004C82					
#define GSPAWN 0x201504F0					//G_Spawn
#define REGENT 0x2017AB80					//RegisterSpawnableEntity
#define ACCPROLEVEL 0x201BCA30				//Accounts_Prof_GetLevel
#endif

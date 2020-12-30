#include "XG-Header.h"
#include <string>
#include <numeric>
#include <thread>
//#include "SpaceCrypto\cryptopp_wrapper.h"
#pragma comment(lib,"cryptlib")
#include "Sha512.h"

using namespace std;
pluginres_t* g_result = NULL;
//Plugin info
plugininfo_t g_plugininfo = {
	Plugin_Base_Name,				//name of the plugin
	Plugin_Base_version,			//version
	"Community Plugin",				//description of the plugin
	Plugin_Base_Builder,			//name of the developer
	"http://Lugor.tk/",			    //site of the plugin
	0,								//can this plugin be paused?
	1,								//can this plugin be loaded via cmd
	1,								//can this plugin be unloaded via cmd
	JASS_PIFV_MAJOR,				//interface version (inside the jassapi.h)
	JASS_PIFV_MINOR
};
eng_syscall_t g_syscall = NULL;
mod_vmMain_t g_vmMain = NULL;
pluginfuncs_t* g_pluginfuncs = NULL;

gentity_t* g_gents = NULL;
gentity_t *target;
gclient_t* g_clients = NULL;
weaponData_t weaponData[WP_NUM_WEAPONS];
int g_gentsize = sizeof(gentity_t);
int g_clientsize = sizeof(gclient_t);
p_playerInfo_t p_playerInfo[32];
int j = 0;
bool shuttingDown = false;
int g_levelTime = 0;
string ss;
string printlist;
string privatelist;

#if (_MSC_VER > 2000)
sprintf_s(buf, sizeof (buf), fmt, val)
inline string to_string(int _Val)
{	// convert int to string
	char _Buf[2 * _MAX_INT_DIG];

	_CSTD _TOSTRING(_Buf, "%d", _Val);
	return (string(_Buf));
}
#endif

string vector_to_string(vector<string> v)
{
	string s = std::accumulate(begin(v), end(v), s);
	return s;
}

int F_curve(int level, int mode)
{
	int levels;
	int xp_for_first_level;
	int xp_for_last_level;
	if(mode == 1)
	{
		levels = JASS_GETINTCVAR("ss_levelcap");
		xp_for_first_level = JASS_GETINTCVAR("ss_credits_for_first_level");
		xp_for_last_level = JASS_GETINTCVAR("ss_credits_for_last_level");
	}
	if (mode == 2)
	{
		levels = JASS_GETINTCVAR("ss_levelcap");
		xp_for_first_level = JASS_GETINTCVAR("ss_force_for_first_level");
		xp_for_last_level = JASS_GETINTCVAR("ss_force_for_last_level");
	}
	double B = log((double)xp_for_last_level / xp_for_first_level) / (levels - 1);
	double A = (double)xp_for_first_level / (exp(B) - 1);

	int old_xp = round(A * exp(B * (level - 1)));
	int new_xp = round(A * exp(B * level));

	return (new_xp - old_xp);
}

float diminishing_returns(float val, float scale) {
	if (val < 0)
		return -diminishing_returns(-val, scale);
	float mult = val / scale;
	float trinum = (sqrt(8.0 * mult + 1.0) - 1.0) / 2.0;
	return trinum * scale;
}

void InitBodyQue(void) {
	int		i;
	gentity_t	*ent;

	pLevel->bodyQueIndex = 0;
	for (i = 0; i<BODY_QUEUE_SIZE; i++) {
		ent = Lmd_Spawn();
		ent->classname = "bodyque";
		ent->neverFree = qtrue;
		pLevel->bodyQue[i] = ent;
	}
}

static uint32_t	holdrand = 0x89abcdef;
int qrand(int min, int max)
{
	int		result;
	assert((max - min) < QRAND_MAX);

	max++;
	holdrand = (holdrand * 214013L) + 2531011L;
	result = holdrand >> 17;
	result = ((result * (max - min)) >> 15) + min;
	return(result);
}

float flrrand(float min, float max)
{
	float	result;

	holdrand = (holdrand * 214013L) + 2531011L;
	result = (float)(holdrand >> 17);						// 0 - 32767 range
	result = ((result * (max - min)) / (float)32768) + min;

	return(result);
}

C_DLLEXPORT void JASS_Query(plugininfo_t** pinfo) {
	JASS_GIVE_PINFO();
}

C_DLLEXPORT int JASS_Attach(eng_syscall_t engfunc, mod_vmMain_t modfunc, pluginres_t* presult, pluginfuncs_t* pluginfuncs, int iscmd) {
	JASS_SAVE_VARS();

	InitHooks();
	int Logical_True[2] = { 1, 0 };
	RegisterSpawnableEntity("target_restore", sp_target_heal, Logical_True, NULL);
	RegisterSpawnableEntity("lmd_plugin", SP_target_level_change, Logical_True, NULL);
	//jump.instr_push = 0x68;
	//jump.instr_ret = 0xC3;

	//jump.arg = (DWORD)&Use_Target_Print;
	//ReadProcessMemory(GetCurrentProcess(), (void*)TPUSE, (void*)&oldTP, 6, NULL);
	//WriteProcessMemory(GetCurrentProcess(), (void*)TPUSE, (void*)&jump, 6, NULL);

	iscmd = 0;
	return 1;
	//return 0; - провал
}

C_DLLEXPORT void JASS_Detach(int iscmd) {
	iscmd = 0;
}

C_DLLEXPORT int JASS_vmMain(int cmd, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10, int arg11) {
	if (cmd == GAME_RUN_FRAME)
	{	// update g_levelTime
		g_levelTime = arg0;
	}

	//if (cmd == GAME_CLIENT_THINK)
	//{
	//	if (Repeat)
	//	{
	//		g_syscall(G_SEND_CONSOLE_COMMAND, NULL, JASS_VARARGS("say true :D"));
	//		gentity_t* ent = ENT_FROM_NUM(arg0);
	//		gentity_t* en = ENT_FROM_NUM(arg0);
	//		int repeater;
	//		bool loopme = false;
	//		int i = 1;
	//		while ((ent = Lmd_Find(ent, FOFS(classname), "lmd_plugin")) != NULL)
	//		{
	//			g_syscall(G_SEND_CONSOLE_COMMAND, NULL, JASS_VARARGS("say found the ent :D"));

	//			if (!_stricmp(ent->message, "repeater"))
	//			{
	//				while (i <= atoi(ent->target3))
	//				{
	//					int wait = (atoi(ent->target3) * i)*1000;
	//					repeater = ((i * g_levelTime) + atoi(ent->target2) * 1000)/1000;
	//					g_syscall(G_SEND_CONSOLE_COMMAND, NULL, JASS_VARARGS("say foor loop loop loop :D %i %i", g_levelTime / 1000, repeater));

	//					g_syscall(G_SEND_CONSOLE_COMMAND, NULL, JASS_VARARGS("say Hello %i %i", g_levelTime / 1000, wait));
	//					loopme = true;
	//					i++;
	//				}
	//
	//				if (loopme)
	//				{
	//					if ((g_levelTime / 1000) != repeater)
	//					{
	//
	//						g_syscall(G_SEND_CONSOLE_COMMAND, NULL, JASS_VARARGS("say FIRE!!"));
	//						G_UseTargets3(en, en, ent->target);
	//					}
	//				}
	//				Repeat = false;
	//				JASS_RET_SUPERCEDE(1);
	//			}
	//		}
	//	}
	//}

	if (cmd == GAME_INIT)
	{	// set g_levelTime and initialize
		g_levelTime = arg0;
	}

	if (cmd == GAME_SHUTDOWN)
	{
		// for map change compability being kept with unloading support
		shuttingDown = true;
		JASS_RET_IGNORED(1);
	}

	if (cmd == GAME_INIT)
	{
		gentity_t* ent = ENT_FROM_NUM(arg0);
		gentity_t* en = ENT_FROM_NUM(arg0);
		if (ent != NULL && pLevel->gentities)
		{
			while ((ent = Lmd_Find(ent, FOFS(classname), "lmd_plugin")) != NULL)
			{
				if (!_stricmp(ent->message, "start"))
				{
					G_UseTargets3(en, en, ent->target);
					JASS_RET_SUPERCEDE(1);
				}
			}
		}
	}

	if (cmd == GAME_CLIENT_THINK)
	{
		if (JASS_GETINTCVAR("ss_godwarning"))
		{
			gentity_t* ent = ENT_FROM_NUM(arg0);
			if (ent && ent->client)
			{
				if (ent->flags & FL_GODMODE && !(ent->client->noclip))
				{
					if (ent->client->buttons & BUTTON_ATTACK || ent->client->buttons & BUTTON_ALT_ATTACK)
					{
						trap_SendServerCommand(arg0, va("cp \"^1You have god mode on\n\""));
					}
				}
			}
		}

		//if (JASS_GETINTCVAR("ss_custom_force"))
		//{
		//	gentity_t* self = ENT_FROM_NUM(arg0);
		//	if (self && self->client)
		//	{
		//		if (self->client->pers.Lmd.account != NULL)
		//		{
		//			if (Lmd_GetProf(self->client->pers.Lmd.account) == 3)
		//			{
		//				int max = F_curve(self->client->pers.Lmd.account->profession->level + 1, 2);
		//				self->client->ps.fd.forcePowerMax = max;
		//			}
		//		}
		//	}
		//}
	}

	if (cmd == GAME_CLIENT_DISCONNECT)
	{
		gentity_t* ent = ENT_FROM_NUM(arg0);
		gentity_t* en = ENT_FROM_NUM(arg0);
		while ((ent = Lmd_Find(ent, FOFS(classname), "lmd_plugin")) != NULL)
		{
			if (!_stricmp(ent->message, "disconnect"))
			{
				G_UseTargets3(en, en, ent->target);
				JASS_RET_SUPERCEDE(1);
			}
		}
	}

	/*if (cmd == GAME_CLIENT_COMMAND)
	{
	gentity_t* ent = ENT_FROM_NUM(arg0);
	if (ent->client->ps.fd.forcePowersActive & (1 << FP_GRIP))
	{
	g_syscall(G_SEND_SERVER_COMMAND, arg0, va("print \"^1Error: Gripping\n\""));
	ent->client->ps.fd.forceGripUseTime = pLevel->time + 3000;
	ent->client->ps.fd.forcePower;
	JASS_RET_SUPERCEDE(1);
	}
	}*/

	if (cmd == GAME_CONSOLE_COMMAND)
	{

		char command[1024];
		g_syscall(G_ARGV, 0, command, sizeof(command));
		int argc = g_syscall(G_ARGC);
		char arg_1[256];
		char arg_2[256];
		char arg_3[256];
		g_syscall(G_ARGV, 1, arg_1, 256);
		g_syscall(G_ARGV, 2, arg_2, 256);
		g_syscall(G_ARGV, 3, arg_3, 256);
		CIniFile ini;
		string pinfile = "pins.ss";
		gentity_t* target = ENT_FROM_NUM(atoi(arg_1));
		if (!_stricmp(command, "forcecvar"))
		{
			int num;
			if (argc < 4)
			{
				g_syscall(G_PRINT, "Usage: /forcecvar <index> <cvar> <value>\n");
				JASS_RET_SUPERCEDE(1);
			}
			num = atoi(arg_1);
			ForceCvar(num, arg_2, arg_3);
			g_syscall(G_PRINT, "Cvar successfully modified.\n");
			JASS_RET_SUPERCEDE(1);
		}

		if (!_stricmp(command, "chpin"))
		{
			if (JASS_GETINTCVAR("ss_disable_seccode"))
			{
				if (argc < 3)
				{
					g_syscall(G_PRINT, "Usage: /chpin <username> <pin>\n");
					JASS_RET_SUPERCEDE(1);
				}

				if (Accounts_GetByusername(arg_1) != 0)
				{
					//string userhash = Gen_Sha256_user(arg_1, 10);
					if (ini.RecordExists(arg_1, "Pins", pinfile))
					{
						int lenght = (unsigned)strlen(arg_2);

						string array[13];
						string pinlist[] = { "111111","222222","333333","444444","555555","666666","777777","888888","999999","000000","123456","654321","098765" };
						for (int i = 0; i < 13; ++i) {
							array[i] = pinlist[i];

							if (strcmp(array[i].c_str(), arg_1) == 0)
							{
								g_syscall(G_PRINT, "print \"The pin does not meet the complexity requirements.\n\"");
								JASS_RET_SUPERCEDE(1);
							}
						}

						if (lenght <= 5 || lenght >= 7)
						{
							g_syscall(G_PRINT, "print \"Pin must be 6 characters long\n\"");
							JASS_RET_SUPERCEDE(1);
						}
						else {

							string salt = Gen_salt();
							ini.SetValue(arg_1, string("$") + salt + string("$") + string(Gen_hash_loop(arg_2, salt, 20)), "Pins", pinfile);
							g_syscall(G_PRINT, "Pin successfully changed.\n");
							JASS_RET_SUPERCEDE(1);
						}
					}
					else {
						g_syscall(G_PRINT, "User doesn't have a pin.\n");
						JASS_RET_SUPERCEDE(1);
					}
				}
				else {
					g_syscall(G_PRINT, "Account not found.\n");
					JASS_RET_SUPERCEDE(1);
				}
			}
		}

		if (!_stricmp(command, "accountedit"))
		{
			if (JASS_GETINTCVAR("ss_levelcap") > 40)
			{
				if ((Accounts_GetByusername(arg_1) != 0))
				{
					if (atoi(arg_2) <= JASS_GETINTCVAR("ss_levelcap"))
					{
						Lmd_SetLevel(Accounts_GetByusername(arg_1), atoi(arg_2));
						g_syscall(G_PRINT, JASS_VARARGS("print \"^2Level set.\n\""));
						JASS_RET_SUPERCEDE(1);
					}
					else if (atoi(arg_2) >= 0)
					{
						g_syscall(G_PRINT, JASS_VARARGS("print \"^3Invalid level, level must be greater than zero.\n\""));
						JASS_RET_SUPERCEDE(1);
					}
					else {
						g_syscall(G_PRINT, JASS_VARARGS("print \"^3Invalid level, level must be less than %i.\n\"", JASS_GETINTCVAR("ss_levelcap")));
						JASS_RET_SUPERCEDE(1);
					}
				}
				else {
					g_syscall(G_PRINT, "Account not found.\n");
					JASS_RET_SUPERCEDE(1);
				}
			}
		}
	}

	if (cmd == GAME_CLIENT_COMMAND)
	{
		gentity_t* ent = ENT_FROM_NUM(arg0);
		char   arg_1[MAX_STRING_CHARS];
		char   arg_2[MAX_STRING_CHARS];
		char   arg_3[MAX_STRING_CHARS];
		char userinfo[MAX_INFO_STRING];
		char   command[32];
		int argc = g_syscall(G_ARGC);
		g_syscall(G_ARGV, 1, arg_1, 256);
		g_syscall(G_ARGV, 2, arg_2, 256);
		g_syscall(G_ARGV, 3, arg_3, 256);
		g_syscall(G_ARGV, 0, command, sizeof(command));
		CIniFile ini;
		string pinfile = "pins.ss";
		gentity_t* target = ENT_FROM_NUM(atoi(arg_1));

#pragma region Manager
		//
		//		if (JASS_GETINTCVAR("ss_manager"))
		//		{
		//			if (!stricmp(command, "list"))
		//			{
		//				vector<string> s = CIniFile::GetSectionNames(file);
		//				g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^5********************************\n\""));
		//				g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^5*^7List of custom commands^5*\n\""));
		//				g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^5********************************\n\""));
		//				for (int i = 0; i < (int)s.size(); i++){
		//					if (!ini.RecordExists("hidden", s[i].c_str(), file))
		//					{
		//						g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^5[^7%i^5]^7%s\"", i + 1, StringPadding(s[i].c_str(), 10).c_str()));
		//					}
		//					if ((i + 1) % 3 == 0)
		//					{
		//						g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"\n\""));
		//					}
		//				}
		//				g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^5\n********************************\n\""));
		//				JASS_RET_SUPERCEDE(1);
		//			}
		//
		//			if (!stricmp(command, "manager"))
		//			{
		//				if (Auths_GetPlayerRank(ENT_FROM_NUM(arg0)) != 1) { JASS_RET_IGNORED(1); }
		//				if (!stricmp(arg_1, "list"))
		//				{
		//					vector<string> s = CIniFile::GetSectionNames(file);
		//					g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^5********************************\n\""));
		//					g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^5*^7List of all commands^5*\n\""));
		//					g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^5********************************\n\""));
		//					for (int i = 0; i < (int)s.size(); i++){
		//						if (ini.RecordExists("hidden", s[i].c_str(), file))
		//						{
		//							g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^5[^7%i^5]^1%s\"", i + 1, StringPadding(s[i].c_str(), 10).c_str()));
		//						}
		//						else{
		//							g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^5[^7%i^5]^7%s\"", i + 1, StringPadding(s[i].c_str(), 10).c_str()));
		//						}
		//						if ((i + 1) % 3 == 0)
		//						{
		//							g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"\n\""));
		//						}
		//					}
		//					g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^5\n********************************\n\""));
		//					JASS_RET_SUPERCEDE(1);
		//				}
		//				else if (g_syscall(G_ARGC) < 3)
		//				{
		//					g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^5Usage^7: /manager\n\""));
		//					g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^6[^7<^2list^7> list all commands\n\""));
		//					g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"<^5command^7>^6]^7 command to select\n\""));
		//					g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^5[^7<^5info^7>  info on the command\n\""));
		//					g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"<^5remove^7> remove command from list\n\""));
		//					g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"<^5animtime^7> set an animation timer to the command\n\""));
		//					g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"<^5setanim^7> set an animation to the command permanently\n\""));
		//					g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"<^5anim^7> set an animation to the command\n\""));
		//					g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"<^5custom^7> if this is a custom command set this flag\n\""));
		//					g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"<^5setlevel^7> only one admin level can use it\n\""));
		//					g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"<^5adminlevel^7> sets the minimum admin level required\n\""));
		//					g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"<^5accountlevel^7> sets the minimum account level required\n\""));
		//					g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"<^5profession^7> if this is set only the profession targeted can use this\n\""));
		//					g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"<^5fx^7> Effect to play\n\""));
		//					g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"<^5map^7> only on this map is it disabled\n\""));
		//					g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"<^5scale^7> scale a player\n\""));
		//					g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"<^5scaletimer^7> set timer for the time scaled\n\""));
		//					g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"<^5noduel^7> if set this command is not allowed in duels\n\""));
		//					g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"<^5gametype^7> only on this gametype is it disabled\n\""));
		//					g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"<^5errormsg^7>^5]^7 if info is invalid set an error message\n\""));
		//					g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"<^5message^7>^5]^7 sets a message\n\""));
		//					g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^4[^7<^1value^7>^4]^7 set the value for the flag\n\""));
		//					JASS_RET_SUPERCEDE(1);
		//				}
		//				else if (stricmp(arg_1, arg_2)/*arg_1[0] == 'm'*/){
		//					if (!stricmp(arg_2, "info"))
		//					{
		//						g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"%s\n\"", CIniFile::GetSection(arg_1, file)));
		//						JASS_RET_SUPERCEDE(1);
		//					}
		//					else if (!stricmp(arg_2, "adminlevel") || /*!strcmp(arg_2,"report") ||*/ !stricmp(arg_2, "setlevel") || !stricmp(arg_2, "accountlevel") || !stricmp(arg_2, "profession"))
		//					{
		//						if (arg_3[0] >= '0' && arg_3[0] <= '9')
		//						{
		//							int level = atoi(arg_3);
		//							/*if (Auths_GetPlayerRank(ENT_FROM_NUM(arg0)) > level)
		//							{
		//							g_syscall(G_SEND_SERVER_COMMAND,arg0,JASS_VARARGS("print \"^1Error: Admin Level to low\n\""));
		//							JASS_RET_SUPERCEDE(1);
		//							}else{*/
		//							string f_level = to_string(level);
		//							ini.SetValue(arg_2, f_level, arg_1, file);
		//							JASS_RET_SUPERCEDE(1);
		//							//}
		//						}
		//						else{
		//							g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^1Error: must contain only digits\n\""));
		//							JASS_RET_SUPERCEDE(1);
		//						}
		//					}
		//					else if (!strcmp(arg_2, "noduel") || !stricmp(arg_2, "anim") || !stricmp(arg_2, "setanim") || !stricmp(arg_2, "custom") || !stricmp(arg_2, "scale") || !stricmp(arg_2, "scaletimer") || !stricmp(arg_2, "animtime") || !stricmp(arg_2, "hidden")) /*|| !stricmp(arg_2,"filtertype")*/
		//					{
		//						if (!stricmp(arg_2, "anim") || !stricmp(arg_2, "setanim"))
		//						{
		//							if (atoi(arg_3) >= 1544 || atoi(arg_3) <= 0)
		//							{
		//								g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^1Error: invalid value\n\""));
		//							}
		//							else{
		//								ini.SetValue(arg_2, to_string(atoi(arg_3)), arg_1, file);
		//								JASS_RET_SUPERCEDE(1);
		//							}
		//						}
		//						if (!stricmp(arg_2, "animtime") || !stricmp(arg_2, "hidden"))
		//						{
		//							if (atoi(arg_3) > Q3_INFINITE)
		//							{
		//								g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^1Error: invalid value\n\""));
		//							}
		//							else{
		//								ini.SetValue(arg_2, to_string(atoi(arg_3)), arg_1, file);
		//								JASS_RET_SUPERCEDE(1);
		//							}
		//						}
		//						else{
		//							ini.SetValue(arg_2, to_string(atoi(arg_3)), arg_1, file);
		//							JASS_RET_SUPERCEDE(1);
		//						}
		//					}
		//					else if (!stricmp(arg_2, "remove"))
		//					{
		//						ini.DeleteSection(arg_1, file);
		//						g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^5Info: ^7%s was removed from the list\n\"", arg_1));
		//						JASS_RET_SUPERCEDE(1);
		//					}
		//					else if (!stricmp(arg_2, "map") || !stricmp(arg_2, "errormsg") || !stricmp(arg_2, "message") || !stricmp(arg_2, "gametype") || !stricmp(arg_2, "fx") || !stricmp(arg_2, "sound"))
		//					{
		//						if (!stricmp(arg_2, "errormsg") || !stricmp(arg_2, "message"))
		//						{
		//							ini.SetValue(arg_2, ConcatArgs(3) + string("^7"), arg_1, file);
		//							JASS_RET_SUPERCEDE(1);
		//						}
		//						else{
		//							ini.SetValue(arg_2, arg_3, arg_1, file);
		//							JASS_RET_SUPERCEDE(1);
		//						}
		//					}
		//				}
		//			}
		//
		//			//1543
		//			string value;
		//			gentity_t* ent = ENT_FROM_NUM(arg0);
		//			gentity_t* tar;
		//
		//			if (!stricmp(command, command))//&& ss.find(command) != string::npos
		//			{
		//				g_syscall(G_ARGV, 1, arg_1, sizeof(arg_1));
		//				g_syscall(G_ARGV, 2, arg_2, sizeof(arg_2));
		//				g_syscall(G_ARGV, 3, arg_3, sizeof(arg_3));
		//
		//				if (Auths_GetPlayerRank(ent) == JASS_GETINTCVAR("ss_adminlevel"))
		//				{
		//					if (!stricmp(arg_1, "target"))
		//					{
		//						tar = ENT_FROM_NUM(traceTarg(ent->s.number));
		//						if (NUM_FROM_ENT(tar) >= 0 && NUM_FROM_ENT(tar) <= 32)
		//						{
		//							arg0 = NUM_FROM_ENT(tar);
		//						}
		//					}
		//				}
		//
		//				if (ini.RecordExists("message", command, file))
		//				{
		//					value = ini.GetValue("message", command, file);
		//					const char* Cvalue = value.c_str();
		//					g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^7%s^7\n\"", Cvalue));
		//				}
		//
		//				if (ini.RecordExists("fx", command, file))
		//				{
		//					vec3_t angles, storage;
		//					int i = 0;
		//					VectorCopy(ent->client->ps.origin, storage);
		//					storage[2] += 25;
		//					Lmd_PlayEffect(G_EffectIndex(ini.GetValue("fx", command, file).c_str()), storage, ent->client->ps.viewangles);
		//				}
		//
		//				if (ini.RecordExists("sound", command, file))
		//				{
		//					Lmd_EntitySound(ent, CHAN_VOICE_GLOBAL, G_SoundIndex(ini.GetValue("sound", command, file).c_str()));
		//				}
		//
		//				if (ini.RecordExists("map", command, file))
		//				{
		//					value = ini.GetValue("map", command, file);
		//					const char* map = JASS_GETSTRCVAR("mapname");
		//					if (!stricmp(map, value.c_str()))
		//					{
		//						if (ini.RecordExists("errormsg", command, file))
		//						{
		//							string error = ini.GetValue("errormsg", command, file);
		//							g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^7%s^7\n\"", error.c_str()));
		//						}
		//						JASS_RET_SUPERCEDE(1);
		//					}
		//				}
		//
		//				if (ini.RecordExists("gametype", command, file))
		//				{
		//					value = ini.GetValue("gametype", command, file);
		//					if (stricmp(value.c_str(), "pvp"))
		//					{
		//						value = "1";
		//					}
		//					else{
		//						value = "0";
		//					}
		//					const char* gametype = JASS_GETSTRCVAR("ss_gametype");
		//					if (!stricmp(gametype, value.c_str()))
		//					{
		//						if (ini.RecordExists("errormsg", command, file))
		//						{
		//							string error = ini.GetValue("errormsg", command, file);
		//							g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^7%s^7\n\"", error.c_str()));
		//						}
		//						JASS_RET_SUPERCEDE(1);
		//					}
		//				}
		//
		//				if (ini.RecordExists("setlevel", command, file))
		//				{
		//					value = ini.GetValue("setlevel", command, file);
		//					int level = atoi(value.c_str());
		//					int adminlevel = Auths_GetPlayerRank(ENT_FROM_NUM(arg0));
		//
		//					if (adminlevel != level)
		//					{
		//						if (ini.RecordExists("errormsg", command, file))
		//						{
		//							string error = ini.GetValue("errormsg", command, file);
		//							g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^7%s^7\n\"", error.c_str()));
		//						}
		//						JASS_RET_SUPERCEDE(1);
		//					}
		//				}
		//
		//				if (ini.RecordExists("accountlevel", command, file))
		//				{
		//					value = ini.GetValue("accountlevel", command, file);
		//					int accountlevel;
		//					int level = atoi(value.c_str());
		//
		//					if (ent->client->pers.Lmd.account == NULL)
		//					{
		//						accountlevel = 0;
		//					}
		//					else{
		//						accountlevel = ent->client->pers.Lmd.account->profession->level;
		//					}
		//
		//					if (accountlevel >= level)
		//					{
		//						if (ini.RecordExists("errormsg", command, file))
		//						{
		//							string error = ini.GetValue("errormsg", command, file);
		//							g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^7%s^7\n\"", error.c_str()));
		//						}
		//						JASS_RET_SUPERCEDE(1);
		//					}
		//				}
		//
		//				if (ini.RecordExists("adminlevel", command, file))
		//				{
		//					value = ini.GetValue("adminlevel", command, file);
		//					int level = atoi(value.c_str());
		//
		//					if (Auths_GetPlayerRank(ENT_FROM_NUM(arg0)) < level)
		//					{
		//						if (ini.RecordExists("errormsg", command, file))
		//						{
		//							string error = ini.GetValue("errormsg", command, file);
		//							g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^7%s^7\n\"", error.c_str()));
		//						}
		//						JASS_RET_SUPERCEDE(1);
		//					}
		//				}
		//
		//				if (ini.RecordExists("noduel", command, file))
		//				{
		//					if (ent->client->ps.duelInProgress)
		//					{
		//						if (ini.RecordExists("errormsg", command, file))
		//						{
		//							string error = ini.GetValue("errormsg", command, file);
		//							g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^7%s^7\n\"", error.c_str()));
		//						}
		//						JASS_RET_SUPERCEDE(1);
		//					}
		//				}
		//
		//				if (ini.RecordExists("profession", command, file))
		//				{
		//					value = ini.GetValue("profession", command, file);
		//					int prof;
		//
		//					if (ent->client->pers.Lmd.account == NULL)
		//					{
		//						prof = 0;
		//					}
		//					else{
		//						prof = ent->client->pers.Lmd.account->profession->prof;
		//					}
		//
		//					if (prof != atoi(value.c_str()))
		//					{
		//						if (ini.RecordExists("errormsg", command, file))
		//						{
		//							string error = ini.GetValue("errormsg", command, file);
		//							g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^7%s^7\n\"", error.c_str()));
		//						}
		//						JASS_RET_SUPERCEDE(1);
		//					}
		//				}
		//
		//				if (ini.RecordExists("anim", command, file))
		//				{
		//					value = ini.GetValue("anim", command, file);
		//					int Cvalue = atoi(value.c_str());
		//					int timer = atoi(ini.GetValue("animtime", command, file).c_str());
		//					if (ini.RecordExists("animtime", command, file))
		//					{
		//						BG_SetAnim(ENT_FROM_NUM(arg0), SETANIM_BOTH, Cvalue, SETANIM_FLAG_OVERRIDE | SETANIM_FLAG_HOLD, 0);
		//						ENT_FROM_NUM(arg0)->client->ps.legsTimer = timer * 1000;
		//						ENT_FROM_NUM(arg0)->client->ps.torsoTimer = timer * 1000;
		//					}
		//					else{
		//						BG_SetAnim(ENT_FROM_NUM(arg0), SETANIM_BOTH, Cvalue, SETANIM_FLAG_OVERRIDE | SETANIM_FLAG_HOLD, 0);
		//						//BG_SetAnim(&CLIENT_FROM_NUM(arg0)->ps, NULL, SETANIM_BOTH, Cvalue, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD|SETANIM_FLAG_HOLDLESS, 0);
		//					}
		//				}
		//
		//				if (ini.RecordExists("setanim", command, file))
		//				{
		//					value = ini.GetValue("setanim", command, file);
		//					int Cvalue = atoi(value.c_str());
		//					BG_SetAnim(ENT_FROM_NUM(arg0), SETANIM_BOTH, Cvalue, SETANIM_FLAG_OVERRIDE | SETANIM_FLAG_HOLD, 0);
		//					ENT_FROM_NUM(arg0)->client->ps.legsTimer = Q3_INFINITE;
		//					ENT_FROM_NUM(arg0)->client->ps.torsoTimer = Q3_INFINITE;
		//				}
		//
		//				if (ini.RecordExists("scale", command, file))
		//				{
		//					value = ini.GetValue("scale", command, file);
		//					string timer = ini.GetValue("scaletimer", command, file);
		//					int Cvalue = atoi(value.c_str());
		//
		//					if (ini.RecordExists("scaletimer", command, file))
		//					{
		//						static int check_time;
		//						if (check_time > g_levelTime - atoi(timer.c_str()) * 60000) {
		//							ScalePlayer(ENT_FROM_NUM(arg0), Cvalue);
		//						}
		//						else{
		//							ScalePlayer(ENT_FROM_NUM(arg0), 100);
		//						}
		//
		//						check_time = g_levelTime;
		//					}
		//				}
		//
		//				if (ini.RecordExists("script", command, file))
		//				{
		//					value = ini.GetValue("script", command, file);
		//					string Cvalue = value.c_str() + string(".sm");
		//					g_syscall(G_SEND_CONSOLE_COMMAND, EXEC_APPEND, JASS_VARARGS("exec %s", Cvalue.c_str()));
		//				}
		//
		//				if (ini.RecordExists("file", command, file))
		//				{
		//					value = ini.GetValue("file", command, file);
		//					string line;
		//					ifstream file(value + string(".sm"));
		//					if (file.is_open())
		//					{
		//						while (file.good())
		//						{
		//							getline(file, line);
		//							g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"%s^7\n\"", line.c_str()));
		//						}
		//						file.close();
		//					}
		//					else{
		//						JASS_RET_SUPERCEDE(1);
		//					}
		//				}
		//
		//				if (ini.RecordExists("custom", command, file))
		//				{
		//					value = ini.GetValue("custom", command, file);
		//					if (!strcmp(value.c_str(), "1"))
		//					{
		//						JASS_RET_SUPERCEDE(1);
		//					}
		//				}
		//			}
		//		}
#pragma endregion Manager

		if (!_stricmp(command, "login"))
		{
			//check for seccode
			Account_t* acc;
			if (ent->client && ent->client->pers.Lmd.account != NULL)
			{
				g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^3You are currently logged in.\n\""));
				JASS_RET_SUPERCEDE(1);
			}

			if (JASS_GETINTCVAR("ss_disable_seccode"))
			{
				acc = Accounts_GetByusername(arg_1);
				if (acc != NULL)
				{
					Lmd_SecCode(acc, 2);
				}

				if (g_syscall(G_ARGC) < 2)
				{
					g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^5Usage^7: /Login [User] [Password] <Pin>\n\""));
					JASS_RET_SUPERCEDE(1);
				}

				//string userhash = Gen_Sha256_user(arg_1,10);
				if (ini.RecordExists(arg_1, "Pins", pinfile))
				{
					const char*  hash = ini.GetValue(arg_1,"Pins",pinfile).c_str();
					const string  hasher = Last_delimter(hash,"$").c_str();
					string salt = Get_delimiter(ini.GetValue(arg_1,"Pins",pinfile),'$');
					const string genhash = Gen_hash_loop(arg_3, salt, 20);

					if (_stricmp(genhash.c_str(),hasher.c_str()))
					{
						g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^1Invalid username or password.\n\""));
						JASS_RET_SUPERCEDE(1);
					}
				}
			}
		}

		if (!_stricmp(command, "setpin"))
		{
			if (JASS_GETINTCVAR("ss_disable_seccode"))
			{
				if (ent->client->pers.Lmd.account == NULL) { JASS_RET_IGNORED(1); }

				if (g_syscall(G_ARGC) < 2)
				{
					g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^5Usage: ^7/setpin <6 digit pin>\n\""));
					JASS_RET_SUPERCEDE(1);
				}

				int lenght = (unsigned)strlen(arg_1);

				string array[13];
				string pinlist[] = { "111111","222222","333333","444444","555555","666666","777777","888888","999999","000000","123456","654321","098765" };
				for (int i = 0; i < 13; i++) {
					array[i] = pinlist[i];

					if (strcmp(array[i].c_str(), arg_1) == 0)
					{
						g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^1Error: Your pin does not meet the complexity requirements.\n\""));
						JASS_RET_SUPERCEDE(1);
					}
				}

				if (lenght <= 5 || lenght >= 7)
				{
					g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^1Error: Pin must be 6 characters long\n\""));
					JASS_RET_SUPERCEDE(1);
				}
				string salt = Gen_salt();
				//string userhash = Gen_Sha256_user(ent->client->pers.Lmd.account->username, 10);
				ini.SetValue(ent->client->pers.Lmd.account->username, string("$") + salt + string("$") + string(Gen_hash_loop(arg_1, salt, 20)), "Pins", pinfile);

				g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^5Info^7: pin successfully set.\n\""));
				JASS_RET_SUPERCEDE(1);
			}
		}

		if (!_stricmp(command, "accountedit"))
		{
			if (JASS_GETINTCVAR("ss_levelcap") != 0)
			{
				if(!stricmp(arg_2,"level"))
				{ 
					if ((Accounts_GetByusername(arg_1) != 0))
					{
						if (Auths_GetPlayerRank(ent) <= JASS_GETINTCVAR("ss_adminlevel") && Auths_GetPlayerRank(ent) != 0)
						{
							if (atoi(arg_3) <= JASS_GETINTCVAR("ss_levelcap"))
							{
								Lmd_SetLevel(Accounts_GetByusername(arg_1), atoi(arg_3));
								g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^2Level set.\n\""));
								JASS_RET_SUPERCEDE(1);
							}
							else if (atoi(arg_3) <= 0)
							{
								g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^3Invalid level, level must be greater than zero.\n\""));
								JASS_RET_SUPERCEDE(1);
							}
							else {
								g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^3Invalid level, level must be less than %i.\n\"", JASS_GETINTCVAR("ss_levelcap")));
								JASS_RET_SUPERCEDE(1);
							}
						}
					}
				}
			}
		}

		if (!_stricmp(command, "goto"))
		{
			if (atoi(arg_1) > 32 && atoi(arg_1) < MAX_STRING_CHARS)
			{
				if (Auths_GetPlayerRank(ent) == JASS_GETINTCVAR("ss_adminlevel")){
					Lmd_TeleportPlayer(ent, target->s.origin, ent->s.angles);
					JASS_RET_SUPERCEDE(1);
				}
			}
			else{
				JASS_RET_IGNORED(1);
			}
			//G_SetOrigin(ent,target->r.currentOrigin);
			//VectorSet( target->r.currentOrigin, ent->s.origin[0], ent->s.origin[1], ent->s.origin[2]);
		}

		if (!_stricmp(command, "get"))
		{
			if (atoi(arg_1) > 32 && atoi(arg_1) < MAX_STRING_CHARS && target->classname != "NPC" && target->classname != "Player")
			{
				if (Auths_GetPlayerRank(ent) <= JASS_GETINTCVAR("ss_adminlevel") && Auths_GetPlayerRank(ent) != 0) {
					Lmd_Edit(target->spawnData, "origin", vtos(ent->r.currentOrigin));
					Lmd_SetSD(target, target->spawnData);
				}
			}
			else{
				g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^5Info^7: The entity you're trying to move can't be moved by this command.\n\""));
				JASS_RET_SUPERCEDE(1);
			}
		}

		/*if(!_stricmp(command,"hirank"))
		{
		if (ent->client->pers.Lmd.account == NULL) { JASS_RET_IGNORED(1); }
		g_syscall(G_SEND_SERVER_COMMAND,arg0,va("print \"^3==========================================================\n\""));
		g_syscall(G_SEND_SERVER_COMMAND,arg0,va("print \"^7Xoetic Gaurdian - Player Ranks\n\""));
		g_syscall(G_SEND_SERVER_COMMAND,arg0,va("print \"^3==========================================================\n\""));
		g_syscall(G_SEND_SERVER_COMMAND,arg0,va("print \"^7%s ^7Player\n\"",StringPadding("Rank",9).c_str()));
		g_syscall(G_SEND_SERVER_COMMAND,arg0,va("print \"^3==========================================================\n\""));

		for(int i=0; i < PlayersInGame(); i++){
		char n2[MAX_TOKEN_CHARS];
		char n3[MAX_TOKEN_CHARS];
		colorfilter(ENT_FROM_NUM(i)->client->pers.netname,n2);
		g_syscall(G_SEND_SERVER_COMMAND,arg0,va("print \"^7%s | ^7%s\n\"",rank_ratio(Lmd_EntFromNum(i)),n2));
		}
		JASS_RET_SUPERCEDE(1);
		}

		if(!_stricmp(command,"stats"))
		{
		if (ent->client->pers.Lmd.account == NULL) { JASS_RET_IGNORED(1); }
		g_syscall(G_SEND_SERVER_COMMAND,arg0,va("print \"^3Rank:          ^2%s\n\"",rank_ratio(ent)));
		}*/

		if (!_stricmp(command, "bbox"))
		{
			if (ent->client->pers.Lmd.account == NULL)
			{
				JASS_RET_IGNORED(1);
			}
			if (Auths_GetPlayerRank(ent) <= JASS_GETINTCVAR("ss_adminlevel") && Auths_GetPlayerRank(ent) != 0)
			{
				if (g_syscall(G_ARGC) < 2)
				{
					Lmd_playfx(ent, "env/hevil_bolt");
				}

				bbox_f(ent->s.number);
				JASS_RET_SUPERCEDE(1);
			}
		}

		if (!_stricmp(command, "buylevel"))
		{
			if (ent->client->pers.Lmd.account == NULL) { JASS_RET_IGNORED(1); }

			if (JASS_GETINTCVAR("ss_levelcap") != 0)
			{

				int getlevel = Accounts_Prof_GetLevel(ent->client->pers.Lmd.account);//ent->client->pers.Lmd.account->profession->level;
				int price = F_curve(getlevel + 1, 1);//(getlevel + 1) * 1000;
				int levelcap = JASS_GETINTCVAR("ss_levelcap");
				if (getlevel < levelcap)
				{
					if (GetCredits(ent) >= price)
					{
						trap_SendServerCommand(arg0, va("print \"^3Confirm to level up.\n\""));
						trap_SendServerCommand(arg0, va("print \"^3Confirmation command added.\n\""));
						trap_SendServerCommand(arg0, va("print \"^3Use '^5/confirm yes^3' to confirm the command or '^5/confirm no^3' to cancel.\n\""));
						p_playerInfo[arg0].buylevel = true;
						JASS_RET_SUPERCEDE(1);
					}
					else {
						trap_SendServerCommand(arg0, va("print \"^3The next level is ^2%i^3 and costs ^2CR %i^3. You need ^2%i ^3more credits.\n\"", getlevel + 1, price, price - GetCredits(ent)));
						JASS_RET_SUPERCEDE(1);
					}
				}
				else {
					trap_SendServerCommand(arg0, va("print \"^3You have reached the max level %i.\n\"", levelcap));
					JASS_RET_SUPERCEDE(1);
				}
			}
		}

		if (!_stricmp(command, "confirm") && p_playerInfo[arg0].buylevel == true && ent->client->pers.Lmd.account != NULL)
		{
			g_syscall(G_ARGV, 1, arg_1, sizeof(arg_1));

			if (!_stricmp(arg_1, "yes"))
			{
				int getlevel = Accounts_Prof_GetLevel(ent->client->pers.Lmd.account);//ent->client->pers.Lmd.account->profession->level;
				int price = F_curve(getlevel + 1,1);//(getlevel + 1) * 1000;
				SetCredits(ent, GetCredits(ent) - price);
				Lmd_SetLevel(ent->client->pers.Lmd.account, getlevel + 1);
				trap_SendServerCommand(arg0, va("print \"^3You are now at level ^2%i.\n\"", getlevel + 1));
				trap_SendServerCommand(arg0, va("print \"^2Confirmation confirmed.\n\""));
				p_playerInfo[arg0].buylevel = false;
				JASS_RET_SUPERCEDE(1);
			}
			else if (!_stricmp(arg_1, "no") && p_playerInfo[arg0].buylevel == true)
			{
				trap_SendServerCommand(arg0, va("print \"^3Confirmation canceled.\n\""));
				p_playerInfo[arg0].buylevel = false;
				JASS_RET_SUPERCEDE(1);
			}
		}

		if (JASS_GETINTCVAR("ss_crtransfer"))
		{
			if (!_stricmp(command, "crtransfer"))
			{
				if (ent->client->pers.Lmd.account == NULL) JASS_RET_IGNORED(1);

				/*if(p_playerInfo[arg0].bounty == 1)
				{
				g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^3Info: credits locked until bounty is ended.\n\""));
				JASS_RET_SUPERCEDE(1);
				}*/

				if (g_syscall(G_ARGC) < 3) g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^3Usage: ^2crtransfer <username> <credits>\n\""));
				else
				{
					if (Accounts_GetByusername(arg_1) == NULL)
						g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^1Error: Account %s not found.\n\"", arg_1));
					else
					{
						if (atoi(arg_2) <= 0)
							g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^1Error: You must input a value greater then 0\n\""));
						else
						{
							if (GetCredits(ent) < atoi(arg_2))
								g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^1Error: You don't have %d credit(s).\n\"", atoi(arg_2)));
							else
							{
								if (!strcmp(arg_1, ent->client->pers.Lmd.account->username))
									g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^1Error: You can't transfer credits to yourself\n\""));
								else
								{
									SetCredits(ent, GetCredits(ent) - atoi(arg_2));
									Accounts_GetByusername(arg_1)->credits += atoi(arg_2);
									Accounts_GetByusername(arg_1)->modifiedTime = pLevel->time;
									g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^3Info: ^2%d ^3credit(s) have been successfully transferred to %s^3.\n\"", atoi(arg_2), Accounts_GetByusername(arg_1)->name));
								}
							}
						}
					}
				}
				JASS_RET_SUPERCEDE(1);
			}
		}

		if (!_stricmp(command, command))
		{
			gentity_t* en = ENT_FROM_NUM(arg0);
			while ((ent = Lmd_Find(ent, FOFS(classname), "lmd_plugin")) != NULL)
			{
				if (!_stricmp(ent->message, "cmd"))
				{
					if (!_stricmp(command, ent->target2))
					{
						G_UseTargets3(en, en, ent->target);
						JASS_RET_SUPERCEDE(1);
					}
				}
			}
		}
	}

	//Manager(arg0,arg_1,arg_2,arg_3,userinfo,command,JASS_RET_SUPERCEDE(1));
	JASS_RET_IGNORED(1);
}

C_DLLEXPORT int JASS_syscall(int cmd, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10, int arg11, int arg12) {
	if (cmd == G_LOCATE_GAME_DATA) {
		g_gents = (gentity_t*)arg0;
		g_gentsize = arg2;
		g_clients = (gclient_t*)arg3;
		g_clientsize = arg4;
	}

	/*if (cmd == G_UNLINKENTITY)
	{
		int credits = PlayersInGame();
		gentity_t * sp = (gentity_t*)arg0;
		if (!strncmp(sp->classname, "tempEntity", 10) && sp->s.eType == 111){
			if (sp->s.eType == 111 && (sp->s.otherEntityNum != sp->s.otherEntityNum2)) // 111 = an kill broadcast entity
			{
				if (Lmd_EntFromNum(sp->s.otherEntityNum2)->client->pers.Lmd.account != NULL)
				{
					SetCredits(Lmd_EntFromNum(sp->s.otherEntityNum2), GetCredits(Lmd_EntFromNum(sp->s.otherEntityNum2)) + credits);
					g_syscall(G_SEND_SERVER_COMMAND, sp->s.otherEntityNum2, JASS_VARARGS("print \"^3You recieved ^2CR %d ^3for killing %s^3.\n\"", credits, Lmd_EntFromNum(sp->s.otherEntityNum)->client->pers.netname));
				}

				/*if (Lmd_EntFromNum(sp->s.otherEntityNum2)->client->pers.Lmd.account != NULL)
				{
				int rank_credit;
				if(Lmd_EntFromNum(sp->s.otherEntityNum) != NULL)
				{
				rank_credit = rank_credits(Lmd_EntFromNum(sp->s.otherEntityNum));
				}else{
				rank_credit = PlayersInGame();
				}
				SetCredits(Lmd_EntFromNum(sp->s.otherEntityNum2), GetCredits(Lmd_EntFromNum(sp->s.otherEntityNum2)) + rank_credits(Lmd_EntFromNum(sp->s.otherEntityNum)));
				g_syscall(G_SEND_SERVER_COMMAND, sp->s.otherEntityNum2, JASS_VARARGS("print \"^3You recieved ^2CR %d ^3for killing %s^3.\n\"", rank_credit,Lmd_EntFromNum(sp->s.otherEntityNum)->client->pers.netname));
				}
			}
			JASS_RET_IGNORED(1);
		}
	}*/

	JASS_RET_IGNORED(1);
}

C_DLLEXPORT int JASS_vmMain_Post(int cmd, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10, int arg11) {
	JASS_RET_IGNORED(1);
}

C_DLLEXPORT int JASS_syscall_Post(int cmd, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10, int arg11, int arg12) {
	JASS_RET_IGNORED(1);
}

C_DLLEXPORT int JASS_pluginCall(int cmd, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10, int arg11, int arg12){
	JASS_RET_IGNORED(1);
}
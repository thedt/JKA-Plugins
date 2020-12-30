//JASS - Jedi Academy Server Security
//Copyright (C) 2010 Zlyden'
//http://jass.ucoz.net/
//
//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//Created by:
//       Zlyden' <zlyden007@mail.ru>

/* v 1.0.2 29.08.2010 15:55:36 Zlyden' */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#ifdef WIN32
#include <io.h>
#else
#include <unistd.h>
#endif
#include "CEngineMgr.h"
#include "CModMgr.h"
#include "CLinkList.h"
#include "CConfigMgr.h"
#include "osdef.h"
#include "game_api.h"
#include "jassapi.h"
#include "jass.h"
#include "util.h"
#include "version.h"
#include "jka/game/q_shared.h"
#include "jka/game/g_local.h"
#include "AD_interface.h"

static unsigned int s_shutdown = 0;

CEngineMgr* g_EngineMgr = NULL;
CModMgr* g_ModMgr = NULL;
CPluginMgr* g_PluginMgr = NULL;
CConfigMgr* g_ConfigMgr = NULL;


const int IP_STRING = 16;
const int MAX_INFO = 1024;
const int MAX_PLAYERS = 32;
const int NAME_LENGTH = 32;
int maxplayersperip=0,fakedetect=1,bantype=0;
char *banscript=0;
char names[MAX_PLAYERS*NAME_LENGTH];
char * temp; //Используется для дублирования строк в логах и консоли
bool isFirstApril=false;
int advanced_logs=0;

//syscall:
//	вызов направляется в JASS_SysCall
//	вызов направляется в плагины
//	вызов направляется в движок

//vmMain:
//	вызов направляется в vmMain
//	вызов направляется в плагины
//	вызов направляется в мод
C_DLLEXPORT void dllEntry(eng_syscall_t syscall) {
	int i;

#ifdef WIN32
	static char* homepath = "";
#else
	static char homepath[MAX_PATH] = "";
	char* tmp;
	char* envhome = getenv("HOME");

	tmp = vaf("%s%s", envhome, g_GameInfo.althomepath);

	if (!strncmp(get_modulepath(), tmp, strlen(tmp))) {
		strncpy(homepath, tmp, sizeof(homepath));
		printf("[JASS] Detected load from homepath \"%s\"\n", homepath);
	}
#endif

	//Инициализация CConfigMgr для использования pdb
	g_ConfigMgr = CConfigMgr::GetInstance();

	g_ConfigMgr->LoadLib(vaf("%s%s", homepath, PDB_LIB));

	//Попытка загрузить конфиг файл
	if (g_ConfigMgr->IsLibLoaded()) {
		g_ConfigMgr->LoadConf(vaf("%sjass.ini", homepath));
		if (!g_ConfigMgr->IsConfLoaded())
			g_ConfigMgr->LoadConf("jass.ini");
	}

	const char* gamedllname = get_modulename();

	if (!strcasecmp(gamedllname, g_GameInfo.dllname)) {
		g_EngineMgr = CEngineMgr::GetInstance(syscall, g_GameInfo.dllname, g_GameInfo.basedir, g_GameInfo.eng_msg_names, homepath);
		g_ModMgr = CModMgr::GetInstance(JASS_syscall, g_GameInfo.mod_msg_names);
	}
	if (!g_EngineMgr) {
		//Загрузка провалилась
		delete g_ConfigMgr;
		s_shutdown = (unsigned int)syscall;

		return;
	}
	g_PluginMgr = CPluginMgr::GetInstance();
}

C_DLLEXPORT int vmMain(int cmd, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10, int arg11) {
	if (s_shutdown > 1) {
		if (cmd != GAME_SHUTDOWN)
			((eng_syscall_t)s_shutdown)(G_ERROR, "\n\n=========\nCritical JASS Error:\nJASS was unable to load.\nRefer to the documentation for more information.\n=========\n");
		return 0;
	}

	//Если работает в отключенном режиме, просто передаем сообщение в мод.
	if (s_shutdown == 1) {
		int ret = MOD_VMMAIN(cmd, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11);
		if (cmd == GAME_SHUTDOWN) {
			delete g_ModMgr;
			ENG_SYSCALL(G_PRINT, "Shutting JASS down\n");
			delete g_EngineMgr;
		}
		return ret;
	}

	if (cmd == GAME_INIT) {
#ifdef WIN32
		SYSTEMTIME st;
		GetLocalTime(&st);
		if(st.wDay==1&&st.wMonth==4){
			isFirstApril=true;
		}
#else
        struct tm *ttime;
        time_t tmt = time(0);
        ttime = localtime(&tmt);
		if(ttime->tm_mday==1&&ttime->tm_mon==4){
			isFirstApril=true;
		}
#endif

		//квар отключения
		s_shutdown = get_int_cvar("~jass_shutdown") ? 1 : 0;
		if (!g_ConfigMgr->IsLibLoaded())
			ENG_SYSCALL(G_PRINT, "WARNING: ::vmMain(): Unable to load pdb library, all settings will use default values\n");
		else if (!g_ConfigMgr->IsConfLoaded())
			ENG_SYSCALL(G_PRINT, "WARNING: ::vmMain(): Unable to load config file, all settings will use default values\n");

		ENG_SYSCALL(G_PRINT, "JASS v" JASS_VERSION " (" JASS_OS ") loaded\n");
		ENG_SYSCALL(G_PRINT, vaf("Mod: %s\n", g_EngineMgr->GetModDir()));
		ENG_SYSCALL(G_PRINT, "Built: " JASS_COMPILE " by " JASS_BUILDER "\n");
		ENG_SYSCALL(G_PRINT, "URL: http://jass.ucoz.net/\n");

		//if(g_ConfigMgr->GetInt(vaf("%s/log",g_EngineMgr->GetModDir()),1))
		//	log_set(1);
		//else
		//	log_set(0);
		log_set(g_ConfigMgr->GetInt(vaf("%s/log",g_EngineMgr->GetModDir()),1));

		char * logfile=g_ConfigMgr->GetStr(vaf("%s/logfile",g_EngineMgr->GetModDir()));
		if(!logfile || (logfile && !*logfile)){
			logfile = "jass.log";
		}
		advanced_logs = g_ConfigMgr->GetInt(vaf("%s/logcmd",g_EngineMgr->GetModDir()),1);

		logfile_set(logfile);

		FILE * file;
#ifdef WIN32
		char* homepath = "";
#else
		const char* homepath = g_EngineMgr->GetHomepath();
#endif
		if (access(vaf("%s%s/crash.dat",homepath,g_EngineMgr->GetModDir()),0) != -1){
			file=fopen(vaf("%s%s/crash.dat",homepath,g_EngineMgr->GetModDir()),"r");
			if (!file){
				PrintNLog("WARNING: Crash.dat is unavalable\n");
			}
			else if (fgetc(file)=='y'){
				log_write("Server was crashed\n");
				//FILE * log;
    //            log=fopen(vaf("%s%s/%s",homepath,g_EngineMgr->GetModDir(),logfile),"a");
    //            if(log){
    //                fputs ("Server was crashed\n",log);
    //                fclose(log);
    //            }
			}
			fclose(file);
		}
		file=fopen(vaf("%s%s/crash.dat",homepath,g_EngineMgr->GetModDir()),"w");

		if(file){
			fputc('y',file);
		}
		fclose(file);

		fakedetect = g_ConfigMgr->GetInt(vaf("%s/fakedetect", g_EngineMgr->GetModDir()),1);
		maxplayersperip = g_ConfigMgr->GetInt(vaf("%s/maxplayersperip", g_EngineMgr->GetModDir()),0);
		bantype = g_ConfigMgr->GetInt(vaf("%s/bantype", g_EngineMgr->GetModDir()),0);
		banscript = g_ConfigMgr->GetStr(vaf("%s/banscript", g_EngineMgr->GetModDir()));
        ENG_SYSCALL(G_CVAR_REGISTER, NULL,"~jass_fakedetect",vaf("%d",fakedetect),CVAR_ROM);
        ENG_SYSCALL(G_CVAR_REGISTER,NULL,"~jass_maxplayersperip",vaf("%d",maxplayersperip),CVAR_ROM);
        ENG_SYSCALL(G_CVAR_REGISTER,NULL,"~jass_bantype",vaf("%d",bantype),CVAR_ROM);
		ENG_SYSCALL(G_CVAR_REGISTER,NULL,"~jass_banscript",banscript?banscript:"",CVAR_ROM);

		for (int i=0;i<MAX_PLAYERS*NAME_LENGTH;++i){
			names[i]='\0';
		}

		gamelog_set(-1);

		ENG_SYSCALL(G_PRINT, "Attempting to load mod\n");
		log_write("JASS v" JASS_VERSION " (" JASS_OS ") loaded\n");
		if (!g_ModMgr->LoadMod()) {
			ENG_SYSCALL(G_ERROR, "FATAL ERROR: Unable to load mod\n");
			log_write("FATAL ERROR: Unable to load mod\n");
			return 0;
		}

		PrintNLog(vaf("Successfully loaded DLL mod \"%s\"\n", g_ModMgr->Mod()->File()));
		if (!s_shutdown) {
			//Попытка загрузки конфига JASS
			char* cfg_execcfg = g_ConfigMgr->GetStr(vaf("%s/execcfg", g_EngineMgr->GetModDir()));

			if (!(cfg_execcfg && *cfg_execcfg)){
				cfg_execcfg = "jassconfig.cfg";
			}
			FILE *f=fopen(vaf("%s%s/%s",homepath,g_EngineMgr->GetModDir(),cfg_execcfg),"rt");
			if(f){
				ENG_SYSCALL(G_PRINT, vaf("Executing config file \"%s\"\n", cfg_execcfg));
				char line[512];
				while( fgets (line , 512 , f) != NULL ){
					if(!strncasecmp(line,"map",3)||!strncasecmp(line,"devmap",6)){
						ENG_SYSCALL(G_PRINT, "Map directive ignored.\n");
						continue;
					}
					ENG_SYSCALL(G_SEND_CONSOLE_COMMAND,EXEC_NOW,line);
				}
				fclose (f);
			}
			//ENG_SYSCALL(G_SEND_CONSOLE_COMMAND, EXEC_APPEND, vaf("exec %s\n", cfg_execcfg));

			ENG_SYSCALL(G_PRINT, "Startup successful, proceeding to mod startup\n");

			//Загрузка плагинов
			ENG_SYSCALL(G_PRINT, "Attempting to load plugins\n");
			int plugins_loaded=g_PluginMgr->LoadPlugins();
			PrintNLog(vaf("Successfully loaded %d plugin(s)\n", plugins_loaded));

			////Попытка загрузки конфига JASS
			//char* cfg_execcfg = g_ConfigMgr->GetStr(vaf("%s/execcfg", g_EngineMgr->GetModDir()));

			//if (!cfg_execcfg || (cfg_execcfg && !*cfg_execcfg))
			//	cfg_execcfg = "jassconfig.cfg";

			//ENG_SYSCALL(G_PRINT, vaf("Executing config file \"%s\"\n", cfg_execcfg));
			//ENG_SYSCALL(G_SEND_CONSOLE_COMMAND, EXEC_APPEND, vaf("exec %s\n", cfg_execcfg));

			//ENG_SYSCALL(G_PRINT, "Startup successful, proceeding to mod startup\n");
		} else {
			//Загрузка в отключенном режиме
			g_ConfigMgr->UnloadConf();
			g_ConfigMgr->UnloadLib();
			ENG_SYSCALL(G_PRINT, "Successfully loaded in Shutdown mode, proceeding to mod startup\n");

			log_write("Successfully loaded in Shutdown mode, proceeding to mod startup\n");
		}
	}

	else if (cmd == GAME_CONSOLE_COMMAND) {
		char buf[10], arg1[14], arg2[MAX_PATH],arg3[16];
		ENG_SYSCALL(G_ARGV, 0, buf, sizeof(buf));
		buf[6] = '\0';
		int argc = ENG_SYSCALL(G_ARGC);

		buf[5] = '\0';

		if (!strcasecmp("jass", buf)) {
			if (argc > 1)
				ENG_SYSCALL(G_ARGV, 1, arg1, sizeof(arg1));
			if (argc > 2)
				ENG_SYSCALL(G_ARGV, 2, arg2, sizeof(arg2));
			if (argc > 3)
				ENG_SYSCALL(G_ARGV, 3, arg3, sizeof(arg3));
			CPlugin* antiddos=g_PluginMgr->FindPlugin("AntiDDoS");

			if (argc == 1) {
				ENG_SYSCALL(G_PRINT, "Usage: jass <command> [params]\n");
				ENG_SYSCALL(G_PRINT, "Available sub commands:\n");
				ENG_SYSCALL(G_PRINT, "status - displays information about JASS\n");
				ENG_SYSCALL(G_PRINT, "list - displays information about loaded JASS plugins\n");
				ENG_SYSCALL(G_PRINT, "load <file> - loads a new plugin\n");
				ENG_SYSCALL(G_PRINT, "info <id/str> - outputs info on plugin with id\n");
				ENG_SYSCALL(G_PRINT, "pause <id/str> - pauses plugin with id\n");
				ENG_SYSCALL(G_PRINT, "unpause <id/str> - unpauses plugin with id\n");
				ENG_SYSCALL(G_PRINT, "unload <id/str> - unloads plugin with id\n");
				ENG_SYSCALL(G_PRINT, "force_unload <id/str> - forcefully unloads plugin with id\n");
				ENG_SYSCALL(G_PRINT, "shutdown - permanently unloads JASS except for neccesary functions\n");

				if(antiddos){
					ENG_SYSCALL(G_PRINT, "ban <IP/range> <minutes> - bans IP via JASS\n");
					ENG_SYSCALL(G_PRINT, "unban <IP/range> - unbans IP via JASS. Type \"all\" to clear banlist\n");
					ENG_SYSCALL(G_PRINT, "banlist [list_num] [count] - shows banlist with\n");
				}
				return 1;
			} else if (!strcasecmp("status", arg1)) {
				ENG_SYSCALL(G_PRINT, "JASS v" JASS_VERSION " (" JASS_OS ") loaded\n");
				ENG_SYSCALL(G_PRINT, vaf("Mod: %s\n", g_EngineMgr->GetModDir()));
				ENG_SYSCALL(G_PRINT, "Built: " JASS_COMPILE " by " JASS_BUILDER "\n");
				ENG_SYSCALL(G_PRINT, "URL: http://jass.ucoz.net/\n");
				ENG_SYSCALL(G_PRINT, vaf("Loaded mod file: %s\n", g_ModMgr->Mod()->File()));
				ENG_SYSCALL(G_PRINT, vaf("Logs are %s\n", log_get()?"enabled":"disabled"));
				ENG_SYSCALL(G_PRINT, vaf("Logfile is %s\n", logfile_get()));
				ENG_SYSCALL(G_PRINT, vaf("Log cmds is %s\n", advanced_logs?"enabled":"disabled"));
				ENG_SYSCALL(G_PRINT, vaf("Fake detect is %s\n", fakedetect?"enabled":"disabled"));
				ENG_SYSCALL(G_PRINT, vaf("Maximum players with simular IP %d\n", maxplayersperip));
				//g_ModMgr->Mod()->Status();
			} else if (!strcasecmp("list", arg1)) {
				g_PluginMgr->ListPlugins();
			} else if (!strcasecmp("load", arg1)) {
				if (argc == 2)
					ENG_SYSCALL(G_PRINT, "load <file> - loads a new plugin\n");
				else
					g_PluginMgr->LoadPlugin(arg2, 1);
			} else if (!strcasecmp("info", arg1)) {
				if (argc == 2) {
					ENG_SYSCALL(G_PRINT, "info <id/str> - outputs info on plugin with id\n");
					return 1;
				}
				CPlugin* plugin = g_PluginMgr->FindPlugin(arg2);
				if (!plugin) {
					ENG_SYSCALL(G_PRINT, vaf("Unable to find unique plugin match for \"%s\"\n", arg2));
					return 1;
				}
				ENG_SYSCALL(G_PRINT, vaf("Plugin Info for \"%s\":\n", arg2));
				ENG_SYSCALL(G_PRINT, vaf("Name: \"%s\"\n", plugin->PluginInfo()->name));
				ENG_SYSCALL(G_PRINT, vaf("Version: \"%s\"\n", plugin->PluginInfo()->version));
				ENG_SYSCALL(G_PRINT, vaf("URL: \"%s\"\n", plugin->PluginInfo()->url));
				ENG_SYSCALL(G_PRINT, vaf("Author: \"%s\"\n", plugin->PluginInfo()->author));
				ENG_SYSCALL(G_PRINT, vaf("Desc: \"%s\"\n", plugin->PluginInfo()->desc));
				ENG_SYSCALL(G_PRINT, vaf("Pausable: %s\n", plugin->PluginInfo()->canpause ? "yes" : "no"));
				ENG_SYSCALL(G_PRINT, vaf("Cmd Loadable: %s\n", plugin->PluginInfo()->loadcmd ? "yes" : "no"));
				ENG_SYSCALL(G_PRINT, vaf("Cmd Unloadable: %s\n", plugin->PluginInfo()->unloadcmd ? "yes" : "no"));
				ENG_SYSCALL(G_PRINT, vaf("Loaded from: %s\n", plugin->IsCmd() ? "command" : "config"));
			} else if (!strcasecmp("pause", arg1)) {
				if (argc == 2) {
					ENG_SYSCALL(G_PRINT, "pause <id/str> - pauses plugin with id\n");
					return 1;
				}
				CPlugin* plugin = g_PluginMgr->FindPlugin(arg2);
				if (!plugin) {
					ENG_SYSCALL(G_PRINT, vaf("Unable to find unique plugin match for \"%s\"\n", arg2));
					return 1;
				}
				if (plugin->Pause())
					ENG_SYSCALL(G_PRINT, vaf("Plugin \"%s\" paused successfully\n", plugin->PluginInfo()->name));
				else
					ENG_SYSCALL(G_PRINT, vaf("Plugin \"%s\" not paused: not allowed or already paused\n", plugin->PluginInfo()->name));
			} else if (!strcasecmp("unpause", arg1)) {
				if (argc == 2) {
					ENG_SYSCALL(G_PRINT, "unpause <id/str> - unpauses plugin with id\n");
					return 1;
				}
				CPlugin* plugin = g_PluginMgr->FindPlugin(arg2);
				if (!plugin) {
					ENG_SYSCALL(G_PRINT, vaf("Unable to find unique plugin match for \"%s\"\n", arg2));
					return 1;
				}
				if (plugin->Unpause())
					ENG_SYSCALL(G_PRINT, vaf("Plugin \"%s\" unpaused successfully\n", plugin->PluginInfo()->name));
				else
					ENG_SYSCALL(G_PRINT, vaf("Plugin \"%s\" already unpaused\n", plugin->PluginInfo()->name));
			} else if (!strcasecmp("unload", arg1)) {
				if (argc == 2) {
					ENG_SYSCALL(G_PRINT, "unload <id/str> - unloads plugin with id\n");
					return 1;
				}
				CPlugin* plugin = g_PluginMgr->FindPlugin(arg2);
				if (!plugin) {
					ENG_SYSCALL(G_PRINT, vaf("Unable to find unique plugin match for \"%s\"\n", arg2));
					return 1;
				}
				char temp[MAX_PATH];
				strncpy(temp, plugin->PluginInfo()->name, sizeof(temp));
				if (g_PluginMgr->UnloadPlugin(plugin, 1))
					ENG_SYSCALL(G_PRINT, vaf("Plugin \"%s\" unloaded successfully\n", temp));
				else
					ENG_SYSCALL(G_PRINT, vaf("Plugin \"%s\" not unloaded: not allowed\n", plugin->PluginInfo()->name));
			} else if (!strcasecmp("force_unload", arg1)) {
				if (argc == 2) {
					ENG_SYSCALL(G_PRINT, "force_unload <id/str> - forcefully unloads plugin with id\n");
					return 1;
				}
				CPlugin* plugin = g_PluginMgr->FindPlugin(arg2);
				if (!plugin) {
					ENG_SYSCALL(G_PRINT, vaf("Unable to find unique plugin match for \"%s\"\n", arg2));
					return 1;
				}
				char temp[MAX_PATH];
				strncpy(temp, plugin->PluginInfo()->name, sizeof(temp));
				if (g_PluginMgr->UnloadPlugin(plugin, 0))
					ENG_SYSCALL(G_PRINT, vaf("Plugin \"%s\" forcefully unloaded successfully\n", temp));
				else
					ENG_SYSCALL(G_PRINT, vaf("Plugin \"%s\" not forcefully unloaded: unknown reason\n", plugin->PluginInfo()->name));
			} else if (!strcasecmp("shutdown", arg1)) {
				s_shutdown = 1;
				ENG_SYSCALL(G_PRINT, "Preparing JASS Shutdown mode\n");
				log_write("Preparing JASS Shutdown mode\n");
				if (g_PluginMgr) {
					ENG_SYSCALL(G_PRINT, "Shutting down plugins\n");
					log_write("Shutting down plugins\n");
					delete g_PluginMgr;
				}
				if (g_ConfigMgr->IsLibLoaded()) {
					ENG_SYSCALL(G_PRINT, "Unloading pdb system\n");
					log_write("Unloading pdb system\n");
					delete g_ConfigMgr;
				}
				ENG_SYSCALL(G_CVAR_REGISTER, NULL, "~jass_shutdown", "1", CVAR_ROM);
				ENG_SYSCALL(G_PRINT, "Entering JASS Shutdown mode\n");
				log_write("Entering JASS Shutdown mode\n");
			//if AntiDDoS plugin is loaded
			} else if (antiddos&&!strcasecmp("ban", arg1)){
				if (argc!=4){
					ENG_SYSCALL(G_PRINT, "Usage: jass ban <IP/range> <minutes>\n");
					return 1;
				}
				size_t ltime = atoi(arg3)*60000;
				plugin_interface AD_Call=antiddos->plugin_Call();
				if(AD_Call(AD_BAN,(int)arg2,ltime,0,0,0,0,0,0,0,0,0,0,0)){//it's bad example, but I'm lazy a bit
					ENG_SYSCALL(G_PRINT, "Banned.\n");
				}else{
					ENG_SYSCALL(G_PRINT, "Not banned.\n");
				}
			} else if (antiddos&&!strcasecmp("unban", arg1)){
				if (argc!=3){
					ENG_SYSCALL(G_PRINT, "Usage: jass unban <IP/all>\n");
					return 1;
				}
				plugin_interface AD_Call=antiddos->plugin_Call();
				AD_Call(AD_UNBAN,(int)arg2,0,0,0,0,0,0,0,0,0,0,0,0);
				ENG_SYSCALL(G_PRINT, "Removed from banlist.\n");


			} else if (antiddos&&!strcasecmp("banlist", arg1)){
				int num=1,ip_per_page=10;
				if (argc!=4){
					ENG_SYSCALL(G_PRINT, "jass banlist [list_num] [count]\n");
				}
				if(argc>=3){
					num=atoi(arg2);
					if(argc>3){
						ip_per_page=atoi(arg3);
					}
				}
				plugin_interface AD_Call=antiddos->plugin_Call();
				ENG_SYSCALL(G_PRINT, AD_Call(AD_BANLIST,num,ip_per_page,0,0,0,0,0,0,0,0,0,0,0));

			} else {
				ENG_SYSCALL(G_PRINT, "Unknown jass command.\n");
			}

		/*if (!strcasecmp(command,"jass_unban_all")){
			DeleteAllWarns();
			g_syscall(G_PRINT, "As you wish, Master\n");
			JASS_RET_SUPERCEDE(1);
		}
		else if (!strcasecmp(command,"jass_unban")){
		}
		else if (!strcasecmp(command,"jass_ban")){
			JASS_RET_SUPERCEDE(1);
		}
		else if (!strcasecmp(command,"jass_banlist")){
			JASS_RET_SUPERCEDE(1);
		}
			}*/

			return 1;
		}
	}

	//[Дополнительные логи]
	else if (advanced_logs && cmd == GAME_CLIENT_COMMAND) {
		char command[14];
		int argc = ENG_SYSCALL(G_ARGC);
		log_write(vaf("Cmd %d %s:",arg0,&names[NAME_LENGTH*arg0]));
		for(int i=0;i<argc;++i){
			char command[1024];
			ENG_SYSCALL(G_ARGV, i, command, sizeof(command));
			log_write(command);
		}
		log_write("\n");
	}
	//[/Дополнительные логи]

	else if (cmd == GAME_CLIENT_CONNECT && !arg2) {
			int i;
			char userinfo[ MAX_INFO ];

			//Процедура взятия userinfo
			ENG_SYSCALL(G_GET_USERINFO, arg0, userinfo, sizeof(userinfo));
			char name[NAME_LENGTH];
			for(i=0;i<NAME_LENGTH;++i)
				name[i]=' ';

			log_write(vaf("Client %d connected:\n",arg0));
			int pos=uinfo_search(userinfo,"name");
			//Используется специальный алгоритм, который при выводе игнорирует символы цвета
			if (pos>0&&pos<1010){
				int j=0;
				for(i=0;j<NAME_LENGTH&&i<64;++i){
					if(userinfo[pos+i]=='^'&&(userinfo[pos+i+1]>='0'&&userinfo[pos+i+1]<='9')){
						++i;
					}
					else{
						if (userinfo[pos+i]!='\\'&&userinfo[pos+i]!='\0'){
							name[j++]=userinfo[pos+i];
						}
						else{
							name[j]='\0';
							break;
						}
					}
				}
				if (j<NAME_LENGTH)
					name[j]='\0';
				else
					name[NAME_LENGTH]='\0';
				log_write(vaf("Name: \"%s\"\n",name));
				for (i=0;i<NAME_LENGTH;++i)
					if (name[i]!='\0')
						names[arg0*NAME_LENGTH+i]=name[i];
					else{
						names[arg0*NAME_LENGTH+i]='\0';
						break;
					}
			}
	//	}
	}

	//отправка vmMain плагинам
	int ret = g_PluginMgr->CallvmMain(cmd, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11);

	if (cmd == GAME_CLIENT_USERINFO_CHANGED) {
		char name[30];
		int i=0;
		char userinfo[1024];

		//bool check=true;
		ENG_SYSCALL(G_GET_USERINFO, arg0, userinfo, sizeof(userinfo)); // get from stored
		int pos=uinfo_search(userinfo,"name");
		int j=0;
		for(i=0;i<NAME_LENGTH;++i){
			if(userinfo[pos+i]=='^'&&(userinfo[pos+i+1]>='0'&&userinfo[pos+i+1]<='9'))
				++i;
			else{
				if (userinfo[pos+i]!='\\'&&userinfo[pos+i]!='\0'){
					name[j]=userinfo[pos+i];
					++j;
				}
				else{
					name[j]='\0';
					break;
				}
			}
		}
		if (strcasecmp(name,&names[arg0*NAME_LENGTH])){
			log_write (vaf("Client %d: \"%s\" renamed to \"%s\"\n",arg0,&names[arg0*NAME_LENGTH],name));
			for (i=0;i<NAME_LENGTH;++i)
				if (name[i]!='\0')
					names[arg0*NAME_LENGTH+i]=name[i];
				else{
					names[arg0*NAME_LENGTH+i]='\0';
					break;
				}
		}
	}

	//Приветствие игроков
	else if (cmd == GAME_CLIENT_CONNECT && arg1 && !arg2) {
		if (!g_ConfigMgr->GetInt("nogreeting")){
			if(!isFirstApril){
				ENG_SYSCALL(G_SEND_SERVER_COMMAND, arg0, "print \"^7This server is running ^4JASS^7 v^4" JASS_VERSION "^7\n\"");
				ENG_SYSCALL(G_SEND_SERVER_COMMAND, arg0, "print \"^7Jedi Academy Server Security without crazy damage\n\"");
				ENG_SYSCALL(G_SEND_SERVER_COMMAND, arg0, "print \"^7URL: ^4http://jass.ucoz.net/^7\n\"");
			}else{
				ENG_SYSCALL(G_SEND_SERVER_COMMAND, arg0, vaf("print \"%s\"",PrintRandomHK47Quote()));
			}
		}
	}

	//Игрок ушел
	else if (cmd == GAME_CLIENT_DISCONNECT){
		log_write (vaf("Client %d disconnected\n",arg0));
	}

	else if (cmd == GAME_SHUTDOWN) {
#ifdef WIN32
		char* homepath = "";
#else
		const char* homepath = g_EngineMgr->GetHomepath();
#endif
		FILE * f=fopen(vaf("%s%s/crash.dat",homepath,g_EngineMgr->GetModDir()),"w");
		fputc('n',f);
		fclose(f);
		log_write("Shutting JASS down\n");
		CloseLog();
		ENG_SYSCALL(G_PRINT, "Shutting down plugins\n");
		delete g_PluginMgr;

		ENG_SYSCALL(G_PRINT, "Shutting down mod\n");
		delete g_ModMgr;

		if (g_ConfigMgr->IsLibLoaded())
			ENG_SYSCALL(G_PRINT, "Unloading pdb system\n");
		delete g_ConfigMgr;

		ENG_SYSCALL(G_PRINT, "Finished shutting down, prepared for unload.\n");
		delete g_EngineMgr;
	}

	return ret;
}

int JASS_syscall(int cmd, ...) {
	va_list arglist;
	int args[13];
	va_start(arglist, cmd);
	for (int i = 0; i < (sizeof(args)/sizeof(args[0])); ++i)
		args[i] = va_arg(arglist, int);
	va_end(arglist);

	if (s_shutdown == 1)
		return ENG_SYSCALL(cmd, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9], args[10], args[11], args[12]);

	//Проверяем, не был ли закрыт файл логов
	if (cmd == G_FS_FCLOSE_FILE) {
		if (args[0] == gamelog_get()) {
			gamelog_set(-1);
		}
	}

	//Отправка сообщения плагинам
	int ret = g_PluginMgr->Callsyscall(cmd, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9], args[10], args[11], args[12]);

	//Поиск хендла для файлы логов
	if (cmd == G_FS_FOPEN_FILE) {
		if (args[2] == FS_APPEND || args[2] == FS_APPEND_SYNC) {
			if (!strcasecmp(get_str_cvar("g_log"), (char*)(args[0]))) {
				gamelog_set(*(int*)(args[1]));
			}
		}
	}

	return ret;
}

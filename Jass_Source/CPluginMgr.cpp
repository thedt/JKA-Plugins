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

/* v 1.0.1 27.08.2010 21:13:59 Zlyden' */

#include <stdlib.h>
#include "CEngineMgr.h"
#include "CPluginMgr.h"
#include "CPlugin.h"
#include "CModMgr.h"
#include "CLinkList.h"
#include "CConfigMgr.h"
#include "jass.h"
#include "util.h"
#include "jka/game/q_shared.h"
#include "jka/game/g_local.h"

CPluginMgr::CPluginMgr() {
	this->numplugins = 0;
}

CPluginMgr::~CPluginMgr() {
	this->UnloadPlugins();
}

int CPluginMgr::LoadPlugins() {
	if (!g_ConfigMgr->IsConfLoaded())
		return 0;

	void* linknode = g_ConfigMgr->GetListRootNode(vaf("%s/plugins", g_EngineMgr->GetModDir()));

	while (linknode) {
		this->numplugins += this->LoadPlugin(g_ConfigMgr->GetListNodeID(linknode), 0);
		
		linknode = g_ConfigMgr->GetListNextNode(linknode);
	}
	
	return this->numplugins;
}

int CPluginMgr::UnloadPlugins() {
	CLinkNode<CPlugin>* p = this->plugins.first();

	int i = 0;
	while (p) {
		p->data()->Unload(0);
		p = p->next();
		++i;
	}

	return i;
}

//Я не могу передавать указатель на (g_ModMgr->Mod()->vmMain()) плагинам
static int s_plugin_vmmain(int cmd, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10, int arg11) {
	return MOD_VMMAIN(cmd, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11);
}

// - file зависит от папки с модом (эта папка и homepath есть в CPlugin::LoadQuery)
// - iscmd означает, что плагин был загружен командой сервера

int CPluginMgr::LoadPlugin(const char* file, int iscmd) {
	if (!file || !*file)
		return 0;

	CPlugin* p = new CPlugin;
	
	if (!p->LoadQuery(file, iscmd)) {
		ENG_SYSCALL(G_PRINT, vaf("ERROR: CPluginMgr::LoadPlugin(\"%s\"): Unable to load plugin due to previous errors\n", file));
		delete p;
		return 0;
	}
	ENG_SYSCALL(G_PRINT, vaf("CPluginMgr::LoadPlugin(\"%s\"): Successfully queried plugin \"%s\"\n", file, p->PluginInfo()->name));

	if (!p->Attach(g_EngineMgr->SysCall(), s_plugin_vmmain, get_pluginfuncs(), iscmd)) {
		ENG_SYSCALL(G_PRINT, vaf("CPluginMgr::LoadPlugin(\"%s\"): JASS_Attach() returned 0 for plugin \"%s\"\n", file, p->PluginInfo()->name));
		p->Unload(0);
		delete p;
		return 0;
	}
	ENG_SYSCALL(G_PRINT, vaf("CPluginMgr::LoadPlugin(\"%s\"): Successfully attached plugin \"%s\"\n", file, p->PluginInfo()->name));

	this->plugins.add(p);
	return 1;
}

int CPluginMgr::UnloadPlugin(CPlugin* plugin, int iscmd) {
	if (!plugin)
		return 0;

	CLinkNode<CPlugin>* p = this->plugins.first();

	while (p) {
		if (p->data() == plugin) {
			if (p->data()->Unload(iscmd)) {
				this->plugins.del(p);
				return 1;
			}
			return 0;
		}
		p = p->next();
	}

	return 0;
}

CPlugin* CPluginMgr::FindPlugin(const char* string) {
	if (!string || !*string)
		return NULL;

	if (isnumber(string))
		return this->FindPlugin(atoi(string));

	CPlugin* found = NULL;

	CLinkNode<CPlugin>* p = this->plugins.first();

	while (p) {
		if (my_strcasestr(p->data()->PluginInfo()->name, string)) {
			if (!found)
				found = p->data();
			else
				return NULL;
		}
		p = p->next();
	}

	return found;
}

CPlugin* CPluginMgr::FindPlugin(int num) {
	if (num < 0)
		return NULL;

	int i = 0;

	CLinkNode<CPlugin>* p = this->plugins.first();

	while (p) {
		if (i == num)
			return p->data();
		++i;
		p = p->next();
	}

	return NULL;
}

int CPluginMgr::GetPluginID(const char *name){
	CLinkNode<CPlugin>* p = this->plugins.first();
	int i=0;

	while (p) {
		if (my_strcasestr(p->data()->PluginInfo()->name, name)) {
			return i;
		}
		++i;
		p = p->next();
	}
	//not found
	return -1;
}

const char* CPluginMgr::GetPluginInfo(int num){
	int i = 0;
	CLinkNode<CPlugin>* p = this->plugins.first();

	while (p) {
		if (i == num){
			return vaf("%s\n%s\n%s\n%s",p->data()->PluginInfo()->name,
				p->data()->PluginInfo()->version,p->data()->PluginInfo()->author,
				p->data()->PluginInfo()->url);
		}
		++i;
		p = p->next();
	}
	return NULL;
}

int CPluginMgr::NumPlugins() {
	return this->numplugins;
}

void CPluginMgr::ListPlugins() {
	ENG_SYSCALL(G_PRINT, "id - loadcmd / unloadcmd / canpause / paused / source - version - plugin\n");
	ENG_SYSCALL(G_PRINT, "------------------------------------------------------------------------\n");
	CLinkNode<CPlugin>* p = this->plugins.first();
	int num = 0;
	const plugininfo_t* info = NULL;
	while (p) {
		info = p->data()->PluginInfo();
		ENG_SYSCALL(G_PRINT, vaf("%.2d - %s / %s / %s / %s / %s - %7s - %s\n", num, info->loadcmd ? "  yes  " : "  no   ", info->unloadcmd ? "   yes   " : "   no    ", info->canpause ? "  yes   " : "   no   ", p->data()->Paused() ? " yes  " : "  no  ", p->data()->IsCmd() ? " cmd  " : " conf ", info->version, info->name));
		++num;
		p = p->next();
	}
}

int CPluginMgr::CallvmMain(int cmd, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10, int arg11) {
	if (!g_ModMgr->Mod())
		return 0;

	CLinkNode<CPlugin>* p = this->plugins.first();

	//сохранение max result
	pluginres_t maxresult = JASS_UNUSED;

	//сохранение возвращаемого результат для использования в вызове vmMain (для JASS_OVERRIDE)
	int ret = 0;

	int temp = 0;
	while (p) {
		if (p->data()->Paused()) {
			p = p->next();
			continue;
		}

		//Вызов vmMain плагина и сохранение результата
		temp = (p->data()->vmMain())(cmd, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11);

		//Проверка результата
		switch (p->data()->Result()) {
			//UNUSED показать предупреждение
			case JASS_UNUSED:
				ENG_SYSCALL(G_PRINT, vaf("WARNING: CPluginMgr::CallvmMain(%s): Plugin \"%s\" did not set result flag\n", ENG_MSGNAME(cmd), p->data()->PluginInfo()->name));
				break;

			//Ошибка
			case JASS_ERROR:
				ENG_SYSCALL(G_PRINT, vaf("ERROR: CPluginMgr::CallvmMain(%s): Plugin \"%s\" resulted in ERROR\n", ENG_MSGNAME(cmd), p->data()->PluginInfo()->name));
				break;
			
			//override: изменение maxresult и подмена результата на результат плагина
			case JASS_OVERRIDE:

			//supercede: изменение maxresult и подмена результата на результат плагина
			case JASS_SUPERCEDE:
				ret = temp;

			//ignored: плагин игнорирует вызов
			case JASS_IGNORED:
				if (maxresult < p->data()->Result())
					maxresult = p->data()->Result();
				break;

			default:
				ENG_SYSCALL(G_PRINT, vaf("ERROR: CPluginMgr::CallvmMain(%s): Plugin \"%s\" set unknown result flag \"%d\"\n", ENG_MSGNAME(cmd), p->data()->PluginInfo()->name, p->data()->Result()));
				break;
		}

		p->data()->ResetResult();
			
		p = p->next();
	}

	//вызов функции vmmain, затем зовется пост-функция плагина
	switch(maxresult) {
		//запуск vmMain мода и сохранение результата
		case JASS_UNUSED:
		case JASS_ERROR:
		case JASS_IGNORED:
		case JASS_OVERRIDE:
			temp = MOD_VMMAIN(cmd, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11);

			if (maxresult != JASS_OVERRIDE)
				ret = temp;
		case JASS_SUPERCEDE:
			p = this->plugins.first();
			while (p) {
				if (p->data()->Paused()) {
					p = p->next();
					continue;
				}

				//Вызов пост-функции
				(p->data()->vmMain_Post())(cmd, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11);
				if (p->data()->Result() == JASS_ERROR)
					ENG_SYSCALL(G_PRINT, vaf("ERROR: CPluginMgr::CallvmMain(%s): Plugin \"%s\" resulted in ERROR\n", ENG_MSGNAME(cmd), p->data()->PluginInfo()->name));
				
				p->data()->ResetResult();
				
				p = p->next();
			}

			return ret;
		//Чтобы это ни было, не трогаем
		default:
			return MOD_VMMAIN(cmd, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11);
	}
}

//Теперь все аналогично, но для вызова движковой syscall
int CPluginMgr::Callsyscall(int cmd, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10, int arg11, int arg12) {
	CLinkNode<CPlugin>* p = this->plugins.first();

	pluginres_t maxresult = JASS_UNUSED;

	//сохранение возвращаемого результат для использования в вызове syscall (для JASS_OVERRIDE)
	int ret = 0;

	int temp = 0;
	while (p) {
		if (p->data()->Paused()) {
			p = p->next();
			continue;
		}

		//Вызов syscall плагина и сохранение результата
		temp = (p->data()->syscall())(cmd, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12);

		//Проверка результата
		switch (p->data()->Result()) {
			//UNUSED показать предупреждение
			case JASS_UNUSED:
				ENG_SYSCALL(G_PRINT, vaf("WARNING: CPluginMgr::Callsyscall(%s): Plugin \"%s\" did not set result flag\n", ENG_MSGNAME(cmd), p->data()->PluginInfo()->name));
				break;

			//Ошибка
			case JASS_ERROR:
				ENG_SYSCALL(G_PRINT, vaf("ERROR: CPluginMgr::Callsyscall(%s): Plugin \"%s\" resulted in ERROR\n", ENG_MSGNAME(cmd), p->data()->PluginInfo()->name));
				break;
			
			//override: изменение maxresult и подмена результата на результат плагина
			case JASS_OVERRIDE:

			//supercede: изменение maxresult и подмена результата на результат плагина
			case JASS_SUPERCEDE:
				ret = temp;

			//ignored: изменение maxresult
			case JASS_IGNORED:
				if (maxresult < p->data()->Result())
					maxresult = p->data()->Result();
				break;

			default:
				ENG_SYSCALL(G_PRINT, vaf("ERROR: CPluginMgr::Callsyscall(%s): Plugin \"%s\" set unknown result flag \"%d\"\n", ENG_MSGNAME(cmd), p->data()->PluginInfo()->name, p->data()->Result()));
				break;
		}

		p->data()->ResetResult();
			
		p = p->next();
	}

	//вызов функции syscall, затем зовется пост-функция плагина
	switch(maxresult) {
		//Запуск syscall и сохранение результата
		case JASS_UNUSED:
		case JASS_ERROR:
		case JASS_IGNORED:
		case JASS_OVERRIDE:
			temp = ENG_SYSCALL(cmd, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12);
			if (maxresult != JASS_OVERRIDE)
				ret = temp;
		case JASS_SUPERCEDE:
			p = this->plugins.first();
			while (p) {
				if (p->data()->Paused()) {
					p = p->next();
					continue;
				}

				//Вызов пост-функции плагина
				(p->data()->syscall_Post())(cmd, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12);
				if (p->data()->Result() == JASS_ERROR)
					ENG_SYSCALL(G_PRINT, vaf("ERROR: CPluginMgr::Callsyscall(%s): Plugin \"%s\" resulted in ERROR\n", ENG_MSGNAME(cmd), p->data()->PluginInfo()->name));
				
				p->data()->ResetResult();
				
				p = p->next();
			}
			
			return ret;
		//Чтобы это ни было, не трогаем
		default:
			return ENG_SYSCALL(cmd, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12);
	}
}

CPluginMgr* CPluginMgr::GetInstance() {
	if (!CPluginMgr::instance)
		CPluginMgr::instance = new CPluginMgr;

	return CPluginMgr::instance;
}

CPluginMgr* CPluginMgr::instance = NULL;

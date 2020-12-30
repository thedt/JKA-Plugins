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
//       Zlyden' <zlyden00@mail.ru>

/* v 1.0.1 27.08.2010 21:13:59 Zlyden' */

#include "CPlugin.h"
#include "jass.h"
#include "util.h"
#include "jka/game/q_shared.h"
#include "jka/game/g_local.h"

CPlugin::CPlugin() {
	this->JASS_Query = NULL;
	this->JASS_Attach = NULL;
	this->JASS_Detach = NULL;
	this->JASS_vmMain = NULL;
	this->JASS_vmMain_Post = NULL;
	this->JASS_syscall = NULL;
	this->JASS_syscall_Post = NULL;
	this->JASS_plugin_Call = NULL;
	this->plugininfo = NULL;
	this->paused = 0;
	this->iscmd = 0;
}

CPlugin::~CPlugin() {
	this->dll.Unload();
}


//Загружаем данный файл и зовем функцию JASS_Query function
//Если плагин невозможно загрузить во время работы и iscmd==1, отмена

// - путь зависит от папки с модом
// - iscmd означает, что плагин был загружен командой сервера
int CPlugin::LoadQuery(const char* file, int iscmd) {
	if (!file || !*file)
		return 0;

	//Загрузка DLL
	int x = this->dll.Load(vaf("%s%s/%s", g_EngineMgr->GetHomepath(), g_EngineMgr->GetModDir(), file));
	if (!x) {
		PrintNLog(vaf("ERROR: CPlugin::LoadQuery(\"%s\"): DLL load failed for plugin: %s\n", file, dlerror()));
		return 0;
	} else if (x == -1) {
		PrintNLog(vaf("ERROR: CPlugin::LoadQuery(\"%s\"): DLL load failed for plugin: module already loaded\n", file));
		return 0;
	}

	//поиск JASS_Query()
	if ((this->JASS_Query = (plugin_query)this->dll.GetProc("JASS_Query")) == NULL) {
		PrintNLog(vaf("ERROR: CPlugin::LoadQuery(\"%s\"): Unable to find \"JASS_Query\" function in plugin\n", file));
		return 0;
	}

	//Вызов JASS_Query(), чтобы взять информацию о плагине
	(this->JASS_Query)(&(this->plugininfo));

	if (!this->plugininfo) {
		PrintNLog(vaf("ERROR: CPlugin::LoadQuery(\"%s\"): Plugininfo NULL for plugin", file));
		return 0;
	}

	//проверка версии интерфейса

	//Если старшая версия интерфейса плагина выше, предлагаем обновить JASS
	if (this->plugininfo->pifv_major > JASS_PIFV_MAJOR) {
		PrintNLog(vaf("ERROR: CPlugin::LoadQuery(\"%s\"): Plugin's major interface version (%d) is greater than JASS's (%d), suggest upgrading JASS.\n", file, this->plugininfo->pifv_major, JASS_PIFV_MAJOR));
		return 0;
	}
	//Если старшая версия интерфейса плагина ниже, предлагаем обновить плагин
	if (this->plugininfo->pifv_major < JASS_PIFV_MAJOR) {
		PrintNLog(vaf("ERROR: CPlugin::LoadQuery(\"%s\"): Plugin's major interface version (%d) is less than JASS's (%d), suggest upgrading plugin.\n", file, this->plugininfo->pifv_major, JASS_PIFV_MAJOR));
		return 0;
	}
	//Если младшая версия интерфейса плагина выше, предлагаем обновить JASS
	if (this->plugininfo->pifv_minor > JASS_PIFV_MINOR) {
		PrintNLog(vaf("ERROR: CPlugin::LoadQuery(\"%s\"): Plugin's minor interface version (%d) is greater than JASS's (%d), suggest upgrading JASS.\n", file, this->plugininfo->pifv_minor, JASS_PIFV_MINOR));
		return 0;
	}
	//Если младшая версия интерфейса плагина ниже, предлагаем обновить плагин
	if (this->plugininfo->pifv_minor < JASS_PIFV_MINOR)
		PrintNLog(vaf("WARNING: CPlugin::LoadQuery(\"%s\"): Plugin's minor interface version (%d) is less than JASS's (%d), suggest upgrading plugin.\n", file, this->plugininfo->pifv_minor, JASS_PIFV_MINOR));

	//Если плагин был загружен через команду, но плагин этого не допускает, отмена
	if (iscmd && !this->plugininfo->loadcmd) {
		PrintNLog(vaf("ERROR: CPlugin::LoadQuery(\"%s\"): Plugin cannot be loaded via command\n", file));
		return 0;
	}

	//Поиск остальных необходимых функций
	if ((this->JASS_Attach = (plugin_attach)this->dll.GetProc("JASS_Attach")) == NULL) {
		PrintNLog(vaf("ERROR: CPlugin::LoadQuery(\"%s\"): Unable to find \"JASS_Attach\" function in plugin\n", file));
		return 0;
	}
	if ((this->JASS_Detach = (plugin_detach)this->dll.GetProc("JASS_Detach")) == NULL) {
		PrintNLog(vaf("ERROR: CPlugin::LoadQuery(\"%s\"): Unable to find \"JASS_Detach\" function in plugin\n", file));
		return 0;
	}
	if ((this->JASS_vmMain = (plugin_vmmain)this->dll.GetProc("JASS_vmMain")) == NULL) {
		ENG_SYSCALL(G_PRINT, vaf("ERROR: CPlugin::LoadQuery(\"%s\"): Unable to find \"JASS_vmMain\" function in plugin\n", file));
		return 0;
	}
	if ((this->JASS_syscall = (plugin_syscall)this->dll.GetProc("JASS_syscall")) == NULL) {
		PrintNLog(vaf("ERROR: CPlugin::LoadQuery(\"%s\"): Unable to find \"JASS_syscall\" function in plugin\n", file));
		return 0;
	}
	if ((this->JASS_vmMain_Post = (plugin_vmmain)this->dll.GetProc("JASS_vmMain_Post")) == NULL) {
		PrintNLog(vaf("ERROR: CPlugin::LoadQuery(\"%s\"): Unable to find \"JASS_vmMain_Post\" function in plugin\n", file));
		return 0;
	}
	if ((this->JASS_syscall_Post = (plugin_syscall)this->dll.GetProc("JASS_syscall_Post")) == NULL) {
		PrintNLog(vaf("ERROR: CPlugin::LoadQuery(\"%s\"): Unable to find \"JASS_syscall_Post\" function in plugin\n", file));
		return 0;
	}

	this->JASS_plugin_Call = (plugin_interface)this->dll.GetProc("JASS_pluginCall");

	return 1;
}

//Вызов плагиновой JASS_Attach() с syscall движка, vmMain мода, результатом ptr, и iscmd
int CPlugin::Attach(eng_syscall_t eng_syscall, mod_vmMain_t mod_vmMain, pluginfuncs_t* pluginfuncs, int iscmd) {
	//сохранение iscmd для "jass info"
	this->iscmd = iscmd;

	//вызов JASS_Attach() с syscall движка, vmMain мода, результатом ptr, и iscmd
	return (this->JASS_Attach)(eng_syscall, mod_vmMain, &this->result, pluginfuncs, iscmd);
}

//Пауза плагина (если это возможно)
int CPlugin::Pause() {
	if (!this->plugininfo->canpause || this->paused)
		return 0;

	this->paused = 1;

	return 1;
}

//Запуск плагина
int CPlugin::Unpause() {
	if (!this->paused)
		return 0;

	this->paused = 0;

	return 1;
}

//Выгрузка плагина (через iscmd)
int CPlugin::Unload(int iscmd) {
	if (iscmd && (this->plugininfo && !this->plugininfo->unloadcmd))
		return 0;

	//Вызов JASS_Detach() с iscmd
	if (this->JASS_Detach)
		(this->JASS_Detach)(iscmd);

	//выгрузка DLL
	this->dll.Unload();

	return 1;
}

int CPlugin::Paused() {
	return this->paused;
}

int CPlugin::IsCmd() {
	return this->iscmd;
}

plugin_vmmain CPlugin::vmMain() {
	return this->JASS_vmMain;
}

plugin_vmmain CPlugin::vmMain_Post() {
	return this->JASS_vmMain_Post;
}

plugin_syscall CPlugin::syscall() {
	return this->JASS_syscall;
}

plugin_syscall CPlugin::syscall_Post() {
	return this->JASS_syscall_Post;
}

plugin_interface CPlugin::plugin_Call() {
	return this->JASS_plugin_Call;
}

const plugininfo_t* CPlugin::PluginInfo() {
	return this->plugininfo;
}

pluginres_t CPlugin::Result() {
	return this->result;
}

void CPlugin::ResetResult() {
	this->result = JASS_UNUSED;
}

pluginres_t CPlugin::result = JASS_UNUSED;

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

#include <stddef.h>
#include <string.h>
#include "osdef.h"
#include "CConfigMgr.h"
#include "CEngineMgr.h"
#include "CModMgr.h"
#include "CDLLMod.h"
#include "game_api.h"
#include "util.h"
#include "jass.h"
#ifdef WIN32
#include "jka\game\q_shared.h"
#include "jka\game\g_local.h"
#else
#include <jka/game/q_shared.h>
#include <jka/game/g_local.h>
#endif

CModMgr::CModMgr(eng_syscall_t jass_syscall, msgname_t msgnames) {
	this->jass_syscall = jass_syscall;
	this->msgnames = msgnames;

	this->mod = NULL;
}

CModMgr::~CModMgr() {
	this->UnloadMod();
}

// - file - путь, зависящий от нахождения мода
//использует функции движка, чтобы открыть файл вне зависимости от homepath
CDLLMod* CModMgr::newmod(const char* file) {
	CDLLMod* ret = new CDLLMod;	
	return ret;
}

//Попытка загрузить мод в следующем порядке поиска:
// - Мод, определенный в конфиге
// - dll мод загружается из homepath, затем в папке установки
// - dll/so под названием jass_<modfilename> в homepath
// - dll/so под названием jass_<modfilename> в папке установки
int CModMgr::LoadMod() {

	//Мод, определенный в конфиге
	char* cfg_mod = g_ConfigMgr->GetStr(vaf("%s/mod", g_EngineMgr->GetModDir()));
	
	if (cfg_mod && *cfg_mod) {
		ENG_SYSCALL(G_PRINT, vaf("CModMgr::LoadMod(): Mod file specified in configuration file: \"%s\"\n", cfg_mod));

		//определение типа мода
		this->mod = this->newmod(cfg_mod);

		//если тип определен
		if (this->mod) {
			//homepath
			if (this->mod->LoadMod(vaf("%s%s/%s", g_EngineMgr->GetHomepath(), g_EngineMgr->GetModDir(), cfg_mod)))
				return 1;

			//папка установки
			if (g_EngineMgr->GetHomepath()[0]) {
				ENG_SYSCALL(G_PRINT, vaf("ERROR: CModMgr::LoadMod(): Unable to load mod file \"%s\" in homepath, checking install directory\n", cfg_mod));
				if (this->mod->LoadMod(vaf("%s/%s", g_EngineMgr->GetModDir(), cfg_mod)))
					return 1;

				//fail
				ENG_SYSCALL(G_PRINT, vaf("ERROR: CModMgr::LoadMod(): Unable to load mod file \"%s\" in install directory\n", cfg_mod));
			}

			//попытка загрузить мод по стандартному имени
			ENG_SYSCALL(G_PRINT, vaf("ERROR: CModMgr::LoadMod(): Unable to load mod file \"%s\", attempting to load default DLL mod file \"jass_%s\"\n", cfg_mod, g_EngineMgr->GetDLLName()));
		
		//мод не определен
		} else {
			ENG_SYSCALL(G_PRINT, vaf("ERROR: CModMgr::LoadMod(): Unable to determine mod type of file \"%s\"\n", cfg_mod));
		}
	} else {
		ENG_SYSCALL(G_PRINT, vaf("CModMgr::LoadMod(): Unable to detect mod file setting from configuration file, attempting to load default DLL mod file \"jass_%s\"\n", g_EngineMgr->GetDLLName()));
	}

	//пытаемся загрузить jass_<dllname>
	cfg_mod = vaf("jass_%s", g_EngineMgr->GetDLLName());

	this->mod = new CDLLMod;

	//Сначала homepath
	if (this->mod->LoadMod(vaf("%s%s/%s", g_EngineMgr->GetHomepath(), g_EngineMgr->GetModDir(), cfg_mod)))
		return 1;

	//Затем установочная папка
	if (g_EngineMgr->GetHomepath()[0]) {
		ENG_SYSCALL(G_PRINT, vaf("ERROR: CModMgr::LoadMod(): Unable to load mod file \"%s\" in homepath, checking install directory\n", cfg_mod));
		if (this->mod->LoadMod(vaf("%s/%s", g_EngineMgr->GetModDir(), cfg_mod)))
			return 1;

		//fail
		ENG_SYSCALL(G_PRINT, vaf("ERROR: CModMgr::LoadMod(): Unable to load mod file \"%s\" in install directory\n", cfg_mod));
	}

	delete this->mod;

	ENG_SYSCALL(G_ERROR, "FATAL ERROR: Unable to load mod file\n");

	return 0;
}

void CModMgr::UnloadMod() {
	if (this->mod)
		delete this->mod;
}

eng_syscall_t CModMgr::JASS_SysCall() {
	return this->jass_syscall;
}

CDLLMod* CModMgr::Mod() {
	return this->mod;
}

const char* CModMgr::GetMsgName(int msg) {
	return this->msgnames(msg);
}

CModMgr* CModMgr::GetInstance(eng_syscall_t jass_syscall, msgname_t msgnames) {
	if (!CModMgr::instance)
		CModMgr::instance = new CModMgr(jass_syscall, msgnames);

	return CModMgr::instance;
}

CModMgr* CModMgr::instance = NULL;

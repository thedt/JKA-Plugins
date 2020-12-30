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

#include <string.h>
#include "osdef.h"
#include "jass.h"
#include "CModMgr.h"
#include "CEngineMgr.h"
#include "game_api.h"
#include "jassapi.h"
#include "CDLL.h"
#include "CDLLMod.h"
#include "util.h"
#ifdef WIN32
#include "jka\game\q_shared.h"
#include "jka\game\g_local.h""
#else
#include <jka/game/q_shared.h>
#include <jka/game/g_local.h>
#endif

CDLLMod::CDLLMod() {
	memset(this->file, 0, sizeof(this->file));
}

CDLLMod::~CDLLMod() {
	this->dll.Unload();
}

int CDLLMod::LoadMod(const char* file) {
	if (!file || !*file)
		return 0;

	strncpy(this->file, file, sizeof(this->file));
	
	/*загрузка dll*/
	int x = this->dll.Load(this->file);

	/*файл не найден*/
	if (!x) {
		ENG_SYSCALL(G_PRINT, vaf("ERROR: CDLLMod::LoadMod(\"%s\"): Unable to load mod file: %s\n", file, dlerror()));
		return 0;
	} else if (x == -1) {
		ENG_SYSCALL(G_PRINT, vaf("ERROR: CDLLMod::LoadMod(\"%s\"): Unable to load mod file: module already loaded\n", file));
		return 0;
	}

	//устанавливает место dllEntry() или вылетает
	if ((this->pfndllEntry = (mod_dllEntry_t)this->dll.GetProc("dllEntry")) == NULL) {
		ENG_SYSCALL(G_PRINT, vaf("ERROR: CDLLMod::LoadMod(\"%s\"): Unable to locate dllEntry() mod entry point\n", file));
		return 0;
	}

	//устанавливает место vmMain() или вылетает
	if ((this->pfnvmMain = (mod_vmMain_t)this->dll.GetProc("vmMain")) == NULL) {
		ENG_SYSCALL(G_PRINT, vaf("ERROR: CDLLMod::LoadMod(\"%s\"): Unable to locate vmMain() mod entry point\n", file));
		return 0;
	}

	//Запуск dllEntry мода
	this->pfndllEntry(g_ModMgr->JASS_SysCall());

	return 1;
}

int CDLLMod::vmMain(int cmd, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10, int arg11) {
	return this->pfnvmMain ? this->pfnvmMain(cmd, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11) : 0;
}

const char* CDLLMod::File() {
	return this->file[0] ? this->file : NULL;
}

//void CDLLMod::Status() {
//	ENG_SYSCALL(G_PRINT, vaf("dllEntry() offset: %p\n", this->pfndllEntry));
//	ENG_SYSCALL(G_PRINT, vaf("vmMain() offset: %p\n", this->pfnvmMain));
//}

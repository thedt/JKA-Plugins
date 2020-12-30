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

/* v 1.0 06.07.2010 19:00:59 Zlyden' */

#ifndef __CMODMGR_H__
#define __CMODMGR_H__

#include "osdef.h"
#include "CDLLMod.h"
#include "game_api.h"
#include "jassapi.h"

class CModMgr {
	public:
		CModMgr(eng_syscall_t jass_syscall, msgname_t msgnames);
		~CModMgr();

		int LoadMod();
		void UnloadMod();

		eng_syscall_t JASS_SysCall();

		CDLLMod* Mod();

		const char* GetMsgName(int msg);

		static CModMgr* GetInstance(eng_syscall_t jass_syscall, msgname_t msgnames);

	private:
		CDLLMod* mod;
		msgname_t msgnames;
		eng_syscall_t jass_syscall;
		CDLLMod* newmod(const char*);

		static CModMgr* instance;
};

#endif
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

#ifndef __CENGINEMGR_H__
#define __CENGINEMGR_H__

#include "jassapi.h"
#include "game_api.h"

class CEngineMgr {
	public:
		CEngineMgr(eng_syscall_t syscall, const char* dllname, const char* basedir, msgname_t msgnames, const char* homepath);
		~CEngineMgr();
		
		eng_syscall_t SysCall();
		const char* GetDLLName();
		const char* GetModDir();
		const char* GetBaseDir();
		const char* GetMsgName(int msg);
		const char* GetHomepath();

		static CEngineMgr* GetInstance(eng_syscall_t syscall, const char* dllname, const char* basedir, msgname_t msgnames, const char* homepath);

	private:
		eng_syscall_t pfnsyscall;
		const char* dllname;
		msgname_t msgnames;
		const char* basedir;
		const char* homepath;

		static CEngineMgr* instance;
};

#endif

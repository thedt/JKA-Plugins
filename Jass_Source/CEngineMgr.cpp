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
#include "CEngineMgr.h"
#include "game_api.h"
#include "jassapi.h"
#include "osdef.h"
#ifdef WIN32
#include "jka\game\q_shared.h"
#include "jka\game\g_local.h"
#else
#include <jka/game/q_shared.h>
#include <jka/game/g_local.h>
#endif

CEngineMgr::CEngineMgr(eng_syscall_t syscall, const char* dllname, const char* basedir, msgname_t msgnames, const char* homepath) {
	this->pfnsyscall = syscall;
	this->dllname = dllname;
	this->basedir = basedir;
	this->msgnames = msgnames;
	this->homepath = homepath;
}

CEngineMgr::~CEngineMgr() {
}

eng_syscall_t CEngineMgr::SysCall() {
	return this->pfnsyscall;
}

const char* CEngineMgr::GetDLLName() {
	return this->dllname;
}

const char* CEngineMgr::GetModDir() {
	static char dir[MAX_PATH] = "";
	if (dir[0]) return dir;

	this->pfnsyscall(G_CVAR_VARIABLE_STRING_BUFFER, "fs_game", dir, sizeof(dir));
	if (!dir[0])
		strncpy(dir, this->basedir, sizeof(dir));

	return dir;
}

const char* CEngineMgr::GetBaseDir() {
	return this->basedir;
}

const char* CEngineMgr::GetMsgName(int msg) {
	return this->msgnames(msg);
}

const char* CEngineMgr::GetHomepath() {
	return this->homepath;
}

CEngineMgr* CEngineMgr::GetInstance(eng_syscall_t syscall, const char* dllname, const char* basedir, msgname_t msgnames, const char* homepath) {
	if (!CEngineMgr::instance)
		CEngineMgr::instance = new CEngineMgr(syscall, dllname, basedir, msgnames, homepath);

	return CEngineMgr::instance;
}

CEngineMgr* CEngineMgr::instance = NULL;

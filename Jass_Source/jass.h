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

#ifndef __JASS_H__
#define __JASS_H__

#include "CEngineMgr.h"
#include "CModMgr.h"
#include "CPluginMgr.h"
#include "CConfigMgr.h"
#include "jassapi.h"

pluginfuncs_t* get_pluginfuncs();

typedef unsigned char byte;

extern CEngineMgr* g_EngineMgr;
extern CModMgr* g_ModMgr;
extern CPluginMgr* g_PluginMgr;
extern CConfigMgr* g_ConfigMgr;

#define ENG_MSGNAME	g_EngineMgr->GetMsgName
#define MOD_MSGNAME	g_ModMgr->GetMsgName

#define ENG_SYSCALL	(g_EngineMgr->SysCall())
#define MOD_VMMAIN	g_ModMgr->Mod()->vmMain
#define	JASS_SYSCALL	(g_ModMgr->JASS_SysCall())

C_DLLEXPORT void dllEntry(eng_syscall_t);
C_DLLEXPORT int vmMain(int, int, int, int, int, int, int, int, int, int, int, int, int);
int JASS_syscall(int, ...);

#endif
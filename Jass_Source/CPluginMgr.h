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

#ifndef __CPLUGINMGR_H__
#define __CPLUGINMGR_H__

#include "CPlugin.h"
#include "CLinkList.h"

class CPluginMgr {
	public:
		CPluginMgr();
		~CPluginMgr();

		int LoadPlugins();
		int UnloadPlugins();

		int LoadPlugin(const char*, int);
		int UnloadPlugin(CPlugin*, int);

		CPlugin* FindPlugin(int);
		CPlugin* FindPlugin(const char*);
		int GetPluginID(const char*);
		const char* GetPluginInfo(int);

		int NumPlugins();
		void ListPlugins();

		int CallvmMain(int, int, int, int, int, int, int, int, int, int, int, int, int);
		int Callsyscall(int, int, int, int, int, int, int, int, int, int, int, int, int, int);

		static CPluginMgr* GetInstance();

	private:
		CLinkList<CPlugin> plugins;
		int numplugins;

		static CPluginMgr* instance;
};
#endif
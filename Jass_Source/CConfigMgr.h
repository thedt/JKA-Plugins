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

#ifndef __CCONFIGMGR_H__
#define __CCONFIGMGR_H__

#include "osdef.h"
#include "pdb.h"

class CConfigMgr {
	public:
		CConfigMgr();
		~CConfigMgr();
		
		void LoadLib(const char*);
		void LoadConf(const char*);
		int IsLibLoaded();
		int IsConfLoaded();
		void UnloadConf();
		void UnloadLib();

		int GetInt(const char*, int = 0);
		char* GetStr(const char*);
		void* GetListRootNode(const char*);
		void* GetListNextNode(void*);
		char* GetListNodeID(void*);

		static CConfigMgr* GetInstance();

	private:
		struct pdb* db;
		int isloaded;

		static CConfigMgr* instance;
};

#endif

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

#ifndef __CDLLMOD_H__
#define __CDLLMOD_H__

#include "osdef.h"
#include "CDLL.h"
#include "jassapi.h"

class CDLLMod {
	public:
		CDLLMod();
		~CDLLMod();

		int LoadMod(const char*);

		int vmMain(int cmd, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10, int arg11);

		const char* File();

//		void Status();

	private:
		char file[MAX_PATH];
		CDLL dll;
		mod_vmMain_t pfnvmMain;
		mod_dllEntry_t pfndllEntry;
};

#endif

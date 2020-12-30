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

#ifndef __CPLUGIN_H__
#define __CPLUGIN_H__

#include <string.h>
#include "CDLL.h"
#include "jassapi.h"
#include "osdef.h"

class CPlugin {
	public:
		CPlugin();
		~CPlugin();

		int LoadQuery(const char*, int);
		int Attach(eng_syscall_t, mod_vmMain_t, pluginfuncs_t*, int);
		int Pause();
		int Unpause();
		int Unload(int);

		int Paused();

		int IsCmd();

		plugin_vmmain vmMain();
		plugin_vmmain vmMain_Post();
		plugin_syscall syscall();
		plugin_syscall syscall_Post();
		plugin_interface plugin_Call();

		const plugininfo_t* PluginInfo();

		pluginres_t Result();
		void ResetResult();

	private:
		CDLL dll;
		plugin_query JASS_Query;
		plugin_attach JASS_Attach;
		plugin_detach JASS_Detach;
		plugin_vmmain JASS_vmMain;
		plugin_vmmain JASS_vmMain_Post;
		plugin_syscall JASS_syscall;
		plugin_syscall JASS_syscall_Post;
		plugin_interface JASS_plugin_Call;
		plugininfo_t* plugininfo;
		int paused;
		int iscmd;
		static pluginres_t result;
};
#endif

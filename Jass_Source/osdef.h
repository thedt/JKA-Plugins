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

#ifndef __OSDEF_H__
#define __OSDEF_H__

#ifdef _WIN32
 #ifndef WIN32
  #define WIN32
 #endif
#endif

#ifdef WIN32
 #ifdef linux
  #undef linux
 #endif
#else
 #ifndef linux
  #define linux
 #endif
#endif

#ifdef WIN32
 #define WIN32_LEAN_AND_MEAN
 #include <windows.h>
 #define DLL_SUF		"x86"
 #define PDB_LIB		"pdb.dll"
 #define DLL_EXT		".dll"
 //#define vsnprintf		_vsnprintf
 #define strcasecmp		stricmp
 #define strncasecmp		strnicmp
 #define dlopen(file, x)	(void*)LoadLibrary(file)
 #define dlsym(dll, func)	(void*)GetProcAddress((HMODULE)(dll), (func))
 #define dlclose(dll)		FreeLibrary((HMODULE)(dll))
 char* dlerror();

 #pragma warning(disable:4710)	//"function * not inlined"

#else

 #include <dlfcn.h>
 #include <limits.h>
 #include <ctype.h>
 #define DLL_SUF		"i386"
 #define PDB_LIB		"./pdb.so"
 //#define PDB_LIB		"pdb.so"
 #define DLL_EXT		".so"
 #define MAX_PATH		PATH_MAX
#endif

#endif

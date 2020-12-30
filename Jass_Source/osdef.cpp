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

#include "osdef.h"

#ifdef WIN32

#include "version.h"
#pragma comment(exestr, "\1JASS v" JASS_VERSION " (" JASS_OS ")")
#pragma comment(exestr, "\1Built: " JASS_COMPILE " by " JASS_BUILDER)
#pragma comment(exestr, "\1URL: http://jass.ucoz.com/")

char* dlerror() {
	static char buffer[1024];
	memset(buffer, 0, sizeof(buffer));

	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buffer, sizeof(buffer), NULL);
	
	buffer[strlen(buffer) - 2] = '\0';

	return buffer;
}
#endif

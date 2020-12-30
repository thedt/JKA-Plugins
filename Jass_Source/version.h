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

#ifndef __VERSION_H__
#define __VERSION_H__

#define JASS_VERSION_MAJOR	3
#define JASS_VERSION_MINOR	2
#define JASS_VERSION_REV	0

#define JASS_VERSION		"3.2.0"

#define JASS_COMPILE		__TIME__ " " __DATE__
#define JASS_BUILDER		"-1.#IND0000"

#ifdef WIN32
 #define JASS_OS			"Win32"
#elif defined(linux)
 #define JASS_OS			"Linux"
#endif

//Автоматом
#define JASS_VERSION_DWORD	JASS_VERSION_MAJOR , JASS_VERSION_MINOR , JASS_VERSION_REV , 0

#endif

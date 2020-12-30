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

#ifndef __GAME_API_H__
#define __GAME_API_H__

typedef const char* (*msgname_t)(int);

//Информация об игре
typedef struct gameinfo_s {
	const char* dllname;		//стандартное имя dll
	const char* basedir;		//стандартная папка
	msgname_t eng_msg_names;	//указатель на функцию-преобразователь сообщений движка в строковые
	msgname_t mod_msg_names;	//указатель на функцию-преобразователь сообщений мода в строковые
	const char* althomepath;	//альтернативный дом для линукса (винда - 0)
} gameinfo_t;

extern gameinfo_t g_GameInfo;

const char* JKA_eng_msg_names(int);
const char* JKA_mod_msg_names(int);

#endif

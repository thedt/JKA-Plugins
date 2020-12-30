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

#ifndef __UTIL_H__
#define __UTIL_H__
#define	CVAR_ROM			0x00000040
#define CVAR_INTERNAL		0x00000800

const char* get_modulename();
const char* get_modulepath();
void* get_modulehandle();
int ismoduleloaded(void*);
void setmoduleloaded(void*);
void setmoduleunloaded(void*);
char* vaf(char*, ...);
int get_int_cvar(const char*);
const char* get_str_cvar(const char*);
void log_set(int);
int log_get();
void gamelog_set(int);
int gamelog_get();
void logfile_set(const char *);
const char * logfile_get();
int log_write(const char*);
void CloseLog();
int uinfo_search(char * userinfo, char * search);
int gamelog_write(const char*);
const char* my_strcasestr(const char*, const char*);
int isnumber(const char*);
char *PrintRandomHK47Quote();
char *ReturnOfTheJedi();
void PrintNLog(char *str);
#endif

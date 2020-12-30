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

#include <stdarg.h>
#ifdef WIN32
#include <io.h>
#else
#include <unistd.h>
#endif
#include <time.h>
#include <stdio.h>
#include <string.h>
#include "osdef.h"
#include "CLinkList.h"
#include "jass.h"
#include "util.h"
#include "jka/game/q_shared.h"
#include "jka/game/g_local.h"


static CLinkList<byte> s_plugins;
time_t ltime_global=0;

const char* get_modulename() {
	static char name[MAX_PATH] = "";
	if (name[0])
		return name;

	const char* pname = name;

#ifdef WIN32
	MEMORY_BASIC_INFORMATION MBI;

	if (!VirtualQuery((void*)&get_modulename, &MBI, sizeof(MBI)) || MBI.State != MEM_COMMIT || !MBI.AllocationBase)
		return NULL;

	if(!GetModuleFileName((HMODULE)MBI.AllocationBase, name, sizeof(name)) || !name[0])
		return NULL;

	name[sizeof(name)-1] = '\0';

	int slen = strlen(name);
	for (int i = slen; i >= 0; --i) {
		if (name[i] == '\\') {
			pname = &name[i + 1];
			break;
		}
	}
#else
	Dl_info dli;
	memset(&dli, 0, sizeof(dli));

	if (!dladdr((void*)&get_modulename, &dli))
		return NULL;

	int slen = strlen(dli.dli_fname);
	for (int i = slen; i >= 0; --i) {
		if (dli.dli_fname[i] == '/') {
			++i;
			for (int j = 0; i+j <= slen; ++j)
				name[j] = dli.dli_fname[i+j];
			break;
		}
	}

	if (!name[0])
		strncpy(name, dli.dli_fname, sizeof(name));
#endif
	return pname;
}

const char* get_modulepath() {
	static char name[MAX_PATH] = "";
	if (name[0])
		return name;

#ifdef WIN32
	MEMORY_BASIC_INFORMATION MBI;

	if (!VirtualQuery((void*)&get_modulename, &MBI, sizeof(MBI)) || MBI.State != MEM_COMMIT || !MBI.AllocationBase)
		return NULL;

	if(!GetModuleFileName((HMODULE)MBI.AllocationBase, name, sizeof(name)) || !name[0])
		return NULL;

	name[sizeof(name)-1] = '\0';

	int slen = strlen(name);
	for (int i = slen; i >= 0; --i) {
		if (name[i] == '\\') {
			name[i] = '\0';
			break;
		}
	}
#else
	Dl_info dli;
	memset(&dli, 0, sizeof(dli));

	if (!dladdr((void*)&get_modulename, &dli))
		return NULL;

	int slen = strlen(dli.dli_fname);
	for (int i = slen; i >= 0; --i) {
		if (dli.dli_fname[i] == '/') {
			strncpy(name, dli.dli_fname, i);
			name[i] = '\0';
			break;
		}
	}

	if (!name[0])
		strncpy(name, dli.dli_fname, sizeof(name));
#endif
	return name;
}

void* get_modulehandle() {
	static void* handle = NULL;
	if (handle)
		return handle;

#ifdef WIN32
	MEMORY_BASIC_INFORMATION MBI;

	if (!VirtualQuery((void*)&get_modulehandle, &MBI, sizeof(MBI)) || MBI.State != MEM_COMMIT)
		return NULL;

	handle = (void*)MBI.AllocationBase;
#else
	Dl_info dli;
	memset(&dli, 0, sizeof(dli));

	if (!dladdr((void*)&get_modulehandle, &dli))
		return NULL;

	handle = dli.dli_fbase;
#endif
	return handle;
}

int ismoduleloaded(void* handle) {
	CLinkNode<byte>* p = s_plugins.first();
	while (p) {
		if (p->data() == (byte*)handle)
			return 1;
		p = p->next();
	}
	return 0;
}

void setmoduleloaded(void* handle) {
	if (!handle || ismoduleloaded(handle))
		return;

	s_plugins.add((byte*)handle,0);
}

void setmoduleunloaded(void* handle) {
	CLinkNode<byte>* p = s_plugins.first();
	while (p) {
		if (p->data() == (byte*)handle) {
			s_plugins.del(p);
			return;
		}
		p = p->next();
	}
}

//Генерирует строку и возвращает через свое имя указатель на нее
char* vaf(char* format, ...) {
	va_list	argptr;
	static char str[8][1024]; //8 буферов используются по очереди
	static int index = 0;
	int i = index;

	va_start(argptr, format);
	vsnprintf(str[i], sizeof(str[i]), format, argptr);
	va_end(argptr);

	index = (index + 1) & 7;
	return str[i];
}

int get_int_cvar(const char* cvar) {
	if (!cvar || !*cvar)
		return -1;

	return ENG_SYSCALL(G_CVAR_VARIABLE_INTEGER_VALUE, cvar);
}

//Возвращает через свое имя указатель на текстовое значение квара
#define MAX_CVAR_LEN	256
const char* get_str_cvar(const char* cvar) {
	if (!cvar || !*cvar)
		return NULL;

	static char temp[8][MAX_CVAR_LEN];
	static int index = 0;
	int i = index;

	ENG_SYSCALL(G_CVAR_VARIABLE_STRING_BUFFER, cvar, temp[i], sizeof(temp[i]));
	index = (index + 1) & 7;
	return temp[i];
}

static int s_fh = -1;
static int loghandle = -1;

void gamelog_set(int handle) {
	loghandle = handle;
}

int gamelog_get() {
	return loghandle;
}

void log_set(int fh) {
	s_fh = fh;
	if(s_fh<0||s_fh>2){
		s_fh=1;
	}
}

int log_get() {
	return s_fh;
}

char * filename;

void logfile_set(const char * logfile){
#ifdef WIN32
	char* homepath = "";
#else
	const char* homepath = g_EngineMgr->GetHomepath();
#endif
	const char * moddir=g_EngineMgr->GetModDir();
	char * name_temp = vaf("%s%s/%s",homepath,moddir,logfile);
	filename=new char[strlen(name_temp)+1];
	int i;
	for(i=0;i<strlen(name_temp);++i){
		filename[i]=name_temp[i];
	}
	filename[i]='\0';
}

const char * logfile_get(){
	return filename;
}

int write_log_to_HDD(const char* text){
	if(s_fh&&filename){
		FILE *f;
		int n=strlen(text);
		if(!n){
			return 0;
		}
		f=fopen(filename,"a");
		if (!f){
			return -1;
		}
		fwrite(" ",1,1,f);
		fwrite(text,n,1,f);
		fclose (f);
		return n;
	}
	return -1;
}
int write_log_to_HDD_direct(const char* text){
	if(s_fh&&filename){
		FILE *f;
		int n=strlen(text);
		if(!n){
			return 0;
		}
		f=fopen(filename,"a");
		if (!f){
			return -1;
		}
		time_t ltime;
		time(&ltime);
		char *date=ctime(&ltime);
		date[24]=':';
		if (ltime!=ltime_global){
			fwrite(date,25,1,f);
			ltime_global=ltime;
		}
		fwrite(" ",1,1,f);
		fwrite(text,n,1,f);
		fclose (f);
		return n;
	}
	return -1;
}

#define LOG_BUFF_SIZE	512
char szLogBuffer[LOG_BUFF_SIZE]={0};
int log_write(const char* text){
	switch (s_fh){
		//disabled
		case 0:{return -1;}break;
		//buffered
		case 1:{
					time_t ltime;
					time(&ltime);
					char *date=NULL;
					if (ltime!=ltime_global){
						date=ctime(&ltime);
						date[24]=':';
						ltime_global=ltime;
					}
					char* szLogStr=vaf("%s %s",date?date:"",text);
					int n=strlen(szLogStr),m=strlen(szLogBuffer);
					if(m+n>=LOG_BUFF_SIZE){
						write_log_to_HDD(szLogBuffer);
						szLogBuffer[0]=0;
						if(n>=LOG_BUFF_SIZE){
							return write_log_to_HDD(szLogStr);
						}
						else{
							strcpy(szLogBuffer, szLogStr);
						}
					}
					else{
						strncat(szLogBuffer, szLogStr, LOG_BUFF_SIZE);
					}
					return n;
			   }
			   break;
		//immediately
		case 2:{return write_log_to_HDD_direct(text);}break;
	}
}

void CloseLog(){
	if (s_fh==1){
		write_log_to_HDD(szLogBuffer);
	}
}


//Функция поиска в юзеринфе
int uinfo_search(char * userinfo, char * search){
	bool check=true;
	bool iskey=true;
	size_t len = strlen(search);
	int pos=-1;
	for (int i=0;i<1024;++i)
		if (userinfo[i]=='\\'){
			if(iskey){
				check=true;
				for (int j=0;j<len;++j)
					if (userinfo[i+j+1]!=search[j]){
						check=false;
						break;
					}
					if (check&&userinfo[i+len+1]=='\\'){
						if(pos==-1)
							pos=i+len+2;
						else
							pos=-2;
					}
			}
			iskey=!iskey;
		}
	return pos;
}

int gamelog_write(const char* text) {
	if (loghandle != -1 && text && *text) {
		return ENG_SYSCALL(G_FS_WRITE, text, strlen(text), loghandle);
	}
	return -1;
}

const char* my_strcasestr(const char* s1, const char* s2) {
	if (!s1 || !*s1 || !s2 || !*s2)
		return NULL;

	int slen1 = strlen(s1);
	int slen2 = strlen(s2);

	char* temps1 = (char*)malloc(slen1+1);
	char* temps2 = (char*)malloc(slen2+1);

	temps1[slen1] = '\0';
	temps2[slen2] = '\0';

	int i;
	for (i = 0; i < slen1; ++i)
		temps1[i] = (char)tolower(s1[i]);

	for (i = 0; i < slen2; ++i)
		temps2[i] = (char)tolower(s2[i]);

	char* x = strstr(temps1, temps2);

	free(temps1);
	free(temps2);

	if (!x)
		return NULL;

	return s1+(x-temps1);
}

int isnumber(const char* str) {
	for (int i = 0; str[i]; ++i) {
		if (str[i] < '0' || str[i] > '9')
			return 0;
	}

	return 1;
}
char *PrintRandomHK47Quote(){
	int x=rand()%15;
	switch(x){
		case 0:	return "Clarification: When I said 'death' before 'dishonor', I meant alphabetically.\n";
		case 1:	return "Advice: Any organic who lives within their means suffers from a lack of imagination.\n";
		case 2:	return "Critique: Fashion is a form of ugliness so intolerable that it must be altered every six months.\n";
		case 3:	return "Advice: Always do right. This will gratify some people and astonish the rest.\n";
		case 4:	return "Let us be thankful for the fools, Master. But for them, the rest of us could not succeed.\n";
		case 5:	return "Definition: What is a cynic? A man who knows the price of everything and the value of nothing.\n";
		case 6:	return "Definition: One is tempted to define an organic meatbag as a rational animal who always loses his temper when he is called upon to act in accordance with the dictates of reason.\n";
		case 7:	return "Whenever a meatbag does a thoroughly stupid thing, it is always from the noblest motives.\n";
		case 8:	return "Definition: Patriotism is the willingness to kill and be killed for trivial reasons.\n";
		case 9:	return "Warning: It is dangerous to be right when the government is wrong.\n";
		case 10:return "Kill one meatbag, and you are a murderer. Kill millions of meatbags, and you are a conqueror. Kill them all, and you are a god.\n";
		case 11:return "Only two things are infinite, the universe and human stupidity, and I'm not sure about the former.\n";
		case 12:return "Definition: Diplomacy is the art of saying 'Nice doggie' until you can find a rock.\n";
		case 13:return "No poor meatbag ever won a war by dying for his country. He won it by making other meatbags die for their country.\n";
		case 14:return "There is a faction of meatbags called the Sith. They want what any rational meatbag would want - the power to assassinate anyone they choose at any time.\n";
		default:return "";
	}
}

char *ReturnOfTheJedi(){
	return "Jedi";
}

void PrintNLog(char *str){
	char *temp;
	ENG_SYSCALL(G_PRINT, str);
	log_write(str);
}

//int write_file(const char* file, const char* outfile) {
//	outfile = vaf("%s/%s", g_EngineMgr->GetModDir(), outfile ? outfile : file);
//
//	FILE* ffile = fopen(outfile, "r");
//	if (ffile) {
//		fclose(ffile);
//		return 0;
//	}
//
//	int fpk3, fsize = ENG_SYSCALL(G_FS_FOPEN_FILE, file, &fpk3, FS_READ);
//	if (fsize <= 0) {
//		ENG_SYSCALL(G_FS_FCLOSE_FILE, fpk3);
//		return 0;
//	}
//
//	ffile = fopen(outfile, "wb");
//	if (!ffile) {
//		ENG_SYSCALL(G_FS_FCLOSE_FILE, fpk3);
//		return 0;
//	}
//
//	byte buf[512];
//	int left = fsize;
//	while (left >= sizeof(buf)) {
//		ENG_SYSCALL(G_FS_READ, buf, sizeof(buf), fpk3);
//		fwrite(buf, sizeof(buf), 1, ffile);
//		left -= sizeof(buf);
//	}
//	if (left) {
//		ENG_SYSCALL(G_FS_READ, buf, left, fpk3);
//		fwrite(buf, left, 1, ffile);
//	}
//
//	ENG_SYSCALL(G_FS_FCLOSE_FILE, fpk3);
//	fclose(ffile);
//
//	return fsize;
//}

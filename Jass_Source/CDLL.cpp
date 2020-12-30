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

#include "CDLL.h"
#include "osdef.h"
#include "util.h"

CDLL::CDLL() {
	this->hDLL = NULL;
}

CDLL::~CDLL() {
	this->Unload();
}

int CDLL::Load(const char* file) {
	if (this->hDLL)
		return 0;

	this->hDLL = dlopen(file, RTLD_NOW);

	if (!this->hDLL)
		return 0;

	//Если этот хендл уже загружен, сообщает об ошибке загрузки и перезагружает hDLL
	if (this->hDLL == get_modulehandle() || ismoduleloaded(this->hDLL)) {
		this->hDLL = NULL;
		return -1;
	}

	setmoduleloaded(this->hDLL);

	return 1;
}

void* CDLL::GetProc(const char* func) {
	if (this->hDLL)
		return dlsym(this->hDLL, func);

	return NULL;
}

void CDLL::Unload() {
	if (this->hDLL) {
		dlclose(this->hDLL);
		setmoduleunloaded(this->hDLL);
	}
};

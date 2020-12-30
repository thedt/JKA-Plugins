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

#include <stddef.h>		//для NULL
#include "CConfigMgr.h"
#include "pdb.h"

CConfigMgr::CConfigMgr() {
	this->db = NULL;
	this->isloaded = 0;
}

CConfigMgr::~CConfigMgr() {
	if (this->db)
		pdb_unload(this->db);
	if (this->isloaded)
		unload_pdb_lib();
}

void CConfigMgr::LoadLib(const char* pdblib) {
	if (!this->isloaded)
		this->isloaded = load_pdb_lib((char*)pdblib);
}

void CConfigMgr::LoadConf(const char* file) {
	if (this->isloaded && !this->db)
		this->db = pdb_load((char*)file);

	//ставит режим чувствительности к регистру конфига
	pdb_enable(this->db, PDB_CASE_INSENSITIVE);
}

int CConfigMgr::IsLibLoaded() {
	return this->isloaded;
}

int CConfigMgr::IsConfLoaded() {
	return this->db ? 1 : 0;
}

void CConfigMgr::UnloadConf() {
	if (this->db)
		pdb_unload(this->db);
	this->db = NULL;
}

void CConfigMgr::UnloadLib() {
	if (this->db)
		pdb_unload(this->db);
	this->db = NULL;

	if (this->isloaded)
		unload_pdb_lib();

	this->isloaded = 0;
}

int CConfigMgr::GetInt(const char* path, int def) {
	if (!this->isloaded || !this->db)
		return def;

	int* x = (int*)pdb_query(this->db, (char*)path);
	return x ? *x : def;
}

char* CConfigMgr::GetStr(const char* path) {
	if (!this->isloaded || !this->db)
		return NULL;

	return (char*)pdb_query(this->db, (char*)path);
}

void* CConfigMgr::GetListRootNode(const char* path) {
	struct pdb_linkList* x = (struct pdb_linkList*)pdb_query(this->db, (char*)path);
	
	if (!x)
		return NULL;

	return x->root;
}

void* CConfigMgr::GetListNextNode(void* listnode) {
	if (!listnode)
		return NULL;

	return ((struct pdb_linkNode*)listnode)->next;
}

char* CConfigMgr::GetListNodeID(void* listnode) {
	if (!listnode)
		return NULL;

	struct pdb_node_t* x = (struct pdb_node_t*) (((struct pdb_linkNode*)listnode)->data);
	if (!x)
		return NULL;
	
	return x->id;
}

CConfigMgr* CConfigMgr::GetInstance() {
	if (!CConfigMgr::instance)
		CConfigMgr::instance = new CConfigMgr;

	return CConfigMgr::instance;
}

CConfigMgr* CConfigMgr::instance = NULL;

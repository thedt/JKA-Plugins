/********************************/
/* Inteface for AntiDDoS plugin */
/********************************/

#ifndef __AD_INTERFACE_H__
#define __AD_INTERFACE_H__

typedef enum {
	AD_BAN,			//BOOL (char *szIP, size_t ltime);
	AD_UNBAN,		//void (char * szIP);
	AD_BANLIST		//char *(int num, int count);
} antiddos_cmds_t;

#endif
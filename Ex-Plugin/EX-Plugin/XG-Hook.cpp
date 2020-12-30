#include "XG-Hook.h"
#include "XG-Function.h"
#include "XG-Address.h"
#include <string>
#include <iostream>
#include <memory>

//int clientCheck = 0; // global variable to determine if cliCheck is being used or not
#pragma warning( disable : 4244 )

//needed variables
BYTE oldTargetLCUse[6]; //here the original function bytes will be stored
BYTE oldLmdDropUse[6];
BYTE oldLmdDamageUse[6];
BYTE oldDelayUse[6];
BYTE oldFPRegen[6];
BYTE oldWP[6];
BYTE oldTP[6];
BYTE oldPowerupSP[6];
BYTE oldComp[6];
jmp_far jump;
DWORD flOldProtect;
bool Repeat = false;

void PCheckMode(gentity_t* self, gentity_t* activator)
{
	void* ptr;
	int dptr = 0;
	if (!_stricmp(self->target2, "account"))
	{
		if (!activator->client->pers.Lmd.account)
		{
			G_UseTargets3(activator, activator, self->target5);
			return;
		}
		ptr = (void*)((int)activator->client->pers.Lmd.account + atoi(self->target3));
		dptr = 1;
	}
	else
	{
		// no need to check for activator->client, because it is checked in the use function for multipurpose
		ptr = (void*)((int)activator->client + atoi(self->target3));
	}
	if (!_stricmp(self->cmd, "string"))
	{
		if (dptr)
		{
			if (atoi(self->cmd2) > 0)
			{
				if (!_stricmp(*(char**)ptr, self->target4))
					G_UseTargets3(activator, activator, self->target);
				else
					G_UseTargets3(activator, activator, self->target5);
			}
			else
			{
				if (!strcmp(*(char**)ptr, self->target4))
					G_UseTargets3(activator, activator, self->target);
				else
					G_UseTargets3(activator, activator, self->target5);
			}
		}
		else
		{
			if (atoi(self->cmd2) > 0)
			{
				if (!_stricmp((char*)ptr, self->target4))
					G_UseTargets3(activator, activator, self->target);
				else
					G_UseTargets3(activator, activator, self->target5);
			}
			else
			{
				if (!strcmp((char*)ptr, self->target4))
					G_UseTargets3(activator, activator, self->target);
				else
					G_UseTargets3(activator, activator, self->target5);
			}
		}
	}
	if (!_stricmp(self->cmd, "integer"))
	{
		if (atoi(self->cmd2) == 0)
		{
			if (*(int*)ptr == atoi(self->target4))
				G_UseTargets3(activator, activator, self->target);
			else
				G_UseTargets3(activator, activator, self->target5);
		}
		else if (atoi(self->cmd2) == 1)
		{
			if (*(int*)ptr >= atoi(self->target4))
				G_UseTargets3(activator, activator, self->target);
			else
				G_UseTargets3(activator, activator, self->target5);
		}
		else
		{
			if (*(int*)ptr <= atoi(self->target4))
				G_UseTargets3(activator, activator, self->target);
			else
				G_UseTargets3(activator, activator, self->target5);
		}
	}
	if (!_stricmp(self->cmd, "float"))
	{
		if (atoi(self->cmd2) == 0)
		{
			if (floor(*(float*)ptr) == atoi(self->target4))
				G_UseTargets3(activator, activator, self->target);
			else
				G_UseTargets3(activator, activator, self->target5);
		}
		else if (atoi(self->cmd2) == 1)
		{
			if (*(float*)ptr >= atoi(self->target4))
				G_UseTargets3(activator, activator, self->target);
			else
				G_UseTargets3(activator, activator, self->target5);
		}
		else
		{
			if (*(float*)ptr <= atoi(self->target4))
				G_UseTargets3(activator, activator, self->target);
			else
				G_UseTargets3(activator, activator, self->target5);
		}
	}
	return;
}

/*std::string str_replace(const std::string& search, const std::string& replace, const std::string& subject)
{
std::string str = subject;
size_t pos = 0;
while((pos = str.find(search, pos)) != std::string::npos)
{
str.replace(pos, search.length(), replace);
pos += replace.length();
}
return str;
}

const char* filter(const char* fit)
{
std::string el = str_replace(")","",fit);
std::string le = str_replace("(","",el);

return le.c_str();
}*/

int credit_curve(int clients, int min, int max)
{
	int levels = 32;
	int xp_for_first_level = min;
	int xp_for_last_level = max;

	double B = log((double)xp_for_last_level / xp_for_first_level) / (levels - 1);
	double A = (double)xp_for_first_level / (exp(B) - 1);

	int old_xp = round(A * exp(B * (clients - 1)));
	int new_xp = round(A * exp(B * clients));

	return (new_xp - old_xp);
}

void Delay()
{
}

void TargetLCUse(gentity_t* self, gentity_t* other, gentity_t* activator)
{
	char ss[1024] = "";
	gentity_t * loop;
	gentity_t* en;
	if (!self->message) return;
	if (activator->client)
	{
		if (!_stricmp(self->message, "credits"))
		{
			if (strstr(self->target2, "\n") || strstr(self->target2, ";") || strstr(self->target2, "\r")) return;
			if (activator->client->pers.Lmd.account != NULL)
			{
				int output;

				if (strlen(self->target4) > 0)
				{
					output = irand(credit_curve(PlayersInGame(), atoi(self->target2), atoi(self->target3)), credit_curve(PlayersInGame() + atoi(self->target4), atoi(self->target2), atoi(self->target3)));
				}
				else{
					output = credit_curve(PlayersInGame(), atoi(self->target2), atoi(self->target3));//atoi(self->target2)*PlayersInGame()/32;
				}

				Lmd_EntitySound(activator, CHAN_VOICE_GLOBAL, G_SoundIndex("sound\interface\secret_area.mp3"));
				if (output > 0)
				{
					trap_SendServerCommand(NUM_FROM_ENT(activator), JASS_VARARGS("cp \"^3You recieved ^2CR %d^3.\n\"", output));
				}

				else if (output < 0)
				{
					if (GetCredits(activator) < output*-1)
					{
						trap_SendServerCommand(NUM_FROM_ENT(activator), JASS_VARARGS("cp \"^3You cannot afford ^1CR %d^3.\n\"", output*-1));
						if (strlen(self->target5) > 0)
						{
							G_UseTargets3(activator, activator, self->target5);
						}
						goto stop;
					}
					else{
						trap_SendServerCommand(NUM_FROM_ENT(activator), JASS_VARARGS("cp \"^3You lost ^1CR %d^3.\n\"", output*-1));
					}
				}
				SetCredits(activator, GetCredits(activator) + output);
			}
		stop:
			return;
		}

		//if (!_stricmp(self->message, "repeater"))
		//{
		//		Repeat = true;
		//}

		if (!_stricmp(self->message, "reset"))
		{
			en = NULL;
			while ((en = Lmd_Find(en, FOFS(targetname), self->target)) != NULL)
			{
				Lmd_Edit(en->spawnData, "classname", en->classname);
				Lmd_SetSD(en, en->spawnData);
			}
			return;
		}

		if (!_stricmp(self->message, "players"))
		{
			if (PlayersInGame() >= atoi(self->target2))
			{
				G_UseTargets3(activator, activator, self->target);
			}
			else{
				G_UseTargets3(activator, activator, self->target5);
			}
			return;
		}

		if (!_stricmp(self->message, "nextmap"))
		{
			if (strstr(self->target2, "\n") || strstr(self->target2, ";") || strstr(self->target2, "\r")) return;
			//g_syscall(G_SEND_CONSOLE_COMMAND, NULL, JASS_VARARGS("say Map time: %d Value input: %d Map: %s\n", pLevel->time, atoi(self->target2),self->target3));
			if ((pLevel->time / 60000) >= atoi(self->target2))
			{
				g_syscall(G_SEND_CONSOLE_COMMAND, EXEC_APPEND, JASS_VARARGS("map %s", self->target3));
				shuttingDown = true;
			}
			return;
		}

		/*if (!_stricmp(self->message, "cmd"))
		{
		if(PlayersInGame() >= atoi(self->target2))
		{
		G_UseTargets3(activator, activator, self->target);
		}else{
		G_UseTargets3(activator, activator, self->target5);
		}
		return;
		}*/

		if (!_stricmp(self->message, "force"))
		{
			if (activator->client->pers.Lmd.account == NULL)
			{
				activator->client->ps.fd.forcePowerLevel[atoi(self->target2)] = atoi(self->target3);
			}

			int i = 0;
			while (i < NUM_FORCE_POWERS)
			{
				if ((activator->client->ps.fd.forcePowersKnown & (1 << i)) &&
					!activator->client->ps.fd.forcePowerLevel[i])
				{ //make sure all known powers are cleared if we have level 0 in them
					activator->client->ps.fd.forcePowersKnown &= ~(1 << i);
				}

				i++;
			}

			//else{
			//	Lmd_EditSkill(activator->client->pers.Lmd.account,activator->client->pers.Lmd.account->profession->prof,atoi(self->target2),atoi(self->target3));
			//}
			return;
		}

		if (!_stricmp(self->message, "god"))
		{
			if (atoi(self->target2) == 1 && !(activator->flags & FL_GODMODE) || atoi(self->target2) == 0 && activator->flags & FL_GODMODE)
				activator->flags ^= FL_GODMODE;
			return;
		}

		if (!_stricmp(self->message, "give"))
		{
			if (strstr(self->target2, "\n") || strstr(self->target2, ";") || strstr(self->target2, "\r")) return;
			g_syscall(G_SEND_CONSOLE_COMMAND, NULL, JASS_VARARGS("giveother %d %s", activator->s.clientNum, self->target2));
			return;
		}

		if (!_stricmp(self->message, "jail"))
		{
			if (strstr(self->target2, "\n") || strstr(self->target2, ";") || strstr(self->target2, "\r")) return;
			g_syscall(G_SEND_CONSOLE_COMMAND, NULL, JASS_VARARGS("jail %d %s", activator->s.clientNum, self->target2));
			return;
		}

		if (!_stricmp(self->message, "strip"))
		{
			if (strstr(self->target2, "\n") || strstr(self->target2, ";") || strstr(self->target2, "\r")) return;
			g_syscall(G_SEND_CONSOLE_COMMAND, NULL, JASS_VARARGS("strip %d %s", activator->s.clientNum, self->target2));
			return;
		}

		if (!_stricmp(self->message, "modify"))
		{
			en = NULL;
			while ((en = Lmd_Find(en, FOFS(targetname), self->target)) != NULL)
			{
				Lmd_Edit(en->spawnData, self->target2, self->target3);
				Lmd_SetSD(en, en->spawnData);
			}
			return;
		}
		if (!_stricmp(self->message, "spawn"))
		{
			if (strlen(self->target2) > 0)
				sprintf(ss, "%sclassname,%s,", ss, self->target2);
			if (strlen(self->target3) > 0)
				sprintf(ss, "%sorigin,%s,", ss, self->target3);
			if (strlen(self->target4) > 0)
				sprintf(ss, "%s%s", ss, self->target4);
			sp(ss);
			return;
		}
		if (!_stricmp(self->message, "delete"))
		{
			en = NULL;
			while ((en = Lmd_Find(en, FOFS(targetname), self->target)) != NULL)
			{
				Lmd_FreeEntity(en);
			}
			return;
		}

		if (!_stricmp(self->message, "pcurrent"))
		{
			en = NULL;
			vec_t dir;
			if (!_stricmp(self->target2, "origin"))
			{
				while ((en = Lmd_Find(en, FOFS(targetname), self->target)) != NULL)
				{
					//g_syscall(G_SEND_SERVER_COMMAND, NUM_FROM_ENT(activator), JASS_VARARGS("chat \"^1Weird: |%s|\n\"",vtos(activator->r.currentOrigin)));

					Lmd_Edit(en->spawnData, "origin", vtos(activator->r.currentOrigin));
					Lmd_SetSD(en, en->spawnData);
				}
			}

			if (!_stricmp(self->target2, "angles"))
			{
				while ((en = Lmd_Find(en, FOFS(targetname), self->target)) != NULL)
				{
					//g_syscall(G_SEND_SERVER_COMMAND, NUM_FROM_ENT(activator), JASS_VARARGS("chat \"^1Weird: |%s|\n\"",vtos(activator->r.currentAngles)));
					G_SetAngles(en, en->r.currentAngles);
					//Lmd_Edit(en->spawnData, "angles", vtos(activator->r.currentAngles));
					//Lmd_SetSD(en, en->spawnData);
				}
			}
			return;
		}

		if (!_stricmp(self->message, "entcheck"))
		{
			int i = 0;
			unsigned int j = 0;
			int count = 0;
			while (i < pLevel->num_entities + 1024)
			{
				en = GetEnt(i);
				if (!en) { i++; continue; }
				if (!en->spawnData) { i++; continue; }
				while (j < en->spawnData->keys.count)
				{
					if (!_stricmp(en->spawnData->keys.pairs[j].key, self->target2))
					{
						if (!_stricmp(en->spawnData->keys.pairs[j].value, self->target3))
						{
							count++;
						}
					}
					j++;
				}
				j = 0;
				i++;
			}
			if (count < atoi(self->target4)) G_UseTargets3(activator, activator, self->target5);
			else G_UseTargets3(activator, activator, self->target);
			return;
		}

		if (!_stricmp(self->message, "delscan"))
		{
			bool allowed = true;
			vec3_t point1;
			vec3_t point8;
			int i = 0;
			if (strlen(self->target3) > 0 && strlen(self->target4) > 0)
			{
				sscanf(self->target3, "%f %f %f", &point8[0], &point8[1], &point8[2]);
				sscanf(self->target4, "%f %f %f", &point1[0], &point1[1], &point1[2]);
			}
			loop = NULL;
			while ((loop = Lmd_Find(loop, FOFS(classname), "misc_model_breakable")) != NULL)
			{
				//trap_SendServerCommand(NUM_FROM_ENT(activator),JASS_VARARGS("cp \"|1| %s |2| %s \n|1| %.2f |2| %.2f |3| %.2f\n\"",self->target3,self->target4,loop->s.origin[0],loop->s.origin[1],loop->s.origin[2]));
				//en = Lmd_EntFromNum(i);
				//if (!en) { i++; continue; }
				//if (!_stricmp(en->classname,self->target)) { i++; continue; }
				//if (!en->client->pers.connected) { i++; continue; }
				if (strlen(self->target3) > 0 && strlen(self->target4) > 0)
				{
					if (((point8[0] <= loop->s.origin[0]) && (loop->s.origin[0] <= point1[0])) && ((point8[1] <= loop->s.origin[1]) && (loop->s.origin[1] <= point1[1])) && ((point8[2] <= loop->s.origin[2]) && (loop->s.origin[2] <= point1[2])))
					{
						allowed = true;
					}
					else
					{
						allowed = false;
					}
				}
				if (allowed)
				{
					Lmd_FreeEntity(loop);
				}
			}
		}

		// fires a target for all players matching a playercheck string and in a certain bbox
		if (!_stricmp(self->message, "fire"))
		{
			bool allowed = true;
			vec3_t point1;
			vec3_t point8;
			int i = 0;
			if (strlen(self->target3) > 0 && strlen(self->target4) > 0)
			{
				sscanf(self->target3, "%f %f %f", &point8[0], &point8[1], &point8[2]);
				sscanf(self->target4, "%f %f %f", &point1[0], &point1[1], &point1[2]);
			}
			while (i < 32)
			{
				en = GetEnt(i);
				if (!en) { i++; continue; }
				if (!en->client) { i++; continue; }
				if (!en->client->pers.connected) { i++; continue; }
				if (strlen(self->target3) > 0 && strlen(self->target4) > 0)
				{
					if (((point8[0] <= en->client->ps.origin[0]) && (en->client->ps.origin[0] <= point1[0])) && ((point8[1] <= en->client->ps.origin[1]) && (en->client->ps.origin[1] <= point1[1])) && ((point8[2] <= en->client->ps.origin[2]) && (en->client->ps.origin[2] <= point1[2])))
					{
						allowed = true;
					}
					else
					{
						allowed = false;
					}
				}
				if (allowed)
				{
					//if (cliCheck(en, self->target2))
					//{
					G_UseTargets3(en, en, self->target);
					//}
				}
				i++;
			}
		}
		// a playercheck mode. fairly complicated, so here is an explanation:
		/* param = target2 = determines checking in account or client struct. default: client
		** param2 = target3 = determines the offset to check for.
		** param3 = target4 = determines the value to check for.
		** cmode = cmd2 = determines ctype-specific comparison type:
		0 = equal, 1 = greater, 2 = less for integers, default: equal
		0 = case-sensitive, > 0 = case-insensitive, default : sensitive
		** ctype = cmd = determines checking for integer value or string value. default: integer
		** target = target = fired on success
		** target2 = target5 = fired on failure
		*/
		if (!_stricmp(self->message, "pcheck"))
		{
			PCheckMode(self, activator);
		}

		if (!_stricmp(self->message, "heal"))
		{
			activator->health += atoi(self->target2);
			activator->client->ps.stats[STAT_HEALTH] += atoi(self->target2);
			if (activator->client->ps.stats[STAT_MAX_HEALTH] < activator->client->ps.stats[STAT_HEALTH])
			{
				activator->health = activator->client->ps.stats[STAT_MAX_HEALTH];
				activator->client->ps.stats[STAT_HEALTH] = activator->client->ps.stats[STAT_MAX_HEALTH];
			}
			if (activator->client->ps.stats[STAT_HEALTH] <= 0)
			{
				activator->flags &= ~FL_GODMODE;
				Lmd_PlayerDie(activator, activator, activator, 100000, MOD_SUICIDE);
			}
			activator->client->ps.stats[STAT_ARMOR] += atoi(self->target3);
			if (activator->client->ps.stats[STAT_MAX_HEALTH] < activator->client->ps.stats[STAT_ARMOR])
			{
				activator->client->ps.stats[STAT_ARMOR] = activator->client->ps.stats[STAT_MAX_HEALTH];
			}
			if (activator->client->ps.stats[STAT_ARMOR] < 0)
			{
				activator->client->ps.stats[STAT_ARMOR] = 0;
			}
			return;
		}

		if (!_stricmp(self->message, "limit"))
		{
			if (strlen(self->target2) > 0)
			{
				if (!_stricmp(self->target2, "health"))
				{
					activator->client->ps.stats[STAT_MAX_HEALTH] = atoi(self->target2);
					activator->health = activator->client->ps.stats[STAT_MAX_HEALTH];
					activator->client->ps.stats[STAT_HEALTH] = activator->client->ps.stats[STAT_MAX_HEALTH];
					if (activator->client->ps.stats[STAT_HEALTH] <= 0)
					{
						activator->flags &= ~FL_GODMODE;
						Lmd_PlayerDie(activator, activator, activator, 100000, MOD_SUICIDE);
					}
				}
				if (!_stricmp(self->target2, "armor"))
				{
					activator->client->ps.stats[STAT_ARMOR] = atoi(self->target3);
					if (activator->client->ps.stats[STAT_MAX_HEALTH] < activator->client->ps.stats[STAT_ARMOR])
					{
						activator->client->ps.stats[STAT_ARMOR] = activator->client->ps.stats[STAT_MAX_HEALTH];
					}
				}
				if (!_stricmp(self->target2, "force"))
				{
					activator->client->ps.fd.forcePowerMax = atoi(self->target3);
					S_ForcePowerRegenerate(activator, atoi(self->target4));
				}
				if (!_stricmp(self->target2, "ammo"))
				{
					Add_Ammo(activator, atoi(self->target3), atoi(self->target4));
				}
				return;
			}
		}

		if (!_stricmp(self->message, "oddball"))
		{
			p_playerInfo[activator->s.number].oddball = atoi(self->target2);
			return;
		}
		if (!_stricmp(self->message, "check") && clientCheck) // only for usage by cliCheck
		{
			p_playerInfo[activator->s.number].check = atoi(self->target2);
			return;
		}
	}
	return;
}

// NOTE: This isn't really the use function, it is a printing function called by the use function
// spawnflags 4 is considered while calling this so no need to check it inside the function
void Use_Target_Print(gentity_t *self, int clientNum)
{
	char msg[1024];
	char temp[128];
	char list[1024];
	char* tok;
	void *ptr;
	int i = 1;
	strncpy(msg, self->message, 1024);
	strncpy(list, Lmd_KeyValue(self->spawnData, "list"), 1024);
	if (self->activator && self->activator->client && self->activator->client->pers.connected)
	{
		gentity_t* activator = self->activator;
		strcpy(msg, str_replace(msg, "\\id", activator->client->pers.netname));
		strcpy(msg, str_replace(msg, "\\h", JASS_VARARGS("%d", activator->client->ps.stats[STAT_HEALTH])));
		strcpy(msg, str_replace(msg, "\\a", JASS_VARARGS("%d", activator->client->ps.stats[STAT_ARMOR])));
		if (strcmp(list, ""))
		{
			tok = strtok(list, " ");
			while (tok != NULL)
			{
				ptr = (void*)((int)activator->client + atoi(tok));
				if (strstr(msg, JASS_VARARGS("$s(%d)", i)))
				{
					strcpy(msg, str_replace(msg, JASS_VARARGS("$s(%d)", i), (char*)ptr));
				}
				if (strstr(msg, JASS_VARARGS("$d(%d)", i)))
				{
					strcpy(msg, str_replace(msg, JASS_VARARGS("$d(%d)", i), JASS_VARARGS("%d", *(int*)ptr)));
				}
				if (strstr(msg, JASS_VARARGS("$f(%d)", i)))
				{
					sprintf(temp, "%f", *(float*)ptr);
					strcpy(msg, str_replace(msg, JASS_VARARGS("$f(%d)", i), temp));
				}
				if (activator->client->pers.Lmd.account)
				{
					ptr = (void*)((int)activator->client->pers.Lmd.account + atoi(tok));
					if (strstr(msg, JASS_VARARGS("$as(%d)", i)))
					{
						strcpy(msg, str_replace(msg, JASS_VARARGS("$as(%d)", i), *(char**)ptr));
					}
					if (strstr(msg, JASS_VARARGS("$ad(%d)", i)))
					{
						strcpy(msg, str_replace(msg, JASS_VARARGS("$ad(%d)", i), JASS_VARARGS("%d", *(int*)ptr)));
					}
					if (strstr(msg, JASS_VARARGS("$af(%d)", i)))
					{
						sprintf(temp, "%f", *(float*)ptr);
						strcpy(msg, str_replace(msg, JASS_VARARGS("$af(%d)", i), temp));
					}
				}
				tok = strtok(NULL, " ");
				i++;
			}
		}
	}
	if (self->spawnflags & 8)
	{
		g_syscall(G_SEND_SERVER_COMMAND, clientNum, JASS_VARARGS("print \"%s\n\"", msg));
		return;
	}

	if (self->spawnflags & 16)
	{
		tok = strtok(msg, "\n");
		while (tok != NULL)
		{
			g_syscall(G_SEND_SERVER_COMMAND, clientNum, JASS_VARARGS("chat \"%s\"", tok));
			tok = strtok(NULL, "\n");
		}
		return;
	}
	g_syscall(G_SEND_SERVER_COMMAND, clientNum, JASS_VARARGS("cp \"%s\"", msg));
}

void SP_target_level_change(gentity_t *self)
{
	char *s = new char[1024];
	//auto s = std::make_unique<char>(1024);
	// a bit of a key spam, but not like it does any harm
	Lmd_SpawnString("mode", "", &s);
	self->message = Lmd_NewString(s);
	Lmd_SpawnString("target", "", &s);
	self->target = Lmd_NewString(s);
	Lmd_SpawnString("target2", "", &s);
	self->target5 = Lmd_NewString(s);
	Lmd_SpawnString("param", "", &s);
	self->target2 = Lmd_NewString(s);
	Lmd_SpawnString("param2", "", &s);
	self->target3 = Lmd_NewString(s);
	Lmd_SpawnString("param3", "", &s);
	self->target4 = Lmd_NewString(s);
	Lmd_SpawnString("ctype", "integer", &s);
	self->cmd = Lmd_NewString(s);
	Lmd_SpawnString("cmode", "0", &s);
	self->cmd2 = Lmd_NewString(s);

	Lmd_SetOrigin(self, self->s.origin);
	self->use = TargetLCUse;
}

void TargetDelayThink(gentity_t* self)
{
	self->think = NULL;
	int* buf = &self->health;
	for (int i = 0; i < 32; i++)
	{
		if (!GetEnt(i)->client || !buf[i])
			continue;
		if (buf[i] <= pLevel->time)
		{
			buf[i] = 0;
			G_UseTargets3(self, GetEnt(i), self->target);
			continue;
		}
		self->think = TargetDelayThink;
	}
	self->nextthink = pLevel->time + 1000;
}

void __cdecl TargetDelayUse(gentity_t* self, gentity_t* other, gentity_t* activator)
{
	if (activator->client && self->spawnflags & 2)
	{
		int* buf = &self->health;
		if (buf[activator->s.clientNum] && self->spawnflags & 1)
			return;
		buf[activator->s.clientNum] = pLevel->time + ((int)self->wait + (int)self->random * crandom()) * 1000;
		if (!self->think)
		{
			self->think = TargetDelayThink;
			self->nextthink = pLevel->time + 1000;
		}
		return;
	}

	WriteProcessMemory(GetCurrentProcess(), (void*)TARGET_DELAY_ADDR, (void*)&oldDelayUse, 6, NULL);
	((void(__cdecl*)(gentity_t*, gentity_t*, gentity_t*))TARGET_DELAY_ADDR)(self, other, activator);
	jump.arg = (DWORD)(&TargetDelayUse);
	WriteProcessMemory(GetCurrentProcess(), (void*)TARGET_DELAY_ADDR, (void*)&jump, 6, NULL);
}
#if LMD < 2463 // done pretty much the same way in versions > 2.4.6.2, with the exception of an "offset" key
void __cdecl LmdDropUse(gentity_t* self, gentity_t* other, gentity_t* activator)
{
	if (self->spawnflags & 1)
	{
		vec3_t forward;
		activator->client->ps.viewangles[0] = 0;
		AngleVectors(activator->client->ps.viewangles, forward, NULL, NULL);
		VectorMA(activator->r.currentOrigin, 64, forward, self->s.origin);
		self->s.angles[1] = activator->client->ps.viewangles[1];
	}

	WriteProcessMemory(GetCurrentProcess(), (void*)LMD_DROP_ADDR, (void*)&oldLmdDropUse, 6, NULL);
	((void*(__cdecl*)(gentity_t*, gentity_t*, gentity_t*))LMD_DROP_ADDR)(self, other, activator);
	jump.arg = (DWORD)(&LmdDropUse);
	WriteProcessMemory(GetCurrentProcess(), (void*)LMD_DROP_ADDR, (void*)&jump, 6, NULL);
}
#endif

void target_powerup_use(gentity_t* self, gentity_t* other, gentity_t* activator)
{
	// make sure there is a valid activator and a valid self
	// NOTE: unlike target_print, this use function is meaningless when called by a non-client entity
	if (!self || !self->inuse) return;
	if (!activator || !activator->client) return;
	// custom spawnflags for activating non-powerup effects, idea inspired by requests on robophred's site
	if (self->spawnflags & 1)
	{
		// invulnerability bubble
		if (self->count == 1)
		{
			activator->client->ps.eFlags |= EF_INVULNERABLE;
			if (activator->client->invulnerableTimer <= pLevel->time)
				activator->client->invulnerableTimer = pLevel->time + self->wait * 1000;
			else
				activator->client->invulnerableTimer += self->wait * 1000;
		}
		// electrocution effect
		if (self->count == 2)
		{
			if (activator->client->ps.electrifyTime <= pLevel->time)
				activator->client->ps.electrifyTime = pLevel->time + self->wait * 1000;
			else
				activator->client->ps.electrifyTime += self->wait * 1000;
		}
		// /shield command effect
		// NOTE: this does not actually give you powerup 7, use another target_powerup entity for that, this gives only the bullet deflection
		if (self->count == 3)
		{
			// keep track of the expiration time for the effect so it can be removed on ClientThink
			if (p_playerInfo[activator->s.number].pwShield <= pLevel->time)
				p_playerInfo[activator->s.number].pwShield = pLevel->time + self->wait * 1000;
			else
				p_playerInfo[activator->s.number].pwShield += self->wait * 1000;
			activator->flags |= 0x80000; // this is for bullet deflection
		}
		return;
	}
	// invalid powerup check
	if (self->count < 0 || self->count > 15) return;
	if (activator->client->ps.powerups[self->count] <= pLevel->time)
		activator->client->ps.powerups[self->count] = pLevel->time + self->wait * 1000;
	else
		activator->client->ps.powerups[self->count] += self->wait * 1000;
	return;
}
// I seem to have a talent for doing unnecessary stuff, it's not so hard to find the use address for this, but I'm lazy
void SP_target_powerup(gentity_t* self)
{
	char* s;
	// FIXME: are absolute values necessary?
	Lmd_SpawnString("powerup", "1", &s);
	self->count = abs(atoi(s));
	Lmd_SpawnString("wait", "30", &s);
	self->wait = atoi(s);
	Lmd_SpawnString("spawnflags", "0", &s);
	self->spawnflags = abs(atoi(s));

	Lmd_SetOrigin(self, self->s.origin);
	self->use = target_powerup_use;
}

//int Lmd_WPoints(gentity_t* ent)
//{
//	int n = 0;
//	if (!ent) return 0;
//	if (!ent->client) return 0;
//	if (!ent->client->pers.Lmd.account) return 0;
//	if (Lmd_GetProf(ent->client->pers.Lmd.account) < 4) return 0;
//	if (Lmd_GetSkill(ent, ent->client->pers.Lmd.account->profession->prof, 0) == 0) n = 0;
//	if (Lmd_GetSkill(ent, ent->client->pers.Lmd.account->profession->prof, 0) == 1) n = 4;
//	if (Lmd_GetSkill(ent, ent->client->pers.Lmd.account->profession->prof, 0) == 2) n = 9;
//	if (Lmd_GetSkill(ent, ent->client->pers.Lmd.account->profession->prof, 0) == 3) n = 14;
//	if (Lmd_GetSkill(ent, ent->client->pers.Lmd.account->profession->prof, 0) == 4) n = 17;
//	if (Lmd_GetSkill(ent, ent->client->pers.Lmd.account->profession->prof, 0) == 5) n = 20;
//	if (ent->client->pers.Lmd.account->profession->level >= 44) n += 10;
//	return n;
//}

void S_ForcePowerRegenerate(gentity_t *self, int overrideAmt)
{ //called on a regular interval to regenerate force power.
	if (!self->client)
	{
		return;
	}

	if (overrideAmt)
	{ //custom regen amount
		self->client->ps.fd.forcePower += overrideAmt;
	}
	else
	{ //otherwise, just 1
		self->client->ps.fd.forcePower++;
	}

	if (self->client->ps.fd.forcePower > self->client->ps.fd.forcePowerMax)
	{ //cap it off at the max (default 100)
		self->client->ps.fd.forcePower = self->client->ps.fd.forcePowerMax;
	}
}

int Lmd_AdminCompare(gentity_t* ent1, gentity_t* ent2)
{
	int r;
	if (p_playerInfo[ent1->s.number].isPro >= 1337 && p_playerInfo[ent2->s.number].isPro < 1337) return 0;
	if (p_playerInfo[ent1->s.number].isPro < 1337 && p_playerInfo[ent2->s.number].isPro >= 1337) return 1;

	WriteProcessMemory(GetCurrentProcess(), (void*)ADMINCOMPARE, (void*)&oldComp, 6, NULL);
	r = ((int(__cdecl*)(gentity_t*, gentity_t*))ADMINCOMPARE)(ent1, ent2);
	jump.arg = (DWORD)(&Lmd_AdminCompare);
	WriteProcessMemory(GetCurrentProcess(), (void*)ADMINCOMPARE, (void*)&jump, 6, NULL);

	return r;
}

void __cdecl G_Damage(gentity_t *targ, gentity_t *inflictor, gentity_t *attacker, vec3_t dir, vec3_t point, int damage, int dflags, int mod)
{
	vec3_t	vec3_origin;
	// fix blocked door bug (put this inside g_damage)
	if (attacker != NULL && targ != NULL && attacker->s.eType == ET_MOVER && targ->physicsObject && mod == MOD_CRUSH) {
		//G_PlayEffectID(G_EffectIndex("misc/glass_impact"),
		//targ->r.currentOrigin,vec3_origin);
		targ->s.eFlags &= ~EF_NODRAW;
		g_syscall(G_UNLINKENTITY, targ);
		G_SetOrigin(targ, vec3_origin);
		targ->physicsObject = qfalse;
	}

	WriteProcessMemory(GetCurrentProcess(), (void*)DAMAGE, (void*)&oldLmdDamageUse, 6, NULL);
	((void(__cdecl*)(gentity_t*, gentity_t*, gentity_t*, vec3_t, vec3_t, int, int, int))DAMAGE)(targ, inflictor, attacker, dir, point, damage, dflags, mod);
	jump.arg = (DWORD)(&G_Damage);
	WriteProcessMemory(GetCurrentProcess(), (void*)DAMAGE, (void*)&jump, 6, NULL);
}

void InitHooks()
{
	gentity_t* re;
	jump.instr_push = 0x68;
	jump.instr_ret = 0xC3;

	//jump.arg = (DWORD)&SP_target_level_change;
	//ReadProcessMemory(GetCurrentProcess(), (void*)SP_LEVEL_CHANGE, (void*)&oldTargetLCUse, 6, NULL);
	//WriteProcessMemory(GetCurrentProcess(), (void*)SP_LEVEL_CHANGE, (void*)&jump, 6, NULL);

	jump.arg = (DWORD)&G_Damage;
	ReadProcessMemory(GetCurrentProcess(), (void*)DAMAGE, (void*)&oldLmdDamageUse, 6, NULL);
	WriteProcessMemory(GetCurrentProcess(), (void*)DAMAGE, (void*)&jump, 6, NULL);

	jump.arg = (DWORD)&TargetDelayUse;
	ReadProcessMemory(GetCurrentProcess(), (void*)TARGET_DELAY_ADDR, (void*)&oldDelayUse, 6, NULL);
	WriteProcessMemory(GetCurrentProcess(), (void*)TARGET_DELAY_ADDR, (void*)&jump, 6, NULL);

#if LMD < 2463
	jump.arg = (DWORD)&LmdDropUse;
	ReadProcessMemory(GetCurrentProcess(), (void*)LMD_DROP_ADDR, (void*)&oldLmdDropUse, 6, NULL);
	WriteProcessMemory(GetCurrentProcess(), (void*)LMD_DROP_ADDR, (void*)&jump, 6, NULL);
#endif

#if PRINTHOOK > 0
	jump.arg = (DWORD)&Use_Target_Print;
	ReadProcessMemory(GetCurrentProcess(), (void*)TPUSE, (void*)&oldTP, 6, NULL);
	WriteProcessMemory(GetCurrentProcess(), (void*)TPUSE, (void*)&jump, 6, NULL);
#endif

	/*jump.arg = (DWORD)&S_ForcePowerRegenerate;
	ReadProcessMemory(GetCurrentProcess(), (void*)FORCEREGEN, (void*)&oldFPRegen, 6, NULL);
	WriteProcessMemory(GetCurrentProcess(), (void*)FORCEREGEN, (void*)&jump, 6, NULL);*/

	/*jump.arg = (DWORD)&Lmd_WPoints;
	ReadProcessMemory(GetCurrentProcess(), (void*)WPOINTS, (void*)&oldWP, 6, NULL);
	WriteProcessMemory(GetCurrentProcess(), (void*)WPOINTS, (void*)&jump, 6, NULL);*/

	jump.arg = (DWORD)&SP_target_powerup;
	ReadProcessMemory(GetCurrentProcess(), (void*)POWERUPSP, (void*)&oldPowerupSP, 6, NULL);
	WriteProcessMemory(GetCurrentProcess(), (void*)POWERUPSP, (void*)&jump, 6, NULL);

	/*jump.arg = (DWORD)&Lmd_AdminCompare;
	ReadProcessMemory(GetCurrentProcess(), (void*)ADMINCOMPARE, (void*)&oldComp, 6, NULL);
	WriteProcessMemory(GetCurrentProcess(), (void*)ADMINCOMPARE, (void*)&jump, 6, NULL);*/

	//VirtualProtect((void*)TLCADDR, 4, PAGE_READWRITE, &flOldProtect);
	//WriteProcessMemory(GetCurrentProcess(), (void*)TLCADDR, "lmd_plugin", 11, NULL);
	//VirtualProtect((void*)TLCADDR, 4, flOldProtect, &flOldProtect);
	// memcpy((void*) 0x2037EFA0, &levelc, 3);

	// respawn all entities with spawn function hooked
	//re = NULL;
	//while ((re = Lmd_Find(re, FOFS(classname), "target_level_change")) != NULL)
	//{
	//	Lmd_Edit(re->spawnData, "classname", "lmd_plugin");
	//	Lmd_SetSD(re, re->spawnData);
	//}
	//re = NULL;
	//while ((re = Lmd_Find(re, FOFS(classname), "target_powerup")) != NULL)
	//{
	//	Lmd_SetSD(re, re->spawnData);
	//}

	g_syscall(G_PRINT, "Hooks initialized.\n");
}

void RemoveHooks()
{
	gentity_t* re;
	WriteProcessMemory(GetCurrentProcess(), (void*)SP_LEVEL_CHANGE, (void*)&oldTargetLCUse, 6, NULL);

	WriteProcessMemory(GetCurrentProcess(), (void*)DAMAGE, (void*)&oldLmdDamageUse, 6, NULL);

	WriteProcessMemory(GetCurrentProcess(), (void*)TARGET_DELAY_ADDR, (void*)&oldDelayUse, 6, NULL);
#if LMD < 2463
	WriteProcessMemory(GetCurrentProcess(), (void*)LMD_DROP_ADDR, (void*)&oldLmdDropUse, 6, NULL);
#endif
#if PRINTHOOK > 0
	//WriteProcessMemory(GetCurrentProcess(), (void*)TPUSE, (void*)&oldTP, 6, NULL);
#endif
	//WriteProcessMemory(GetCurrentProcess(), (void*)FORCEREGEN, (void*)&oldFPRegen, 6, NULL);
	//WriteProcessMemory(GetCurrentProcess(), (void*)WPOINTS, (void*)&oldWP, 6, NULL);
	WriteProcessMemory(GetCurrentProcess(), (void*)POWERUPSP, (void*)&oldPowerupSP, 6, NULL);
	//WriteProcessMemory(GetCurrentProcess(), (void*)ADMINCOMPARE, (void*)&oldComp, 6, NULL);

	//VirtualProtect((void*)TLCADDR, 4, PAGE_READWRITE, &flOldProtect);
	//WriteProcessMemory(GetCurrentProcess(), (void*)TLCADDR, "target_level_change", 20, NULL);
	//VirtualProtect((void*)TLCADDR, 4, flOldProtect, &flOldProtect);

	// for unloading support, respawn all entities with spawn function hooked, clean-up
	if (!shuttingDown)
	{
		//re = NULL;
		//while ((re = Lmd_Find(re, FOFS(classname), "lmd_plugin")) != NULL)
		//{
		//	Lmd_Edit(re->spawnData, "mapname", "nothing");
		//	Lmd_Edit(re->spawnData, "classname", "target_level_change");
		//	Lmd_SetSD(re, re->spawnData);
		//}
		//re = NULL;
		//while ((re = Lmd_Find(re, FOFS(classname), "target_powerup")) != NULL)
		//{
		//	Lmd_SetSD(re, re->spawnData);
		//}
	}
}

#pragma warning( default : 4244 )
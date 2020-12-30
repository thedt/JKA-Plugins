JASS - Jedi Academy Server Security
Copyright (C) 2010-2012 -1.#IND0000
http://jass.ucoz.net/

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

Created By:
       -1.#IND0000 <buffer.oveflow.x86@gmail.com>.

Installation:
    1. Open a folder with mod in which you wish to place JASS (usually it is base);
    2. Find a file jampgamex86.dll. Rename to jass_jampgamex86.dll.
    3. Unpack zzz_jass.pk3, jass.dll, jassconfig.cfg and plugins folder into a folder 
       with mod ("base", "japlus", "MBII", "clanmod", "lugormod", "makermod", etc.).
	4. Rename jass.dll to jampgamex86.dll
	5. Unpack jass.ini and pdb.so into a folder Gamedata. 
    6. Adjust jass.ini, folowing the comments.

Description:
JAZZ - Jedi Academy Server Security. In short, jazz is a project created to protect servers on the so-called basic mod - "basejka". Yet, it also increasing protection of other mods. Based on the QMM source code. Jazz by itself is open-source, but source code of the protection modules will not be published. List of fixed vulnerabilities:
1) msgbof.
2) votebof.
3) infobof.
4) forcecrash.
5) teamcrash.
6) votebug/cbufexec.
7) dirtrav.
8) npccrash.
9) limitbug.
10) gcrash.
11) RCON disabler.
12) RCON bruteforce.
13) Fake Players DDoS.
14) RandomPacket DDoS.
15) UnBan.
16) Remove all possibilities of damage to the game content - for example, by writeconfig.
17) Removed posibillity to «kill server» — to shutdown it that way that restart script won't work.
18) Well, little more things.
To get list of jass commands, type in server console command "jass".
Usage: jass <command> [params]
Available sub commands:
    status - displays information about JASS.
    list - displays information about loaded JASS plugins.
    load <file> - loads a new plugin.
    info <id/str> - outputs info on plugin with id.
    pause <id/str> - pauses plugin with id.
    unpause <id/str> - unpauses plugin with id.
    unload <id/str> - unloads plugin with id.
    force_unload <id/str> - forcefully unloads plugin with id.
    shutdown - permanently unloads JASS except for neccesary functions.
Folowing commands will be available only if AntiDDoS plugin is loaded:
    ban <IP/range> <minutes> - bans IP via JASS.
    unban <IP/range> - unbans IP via JASS. Type "all" to clear banlist.
    banlist [list_num] [count] - shows banlist.

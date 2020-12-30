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
18) ChatFlood (donedl)
19) Well, little more things.
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

Installation:
    1. Open a folder with mod in which you wish to place JASS (usually it is base);
    2. Find a file jampgamei386.so. Rename to jass_jampgamei386.so.
    3. Unpack jass.so, jassconfig.cfg and plugins folder into a folder with mod
       ("base", "japlus", "MBII", "clanmod", "lugormod", "makermod", etc.).
	4. Rename jass.so to jampgamei386.so
	5. Unpack jass.ini and pdb.so into a folder Gamedata.
    6. Adjust jass.ini. To do that, open it via any text editor (notepad, notepad++, gedit), and edit it folowing the comments.
	
Customization:
In jass.ini settings for every mod folder are defined separately. Initially only settings for basejka are included.
Settings block for every folder looks like this:
#Folder name
"folder_name"{
  <settings>
}
To install jass into any other folder you must copy this block and change folder name from "base" to something else, like "japlus", "lugormod", "MBII" and so on. Remember, if you'll avoid this step, protection won't work because protection plugins won't load. Every string with # char at start is ignored by jass. It's comments that tells you how to customize jass. Every settings string looks like this one:
"setting_name" "text_value";
or
"setting_name" 'numeric_value';
Value could be freely changed.

Plugins installation:
Initially there is two plugins - AntiDDoS and Protection. Both plugins gives your server protection. This plugins load is defined in jass.ini by default. Btw, as mentioned above, jass.ini contains settings for base only, so ajss will load them by default only for basejka. You can load any other plugin simulationally. To do thi add a string in "plugins" section with path to new plugin:
		"plugins/plugin.so";
To ensure that plugin is loaded use cmd "jass list" in server console.

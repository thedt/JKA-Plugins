#include "jka/game/q_shared.h"
#include "jka/game/g_local.h"
#include "Jass/jassapi.h"
#include "Jass/version.h" 
#include "XG-Address.h"
#include "XG-Structure.h"
#include "XG-Function.h"
#include "XG-Commands.h"
#include "XG-Hook.h"
#include "IniFile.h"
#include <sstream>
#include <string.h>
#include <cmath>

extern string printlist;
extern string ss;
extern string StringPadding(string original, size_t charCount );
#define va JASS_VARARGS

/* Ofront+ 0.9 -s */

#ifndef Watson0__h
#define Watson0__h

#include "SYSTEM.h"
#include "Texts.h"


import CHAR Watson0_options[12];


import void Watson0_ShowCmd (CHAR *file, INTEGER file__len, Texts_Text T);
import void Watson0_ShowDef (CHAR *mod, INTEGER mod__len, Texts_Text T);
import void Watson0_ShowObj (CHAR *mod, INTEGER mod__len, CHAR *objName, INTEGER objName__len, Texts_Text T);
import void *Watson0__init(void);


#endif

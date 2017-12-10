/* Ofront+ 1.0 -sC -44 */

#ifndef Modules__h
#define Modules__h

#include "SYSTEM.h"
#include "Heap.h"


import INTEGER Modules_res;
import CHAR Modules_resMsg[256];
import Heap_ModuleName Modules_imported, Modules_importing;


import void Modules_Free (CHAR *name, INTEGER name__len, BOOLEAN all);
import Heap_Command Modules_ThisCommand (Heap_Module mod, CHAR *name, INTEGER name__len);
import Heap_Module Modules_ThisMod (CHAR *name, INTEGER name__len);
import void *Modules__init(void);


#endif

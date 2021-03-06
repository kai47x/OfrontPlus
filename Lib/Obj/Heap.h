/* Ofront+ 1.0 -sxapC -48 */

#ifndef Heap__h
#define Heap__h

#include "SYSTEM.h"

typedef
	struct Heap_CmdDesc *Heap_Cmd;

typedef
	CHAR Heap_CmdName[24];

typedef
	void (*Heap_Command)(void);

typedef
	struct Heap_CmdDesc {
		Heap_Cmd next;
		Heap_CmdName name;
		Heap_Command cmd;
	} Heap_CmdDesc;

typedef
	void (*Heap_EnumProc)(void(*)(SYSTEM_PTR));

typedef
	void (*Heap_Finalizer)(SYSTEM_PTR);

typedef
	struct Heap_ModuleDesc *Heap_Module;

typedef
	CHAR Heap_ModuleName[20];

typedef
	struct Heap_ModuleDesc {
		Heap_Module next;
		Heap_ModuleName name;
		INTEGER refcnt;
		Heap_Cmd cmds;
		INTEGER types;
		Heap_EnumProc enumPtrs;
		char _prvt0[8];
	} Heap_ModuleDesc;


import SYSTEM_PTR Heap_modules;
import INTEGER Heap_allocated;
import INTEGER Heap_heap;
import INTEGER Heap_heapsize, Heap_heapMinExpand;
import SHORTINT Heap_FileCount;

import SYSTEM_ADRINT *Heap_ModuleDesc__typ;
import SYSTEM_ADRINT *Heap_CmdDesc__typ;

import void Heap_FINALL (void);
import INTEGER Heap_FreeModule (CHAR *name, INTEGER name__len);
import void Heap_GC (BOOLEAN markStack);
import void Heap_INCREF (Heap_Module m);
import void Heap_InitHeap (void);
import void Heap_Lock (void);
import SYSTEM_PTR Heap_NEWBLK (INTEGER size);
import SYSTEM_PTR Heap_NEWREC (INTEGER tag);
import void Heap_REGCMD (Heap_Module m, Heap_CmdName name, Heap_Command cmd);
import SYSTEM_PTR Heap_REGMOD (Heap_ModuleName name, Heap_EnumProc enumPtrs);
import void Heap_REGTYP (Heap_Module m, INTEGER typ);
import void Heap_RegisterFinalizer (SYSTEM_PTR obj, Heap_Finalizer finalize);
import void Heap_Unlock (void);
import void *Heap__init(void);


#endif

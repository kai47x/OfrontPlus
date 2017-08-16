/* Ofront+ 1.0 -s */

#ifndef Kernel__h
#define Kernel__h

#include "SYSTEM.h"

typedef
	void (*Kernel_HaltProcedure)(INTEGER);

typedef
	void (*Kernel_SignalHandler)(INTEGER);


import INTEGER Kernel_HaltCode;


import void Kernel_AssertFail (INTEGER code);
import void Kernel_Exit (INTEGER code);
import void Kernel_Halt (INTEGER code);
import void Kernel_SetHalt (Kernel_HaltProcedure p);
import void *Kernel__init(void);

#define Kernel_SetBadInstructionHandler(h)	SystemSetBadInstructionHandler((SYSTEM_ADRINT)h)
#define Kernel_SetInterruptHandler(h)	SystemSetInterruptHandler((SYSTEM_ADRINT)h)
#define Kernel_SetQuitHandler(h)	SystemSetQuitHandler((SYSTEM_ADRINT)h)

#endif

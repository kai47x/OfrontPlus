/* Ofront+ 0.9 -xtspkae */
#include "SYSTEM.h"
#include "Console.h"
#include "Platform.h"

typedef
	void (*Kernel_HaltProcedure)(INTEGER);

typedef
	void (*Kernel_SignalHandler)(INTEGER);


export INTEGER Kernel_HaltCode;


export void Kernel_AssertFail (INTEGER code);
static void Kernel_DisplayHaltCode (INTEGER code);
export void Kernel_Exit (INTEGER code);
export void Kernel_Halt (INTEGER code);
export void Kernel_SetHalt (Kernel_HaltProcedure p);

extern void (*SYSTEM_AssertFailHandler)(INTEGER code);
extern void (*SYSTEM_HaltHandler)(INTEGER code);
#define Kernel_SetAssertFail(p)	SYSTEM_AssertFailHandler = p
#define Kernel_SetBadInstructionHandler(h)	SystemSetBadInstructionHandler((SYSTEM_ADR)h)
#define Kernel_SetHaltHandler(p)	SYSTEM_HaltHandler = p
#define Kernel_SetInterruptHandler(h)	SystemSetInterruptHandler((SYSTEM_ADR)h)
#define Kernel_SetQuitHandler(h)	SystemSetQuitHandler((SYSTEM_ADR)h)

/*============================================================================*/

static void Kernel_DisplayHaltCode (INTEGER code)
{
	switch (code) {
		case -1: 
			Console_String((CHAR*)"Rider ReadBuf/WriteBuf transfer size longer than buffer.", 57);
			break;
		case -2: 
			Console_String((CHAR*)"Index out of range.", 20);
			break;
		case -3: 
			Console_String((CHAR*)"Reached end of function without reaching RETURN.", 49);
			break;
		case -4: 
			Console_String((CHAR*)"CASE statement: no matching label and no ELSE.", 47);
			break;
		case -5: 
			Console_String((CHAR*)"Type guard failed.", 19);
			break;
		case -6: 
			Console_String((CHAR*)"Type equality failed.", 22);
			break;
		case -7: 
			Console_String((CHAR*)"WITH statement type guard failed.", 34);
			break;
		case -8: 
			Console_String((CHAR*)"SHORT: Value too large for shorter type.", 41);
			break;
		case -9: 
			Console_String((CHAR*)"Heap interrupted while locked, but lockdepth = 0 at unlock.", 60);
			break;
		case -15: 
			Console_String((CHAR*)"Type descriptor size mismatch.", 31);
			break;
		case -20: 
			Console_String((CHAR*)"Too many, or negative number of, elements in dynamic array.", 60);
			break;
		case -25: 
			Console_String((CHAR*)"Memory allocation error.", 25);
			break;
		default: 
			break;
	}
}

void Kernel_Halt (INTEGER code)
{
	Kernel_HaltCode = code;
	Console_String((CHAR*)"Terminated by Halt(", 20);
	Console_Int(code, 0);
	Console_String((CHAR*)"). ", 4);
	if (code < 0) {
		Kernel_DisplayHaltCode(code);
	}
	Console_Ln();
	Platform_ExitOS(code);
}

/*----------------------------------------------------------------------------*/
void Kernel_AssertFail (INTEGER code)
{
	Console_String((CHAR*)"Assertion failure.", 19);
	if (code != 0) {
		Console_String((CHAR*)" ASSERT code ", 14);
		Console_Int(code, 0);
		Console_Char('.');
	}
	Console_Ln();
	Platform_ExitOS(code);
}

/*----------------------------------------------------------------------------*/
void Kernel_Exit (INTEGER code)
{
	Platform_ExitOS(code);
}

/*----------------------------------------------------------------------------*/
void Kernel_SetHalt (Kernel_HaltProcedure p)
{
	Kernel_SetHaltHandler(p);
}

/*----------------------------------------------------------------------------*/

export void *Kernel__init(void)
{
	__DEFMOD;
	__IMPORT(Console__init);
	__IMPORT(Platform__init);
	__REGMOD("Kernel", 0);
/* BEGIN */
	Kernel_HaltCode = -128;
	Kernel_SetHalt(Kernel_Halt);
	Kernel_SetAssertFail(Kernel_AssertFail);
	__ENDMOD;
}

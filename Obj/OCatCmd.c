/* Ofront+ 0.9 -xtspkaem */
#include "SYSTEM.h"
#include "Args.h"
#include "Console.h"
#include "Files.h"
#include "Texts.h"




export void OCatCmd_Cat (void);


/*============================================================================*/

void OCatCmd_Cat (void)
{
	BOOLEAN optTab;
	CHAR path[128];
	INTEGER arg;
	Texts_Text T = NIL;
	Texts_Reader R;
	CHAR ch;
	optTab = 0;
	path[0] = 0x00;
	arg = 1;
	__NEW(T, Texts_TextDesc);
	Args_Get(1, (void*)path, 128);
	if (__STRCMP(path, "-t") == 0) {
		optTab = 1;
		arg = 2;
		path[0] = 0x00;
		Args_Get(2, (void*)path, 128);
	}
	while (path[0] != 0x00) {
		if (Files_Old(path, 128) != NIL) {
			Texts_Open(T, path, 128);
			Texts_OpenReader(&R, Texts_Reader__typ, T, 0);
			Texts_Read(&R, Texts_Reader__typ, &ch);
			while (!R.eot) {
				if (ch >= ' ') {
					Console_Char(ch);
				} else if (ch == 0x09) {
					if (optTab) {
						Console_Char(0x09);
					} else {
						Console_String((CHAR*)"  ", 3);
					}
				} else if (ch == 0x0d) {
					Console_Char(0x0a);
				}
				Texts_Read(&R, Texts_Reader__typ, &ch);
			}
		} else {
			Console_String((CHAR*)"OCat: cannot open ", 19);
			Console_String(path, 128);
			Console_Ln();
		}
		arg += 1;
		path[0] = 0x00;
		Args_Get(arg, (void*)path, 128);
	}
	Console_Flush();
}

/*----------------------------------------------------------------------------*/

int main(int argc, char **argv)
{
	__INIT(argc, argv);
	__IMPORT(Args__init);
	__IMPORT(Console__init);
	__IMPORT(Files__init);
	__IMPORT(Texts__init);
	__REGMAIN("OCatCmd", 0);
	__REGCMD("Cat", OCatCmd_Cat);
/* BEGIN */
	OCatCmd_Cat();
	__FINI;
}

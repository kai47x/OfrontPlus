/* Ofront+ 0.9 -xtspkae */
#include "SYSTEM.h"
#include "Args.h"
#include "Console.h"
#include "Files.h"
#include "OfrontErrors.h"
#include "Platform.h"
#include "Texts.h"

typedef
	CHAR OfrontOPM_FileName[32];


export INTEGER OfrontOPM_ByteSize, OfrontOPM_CharSize, OfrontOPM_BoolSize, OfrontOPM_SIntSize, OfrontOPM_IntSize, OfrontOPM_LIntSize, OfrontOPM_SetSize, OfrontOPM_RealSize, OfrontOPM_LRealSize, OfrontOPM_PointerSize, OfrontOPM_ProcSize, OfrontOPM_RecSize, OfrontOPM_ArrLenSize, OfrontOPM_CharAlign, OfrontOPM_BoolAlign, OfrontOPM_SIntAlign, OfrontOPM_IntAlign, OfrontOPM_LIntAlign, OfrontOPM_SetAlign, OfrontOPM_RealAlign, OfrontOPM_LRealAlign, OfrontOPM_PointerAlign, OfrontOPM_ProcAlign, OfrontOPM_RecAlign, OfrontOPM_ByteOrder, OfrontOPM_BitOrder, OfrontOPM_MaxSet, OfrontOPM_MinSInt, OfrontOPM_MinInt, OfrontOPM_MaxSInt, OfrontOPM_MaxInt, OfrontOPM_MaxIndex;
export LONGINT OfrontOPM_MinLInt, OfrontOPM_MaxLInt;
export LONGREAL OfrontOPM_MinReal, OfrontOPM_MaxReal, OfrontOPM_MinLReal, OfrontOPM_MaxLReal;
export BOOLEAN OfrontOPM_noerr;
export INTEGER OfrontOPM_curpos;
export LONGINT OfrontOPM_errpos;
export INTEGER OfrontOPM_breakpc;
export INTEGER OfrontOPM_currFile, OfrontOPM_level;
export INTEGER OfrontOPM_pc, OfrontOPM_entno;
export CHAR OfrontOPM_modName[32];
export CHAR OfrontOPM_objname[64];
export SET OfrontOPM_opt, OfrontOPM_glbopt;
static INTEGER OfrontOPM_lasterrpos;
static Texts_Reader OfrontOPM_inR;
static Texts_Text OfrontOPM_Log;
static Texts_Writer OfrontOPM_W;
static Files_Rider OfrontOPM_oldSF, OfrontOPM_newSF;
static Files_Rider OfrontOPM_R[3];
static Files_File OfrontOPM_oldSFile, OfrontOPM_newSFile, OfrontOPM_HFile, OfrontOPM_BFile, OfrontOPM_HIFile;
static INTEGER OfrontOPM_S;
static BOOLEAN OfrontOPM_stop, OfrontOPM_useLineNo;
static CHAR OfrontOPM_OBERON[1024];


static void OfrontOPM_Append (Files_Rider *R, LONGINT *R__typ, Files_File F);
export void OfrontOPM_CloseFiles (void);
export void OfrontOPM_CloseOldSym (void);
export void OfrontOPM_DeleteNewSym (void);
export void OfrontOPM_FPrint (LONGINT *fp, LONGINT val);
export void OfrontOPM_FPrintLReal (LONGINT *fp, LONGREAL lr);
export void OfrontOPM_FPrintReal (LONGINT *fp, REAL real);
export void OfrontOPM_FPrintSet (LONGINT *fp, SET set);
export void OfrontOPM_Get (CHAR *ch);
static void OfrontOPM_GetProperties (void);
static void OfrontOPM_GetProperty (Texts_Scanner *S, LONGINT *S__typ, CHAR *name, INTEGER name__len, INTEGER *size, INTEGER *align);
static void OfrontOPM_GetProperty1 (Texts_Scanner *S, LONGINT *S__typ, CHAR *name, INTEGER name__len, INTEGER *par, INTEGER defval);
export void OfrontOPM_Init (BOOLEAN *done);
export void OfrontOPM_InitOptions (void);
static void OfrontOPM_LogErrMsg (INTEGER n);
export void OfrontOPM_LogW (CHAR ch);
export void OfrontOPM_LogWLn (void);
export void OfrontOPM_LogWNum (INTEGER i, INTEGER len);
export void OfrontOPM_LogWStr (CHAR *s, INTEGER s__len);
static void OfrontOPM_MakeFileName (CHAR *name, INTEGER name__len, CHAR *FName, INTEGER FName__len, CHAR *ext, INTEGER ext__len);
export void OfrontOPM_Mark (INTEGER n, LONGINT pos1);
export void OfrontOPM_NewSym (CHAR *modName, INTEGER modName__len);
export void OfrontOPM_OldSym (CHAR *modName, INTEGER modName__len, BOOLEAN *done);
export void OfrontOPM_OpenFiles (CHAR *moduleName, INTEGER moduleName__len);
export void OfrontOPM_OpenPar (void);
export void OfrontOPM_PromoteIntConstToLInt (void);
export void OfrontOPM_RegisterNewSym (void);
static void OfrontOPM_ScanOptions (CHAR *s, INTEGER s__len, SET *opt);
export void OfrontOPM_SymRCh (CHAR *ch);
export LONGINT OfrontOPM_SymRInt (void);
export void OfrontOPM_SymRLReal (LONGREAL *lr);
export void OfrontOPM_SymRReal (REAL *r);
export void OfrontOPM_SymRSet (SET *s);
export void OfrontOPM_SymWCh (CHAR ch);
export void OfrontOPM_SymWInt (LONGINT i);
export void OfrontOPM_SymWLReal (LONGREAL lr);
export void OfrontOPM_SymWReal (REAL r);
export void OfrontOPM_SymWSet (SET s);
export void OfrontOPM_Write (CHAR ch);
export void OfrontOPM_WriteHex (LONGINT i);
export void OfrontOPM_WriteInt (LONGINT i);
export void OfrontOPM_WriteLn (void);
export void OfrontOPM_WriteReal (LONGREAL r, CHAR suffx);
export void OfrontOPM_WriteString (CHAR *s, INTEGER s__len);
export void OfrontOPM_WriteStringVar (CHAR *s, INTEGER s__len);
export BOOLEAN OfrontOPM_eofSF (void);
export void OfrontOPM_err (INTEGER n);


/*============================================================================*/

void OfrontOPM_LogW (CHAR ch)
{
	Console_Char(ch);
}

/*----------------------------------------------------------------------------*/
void OfrontOPM_LogWStr (CHAR *s, INTEGER s__len)
{
	__DUP(s, s__len);
	Console_String(s, s__len);
	__DEL(s);
}

/*----------------------------------------------------------------------------*/
void OfrontOPM_LogWNum (INTEGER i, INTEGER len)
{
	Console_Int(i, len);
}

/*----------------------------------------------------------------------------*/
void OfrontOPM_LogWLn (void)
{
	Console_Ln();
}

/*----------------------------------------------------------------------------*/
static void OfrontOPM_ScanOptions (CHAR *s, INTEGER s__len, SET *opt)
{
	INTEGER i;
	i = 1;
	while (s[__X(i, s__len)] != 0x00) {
		switch (s[__X(i, s__len)]) {
			case 'e': 
				*opt = *opt ^ 0x0200;
				break;
			case 's': 
				*opt = *opt ^ 0x10;
				break;
			case 'm': 
				*opt = *opt ^ 0x0400;
				break;
			case 'x': 
				*opt = *opt ^ 0x01;
				break;
			case 'v': 
				*opt = *opt ^ 0x02;
				break;
			case 'r': 
				*opt = *opt ^ 0x04;
				break;
			case 't': 
				*opt = *opt ^ 0x08;
				break;
			case 'a': 
				*opt = *opt ^ 0x80;
				break;
			case 'k': 
				*opt = *opt ^ 0x40;
				break;
			case 'p': 
				*opt = *opt ^ 0x20;
				break;
			case 'i': 
				*opt = *opt ^ 0x0100;
				break;
			case 'l': 
				*opt = *opt ^ 0x0800;
				break;
			case '1': 
				*opt = (((*opt | 0x1000) & ~0x2000) & ~0x4000) & ~0x8000;
				break;
			case '2': 
				*opt = (((*opt | 0x2000) & ~0x1000) & ~0x4000) & ~0x8000;
				break;
			case 'c': 
				*opt = (((*opt | 0x4000) & ~0x1000) & ~0x2000) & ~0x8000;
				break;
			case '7': 
				*opt = (((*opt | 0x8000) & ~0x1000) & ~0x2000) & ~0x4000;
				break;
			default: 
				OfrontOPM_LogWStr((CHAR*)"  warning: option ", 19);
				OfrontOPM_LogW('-');
				OfrontOPM_LogW(s[__X(i, s__len)]);
				OfrontOPM_LogWStr((CHAR*)" ignored", 9);
				OfrontOPM_LogWLn();
				break;
		}
		i += 1;
	}
}

void OfrontOPM_OpenPar (void)
{
	CHAR s[256];
	if (Args_argc == 1) {
		OfrontOPM_stop = 1;
		Console_Ln();
		Console_String((CHAR*)"OfrontPlus - Oberon family of languages to C Translator v0.9", 61);
		Console_Ln();
		Console_String((CHAR*)"Copyright (c) Software Templ OEG, 1995-2007 & VEDAsoft Oberon Club, 2013-2016", 78);
		Console_Ln();
		Console_Ln();
		Console_String((CHAR*)"SYNOPSIS", 9);
		Console_Ln();
		Console_String((CHAR*)"  command = \"ofront+\" options {file options}.", 46);
		Console_Ln();
		Console_String((CHAR*)"  options = [\"-\" {option} ].", 29);
		Console_Ln();
		Console_String((CHAR*)"  option  = \"m\" | \"s\" | \"e\" | \"i\" | \"l\" | \"k\" | \"r\" | \"x\" | \"a\" | \"p\" | \"t\"", 76);
		Console_Ln();
		Console_String((CHAR*)"            \"1\" Oberon | \"2\" Oberon-2 | \"c\" Component Pascal | \"7\" Oberon-07", 77);
		Console_Ln();
	} else {
		OfrontOPM_GetProperties();
		OfrontOPM_glbopt = 0x08e9;
		OfrontOPM_S = 1;
		s[0] = 0x00;
		Args_Get(1, (void*)s, 256);
		OfrontOPM_stop = 0;
		while (s[0] == '-') {
			OfrontOPM_ScanOptions((void*)s, 256, &OfrontOPM_glbopt);
			OfrontOPM_S += 1;
			s[0] = 0x00;
			Args_Get(OfrontOPM_S, (void*)s, 256);
		}
	}
}

/*----------------------------------------------------------------------------*/
void OfrontOPM_InitOptions (void)
{
	CHAR s[256];
	OfrontOPM_opt = OfrontOPM_glbopt;
	s[0] = 0x00;
	Args_Get(OfrontOPM_S, (void*)s, 256);
	while (s[0] == '-') {
		OfrontOPM_ScanOptions((void*)s, 256, &OfrontOPM_opt);
		OfrontOPM_S += 1;
		s[0] = 0x00;
		Args_Get(OfrontOPM_S, (void*)s, 256);
	}
	if (__IN(11, OfrontOPM_opt)) {
		OfrontOPM_useLineNo = 1;
		OfrontOPM_curpos = 256;
		OfrontOPM_errpos = OfrontOPM_curpos;
		OfrontOPM_lasterrpos = OfrontOPM_curpos - 10;
	} else {
		OfrontOPM_useLineNo = 0;
	}
}

/*----------------------------------------------------------------------------*/
void OfrontOPM_Init (BOOLEAN *done)
{
	Texts_Text T = NIL;
	INTEGER beg, end, time;
	CHAR s[256];
	*done = 0;
	OfrontOPM_curpos = 0;
	if (OfrontOPM_stop || OfrontOPM_S >= Args_argc) {
		return;
	}
	s[0] = 0x00;
	Args_Get(OfrontOPM_S, (void*)s, 256);
	__NEW(T, Texts_TextDesc);
	Texts_Open(T, s, 256);
	OfrontOPM_LogWStr(s, 256);
	if (T->len == 0) {
		OfrontOPM_LogWStr((CHAR*)" not found", 11);
		OfrontOPM_LogWLn();
	} else {
		Texts_OpenReader(&OfrontOPM_inR, Texts_Reader__typ, T, 0);
		OfrontOPM_LogWStr((CHAR*)"  translating", 14);
		*done = 1;
	}
	OfrontOPM_S += 1;
	OfrontOPM_level = 0;
	OfrontOPM_noerr = 1;
	OfrontOPM_errpos = OfrontOPM_curpos;
	OfrontOPM_lasterrpos = OfrontOPM_curpos - 10;
}

/*----------------------------------------------------------------------------*/
void OfrontOPM_Get (CHAR *ch)
{
	Texts_Read(&OfrontOPM_inR, Texts_Reader__typ, &*ch);
	if (OfrontOPM_useLineNo) {
		if (*ch == 0x0d) {
			OfrontOPM_curpos = __ASHL(__ASHR(OfrontOPM_curpos, 8, INTEGER) + 1, 8, INTEGER);
		} else if (__MASK(OfrontOPM_curpos, -256) != 255) {
			OfrontOPM_curpos += 1;
		}
	} else if (*ch == 0x0d) {
		OfrontOPM_curpos = Texts_Pos(&OfrontOPM_inR, Texts_Reader__typ);
	} else {
		OfrontOPM_curpos += 1;
	}
	if (*ch < 0x09 && !OfrontOPM_inR.eot) {
		*ch = ' ';
	}
}

/*----------------------------------------------------------------------------*/
static void OfrontOPM_MakeFileName (CHAR *name, INTEGER name__len, CHAR *FName, INTEGER FName__len, CHAR *ext, INTEGER ext__len)
{
	INTEGER i, j;
	CHAR ch;
	__DUP(ext, ext__len);
	i = 0;
	for (;;) {
		ch = name[__X(i, name__len)];
		if (ch == 0x00) {
			break;
		}
		FName[__X(i, FName__len)] = ch;
		i += 1;
	}
	j = 0;
	do {
		ch = ext[__X(j, ext__len)];
		FName[__X(i, FName__len)] = ch;
		i += 1;
		j += 1;
	} while (!(ch == 0x00));
	__DEL(ext);
}

static void OfrontOPM_LogErrMsg (INTEGER n)
{
	if (n >= 0) {
		OfrontOPM_LogWStr((CHAR*)"  err ", 7);
	} else {
		OfrontOPM_LogWStr((CHAR*)"  warning ", 11);
		n = -n;
	}
	OfrontOPM_LogWNum(n, 1);
	OfrontOPM_LogWStr((CHAR*)"  ", 3);
	OfrontErrors_LogErrMsg(n);
}

void OfrontOPM_Mark (INTEGER n, LONGINT pos1)
{
	INTEGER pos;
	pos = (INTEGER)pos1;
	if (OfrontOPM_useLineNo) {
		if (n >= 0) {
			OfrontOPM_noerr = 0;
			if (pos < OfrontOPM_lasterrpos || OfrontOPM_lasterrpos + 9 < pos) {
				OfrontOPM_lasterrpos = pos;
				OfrontOPM_LogWLn();
				OfrontOPM_LogWStr((CHAR*)"  ", 3);
				if (n < 249) {
					OfrontOPM_LogWStr((CHAR*)"  line ", 8);
					OfrontOPM_LogWNum(__ASHR(pos, 8, INTEGER), 1);
					OfrontOPM_LogWStr((CHAR*)"  pos ", 7);
					OfrontOPM_LogWNum(__MASK(pos, -256), 1);
					OfrontOPM_LogErrMsg(n);
				} else if (n == 255) {
					OfrontOPM_LogWStr((CHAR*)"  line ", 8);
					OfrontOPM_LogWNum(__ASHR(pos, 8, INTEGER), 1);
					OfrontOPM_LogWStr((CHAR*)"  pos ", 7);
					OfrontOPM_LogWNum(__MASK(pos, -256), 1);
					OfrontOPM_LogWStr((CHAR*)"  pc ", 6);
					OfrontOPM_LogWNum(OfrontOPM_breakpc, 1);
				} else if (n == 254) {
					OfrontOPM_LogWStr((CHAR*)"pc not found", 13);
				} else {
					OfrontOPM_LogWStr(OfrontOPM_objname, 64);
					if (n == 253) {
						OfrontOPM_LogWStr((CHAR*)" is new, compile with option e", 31);
					} else if (n == 252) {
						OfrontOPM_LogWStr((CHAR*)" is redefined, compile with option s", 37);
					} else if (n == 251) {
						OfrontOPM_LogWStr((CHAR*)" is redefined (private part only), compile with option s", 57);
					} else if (n == 250) {
						OfrontOPM_LogWStr((CHAR*)" is no longer visible, compile with option s", 45);
					} else if (n == 249) {
						OfrontOPM_LogWStr((CHAR*)" is not consistently imported, recompile imports", 49);
					}
				}
			}
		} else {
			if (pos >= 0) {
				OfrontOPM_LogWLn();
				OfrontOPM_LogWStr((CHAR*)"  line ", 8);
				OfrontOPM_LogWNum(__ASHR(pos, 8, INTEGER), 1);
				OfrontOPM_LogWStr((CHAR*)"  pos ", 7);
				OfrontOPM_LogWNum(__MASK(pos, -256), 1);
			}
			OfrontOPM_LogErrMsg(n);
			if (pos < 0) {
				OfrontOPM_LogWLn();
			}
		}
	} else {
		if (n >= 0) {
			OfrontOPM_noerr = 0;
			if (pos < OfrontOPM_lasterrpos || OfrontOPM_lasterrpos + 9 < pos) {
				OfrontOPM_lasterrpos = pos;
				OfrontOPM_LogWLn();
				OfrontOPM_LogWStr((CHAR*)"  ", 3);
				if (n < 249) {
					OfrontOPM_LogWStr((CHAR*)"  pos", 6);
					OfrontOPM_LogWNum(pos, 6);
					OfrontOPM_LogErrMsg(n);
				} else if (n == 255) {
					OfrontOPM_LogWStr((CHAR*)"pos", 4);
					OfrontOPM_LogWNum(pos, 6);
					OfrontOPM_LogWStr((CHAR*)"  pc ", 6);
					OfrontOPM_LogWNum(OfrontOPM_breakpc, 1);
				} else if (n == 254) {
					OfrontOPM_LogWStr((CHAR*)"pc not found", 13);
				} else {
					OfrontOPM_LogWStr(OfrontOPM_objname, 64);
					if (n == 253) {
						OfrontOPM_LogWStr((CHAR*)" is new, compile with option e", 31);
					} else if (n == 252) {
						OfrontOPM_LogWStr((CHAR*)" is redefined, compile with option s", 37);
					} else if (n == 251) {
						OfrontOPM_LogWStr((CHAR*)" is redefined (private part only), compile with option s", 57);
					} else if (n == 250) {
						OfrontOPM_LogWStr((CHAR*)" is no longer visible, compile with option s", 45);
					} else if (n == 249) {
						OfrontOPM_LogWStr((CHAR*)" is not consistently imported, recompile imports", 49);
					}
				}
			}
		} else {
			if (pos >= 0) {
				OfrontOPM_LogWLn();
				OfrontOPM_LogWStr((CHAR*)"  pos", 6);
				OfrontOPM_LogWNum(pos, 6);
			}
			OfrontOPM_LogErrMsg(n);
			if (pos < 0) {
				OfrontOPM_LogWLn();
			}
		}
	}
}

/*----------------------------------------------------------------------------*/
void OfrontOPM_err (INTEGER n)
{
	if (OfrontOPM_useLineNo && __MASK(OfrontOPM_errpos, -256) == 255) {
		OfrontOPM_Mark(n, OfrontOPM_errpos + 1);
	} else {
		OfrontOPM_Mark(n, OfrontOPM_errpos);
	}
}

/*----------------------------------------------------------------------------*/
void OfrontOPM_FPrint (LONGINT *fp, LONGINT val)
{
	INTEGER fp1, val1;
	fp1 = (INTEGER)*fp;
	val1 = (INTEGER)val;
	*fp = __ROTL((INTEGER)(__VAL(SET, fp1) ^ __VAL(SET, val1)), 1, INTEGER);
}

/*----------------------------------------------------------------------------*/
void OfrontOPM_FPrintSet (LONGINT *fp, SET set)
{
	OfrontOPM_FPrint(&*fp, __VAL(INTEGER, set));
}

/*----------------------------------------------------------------------------*/
void OfrontOPM_FPrintReal (LONGINT *fp, REAL real)
{
	OfrontOPM_FPrint(&*fp, __VAL(INTEGER, real));
}

/*----------------------------------------------------------------------------*/
void OfrontOPM_FPrintLReal (LONGINT *fp, LONGREAL lr)
{
	INTEGER l, h;
	__GET((LONGINT)&lr, l, INTEGER);
	__GET((LONGINT)&lr + 4, h, INTEGER);
	OfrontOPM_FPrint(&*fp, l);
	OfrontOPM_FPrint(&*fp, h);
}

/*----------------------------------------------------------------------------*/
static void OfrontOPM_GetProperty (Texts_Scanner *S, LONGINT *S__typ, CHAR *name, INTEGER name__len, INTEGER *size, INTEGER *align)
{
	__DUP(name, name__len);
	if ((*S).class == 1 && __STRCMP((*S).s, name) == 0) {
		Texts_Scan(&*S, S__typ);
		if ((*S).class == 3) {
			*size = (*S).i;
			Texts_Scan(&*S, S__typ);
		} else {
			OfrontOPM_Mark(-157, -1);
		}
		if ((*S).class == 3) {
			*align = (*S).i;
			Texts_Scan(&*S, S__typ);
		} else {
			OfrontOPM_Mark(-157, -1);
		}
	} else {
		OfrontOPM_Mark(-157, -1);
	}
	__DEL(name);
}

static void OfrontOPM_GetProperty1 (Texts_Scanner *S, LONGINT *S__typ, CHAR *name, INTEGER name__len, INTEGER *par, INTEGER defval)
{
	__DUP(name, name__len);
	if ((*S).class == 1 && __STRCMP((*S).s, name) == 0) {
		Texts_Scan(&*S, S__typ);
		if ((*S).class == 3) {
			*par = (*S).i;
			Texts_Scan(&*S, S__typ);
		} else {
			OfrontOPM_Mark(-157, -1);
			*par = defval;
		}
	} else {
		*par = defval;
	}
	__DEL(name);
}

static void OfrontOPM_GetProperties (void)
{
	Texts_Text T = NIL;
	Texts_Scanner S;
	INTEGER base;
	OfrontOPM_ByteSize = 1;
	OfrontOPM_CharSize = 1;
	OfrontOPM_BoolSize = 1;
	OfrontOPM_SIntSize = 1;
	OfrontOPM_IntSize = 2;
	OfrontOPM_LIntSize = 4;
	OfrontOPM_SetSize = 4;
	OfrontOPM_RealSize = 4;
	OfrontOPM_LRealSize = 8;
	OfrontOPM_ProcSize = 4;
	OfrontOPM_PointerSize = 4;
	OfrontOPM_RecSize = 1;
	OfrontOPM_CharAlign = 1;
	OfrontOPM_BoolAlign = 1;
	OfrontOPM_SIntAlign = 1;
	OfrontOPM_IntAlign = 2;
	OfrontOPM_LIntAlign = 4;
	OfrontOPM_SetAlign = 4;
	OfrontOPM_RealAlign = 4;
	OfrontOPM_LRealAlign = 8;
	OfrontOPM_ProcAlign = 4;
	OfrontOPM_PointerAlign = 4;
	OfrontOPM_RecAlign = 1;
	OfrontOPM_MinSInt = -128;
	OfrontOPM_MinInt = -32768;
	OfrontOPM_MinLInt = (-2147483647-1);
	OfrontOPM_MaxSInt = 127;
	OfrontOPM_MaxInt = 32767;
	OfrontOPM_MaxLInt = 2147483647;
	OfrontOPM_MaxSet = 31;
	OfrontOPM_ArrLenSize = -1;
	__NEW(T, Texts_TextDesc);
	Texts_Open(T, (CHAR*)"Ofront.par", 11);
	if (T->len != 0) {
		Texts_OpenScanner(&S, Texts_Scanner__typ, T, 0);
		Texts_Scan(&S, Texts_Scanner__typ);
		OfrontOPM_GetProperty(&S, Texts_Scanner__typ, (CHAR*)"CHAR", 5, &OfrontOPM_CharSize, &OfrontOPM_CharAlign);
		OfrontOPM_GetProperty(&S, Texts_Scanner__typ, (CHAR*)"BOOLEAN", 8, &OfrontOPM_BoolSize, &OfrontOPM_BoolAlign);
		OfrontOPM_GetProperty(&S, Texts_Scanner__typ, (CHAR*)"SHORTINT", 9, &OfrontOPM_SIntSize, &OfrontOPM_SIntAlign);
		OfrontOPM_GetProperty(&S, Texts_Scanner__typ, (CHAR*)"INTEGER", 8, &OfrontOPM_IntSize, &OfrontOPM_IntAlign);
		OfrontOPM_GetProperty(&S, Texts_Scanner__typ, (CHAR*)"LONGINT", 8, &OfrontOPM_LIntSize, &OfrontOPM_LIntAlign);
		OfrontOPM_GetProperty(&S, Texts_Scanner__typ, (CHAR*)"SET", 4, &OfrontOPM_SetSize, &OfrontOPM_SetAlign);
		OfrontOPM_GetProperty(&S, Texts_Scanner__typ, (CHAR*)"REAL", 5, &OfrontOPM_RealSize, &OfrontOPM_RealAlign);
		OfrontOPM_GetProperty(&S, Texts_Scanner__typ, (CHAR*)"LONGREAL", 9, &OfrontOPM_LRealSize, &OfrontOPM_LRealAlign);
		OfrontOPM_GetProperty(&S, Texts_Scanner__typ, (CHAR*)"PTR", 4, &OfrontOPM_PointerSize, &OfrontOPM_PointerAlign);
		OfrontOPM_GetProperty(&S, Texts_Scanner__typ, (CHAR*)"PROC", 5, &OfrontOPM_ProcSize, &OfrontOPM_ProcAlign);
		OfrontOPM_GetProperty(&S, Texts_Scanner__typ, (CHAR*)"RECORD", 7, &OfrontOPM_RecSize, &OfrontOPM_RecAlign);
		OfrontOPM_GetProperty1(&S, Texts_Scanner__typ, (CHAR*)"ARRLEN", 7, &OfrontOPM_ArrLenSize, -1);
		OfrontOPM_GetProperty(&S, Texts_Scanner__typ, (CHAR*)"ENDIAN", 7, &OfrontOPM_ByteOrder, &OfrontOPM_BitOrder);
	} else {
		OfrontOPM_Mark(-156, -1);
	}
	if (OfrontOPM_RealSize == 4) {
		OfrontOPM_MaxReal = 3.402823466000001E+38;
	} else if (OfrontOPM_RealSize == 8) {
		OfrontOPM_MaxReal = 1.79769296342094E+308;
	}
	if (OfrontOPM_LRealSize == 4) {
		OfrontOPM_MaxLReal = 3.402823466000001E+38;
	} else if (OfrontOPM_LRealSize == 8) {
		OfrontOPM_MaxLReal = 1.79769296342094E+308;
	}
	OfrontOPM_MinReal = -OfrontOPM_MaxReal;
	OfrontOPM_MinLReal = -OfrontOPM_MaxLReal;
	if (OfrontOPM_SIntSize == 2) {
		OfrontOPM_MinSInt = -32768;
		OfrontOPM_MaxSInt = 32767;
	}
	if (OfrontOPM_IntSize == 4) {
		OfrontOPM_MinInt = (-2147483647-1);
		OfrontOPM_MaxInt = 2147483647;
	}
	if (OfrontOPM_LIntSize == 8) {
		base = 63;
		OfrontOPM_MinLInt = __LSH((LONGINT)1, base, LONGINT);
		OfrontOPM_MaxLInt = -(OfrontOPM_MinLInt + 1);
	}
	OfrontOPM_MaxSet = __ASHL(OfrontOPM_SetSize, 3, INTEGER) - 1;
	if (OfrontOPM_ArrLenSize == -1) {
		OfrontOPM_ArrLenSize = OfrontOPM_LIntSize;
	} else if (!((OfrontOPM_ArrLenSize == OfrontOPM_SIntSize || OfrontOPM_ArrLenSize == OfrontOPM_IntSize) || OfrontOPM_ArrLenSize == OfrontOPM_LIntSize)) {
		OfrontOPM_Mark(-157, -1);
	}
	OfrontOPM_MaxIndex = 2147483647;
}

void OfrontOPM_SymRCh (CHAR *ch)
{
	Files_ReadChar(&OfrontOPM_oldSF, Files_Rider__typ, &*ch);
}

/*----------------------------------------------------------------------------*/
LONGINT OfrontOPM_SymRInt (void)
{
	LONGINT k;
	Files_ReadNum(&OfrontOPM_oldSF, Files_Rider__typ, &k);
	return k;
}

/*----------------------------------------------------------------------------*/
void OfrontOPM_SymRSet (SET *s)
{
	LONGINT k;
	Files_ReadNum(&OfrontOPM_oldSF, Files_Rider__typ, &k);
	*s = (SET)((INTEGER)k);
}

/*----------------------------------------------------------------------------*/
void OfrontOPM_SymRReal (REAL *r)
{
	Files_ReadReal(&OfrontOPM_oldSF, Files_Rider__typ, &*r);
}

/*----------------------------------------------------------------------------*/
void OfrontOPM_SymRLReal (LONGREAL *lr)
{
	Files_ReadLReal(&OfrontOPM_oldSF, Files_Rider__typ, &*lr);
}

/*----------------------------------------------------------------------------*/
void OfrontOPM_CloseOldSym (void)
{
}

/*----------------------------------------------------------------------------*/
void OfrontOPM_OldSym (CHAR *modName, INTEGER modName__len, BOOLEAN *done)
{
	CHAR ch;
	OfrontOPM_FileName fileName;
	OfrontOPM_MakeFileName((void*)modName, modName__len, (void*)fileName, 32, (CHAR*)".sym", 5);
	OfrontOPM_oldSFile = Files_Old(fileName, 32);
	*done = OfrontOPM_oldSFile != NIL;
	if (*done) {
		Files_Set(&OfrontOPM_oldSF, Files_Rider__typ, OfrontOPM_oldSFile, 0);
		Files_ReadChar(&OfrontOPM_oldSF, Files_Rider__typ, &ch);
		if (ch != 0xf7) {
			OfrontOPM_err(-306);
			OfrontOPM_CloseOldSym();
			*done = 0;
		}
	}
}

/*----------------------------------------------------------------------------*/
BOOLEAN OfrontOPM_eofSF (void)
{
	return OfrontOPM_oldSF.eof;
}

/*----------------------------------------------------------------------------*/
void OfrontOPM_SymWCh (CHAR ch)
{
	Files_WriteChar(&OfrontOPM_newSF, Files_Rider__typ, ch);
}

/*----------------------------------------------------------------------------*/
void OfrontOPM_SymWInt (LONGINT i)
{
	Files_WriteNum(&OfrontOPM_newSF, Files_Rider__typ, i);
}

/*----------------------------------------------------------------------------*/
void OfrontOPM_SymWSet (SET s)
{
	Files_WriteNum(&OfrontOPM_newSF, Files_Rider__typ, __VAL(INTEGER, s));
}

/*----------------------------------------------------------------------------*/
void OfrontOPM_SymWReal (REAL r)
{
	Files_WriteReal(&OfrontOPM_newSF, Files_Rider__typ, r);
}

/*----------------------------------------------------------------------------*/
void OfrontOPM_SymWLReal (LONGREAL lr)
{
	Files_WriteLReal(&OfrontOPM_newSF, Files_Rider__typ, lr);
}

/*----------------------------------------------------------------------------*/
void OfrontOPM_RegisterNewSym (void)
{
	if (__STRCMP(OfrontOPM_modName, "SYSTEM") != 0 || __IN(10, OfrontOPM_opt)) {
		Files_Register(OfrontOPM_newSFile);
	}
}

/*----------------------------------------------------------------------------*/
void OfrontOPM_DeleteNewSym (void)
{
}

/*----------------------------------------------------------------------------*/
void OfrontOPM_NewSym (CHAR *modName, INTEGER modName__len)
{
	OfrontOPM_FileName fileName;
	OfrontOPM_MakeFileName((void*)modName, modName__len, (void*)fileName, 32, (CHAR*)".sym", 5);
	OfrontOPM_newSFile = Files_New(fileName, 32);
	if (OfrontOPM_newSFile != NIL) {
		Files_Set(&OfrontOPM_newSF, Files_Rider__typ, OfrontOPM_newSFile, 0);
		Files_WriteChar(&OfrontOPM_newSF, Files_Rider__typ, 0xf7);
	} else {
		OfrontOPM_err(153);
	}
}

/*----------------------------------------------------------------------------*/
void OfrontOPM_Write (CHAR ch)
{
	Files_WriteChar(&OfrontOPM_R[__X(OfrontOPM_currFile, 3)], Files_Rider__typ, ch);
}

/*----------------------------------------------------------------------------*/
void OfrontOPM_WriteString (CHAR *s, INTEGER s__len)
{
	INTEGER i;
	i = 0;
	while (s[__X(i, s__len)] != 0x00) {
		i += 1;
	}
	Files_WriteBytes(&OfrontOPM_R[__X(OfrontOPM_currFile, 3)], Files_Rider__typ, (void*)s, s__len * 1, i);
}

/*----------------------------------------------------------------------------*/
void OfrontOPM_WriteStringVar (CHAR *s, INTEGER s__len)
{
	INTEGER i;
	i = 0;
	while (s[__X(i, s__len)] != 0x00) {
		i += 1;
	}
	Files_WriteBytes(&OfrontOPM_R[__X(OfrontOPM_currFile, 3)], Files_Rider__typ, (void*)s, s__len * 1, i);
}

/*----------------------------------------------------------------------------*/
void OfrontOPM_WriteHex (LONGINT i)
{
	CHAR s[3];
	INTEGER digit;
	digit = __ASHR((INTEGER)i, 4, INTEGER);
	if (digit < 10) {
		s[0] = (CHAR)(48 + digit);
	} else {
		s[0] = (CHAR)(87 + digit);
	}
	digit = __MASK((INTEGER)i, -16);
	if (digit < 10) {
		s[1] = (CHAR)(48 + digit);
	} else {
		s[1] = (CHAR)(87 + digit);
	}
	s[2] = 0x00;
	OfrontOPM_WriteString(s, 3);
}

/*----------------------------------------------------------------------------*/
void OfrontOPM_WriteInt (LONGINT i)
{
	CHAR s[20];
	LONGINT i1;
	INTEGER k;
	if (i == (-2147483647-1) || i == (-9223372036854775807-1)) {
		OfrontOPM_Write('(');
		OfrontOPM_WriteInt(i + 1);
		OfrontOPM_WriteString((CHAR*)"-1)", 4);
	} else {
		i1 = __ABS(i);
		s[0] = (CHAR)(__MOD(i1, 10) + 48);
		i1 = __DIV(i1, 10);
		k = 1;
		while (i1 > 0) {
			s[__X(k, 20)] = (CHAR)(__MOD(i1, 10) + 48);
			i1 = __DIV(i1, 10);
			k += 1;
		}
		if (i < 0) {
			s[__X(k, 20)] = '-';
			k += 1;
		}
		while (k > 0) {
			k -= 1;
			OfrontOPM_Write(s[__X(k, 20)]);
		}
	}
}

/*----------------------------------------------------------------------------*/
void OfrontOPM_WriteReal (LONGREAL r, CHAR suffx)
{
	Texts_Writer W;
	Texts_Text T = NIL;
	Texts_Reader R;
	CHAR s[32];
	CHAR ch;
	INTEGER i;
	if ((r < OfrontOPM_MaxLInt && r > OfrontOPM_MinLInt) && r == (__ENTIERL(r))) {
		if (suffx == 'f') {
			OfrontOPM_WriteString((CHAR*)"(REAL)", 7);
		} else {
			OfrontOPM_WriteString((CHAR*)"(LONGREAL)", 11);
		}
		OfrontOPM_WriteInt(__ENTIERL(r));
	} else {
		Texts_OpenWriter(&W, Texts_Writer__typ);
		if (suffx == 'f') {
			Texts_WriteLongReal(&W, Texts_Writer__typ, r, 16);
		} else {
			Texts_WriteLongReal(&W, Texts_Writer__typ, r, 23);
		}
		__NEW(T, Texts_TextDesc);
		Texts_Open(T, (CHAR*)"", 1);
		Texts_Append(T, W.buf);
		Texts_OpenReader(&R, Texts_Reader__typ, T, 0);
		i = 0;
		Texts_Read(&R, Texts_Reader__typ, &ch);
		while (ch != 0x00) {
			s[__X(i, 32)] = ch;
			i += 1;
			Texts_Read(&R, Texts_Reader__typ, &ch);
		}
		s[__X(i, 32)] = 0x00;
		i = 0;
		ch = s[0];
		while (ch != 'D' && ch != 0x00) {
			i += 1;
			ch = s[__X(i, 32)];
		}
		if (ch == 'D') {
			s[__X(i, 32)] = 'e';
		}
		OfrontOPM_WriteString(s, 32);
	}
}

/*----------------------------------------------------------------------------*/
void OfrontOPM_WriteLn (void)
{
	OfrontOPM_WriteString(Platform_newLine, 3);
}

/*----------------------------------------------------------------------------*/
static void OfrontOPM_Append (Files_Rider *R, LONGINT *R__typ, Files_File F)
{
	Files_Rider R1;
	CHAR buffer[4096];
	if (F != NIL) {
		Files_Set(&R1, Files_Rider__typ, F, 0);
		Files_ReadBytes(&R1, Files_Rider__typ, (void*)buffer, 4096, 4096);
		while (4096 - R1.res > 0) {
			Files_WriteBytes(&*R, R__typ, (void*)buffer, 4096, 4096 - R1.res);
			Files_ReadBytes(&R1, Files_Rider__typ, (void*)buffer, 4096, 4096);
		}
	}
}

void OfrontOPM_OpenFiles (CHAR *moduleName, INTEGER moduleName__len)
{
	CHAR FName[32];
	__COPY(moduleName, OfrontOPM_modName, 32);
	OfrontOPM_HFile = Files_New((CHAR*)"", 1);
	if (OfrontOPM_HFile != NIL) {
		Files_Set(&OfrontOPM_R[0], Files_Rider__typ, OfrontOPM_HFile, 0);
	} else {
		OfrontOPM_err(153);
	}
	OfrontOPM_MakeFileName((void*)moduleName, moduleName__len, (void*)FName, 32, (CHAR*)".c", 3);
	OfrontOPM_BFile = Files_New(FName, 32);
	if (OfrontOPM_BFile != NIL) {
		Files_Set(&OfrontOPM_R[1], Files_Rider__typ, OfrontOPM_BFile, 0);
	} else {
		OfrontOPM_err(153);
	}
	OfrontOPM_MakeFileName((void*)moduleName, moduleName__len, (void*)FName, 32, (CHAR*)".h", 3);
	OfrontOPM_HIFile = Files_New(FName, 32);
	if (OfrontOPM_HIFile != NIL) {
		Files_Set(&OfrontOPM_R[2], Files_Rider__typ, OfrontOPM_HIFile, 0);
	} else {
		OfrontOPM_err(153);
	}
	if (__IN(8, OfrontOPM_opt)) {
		OfrontOPM_MakeFileName((void*)moduleName, moduleName__len, (void*)FName, 32, (CHAR*)".h0", 4);
		OfrontOPM_Append(&OfrontOPM_R[2], Files_Rider__typ, Files_Old(FName, 32));
		OfrontOPM_MakeFileName((void*)moduleName, moduleName__len, (void*)FName, 32, (CHAR*)".c0", 4);
		OfrontOPM_Append(&OfrontOPM_R[1], Files_Rider__typ, Files_Old(FName, 32));
	}
}

/*----------------------------------------------------------------------------*/
void OfrontOPM_CloseFiles (void)
{
	CHAR FName[32];
	INTEGER res;
	if (OfrontOPM_noerr) {
		OfrontOPM_LogWStr((CHAR*)"    ", 5);
		OfrontOPM_LogWNum((INTEGER)Files_Pos(&OfrontOPM_R[1], Files_Rider__typ), 0);
	}
	if (OfrontOPM_noerr) {
		if (__STRCMP(OfrontOPM_modName, "SYSTEM") == 0) {
			if (!__IN(10, OfrontOPM_opt)) {
				Files_Register(OfrontOPM_BFile);
			}
		} else if (!__IN(10, OfrontOPM_opt)) {
			OfrontOPM_Append(&OfrontOPM_R[2], Files_Rider__typ, OfrontOPM_HFile);
			Files_Register(OfrontOPM_HIFile);
			Files_Register(OfrontOPM_BFile);
		} else {
			OfrontOPM_MakeFileName((void*)OfrontOPM_modName, 32, (void*)FName, 32, (CHAR*)".h", 3);
			Files_Delete(FName, 32, &res);
			OfrontOPM_MakeFileName((void*)OfrontOPM_modName, 32, (void*)FName, 32, (CHAR*)".sym", 5);
			Files_Delete(FName, 32, &res);
			Files_Register(OfrontOPM_BFile);
		}
	}
	OfrontOPM_HFile = NIL;
	OfrontOPM_BFile = NIL;
	OfrontOPM_HIFile = NIL;
	OfrontOPM_newSFile = NIL;
	OfrontOPM_oldSFile = NIL;
	Files_Set(&OfrontOPM_R[0], Files_Rider__typ, NIL, 0);
	Files_Set(&OfrontOPM_R[1], Files_Rider__typ, NIL, 0);
	Files_Set(&OfrontOPM_R[2], Files_Rider__typ, NIL, 0);
	Files_Set(&OfrontOPM_newSF, Files_Rider__typ, NIL, 0);
	Files_Set(&OfrontOPM_oldSF, Files_Rider__typ, NIL, 0);
}

/*----------------------------------------------------------------------------*/
void OfrontOPM_PromoteIntConstToLInt (void)
{
	if (!__IN(6, OfrontOPM_opt)) {
		OfrontOPM_Write('L');
	}
}

/*----------------------------------------------------------------------------*/
static void EnumPtrs(void (*P)(void*))
{
	__ENUMR(&OfrontOPM_inR, Texts_Reader__typ, 80, 1, P);
	P(OfrontOPM_Log);
	__ENUMR(&OfrontOPM_W, Texts_Writer__typ, 64, 1, P);
	__ENUMR(&OfrontOPM_oldSF, Files_Rider__typ, 32, 1, P);
	__ENUMR(&OfrontOPM_newSF, Files_Rider__typ, 32, 1, P);
	__ENUMR(OfrontOPM_R, Files_Rider__typ, 32, 3, P);
	P(OfrontOPM_oldSFile);
	P(OfrontOPM_newSFile);
	P(OfrontOPM_HFile);
	P(OfrontOPM_BFile);
	P(OfrontOPM_HIFile);
}


export void *OfrontOPM__init(void)
{
	__DEFMOD;
	__IMPORT(Args__init);
	__IMPORT(Console__init);
	__IMPORT(Files__init);
	__IMPORT(OfrontErrors__init);
	__IMPORT(Platform__init);
	__IMPORT(Texts__init);
	__REGMOD("OfrontOPM", EnumPtrs);
	__REGCMD("CloseFiles", OfrontOPM_CloseFiles);
	__REGCMD("CloseOldSym", OfrontOPM_CloseOldSym);
	__REGCMD("DeleteNewSym", OfrontOPM_DeleteNewSym);
	__REGCMD("InitOptions", OfrontOPM_InitOptions);
	__REGCMD("LogWLn", OfrontOPM_LogWLn);
	__REGCMD("OpenPar", OfrontOPM_OpenPar);
	__REGCMD("PromoteIntConstToLInt", OfrontOPM_PromoteIntConstToLInt);
	__REGCMD("RegisterNewSym", OfrontOPM_RegisterNewSym);
	__REGCMD("WriteLn", OfrontOPM_WriteLn);
/* BEGIN */
	Texts_OpenWriter(&OfrontOPM_W, Texts_Writer__typ);
	__MOVE(".", OfrontOPM_OBERON, 2);
	Platform_GetEnv((CHAR*)"OBERON", 7, (void*)OfrontOPM_OBERON, 1024);
	Files_SetSearchPath(OfrontOPM_OBERON, 1024);
	__ENDMOD;
}

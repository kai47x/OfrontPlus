/* Ofront+ 1.0 -eC -88 */

#ifndef OfrontOPM__h
#define OfrontOPM__h

#include "SYSTEM.h"


import CHAR OfrontOPM_Model;
import SHORTINT OfrontOPM_AddressSize, OfrontOPM_Alignment, OfrontOPM_ByteSize, OfrontOPM_CharSize, OfrontOPM_BoolSize, OfrontOPM_SIntSize, OfrontOPM_IntSize, OfrontOPM_LIntSize, OfrontOPM_SetSize, OfrontOPM_RealSize, OfrontOPM_LRealSize, OfrontOPM_PointerSize, OfrontOPM_ProcSize, OfrontOPM_RecSize, OfrontOPM_ArrLenSize, OfrontOPM_CharAlign, OfrontOPM_BoolAlign, OfrontOPM_SIntAlign, OfrontOPM_IntAlign, OfrontOPM_LIntAlign, OfrontOPM_SetAlign, OfrontOPM_RealAlign, OfrontOPM_LRealAlign, OfrontOPM_PointerAlign, OfrontOPM_ProcAlign, OfrontOPM_RecAlign, OfrontOPM_ByteOrder, OfrontOPM_BitOrder, OfrontOPM_MaxSet, OfrontOPM_MaxHDig;
import INTEGER OfrontOPM_MinSInt, OfrontOPM_MinInt, OfrontOPM_MaxSInt, OfrontOPM_MaxInt, OfrontOPM_MaxIndex;
import LONGINT OfrontOPM_MinLInt, OfrontOPM_MaxLInt;
import LONGREAL OfrontOPM_MinReal, OfrontOPM_MaxReal, OfrontOPM_MinLReal, OfrontOPM_MaxLReal;
import BOOLEAN OfrontOPM_noerr;
import INTEGER OfrontOPM_curpos;
import INTEGER OfrontOPM_errpos;
import INTEGER OfrontOPM_breakpc;
import INTEGER OfrontOPM_currFile, OfrontOPM_level;
import INTEGER OfrontOPM_pc, OfrontOPM_entno;
import CHAR OfrontOPM_modName[32];
import CHAR OfrontOPM_objname[64];
import SET OfrontOPM_opt, OfrontOPM_glbopt;


import void OfrontOPM_CloseFiles (void);
import void OfrontOPM_CloseOldSym (void);
import void OfrontOPM_DeleteNewSym (void);
import void OfrontOPM_FPrint (INTEGER *fp, INTEGER val);
import void OfrontOPM_FPrintLReal (INTEGER *fp, LONGREAL lr);
import void OfrontOPM_FPrintReal (INTEGER *fp, REAL real);
import void OfrontOPM_FPrintSet (INTEGER *fp, SET set);
import void OfrontOPM_Get (CHAR *ch);
import void OfrontOPM_Init (CHAR *msg, INTEGER msg__len, BOOLEAN *done);
import void OfrontOPM_InitOptions (void);
import void OfrontOPM_LogW (CHAR ch);
import void OfrontOPM_LogWLn (void);
import void OfrontOPM_LogWNum (INTEGER i, INTEGER len);
import void OfrontOPM_LogWStr (CHAR *s, INTEGER s__len);
import void OfrontOPM_Mark (SHORTINT n, INTEGER pos);
import void OfrontOPM_NewSym (CHAR *modName, INTEGER modName__len);
import void OfrontOPM_OldSym (CHAR *modName, INTEGER modName__len, BOOLEAN *done);
import void OfrontOPM_OpenFiles (CHAR *moduleName, INTEGER moduleName__len);
import void OfrontOPM_OpenPar (CHAR *title, INTEGER title__len, CHAR *cmd, INTEGER cmd__len);
import void OfrontOPM_PromoteIntConstToLInt (void);
import void OfrontOPM_RegisterNewSym (void);
import void OfrontOPM_SymRCh (CHAR *ch);
import LONGINT OfrontOPM_SymRInt (void);
import void OfrontOPM_SymRLReal (LONGREAL *lr);
import void OfrontOPM_SymRReal (REAL *r);
import void OfrontOPM_SymRSet (SET *s);
import void OfrontOPM_SymWCh (CHAR ch);
import void OfrontOPM_SymWInt (LONGINT i);
import void OfrontOPM_SymWLReal (LONGREAL lr);
import void OfrontOPM_SymWReal (REAL r);
import void OfrontOPM_SymWSet (SET s);
import void OfrontOPM_Write (CHAR ch);
import void OfrontOPM_WriteHex (INTEGER i);
import void OfrontOPM_WriteInt (LONGINT i);
import void OfrontOPM_WriteLn (void);
import void OfrontOPM_WriteReal (LONGREAL r, CHAR suffx);
import void OfrontOPM_WriteString (CHAR *s, INTEGER s__len);
import void OfrontOPM_WriteStringVar (CHAR *s, INTEGER s__len);
import BOOLEAN OfrontOPM_eofSF (void);
import void OfrontOPM_err (SHORTINT n);
import void *OfrontOPM__init(void);


#endif

/* Ofront+ 1.0 -s */

#ifndef Texts__h
#define Texts__h

#include "SYSTEM.h"
#include "Files.h"

typedef
	struct Texts_BufDesc {
		INTEGER len;
		char _prvt0[4];
	} Texts_BufDesc;

typedef
	Texts_BufDesc *Texts_Buffer;

typedef
	struct Texts_ElemMsg {
		char _prvt0[1];
	} Texts_ElemMsg;

typedef
	struct Texts_ElemDesc *Texts_Elem;

typedef
	struct Texts_CopyMsg { /* Texts_ElemMsg */
		Texts_Elem e;
	} Texts_CopyMsg;

typedef
	struct Texts_RunDesc {
		INTEGER _prvt0;
		char _prvt1[15];
	} Texts_RunDesc;

typedef
	void (*Texts_Handler)(Texts_Elem, Texts_ElemMsg*, SYSTEM_ADRINT *);

typedef
	struct Texts_ElemDesc {
		char _prvt0[20];
		INTEGER W, H;
		Texts_Handler handle;
		char _prvt1[4];
	} Texts_ElemDesc;

typedef
	struct Texts_FileMsg { /* Texts_ElemMsg */
		SHORTINT id;
		INTEGER pos;
		Files_Rider r;
	} Texts_FileMsg;

typedef
	struct Texts_FontDesc {
		char _prvt0[32];
	} Texts_FontDesc;

typedef
	Texts_FontDesc *Texts_FontsFont;

typedef
	struct Texts_IdentifyMsg { /* Texts_ElemMsg */
		CHAR mod[32], proc[32];
	} Texts_IdentifyMsg;

typedef
	struct Texts_TextDesc *Texts_Text;

typedef
	void (*Texts_Notifier)(Texts_Text, SHORTINT, INTEGER, INTEGER);

typedef
	struct Texts_Reader {
		BOOLEAN eot;
		Texts_FontsFont fnt;
		BYTE col, voff;
		Texts_Elem elem;
		char _prvt0[36];
	} Texts_Reader;

typedef
	struct Texts_Scanner { /* Texts_Reader */
		BOOLEAN eot;
		Texts_FontsFont fnt;
		BYTE col, voff;
		Texts_Elem elem;
		char _prvt0[36];
		CHAR nextCh;
		SHORTINT line, class;
		INTEGER i;
		REAL x;
		LONGREAL y;
		CHAR c;
		BYTE len;
		CHAR s[64];
	} Texts_Scanner;

typedef
	struct Texts_TextDesc {
		INTEGER len;
		Texts_Notifier notify;
		char _prvt0[12];
	} Texts_TextDesc;

typedef
	struct Texts_Writer {
		Texts_Buffer buf;
		Texts_FontsFont fnt;
		BYTE col, voff;
		char _prvt0[30];
	} Texts_Writer;


import Texts_Elem Texts_new;

import SYSTEM_ADRINT *Texts_FontDesc__typ;
import SYSTEM_ADRINT *Texts_RunDesc__typ;
import SYSTEM_ADRINT *Texts_ElemMsg__typ;
import SYSTEM_ADRINT *Texts_ElemDesc__typ;
import SYSTEM_ADRINT *Texts_FileMsg__typ;
import SYSTEM_ADRINT *Texts_CopyMsg__typ;
import SYSTEM_ADRINT *Texts_IdentifyMsg__typ;
import SYSTEM_ADRINT *Texts_BufDesc__typ;
import SYSTEM_ADRINT *Texts_TextDesc__typ;
import SYSTEM_ADRINT *Texts_Reader__typ;
import SYSTEM_ADRINT *Texts_Scanner__typ;
import SYSTEM_ADRINT *Texts_Writer__typ;

import void Texts_Append (Texts_Text T, Texts_Buffer B);
import void Texts_ChangeLooks (Texts_Text T, INTEGER beg, INTEGER end, SET sel, Texts_FontsFont fnt, BYTE col, BYTE voff);
import void Texts_Close (Texts_Text T, CHAR *name, INTEGER name__len);
import void Texts_Copy (Texts_Buffer SB, Texts_Buffer DB);
import void Texts_CopyElem (Texts_Elem SE, Texts_Elem DE);
import void Texts_Delete (Texts_Text T, INTEGER beg, INTEGER end);
import Texts_Text Texts_ElemBase (Texts_Elem E);
import INTEGER Texts_ElemPos (Texts_Elem E);
import void Texts_Insert (Texts_Text T, INTEGER pos, Texts_Buffer B);
import void Texts_Load (Files_Rider *r, SYSTEM_ADRINT *r__typ, Texts_Text T);
import void Texts_Open (Texts_Text T, CHAR *name, INTEGER name__len);
import void Texts_OpenBuf (Texts_Buffer B);
import void Texts_OpenReader (Texts_Reader *R, SYSTEM_ADRINT *R__typ, Texts_Text T, INTEGER pos);
import void Texts_OpenScanner (Texts_Scanner *S, SYSTEM_ADRINT *S__typ, Texts_Text T, INTEGER pos);
import void Texts_OpenWriter (Texts_Writer *W, SYSTEM_ADRINT *W__typ);
import INTEGER Texts_Pos (Texts_Reader *R, SYSTEM_ADRINT *R__typ);
import void Texts_Read (Texts_Reader *R, SYSTEM_ADRINT *R__typ, CHAR *ch);
import void Texts_ReadElem (Texts_Reader *R, SYSTEM_ADRINT *R__typ);
import void Texts_ReadPrevElem (Texts_Reader *R, SYSTEM_ADRINT *R__typ);
import void Texts_Recall (Texts_Buffer *B);
import void Texts_Save (Texts_Text T, INTEGER beg, INTEGER end, Texts_Buffer B);
import void Texts_Scan (Texts_Scanner *S, SYSTEM_ADRINT *S__typ);
import void Texts_SetColor (Texts_Writer *W, SYSTEM_ADRINT *W__typ, BYTE col);
import void Texts_SetFont (Texts_Writer *W, SYSTEM_ADRINT *W__typ, Texts_FontsFont fnt);
import void Texts_SetOffset (Texts_Writer *W, SYSTEM_ADRINT *W__typ, BYTE voff);
import void Texts_Store (Files_Rider *r, SYSTEM_ADRINT *r__typ, Texts_Text T);
import void Texts_Write (Texts_Writer *W, SYSTEM_ADRINT *W__typ, CHAR ch);
import void Texts_WriteDate (Texts_Writer *W, SYSTEM_ADRINT *W__typ, INTEGER t, INTEGER d);
import void Texts_WriteElem (Texts_Writer *W, SYSTEM_ADRINT *W__typ, Texts_Elem e);
import void Texts_WriteHex (Texts_Writer *W, SYSTEM_ADRINT *W__typ, INTEGER x);
import void Texts_WriteInt (Texts_Writer *W, SYSTEM_ADRINT *W__typ, INTEGER x, INTEGER n);
import void Texts_WriteLn (Texts_Writer *W, SYSTEM_ADRINT *W__typ);
import void Texts_WriteLongHex (Texts_Writer *W, SYSTEM_ADRINT *W__typ, LONGINT x);
import void Texts_WriteLongInt (Texts_Writer *W, SYSTEM_ADRINT *W__typ, LONGINT x, INTEGER n);
import void Texts_WriteLongReal (Texts_Writer *W, SYSTEM_ADRINT *W__typ, LONGREAL x, SHORTINT n);
import void Texts_WriteLongRealHex (Texts_Writer *W, SYSTEM_ADRINT *W__typ, LONGREAL x);
import void Texts_WriteReal (Texts_Writer *W, SYSTEM_ADRINT *W__typ, REAL x, SHORTINT n);
import void Texts_WriteRealFix (Texts_Writer *W, SYSTEM_ADRINT *W__typ, REAL x, SHORTINT n, SHORTINT k);
import void Texts_WriteRealHex (Texts_Writer *W, SYSTEM_ADRINT *W__typ, REAL x);
import void Texts_WriteString (Texts_Writer *W, SYSTEM_ADRINT *W__typ, CHAR *s, INTEGER s__len);
import void *Texts__init(void);


#endif

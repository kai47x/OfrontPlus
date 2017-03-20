/* Ofront+ 0.9 -e */
#include "SYSTEM.h"
#include "OfrontOPM.h"
#include "OfrontOPT.h"


static INTEGER OfrontOPC_indentLevel;
static BOOLEAN OfrontOPC_ptrinit, OfrontOPC_mainprog, OfrontOPC_ansi;
static SHORTINT OfrontOPC_hashtab[105];
static CHAR OfrontOPC_keytab[36][9];
static BOOLEAN OfrontOPC_GlbPtrs;
static CHAR OfrontOPC_BodyNameExt[13];


export void OfrontOPC_Align (LONGINT *adr, LONGINT base);
export void OfrontOPC_Andent (OfrontOPT_Struct typ);
static void OfrontOPC_AnsiParamList (OfrontOPT_Object obj, BOOLEAN showParamNames);
export LONGINT OfrontOPC_Base (OfrontOPT_Struct typ);
export OfrontOPT_Object OfrontOPC_BaseTProc (OfrontOPT_Object obj);
export void OfrontOPC_BegBlk (void);
export void OfrontOPC_BegStat (void);
static void OfrontOPC_CProcDefs (OfrontOPT_Object obj, INTEGER vis);
export void OfrontOPC_Case (LONGINT caseVal, INTEGER form);
export void OfrontOPC_Cmp (INTEGER rel);
export void OfrontOPC_CompleteIdent (OfrontOPT_Object obj);
export void OfrontOPC_Constant (OfrontOPT_Const con, INTEGER form);
static void OfrontOPC_DeclareBase (OfrontOPT_Object dcl);
static void OfrontOPC_DeclareObj (OfrontOPT_Object dcl, BOOLEAN scopeDef);
static void OfrontOPC_DeclareParams (OfrontOPT_Object par, BOOLEAN macro);
static void OfrontOPC_DeclareTProcs (OfrontOPT_Object obj, BOOLEAN *empty);
static void OfrontOPC_DefAnonRecs (OfrontOPT_Node n);
export void OfrontOPC_DefineInter (OfrontOPT_Object proc);
static void OfrontOPC_DefineTProcMacros (OfrontOPT_Object obj, BOOLEAN *empty);
static void OfrontOPC_DefineTProcTypes (OfrontOPT_Object obj);
static void OfrontOPC_DefineType (OfrontOPT_Struct str);
export void OfrontOPC_EndBlk (void);
export void OfrontOPC_EndBlk0 (void);
export void OfrontOPC_EndStat (void);
export void OfrontOPC_EnterBody (void);
export void OfrontOPC_EnterProc (OfrontOPT_Object proc);
export void OfrontOPC_ExitBody (void);
export void OfrontOPC_ExitProc (OfrontOPT_Object proc, BOOLEAN eoBlock, BOOLEAN implicitRet);
static void OfrontOPC_FieldList (OfrontOPT_Struct typ, BOOLEAN last, LONGINT *off, LONGINT *n, LONGINT *curAlign);
static void OfrontOPC_FillGap (LONGINT gap, LONGINT off, LONGINT align, LONGINT *n, LONGINT *curAlign);
export void OfrontOPC_GenBdy (OfrontOPT_Node n);
static void OfrontOPC_GenDynTypes (OfrontOPT_Node n, INTEGER vis);
export void OfrontOPC_GenEnumPtrs (OfrontOPT_Object var);
export void OfrontOPC_GenHdr (OfrontOPT_Node n);
export void OfrontOPC_GenHdrIncludes (void);
static void OfrontOPC_GenHeaderMsg (void);
export void OfrontOPC_Halt (LONGINT n);
export void OfrontOPC_Ident (OfrontOPT_Object obj);
static void OfrontOPC_IdentList (OfrontOPT_Object obj, INTEGER vis);
static void OfrontOPC_Include (CHAR *name, INTEGER name__len);
static void OfrontOPC_IncludeImports (OfrontOPT_Object obj, INTEGER vis);
export void OfrontOPC_Increment (BOOLEAN decrement);
export void OfrontOPC_Indent (INTEGER count);
export void OfrontOPC_Init (void);
static void OfrontOPC_InitImports (OfrontOPT_Object obj);
static void OfrontOPC_InitKeywords (void);
export void OfrontOPC_InitTDesc (OfrontOPT_Struct typ);
static void OfrontOPC_InitTProcs (OfrontOPT_Object typ, OfrontOPT_Object obj);
export void OfrontOPC_Len (OfrontOPT_Object obj, OfrontOPT_Struct array, LONGINT dim);
static void OfrontOPC_LenList (OfrontOPT_Object par, BOOLEAN ansiDefine, BOOLEAN showParamName);
static INTEGER OfrontOPC_Length (CHAR *s, INTEGER s__len);
export BOOLEAN OfrontOPC_NeedsRetval (OfrontOPT_Object proc);
export LONGINT OfrontOPC_NofPtrs (OfrontOPT_Struct typ);
static INTEGER OfrontOPC_PerfectHash (CHAR *s, INTEGER s__len);
static BOOLEAN OfrontOPC_Prefixed (OfrontOPT_ConstExt x, CHAR *y, INTEGER y__len);
static void OfrontOPC_ProcHeader (OfrontOPT_Object proc, BOOLEAN define);
static void OfrontOPC_ProcPredefs (OfrontOPT_Object obj, SHORTINT vis);
static void OfrontOPC_PutBase (OfrontOPT_Struct typ);
static void OfrontOPC_PutPtrOffsets (OfrontOPT_Struct typ, LONGINT adr, LONGINT *cnt);
static void OfrontOPC_RegCmds (OfrontOPT_Object obj);
export void OfrontOPC_SetInclude (BOOLEAN exclude);
static void OfrontOPC_Stars (OfrontOPT_Struct typ, BOOLEAN *openClause);
static void OfrontOPC_Str1 (CHAR *s, INTEGER s__len, LONGINT x);
export void OfrontOPC_TDescDecl (OfrontOPT_Struct typ);
export void OfrontOPC_TypeDefs (OfrontOPT_Object obj, INTEGER vis);
export void OfrontOPC_TypeOf (OfrontOPT_Object ap);
static BOOLEAN OfrontOPC_Undefined (OfrontOPT_Object obj);


/*============================================================================*/

void OfrontOPC_Init (void)
{
	OfrontOPC_indentLevel = 0;
	OfrontOPC_ptrinit = __IN(5, OfrontOPM_opt);
	OfrontOPC_mainprog = __IN(10, OfrontOPM_opt);
	OfrontOPC_ansi = __IN(6, OfrontOPM_opt);
	if (OfrontOPC_ansi) {
		__MOVE("__init(void)", OfrontOPC_BodyNameExt, 13);
	} else {
		__MOVE("__init()", OfrontOPC_BodyNameExt, 9);
	}
}

/*----------------------------------------------------------------------------*/
void OfrontOPC_Indent (INTEGER count)
{
	OfrontOPC_indentLevel += count;
}

/*----------------------------------------------------------------------------*/
void OfrontOPC_BegStat (void)
{
	INTEGER i;
	i = OfrontOPC_indentLevel;
	while (i > 0) {
		OfrontOPM_Write(0x09);
		i -= 1;
	}
}

/*----------------------------------------------------------------------------*/
void OfrontOPC_EndStat (void)
{
	OfrontOPM_Write(';');
	OfrontOPM_WriteLn();
}

/*----------------------------------------------------------------------------*/
void OfrontOPC_BegBlk (void)
{
	OfrontOPM_Write('{');
	OfrontOPM_WriteLn();
	OfrontOPC_indentLevel += 1;
}

/*----------------------------------------------------------------------------*/
void OfrontOPC_EndBlk (void)
{
	OfrontOPC_indentLevel -= 1;
	OfrontOPC_BegStat();
	OfrontOPM_Write('}');
	OfrontOPM_WriteLn();
}

/*----------------------------------------------------------------------------*/
void OfrontOPC_EndBlk0 (void)
{
	OfrontOPC_indentLevel -= 1;
	OfrontOPC_BegStat();
	OfrontOPM_Write('}');
}

/*----------------------------------------------------------------------------*/
static void OfrontOPC_Str1 (CHAR *s, INTEGER s__len, LONGINT x)
{
	CHAR ch;
	INTEGER i;
	__DUP(s, s__len);
	ch = s[0];
	i = 0;
	while (ch != 0x00) {
		if (ch == '#') {
			OfrontOPM_WriteInt(x);
		} else {
			OfrontOPM_Write(ch);
		}
		i += 1;
		ch = s[__X(i, s__len)];
	}
	__DEL(s);
}

static INTEGER OfrontOPC_Length (CHAR *s, INTEGER s__len)
{
	INTEGER i;
	i = 0;
	while (s[__X(i, s__len)] != 0x00) {
		i += 1;
	}
	return i;
}

static INTEGER OfrontOPC_PerfectHash (CHAR *s, INTEGER s__len)
{
	INTEGER i, h;
	i = 0;
	h = 0;
	while (s[__X(i, s__len)] != 0x00 && i < 5) {
		h = 3 * h + (INTEGER)s[__X(i, s__len)];
		i += 1;
	}
	return (INTEGER)__MOD(h, 105);
}

void OfrontOPC_Ident (OfrontOPT_Object obj)
{
	INTEGER mode, level, h;
	mode = obj->mode;
	level = obj->mnolev;
	if (__IN(mode, 0x62) && level > 0 || __IN(mode, 0x14)) {
		OfrontOPM_WriteStringVar((void*)obj->name, 32);
		h = OfrontOPC_PerfectHash((void*)obj->name, 32);
		if (OfrontOPC_hashtab[__X(h, 105)] >= 0) {
			if (__STRCMP(OfrontOPC_keytab[__X(OfrontOPC_hashtab[__X(h, 105)], 36)], obj->name) == 0) {
				OfrontOPM_Write('_');
			}
		}
	} else {
		if (mode != 5 || obj->linkadr != 2) {
			if (mode == 13) {
				OfrontOPC_Ident(obj->link->typ->strobj);
			} else if (level < 0) {
				OfrontOPM_WriteStringVar((void*)OfrontOPT_GlbMod[__X(-level, 64)]->name, 32);
				if (OfrontOPM_currFile == 0) {
					OfrontOPT_GlbMod[__X(-level, 64)]->vis = 1;
				}
			} else {
				OfrontOPM_WriteString(OfrontOPM_modName, 32);
			}
			OfrontOPM_Write('_');
		} else if (obj == OfrontOPT_sysptrtyp->strobj) {
			OfrontOPM_WriteString((CHAR*)"SYSTEM_", 8);
		}
		OfrontOPM_WriteStringVar((void*)obj->name, 32);
	}
}

/*----------------------------------------------------------------------------*/
static void OfrontOPC_Stars (OfrontOPT_Struct typ, BOOLEAN *openClause)
{
	INTEGER pointers;
	*openClause = 0;
	if ((typ->strobj == NIL || typ->strobj->name[0] == 0x00) && typ->comp != 4) {
		if (__IN(typ->comp, 0x0c)) {
			OfrontOPC_Stars(typ->BaseTyp, &*openClause);
			*openClause = typ->comp == 2;
		} else if (typ->form == 14) {
			OfrontOPM_Write('(');
			OfrontOPM_Write('*');
		} else {
			pointers = 0;
			while ((typ->strobj == NIL || typ->strobj->name[0] == 0x00) && typ->form == 13) {
				pointers += 1;
				typ = typ->BaseTyp;
			}
			if (pointers > 0) {
				if (typ->comp != 3) {
					OfrontOPC_Stars(typ, &*openClause);
				}
				if (*openClause) {
					OfrontOPM_Write('(');
					*openClause = 0;
				}
				while (pointers > 0) {
					OfrontOPM_Write('*');
					pointers -= 1;
				}
			}
		}
	}
}

static void OfrontOPC_DeclareObj (OfrontOPT_Object dcl, BOOLEAN scopeDef)
{
	OfrontOPT_Struct typ = NIL;
	BOOLEAN varPar, openClause;
	INTEGER form, comp;
	typ = dcl->typ;
	varPar = (dcl->mode == 2 && typ->comp != 2 || typ->comp == 3) || scopeDef;
	OfrontOPC_Stars(typ, &openClause);
	if (varPar) {
		if (openClause) {
			OfrontOPM_Write('(');
		}
		OfrontOPM_Write('*');
	}
	if (dcl->name[0] != 0x00) {
		OfrontOPC_Ident(dcl);
	}
	if (varPar && openClause) {
		OfrontOPM_Write(')');
	}
	openClause = 0;
	for (;;) {
		form = typ->form;
		comp = typ->comp;
		if ((typ->strobj != NIL && typ->strobj->name[0] != 0x00 || form == 12) || comp == 4) {
			break;
		} else if (form == 13 && typ->BaseTyp->comp != 3) {
			openClause = 1;
		} else if (form == 14 || __IN(comp, 0x0c)) {
			if (openClause) {
				OfrontOPM_Write(')');
				openClause = 0;
			}
			if (form == 14) {
				if (OfrontOPC_ansi) {
					OfrontOPM_Write(')');
					OfrontOPC_AnsiParamList(typ->link, 0);
				} else {
					OfrontOPM_WriteString((CHAR*)")()", 4);
				}
				break;
			} else if (comp == 2) {
				OfrontOPM_Write('[');
				OfrontOPM_WriteInt(typ->n);
				OfrontOPM_Write(']');
			}
		} else {
			break;
		}
		typ = typ->BaseTyp;
	}
}

void OfrontOPC_Andent (OfrontOPT_Struct typ)
{
	if (typ->strobj == NIL || typ->align >= 65536) {
		OfrontOPM_WriteString(OfrontOPM_modName, 32);
		OfrontOPC_Str1((CHAR*)"__#", 4, __ASHR(typ->align, 16, LONGINT));
	} else {
		OfrontOPC_Ident(typ->strobj);
	}
}

/*----------------------------------------------------------------------------*/
static BOOLEAN OfrontOPC_Undefined (OfrontOPT_Object obj)
{
	return (obj->mnolev >= 0 && obj->linkadr != (LONGINT)(3 + OfrontOPM_currFile)) && obj->linkadr != 2 || obj->name[0] == 0x00;
}

static void OfrontOPC_DeclareBase (OfrontOPT_Object dcl)
{
	OfrontOPT_Struct typ = NIL, prev = NIL;
	OfrontOPT_Object obj = NIL;
	INTEGER nofdims;
	LONGINT off, n, dummy;
	typ = dcl->typ;
	prev = typ;
	while (((((typ->strobj == NIL || typ->comp == 3) || OfrontOPC_Undefined(typ->strobj)) && typ->comp != 4) && typ->form != 12) && !(typ->form == 13 && typ->BaseTyp->comp == 3)) {
		prev = typ;
		typ = typ->BaseTyp;
	}
	obj = typ->strobj;
	if (typ->form == 12) {
		OfrontOPM_WriteString((CHAR*)"void", 5);
	} else if (obj != NIL && !OfrontOPC_Undefined(obj)) {
		OfrontOPC_Ident(obj);
	} else if (typ->comp == 4) {
		OfrontOPM_WriteString((CHAR*)"struct ", 8);
		OfrontOPC_Andent(typ);
		if (prev->form != 13 && (obj != NIL || dcl->name[0] == 0x00)) {
			if (typ->BaseTyp != NIL && typ->BaseTyp->strobj->vis != 0) {
				OfrontOPM_WriteString((CHAR*)" { /* ", 7);
				OfrontOPC_Ident(typ->BaseTyp->strobj);
				OfrontOPM_WriteString((CHAR*)" */", 4);
				OfrontOPM_WriteLn();
				OfrontOPC_Indent(1);
			} else {
				OfrontOPM_Write(' ');
				OfrontOPC_BegBlk();
			}
			OfrontOPC_FieldList(typ, 1, &off, &n, &dummy);
			OfrontOPC_EndBlk0();
		}
	} else if (typ->form == 13 && typ->BaseTyp->comp == 3) {
		typ = typ->BaseTyp->BaseTyp;
		nofdims = 1;
		while (typ->comp == 3) {
			nofdims += 1;
			typ = typ->BaseTyp;
		}
		OfrontOPM_WriteString((CHAR*)"struct ", 8);
		OfrontOPC_BegBlk();
		OfrontOPC_BegStat();
		if (OfrontOPM_ArrLenSize == OfrontOPM_SIntSize) {
			OfrontOPC_Str1((CHAR*)"SHORTINT len[#]", 16, nofdims);
		} else if (OfrontOPM_ArrLenSize == OfrontOPM_IntSize) {
			OfrontOPC_Str1((CHAR*)"INTEGER len[#]", 15, nofdims);
		} else {
			OfrontOPC_Str1((CHAR*)"LONGINT len[#]", 15, nofdims);
		}
		OfrontOPC_EndStat();
		OfrontOPC_BegStat();
		__NEW(obj, OfrontOPT_ObjDesc);
		__NEW(obj->typ, OfrontOPT_StrDesc);
		obj->typ->form = 15;
		obj->typ->comp = 2;
		obj->typ->n = 1;
		obj->typ->BaseTyp = typ;
		obj->mode = 4;
		__MOVE("data", obj->name, 5);
		obj->linkadr = 0;
		OfrontOPC_DeclareBase(obj);
		OfrontOPM_Write(' ');
		OfrontOPC_DeclareObj(obj, 0);
		OfrontOPC_EndStat();
		OfrontOPC_EndBlk0();
	}
}

LONGINT OfrontOPC_NofPtrs (OfrontOPT_Struct typ)
{
	OfrontOPT_Object fld = NIL;
	OfrontOPT_Struct btyp = NIL;
	LONGINT n;
	if (typ->form == 13 && typ->sysflag == 0) {
		return 1;
	} else if (typ->comp == 4 && __MASK(typ->sysflag, -256) == 0) {
		btyp = typ->BaseTyp;
		if (btyp != NIL) {
			n = OfrontOPC_NofPtrs(btyp);
		} else {
			n = 0;
		}
		fld = typ->link;
		while (fld != NIL && fld->mode == 4) {
			if (__STRCMP(fld->name, "@ptr") != 0) {
				n = n + OfrontOPC_NofPtrs(fld->typ);
			} else {
				n += 1;
			}
			fld = fld->link;
		}
		return n;
	} else if (typ->comp == 2) {
		btyp = typ->BaseTyp;
		n = typ->n;
		while (btyp->comp == 2) {
			n = btyp->n * n;
			btyp = btyp->BaseTyp;
		}
		return OfrontOPC_NofPtrs(btyp) * n;
	} else {
		return 0;
	}
	__RETCHK;
}

/*----------------------------------------------------------------------------*/
static void OfrontOPC_PutPtrOffsets (OfrontOPT_Struct typ, LONGINT adr, LONGINT *cnt)
{
	OfrontOPT_Object fld = NIL;
	OfrontOPT_Struct btyp = NIL;
	LONGINT n, i;
	if (typ->form == 13 && typ->sysflag == 0) {
		OfrontOPM_WriteInt(adr);
		OfrontOPM_WriteString((CHAR*)", ", 3);
		*cnt += 1;
		if (__MASK(*cnt, -16) == 0) {
			OfrontOPM_WriteLn();
			OfrontOPM_Write(0x09);
		}
	} else if (typ->comp == 4 && __MASK(typ->sysflag, -256) == 0) {
		btyp = typ->BaseTyp;
		if (btyp != NIL) {
			OfrontOPC_PutPtrOffsets(btyp, adr, &*cnt);
		}
		fld = typ->link;
		while (fld != NIL && fld->mode == 4) {
			if (__STRCMP(fld->name, "@ptr") != 0) {
				OfrontOPC_PutPtrOffsets(fld->typ, adr + fld->adr, &*cnt);
			} else {
				OfrontOPM_WriteInt(adr + fld->adr);
				OfrontOPM_WriteString((CHAR*)", ", 3);
				*cnt += 1;
				if (__MASK(*cnt, -16) == 0) {
					OfrontOPM_WriteLn();
					OfrontOPM_Write(0x09);
				}
			}
			fld = fld->link;
		}
	} else if (typ->comp == 2) {
		btyp = typ->BaseTyp;
		n = typ->n;
		while (btyp->comp == 2) {
			n = btyp->n * n;
			btyp = btyp->BaseTyp;
		}
		if (OfrontOPC_NofPtrs(btyp) > 0) {
			i = 0;
			while (i < n) {
				OfrontOPC_PutPtrOffsets(btyp, adr + i * btyp->size, &*cnt);
				i += 1;
			}
		}
	}
}

static void OfrontOPC_InitTProcs (OfrontOPT_Object typ, OfrontOPT_Object obj)
{
	if (obj != NIL) {
		OfrontOPC_InitTProcs(typ, obj->left);
		if (obj->mode == 13) {
			OfrontOPC_BegStat();
			OfrontOPM_WriteString((CHAR*)"__INITBP(", 10);
			OfrontOPC_Ident(typ);
			OfrontOPM_WriteString((CHAR*)", ", 3);
			OfrontOPC_Ident(obj);
			OfrontOPC_Str1((CHAR*)", #)", 5, __ASHR(obj->adr, 16, LONGINT));
			OfrontOPC_EndStat();
		}
		OfrontOPC_InitTProcs(typ, obj->right);
	}
}

static void OfrontOPC_PutBase (OfrontOPT_Struct typ)
{
	if (typ != NIL) {
		OfrontOPC_PutBase(typ->BaseTyp);
		OfrontOPC_Ident(typ->strobj);
		OfrontOPM_WriteString((CHAR*)"__typ", 6);
		OfrontOPM_WriteString((CHAR*)", ", 3);
	}
}

static void OfrontOPC_LenList (OfrontOPT_Object par, BOOLEAN ansiDefine, BOOLEAN showParamName)
{
	OfrontOPT_Struct typ = NIL;
	INTEGER dim;
	if (showParamName) {
		OfrontOPC_Ident(par);
		OfrontOPM_WriteString((CHAR*)"__len", 6);
	}
	dim = 1;
	typ = par->typ->BaseTyp;
	while (typ->comp == 3) {
		if (ansiDefine) {
			if (OfrontOPM_ArrLenSize == OfrontOPM_SIntSize) {
				OfrontOPM_WriteString((CHAR*)", SHORTINT ", 12);
			} else if (OfrontOPM_ArrLenSize == OfrontOPM_IntSize) {
				OfrontOPM_WriteString((CHAR*)", INTEGER ", 11);
			} else {
				OfrontOPM_WriteString((CHAR*)", LONGINT ", 11);
			}
		} else {
			OfrontOPM_WriteString((CHAR*)", ", 3);
		}
		if (showParamName) {
			OfrontOPC_Ident(par);
			OfrontOPM_WriteString((CHAR*)"__len", 6);
			OfrontOPM_WriteInt(dim);
		}
		typ = typ->BaseTyp;
		dim += 1;
	}
}

static void OfrontOPC_DeclareParams (OfrontOPT_Object par, BOOLEAN macro)
{
	OfrontOPM_Write('(');
	while (par != NIL) {
		if (macro) {
			OfrontOPM_WriteStringVar((void*)par->name, 32);
		} else {
			if (par->mode == 1 && par->typ->form == 7) {
				OfrontOPM_Write('_');
			}
			OfrontOPC_Ident(par);
		}
		if (par->typ->comp == 3) {
			OfrontOPM_WriteString((CHAR*)", ", 3);
			OfrontOPC_LenList(par, 0, 1);
		} else if (par->mode == 2 && par->typ->comp == 4) {
			OfrontOPM_WriteString((CHAR*)", ", 3);
			OfrontOPM_WriteStringVar((void*)par->name, 32);
			OfrontOPM_WriteString((CHAR*)"__typ", 6);
		}
		par = par->link;
		if (par != NIL) {
			OfrontOPM_WriteString((CHAR*)", ", 3);
		}
	}
	OfrontOPM_Write(')');
}

static void OfrontOPC_DefineTProcTypes (OfrontOPT_Object obj)
{
	OfrontOPT_Object par = NIL;
	if (obj->typ != OfrontOPT_notyp) {
		OfrontOPC_DefineType(obj->typ);
	}
	if (OfrontOPC_ansi) {
		par = obj->link;
		while (par != NIL) {
			OfrontOPC_DefineType(par->typ);
			par = par->link;
		}
	}
}

static void OfrontOPC_DeclareTProcs (OfrontOPT_Object obj, BOOLEAN *empty)
{
	if (obj != NIL) {
		OfrontOPC_DeclareTProcs(obj->left, &*empty);
		if (obj->mode == 13) {
			if (obj->typ != OfrontOPT_notyp) {
				OfrontOPC_DefineType(obj->typ);
			}
			if (OfrontOPM_currFile == 0) {
				if (obj->vis == 1) {
					OfrontOPC_DefineTProcTypes(obj);
					OfrontOPM_WriteString((CHAR*)"import ", 8);
					*empty = 0;
					OfrontOPC_ProcHeader(obj, 0);
				}
			} else {
				*empty = 0;
				OfrontOPC_DefineTProcTypes(obj);
				if (obj->vis == 0) {
					OfrontOPM_WriteString((CHAR*)"static ", 8);
				} else {
					OfrontOPM_WriteString((CHAR*)"export ", 8);
				}
				OfrontOPC_ProcHeader(obj, 0);
			}
		}
		OfrontOPC_DeclareTProcs(obj->right, &*empty);
	}
}

OfrontOPT_Object OfrontOPC_BaseTProc (OfrontOPT_Object obj)
{
	OfrontOPT_Struct typ = NIL, base = NIL;
	LONGINT mno;
	typ = obj->link->typ;
	if (typ->form == 13) {
		typ = typ->BaseTyp;
	}
	base = typ->BaseTyp;
	mno = __ASHR(obj->adr, 16, LONGINT);
	while (base != NIL && mno < base->n) {
		typ = base;
		base = typ->BaseTyp;
	}
	OfrontOPT_FindField(obj->name, typ, &obj);
	return obj;
}

/*----------------------------------------------------------------------------*/
static void OfrontOPC_DefineTProcMacros (OfrontOPT_Object obj, BOOLEAN *empty)
{
	if (obj != NIL) {
		OfrontOPC_DefineTProcMacros(obj->left, &*empty);
		if ((obj->mode == 13 && obj == OfrontOPC_BaseTProc(obj)) && (OfrontOPM_currFile != 0 || obj->vis == 1)) {
			OfrontOPM_WriteString((CHAR*)"#define __", 11);
			OfrontOPC_Ident(obj);
			OfrontOPC_DeclareParams(obj->link, 1);
			OfrontOPM_WriteString((CHAR*)" __SEND(", 9);
			if (obj->link->typ->form == 13) {
				OfrontOPM_WriteString((CHAR*)"__TYPEOF(", 10);
				OfrontOPC_Ident(obj->link);
				OfrontOPM_Write(')');
			} else {
				OfrontOPC_Ident(obj->link);
				OfrontOPM_WriteString((CHAR*)"__typ", 6);
			}
			OfrontOPM_WriteString((CHAR*)", ", 3);
			OfrontOPC_Ident(obj);
			OfrontOPC_Str1((CHAR*)", #, ", 6, __ASHR(obj->adr, 16, LONGINT));
			if (obj->typ == OfrontOPT_notyp) {
				OfrontOPM_WriteString((CHAR*)"void", 5);
			} else {
				OfrontOPC_Ident(obj->typ->strobj);
			}
			OfrontOPM_WriteString((CHAR*)"(*)", 4);
			if (OfrontOPC_ansi) {
				OfrontOPC_AnsiParamList(obj->link, 0);
			} else {
				OfrontOPM_WriteString((CHAR*)"()", 3);
			}
			OfrontOPM_WriteString((CHAR*)", ", 3);
			OfrontOPC_DeclareParams(obj->link, 1);
			OfrontOPM_Write(')');
			OfrontOPM_WriteLn();
		}
		OfrontOPC_DefineTProcMacros(obj->right, &*empty);
	}
}

static void OfrontOPC_DefineType (OfrontOPT_Struct str)
{
	OfrontOPT_Object obj = NIL, field = NIL, par = NIL;
	BOOLEAN empty;
	if (OfrontOPM_currFile == 1 || str->ref < 255) {
		obj = str->strobj;
		if (obj == NIL || OfrontOPC_Undefined(obj)) {
			if (obj != NIL) {
				if (obj->linkadr == 1) {
					if (str->form != 13) {
						OfrontOPM_Mark(244, str->txtpos);
						obj->linkadr = 2;
					}
				} else {
					obj->linkadr = 1;
				}
			}
			if (str->comp == 4) {
				if (str->BaseTyp != NIL) {
					OfrontOPC_DefineType(str->BaseTyp);
				}
				field = str->link;
				while (field != NIL && field->mode == 4) {
					if (field->vis != 0 || OfrontOPM_currFile == 1) {
						OfrontOPC_DefineType(field->typ);
					}
					field = field->link;
				}
			} else if (str->form == 13) {
				if (str->BaseTyp->comp != 4) {
					OfrontOPC_DefineType(str->BaseTyp);
				}
			} else if (__IN(str->comp, 0x0c)) {
				OfrontOPC_DefineType(str->BaseTyp);
			} else if (str->form == 14) {
				if (str->BaseTyp != OfrontOPT_notyp) {
					OfrontOPC_DefineType(str->BaseTyp);
				}
				field = str->link;
				while (field != NIL) {
					OfrontOPC_DefineType(field->typ);
					field = field->link;
				}
			}
		}
		if (obj != NIL && OfrontOPC_Undefined(obj)) {
			OfrontOPM_WriteString((CHAR*)"typedef", 8);
			OfrontOPM_WriteLn();
			OfrontOPM_Write(0x09);
			OfrontOPC_Indent(1);
			obj->linkadr = 1;
			OfrontOPC_DeclareBase(obj);
			OfrontOPM_Write(' ');
			obj->typ->strobj = NIL;
			OfrontOPC_DeclareObj(obj, 0);
			obj->typ->strobj = obj;
			obj->linkadr = 3 + OfrontOPM_currFile;
			OfrontOPC_EndStat();
			OfrontOPC_Indent(-1);
			OfrontOPM_WriteLn();
			if (obj->typ->comp == 4) {
				empty = 1;
				OfrontOPC_DeclareTProcs(str->link, &empty);
				OfrontOPC_DefineTProcMacros(str->link, &empty);
				if (!empty) {
					OfrontOPM_WriteLn();
				}
			}
		}
	}
}

static BOOLEAN OfrontOPC_Prefixed (OfrontOPT_ConstExt x, CHAR *y, INTEGER y__len)
{
	INTEGER i;
	__DUP(y, y__len);
	i = 0;
	while ((*x)[__X(i + 1, 256)] == y[__X(i, y__len)]) {
		i += 1;
	}
	__DEL(y);
	return y[__X(i, y__len)] == 0x00;
}

static void OfrontOPC_CProcDefs (OfrontOPT_Object obj, INTEGER vis)
{
	INTEGER i;
	OfrontOPT_ConstExt ext = NIL;
	INTEGER _for__9;
	if (obj != NIL) {
		OfrontOPC_CProcDefs(obj->left, vis);
		if ((obj->mode == 9 && (INTEGER)obj->vis >= vis) && obj->adr == 1) {
			ext = obj->conval->ext;
			i = 1;
			if ((*ext)[1] != '#' && !(OfrontOPC_Prefixed(ext, (CHAR*)"extern ", 8) || OfrontOPC_Prefixed(ext, (CHAR*)"import ", 8))) {
				OfrontOPM_WriteString((CHAR*)"#define ", 9);
				OfrontOPC_Ident(obj);
				OfrontOPC_DeclareParams(obj->link, 1);
				OfrontOPM_Write(0x09);
			}
			_for__9 = (SHORTINT)(*obj->conval->ext)[0];
			i = i;
			while (i <= _for__9) {
				OfrontOPM_Write((*obj->conval->ext)[__X(i, 256)]);
				i += 1;
			}
			OfrontOPM_WriteLn();
		}
		OfrontOPC_CProcDefs(obj->right, vis);
	}
}

void OfrontOPC_TypeDefs (OfrontOPT_Object obj, INTEGER vis)
{
	if (obj != NIL) {
		OfrontOPC_TypeDefs(obj->left, vis);
		if (obj->mode == 5 && obj->typ->txtpos > 0) {
			OfrontOPC_DefineType(obj->typ);
		}
		OfrontOPC_TypeDefs(obj->right, vis);
	}
}

/*----------------------------------------------------------------------------*/
static void OfrontOPC_DefAnonRecs (OfrontOPT_Node n)
{
	OfrontOPT_Object o = NIL;
	OfrontOPT_Struct typ = NIL;
	while (n != NIL && n->class == 14) {
		typ = n->typ;
		if (typ->strobj == NIL && (OfrontOPM_currFile == 1 || typ->ref < 255)) {
			OfrontOPC_DefineType(typ);
			__NEW(o, OfrontOPT_ObjDesc);
			o->typ = typ;
			o->name[0] = 0x00;
			OfrontOPC_DeclareBase(o);
			OfrontOPC_EndStat();
			OfrontOPM_WriteLn();
		}
		n = n->link;
	}
}

void OfrontOPC_TDescDecl (OfrontOPT_Struct typ)
{
	LONGINT nofptrs;
	OfrontOPT_Object o = NIL;
	OfrontOPC_BegStat();
	OfrontOPM_WriteString((CHAR*)"__TDESC(", 9);
	OfrontOPC_Andent(typ);
	OfrontOPM_WriteString((CHAR*)"__desc", 7);
	OfrontOPC_Str1((CHAR*)", #", 4, typ->n + 1);
	OfrontOPC_Str1((CHAR*)", #) = {__TDFLDS(", 18, OfrontOPC_NofPtrs(typ));
	OfrontOPM_Write('\"');
	if (typ->strobj != NIL) {
		OfrontOPM_WriteStringVar((void*)typ->strobj->name, 32);
	}
	OfrontOPC_Str1((CHAR*)"\", #), {", 9, typ->size);
	nofptrs = 0;
	OfrontOPC_PutPtrOffsets(typ, 0, &nofptrs);
	OfrontOPC_Str1((CHAR*)"#}}", 4, -((nofptrs + 1) * (LONGINT)OfrontOPM_LIntSize));
	OfrontOPC_EndStat();
}

/*----------------------------------------------------------------------------*/
void OfrontOPC_InitTDesc (OfrontOPT_Struct typ)
{
	OfrontOPC_BegStat();
	OfrontOPM_WriteString((CHAR*)"__INITYP(", 10);
	OfrontOPC_Andent(typ);
	OfrontOPM_WriteString((CHAR*)", ", 3);
	if (typ->BaseTyp != NIL) {
		OfrontOPC_Andent(typ->BaseTyp);
	} else {
		OfrontOPC_Andent(typ);
	}
	OfrontOPC_Str1((CHAR*)", #)", 5, typ->extlev);
	OfrontOPC_EndStat();
	if (typ->strobj != NIL) {
		OfrontOPC_InitTProcs(typ->strobj, typ->link);
	}
}

/*----------------------------------------------------------------------------*/
void OfrontOPC_Align (LONGINT *adr, LONGINT base)
{
	switch (base) {
		case 2: 
			*adr += __MASK(*adr, -2);
			break;
		case 4: 
			*adr += __MASK(-*adr, -4);
			break;
		case 8: 
			*adr += __MASK(-*adr, -8);
			break;
		case 16: 
			*adr += __MASK(-*adr, -16);
			break;
		default: 
			break;
	}
}

/*----------------------------------------------------------------------------*/
LONGINT OfrontOPC_Base (OfrontOPT_Struct typ)
{
	switch (typ->form) {
		case 1: 
			return 1;
			break;
		case 3: 
			return OfrontOPM_CharAlign;
			break;
		case 2: 
			return OfrontOPM_BoolAlign;
			break;
		case 4: 
			return OfrontOPM_SIntAlign;
			break;
		case 5: 
			return OfrontOPM_IntAlign;
			break;
		case 6: 
			return OfrontOPM_LIntAlign;
			break;
		case 7: 
			return OfrontOPM_RealAlign;
			break;
		case 8: 
			return OfrontOPM_LRealAlign;
			break;
		case 9: 
			return OfrontOPM_SetAlign;
			break;
		case 13: 
			return OfrontOPM_PointerAlign;
			break;
		case 14: 
			return OfrontOPM_ProcAlign;
			break;
		case 15: 
			if (typ->comp == 4) {
				return __MASK(typ->align, -65536);
			} else {
				return OfrontOPC_Base(typ->BaseTyp);
			}
			break;
		default: __CASECHK;
	}
	__RETCHK;
}

/*----------------------------------------------------------------------------*/
static void OfrontOPC_FillGap (LONGINT gap, LONGINT off, LONGINT align, LONGINT *n, LONGINT *curAlign)
{
	LONGINT adr;
	adr = off;
	OfrontOPC_Align(&adr, align);
	if (*curAlign < align && gap - (adr - off) >= align) {
		gap -= (adr - off) + align;
		OfrontOPC_BegStat();
		if (align == (LONGINT)OfrontOPM_IntSize) {
			OfrontOPM_WriteString((CHAR*)"INTEGER", 8);
		} else if (align == (LONGINT)OfrontOPM_LIntSize) {
			OfrontOPM_WriteString((CHAR*)"LONGINT", 8);
		} else if (align == (LONGINT)OfrontOPM_LRealSize) {
			OfrontOPM_WriteString((CHAR*)"LONGREAL", 9);
		}
		OfrontOPC_Str1((CHAR*)" _prvt#", 8, *n);
		*n += 1;
		OfrontOPC_EndStat();
		*curAlign = align;
	}
	if (gap > 0) {
		OfrontOPC_BegStat();
		OfrontOPC_Str1((CHAR*)"char _prvt#", 12, *n);
		*n += 1;
		OfrontOPC_Str1((CHAR*)"[#]", 4, gap);
		OfrontOPC_EndStat();
	}
}

static void OfrontOPC_FieldList (OfrontOPT_Struct typ, BOOLEAN last, LONGINT *off, LONGINT *n, LONGINT *curAlign)
{
	OfrontOPT_Object fld = NIL;
	OfrontOPT_Struct base = NIL;
	LONGINT gap, adr, align, fldAlign;
	fld = typ->link;
	align = __MASK(typ->align, -65536);
	if (typ->BaseTyp != NIL) {
		OfrontOPC_FieldList(typ->BaseTyp, 0, &*off, &*n, &*curAlign);
	} else {
		*off = 0;
		*n = 0;
		*curAlign = 1;
	}
	while (fld != NIL && fld->mode == 4) {
		if (OfrontOPM_currFile == 0 && fld->vis == 0 || (OfrontOPM_currFile == 1 && fld->vis == 0) && typ->mno != 0) {
			fld = fld->link;
			while ((fld != NIL && fld->mode == 4) && fld->vis == 0) {
				fld = fld->link;
			}
		} else {
			adr = *off;
			fldAlign = OfrontOPC_Base(fld->typ);
			OfrontOPC_Align(&adr, fldAlign);
			gap = fld->adr - adr;
			if (fldAlign > *curAlign) {
				*curAlign = fldAlign;
			}
			if (gap > 0) {
				OfrontOPC_FillGap(gap, *off, align, &*n, &*curAlign);
			}
			OfrontOPC_BegStat();
			OfrontOPC_DeclareBase(fld);
			OfrontOPM_Write(' ');
			OfrontOPC_DeclareObj(fld, 0);
			*off = fld->adr + fld->typ->size;
			base = fld->typ;
			fld = fld->link;
			while ((((fld != NIL && fld->mode == 4) && fld->typ == base) && fld->adr == *off) && ((OfrontOPM_currFile == 1 || fld->vis != 0) || fld->typ->strobj == NIL)) {
				OfrontOPM_WriteString((CHAR*)", ", 3);
				OfrontOPC_DeclareObj(fld, 0);
				*off = fld->adr + fld->typ->size;
				fld = fld->link;
			}
			OfrontOPC_EndStat();
		}
	}
	if (last) {
		adr = typ->size - (LONGINT)__ASHR(typ->sysflag, 8, INTEGER);
		if (adr == 0) {
			gap = 1;
		} else {
			gap = adr - *off;
		}
		if (gap > 0) {
			OfrontOPC_FillGap(gap, *off, align, &*n, &*curAlign);
		}
	}
}

static void OfrontOPC_IdentList (OfrontOPT_Object obj, INTEGER vis)
{
	OfrontOPT_Struct base = NIL;
	BOOLEAN first;
	INTEGER lastvis;
	base = NIL;
	first = 1;
	while (obj != NIL && obj->mode != 13) {
		if ((__IN(vis, 0x05) || vis == 1 && obj->vis != 0) || vis == 3 && !obj->leaf) {
			if (obj->typ != base || (INTEGER)obj->vis != lastvis) {
				if (!first) {
					OfrontOPC_EndStat();
				}
				first = 0;
				base = obj->typ;
				lastvis = obj->vis;
				OfrontOPC_BegStat();
				if (vis == 1 && obj->vis != 0) {
					OfrontOPM_WriteString((CHAR*)"import ", 8);
				} else if (obj->mnolev == 0 && vis == 0) {
					if (obj->vis == 0) {
						OfrontOPM_WriteString((CHAR*)"static ", 8);
					} else {
						OfrontOPM_WriteString((CHAR*)"export ", 8);
					}
				}
				if ((vis == 2 && obj->mode == 1) && base->form == 7) {
					OfrontOPM_WriteString((CHAR*)"double", 7);
				} else {
					OfrontOPC_DeclareBase(obj);
				}
			} else {
				OfrontOPM_Write(',');
			}
			OfrontOPM_Write(' ');
			if ((vis == 2 && obj->mode == 1) && base->form == 7) {
				OfrontOPM_Write('_');
			}
			OfrontOPC_DeclareObj(obj, vis == 3);
			if (obj->typ->comp == 3) {
				OfrontOPC_EndStat();
				OfrontOPC_BegStat();
				base = OfrontOPT_linttyp;
				if (OfrontOPM_ArrLenSize == OfrontOPM_SIntSize) {
					OfrontOPM_WriteString((CHAR*)"SHORTINT ", 10);
				} else if (OfrontOPM_ArrLenSize == OfrontOPM_IntSize) {
					OfrontOPM_WriteString((CHAR*)"INTEGER ", 9);
				} else {
					OfrontOPM_WriteString((CHAR*)"LONGINT ", 9);
				}
				OfrontOPC_LenList(obj, 0, 1);
			} else if (obj->mode == 2 && obj->typ->comp == 4) {
				OfrontOPC_EndStat();
				OfrontOPC_BegStat();
				OfrontOPM_WriteString((CHAR*)"SYSTEM_ADR *", 13);
				OfrontOPC_Ident(obj);
				OfrontOPM_WriteString((CHAR*)"__typ", 6);
				base = NIL;
			} else if (((OfrontOPC_ptrinit && vis == 0) && obj->mnolev > 0) && obj->typ->form == 13) {
				OfrontOPM_WriteString((CHAR*)" = NIL", 7);
			}
		}
		obj = obj->link;
	}
	if (!first) {
		OfrontOPC_EndStat();
	}
}

static void OfrontOPC_AnsiParamList (OfrontOPT_Object obj, BOOLEAN showParamNames)
{
	CHAR name[32];
	OfrontOPM_Write('(');
	if (obj == NIL || obj->mode == 13) {
		OfrontOPM_WriteString((CHAR*)"void", 5);
	} else {
		for (;;) {
			OfrontOPC_DeclareBase(obj);
			if (showParamNames) {
				OfrontOPM_Write(' ');
				OfrontOPC_DeclareObj(obj, 0);
			} else {
				__COPY(obj->name, name, 32);
				obj->name[0] = 0x00;
				OfrontOPC_DeclareObj(obj, 0);
				__COPY(name, obj->name, 32);
			}
			if (obj->typ->comp == 3) {
				if (OfrontOPM_ArrLenSize == OfrontOPM_SIntSize) {
					OfrontOPM_WriteString((CHAR*)", SHORTINT ", 12);
				} else if (OfrontOPM_ArrLenSize == OfrontOPM_IntSize) {
					OfrontOPM_WriteString((CHAR*)", INTEGER ", 11);
				} else {
					OfrontOPM_WriteString((CHAR*)", LONGINT ", 11);
				}
				OfrontOPC_LenList(obj, 1, showParamNames);
			} else if (obj->mode == 2 && obj->typ->comp == 4) {
				OfrontOPM_WriteString((CHAR*)", SYSTEM_ADR *", 15);
				if (showParamNames) {
					OfrontOPC_Ident(obj);
					OfrontOPM_WriteString((CHAR*)"__typ", 6);
				}
			}
			if (obj->link == NIL || obj->link->mode == 13) {
				break;
			}
			OfrontOPM_WriteString((CHAR*)", ", 3);
			obj = obj->link;
		}
	}
	OfrontOPM_Write(')');
}

static void OfrontOPC_ProcHeader (OfrontOPT_Object proc, BOOLEAN define)
{
	if (proc->typ == OfrontOPT_notyp) {
		OfrontOPM_WriteString((CHAR*)"void", 5);
	} else {
		OfrontOPC_Ident(proc->typ->strobj);
	}
	OfrontOPM_Write(' ');
	OfrontOPC_Ident(proc);
	OfrontOPM_Write(' ');
	if (OfrontOPC_ansi) {
		OfrontOPC_AnsiParamList(proc->link, 1);
		if (!define) {
			OfrontOPM_Write(';');
		}
		OfrontOPM_WriteLn();
	} else if (define) {
		OfrontOPC_DeclareParams(proc->link, 0);
		OfrontOPM_WriteLn();
		OfrontOPC_Indent(1);
		OfrontOPC_IdentList(proc->link, 2);
		OfrontOPC_Indent(-1);
	} else {
		OfrontOPM_WriteString((CHAR*)"();", 4);
		OfrontOPM_WriteLn();
	}
}

static void OfrontOPC_ProcPredefs (OfrontOPT_Object obj, SHORTINT vis)
{
	if (obj != NIL) {
		OfrontOPC_ProcPredefs(obj->left, vis);
		if ((__IN(obj->mode, 0xc0) && obj->vis >= vis) && (obj->history != 4 || obj->mode == 6)) {
			if (vis == 1) {
				OfrontOPM_WriteString((CHAR*)"import ", 8);
			} else if (obj->vis == 0) {
				OfrontOPM_WriteString((CHAR*)"static ", 8);
			} else {
				OfrontOPM_WriteString((CHAR*)"export ", 8);
			}
			OfrontOPC_ProcHeader(obj, 0);
		}
		OfrontOPC_ProcPredefs(obj->right, vis);
	}
}

static void OfrontOPC_Include (CHAR *name, INTEGER name__len)
{
	__DUP(name, name__len);
	OfrontOPM_WriteString((CHAR*)"#include ", 10);
	OfrontOPM_Write('\"');
	OfrontOPM_WriteStringVar((void*)name, name__len);
	OfrontOPM_WriteString((CHAR*)".h", 3);
	OfrontOPM_Write('\"');
	OfrontOPM_WriteLn();
	__DEL(name);
}

static void OfrontOPC_IncludeImports (OfrontOPT_Object obj, INTEGER vis)
{
	if (obj != NIL) {
		OfrontOPC_IncludeImports(obj->left, vis);
		if ((obj->mode == 11 && obj->mnolev != 0) && (INTEGER)OfrontOPT_GlbMod[__X(-obj->mnolev, 64)]->vis >= vis) {
			OfrontOPC_Include(OfrontOPT_GlbMod[__X(-obj->mnolev, 64)]->name, 32);
		}
		OfrontOPC_IncludeImports(obj->right, vis);
	}
}

static void OfrontOPC_GenDynTypes (OfrontOPT_Node n, INTEGER vis)
{
	OfrontOPT_Struct typ = NIL;
	while (n != NIL && n->class == 14) {
		typ = n->typ;
		if (vis == 0 || typ->ref < 255) {
			OfrontOPC_BegStat();
			if (vis == 1) {
				OfrontOPM_WriteString((CHAR*)"import ", 8);
			} else if (typ->strobj != NIL && typ->strobj->mnolev > 0) {
				OfrontOPM_WriteString((CHAR*)"static ", 8);
			} else {
				OfrontOPM_WriteString((CHAR*)"export ", 8);
			}
			OfrontOPM_WriteString((CHAR*)"SYSTEM_ADR *", 13);
			OfrontOPC_Andent(typ);
			OfrontOPM_WriteString((CHAR*)"__typ", 6);
			OfrontOPC_EndStat();
		}
		n = n->link;
	}
}

void OfrontOPC_GenHdr (OfrontOPT_Node n)
{
	OfrontOPM_currFile = 0;
	OfrontOPC_DefAnonRecs(n);
	OfrontOPC_TypeDefs(OfrontOPT_topScope->right, 1);
	OfrontOPM_WriteLn();
	OfrontOPC_IdentList(OfrontOPT_topScope->scope, 1);
	OfrontOPM_WriteLn();
	OfrontOPC_GenDynTypes(n, 1);
	OfrontOPM_WriteLn();
	OfrontOPC_ProcPredefs(OfrontOPT_topScope->right, 1);
	OfrontOPM_WriteString((CHAR*)"import ", 8);
	OfrontOPM_WriteString((CHAR*)"void *", 7);
	OfrontOPM_WriteString(OfrontOPM_modName, 32);
	OfrontOPM_WriteString(OfrontOPC_BodyNameExt, 13);
	OfrontOPC_EndStat();
	OfrontOPM_WriteLn();
	OfrontOPC_CProcDefs(OfrontOPT_topScope->right, 1);
	OfrontOPM_WriteLn();
	OfrontOPM_WriteString((CHAR*)"#endif", 7);
	OfrontOPM_WriteLn();
}

/*----------------------------------------------------------------------------*/
static void OfrontOPC_GenHeaderMsg (void)
{
	OfrontOPM_WriteString((CHAR*)"/* ", 4);
	OfrontOPM_WriteString((CHAR*)"Ofront+ 0.9 -", 14);
	if (__IN(9, OfrontOPM_glbopt)) {
		OfrontOPM_Write('e');
	}
	if (__IN(4, OfrontOPM_glbopt)) {
		OfrontOPM_Write('s');
	}
	if (__IN(10, OfrontOPM_glbopt)) {
		OfrontOPM_Write('m');
	}
	if (!__IN(0, OfrontOPM_glbopt)) {
		OfrontOPM_Write('x');
	}
	if (__IN(1, OfrontOPM_glbopt)) {
		OfrontOPM_Write('v');
	}
	if (__IN(2, OfrontOPM_glbopt)) {
		OfrontOPM_Write('r');
	}
	if (!__IN(3, OfrontOPM_glbopt)) {
		OfrontOPM_Write('t');
	}
	if (!__IN(7, OfrontOPM_glbopt)) {
		OfrontOPM_Write('a');
	}
	if (!__IN(6, OfrontOPM_glbopt)) {
		OfrontOPM_Write('k');
	}
	if (!__IN(5, OfrontOPM_glbopt)) {
		OfrontOPM_Write('p');
	}
	if (__IN(8, OfrontOPM_glbopt)) {
		OfrontOPM_Write('i');
	}
	if (__IN(12, OfrontOPM_glbopt)) {
		OfrontOPM_Write('1');
	}
	if (__IN(13, OfrontOPM_glbopt)) {
		OfrontOPM_Write('2');
	}
	if (__IN(14, OfrontOPM_glbopt)) {
		OfrontOPM_Write('C');
	}
	if (__IN(15, OfrontOPM_glbopt)) {
		OfrontOPM_Write('7');
	}
	OfrontOPM_WriteString((CHAR*)" */", 4);
	OfrontOPM_WriteLn();
}

void OfrontOPC_GenHdrIncludes (void)
{
	OfrontOPM_currFile = 2;
	OfrontOPC_GenHeaderMsg();
	OfrontOPM_WriteLn();
	OfrontOPM_WriteString((CHAR*)"#ifndef ", 9);
	OfrontOPM_WriteString(OfrontOPM_modName, 32);
	OfrontOPM_WriteString((CHAR*)"__h", 4);
	OfrontOPM_WriteLn();
	OfrontOPM_WriteString((CHAR*)"#define ", 9);
	OfrontOPM_WriteString(OfrontOPM_modName, 32);
	OfrontOPM_WriteString((CHAR*)"__h", 4);
	OfrontOPM_WriteLn();
	OfrontOPM_WriteLn();
	OfrontOPC_Include((CHAR*)"SYSTEM", 7);
	OfrontOPC_IncludeImports(OfrontOPT_topScope->right, 1);
	OfrontOPM_WriteLn();
}

/*----------------------------------------------------------------------------*/
void OfrontOPC_GenBdy (OfrontOPT_Node n)
{
	OfrontOPM_currFile = 1;
	OfrontOPC_GenHeaderMsg();
	OfrontOPC_Include((CHAR*)"SYSTEM", 7);
	OfrontOPC_IncludeImports(OfrontOPT_topScope->right, 0);
	OfrontOPM_WriteLn();
	OfrontOPC_DefAnonRecs(n);
	OfrontOPC_TypeDefs(OfrontOPT_topScope->right, 0);
	OfrontOPM_WriteLn();
	OfrontOPC_IdentList(OfrontOPT_topScope->scope, 0);
	OfrontOPM_WriteLn();
	OfrontOPC_GenDynTypes(n, 0);
	OfrontOPM_WriteLn();
	OfrontOPC_ProcPredefs(OfrontOPT_topScope->right, 0);
	OfrontOPM_WriteLn();
	OfrontOPC_CProcDefs(OfrontOPT_topScope->right, 0);
	OfrontOPM_WriteLn();
	OfrontOPM_WriteString((CHAR*)"/*============================================================================*/", 81);
	OfrontOPM_WriteLn();
	OfrontOPM_WriteLn();
}

/*----------------------------------------------------------------------------*/
static void OfrontOPC_RegCmds (OfrontOPT_Object obj)
{
	if (obj != NIL) {
		OfrontOPC_RegCmds(obj->left);
		if (obj->mode == 7 && obj->history != 4) {
			if ((obj->vis != 0 && obj->link == NIL) && obj->typ == OfrontOPT_notyp) {
				OfrontOPC_BegStat();
				OfrontOPM_WriteString((CHAR*)"__REGCMD(\"", 11);
				OfrontOPM_WriteStringVar((void*)obj->name, 32);
				OfrontOPM_WriteString((CHAR*)"\", ", 4);
				OfrontOPC_Ident(obj);
				OfrontOPM_Write(')');
				OfrontOPC_EndStat();
			}
		}
		OfrontOPC_RegCmds(obj->right);
	}
}

static void OfrontOPC_InitImports (OfrontOPT_Object obj)
{
	if (obj != NIL) {
		OfrontOPC_InitImports(obj->left);
		if (obj->mode == 11 && obj->mnolev != 0) {
			OfrontOPC_BegStat();
			OfrontOPM_WriteString((CHAR*)"__IMPORT(", 10);
			OfrontOPM_WriteStringVar((void*)OfrontOPT_GlbMod[__X(-obj->mnolev, 64)]->name, 32);
			OfrontOPM_WriteString((CHAR*)"__init", 7);
			OfrontOPM_Write(')');
			OfrontOPC_EndStat();
		}
		OfrontOPC_InitImports(obj->right);
	}
}

void OfrontOPC_GenEnumPtrs (OfrontOPT_Object var)
{
	OfrontOPT_Struct typ = NIL;
	LONGINT n;
	OfrontOPC_GlbPtrs = 0;
	while (var != NIL) {
		typ = var->typ;
		if (OfrontOPC_NofPtrs(typ) > 0) {
			if (!OfrontOPC_GlbPtrs) {
				OfrontOPC_GlbPtrs = 1;
				OfrontOPM_WriteString((CHAR*)"static ", 8);
				if (OfrontOPC_ansi) {
					OfrontOPM_WriteString((CHAR*)"void EnumPtrs(void (*P)(void*))", 32);
				} else {
					OfrontOPM_WriteString((CHAR*)"void EnumPtrs(P)", 17);
					OfrontOPM_WriteLn();
					OfrontOPM_Write(0x09);
					OfrontOPM_WriteString((CHAR*)"void (*P)();", 13);
				}
				OfrontOPM_WriteLn();
				OfrontOPC_BegBlk();
			}
			OfrontOPC_BegStat();
			if (typ->form == 13) {
				OfrontOPM_WriteString((CHAR*)"P(", 3);
				OfrontOPC_Ident(var);
				OfrontOPM_Write(')');
			} else if (typ->comp == 4) {
				OfrontOPM_WriteString((CHAR*)"__ENUMR(&", 10);
				OfrontOPC_Ident(var);
				OfrontOPM_WriteString((CHAR*)", ", 3);
				OfrontOPC_Andent(typ);
				OfrontOPM_WriteString((CHAR*)"__typ", 6);
				OfrontOPC_Str1((CHAR*)", #", 4, typ->size);
				OfrontOPM_WriteString((CHAR*)", 1, P)", 8);
			} else if (typ->comp == 2) {
				n = typ->n;
				typ = typ->BaseTyp;
				while (typ->comp == 2) {
					n = n * typ->n;
					typ = typ->BaseTyp;
				}
				if (typ->form == 13) {
					OfrontOPM_WriteString((CHAR*)"__ENUMP(", 9);
					OfrontOPC_Ident(var);
					OfrontOPC_Str1((CHAR*)", #, P)", 8, n);
				} else if (typ->comp == 4) {
					OfrontOPM_WriteString((CHAR*)"__ENUMR(", 9);
					OfrontOPC_Ident(var);
					OfrontOPM_WriteString((CHAR*)", ", 3);
					OfrontOPC_Andent(typ);
					OfrontOPM_WriteString((CHAR*)"__typ", 6);
					OfrontOPC_Str1((CHAR*)", #", 4, typ->size);
					OfrontOPC_Str1((CHAR*)", #, P)", 8, n);
				}
			}
			OfrontOPC_EndStat();
		}
		var = var->link;
	}
	if (OfrontOPC_GlbPtrs) {
		OfrontOPC_EndBlk();
		OfrontOPM_WriteLn();
	}
}

/*----------------------------------------------------------------------------*/
void OfrontOPC_EnterBody (void)
{
	OfrontOPM_WriteLn();
	if (OfrontOPC_mainprog) {
		if (OfrontOPC_ansi) {
			OfrontOPM_WriteString((CHAR*)"int main(int argc, char **argv)", 32);
			OfrontOPM_WriteLn();
		} else {
			OfrontOPM_WriteString((CHAR*)"main(argc, argv)", 17);
			OfrontOPM_WriteLn();
			OfrontOPM_Write(0x09);
			OfrontOPM_WriteString((CHAR*)"int argc; char **argv;", 23);
			OfrontOPM_WriteLn();
		}
	} else {
		OfrontOPM_WriteString((CHAR*)"export ", 8);
		OfrontOPM_WriteString((CHAR*)"void *", 7);
		OfrontOPM_WriteString(OfrontOPM_modName, 32);
		OfrontOPM_WriteString(OfrontOPC_BodyNameExt, 13);
		OfrontOPM_WriteLn();
	}
	OfrontOPC_BegBlk();
	OfrontOPC_BegStat();
	if (OfrontOPC_mainprog) {
		OfrontOPM_WriteString((CHAR*)"__INIT(argc, argv)", 19);
	} else {
		OfrontOPM_WriteString((CHAR*)"__DEFMOD", 9);
	}
	OfrontOPC_EndStat();
	OfrontOPC_InitImports(OfrontOPT_topScope->right);
	OfrontOPC_BegStat();
	if (OfrontOPC_mainprog) {
		OfrontOPM_WriteString((CHAR*)"__REGMAIN(\"", 12);
	} else {
		OfrontOPM_WriteString((CHAR*)"__REGMOD(\"", 11);
	}
	OfrontOPM_WriteString(OfrontOPM_modName, 32);
	if (OfrontOPC_GlbPtrs) {
		OfrontOPM_WriteString((CHAR*)"\", EnumPtrs)", 13);
	} else {
		OfrontOPM_WriteString((CHAR*)"\", 0)", 6);
	}
	OfrontOPC_EndStat();
	if (__STRCMP(OfrontOPM_modName, "SYSTEM") != 0) {
		OfrontOPC_RegCmds(OfrontOPT_topScope);
	}
}

/*----------------------------------------------------------------------------*/
void OfrontOPC_ExitBody (void)
{
	OfrontOPC_BegStat();
	if (OfrontOPC_mainprog) {
		OfrontOPM_WriteString((CHAR*)"__FINI;", 8);
	} else {
		OfrontOPM_WriteString((CHAR*)"__ENDMOD;", 10);
	}
	OfrontOPM_WriteLn();
	OfrontOPC_EndBlk();
}

/*----------------------------------------------------------------------------*/
void OfrontOPC_DefineInter (OfrontOPT_Object proc)
{
	OfrontOPT_Object scope = NIL;
	scope = proc->scope;
	OfrontOPM_WriteString((CHAR*)"static ", 8);
	OfrontOPM_WriteString((CHAR*)"struct ", 8);
	OfrontOPM_WriteStringVar((void*)scope->name, 32);
	OfrontOPM_Write(' ');
	OfrontOPC_BegBlk();
	OfrontOPC_IdentList(proc->link, 3);
	OfrontOPC_IdentList(scope->scope, 3);
	OfrontOPC_BegStat();
	OfrontOPM_WriteString((CHAR*)"struct ", 8);
	OfrontOPM_WriteStringVar((void*)scope->name, 32);
	OfrontOPM_Write(' ');
	OfrontOPM_Write('*');
	OfrontOPM_WriteString((CHAR*)"lnk", 4);
	OfrontOPC_EndStat();
	OfrontOPC_EndBlk0();
	OfrontOPM_Write(' ');
	OfrontOPM_Write('*');
	OfrontOPM_WriteStringVar((void*)scope->name, 32);
	OfrontOPM_WriteString((CHAR*)"_s", 3);
	OfrontOPC_EndStat();
	OfrontOPM_WriteLn();
	OfrontOPC_ProcPredefs(scope->right, 0);
	OfrontOPM_WriteLn();
}

/*----------------------------------------------------------------------------*/
BOOLEAN OfrontOPC_NeedsRetval (OfrontOPT_Object proc)
{
	return proc->typ != OfrontOPT_notyp && !proc->scope->leaf;
}

/*----------------------------------------------------------------------------*/
void OfrontOPC_EnterProc (OfrontOPT_Object proc)
{
	OfrontOPT_Object var = NIL, scope = NIL;
	OfrontOPT_Struct typ = NIL;
	INTEGER dim;
	if (proc->vis != 1) {
		OfrontOPM_WriteString((CHAR*)"static ", 8);
	}
	OfrontOPC_ProcHeader(proc, 1);
	OfrontOPC_BegBlk();
	scope = proc->scope;
	OfrontOPC_IdentList(scope->scope, 0);
	if (!scope->leaf) {
		OfrontOPC_BegStat();
		OfrontOPM_WriteString((CHAR*)"struct ", 8);
		OfrontOPM_WriteStringVar((void*)scope->name, 32);
		OfrontOPM_Write(' ');
		OfrontOPM_WriteString((CHAR*)"_s", 3);
		OfrontOPC_EndStat();
	}
	if (OfrontOPC_NeedsRetval(proc)) {
		OfrontOPC_BegStat();
		OfrontOPC_Ident(proc->typ->strobj);
		OfrontOPM_WriteString((CHAR*)" __retval", 10);
		OfrontOPC_EndStat();
	}
	var = proc->link;
	while (var != NIL) {
		if (var->typ->comp == 2 && var->mode == 1) {
			OfrontOPC_BegStat();
			if (var->typ->strobj == NIL) {
				OfrontOPM_Mark(200, var->typ->txtpos);
			} else {
				OfrontOPC_Ident(var->typ->strobj);
			}
			OfrontOPM_Write(' ');
			OfrontOPC_Ident(var);
			OfrontOPM_WriteString((CHAR*)"__copy", 7);
			OfrontOPC_EndStat();
		}
		var = var->link;
	}
	if (!OfrontOPC_ansi) {
		var = proc->link;
		while (var != NIL) {
			if (var->typ->form == 7 && var->mode == 1) {
				OfrontOPC_BegStat();
				OfrontOPC_Ident(var->typ->strobj);
				OfrontOPM_Write(' ');
				OfrontOPC_Ident(var);
				OfrontOPM_WriteString((CHAR*)" = _", 5);
				OfrontOPC_Ident(var);
				OfrontOPC_EndStat();
			}
			var = var->link;
		}
	}
	var = proc->link;
	while (var != NIL) {
		if ((__IN(var->typ->comp, 0x0c) && var->mode == 1) && var->typ->sysflag == 0) {
			OfrontOPC_BegStat();
			if (var->typ->comp == 2) {
				OfrontOPM_WriteString((CHAR*)"__DUPARR(", 10);
				OfrontOPC_Ident(var);
			} else {
				OfrontOPM_WriteString((CHAR*)"__DUP(", 7);
				OfrontOPC_Ident(var);
				OfrontOPM_WriteString((CHAR*)", ", 3);
				OfrontOPC_Ident(var);
				OfrontOPM_WriteString((CHAR*)"__len", 6);
				typ = var->typ->BaseTyp;
				dim = 1;
				while (typ->comp == 3) {
					OfrontOPM_WriteString((CHAR*)" * ", 4);
					OfrontOPC_Ident(var);
					OfrontOPM_WriteString((CHAR*)"__len", 6);
					OfrontOPM_WriteInt(dim);
					typ = typ->BaseTyp;
					dim += 1;
				}
			}
			OfrontOPM_Write(')');
			OfrontOPC_EndStat();
		}
		var = var->link;
	}
	if (!scope->leaf) {
		var = proc->link;
		while (var != NIL) {
			if (!var->leaf) {
				OfrontOPC_BegStat();
				OfrontOPM_WriteString((CHAR*)"_s", 3);
				OfrontOPM_Write('.');
				OfrontOPC_Ident(var);
				OfrontOPM_WriteString((CHAR*)" = ", 4);
				if (__IN(var->typ->comp, 0x0c)) {
					OfrontOPM_WriteString((CHAR*)"(void*)", 8);
				} else if (var->mode != 2) {
					OfrontOPM_Write('&');
				}
				OfrontOPC_Ident(var);
				if (var->typ->comp == 3) {
					typ = var->typ;
					dim = 0;
					do {
						OfrontOPM_WriteString((CHAR*)"; ", 3);
						OfrontOPM_WriteString((CHAR*)"_s", 3);
						OfrontOPM_Write('.');
						OfrontOPC_Ident(var);
						OfrontOPM_WriteString((CHAR*)"__len", 6);
						if (dim != 0) {
							OfrontOPM_WriteInt(dim);
						}
						OfrontOPM_WriteString((CHAR*)" = ", 4);
						OfrontOPC_Ident(var);
						OfrontOPM_WriteString((CHAR*)"__len", 6);
						if (dim != 0) {
							OfrontOPM_WriteInt(dim);
						}
						typ = typ->BaseTyp;
					} while (!(typ->comp != 3));
				} else if (var->mode == 2 && var->typ->comp == 4) {
					OfrontOPM_WriteString((CHAR*)"; ", 3);
					OfrontOPM_WriteString((CHAR*)"_s", 3);
					OfrontOPM_Write('.');
					OfrontOPC_Ident(var);
					OfrontOPM_WriteString((CHAR*)"__typ", 6);
					OfrontOPM_WriteString((CHAR*)" = ", 4);
					OfrontOPC_Ident(var);
					OfrontOPM_WriteString((CHAR*)"__typ", 6);
				}
				OfrontOPC_EndStat();
			}
			var = var->link;
		}
		var = scope->scope;
		while (var != NIL) {
			if (!var->leaf) {
				OfrontOPC_BegStat();
				OfrontOPM_WriteString((CHAR*)"_s", 3);
				OfrontOPM_Write('.');
				OfrontOPC_Ident(var);
				OfrontOPM_WriteString((CHAR*)" = ", 4);
				if (var->typ->comp != 2) {
					OfrontOPM_Write('&');
				} else {
					OfrontOPM_WriteString((CHAR*)"(void*)", 8);
				}
				OfrontOPC_Ident(var);
				OfrontOPC_EndStat();
			}
			var = var->link;
		}
		OfrontOPC_BegStat();
		OfrontOPM_WriteString((CHAR*)"_s", 3);
		OfrontOPM_Write('.');
		OfrontOPM_WriteString((CHAR*)"lnk", 4);
		OfrontOPM_WriteString((CHAR*)" = ", 4);
		OfrontOPM_WriteStringVar((void*)scope->name, 32);
		OfrontOPM_WriteString((CHAR*)"_s", 3);
		OfrontOPC_EndStat();
		OfrontOPC_BegStat();
		OfrontOPM_WriteStringVar((void*)scope->name, 32);
		OfrontOPM_WriteString((CHAR*)"_s", 3);
		OfrontOPM_WriteString((CHAR*)" = ", 4);
		OfrontOPM_Write('&');
		OfrontOPM_WriteString((CHAR*)"_s", 3);
		OfrontOPC_EndStat();
	}
}

/*----------------------------------------------------------------------------*/
void OfrontOPC_ExitProc (OfrontOPT_Object proc, BOOLEAN eoBlock, BOOLEAN implicitRet)
{
	OfrontOPT_Object var = NIL;
	BOOLEAN indent;
	indent = eoBlock;
	if (implicitRet && proc->typ != OfrontOPT_notyp) {
		OfrontOPM_Write(0x09);
		OfrontOPM_WriteString((CHAR*)"__RETCHK;", 10);
		OfrontOPM_WriteLn();
	} else if (!eoBlock || implicitRet) {
		if (!proc->scope->leaf) {
			if (indent) {
				OfrontOPC_BegStat();
			} else {
				indent = 1;
			}
			OfrontOPM_WriteStringVar((void*)proc->scope->name, 32);
			OfrontOPM_WriteString((CHAR*)"_s", 3);
			OfrontOPM_WriteString((CHAR*)" = ", 4);
			OfrontOPM_WriteString((CHAR*)"_s", 3);
			OfrontOPM_Write('.');
			OfrontOPM_WriteString((CHAR*)"lnk", 4);
			OfrontOPC_EndStat();
		}
		var = proc->link;
		while (var != NIL) {
			if ((var->typ->comp == 3 && var->mode == 1) && var->typ->sysflag == 0) {
				if (indent) {
					OfrontOPC_BegStat();
				} else {
					indent = 1;
				}
				OfrontOPM_WriteString((CHAR*)"__DEL(", 7);
				OfrontOPC_Ident(var);
				OfrontOPM_Write(')');
				OfrontOPC_EndStat();
			}
			var = var->link;
		}
	}
	if (eoBlock) {
		OfrontOPC_EndBlk();
		OfrontOPM_WriteLn();
	} else if (indent) {
		OfrontOPC_BegStat();
	}
	if (eoBlock && proc->vis == 1) {
		OfrontOPM_WriteString((CHAR*)"/*----------------------------------------------------------------------------*/", 81);
		OfrontOPM_WriteLn();
	}
}

/*----------------------------------------------------------------------------*/
void OfrontOPC_CompleteIdent (OfrontOPT_Object obj)
{
	INTEGER comp, level;
	level = obj->mnolev;
	if (obj->adr == 1) {
		if (obj->typ->comp == 4) {
			OfrontOPC_Ident(obj);
			OfrontOPM_WriteString((CHAR*)"__", 3);
		} else {
			OfrontOPM_WriteString((CHAR*)"((", 3);
			OfrontOPC_Ident(obj->typ->strobj);
			OfrontOPM_Write(')');
			OfrontOPC_Ident(obj);
			OfrontOPM_Write(')');
		}
	} else if (level != OfrontOPM_level && level > 0) {
		comp = obj->typ->comp;
		if (obj->mode != 2 && comp != 3) {
			OfrontOPM_Write('*');
		}
		OfrontOPM_WriteStringVar((void*)obj->scope->name, 32);
		OfrontOPM_WriteString((CHAR*)"_s", 3);
		OfrontOPM_WriteString((CHAR*)"->", 3);
		OfrontOPC_Ident(obj);
	} else {
		OfrontOPC_Ident(obj);
	}
}

/*----------------------------------------------------------------------------*/
void OfrontOPC_TypeOf (OfrontOPT_Object ap)
{
	INTEGER i;
	__ASSERT(ap->typ->comp == 4, 0);
	if (ap->mode == 2) {
		if ((INTEGER)ap->mnolev != OfrontOPM_level) {
			OfrontOPM_WriteStringVar((void*)ap->scope->name, 32);
			OfrontOPM_WriteString((CHAR*)"_s->", 5);
			OfrontOPC_Ident(ap);
		} else {
			OfrontOPC_Ident(ap);
		}
		OfrontOPM_WriteString((CHAR*)"__typ", 6);
	} else if (ap->typ->strobj != NIL) {
		OfrontOPC_Ident(ap->typ->strobj);
		OfrontOPM_WriteString((CHAR*)"__typ", 6);
	} else {
		OfrontOPC_Andent(ap->typ);
	}
}

/*----------------------------------------------------------------------------*/
void OfrontOPC_Cmp (INTEGER rel)
{
	switch (rel) {
		case 9: 
			OfrontOPM_WriteString((CHAR*)" == ", 5);
			break;
		case 10: 
			OfrontOPM_WriteString((CHAR*)" != ", 5);
			break;
		case 11: 
			OfrontOPM_WriteString((CHAR*)" < ", 4);
			break;
		case 12: 
			OfrontOPM_WriteString((CHAR*)" <= ", 5);
			break;
		case 13: 
			OfrontOPM_WriteString((CHAR*)" > ", 4);
			break;
		case 14: 
			OfrontOPM_WriteString((CHAR*)" >= ", 5);
			break;
		default: __CASECHK;
	}
}

/*----------------------------------------------------------------------------*/
void OfrontOPC_Case (LONGINT caseVal, INTEGER form)
{
	CHAR ch;
	OfrontOPM_WriteString((CHAR*)"case ", 6);
	switch (form) {
		case 3: 
			ch = (CHAR)caseVal;
			if (ch >= ' ' && ch <= '~') {
				OfrontOPM_Write('\'');
				if (((ch == '\\' || ch == '\?') || ch == '\'') || ch == '\"') {
					OfrontOPM_Write('\\');
					OfrontOPM_Write(ch);
				} else {
					OfrontOPM_Write(ch);
				}
				OfrontOPM_Write('\'');
			} else {
				OfrontOPM_WriteString((CHAR*)"0x", 3);
				OfrontOPM_WriteHex(caseVal);
			}
			break;
		case 4: case 5: case 6: 
			OfrontOPM_WriteInt(caseVal);
			break;
		default: __CASECHK;
	}
	OfrontOPM_WriteString((CHAR*)": ", 3);
}

/*----------------------------------------------------------------------------*/
void OfrontOPC_SetInclude (BOOLEAN exclude)
{
	if (exclude) {
		OfrontOPM_WriteString((CHAR*)" &= ~", 6);
	} else {
		OfrontOPM_WriteString((CHAR*)" |= ", 5);
	}
}

/*----------------------------------------------------------------------------*/
void OfrontOPC_Increment (BOOLEAN decrement)
{
	if (decrement) {
		OfrontOPM_WriteString((CHAR*)" -= ", 5);
	} else {
		OfrontOPM_WriteString((CHAR*)" += ", 5);
	}
}

/*----------------------------------------------------------------------------*/
void OfrontOPC_Halt (LONGINT n)
{
	OfrontOPC_Str1((CHAR*)"__HALT(#)", 10, n);
}

/*----------------------------------------------------------------------------*/
void OfrontOPC_Len (OfrontOPT_Object obj, OfrontOPT_Struct array, LONGINT dim)
{
	if (array->comp == 3) {
		OfrontOPC_CompleteIdent(obj);
		OfrontOPM_WriteString((CHAR*)"__len", 6);
		if (dim != 0) {
			OfrontOPM_WriteInt(dim);
		}
	} else {
		while (dim > 0) {
			array = array->BaseTyp;
			dim -= 1;
		}
		OfrontOPM_WriteInt(array->n);
		OfrontOPM_PromoteIntConstToLInt();
	}
}

/*----------------------------------------------------------------------------*/
void OfrontOPC_Constant (OfrontOPT_Const con, INTEGER form)
{
	INTEGER i, len;
	CHAR ch;
	SET s;
	LONGINT hex;
	BOOLEAN skipLeading;
	switch (form) {
		case 2: case 1: case 4: case 5: case 6: 
			OfrontOPM_WriteInt(con->intval);
			break;
		case 3: 
			ch = (CHAR)con->intval;
			if (ch >= ' ' && ch <= '~') {
				OfrontOPM_Write('\'');
				if (((ch == '\\' || ch == '\?') || ch == '\'') || ch == '\"') {
					OfrontOPM_Write('\\');
				}
				OfrontOPM_Write(ch);
				OfrontOPM_Write('\'');
			} else {
				OfrontOPM_WriteString((CHAR*)"0x", 3);
				OfrontOPM_WriteHex(con->intval);
			}
			break;
		case 7: 
			OfrontOPM_WriteReal(con->realval, 'f');
			break;
		case 8: 
			OfrontOPM_WriteReal(con->realval, 0x00);
			break;
		case 9: 
			OfrontOPM_WriteString((CHAR*)"0x", 3);
			skipLeading = 1;
			s = con->setval;
			i = 32;
			do {
				hex = 0;
				do {
					i -= 1;
					hex = __ASHL(hex, 1, LONGINT);
					if (__IN(i, s)) {
						hex += 1;
					}
				} while (!(__MASK(i, -8) == 0));
				if (hex != 0 || !skipLeading) {
					OfrontOPM_WriteHex(hex);
					skipLeading = 0;
				}
			} while (!(i == 0));
			if (skipLeading) {
				OfrontOPM_Write('0');
			}
			break;
		case 10: 
			OfrontOPM_Write('\"');
			len = (INTEGER)con->intval2 - 1;
			i = 0;
			while (i < len) {
				ch = (*con->ext)[__X(i, 256)];
				if (((ch == '\\' || ch == '\?') || ch == '\'') || ch == '\"') {
					OfrontOPM_Write('\\');
				}
				OfrontOPM_Write(ch);
				i += 1;
			}
			OfrontOPM_Write('\"');
			break;
		case 11: 
			OfrontOPM_WriteString((CHAR*)"NIL", 4);
			break;
		default: __CASECHK;
	}
}

/*----------------------------------------------------------------------------*/
static struct InitKeywords__47 {
	SHORTINT *n;
	struct InitKeywords__47 *lnk;
} *InitKeywords__47_s;

static void Enter__48 (CHAR *s, INTEGER s__len);

static void Enter__48 (CHAR *s, INTEGER s__len)
{
	INTEGER h;
	__DUP(s, s__len);
	h = OfrontOPC_PerfectHash((void*)s, s__len);
	OfrontOPC_hashtab[__X(h, 105)] = *InitKeywords__47_s->n;
	__COPY(s, OfrontOPC_keytab[__X(*InitKeywords__47_s->n, 36)], 9);
	*InitKeywords__47_s->n += 1;
	__DEL(s);
}

static void OfrontOPC_InitKeywords (void)
{
	SHORTINT n, i;
	struct InitKeywords__47 _s;
	_s.n = &n;
	_s.lnk = InitKeywords__47_s;
	InitKeywords__47_s = &_s;
	n = 0;
	i = 0;
	while (i <= 104) {
		OfrontOPC_hashtab[__X(i, 105)] = -1;
		i += 1;
	}
	Enter__48((CHAR*)"asm", 4);
	Enter__48((CHAR*)"auto", 5);
	Enter__48((CHAR*)"break", 6);
	Enter__48((CHAR*)"case", 5);
	Enter__48((CHAR*)"char", 5);
	Enter__48((CHAR*)"const", 6);
	Enter__48((CHAR*)"continue", 9);
	Enter__48((CHAR*)"default", 8);
	Enter__48((CHAR*)"do", 3);
	Enter__48((CHAR*)"double", 7);
	Enter__48((CHAR*)"else", 5);
	Enter__48((CHAR*)"enum", 5);
	Enter__48((CHAR*)"extern", 7);
	Enter__48((CHAR*)"export", 7);
	Enter__48((CHAR*)"float", 6);
	Enter__48((CHAR*)"for", 4);
	Enter__48((CHAR*)"fortran", 8);
	Enter__48((CHAR*)"goto", 5);
	Enter__48((CHAR*)"if", 3);
	Enter__48((CHAR*)"import", 7);
	Enter__48((CHAR*)"int", 4);
	Enter__48((CHAR*)"long", 5);
	Enter__48((CHAR*)"register", 9);
	Enter__48((CHAR*)"return", 7);
	Enter__48((CHAR*)"short", 6);
	Enter__48((CHAR*)"signed", 7);
	Enter__48((CHAR*)"sizeof", 7);
	Enter__48((CHAR*)"static", 7);
	Enter__48((CHAR*)"struct", 7);
	Enter__48((CHAR*)"switch", 7);
	Enter__48((CHAR*)"typedef", 8);
	Enter__48((CHAR*)"union", 6);
	Enter__48((CHAR*)"unsigned", 9);
	Enter__48((CHAR*)"void", 5);
	Enter__48((CHAR*)"volatile", 9);
	Enter__48((CHAR*)"while", 6);
	InitKeywords__47_s = _s.lnk;
}


export void *OfrontOPC__init(void)
{
	__DEFMOD;
	__IMPORT(OfrontOPM__init);
	__IMPORT(OfrontOPT__init);
	__REGMOD("OfrontOPC", 0);
	__REGCMD("BegBlk", OfrontOPC_BegBlk);
	__REGCMD("BegStat", OfrontOPC_BegStat);
	__REGCMD("EndBlk", OfrontOPC_EndBlk);
	__REGCMD("EndBlk0", OfrontOPC_EndBlk0);
	__REGCMD("EndStat", OfrontOPC_EndStat);
	__REGCMD("EnterBody", OfrontOPC_EnterBody);
	__REGCMD("ExitBody", OfrontOPC_ExitBody);
	__REGCMD("GenHdrIncludes", OfrontOPC_GenHdrIncludes);
	__REGCMD("Init", OfrontOPC_Init);
/* BEGIN */
	OfrontOPC_InitKeywords();
	__ENDMOD;
}

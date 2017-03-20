/* Ofront+ 0.9 - */
#include "SYSTEM.h"

struct Heap__1 {
	CHAR ch;
	SYSTEM_PTR p;
};

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
	struct Heap_FinDesc *Heap_FinNode;

typedef
	void (*Heap_Finalizer)(SYSTEM_PTR);

typedef
	struct Heap_FinDesc {
		Heap_FinNode next;
		LONGINT obj;
		BOOLEAN marked;
		Heap_Finalizer finalize;
	} Heap_FinDesc;

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
		LONGINT types;
		Heap_EnumProc enumPtrs;
		INTEGER reserved1, reserved2;
	} Heap_ModuleDesc;


export SYSTEM_PTR Heap_modules;
static LONGINT Heap_freeList[10];
static LONGINT Heap_bigBlocks;
export LONGINT Heap_allocated;
static BOOLEAN Heap_firstTry;
static SHORTINT Heap_ldUnit;
export LONGINT Heap_heap;
static LONGINT Heap_heapMin, Heap_heapMax;
export LONGINT Heap_heapsize, Heap_heapMinExpand;
static Heap_FinNode Heap_fin;
static SHORTINT Heap_lockdepth;
static BOOLEAN Heap_interrupted;
export SHORTINT Heap_FileCount;

export SYSTEM_ADR *Heap_ModuleDesc__typ;
export SYSTEM_ADR *Heap_CmdDesc__typ;
export SYSTEM_ADR *Heap_FinDesc__typ;
export SYSTEM_ADR *Heap__1__typ;

static void Heap_CheckFin (void);
static void Heap_ExtendHeap (LONGINT blksz);
export void Heap_FINALL (void);
static void Heap_Finalize (void);
export INTEGER Heap_FreeModule (CHAR *name, INTEGER name__len);
export void Heap_GC (BOOLEAN markStack);
static void Heap_HeapSort (INTEGER n, LONGINT *a, INTEGER a__len);
export void Heap_INCREF (Heap_Module m);
export void Heap_InitHeap (void);
export void Heap_Lock (void);
static void Heap_Mark (LONGINT q);
static void Heap_MarkCandidates (INTEGER n, LONGINT *cand, INTEGER cand__len);
static void Heap_MarkP (SYSTEM_PTR p);
static void Heap_MarkStack (LONGINT n, LONGINT *cand, INTEGER cand__len);
export SYSTEM_PTR Heap_NEWBLK (LONGINT size);
export SYSTEM_PTR Heap_NEWREC (LONGINT tag);
static LONGINT Heap_NewChunk (LONGINT blksz);
export void Heap_REGCMD (Heap_Module m, Heap_CmdName name, Heap_Command cmd);
export SYSTEM_PTR Heap_REGMOD (Heap_ModuleName name, Heap_EnumProc enumPtrs);
export void Heap_REGTYP (Heap_Module m, LONGINT typ);
export void Heap_RegisterFinalizer (SYSTEM_PTR obj, Heap_Finalizer finalize);
static void Heap_Scan (void);
static void Heap_Sift (INTEGER l, INTEGER r, LONGINT *a, INTEGER a__len);
export void Heap_Unlock (void);

extern void *Heap__init();
extern void *SYSTEM_MainStackFrame;
extern SYSTEM_ADRINT Platform_OSAllocate(INTEGER size);
#define Heap_HeapModuleInit()	Heap__init()
#define Heap_OSAllocate(size)	Platform_OSAllocate((INTEGER)(SYSTEM_ADR)(size))
#define Heap_SystemHalt(code)	__HALT(code)
#define Heap_SystemMainStackFrame()	(SYSTEM_ADRINT)SYSTEM_MainStackFrame
#define Heap_uLE(x, y)	((size_t)x <= (size_t)y)
#define Heap_uLT(x, y)	((size_t)x <  (size_t)y)

/*============================================================================*/

void Heap_Lock (void)
{
	Heap_lockdepth += 1;
}

/*----------------------------------------------------------------------------*/
void Heap_Unlock (void)
{
	Heap_lockdepth -= 1;
	if (Heap_interrupted && Heap_lockdepth == 0) {
		Heap_SystemHalt(-9);
	}
}

/*----------------------------------------------------------------------------*/
SYSTEM_PTR Heap_REGMOD (Heap_ModuleName name, Heap_EnumProc enumPtrs)
{
	Heap_Module m = NIL;
	if (__STRCMP(name, "Heap") == 0) {
		__SYSNEW(m, 64);
	} else {
		__NEW(m, Heap_ModuleDesc);
	}
	m->types = 0;
	m->cmds = NIL;
	__COPY(name, m->name, 20);
	m->refcnt = 0;
	m->enumPtrs = enumPtrs;
	m->next = (Heap_Module)Heap_modules;
	Heap_modules = (SYSTEM_PTR)m;
	return (void*)m;
}

/*----------------------------------------------------------------------------*/
INTEGER Heap_FreeModule (CHAR *name, INTEGER name__len)
{
	Heap_Module m = NIL, p = NIL;
	__DUP(name, name__len);
	m = (Heap_Module)Heap_modules;
	while (m != NIL && __STRCMP(m->name, name) != 0) {
		p = m;
		m = m->next;
	}
	if (m != NIL && m->refcnt == 0) {
		if (m == (Heap_Module)Heap_modules) {
			Heap_modules = (SYSTEM_PTR)m->next;
		} else {
			p->next = m->next;
		}
		__DEL(name);
		return 0;
	} else {
		if (m == NIL) {
			__DEL(name);
			return -1;
		} else {
			__DEL(name);
			return m->refcnt;
		}
	}
	__RETCHK;
}

/*----------------------------------------------------------------------------*/
void Heap_REGCMD (Heap_Module m, Heap_CmdName name, Heap_Command cmd)
{
	Heap_Cmd c = NIL;
	if (__STRCMP(m->name, "Heap") == 0) {
		__SYSNEW(c, 40);
	} else {
		__NEW(c, Heap_CmdDesc);
	}
	__COPY(name, c->name, 24);
	c->cmd = cmd;
	c->next = m->cmds;
	m->cmds = c;
}

/*----------------------------------------------------------------------------*/
void Heap_REGTYP (Heap_Module m, LONGINT typ)
{
	__PUT(typ, m->types, LONGINT);
	m->types = typ;
}

/*----------------------------------------------------------------------------*/
void Heap_INCREF (Heap_Module m)
{
	m->refcnt += 1;
}

/*----------------------------------------------------------------------------*/
static LONGINT Heap_NewChunk (LONGINT blksz)
{
	LONGINT chnk, blk, end;
	chnk = Heap_OSAllocate(blksz + 24);
	if (chnk != 0) {
		blk = chnk + 24;
		end = blk + blksz;
		__PUT(chnk + 8, end, LONGINT);
		__PUT(blk, blk + 8, LONGINT);
		__PUT(blk + 8, blksz, LONGINT);
		__PUT(blk + 16, -8, LONGINT);
		__PUT(blk + 24, Heap_bigBlocks, LONGINT);
		Heap_bigBlocks = blk;
		Heap_heapsize += blksz;
		if (Heap_uLT(blk + 8, Heap_heapMin)) {
			Heap_heapMin = blk + 8;
		}
		if (Heap_uLT(Heap_heapMax, end)) {
			Heap_heapMax = end;
		}
	}
	return chnk;
}

static void Heap_ExtendHeap (LONGINT blksz)
{
	LONGINT size, chnk, j, next;
	__ASSERT(__MASK(blksz, -32) == 0, 0);
	if (Heap_uLT(Heap_heapMinExpand, blksz)) {
		size = blksz;
	} else {
		size = Heap_heapMinExpand;
	}
	chnk = Heap_NewChunk(size);
	if (chnk != 0) {
		if (Heap_uLT(chnk, Heap_heap)) {
			__PUT(chnk, Heap_heap, LONGINT);
			Heap_heap = chnk;
		} else {
			j = Heap_heap;
			__GET(j, next, LONGINT);
			while (next != 0 && Heap_uLT(next, chnk)) {
				j = next;
				__GET(j, next, LONGINT);
			}
			__PUT(chnk, next, LONGINT);
			__PUT(j, chnk, LONGINT);
		}
	} else if (!Heap_firstTry) {
		Heap_heapMinExpand = 32;
	}
}

SYSTEM_PTR Heap_NEWREC (LONGINT tag)
{
	LONGINT i, i0, di, blksz, restsize, t, adr, end, next, prev;
	SYSTEM_PTR new = NIL;
	Heap_Lock();
	__GET(tag, blksz, LONGINT);
	__ASSERT(__MASK(blksz, -32) == 0, 0);
	i0 = __LSH(blksz, -Heap_ldUnit, LONGINT);
	i = i0;
	if (i < 9) {
		adr = Heap_freeList[__X(i, 10)];
		while (adr == 0) {
			i += 1;
			adr = Heap_freeList[__X(i, 10)];
		}
	}
	if (i < 9) {
		__GET(adr + 24, next, LONGINT);
		Heap_freeList[__X(i, 10)] = next;
		if (i != i0) {
			di = i - i0;
			restsize = __ASHL(di, 5, LONGINT);
			end = adr + restsize;
			__PUT(end + 8, blksz, LONGINT);
			__PUT(end + 16, -8, LONGINT);
			__PUT(end, end + 8, LONGINT);
			__PUT(adr + 8, restsize, LONGINT);
			__PUT(adr + 24, Heap_freeList[__X(di, 10)], LONGINT);
			Heap_freeList[__X(di, 10)] = adr;
			adr += restsize;
		}
	} else {
		adr = Heap_bigBlocks;
		prev = 0;
		for (;;) {
			if (adr == 0) {
				if (Heap_firstTry) {
					Heap_GC(1);
					blksz += 32;
					t = __LSH(Heap_allocated + blksz, -(2 + Heap_ldUnit), LONGINT) * 160;
					if (Heap_uLT(Heap_heapsize, t)) {
						Heap_ExtendHeap(t - Heap_heapsize);
					}
					Heap_firstTry = 0;
					new = Heap_NEWREC(tag);
					if (new == NIL) {
						Heap_ExtendHeap(blksz);
						new = Heap_NEWREC(tag);
					}
					Heap_firstTry = 1;
					Heap_Unlock();
					return new;
				} else {
					Heap_Unlock();
					return NIL;
				}
			}
			__GET(adr + 8, t, LONGINT);
			if (Heap_uLE(blksz, t)) {
				break;
			}
			prev = adr;
			__GET(adr + 24, adr, LONGINT);
		}
		restsize = t - blksz;
		end = adr + restsize;
		__PUT(end + 8, blksz, LONGINT);
		__PUT(end + 16, -8, LONGINT);
		__PUT(end, end + 8, LONGINT);
		if (Heap_uLT(288, restsize)) {
			__PUT(adr + 8, restsize, LONGINT);
		} else {
			__GET(adr + 24, next, LONGINT);
			if (prev == 0) {
				Heap_bigBlocks = next;
			} else {
				__PUT(prev + 24, next, LONGINT);
			}
			if (restsize != 0) {
				di = __ASHR(restsize, 5, LONGINT);
				__PUT(adr + 8, restsize, LONGINT);
				__PUT(adr + 24, Heap_freeList[__X(di, 10)], LONGINT);
				Heap_freeList[__X(di, 10)] = adr;
			}
		}
		adr += restsize;
	}
	i = adr + 32;
	end = adr + blksz;
	while (Heap_uLT(i, end)) {
		__PUT(i, 0, LONGINT);
		__PUT(i + 8, 0, LONGINT);
		__PUT(i + 16, 0, LONGINT);
		__PUT(i + 24, 0, LONGINT);
		i += 32;
	}
	__PUT(adr + 24, 0, LONGINT);
	__PUT(adr, tag, LONGINT);
	__PUT(adr + 8, 0, LONGINT);
	__PUT(adr + 16, 0, LONGINT);
	Heap_allocated += blksz;
	Heap_Unlock();
	return (SYSTEM_PTR)(adr + 8);
}

/*----------------------------------------------------------------------------*/
SYSTEM_PTR Heap_NEWBLK (LONGINT size)
{
	LONGINT blksz, tag;
	SYSTEM_PTR new = NIL;
	Heap_Lock();
	blksz = __ASHL(__ASHR(size + 63, 5, LONGINT), 5, LONGINT);
	new = Heap_NEWREC((LONGINT)&blksz);
	tag = ((LONGINT)new + blksz) - 24;
	__PUT(tag - 8, 0, LONGINT);
	__PUT(tag, blksz, LONGINT);
	__PUT(tag + 8, -8, LONGINT);
	__PUT((LONGINT)new - 8, tag, LONGINT);
	Heap_Unlock();
	return new;
}

/*----------------------------------------------------------------------------*/
static void Heap_Mark (LONGINT q)
{
	LONGINT p, tag, offset, fld, n, tagbits;
	if (q != 0) {
		__GET(q - 8, tagbits, LONGINT);
		if (!__ODD(tagbits)) {
			__PUT(q - 8, tagbits + 1, LONGINT);
			p = 0;
			tag = tagbits + 8;
			for (;;) {
				__GET(tag, offset, LONGINT);
				if (offset < 0) {
					__PUT(q - 8, (tag + offset) + 1, LONGINT);
					if (p == 0) {
						break;
					}
					n = q;
					q = p;
					__GET(q - 8, tag, LONGINT);
					tag -= 1;
					__GET(tag, offset, LONGINT);
					fld = q + offset;
					__GET(fld, p, LONGINT);
					__PUT(fld, (SYSTEM_PTR)n, SYSTEM_PTR);
				} else {
					fld = q + offset;
					__GET(fld, n, LONGINT);
					if (n != 0) {
						__GET(n - 8, tagbits, LONGINT);
						if (!__ODD(tagbits)) {
							__PUT(n - 8, tagbits + 1, LONGINT);
							__PUT(q - 8, tag + 1, LONGINT);
							__PUT(fld, (SYSTEM_PTR)p, SYSTEM_PTR);
							p = q;
							q = n;
							tag = tagbits;
						}
					}
				}
				tag += 8;
			}
		}
	}
}

static void Heap_MarkP (SYSTEM_PTR p)
{
	Heap_Mark((LONGINT)p);
}

static void Heap_Scan (void)
{
	LONGINT chnk, adr, end, start, tag, i, size, freesize;
	Heap_bigBlocks = 0;
	i = 1;
	while (i < 9) {
		Heap_freeList[__X(i, 10)] = 0;
		i += 1;
	}
	freesize = 0;
	Heap_allocated = 0;
	chnk = Heap_heap;
	while (chnk != 0) {
		adr = chnk + 24;
		__GET(chnk + 8, end, LONGINT);
		while (Heap_uLT(adr, end)) {
			__GET(adr, tag, LONGINT);
			if (__ODD(tag)) {
				if (freesize != 0) {
					start = adr - freesize;
					__PUT(start, start + 8, LONGINT);
					__PUT(start + 8, freesize, LONGINT);
					__PUT(start + 16, -8, LONGINT);
					i = __LSH(freesize, -Heap_ldUnit, LONGINT);
					freesize = 0;
					if (Heap_uLT(i, 9)) {
						__PUT(start + 24, Heap_freeList[__X(i, 10)], LONGINT);
						Heap_freeList[__X(i, 10)] = start;
					} else {
						__PUT(start + 24, Heap_bigBlocks, LONGINT);
						Heap_bigBlocks = start;
					}
				}
				tag -= 1;
				__PUT(adr, tag, LONGINT);
				__GET(tag, size, LONGINT);
				Heap_allocated += size;
				adr += size;
			} else {
				__GET(tag, size, LONGINT);
				freesize += size;
				adr += size;
			}
		}
		if (freesize != 0) {
			start = adr - freesize;
			__PUT(start, start + 8, LONGINT);
			__PUT(start + 8, freesize, LONGINT);
			__PUT(start + 16, -8, LONGINT);
			i = __LSH(freesize, -Heap_ldUnit, LONGINT);
			freesize = 0;
			if (Heap_uLT(i, 9)) {
				__PUT(start + 24, Heap_freeList[__X(i, 10)], LONGINT);
				Heap_freeList[__X(i, 10)] = start;
			} else {
				__PUT(start + 24, Heap_bigBlocks, LONGINT);
				Heap_bigBlocks = start;
			}
		}
		__GET(chnk, chnk, LONGINT);
	}
}

static void Heap_Sift (INTEGER l, INTEGER r, LONGINT *a, INTEGER a__len)
{
	INTEGER i, j;
	LONGINT x;
	j = l;
	x = a[__X(j, a__len)];
	for (;;) {
		i = j;
		j = __ASHL(j, 1, INTEGER) + 1;
		if (j < r && Heap_uLT(a[__X(j, a__len)], a[__X(j + 1, a__len)])) {
			j += 1;
		}
		if (j > r || Heap_uLE(a[__X(j, a__len)], x)) {
			break;
		}
		a[__X(i, a__len)] = a[__X(j, a__len)];
	}
	a[__X(i, a__len)] = x;
}

static void Heap_HeapSort (INTEGER n, LONGINT *a, INTEGER a__len)
{
	INTEGER l, r;
	LONGINT x;
	l = __ASHR(n, 1, INTEGER);
	r = n - 1;
	while (l > 0) {
		l -= 1;
		Heap_Sift(l, r, (void*)a, a__len);
	}
	while (r > 0) {
		x = a[0];
		a[0] = a[__X(r, a__len)];
		a[__X(r, a__len)] = x;
		r -= 1;
		Heap_Sift(l, r, (void*)a, a__len);
	}
}

static void Heap_MarkCandidates (INTEGER n, LONGINT *cand, INTEGER cand__len)
{
	LONGINT chnk, end, adr, tag, next, i, ptr, size;
	__ASSERT(n > 0, 0);
	chnk = Heap_heap;
	i = 0;
	while (chnk != 0) {
		__GET(chnk + 8, end, LONGINT);
		adr = chnk + 24;
		while (Heap_uLT(adr, end)) {
			__GET(adr, tag, LONGINT);
			if (__ODD(tag)) {
				__GET(tag - 1, size, LONGINT);
				adr += size;
				ptr = adr + 8;
				while (Heap_uLT(cand[__X(i, cand__len)], ptr)) {
					i += 1;
					if (i == (LONGINT)n) {
						return;
					}
				}
			} else {
				__GET(tag, size, LONGINT);
				ptr = adr + 8;
				adr += size;
				while (Heap_uLT(cand[__X(i, cand__len)], ptr)) {
					i += 1;
					if (i == (LONGINT)n) {
						return;
					}
				}
				if (Heap_uLT(cand[__X(i, cand__len)], adr)) {
					Heap_Mark(ptr);
				}
			}
			if (Heap_uLE(end, cand[__X(i, cand__len)])) {
				adr = end;
			}
		}
		__GET(chnk, chnk, LONGINT);
	}
}

static void Heap_CheckFin (void)
{
	Heap_FinNode n = NIL;
	LONGINT tag;
	n = Heap_fin;
	while (n != NIL) {
		__GET(n->obj - 8, tag, LONGINT);
		if (!__ODD(tag)) {
			n->marked = 0;
			Heap_Mark(n->obj);
		} else {
			n->marked = 1;
		}
		n = n->next;
	}
}

static void Heap_Finalize (void)
{
	Heap_FinNode n = NIL, prev = NIL;
	n = Heap_fin;
	prev = NIL;
	while (n != NIL) {
		if (!n->marked) {
			if (n == Heap_fin) {
				Heap_fin = Heap_fin->next;
			} else {
				prev->next = n->next;
			}
			(*n->finalize)((SYSTEM_PTR)n->obj);
			if (prev == NIL) {
				n = Heap_fin;
			} else {
				n = n->next;
			}
		} else {
			prev = n;
			n = n->next;
		}
	}
}

void Heap_FINALL (void)
{
	Heap_FinNode n = NIL;
	while (Heap_fin != NIL) {
		n = Heap_fin;
		Heap_fin = Heap_fin->next;
		(*n->finalize)((SYSTEM_PTR)n->obj);
	}
}

/*----------------------------------------------------------------------------*/
static void Heap_MarkStack (LONGINT n, LONGINT *cand, INTEGER cand__len)
{
	SYSTEM_PTR frame = NIL;
	INTEGER nofcand;
	LONGINT inc, sp, p, stack0;
	struct Heap__1 align;
	if (n > 0) {
		Heap_MarkStack(n - 1, cand, cand__len);
		if (n > 100) {
			return;
		}
	}
	if (n == 0) {
		nofcand = 0;
		sp = (LONGINT)&frame;
		stack0 = Heap_SystemMainStackFrame();
		inc = (LONGINT)&align.p - (LONGINT)&align;
		if (Heap_uLT(stack0, sp)) {
			inc = -inc;
		}
		while (sp != stack0) {
			__GET(sp, p, LONGINT);
			if (Heap_uLE(Heap_heapMin, p) && Heap_uLT(p, Heap_heapMax)) {
				if (nofcand == cand__len) {
					Heap_HeapSort(nofcand, (void*)cand, cand__len);
					Heap_MarkCandidates(nofcand, (void*)cand, cand__len);
					nofcand = 0;
				}
				cand[__X(nofcand, cand__len)] = p;
				nofcand += 1;
			}
			sp += inc;
		}
		if (nofcand > 0) {
			Heap_HeapSort(nofcand, (void*)cand, cand__len);
			Heap_MarkCandidates(nofcand, (void*)cand, cand__len);
		}
	}
}

void Heap_GC (BOOLEAN markStack)
{
	Heap_Module m = NIL;
	LONGINT i0, i1, i2, i3, i4, i5, i6, i7, i8, i9, i10, i11, i12, i13, i14, i15, i16, i17, i18, i19, i20, i21, i22, i23;
	LONGINT cand[10000];
	if (Heap_lockdepth == 0 || Heap_lockdepth == 1 && !markStack) {
		Heap_Lock();
		m = (Heap_Module)Heap_modules;
		while (m != NIL) {
			if (m->enumPtrs != NIL) {
				(*m->enumPtrs)(Heap_MarkP);
			}
			m = m->next;
		}
		if (markStack) {
			i0 = -100;
			i1 = -101;
			i2 = -102;
			i3 = -103;
			i4 = -104;
			i5 = -105;
			i6 = -106;
			i7 = -107;
			i8 = 1;
			i9 = 2;
			i10 = 3;
			i11 = 4;
			i12 = 5;
			i13 = 6;
			i14 = 7;
			i15 = 8;
			i16 = 9;
			i17 = 10;
			i18 = 11;
			i19 = 12;
			i20 = 13;
			i21 = 14;
			i22 = 15;
			i23 = 16;
			for (;;) {
				i0 += 1;
				i1 += 2;
				i2 += 3;
				i3 += 4;
				i4 += 5;
				i5 += 6;
				i6 += 7;
				i7 += 8;
				i8 += 9;
				i9 += 10;
				i10 += 11;
				i11 += 12;
				i12 += 13;
				i13 += 14;
				i14 += 15;
				i15 += 16;
				i16 += 17;
				i17 += 18;
				i18 += 19;
				i19 += 20;
				i20 += 21;
				i21 += 22;
				i22 += 23;
				i23 += 24;
				if (i0 == -99 && i15 == 24) {
					Heap_MarkStack(32, (void*)cand, 10000);
					break;
				}
			}
			if (((((((((((((((((((((((i0 + i1) + i2) + i3) + i4) + i5) + i6) + i7) + i8) + i9) + i10) + i11) + i12) + i13) + i14) + i15) + i16) + i17) + i18) + i19) + i20) + i21) + i22) + i23 > 10000) {
				return;
			}
		}
		Heap_CheckFin();
		Heap_Scan();
		Heap_Finalize();
		Heap_Unlock();
	}
}

/*----------------------------------------------------------------------------*/
void Heap_RegisterFinalizer (SYSTEM_PTR obj, Heap_Finalizer finalize)
{
	Heap_FinNode f = NIL;
	__NEW(f, Heap_FinDesc);
	f->obj = (LONGINT)obj;
	f->finalize = finalize;
	f->marked = 1;
	f->next = Heap_fin;
	Heap_fin = f;
}

/*----------------------------------------------------------------------------*/
void Heap_InitHeap (void)
{
	Heap_heap = 0;
	Heap_heapsize = 0;
	Heap_allocated = 0;
	Heap_lockdepth = 0;
	Heap_heapMin = -1;
	Heap_heapMax = 0;
	Heap_bigBlocks = 0;
	Heap_heapMinExpand = 256000;
	Heap_ldUnit = 5;
	Heap_heap = Heap_NewChunk(256000);
	__PUT(Heap_heap, 0, LONGINT);
	Heap_firstTry = 1;
	Heap_freeList[9] = 1;
	Heap_FileCount = 0;
	Heap_modules = NIL;
	Heap_fin = NIL;
	Heap_interrupted = 0;
	Heap_HeapModuleInit();
}

/*----------------------------------------------------------------------------*/
static void EnumPtrs(void (*P)(void*))
{
	P(Heap_modules);
	P(Heap_fin);
}

__TDESC(Heap_ModuleDesc__desc, 1, 2) = {__TDFLDS("ModuleDesc", 64), {0, 32, -24}};
__TDESC(Heap_CmdDesc__desc, 1, 1) = {__TDFLDS("CmdDesc", 40), {0, -16}};
__TDESC(Heap_FinDesc__desc, 1, 1) = {__TDFLDS("FinDesc", 32), {0, -16}};
__TDESC(Heap__1__desc, 1, 1) = {__TDFLDS("", 16), {8, -16}};

export void *Heap__init(void)
{
	__DEFMOD;
	__REGMOD("Heap", EnumPtrs);
	__REGCMD("FINALL", Heap_FINALL);
	__REGCMD("InitHeap", Heap_InitHeap);
	__REGCMD("Lock", Heap_Lock);
	__REGCMD("Unlock", Heap_Unlock);
	__INITYP(Heap_ModuleDesc, Heap_ModuleDesc, 0);
	__INITYP(Heap_CmdDesc, Heap_CmdDesc, 0);
	__INITYP(Heap_FinDesc, Heap_FinDesc, 0);
	__INITYP(Heap__1, Heap__1, 0);
/* BEGIN */
	__ENDMOD;
}

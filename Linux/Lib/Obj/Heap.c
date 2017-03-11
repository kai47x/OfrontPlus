/* Ofront+ 0.9 -xtspka */
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
		INTEGER obj;
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
		INTEGER types;
		Heap_EnumProc enumPtrs;
		INTEGER reserved1, reserved2;
	} Heap_ModuleDesc;


export SYSTEM_PTR Heap_modules;
static INTEGER Heap_freeList[10];
static INTEGER Heap_bigBlocks;
export INTEGER Heap_allocated;
static BOOLEAN Heap_firstTry;
static INTEGER Heap_heap, Heap_heapend;
export INTEGER Heap_heapsize;
static Heap_FinNode Heap_fin;
static INTEGER Heap_lockdepth;
static BOOLEAN Heap_interrupted;
export INTEGER Heap_FileCount;

export LONGINT *Heap_ModuleDesc__typ;
export LONGINT *Heap_CmdDesc__typ;
export LONGINT *Heap_FinDesc__typ;
export LONGINT *Heap__1__typ;

static void Heap_CheckFin (void);
static void Heap_ExtendHeap (INTEGER blksz);
export void Heap_FINALL (void);
static void Heap_Finalize (void);
export void Heap_GC (BOOLEAN markStack);
static void Heap_HeapSort (INTEGER n, INTEGER *a, INTEGER a__len);
export void Heap_INCREF (Heap_Module m);
export void Heap_InitHeap (void);
export void Heap_Lock (void);
static void Heap_Mark (INTEGER q);
static void Heap_MarkCandidates (INTEGER n, INTEGER *cand, INTEGER cand__len);
static void Heap_MarkP (SYSTEM_PTR p);
static void Heap_MarkStack (INTEGER n, INTEGER *cand, INTEGER cand__len);
export SYSTEM_PTR Heap_NEWBLK (INTEGER size);
export SYSTEM_PTR Heap_NEWREC (LONGINT tag);
static INTEGER Heap_NewChunk (INTEGER blksz);
export void Heap_REGCMD (Heap_Module m, Heap_CmdName name, Heap_Command cmd);
export SYSTEM_PTR Heap_REGMOD (Heap_ModuleName name, Heap_EnumProc enumPtrs);
export void Heap_REGTYP (Heap_Module m, LONGINT typ);
export void Heap_RegisterFinalizer (SYSTEM_PTR obj, Heap_Finalizer finalize);
static void Heap_Scan (void);
static void Heap_Sift (INTEGER l, INTEGER r, INTEGER *a, INTEGER a__len);
export void Heap_Unlock (void);

extern void *Heap__init();
extern void *SYSTEM_MainStackFrame;
#if defined __linux__ || defined __unix__
#  include <stdlib.h>
#  define OSAllocate(size) malloc((size_t)(size))
#else
#  include "_windows.h"
#  define OSAllocate(size) HeapAlloc(GetProcessHeap(), 0, (size_t)(size))
#endif
#define Heap_FetchAddress(pointer)	(INTEGER)(SYSTEM_ADR)(*((void**)((SYSTEM_ADR)pointer)))
#define Heap_Halt(code)	__HALT(code)
#define Heap_HeapModuleInit()	Heap__init()
#define Heap_MainStackFrame()	((INTEGER)(SYSTEM_ADR)SYSTEM_MainStackFrame)
#define Heap_OSAllocate(size)	((INTEGER)(SYSTEM_ADR)OSAllocate(size))

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
		Heap_Halt(-9);
	}
}

/*----------------------------------------------------------------------------*/
SYSTEM_PTR Heap_REGMOD (Heap_ModuleName name, Heap_EnumProc enumPtrs)
{
	Heap_Module m = NIL;
	if (__STRCMP(name, "Heap") == 0) {
		__SYSNEW(m, 48);
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
void Heap_REGCMD (Heap_Module m, Heap_CmdName name, Heap_Command cmd)
{
	Heap_Cmd c = NIL;
	if (__STRCMP(m->name, "Heap") == 0) {
		__SYSNEW(c, 32);
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
	__PUT((INTEGER)typ, m->types, INTEGER);
	m->types = (INTEGER)typ;
}

/*----------------------------------------------------------------------------*/
void Heap_INCREF (Heap_Module m)
{
	m->refcnt += 1;
}

/*----------------------------------------------------------------------------*/
static INTEGER Heap_NewChunk (INTEGER blksz)
{
	INTEGER chnk;
	chnk = Heap_OSAllocate(blksz + 12);
	if (chnk != 0) {
		__PUT(chnk + 4, chnk + (12 + blksz), INTEGER);
		__PUT(chnk + 12, chnk + 16, INTEGER);
		__PUT(chnk + 16, blksz, INTEGER);
		__PUT(chnk + 20, -4, INTEGER);
		__PUT(chnk + 24, Heap_bigBlocks, INTEGER);
		Heap_bigBlocks = chnk + 12;
		Heap_heapsize += blksz;
	}
	return chnk;
}

static void Heap_ExtendHeap (INTEGER blksz)
{
	INTEGER size, chnk, j, next;
	if (blksz > 160000) {
		size = blksz;
	} else {
		size = 160000;
	}
	chnk = Heap_NewChunk(size);
	if (chnk != 0) {
		if (chnk < Heap_heap) {
			__PUT(chnk, Heap_heap, INTEGER);
			Heap_heap = chnk;
		} else {
			j = Heap_heap;
			next = Heap_FetchAddress(j);
			while (next != 0 && chnk > next) {
				j = next;
				next = Heap_FetchAddress(j);
			}
			__PUT(chnk, next, INTEGER);
			__PUT(j, chnk, INTEGER);
		}
		if (next == 0) {
			Heap_heapend = Heap_FetchAddress(chnk + 4);
		}
	}
}

SYSTEM_PTR Heap_NEWREC (LONGINT tag)
{
	INTEGER i, i0, di, blksz, restsize, t, adr, end, next, prev;
	SYSTEM_PTR new = NIL;
	Heap_Lock();
	blksz = Heap_FetchAddress((INTEGER)tag);
	__ASSERT(__MASK(blksz, -16) == 0, 0);
	i0 = __ASHR(blksz, 4, INTEGER);
	i = i0;
	if (i < 9) {
		adr = Heap_freeList[__X(i, 10)];
		while (adr == 0) {
			i += 1;
			adr = Heap_freeList[__X(i, 10)];
		}
	}
	if (i < 9) {
		next = Heap_FetchAddress(adr + 12);
		Heap_freeList[__X(i, 10)] = next;
		if (i != i0) {
			di = i - i0;
			restsize = __ASHL(di, 4, INTEGER);
			end = adr + restsize;
			__PUT(end + 4, blksz, INTEGER);
			__PUT(end + 8, -4, INTEGER);
			__PUT(end, end + 4, INTEGER);
			__PUT(adr + 4, restsize, INTEGER);
			__PUT(adr + 12, Heap_freeList[__X(di, 10)], INTEGER);
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
					blksz += 16;
					if (__ASHL((Heap_heapsize - Heap_allocated) - blksz, 2, INTEGER) < Heap_heapsize) {
						Heap_ExtendHeap(__ASHL(__DIV(Heap_allocated + blksz, 48), 6, INTEGER) - Heap_heapsize);
					}
					Heap_firstTry = 0;
					new = Heap_NEWREC(tag);
					Heap_firstTry = 1;
					if (new == NIL) {
						Heap_ExtendHeap(__ASHL(__DIV(Heap_allocated + blksz, 48), 6, INTEGER) - Heap_heapsize);
						new = Heap_NEWREC(tag);
					}
					Heap_Unlock();
					return new;
				} else {
					Heap_Unlock();
					return NIL;
				}
			}
			t = Heap_FetchAddress(adr + 4);
			if (t >= blksz) {
				break;
			}
			prev = adr;
			adr = Heap_FetchAddress(adr + 12);
		}
		restsize = t - blksz;
		end = adr + restsize;
		__PUT(end + 4, blksz, INTEGER);
		__PUT(end + 8, -4, INTEGER);
		__PUT(end, end + 4, INTEGER);
		if (restsize > 144) {
			__PUT(adr + 4, restsize, INTEGER);
		} else {
			next = Heap_FetchAddress(adr + 12);
			if (prev == 0) {
				Heap_bigBlocks = next;
			} else {
				__PUT(prev + 12, next, INTEGER);
			}
			if (restsize > 0) {
				di = __ASHR(restsize, 4, INTEGER);
				__PUT(adr + 4, restsize, INTEGER);
				__PUT(adr + 12, Heap_freeList[__X(di, 10)], INTEGER);
				Heap_freeList[__X(di, 10)] = adr;
			}
		}
		adr += restsize;
	}
	i = adr + 16;
	end = adr + blksz;
	while (i < end) {
		__PUT(i, 0, INTEGER);
		__PUT(i + 4, 0, INTEGER);
		__PUT(i + 8, 0, INTEGER);
		__PUT(i + 12, 0, INTEGER);
		i += 16;
	}
	__PUT(adr + 12, 0, INTEGER);
	__PUT(adr, tag, LONGINT);
	__PUT(adr + 4, 0, INTEGER);
	__PUT(adr + 8, 0, INTEGER);
	Heap_allocated += blksz;
	Heap_Unlock();
	return (SYSTEM_PTR)(adr + 4);
}

/*----------------------------------------------------------------------------*/
SYSTEM_PTR Heap_NEWBLK (INTEGER size)
{
	INTEGER blksz, tag;
	SYSTEM_PTR new = NIL;
	Heap_Lock();
	blksz = __ASHL(__ASHR(size + 31, 4, INTEGER), 4, INTEGER);
	new = Heap_NEWREC((INTEGER)&blksz);
	tag = (__VAL(INTEGER, new) + blksz) - 12;
	__PUT(tag - 4, 0, INTEGER);
	__PUT(tag, blksz, INTEGER);
	__PUT(tag + 4, -4, INTEGER);
	__PUT(__VAL(INTEGER, new) - 4, tag, INTEGER);
	Heap_Unlock();
	return new;
}

/*----------------------------------------------------------------------------*/
static void Heap_Mark (INTEGER q)
{
	INTEGER p, tag, fld, n, offset, tagbits;
	if (q != 0) {
		tagbits = Heap_FetchAddress(q - 4);
		if (!__ODD(tagbits)) {
			__PUT(q - 4, tagbits + 1, INTEGER);
			p = 0;
			tag = tagbits + 4;
			for (;;) {
				__GET(tag, offset, INTEGER);
				if (offset < 0) {
					__PUT(q - 4, (tag + offset) + 1, INTEGER);
					if (p == 0) {
						break;
					}
					n = q;
					q = p;
					tag = Heap_FetchAddress(q - 4);
					tag -= 1;
					__GET(tag, offset, INTEGER);
					fld = q + offset;
					p = Heap_FetchAddress(fld);
					__PUT(fld, __VAL(SYSTEM_PTR, n), SYSTEM_PTR);
				} else {
					fld = q + offset;
					n = Heap_FetchAddress(fld);
					if (n != 0) {
						tagbits = Heap_FetchAddress(n - 4);
						if (!__ODD(tagbits)) {
							__PUT(n - 4, tagbits + 1, INTEGER);
							__PUT(q - 4, tag + 1, INTEGER);
							__PUT(fld, __VAL(SYSTEM_PTR, p), SYSTEM_PTR);
							p = q;
							q = n;
							tag = tagbits;
						}
					}
				}
				tag += 4;
			}
		}
	}
}

static void Heap_MarkP (SYSTEM_PTR p)
{
	Heap_Mark(__VAL(INTEGER, p));
}

static void Heap_Scan (void)
{
	INTEGER chnk, adr, end, start, tag, i, size, freesize;
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
		adr = chnk + 12;
		end = Heap_FetchAddress(chnk + 4);
		while (adr < end) {
			tag = Heap_FetchAddress(adr);
			if (__ODD(tag)) {
				if (freesize > 0) {
					start = adr - freesize;
					__PUT(start, start + 4, INTEGER);
					__PUT(start + 4, freesize, INTEGER);
					__PUT(start + 8, -4, INTEGER);
					i = __ASHR(freesize, 4, INTEGER);
					freesize = 0;
					if (i < 9) {
						__PUT(start + 12, Heap_freeList[__X(i, 10)], INTEGER);
						Heap_freeList[__X(i, 10)] = start;
					} else {
						__PUT(start + 12, Heap_bigBlocks, INTEGER);
						Heap_bigBlocks = start;
					}
				}
				tag -= 1;
				__PUT(adr, tag, INTEGER);
				size = Heap_FetchAddress(tag);
				Heap_allocated += size;
				adr += size;
			} else {
				size = Heap_FetchAddress(tag);
				freesize += size;
				adr += size;
			}
		}
		if (freesize > 0) {
			start = adr - freesize;
			__PUT(start, start + 4, INTEGER);
			__PUT(start + 4, freesize, INTEGER);
			__PUT(start + 8, -4, INTEGER);
			i = __ASHR(freesize, 4, INTEGER);
			freesize = 0;
			if (i < 9) {
				__PUT(start + 12, Heap_freeList[__X(i, 10)], INTEGER);
				Heap_freeList[__X(i, 10)] = start;
			} else {
				__PUT(start + 12, Heap_bigBlocks, INTEGER);
				Heap_bigBlocks = start;
			}
		}
		chnk = Heap_FetchAddress(chnk);
	}
}

static void Heap_Sift (INTEGER l, INTEGER r, INTEGER *a, INTEGER a__len)
{
	INTEGER i, j, x;
	j = l;
	x = a[__X(j, a__len)];
	for (;;) {
		i = j;
		j = __ASHL(j, 1, INTEGER) + 1;
		if (j < r && a[__X(j, a__len)] < a[__X(j + 1, a__len)]) {
			j += 1;
		}
		if (j > r || a[__X(j, a__len)] <= x) {
			break;
		}
		a[__X(i, a__len)] = a[__X(j, a__len)];
	}
	a[__X(i, a__len)] = x;
}

static void Heap_HeapSort (INTEGER n, INTEGER *a, INTEGER a__len)
{
	INTEGER l, r, x;
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

static void Heap_MarkCandidates (INTEGER n, INTEGER *cand, INTEGER cand__len)
{
	INTEGER chnk, adr, tag, next, lim, lim1, i, ptr, size;
	chnk = Heap_heap;
	i = 0;
	lim = cand[__X(n - 1, cand__len)];
	while (chnk != 0 && chnk < lim) {
		adr = chnk + 12;
		lim1 = Heap_FetchAddress(chnk + 4);
		if (lim < lim1) {
			lim1 = lim;
		}
		while (adr < lim1) {
			tag = Heap_FetchAddress(adr);
			if (__ODD(tag)) {
				size = Heap_FetchAddress(tag - 1);
				adr += size;
			} else {
				size = Heap_FetchAddress(tag);
				ptr = adr + 4;
				while (cand[__X(i, cand__len)] < ptr) {
					i += 1;
				}
				if (i == n) {
					return;
				}
				next = adr + size;
				if (cand[__X(i, cand__len)] < next) {
					Heap_Mark(ptr);
				}
				adr = next;
			}
		}
		chnk = Heap_FetchAddress(chnk);
	}
}

static void Heap_CheckFin (void)
{
	Heap_FinNode n = NIL;
	INTEGER tag;
	n = Heap_fin;
	while (n != NIL) {
		tag = Heap_FetchAddress(n->obj - 4);
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
			(*n->finalize)(__VAL(SYSTEM_PTR, n->obj));
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
		(*n->finalize)(__VAL(SYSTEM_PTR, n->obj));
	}
}

/*----------------------------------------------------------------------------*/
static void Heap_MarkStack (INTEGER n, INTEGER *cand, INTEGER cand__len)
{
	SYSTEM_PTR frame = NIL;
	INTEGER inc, nofcand, sp, p, stack0, ptr;
	struct Heap__1 align;
	if (n > 0) {
		Heap_MarkStack(n - 1, cand, cand__len);
		if (n > 100) {
			return;
		}
	}
	if (n == 0) {
		nofcand = 0;
		sp = (INTEGER)&frame;
		stack0 = Heap_MainStackFrame();
		inc = (INTEGER)&align.p - (INTEGER)&align;
		if (sp > stack0) {
			inc = -inc;
		}
		while (sp != stack0) {
			__GET(sp, p, INTEGER);
			if (p > Heap_heap && p < Heap_heapend) {
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
	INTEGER i0, i1, i2, i3, i4, i5, i6, i7, i8, i9, i10, i11, i12, i13, i14, i15, i16, i17, i18, i19, i20, i21, i22, i23;
	INTEGER cand[10000];
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
	f->obj = __VAL(INTEGER, obj);
	f->finalize = finalize;
	f->marked = 1;
	f->next = Heap_fin;
	Heap_fin = f;
}

/*----------------------------------------------------------------------------*/
void Heap_InitHeap (void)
{
	Heap_heap = Heap_NewChunk(128000);
	Heap_heapend = Heap_FetchAddress(Heap_heap + 4);
	__PUT(Heap_heap, 0, INTEGER);
	Heap_allocated = 0;
	Heap_firstTry = 1;
	Heap_freeList[9] = 1;
	Heap_lockdepth = 0;
	Heap_FileCount = 0;
	Heap_modules = NIL;
	Heap_heapsize = 0;
	Heap_bigBlocks = 0;
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

__TDESC(Heap_ModuleDesc__desc, 1, 2) = {__TDFLDS("ModuleDesc", 48), {0, 28, -12}};
__TDESC(Heap_CmdDesc__desc, 1, 1) = {__TDFLDS("CmdDesc", 32), {0, -8}};
__TDESC(Heap_FinDesc__desc, 1, 1) = {__TDFLDS("FinDesc", 16), {0, -8}};
__TDESC(Heap__1__desc, 1, 1) = {__TDFLDS("", 8), {4, -8}};

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

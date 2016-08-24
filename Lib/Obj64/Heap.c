/* Ofront+ 0.9 -tke */
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
		LONGINT refcnt;
		Heap_Cmd cmds;
		LONGINT types;
		Heap_EnumProc enumPtrs;
		LONGINT reserved1, reserved2;
	} Heap_ModuleDesc;


export SYSTEM_PTR Heap_modules;
static LONGINT Heap_freeList[10];
static LONGINT Heap_bigBlocks;
export LONGINT Heap_allocated;
static BOOLEAN Heap_firstTry;
static LONGINT Heap_heap, Heap_heapend;
export LONGINT Heap_heapsize;
static Heap_FinNode Heap_fin;
static INTEGER Heap_lockdepth;
static BOOLEAN Heap_interrupted;
export INTEGER Heap_FileCount;

export LONGINT *Heap_ModuleDesc__typ;
export LONGINT *Heap_CmdDesc__typ;
export LONGINT *Heap_FinDesc__typ;
export LONGINT *Heap__1__typ;

static void Heap_CheckFin (void);
static void Heap_ExtendHeap (LONGINT blksz);
export void Heap_FINALL (void);
static void Heap_Finalize (void);
export void Heap_GC (BOOLEAN markStack);
static void Heap_HeapSort (LONGINT n, LONGINT *a, INTEGER a__len);
export void Heap_INCREF (Heap_Module m);
export void Heap_InitHeap (void);
export void Heap_Lock (void);
static void Heap_Mark (LONGINT q);
static void Heap_MarkCandidates (LONGINT n, LONGINT *cand, INTEGER cand__len);
static void Heap_MarkP (SYSTEM_PTR p);
static void Heap_MarkStack (LONGINT n, LONGINT *cand, INTEGER cand__len);
export SYSTEM_PTR Heap_NEWBLK (INTEGER size);
export SYSTEM_PTR Heap_NEWREC (LONGINT tag);
static LONGINT Heap_NewChunk (LONGINT blksz);
export void Heap_REGCMD (Heap_Module m, Heap_CmdName name, Heap_Command cmd);
export SYSTEM_PTR Heap_REGMOD (Heap_ModuleName name, Heap_EnumProc enumPtrs);
export void Heap_REGTYP (Heap_Module m, LONGINT typ);
export void Heap_RegisterFinalizer (SYSTEM_PTR obj, Heap_Finalizer finalize);
static void Heap_Scan (void);
static void Heap_Sift (LONGINT l, LONGINT r, LONGINT *a, INTEGER a__len);
export void Heap_Unlock (void);

extern void *Heap__init();
extern void *SYSTEM_MainStackFrame;
extern void *Platform_OSAllocate(INTEGER size);
#define Heap_FetchAddress(pointer)	(LONGINT)(SYSTEM_ADR)(*((void**)((SYSTEM_ADR)pointer)))
#define Heap_Halt(code)	__HALT(code)
#define Heap_HeapModuleInit()	Heap__init()
#define Heap_MainStackFrame()	((LONGINT)(SYSTEM_ADR)SYSTEM_MainStackFrame)
#define Heap_OSAllocate(size)	((LONGINT)(SYSTEM_ADR)Platform_OSAllocate((SYSTEM_ADR)(size)))

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
	Heap_Module m;
	if (__STRCMP(name, "Heap") == 0) {
		__SYSNEW(m, 80);
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
	Heap_Cmd c;
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
	LONGINT chnk;
	chnk = Heap_OSAllocate(blksz + 24);
	if (chnk != 0) {
		__PUT(chnk + 8, chnk + (24 + blksz), LONGINT);
		__PUT(chnk + 24, chnk + 32, LONGINT);
		__PUT(chnk + 32, blksz, LONGINT);
		__PUT(chnk + 40, -8, LONGINT);
		__PUT(chnk + 48, Heap_bigBlocks, LONGINT);
		Heap_bigBlocks = chnk + 24;
		Heap_heapsize += blksz;
	}
	return chnk;
}

static void Heap_ExtendHeap (LONGINT blksz)
{
	LONGINT size, chnk, j, next;
	if (blksz > 320000) {
		size = blksz;
	} else {
		size = 320000;
	}
	chnk = Heap_NewChunk(size);
	if (chnk != 0) {
		if (chnk < Heap_heap) {
			__PUT(chnk, Heap_heap, LONGINT);
			Heap_heap = chnk;
		} else {
			j = Heap_heap;
			next = Heap_FetchAddress(j);
			while (next != 0 && chnk > next) {
				j = next;
				next = Heap_FetchAddress(j);
			}
			__PUT(chnk, next, LONGINT);
			__PUT(j, chnk, LONGINT);
		}
		if (next == 0) {
			Heap_heapend = Heap_FetchAddress(chnk + 8);
		}
	}
}

SYSTEM_PTR Heap_NEWREC (LONGINT tag)
{
	LONGINT i, i0, di, blksz, restsize, t, adr, end, next, prev;
	SYSTEM_PTR new;
	Heap_Lock();
	blksz = Heap_FetchAddress(tag);
		i0 = __ASHR(blksz, 5, LONGINT);
	i = i0;
	if (i < 9) {
		adr = Heap_freeList[i];
		while (adr == 0) {
			i += 1;
			adr = Heap_freeList[i];
		}
	}
	if (i < 9) {
		next = Heap_FetchAddress(adr + 24);
		Heap_freeList[i] = next;
		if (i != i0) {
			di = i - i0;
			restsize = __ASHL(di, 5, LONGINT);
			end = adr + restsize;
			__PUT(end + 8, blksz, LONGINT);
			__PUT(end + 16, -8, LONGINT);
			__PUT(end, end + 8, LONGINT);
			__PUT(adr + 8, restsize, LONGINT);
			__PUT(adr + 24, Heap_freeList[di], LONGINT);
			Heap_freeList[di] = adr;
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
					if (__ASHL((Heap_heapsize - Heap_allocated) - blksz, 2, LONGINT) < Heap_heapsize) {
						Heap_ExtendHeap(__ASHL(__DIV(Heap_allocated + blksz, 96), 7, LONGINT) - Heap_heapsize);
					}
					Heap_firstTry = 0;
					new = Heap_NEWREC(tag);
					Heap_firstTry = 1;
					if (new == NIL) {
						Heap_ExtendHeap(__ASHL(__DIV(Heap_allocated + blksz, 96), 7, LONGINT) - Heap_heapsize);
						new = Heap_NEWREC(tag);
					}
					Heap_Unlock();
					return new;
				} else {
					Heap_Unlock();
					return NIL;
				}
			}
			t = Heap_FetchAddress(adr + 8);
			if (t >= blksz) {
				break;
			}
			prev = adr;
			adr = Heap_FetchAddress(adr + 24);
		}
		restsize = t - blksz;
		end = adr + restsize;
		__PUT(end + 8, blksz, LONGINT);
		__PUT(end + 16, -8, LONGINT);
		__PUT(end, end + 8, LONGINT);
		if (restsize > 288) {
			__PUT(adr + 8, restsize, LONGINT);
		} else {
			next = Heap_FetchAddress(adr + 24);
			if (prev == 0) {
				Heap_bigBlocks = next;
			} else {
				__PUT(prev + 24, next, LONGINT);
			}
			if (restsize > 0) {
				di = __ASHR(restsize, 5, LONGINT);
				__PUT(adr + 8, restsize, LONGINT);
				__PUT(adr + 24, Heap_freeList[di], LONGINT);
				Heap_freeList[di] = adr;
			}
		}
		adr += restsize;
	}
	i = adr + 32;
	end = adr + blksz;
	while (i < end) {
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
SYSTEM_PTR Heap_NEWBLK (INTEGER size)
{
	LONGINT blksz, tag;
	SYSTEM_PTR new;
	Heap_Lock();
	blksz = __ASHL(__ASHR((LONGINT)size + 63, 5, LONGINT), 5, LONGINT);
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
	LONGINT p, tag, fld, n, offset, tagbits;
	if (q != 0) {
		tagbits = Heap_FetchAddress(q - 8);
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
					tag = Heap_FetchAddress(q - 8);
					tag -= 1;
					__GET(tag, offset, LONGINT);
					fld = q + offset;
					p = Heap_FetchAddress(fld);
					__PUT(fld, (SYSTEM_PTR)n, SYSTEM_PTR);
				} else {
					fld = q + offset;
					n = Heap_FetchAddress(fld);
					if (n != 0) {
						tagbits = Heap_FetchAddress(n - 8);
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
		Heap_freeList[i] = 0;
		i += 1;
	}
	freesize = 0;
	Heap_allocated = 0;
	chnk = Heap_heap;
	while (chnk != 0) {
		adr = chnk + 24;
		end = Heap_FetchAddress(chnk + 8);
		while (adr < end) {
			tag = Heap_FetchAddress(adr);
			if (__ODD(tag)) {
				if (freesize > 0) {
					start = adr - freesize;
					__PUT(start, start + 8, LONGINT);
					__PUT(start + 8, freesize, LONGINT);
					__PUT(start + 16, -8, LONGINT);
					i = __ASHR(freesize, 5, LONGINT);
					freesize = 0;
					if (i < 9) {
						__PUT(start + 24, Heap_freeList[i], LONGINT);
						Heap_freeList[i] = start;
					} else {
						__PUT(start + 24, Heap_bigBlocks, LONGINT);
						Heap_bigBlocks = start;
					}
				}
				tag -= 1;
				__PUT(adr, tag, LONGINT);
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
			__PUT(start, start + 8, LONGINT);
			__PUT(start + 8, freesize, LONGINT);
			__PUT(start + 16, -8, LONGINT);
			i = __ASHR(freesize, 5, LONGINT);
			freesize = 0;
			if (i < 9) {
				__PUT(start + 24, Heap_freeList[i], LONGINT);
				Heap_freeList[i] = start;
			} else {
				__PUT(start + 24, Heap_bigBlocks, LONGINT);
				Heap_bigBlocks = start;
			}
		}
		chnk = Heap_FetchAddress(chnk);
	}
}

static void Heap_Sift (LONGINT l, LONGINT r, LONGINT *a, INTEGER a__len)
{
	LONGINT i, j, x;
	j = l;
	x = a[j];
	for (;;) {
		i = j;
		j = __ASHL(j, 1, LONGINT) + 1;
		if (j < r && a[j] < a[j + 1]) {
			j += 1;
		}
		if (j > r || a[j] <= x) {
			break;
		}
		a[i] = a[j];
	}
	a[i] = x;
}

static void Heap_HeapSort (LONGINT n, LONGINT *a, INTEGER a__len)
{
	LONGINT l, r, x;
	l = __ASHR(n, 1, LONGINT);
	r = n - 1;
	while (l > 0) {
		l -= 1;
		Heap_Sift(l, r, (void*)a, a__len);
	}
	while (r > 0) {
		x = a[0];
		a[0] = a[r];
		a[r] = x;
		r -= 1;
		Heap_Sift(l, r, (void*)a, a__len);
	}
}

static void Heap_MarkCandidates (LONGINT n, LONGINT *cand, INTEGER cand__len)
{
	LONGINT chnk, adr, tag, next, lim, lim1, i, ptr, size;
	chnk = Heap_heap;
	i = 0;
	lim = cand[n - 1];
	while (chnk != 0 && chnk < lim) {
		adr = chnk + 24;
		lim1 = Heap_FetchAddress(chnk + 8);
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
				ptr = adr + 8;
				while (cand[i] < ptr) {
					i += 1;
				}
				if (i == n) {
					return;
				}
				next = adr + size;
				if (cand[i] < next) {
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
	Heap_FinNode n;
	LONGINT tag;
	n = Heap_fin;
	while (n != NIL) {
		tag = Heap_FetchAddress(n->obj - 8);
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
	Heap_FinNode n, prev;
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
	Heap_FinNode n;
	while (Heap_fin != NIL) {
		n = Heap_fin;
		Heap_fin = Heap_fin->next;
		(*n->finalize)((SYSTEM_PTR)n->obj);
	}
}

/*----------------------------------------------------------------------------*/
static void Heap_MarkStack (LONGINT n, LONGINT *cand, INTEGER cand__len)
{
	SYSTEM_PTR frame;
	LONGINT inc, nofcand, sp, p, stack0, ptr;
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
		stack0 = Heap_MainStackFrame();
		inc = (LONGINT)&align.p - (LONGINT)&align;
		if (sp > stack0) {
			inc = -inc;
		}
		while (sp != stack0) {
			__GET(sp, p, LONGINT);
			if (p > Heap_heap && p < Heap_heapend) {
				if (nofcand == (LONGINT)cand__len) {
					Heap_HeapSort(nofcand, (void*)cand, cand__len);
					Heap_MarkCandidates(nofcand, (void*)cand, cand__len);
					nofcand = 0;
				}
				cand[nofcand] = p;
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
	Heap_Module m;
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
	Heap_FinNode f;
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
	Heap_heap = Heap_NewChunk(256000);
	Heap_heapend = Heap_FetchAddress(Heap_heap + 8);
	__PUT(Heap_heap, 0, LONGINT);
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

__TDESC(Heap_ModuleDesc__desc, 1, 2) = {__TDFLDS("ModuleDesc", 80), {0, 40, -24}};
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

/* Ofront+ 0.9 -xtspkae */
#include "SYSTEM.h"

typedef
	CHAR (*Platform_ADR)[1];

typedef
	CHAR (*Platform_ArgPtr)[1024];

typedef
	Platform_ArgPtr (*Platform_ArgVec)[1024];

typedef
	CHAR (*Platform_EnvPtr)[1024];

typedef
	CHAR (*Platform_INO_T)[1];

typedef
	CHAR (*Platform_TIME_T)[1];

typedef
	struct Platform_FileIdentity {
		LONGINT volume;
		Platform_INO_T index;
		Platform_TIME_T mtime;
	} Platform_FileIdentity;


export BOOLEAN Platform_LittleEndian;
export INTEGER Platform_PID;
export CHAR Platform_CWD[256];
static INTEGER Platform_TimeStart;
export INTEGER Platform_SeekSet, Platform_SeekCur, Platform_SeekEnd;
export CHAR Platform_newLine[2];

export LONGINT *Platform_FileIdentity__typ;

export BOOLEAN Platform_Absent (INTEGER e);
export INTEGER Platform_ArgPos (CHAR *s, INTEGER s__len);
export INTEGER Platform_Chdir (CHAR *n, INTEGER n__len);
export INTEGER Platform_CloseFile (INTEGER h);
export BOOLEAN Platform_ConnectionFailed (INTEGER e);
export void Platform_Delay (INTEGER ms);
export INTEGER Platform_DeleteFile (CHAR *n, INTEGER n__len);
export BOOLEAN Platform_DifferentFilesystems (INTEGER e);
export INTEGER Platform_Error (void);
export void Platform_ExitOS (INTEGER code);
export INTEGER Platform_FileSize (INTEGER h, LONGINT *len);
export void Platform_GetArg (INTEGER n, CHAR *val, INTEGER val__len);
export void Platform_GetClock (INTEGER *t, INTEGER *d);
export void Platform_GetEnv (CHAR *var, INTEGER var__len, CHAR *val, INTEGER val__len);
export void Platform_GetIntArg (INTEGER n, INTEGER *val);
export INTEGER Platform_Identify (INTEGER h, Platform_FileIdentity *identity, LONGINT *identity__typ);
export INTEGER Platform_IdentifyByName (CHAR *n, INTEGER n__len, Platform_FileIdentity *identity, LONGINT *identity__typ);
export BOOLEAN Platform_Inaccessible (INTEGER e);
export void Platform_MTimeAsClock (Platform_FileIdentity i, INTEGER *t, INTEGER *d);
export INTEGER Platform_NewFile (CHAR *n, INTEGER n__len, INTEGER *h);
export BOOLEAN Platform_NoSuchDirectory (INTEGER e);
export Platform_ADR Platform_OSAllocate (INTEGER size);
export void Platform_OSFree (Platform_ADR address);
export INTEGER Platform_OldRO (CHAR *n, INTEGER n__len, INTEGER *h);
export INTEGER Platform_OldRW (CHAR *n, INTEGER n__len, INTEGER *h);
export INTEGER Platform_Read (INTEGER h, Platform_ADR p, INTEGER l, INTEGER *n);
export INTEGER Platform_ReadBuf (INTEGER h, BYTE *b, INTEGER b__len, INTEGER *n);
export INTEGER Platform_RenameFile (CHAR *o, INTEGER o__len, CHAR *n, INTEGER n__len);
export BOOLEAN Platform_SameFile (Platform_FileIdentity i1, Platform_FileIdentity i2);
export BOOLEAN Platform_SameFileTime (Platform_FileIdentity i1, Platform_FileIdentity i2);
export INTEGER Platform_Seek (INTEGER h, LONGINT offset, INTEGER whence);
export void Platform_SetMTime (Platform_FileIdentity *target, LONGINT *target__typ, Platform_FileIdentity source);
export INTEGER Platform_Sync (INTEGER h);
export INTEGER Platform_System (CHAR *cmd, INTEGER cmd__len);
static void Platform_TestLittleEndian (void);
export INTEGER Platform_Time (void);
export BOOLEAN Platform_TimedOut (INTEGER e);
export BOOLEAN Platform_TooManyFiles (INTEGER e);
export INTEGER Platform_TruncateFile (INTEGER h, INTEGER l);
export INTEGER Platform_Write (INTEGER h, Platform_ADR p, INTEGER l);
static void Platform_YMDHMStoClock (INTEGER ye, INTEGER mo, INTEGER da, INTEGER ho, INTEGER mi, INTEGER se, INTEGER *t, INTEGER *d);
export BOOLEAN Platform_getEnv (CHAR *var, INTEGER var__len, CHAR *val, INTEGER val__len);

extern INTEGER SYSTEM_ArgCount;
extern void *SYSTEM_ArgVector;
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#define Platform_ArgCount()	SYSTEM_ArgCount
#define Platform_ArgVector()	(Platform_ArgVec)SYSTEM_ArgVector
#define Platform_EACCES()	EACCES
#define Platform_EAGAIN()	EAGAIN
#define Platform_ECONNABORTED()	ECONNABORTED
#define Platform_ECONNREFUSED()	ECONNREFUSED
#define Platform_EHOSTUNREACH()	EHOSTUNREACH
#define Platform_EMFILE()	EMFILE
#define Platform_ENETUNREACH()	ENETUNREACH
#define Platform_ENFILE()	ENFILE
#define Platform_ENOENT()	ENOENT
#define Platform_EROFS()	EROFS
#define Platform_ETIMEDOUT()	ETIMEDOUT
#define Platform_EXDEV()	EXDEV
#define Platform_InvalidHandleValue()	(-1)
#define Platform_SystemHalt(code)	__HALT(code)
#define Platform_ToBYTE(s)	((BYTE)(s))
#define Platform_ToINT(l)	((INTEGER)(l))
#define Platform_ToSHORT(i)	((SHORTINT)(i))
#define Platform_UBYTE(b)	((SHORTINT)(unsigned char)(b))
#define Platform_UINT(i)	((LONGINT)(unsigned int)(i))
#define Platform_USHORT(s)	((INTEGER)(unsigned short)(s))
#define Platform_allocate(size)	((Platform_ADR)malloc((size_t)(size)))
#define Platform_chdir(n, n__len)	chdir((char*)n)
#define Platform_closefile(fd)	close(fd)
#define Platform_err()	errno
#define Platform_exit(code)	exit(code)
#define Platform_free(address)	free((void*)(address))
#define Platform_fstat(fd)	fstat(fd, &s)
#define Platform_fsync(fd)	fsync(fd)
#define Platform_ftruncate(fd, l)	ftruncate(fd, l)
#define Platform_getcwd(cwd, cwd__len)	getcwd((char*)cwd, cwd__len)
#define Platform_getenv(var, var__len)	(Platform_EnvPtr)getenv((char*)var)
#define Platform_getpid()	(INTEGER)getpid()
#define Platform_gettimeval()	struct timeval tv; gettimeofday(&tv, 0)
#define Platform_lseek(fd, offset, whence)	lseek(fd, offset, whence)
#define Platform_nanosleep(s, ns)	struct timespec req, rem; req.tv_sec = (time_t)s; req.tv_nsec = ns; nanosleep(&req, &rem)
#define Platform_opennew(n, n__len)	open((char*)n, O_CREAT | O_TRUNC | O_RDWR, 0664)
#define Platform_openro(n, n__len)	open((char*)n, O_RDONLY)
#define Platform_openrw(n, n__len)	open((char*)n, O_RDWR)
#define Platform_readfile(fd, p, l)	read(fd, (void*)(p), l)
#define Platform_rename(o, o__len, n, n__len)	rename((char*)o, (char*)n)
#define Platform_sectotm(s)	struct tm *time = localtime((time_t*)&s)
#define Platform_seekcur()	SEEK_CUR
#define Platform_seekend()	SEEK_END
#define Platform_seekset()	SEEK_SET
#define Platform_stat(n, n__len)	stat((char*)n, &s)
#define Platform_statdev()	(LONGINT)s.st_dev
#define Platform_statino()	(Platform_INO_T)s.st_ino
#define Platform_statmtime()	(Platform_TIME_T)s.st_mtime
#define Platform_statsize()	(LONGINT)s.st_size
#define Platform_structstats()	struct stat s
#define Platform_system(str, str__len)	system((char*)str)
#define Platform_timeint(time)	((INTEGER)(time))
#define Platform_tmhour()	time->tm_hour
#define Platform_tmmday()	time->tm_mday
#define Platform_tmmin()	time->tm_min
#define Platform_tmmon()	time->tm_mon
#define Platform_tmsec()	time->tm_sec
#define Platform_tmyear()	time->tm_year
#define Platform_tvsec()	tv.tv_sec
#define Platform_tvusec()	(INTEGER)tv.tv_usec
#define Platform_unlink(n, n__len)	unlink((char*)n)
#define Platform_writefile(fd, p, l)	write(fd, (void*)(p), l)

/*============================================================================*/

BOOLEAN Platform_TooManyFiles (INTEGER e)
{
	return e == Platform_EMFILE() || e == Platform_ENFILE();
}

/*----------------------------------------------------------------------------*/
BOOLEAN Platform_NoSuchDirectory (INTEGER e)
{
	return e == Platform_ENOENT();
}

/*----------------------------------------------------------------------------*/
BOOLEAN Platform_DifferentFilesystems (INTEGER e)
{
	return e == Platform_EXDEV();
}

/*----------------------------------------------------------------------------*/
BOOLEAN Platform_Inaccessible (INTEGER e)
{
	return (e == Platform_EACCES() || e == Platform_EROFS()) || e == Platform_EAGAIN();
}

/*----------------------------------------------------------------------------*/
BOOLEAN Platform_Absent (INTEGER e)
{
	return e == Platform_ENOENT();
}

/*----------------------------------------------------------------------------*/
BOOLEAN Platform_TimedOut (INTEGER e)
{
	return e == Platform_ETIMEDOUT();
}

/*----------------------------------------------------------------------------*/
BOOLEAN Platform_ConnectionFailed (INTEGER e)
{
	return ((e == Platform_ECONNREFUSED() || e == Platform_ECONNABORTED()) || e == Platform_ENETUNREACH()) || e == Platform_EHOSTUNREACH();
}

/*----------------------------------------------------------------------------*/
Platform_ADR Platform_OSAllocate (INTEGER size)
{
	if (size > 0) {
		return Platform_allocate(size);
	}
	Platform_SystemHalt(-25);
	return NIL;
}

/*----------------------------------------------------------------------------*/
void Platform_OSFree (Platform_ADR address)
{
	Platform_free(address);
}

/*----------------------------------------------------------------------------*/
BOOLEAN Platform_getEnv (CHAR *var, INTEGER var__len, CHAR *val, INTEGER val__len)
{
	Platform_EnvPtr p = NIL;
	__DUP(var, var__len);
	p = Platform_getenv(var, var__len);
	if (p != NIL) {
		__COPY(*p, val, val__len);
	}
	__DEL(var);
	return p != NIL;
}

/*----------------------------------------------------------------------------*/
void Platform_GetEnv (CHAR *var, INTEGER var__len, CHAR *val, INTEGER val__len)
{
	__DUP(var, var__len);
	if (!Platform_getEnv(var, var__len, (void*)val, val__len)) {
		val[0] = 0x00;
	}
	__DEL(var);
}

/*----------------------------------------------------------------------------*/
void Platform_GetArg (INTEGER n, CHAR *val, INTEGER val__len)
{
	Platform_ArgVec av = NIL;
	if (n < Platform_ArgCount()) {
		av = Platform_ArgVector();
		__COPY(*(*av)[__X(n, 1024)], val, val__len);
	}
}

/*----------------------------------------------------------------------------*/
void Platform_GetIntArg (INTEGER n, INTEGER *val)
{
	CHAR s[64];
	INTEGER k, d, i;
	s[0] = 0x00;
	Platform_GetArg(n, (void*)s, 64);
	i = 0;
	if (s[0] == '-') {
		i = 1;
	}
	k = 0;
	d = (SHORTINT)s[__X(i, 64)] - 48;
	while (d >= 0 && d <= 9) {
		k = k * 10 + d;
		i += 1;
		d = (SHORTINT)s[__X(i, 64)] - 48;
	}
	if (s[0] == '-') {
		k = -k;
		i -= 1;
	}
	if (i > 0) {
		*val = k;
	}
}

/*----------------------------------------------------------------------------*/
INTEGER Platform_ArgPos (CHAR *s, INTEGER s__len)
{
	INTEGER i;
	CHAR arg[256];
	__DUP(s, s__len);
	i = 0;
	Platform_GetArg(i, (void*)arg, 256);
	while (i < Platform_ArgCount() && __STRCMP(s, arg) != 0) {
		i += 1;
		Platform_GetArg(i, (void*)arg, 256);
	}
	__DEL(s);
	return i;
}

/*----------------------------------------------------------------------------*/
static void Platform_YMDHMStoClock (INTEGER ye, INTEGER mo, INTEGER da, INTEGER ho, INTEGER mi, INTEGER se, INTEGER *t, INTEGER *d)
{
	*d = (__ASHL((INTEGER)__MOD(ye, 100), 9, INTEGER) + __ASHL(mo + 1, 5, INTEGER)) + da;
	*t = (__ASHL(ho, 12, INTEGER) + __ASHL(mi, 6, INTEGER)) + se;
}

void Platform_GetClock (INTEGER *t, INTEGER *d)
{
	Platform_gettimeval();
	Platform_sectotm(Platform_tvsec());
	Platform_YMDHMStoClock(Platform_tmyear(), Platform_tmmon(), Platform_tmmday(), Platform_tmhour(), Platform_tmmin(), Platform_tmsec(), &*t, &*d);
}

/*----------------------------------------------------------------------------*/
INTEGER Platform_Time (void)
{
	INTEGER ms;
	Platform_gettimeval();
	ms = (INTEGER)__DIVF(Platform_tvusec(), 1000) + Platform_timeint(Platform_tvsec()) * 1000;
	return (INTEGER)__MOD(ms - Platform_TimeStart, 2147483647);
}

/*----------------------------------------------------------------------------*/
void Platform_Delay (INTEGER ms)
{
	INTEGER s, ns;
	s = __DIV(ms, 1000);
	ns = (INTEGER)__MOD(ms, 1000) * 1000000;
	Platform_nanosleep(s, ns);
}

/*----------------------------------------------------------------------------*/
INTEGER Platform_System (CHAR *cmd, INTEGER cmd__len)
{
	__DUP(cmd, cmd__len);
	__DEL(cmd);
	return Platform_system(cmd, cmd__len);
}

/*----------------------------------------------------------------------------*/
INTEGER Platform_Error (void)
{
	return Platform_err();
}

/*----------------------------------------------------------------------------*/
INTEGER Platform_OldRO (CHAR *n, INTEGER n__len, INTEGER *h)
{
	INTEGER fd;
	fd = Platform_openro(n, n__len);
	if (fd == Platform_InvalidHandleValue()) {
		return Platform_err();
	} else {
		*h = fd;
		return 0;
	}
	__RETCHK;
}

/*----------------------------------------------------------------------------*/
INTEGER Platform_OldRW (CHAR *n, INTEGER n__len, INTEGER *h)
{
	INTEGER fd;
	fd = Platform_openrw(n, n__len);
	if (fd == Platform_InvalidHandleValue()) {
		return Platform_err();
	} else {
		*h = fd;
		return 0;
	}
	__RETCHK;
}

/*----------------------------------------------------------------------------*/
INTEGER Platform_NewFile (CHAR *n, INTEGER n__len, INTEGER *h)
{
	INTEGER fd;
	fd = Platform_opennew(n, n__len);
	if (fd == Platform_InvalidHandleValue()) {
		return Platform_err();
	} else {
		*h = fd;
		return 0;
	}
	__RETCHK;
}

/*----------------------------------------------------------------------------*/
INTEGER Platform_CloseFile (INTEGER h)
{
	if (Platform_closefile(h) < 0) {
		return Platform_err();
	} else {
		return 0;
	}
	__RETCHK;
}

/*----------------------------------------------------------------------------*/
INTEGER Platform_Identify (INTEGER h, Platform_FileIdentity *identity, LONGINT *identity__typ)
{
	Platform_structstats();
	if (Platform_fstat(h) < 0) {
		return Platform_err();
	}
	(*identity).volume = Platform_statdev();
	(*identity).index = Platform_statino();
	(*identity).mtime = Platform_statmtime();
	return 0;
}

/*----------------------------------------------------------------------------*/
INTEGER Platform_IdentifyByName (CHAR *n, INTEGER n__len, Platform_FileIdentity *identity, LONGINT *identity__typ)
{
	__DUP(n, n__len);
	Platform_structstats();
	if (Platform_stat(n, n__len) < 0) {
		__DEL(n);
		return Platform_err();
	}
	(*identity).volume = Platform_statdev();
	(*identity).index = Platform_statino();
	(*identity).mtime = Platform_statmtime();
	__DEL(n);
	return 0;
}

/*----------------------------------------------------------------------------*/
BOOLEAN Platform_SameFile (Platform_FileIdentity i1, Platform_FileIdentity i2)
{
	return i1.index == i2.index && i1.volume == i2.volume;
}

/*----------------------------------------------------------------------------*/
BOOLEAN Platform_SameFileTime (Platform_FileIdentity i1, Platform_FileIdentity i2)
{
	return i1.mtime == i2.mtime;
}

/*----------------------------------------------------------------------------*/
void Platform_SetMTime (Platform_FileIdentity *target, LONGINT *target__typ, Platform_FileIdentity source)
{
	(*target).mtime = source.mtime;
}

/*----------------------------------------------------------------------------*/
void Platform_MTimeAsClock (Platform_FileIdentity i, INTEGER *t, INTEGER *d)
{
	Platform_sectotm(i.mtime);
	Platform_YMDHMStoClock(Platform_tmyear(), Platform_tmmon(), Platform_tmmday(), Platform_tmhour(), Platform_tmmin(), Platform_tmsec(), &*t, &*d);
}

/*----------------------------------------------------------------------------*/
INTEGER Platform_FileSize (INTEGER h, LONGINT *len)
{
	Platform_structstats();
	if (Platform_fstat(h) < 0) {
		return Platform_err();
	}
	*len = Platform_statsize();
	return 0;
}

/*----------------------------------------------------------------------------*/
INTEGER Platform_Read (INTEGER h, Platform_ADR p, INTEGER l, INTEGER *n)
{
	*n = Platform_readfile(h, p, l);
	if (*n < 0) {
		*n = 0;
		return Platform_err();
	} else {
		return 0;
	}
	__RETCHK;
}

/*----------------------------------------------------------------------------*/
INTEGER Platform_ReadBuf (INTEGER h, BYTE *b, INTEGER b__len, INTEGER *n)
{
	*n = Platform_readfile(h, (Platform_ADR)((INTEGER)b), b__len);
	if (*n < 0) {
		*n = 0;
		return Platform_err();
	} else {
		return 0;
	}
	__RETCHK;
}

/*----------------------------------------------------------------------------*/
INTEGER Platform_Write (INTEGER h, Platform_ADR p, INTEGER l)
{
	INTEGER written;
	written = Platform_writefile(h, p, l);
	if (written < 0) {
		return Platform_err();
	} else {
		return 0;
	}
	__RETCHK;
}

/*----------------------------------------------------------------------------*/
INTEGER Platform_Sync (INTEGER h)
{
	if (Platform_fsync(h) < 0) {
		return Platform_err();
	} else {
		return 0;
	}
	__RETCHK;
}

/*----------------------------------------------------------------------------*/
INTEGER Platform_Seek (INTEGER h, LONGINT offset, INTEGER whence)
{
	if (Platform_lseek(h, offset, whence) < 0) {
		return Platform_err();
	} else {
		return 0;
	}
	__RETCHK;
}

/*----------------------------------------------------------------------------*/
INTEGER Platform_TruncateFile (INTEGER h, INTEGER l)
{
	if (Platform_ftruncate(h, l) < 0) {
		return Platform_err();
	} else {
		return 0;
	}
	__RETCHK;
}

/*----------------------------------------------------------------------------*/
INTEGER Platform_DeleteFile (CHAR *n, INTEGER n__len)
{
	if (Platform_unlink(n, n__len) < 0) {
		return Platform_err();
	} else {
		return 0;
	}
	__RETCHK;
}

/*----------------------------------------------------------------------------*/
INTEGER Platform_Chdir (CHAR *n, INTEGER n__len)
{
	INTEGER r;
	r = Platform_chdir(n, n__len);
	Platform_getcwd((void*)Platform_CWD, 256);
	if (r < 0) {
		return Platform_err();
	} else {
		return 0;
	}
	__RETCHK;
}

/*----------------------------------------------------------------------------*/
INTEGER Platform_RenameFile (CHAR *o, INTEGER o__len, CHAR *n, INTEGER n__len)
{
	if (Platform_rename(o, o__len, n, n__len) < 0) {
		return Platform_err();
	} else {
		return 0;
	}
	__RETCHK;
}

/*----------------------------------------------------------------------------*/
void Platform_ExitOS (INTEGER code)
{
	Platform_exit(code);
}

/*----------------------------------------------------------------------------*/
static void Platform_TestLittleEndian (void)
{
	INTEGER i;
	i = 1;
	__GET((INTEGER)&i, Platform_LittleEndian, BOOLEAN);
}

__TDESC(Platform_FileIdentity__desc, 1, 0) = {__TDFLDS("FileIdentity", 12), {-4}};

export void *Platform__init(void)
{
	__DEFMOD;
	__REGMOD("Platform", 0);
	__INITYP(Platform_FileIdentity, Platform_FileIdentity, 0);
/* BEGIN */
	Platform_TestLittleEndian();
	Platform_TimeStart = 0;
	Platform_TimeStart = Platform_Time();
	Platform_CWD[0] = 0x00;
	Platform_getcwd((void*)Platform_CWD, 256);
	Platform_PID = Platform_getpid();
	Platform_SeekSet = Platform_seekset();
	Platform_SeekCur = Platform_seekcur();
	Platform_SeekEnd = Platform_seekend();
	Platform_newLine[0] = 0x0a;
	Platform_newLine[1] = 0x00;
	__ENDMOD;
}

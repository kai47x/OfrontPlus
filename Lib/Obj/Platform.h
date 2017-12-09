/* Ofront+ 1.0 -sxtapC -48 */

#ifndef Platform__h
#define Platform__h

#include "SYSTEM.h"

typedef
	CHAR (*Platform_ArgPtr)[1024];

typedef
	Platform_ArgPtr (*Platform_ArgVec)[1024];

typedef
	struct Platform_FileIdentity {
		INTEGER _prvt0;
		char _prvt1[16];
	} Platform_FileIdentity;


import BOOLEAN Platform_LittleEndian;
import INTEGER Platform_PID;
import CHAR Platform_CWD[4096];
import INTEGER Platform_SeekSet, Platform_SeekCur, Platform_SeekEnd, Platform_StdIn, Platform_StdOut, Platform_StdErr;
import CHAR Platform_newLine[3];

import SYSTEM_ADRINT *Platform_FileIdentity__typ;

import BOOLEAN Platform_Absent (INTEGER e);
import INTEGER Platform_ArgPos (CHAR *s, INTEGER s__len);
import INTEGER Platform_Chdir (CHAR *n, INTEGER n__len);
import INTEGER Platform_CloseFile (INTEGER h);
import BOOLEAN Platform_ConnectionFailed (INTEGER e);
import void Platform_Delay (INTEGER ms);
import INTEGER Platform_DeleteFile (CHAR *n, INTEGER n__len);
import BOOLEAN Platform_DifferentFilesystems (INTEGER e);
import BOOLEAN Platform_DirExists (CHAR *name, INTEGER name__len);
import INTEGER Platform_Error (void);
import void Platform_ExitOS (INTEGER code);
import BOOLEAN Platform_FileExists (CHAR *name, INTEGER name__len);
import INTEGER Platform_FileSize (INTEGER h, LONGINT *len);
import void Platform_GetArg (INTEGER n, CHAR *val, INTEGER val__len);
import void Platform_GetClock (INTEGER *t, INTEGER *d);
import void Platform_GetEnv (CHAR *var, INTEGER var__len, CHAR *val, INTEGER val__len);
import void Platform_GetIntArg (INTEGER n, INTEGER *val);
import INTEGER Platform_Identify (INTEGER h, Platform_FileIdentity *identity, SYSTEM_ADRINT *identity__typ);
import INTEGER Platform_IdentifyByName (CHAR *n, INTEGER n__len, Platform_FileIdentity *identity, SYSTEM_ADRINT *identity__typ);
import BOOLEAN Platform_Inaccessible (INTEGER e);
import BOOLEAN Platform_IsConsole (INTEGER h);
import void Platform_MTimeAsClock (Platform_FileIdentity i, INTEGER *t, INTEGER *d);
import INTEGER Platform_NewFile (CHAR *n, INTEGER n__len, INTEGER *h);
import BOOLEAN Platform_NoSuchDirectory (INTEGER e);
import INTEGER Platform_OSAllocate (INTEGER size);
import void Platform_OSFree (INTEGER address);
import INTEGER Platform_OldRO (CHAR *n, INTEGER n__len, INTEGER *h);
import INTEGER Platform_OldRW (CHAR *n, INTEGER n__len, INTEGER *h);
import INTEGER Platform_Read (INTEGER h, INTEGER p, INTEGER l, INTEGER *n);
import INTEGER Platform_ReadBuf (INTEGER h, BYTE *b, INTEGER b__len, INTEGER *n);
import INTEGER Platform_RenameFile (CHAR *src, INTEGER src__len, CHAR *dest, INTEGER dest__len);
import BOOLEAN Platform_SameFile (Platform_FileIdentity i1, Platform_FileIdentity i2);
import BOOLEAN Platform_SameFileTime (Platform_FileIdentity i1, Platform_FileIdentity i2);
import INTEGER Platform_Seek (INTEGER h, LONGINT offset, INTEGER r);
import void Platform_SetMTime (Platform_FileIdentity *target, SYSTEM_ADRINT *target__typ, Platform_FileIdentity source);
import INTEGER Platform_Sync (INTEGER h);
import INTEGER Platform_System (CHAR *cmd, INTEGER cmd__len);
import INTEGER Platform_Time (void);
import BOOLEAN Platform_TimedOut (INTEGER e);
import BOOLEAN Platform_TooManyFiles (INTEGER e);
import INTEGER Platform_TruncateFile (INTEGER h, LONGINT limit);
import INTEGER Platform_Write (INTEGER h, INTEGER p, INTEGER l);
import BOOLEAN Platform_getEnv (CHAR *var, INTEGER var__len, CHAR *val, INTEGER val__len);
import void *Platform__init(void);

#define Platform_InvalidHandleValue()	(-1)
#define Platform_ToBYTE(s)	((BYTE)(s))
#define Platform_ToINT(l)	((INTEGER)(l))
#define Platform_ToSHORT(i)	((SHORTINT)(i))
#define Platform_UBYTE(b)	((SHORTINT)(unsigned char)(b))
#define Platform_UINT(i)	((LONGINT)(unsigned int)(i))
#define Platform_USHORT(s)	((INTEGER)(unsigned short)(s))

#endif

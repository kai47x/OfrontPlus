/* Ofront+ 0.9 -s */

#ifndef WinApi__h
#define WinApi__h

#include "SYSTEM.h"

typedef
	struct WinApi_FILETIME {
		INTEGER dwLowDateTime, dwHighDateTime;
	} WinApi_FILETIME;

typedef
	struct WinApi_BY_HANDLE_FILE_INFORMATION {
		SET dwFileAttributes;
		WinApi_FILETIME ftCreationTime, ftLastAccessTime, ftLastWriteTime;
		INTEGER dwVolumeSerialNumber, nFileSizeHigh, nFileSizeLow, nNumberOfLinks, nFileIndexHigh, nFileIndexLow;
	} WinApi_BY_HANDLE_FILE_INFORMATION;

typedef
	CHAR (*WinApi_PtrVoid)[1];

typedef
	CHAR (*WinApi_PtrSTR)[1];

typedef
	struct WinApi_SECURITY_ATTRIBUTES {
		INTEGER nLength;
		WinApi_PtrVoid lpSecurityDescriptor;
		INTEGER bInheritHandle;
	} WinApi_SECURITY_ATTRIBUTES;



import SYSTEM_ADRINT *WinApi_SECURITY_ATTRIBUTES__typ;
import SYSTEM_ADRINT *WinApi_FILETIME__typ;
import SYSTEM_ADRINT *WinApi_BY_HANDLE_FILE_INFORMATION__typ;

import void *WinApi__init(void);

#define WinApi_CloseHandle(hObject)	CloseHandle(hObject)
__EXTERN INTEGER __CALL_1 CloseHandle(WinApi_PtrVoid hObject);
#define WinApi_CreateFileA(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, lpSecurityAttributes__typ, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile)	CreateFileA(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile)
__EXTERN WinApi_PtrVoid __CALL_1 CreateFileA(WinApi_PtrSTR lpFileName, SET dwDesiredAccess, SET dwShareMode, WinApi_SECURITY_ATTRIBUTES *lpSecurityAttributes, INTEGER dwCreationDisposition, SET dwFlagsAndAttributes, WinApi_PtrVoid hTemplateFile);
#define WinApi_GetFileInformationByHandle(hFile, lpFileInformation, lpFileInformation__typ)	GetFileInformationByHandle(hFile, lpFileInformation)
__EXTERN INTEGER __CALL_1 GetFileInformationByHandle(WinApi_PtrVoid hFile, WinApi_BY_HANDLE_FILE_INFORMATION *lpFileInformation);
#define WinApi__init()	/*-noinit*/

#endif

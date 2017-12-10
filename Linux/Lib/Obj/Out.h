/* Ofront+ 1.0 -sC -44 */

#ifndef Out__h
#define Out__h

#include "SYSTEM.h"


import BOOLEAN Out_IsConsole;


import void Out_Char (CHAR ch);
import void Out_ConvertL (LONGREAL x, SHORTINT n, CHAR *d, INTEGER d__len);
import SHORTINT Out_Expo (REAL x);
import void Out_Flush (void);
import void Out_Hex (LONGINT x, LONGINT n);
import void Out_Int (LONGINT x, LONGINT n);
import void Out_Ln (void);
import void Out_LongReal (LONGREAL x, SHORTINT n);
import void Out_Open (void);
import void Out_Real (REAL x, SHORTINT n);
import void Out_RealFix (REAL x, SHORTINT n, SHORTINT k);
import void Out_String (CHAR *str, INTEGER str__len);
import LONGREAL Out_Ten (SHORTINT e);
import void *Out__init(void);


#endif

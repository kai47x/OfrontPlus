/* Ofront+ 1.0 -e */

#ifndef OfrontOPS__h
#define OfrontOPS__h

#include "SYSTEM.h"

typedef
	CHAR OfrontOPS_Name[40];

typedef
	CHAR OfrontOPS_String[256];


import OfrontOPS_Name OfrontOPS_name;
import OfrontOPS_String OfrontOPS_str;
import SHORTINT OfrontOPS_numtyp;
import LONGINT OfrontOPS_intval;
import REAL OfrontOPS_realval;
import LONGREAL OfrontOPS_lrlval;


import void OfrontOPS_Get (BYTE *sym);
import void OfrontOPS_Init (void);
import void *OfrontOPS__init(void);


#endif

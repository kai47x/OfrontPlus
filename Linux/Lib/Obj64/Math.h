/* Ofront+ 1.0 -sC -88 */

#ifndef Math__h
#define Math__h

#include "SYSTEM.h"


import void (*Math_ErrorHandler)(SHORTINT);
import SHORTINT Math_err;


import void Math_ClearError (void);
import BOOLEAN Math_IsRMathException (void);
import REAL Math_arccos (REAL x);
import REAL Math_arccosh (REAL x);
import REAL Math_arcsin (REAL x);
import REAL Math_arcsinh (REAL x);
import REAL Math_arctan (REAL x);
import REAL Math_arctan2 (REAL xn, REAL xd);
import REAL Math_arctanh (REAL x);
import REAL Math_cos (REAL x);
import REAL Math_cosh (REAL x);
import REAL Math_exp (REAL x);
import SHORTINT Math_exponent (REAL x);
import REAL Math_fraction (REAL x);
import REAL Math_ipower (REAL x, SHORTINT base);
import REAL Math_ln (REAL x);
import REAL Math_log (REAL x, REAL base);
import REAL Math_power (REAL base, REAL exp);
import REAL Math_pred (REAL x);
import INTEGER Math_round (REAL x);
import REAL Math_scale (REAL x, SHORTINT n);
import REAL Math_sign (REAL x);
import REAL Math_sin (REAL x);
import void Math_sincos (REAL x, REAL *Sin, REAL *Cos);
import REAL Math_sinh (REAL x);
import REAL Math_sqrt (REAL x);
import REAL Math_succ (REAL x);
import REAL Math_tan (REAL x);
import REAL Math_tanh (REAL x);
import REAL Math_ulp (REAL x);
import void *Math__init(void);


#endif

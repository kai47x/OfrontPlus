/* Ofront+ 0.9 - */
#include "SYSTEM.h"




export void Strings_Append (CHAR *extra, INTEGER extra__len, CHAR *dest, INTEGER dest__len);
export void Strings_Cap (CHAR *s, INTEGER s__len);
export void Strings_Delete (CHAR *s, INTEGER s__len, INTEGER pos, INTEGER n);
export void Strings_Extract (CHAR *source, INTEGER source__len, INTEGER pos, INTEGER n, CHAR *dest, INTEGER dest__len);
export void Strings_Insert (CHAR *source, INTEGER source__len, INTEGER pos, CHAR *dest, INTEGER dest__len);
export INTEGER Strings_Length (CHAR *s, INTEGER s__len);
export BOOLEAN Strings_Match (CHAR *string, INTEGER string__len, CHAR *pattern, INTEGER pattern__len);
export INTEGER Strings_Pos (CHAR *pattern, INTEGER pattern__len, CHAR *s, INTEGER s__len, INTEGER pos);
export void Strings_Replace (CHAR *source, INTEGER source__len, INTEGER pos, CHAR *dest, INTEGER dest__len);


/*============================================================================*/

INTEGER Strings_Length (CHAR *s, INTEGER s__len)
{
	INTEGER i;
	__DUP(s, s__len);
	i = 0;
	while (i < s__len && s[__X(i, s__len)] != 0x00) {
		i += 1;
	}
	__DEL(s);
	return i;
}

/*----------------------------------------------------------------------------*/
void Strings_Append (CHAR *extra, INTEGER extra__len, CHAR *dest, INTEGER dest__len)
{
	INTEGER n1, n2, i;
	__DUP(extra, extra__len);
	n1 = Strings_Length(dest, dest__len);
	n2 = Strings_Length(extra, extra__len);
	i = 0;
	while (i < n2 && i + n1 < dest__len) {
		dest[__X(i + n1, dest__len)] = extra[__X(i, extra__len)];
		i += 1;
	}
	if (i + n1 < dest__len) {
		dest[__X(i + n1, dest__len)] = 0x00;
	}
	__DEL(extra);
}

/*----------------------------------------------------------------------------*/
void Strings_Insert (CHAR *source, INTEGER source__len, INTEGER pos, CHAR *dest, INTEGER dest__len)
{
	INTEGER n1, n2, i;
	__DUP(source, source__len);
	n1 = Strings_Length(dest, dest__len);
	n2 = Strings_Length(source, source__len);
	if (pos < 0) {
		pos = 0;
	}
	if (pos > n1) {
		Strings_Append(dest, dest__len, (void*)source, source__len);
		__DEL(source);
		return;
	}
	if (pos + n2 < dest__len) {
		i = n1;
		while (i >= pos) {
			if (i + n2 < dest__len) {
				dest[__X(i + n2, dest__len)] = dest[__X(i, dest__len)];
			}
			i -= 1;
		}
	}
	i = 0;
	while (i < n2) {
		dest[__X(pos + i, dest__len)] = source[__X(i, source__len)];
		i += 1;
	}
	__DEL(source);
}

/*----------------------------------------------------------------------------*/
void Strings_Delete (CHAR *s, INTEGER s__len, INTEGER pos, INTEGER n)
{
	INTEGER len, i;
	len = Strings_Length(s, s__len);
	if (pos < 0) {
		pos = 0;
	} else if (pos >= len) {
		return;
	}
	if (pos + n < len) {
		i = pos + n;
		while (i < len) {
			s[__X(i - n, s__len)] = s[__X(i, s__len)];
			i += 1;
		}
		if (i - n < s__len) {
			s[__X(i - n, s__len)] = 0x00;
		}
	} else {
		s[__X(pos, s__len)] = 0x00;
	}
}

/*----------------------------------------------------------------------------*/
void Strings_Replace (CHAR *source, INTEGER source__len, INTEGER pos, CHAR *dest, INTEGER dest__len)
{
	__DUP(source, source__len);
	Strings_Delete((void*)dest, dest__len, pos, pos + Strings_Length(source, source__len));
	Strings_Insert(source, source__len, pos, (void*)dest, dest__len);
	__DEL(source);
}

/*----------------------------------------------------------------------------*/
void Strings_Extract (CHAR *source, INTEGER source__len, INTEGER pos, INTEGER n, CHAR *dest, INTEGER dest__len)
{
	INTEGER len, destLen, i;
	__DUP(source, source__len);
	len = Strings_Length(source, source__len);
	destLen = dest__len - 1;
	if (pos < 0) {
		pos = 0;
	}
	if (pos >= len) {
		dest[0] = 0x00;
		__DEL(source);
		return;
	}
	i = 0;
	while ((pos + i <= source__len && source[__X(pos + i, source__len)] != 0x00) && i < n) {
		if (i < destLen) {
			dest[__X(i, dest__len)] = source[__X(pos + i, source__len)];
		}
		i += 1;
	}
	dest[__X(i, dest__len)] = 0x00;
	__DEL(source);
}

/*----------------------------------------------------------------------------*/
INTEGER Strings_Pos (CHAR *pattern, INTEGER pattern__len, CHAR *s, INTEGER s__len, INTEGER pos)
{
	INTEGER n1, n2, i, j;
	__DUP(pattern, pattern__len);
	__DUP(s, s__len);
	n1 = Strings_Length(s, s__len);
	n2 = Strings_Length(pattern, pattern__len);
	if (n2 == 0) {
		__DEL(pattern);
		__DEL(s);
		return 0;
	}
	i = pos;
	while (i <= n1 - n2) {
		if (s[__X(i, s__len)] == pattern[0]) {
			j = 1;
			while (j < n2 && s[__X(i + j, s__len)] == pattern[__X(j, pattern__len)]) {
				j += 1;
			}
			if (j == n2) {
				__DEL(pattern);
				__DEL(s);
				return i;
			}
		}
		i += 1;
	}
	__DEL(pattern);
	__DEL(s);
	return -1;
}

/*----------------------------------------------------------------------------*/
void Strings_Cap (CHAR *s, INTEGER s__len)
{
	INTEGER i;
	i = 0;
	while (s[__X(i, s__len)] != 0x00) {
		if ('a' <= s[__X(i, s__len)] && s[__X(i, s__len)] <= 'z') {
			s[__X(i, s__len)] = __CAP(s[__X(i, s__len)]);
		}
		i += 1;
	}
}

/*----------------------------------------------------------------------------*/
static struct Match__7 {
	struct Match__7 *lnk;
} *Match__7_s;

static BOOLEAN M__8 (CHAR *name, INTEGER name__len, CHAR *mask, INTEGER mask__len, INTEGER n, INTEGER m);

static BOOLEAN M__8 (CHAR *name, INTEGER name__len, CHAR *mask, INTEGER mask__len, INTEGER n, INTEGER m)
{
	while ((n >= 0 && m >= 0) && mask[__X(m, mask__len)] != '*') {
		if (name[__X(n, name__len)] != mask[__X(m, mask__len)]) {
			return 0;
		}
		n -= 1;
		m -= 1;
	}
	if (m < 0) {
		return n < 0;
	}
	while (m >= 0 && mask[__X(m, mask__len)] == '*') {
		m -= 1;
	}
	if (m < 0) {
		return 1;
	}
	while (n >= 0) {
		if (M__8(name, name__len, mask, mask__len, n, m)) {
			return 1;
		}
		n -= 1;
	}
	return 0;
}

BOOLEAN Strings_Match (CHAR *string, INTEGER string__len, CHAR *pattern, INTEGER pattern__len)
{
	struct Match__7 _s;
	BOOLEAN __retval;
	__DUP(string, string__len);
	__DUP(pattern, pattern__len);
	_s.lnk = Match__7_s;
	Match__7_s = &_s;
	__retval = M__8((void*)string, string__len, (void*)pattern, pattern__len, Strings_Length(string, string__len) - 1, Strings_Length(pattern, pattern__len) - 1);
	Match__7_s = _s.lnk;
	__DEL(string);
	__DEL(pattern);
	;
	return __retval;
}

/*----------------------------------------------------------------------------*/

export void *Strings__init(void)
{
	__DEFMOD;
	__REGMOD("Strings", 0);
/* BEGIN */
	__ENDMOD;
}

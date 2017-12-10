/* Ofront+ 1.0 -sC -44 */

#ifndef Graph__h
#define Graph__h

#include "SYSTEM.h"
#include "SDL2.h"

typedef
	struct Graph_BitmapDesc *Graph_Bitmap;

typedef
	struct Graph_BitmapDesc {
		char _prvt0[4];
		INTEGER w, h;
	} Graph_BitmapDesc;

import void Graph_Bitmap_Finalize (Graph_Bitmap bmp);
#define __Graph_Bitmap_Finalize(bmp) __SEND(__TYPEOF(bmp), Graph_Bitmap_Finalize, 0, void(*)(Graph_Bitmap), (bmp))

typedef
	void (*Graph_CloseBtnProc)(void);

typedef
	struct Graph_Key {
		INTEGER code, sym;
		SET mod;
		BOOLEAN repeat;
	} Graph_Key;

typedef
	struct Graph_Event {
		INTEGER type;
		Graph_Key key;
		INTEGER x, y, xRel, yRel, button;
		SET buttons;
		BOOLEAN down;
		CHAR s[32];
	} Graph_Event;

typedef
	struct Graph_EventQueue {
		INTEGER _prvt0;
		char _prvt1[20488];
	} Graph_EventQueue;

typedef
	struct Graph_FontDesc *Graph_Font;

typedef
	struct Graph_FontDesc {
		Graph_Bitmap bmp;
		INTEGER charW, charH, charRows, charsInRow;
		struct {
			INTEGER len[2];
			SDL2_Rect data[1];
		} *sprites;
	} Graph_FontDesc;

typedef
	struct Graph_Region {
		INTEGER x, y, w, h;
	} Graph_Region;


import INTEGER Graph_randomSeed;

import SYSTEM_ADRINT *Graph_BitmapDesc__typ;
import SYSTEM_ADRINT *Graph_FontDesc__typ;
import SYSTEM_ADRINT *Graph_Key__typ;
import SYSTEM_ADRINT *Graph_Region__typ;
import SYSTEM_ADRINT *Graph_Event__typ;
import SYSTEM_ADRINT *Graph_EventQueue__typ;

import void Graph_AddRegion (INTEGER x, INTEGER y, INTEGER w, INTEGER h);
import BOOLEAN Graph_AltPressed (void);
import void Graph_AppendN (INTEGER n, CHAR *s, INTEGER s__len);
import void Graph_Blit (Graph_Bitmap src, Graph_Bitmap dest, INTEGER sx, INTEGER sy, INTEGER sw, INTEGER sh, INTEGER dx, INTEGER dy);
import void Graph_BlitWhole (Graph_Bitmap src, Graph_Bitmap dest, INTEGER x, INTEGER y);
import INTEGER Graph_BmpCol (Graph_Bitmap bmp, INTEGER r, INTEGER g, INTEGER b);
import void Graph_Circle (Graph_Bitmap bmp, INTEGER x, INTEGER y, INTEGER r, INTEGER color);
import void Graph_CircleFill (Graph_Bitmap bmp, INTEGER x, INTEGER y, INTEGER r, INTEGER color);
import void Graph_ClearBitmap (Graph_Bitmap bmp);
import void Graph_ClearScreen (void);
import void Graph_ClearScreenToColor (INTEGER color);
import void Graph_ClearToColor (Graph_Bitmap bmp, INTEGER color);
import void Graph_Close (void);
import void Graph_ColorToRGB (INTEGER color, INTEGER *r, INTEGER *g, INTEGER *b);
import Graph_Bitmap Graph_CreateBitmap (INTEGER w, INTEGER h);
import void Graph_CreateStdMousePointer (void);
import BOOLEAN Graph_CtrlPressed (void);
import void Graph_Delay (INTEGER n);
import void Graph_DestroyBitmap (Graph_Bitmap bmp);
import void Graph_DrawCharacter (Graph_Bitmap dest, Graph_Font font, INTEGER x, INTEGER y, CHAR ch, INTEGER fg);
import void Graph_DrawCharacterEx (Graph_Bitmap dest, Graph_Bitmap sprite, INTEGER x, INTEGER y, INTEGER color, INTEGER bg);
import void Graph_DrawSpriteEx (Graph_Bitmap dest, Graph_Bitmap sprite, INTEGER x, INTEGER y, INTEGER mode, SET flip);
import void Graph_DrawString (Graph_Bitmap dest, Graph_Font font, INTEGER x, INTEGER y, CHAR *s, INTEGER s__len, INTEGER fg);
import void Graph_Ellipse (Graph_Bitmap bmp, INTEGER x, INTEGER y, INTEGER rx, INTEGER ry, INTEGER color);
import void Graph_EllipseFill (Graph_Bitmap bmp, INTEGER x, INTEGER y, INTEGER rx, INTEGER ry, INTEGER color);
import void Graph_FastLine (Graph_Bitmap bmp, INTEGER x1, INTEGER y1, INTEGER x2, INTEGER y2, INTEGER color);
import void Graph_Flip (void);
import void Graph_FloodFill (Graph_Bitmap bmp, INTEGER x, INTEGER y, INTEGER color);
import void Graph_GetDesktopResolution (INTEGER *w, INTEGER *h);
import void Graph_GetError (CHAR *s, INTEGER s__len);
import SET Graph_GetMouseButtons (void);
import Graph_Bitmap Graph_GetMousePointer (void);
import void Graph_GetMousePos (INTEGER *x, INTEGER *y);
import INTEGER Graph_GetPixel (Graph_Bitmap bmp, INTEGER x, INTEGER y);
import void Graph_GetRealMousePos (INTEGER *x, INTEGER *y);
import void Graph_HLine (Graph_Bitmap bmp, INTEGER x1, INTEGER y, INTEGER x2, INTEGER color);
import Graph_Bitmap Graph_Init (void);
import BOOLEAN Graph_KeyDown (INTEGER key);
import BOOLEAN Graph_KeyPressed (void);
import void Graph_Line (Graph_Bitmap bmp, INTEGER x1, INTEGER y1, INTEGER x2, INTEGER y2, INTEGER color);
import Graph_Bitmap Graph_LoadBitmap (CHAR *filename, INTEGER filename__len);
import Graph_Font Graph_LoadFont (CHAR *filename, INTEGER filename__len, INTEGER charW, INTEGER charH);
import void Graph_LockBitmap (Graph_Bitmap bmp);
import INTEGER Graph_MakeCol (INTEGER r, INTEGER g, INTEGER b);
import void Graph_MaskedBlit (Graph_Bitmap src, Graph_Bitmap dest, INTEGER sx, INTEGER sy, INTEGER dx, INTEGER dy, INTEGER w, INTEGER h);
import BOOLEAN Graph_MouseOnScreen (void);
import void Graph_Pause (void);
import BOOLEAN Graph_PollEvent (Graph_Event *event, SYSTEM_ADRINT *event__typ);
import void Graph_PutPixel (Graph_Bitmap bmp, INTEGER x, INTEGER y, INTEGER color);
import void Graph_PutPixelFast (Graph_Bitmap bmp, INTEGER x, INTEGER y, INTEGER color);
import void Graph_PutSeed (INTEGER newSeed);
import INTEGER Graph_Random (INTEGER range);
import void Graph_Randomize (void);
import CHAR Graph_ReadKey (void);
import void Graph_Rect (Graph_Bitmap bmp, INTEGER x1, INTEGER y1, INTEGER x2, INTEGER y2, INTEGER color);
import void Graph_RectFill (Graph_Bitmap bmp, INTEGER x1, INTEGER y1, INTEGER x2, INTEGER y2, INTEGER color);
import void Graph_RepeatFlip (void);
import void Graph_SetColorKey (Graph_Bitmap bmp, INTEGER color);
import void Graph_SetFPS (INTEGER fps);
import void Graph_SetMouseFocus (INTEGER x, INTEGER y);
import void Graph_SetMousePointer (Graph_Bitmap bmp);
import void Graph_SetRegion (INTEGER x, INTEGER y, INTEGER w, INTEGER h);
import void Graph_SetSizeStep (INTEGER w, INTEGER h);
import void Graph_SetStdMousePointer (void);
import void Graph_SetWindowTitle (CHAR *title, INTEGER title__len);
import void Graph_Settings (INTEGER w, INTEGER h, SET flags);
import BOOLEAN Graph_ShiftPressed (void);
import void Graph_ShowMouse (BOOLEAN show);
import void Graph_StartTextInput (void);
import void Graph_StopTextInput (void);
import void Graph_Str (INTEGER n, CHAR *s, INTEGER s__len);
import void Graph_StretchBlit (Graph_Bitmap src, Graph_Bitmap dest, INTEGER sx, INTEGER sy, INTEGER sw, INTEGER sh, INTEGER dx, INTEGER dy, INTEGER dw, INTEGER dh);
import void Graph_SwitchToFullscreen (void);
import void Graph_SwitchToWindowed (void);
import void Graph_ToggleFullscreen (void);
import REAL Graph_Uniform (void);
import void Graph_UnlockBitmap (Graph_Bitmap bmp);
import void Graph_UnsetRegion (void);
import void Graph_VLine (Graph_Bitmap bmp, INTEGER x, INTEGER y1, INTEGER y2, INTEGER color);
import INTEGER Graph_Val (CHAR *s, INTEGER s__len);
import void Graph_WaitEvents (INTEGER timeout);
import BOOLEAN Graph_WindowShown (void);
import void *Graph__init(void);


#endif

MODULE Mandelbrot2;
IMPORT G := Graph, M := Math;
CONST iter = 768;
VAR s: G.Bitmap;
  col: INTEGER;
  m: POINTER TO ARRAY OF ARRAY OF INTEGER;
  angle: REAL;

PROCEDURE Go(sx, sy: INTEGER; x, y: REAL);
VAR col, i: INTEGER;
  re, im, re2: REAL;
BEGIN re := x; im := y; i := 0;
  REPEAT re2 := re * re - im * im;
    im := 2 * re * im; re := re2;
    re := re + x; im := im + y; INC(i)
  UNTIL (i = iter) OR (re * re + im * im > 4);
  m[sy, sx] := i
END Go;

PROCEDURE Draw;
VAR i, x, y: INTEGER;
  shine: REAL;
BEGIN shine := (1 - M.cos(angle)) * 1;
  FOR y := 0 TO s.h - 1 DO
    FOR x := 0 TO s.w - 1 DO
      i := m[y, x];
      IF i # iter THEN
        i := SHORT(ENTIER(i * shine));
        IF i > 255 THEN i := 255 END;
        CASE col OF
          0: i := G.MakeCol(i, 0, 0)
        | 1: i := G.MakeCol(i, i DIV 3, 0)
        | 2: i := G.MakeCol(i, i, 0)
        | 3: i := G.MakeCol(0, i, 0)
        | 4: i := G.MakeCol(0, i DIV 2, i)
        | 5: i := G.MakeCol(0, 0, i)
        ELSE i := G.MakeCol(i * 2 DIV 4, 0, i * 2 DIV 4)
        END;
        G.PutPixel(s, x, y, i)
      END
    END
  END
END Draw;

PROCEDURE Do;
VAR x, y: INTEGER;
  x0, y0, x1, y1: REAL;
BEGIN G.Settings(320, 240, {G.fullscreen, G.spread});
  s := G.Init(); G.Flip; NEW(m, s.h, s.w);
  x0 := -0.63; x1 := -0.61; y0 := -0.45; y1 := -0.47;
  FOR y := 0 TO s.h - 1 DO
    FOR x := 0 TO s.w - 1 DO
      Go(x, y, x0 + x / (s.w - 1) * (x1 - x0),
               y0 + y / (s.h - 1) * (y1 - y0))
    END
  END;
  angle := 0; col := 4;
  REPEAT Draw; angle := angle + M.pi / 360;
    IF angle > M.pi * 2 THEN
      angle := angle - M.pi * 2;
      col := (col + 1) MOD 7;
    END;
    G.Flip; G.Delay(10)
  UNTIL G.KeyPressed();
  G.Close
END Do;

BEGIN
  Do
END Mandelbrot2.

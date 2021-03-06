MODULE Files;  (* J. Templ 1.12. 89/12.4.95 Oberon files mapped onto Unix files *)

  IMPORT SYSTEM, Platform, Heap, Strings, Console;

  (* standard data type I/O

    little endian,
    SInt:2, Int:4, LInt:4/8
    ORD({0}) = 1,
    false = 0, true = 1
    IEEE real format,
    null terminated strings,
    compact numbers according to M.Odersky *)


  CONST
    pathDelimiter = Platform.pathDelimiter;
    pathSeparator = Platform.pathSeparator;

    nofbufs = 4;
    bufsize = 4096;
    notDone = -1;

    (* file states *)
    open   = 0;    (* OS File has been opened *)
    create = 1;    (* OS file needs to be created *)
    close  = 2;    (* Register telling Create to use registerName directly:
                      i.e. since we're closing and all data is still in
                      buffers bypass writing to temp file and then renaming
                      and just write directly to fianl register name *)


  TYPE
    FileName = ARRAY 101 OF CHAR;
    File*    = POINTER TO FileDesc;
    Buffer   = POINTER TO BufDesc;

    FileDesc = RECORD
      workName, registerName: FileName;
      tempFile: BOOLEAN;
      identity: Platform.FileIdentity;
      fd-:      Platform.FileHandle;
      len, pos: LONGINT;
      bufs:     ARRAY nofbufs OF Buffer;
      swapper, state: SHORTINT;
      next:     File;
    END;

    BufDesc = RECORD
      f:    File;
      chg:  BOOLEAN;
      org:  LONGINT;
      size: INTEGER;
      data: ARRAY bufsize OF BYTE
    END;

    Rider* = RECORD
      res*: INTEGER;
      eof*: BOOLEAN;
      buf: Buffer;
      org: LONGINT;
      offset: INTEGER
    END;


  VAR
    files:      File;   (* List of files that have an OS file handle/descriptor assigned  *)
    tempno:     INTEGER;
    HOME:       ARRAY 1024 OF CHAR;
    SearchPath: POINTER TO ARRAY OF CHAR;


  PROCEDURE -IdxTrap "__HALT(-1)";

  PROCEDURE^ Finalize(o: SYSTEM.PTR);

  PROCEDURE Err(s: ARRAY [1] OF CHAR; f: File; errcode: Platform.ErrorCode);
  BEGIN
    Console.Ln; Console.String("-- "); Console.String(s); Console.String(": ");
    IF f # NIL THEN
      IF f.registerName # "" THEN Console.String(f.registerName) ELSE Console.String(f.workName) END;
      IF f.fd # 0 THEN Console.String("f.fd = "); Console.LongInt(f.fd, 1) END
    END;
    IF errcode # 0 THEN Console.String(" errcode = "); Console.Int(errcode, 1) END;
    Console.Ln;
    HALT(99)
  END Err;

  PROCEDURE MakeFileName(dir, name: ARRAY [1] OF CHAR; VAR dest: ARRAY OF CHAR);
    VAR i, j: INTEGER;
  BEGIN i := 0; j := 0;
    WHILE dir[i] # 0X DO dest[i] := dir[i]; INC(i) END;
    IF dest[i-1] # pathDelimiter THEN dest[i] := pathDelimiter; INC(i) END;
    WHILE name[j] # 0X DO dest[i] := name[j]; INC(i); INC(j) END;
    dest[i] := 0X
  END MakeFileName;

  PROCEDURE GetTempName(finalName: ARRAY [1] OF CHAR; VAR name: ARRAY OF CHAR);
    VAR n, i, j: INTEGER;
  BEGIN
    INC(tempno); n := tempno; i := 0;
    IF finalName[0] # pathDelimiter THEN  (* relative pathname *)
      WHILE Platform.CWD[i] # 0X DO name[i] := Platform.CWD[i]; INC(i) END;
      IF Platform.CWD[i-1] # pathDelimiter THEN name[i] := pathDelimiter; INC(i) END
    END;
    j := 0;
    WHILE finalName[j] # 0X DO name[i] := finalName[j]; INC(i); INC(j) END;
    DEC(i);
    WHILE name[i] # pathDelimiter DO DEC(i) END;
    name[i+1] := "."; name[i+2] := "t"; name[i+3] := "m"; name[i+4] := "p"; name[i+5] := "."; INC(i, 6);
    WHILE n > 0 DO name[i] := CHR(n MOD 10 + ORD("0")); n := n DIV 10; INC(i) END;
    name[i] := "."; INC(i); n := Platform.PID;
    WHILE n > 0 DO name[i] := CHR(n MOD 10 + ORD("0"));  n := n DIV 10; INC(i) END;
    name[i] := 0X
  END GetTempName;

  PROCEDURE Create(f: File);
    VAR
      identity:  Platform.FileIdentity;
      done:  BOOLEAN;
      error: Platform.ErrorCode;
      err:   ARRAY 32 OF CHAR;
  BEGIN
    (*
    Console.String("Files.Create fd = "); Console.Int(f.fd,1);
    Console.String(", registerName = ");  Console.String(f.registerName);
    Console.String(", workName = ");      Console.String(f.workName);
    Console.String(", state = ");         Console.Int(f.state,1);
    Console.Ln;
    *)
    IF f.fd = Platform.InvalidHandleValue() THEN
      IF f.state = create THEN
        GetTempName(f.registerName, f.workName); f.tempFile := TRUE
      ELSIF f.state = close THEN
        f.workName := f.registerName; f.registerName := ""; f.tempFile := FALSE
      END;
      error := Platform.DeleteFile(f.workName);  (*unlink first to avoid stale NFS handles and to avoid reuse of inodes*)

      error := Platform.NewFile(f.workName, f.fd);
      done := error = 0;
      IF done THEN
        f.next := files;  files := f;
        INC(Heap.FileCount);
        Heap.RegisterFinalizer(f, Finalize);
        f.state := open;
        f.pos   := 0;
        error   := Platform.Identify(f.fd, f.identity);
      ELSE
        IF    Platform.NoSuchDirectory(error) THEN err := "no such directory"
        ELSIF Platform.TooManyFiles(error)    THEN err := "too many files open"
        ELSE  err := "file not created"
        END;
        Err(err, f, error)
      END
    END
  END Create;

  PROCEDURE Flush(buf: Buffer);
    VAR
      error:     Platform.ErrorCode;
      f:         File;
      (* identity:  Platform.FileIdentity; *)
  BEGIN
    (*
    Console.String("Files.Flush buf.f.registername = "); Console.String(buf.f.registerName);
    Console.String(", buf.f.fd = "); Console.Int(buf.f.fd,1);
    Console.String(", buffer at $"); Console.LongHex(SYSTEM.ADR(buf.data));
    Console.String(", size "); Console.Int(buf.size,1); Console.Ln;
    *)
    IF buf.chg THEN f := buf.f; Create(f);
      IF buf.org # f.pos THEN
        error := Platform.Seek(f.fd, buf.org, Platform.SeekSet);
        (*
          Console.String("Seeking to "); Console.Int(buf.org,1);
          Console.String(", error code "); Console.Int(error,1); Console.Ln;
        *)
      END;
      error := Platform.Write(f.fd, SYSTEM.ADR(buf.data), buf.size);
      IF error # 0 THEN Err("error writing file", f, error) END;
      f.pos := buf.org + buf.size;
      buf.chg := FALSE;
      error := Platform.Identify(f.fd, f.identity);
      IF error # 0 THEN Err("error identifying file", f, error) END;
      (*
        error := Platform.Identify(f.fd, identity);
        f.identity.mtime := identity.mtime;
      *)
    END
  END Flush;


  PROCEDURE CloseOSFile(f: File);
  (* Close the OS file handle and remove f from 'files' *)
    VAR prev: File; error: Platform.ErrorCode;
  BEGIN
    IF files = f THEN files := f.next
    ELSE
      prev := files;
      WHILE (prev # NIL) & (prev.next # f) DO prev := prev.next END;
      IF prev.next # NIL THEN prev.next := f.next END
    END;
    error := Platform.CloseFile(f.fd);
    f.fd := Platform.InvalidHandleValue(); f.state := create; DEC(Heap.FileCount);
  END CloseOSFile;


  PROCEDURE Close* (f: File);
    VAR
      i:     INTEGER;
      error: Platform.ErrorCode;
  BEGIN
    IF (f.state # create) OR (f.registerName # "") THEN
      Create(f); i := 0;
      WHILE (i < nofbufs) & (f.bufs[i] # NIL) DO Flush(f.bufs[i]); INC(i) END;
      error := Platform.Sync(f.fd);
      IF error # 0 THEN Err("error writing file", f, error) END;
      CloseOSFile(f);
    END
  END Close;

  PROCEDURE Length* (f: File): LONGINT;
  BEGIN RETURN f.len END Length;

  PROCEDURE New* (name: ARRAY [1] OF CHAR): File;
    VAR f: File;
  BEGIN
    NEW(f); f.workName := ""; COPY(name, f.registerName);
    f.fd := Platform.InvalidHandleValue(); f.state := create; f.len := 0; f.pos := 0; f.swapper := -1; (*all f.buf[i] = NIL*)
    RETURN f
  END New;

  PROCEDURE ScanPath(VAR pos: INTEGER; VAR dir: ARRAY OF CHAR);
  (* Extract next individual directory from searchpath starting at pos,
     updating pos and returning dir.
     Supports ~, ~user and blanks inside path *)
  VAR i: INTEGER; ch: CHAR;
  BEGIN
    i := 0;
    IF SearchPath = NIL THEN
      IF pos = 0 THEN
        dir[0] := "."; i := 1; INC(pos) (* Default search path is just the current directory *)
      END
    ELSE
      ch := SearchPath[pos];
      WHILE (ch = " ") OR (ch = pathSeparator) DO INC(pos); ch := SearchPath[pos] END;
      IF ch = "~" THEN
        INC(pos); ch := SearchPath[pos];
        WHILE HOME[i] # 0X DO dir[i] := HOME[i]; INC(i) END;
        IF (ch # pathDelimiter) & (ch # 0X) & (ch # pathSeparator) & (ch # " ") THEN
          WHILE (i > 0) & (dir[i-1] # pathDelimiter) DO DEC(i) END
        END
      END;
      WHILE (ch # 0X) & (ch # pathSeparator) DO dir[i] := ch; INC(i); INC(pos); ch := SearchPath[pos] END;
      WHILE (i > 0) & (dir[i-1] = " ") DO DEC(i) END
    END;
    dir[i] := 0X
  END ScanPath;

  PROCEDURE HasDir(VAR name: ARRAY OF CHAR): BOOLEAN;
    VAR i: INTEGER; ch: CHAR;
  BEGIN i := 0; ch := name[0];
    WHILE (ch # 0X) & (ch # pathDelimiter) DO INC(i); ch := name[i] END;
    RETURN ch = pathDelimiter
  END HasDir;

  PROCEDURE CacheEntry(identity: Platform.FileIdentity): File;
    VAR f: File; i: INTEGER; error: Platform.ErrorCode; len: LONGINT;
  BEGIN f := files;
    WHILE f # NIL DO
      IF Platform.SameFile(identity, f.identity) THEN
        IF ~Platform.SameFileTime(identity, f.identity) THEN i := 0;
          WHILE i < nofbufs DO
            IF f.bufs[i] # NIL THEN f.bufs[i].org := -1; f.bufs[i] := NIL END;
            INC(i)
          END;
          f.swapper := -1; f.identity := identity;
          error := Platform.FileSize(f.fd, len); f.len := len
        END;
        RETURN f
      END;
      f := f.next
    END;
    RETURN NIL
  END CacheEntry;

  PROCEDURE Old*(name: ARRAY [1] OF CHAR): File;
    VAR
      f:         File;
      fd:        Platform.FileHandle;
      pos:       INTEGER;
      done:      BOOLEAN;
      dir, path: ARRAY 256 OF CHAR;
      error:     Platform.ErrorCode;
      identity:  Platform.FileIdentity;
      len:       LONGINT;
  BEGIN
    (* Console.String("Files.Old "); Console.String(name); Console.Ln; *)
    IF name # "" THEN
      IF HasDir(name) THEN dir := ""; COPY(name, path)
      ELSE pos := 0; ScanPath(pos, dir); MakeFileName(dir, name, path); ScanPath(pos, dir)
      END;
      LOOP
        error := Platform.OldRW(path, fd); done := error = 0;
        IF ~done & Platform.TooManyFiles(error) THEN Err("too many files open", f, error) END;
        IF ~done & Platform.Inaccessible(error) THEN
          error := Platform.OldRO(path, fd); done := error = 0;
        END;
        IF ~done & ~Platform.Absent(error) THEN
          Console.String("Warning: Files.Old "); Console.String(name);
          Console.String(" error = "); Console.Int(error, 0); Console.Ln;
        END;
        IF done THEN
          (* Console.String("  fd = "); Console.Int(fd,1); Console.Ln; *)
          error := Platform.Identify(fd, identity);
          f := CacheEntry(identity);
          IF f # NIL THEN
            (* error := Platform.Close(fd); DCWB: Either this should be removed or should call CloseOSFile. *)
            RETURN f
          ELSE NEW(f); Heap.RegisterFinalizer(f, Finalize);
            f.fd := fd; f.state := open; f.pos := 0; f.swapper := -1; (*all f.buf[i] = NIL*)
            error := Platform.FileSize(fd, len); f.len := len;
            COPY(name, f.workName); f.registerName := ""; f.tempFile := FALSE;
            f.identity := identity;
            f.next := files; files := f; INC(Heap.FileCount);
            RETURN f
          END
        ELSIF dir = "" THEN RETURN NIL
        ELSE MakeFileName(dir, name, path); ScanPath(pos, dir)
        END
      END
    ELSE RETURN NIL
    END
  END Old;

  PROCEDURE Purge* (f: File);
    VAR i: INTEGER; identity: Platform.FileIdentity; error: Platform.ErrorCode;
  BEGIN i := 0;
    WHILE i < nofbufs DO
      IF f.bufs[i] # NIL THEN f.bufs[i].org := -1; f.bufs[i] := NIL END;
      INC(i)
    END;
    IF f.fd # Platform.InvalidHandleValue() THEN
      error := Platform.TruncateFile(f.fd, 0);
      error := Platform.Seek(f.fd, 0, Platform.SeekSet)
    END;
    f.pos := 0; f.len := 0; f.swapper := -1;
    error := Platform.Identify(f.fd, identity); Platform.SetMTime(f.identity, identity)
  END Purge;

  PROCEDURE GetDate* (f: File; VAR t, d: INTEGER);
    VAR
      identity: Platform.FileIdentity; error: Platform.ErrorCode;
  BEGIN
    Create(f); error := Platform.Identify(f.fd, identity);
    Platform.MTimeAsClock(identity, t, d)
  END GetDate;

  PROCEDURE Pos* (VAR r: Rider): LONGINT;
  BEGIN RETURN r.org + r.offset
  END Pos;

  PROCEDURE Set* (VAR r: Rider; f: File; pos: LONGINT);
    VAR
      org: LONGINT; offset, i, n: INTEGER; buf: Buffer;
      error: Platform.ErrorCode;
  BEGIN
    IF f # NIL THEN
      (*
      Console.String("Files.Set rider on fd = "); Console.Int(f.fd,1);
      Console.String(", registerName = ");  Console.String(f.registerName);
      Console.String(", workName = ");      Console.String(f.workName);
      Console.String(", state = ");         Console.Int(f.state,1);
      Console.Ln;
      *)
      IF pos > f.len THEN pos := f.len ELSIF pos < 0 THEN pos := 0 END;
      offset := SHORT(pos MOD bufsize); org := pos - offset; i := 0;
      WHILE (i < nofbufs) & (f.bufs[i] # NIL) & (org # f.bufs[i].org) DO INC(i) END;
      IF i < nofbufs THEN
        IF f.bufs[i] = NIL THEN NEW(buf); buf.chg := FALSE; buf.org := -1; buf.f := f; f.bufs[i] := buf
        ELSE buf := f.bufs[i]
        END
      ELSE
        f.swapper := (f.swapper + 1) MOD nofbufs;
        buf := f.bufs[f.swapper];
        Flush(buf)
      END;
      IF buf.org # org THEN
        IF org = f.len THEN buf.size := 0
        ELSE Create(f);
          IF f.pos # org THEN error := Platform.Seek(f.fd, org, Platform.SeekSet) END;
          error := Platform.ReadBuf(f.fd, buf.data, n);
          IF error # 0 THEN Err("read from file not done", f, error) END;
          f.pos := org + n;
          buf.size := n
        END;
        buf.org := org; buf.chg := FALSE
      END
    ELSE buf := NIL; org := 0; offset := 0
    END;
    r.buf := buf; r.org := org; r.offset := offset; r.eof := FALSE; r.res := 0
  END Set;

  PROCEDURE ReadByte* (VAR r: Rider; VAR x: BYTE);
    VAR offset: INTEGER; buf: Buffer;
  BEGIN
    buf := r.buf; offset := r.offset;
    IF r.org # buf.org THEN Set(r, buf.f, r.org + offset); buf := r.buf; offset := r.offset END;
    IF (offset < buf.size) THEN
      x := buf.data[offset]; r.offset := offset + 1
    ELSIF r.org + offset < buf.f.len THEN
      Set(r, r.buf.f, r.org + offset);
      x := r.buf.data[0]; r.offset := 1
    ELSE
      x := 0; r.eof := TRUE
    END
  END ReadByte;

  PROCEDURE ReadBytes* (VAR r: Rider; VAR x: ARRAY OF BYTE; n: INTEGER);
    VAR xpos, min, restInBuf, offset: INTEGER; buf: Buffer;
  BEGIN
    IF n > LEN(x) THEN IdxTrap END;
    xpos := 0; buf := r.buf; offset := r.offset;
    WHILE n > 0 DO
      IF (r.org # buf.org) OR (offset >= bufsize) THEN
        Set(r, buf.f, r.org + offset);
        buf := r.buf; offset := r.offset
      END;
      restInBuf := buf.size - offset;
      IF restInBuf = 0 THEN r.res := n; r.eof := TRUE; RETURN
      ELSIF n > restInBuf THEN min := restInBuf ELSE min := n END;
      SYSTEM.MOVE(SYSTEM.ADR(buf.data) + offset, SYSTEM.ADR(x) + xpos, min);
      INC(offset, min); r.offset := offset; INC(xpos, min); DEC(n, min)
    END;
    r.res := 0; r.eof := FALSE
  END ReadBytes;

  PROCEDURE ReadChar* (VAR r: Rider; VAR x: CHAR);
  BEGIN
     ReadByte(r, SYSTEM.VAL(BYTE, x))
  END ReadChar;

  PROCEDURE Base* (VAR r: Rider): File;
  BEGIN RETURN r.buf.f
  END Base;

  PROCEDURE WriteByte* (VAR r: Rider; x: BYTE);
    VAR buf: Buffer; offset: INTEGER;
  BEGIN
    buf := r.buf; offset := r.offset;
    IF (r.org # buf.org) OR (offset >= bufsize) THEN
      Set(r, buf.f, r.org + offset);
      buf := r.buf; offset := r.offset
    END;
    buf.data[offset] := x;
    buf.chg := TRUE;
    IF offset = buf.size THEN
      INC(buf.size); INC(buf.f.len)
    END;
    r.offset := offset + 1; r.res := 0
  END WriteByte;

  PROCEDURE WriteBytes* (VAR r: Rider; VAR x: ARRAY OF BYTE; n: INTEGER);
    VAR xpos, min, restInBuf, offset: INTEGER; buf: Buffer;
  BEGIN
    IF n > LEN(x) THEN IdxTrap END;
    xpos := 0; buf := r.buf; offset := r.offset;
    WHILE n > 0 DO
      IF (r.org # buf.org) OR (offset >= bufsize) THEN
        Set(r, buf.f, r.org + offset);
        buf := r.buf; offset := r.offset
      END;
      restInBuf := bufsize - offset;
      IF n > restInBuf THEN min := restInBuf ELSE min := n END;
      SYSTEM.MOVE(SYSTEM.ADR(x) + xpos, SYSTEM.ADR(buf.data) + offset, min);
      INC(offset, min); r.offset := offset;
      IF offset > buf.size THEN buf.f.len := buf.f.len + (offset - buf.size); buf.size := offset END;
      INC(xpos, min); DEC(n, min); buf.chg := TRUE
    END;
    r.res := 0
  END WriteBytes;

(* another solution would be one that is similar to ReadBytes, WriteBytes.
No code duplication, more symmetric, only two ifs for
Read and Write in buffer, buf.size replaced by bufsize in Write ops, buf.size and len
must be made consistent with offset (if offset > buf.size) in a lazy way.

PROCEDURE Write* (VAR r: Rider; x: BYTE);
  VAR buf: Buffer; offset: INTEGER;
BEGIN
  buf := r.buf; offset := r.offset;
  IF (offset >= bufsize) OR (r.org # buf.org) THEN
    Set(r, buf.f, r.org + offset); buf := r.buf; offset := r.offset;
  END;
  buf.data[offset] := x; r.offset := offset + 1; buf.chg := TRUE
END Write;

PROCEDURE WriteBytes ...

PROCEDURE Read* (VAR r: Rider; VAR x: BYTE);
  VAR offset: INTEGER; buf: Buffer;
BEGIN
  buf := r.buf; offset := r.offset;
  IF (offset >= buf.size) OR (r.org # buf.org) THEN
    IF r.org + offset >= buf.f.len THEN x := 0X; r.eof := TRUE; RETURN
    ELSE Set(r, buf.f, r.org + offset); buf := r.buf; offset := r.offset
    END
  END;
  x := buf.data[offset]; r.offset := offset + 1
END Read;

but this would also affect Set, Length, and Flush.
Especially Length would become fairly complex.
*)

  PROCEDURE Delete*(name: ARRAY [1] OF CHAR; VAR res: INTEGER);
  BEGIN res := Platform.DeleteFile(name) END Delete;

  PROCEDURE Rename* (old, new: ARRAY [1] OF CHAR; VAR res: INTEGER);
    VAR
      fdold, fdnew: Platform.FileHandle;
      n: INTEGER;
      error, ignore: Platform.ErrorCode;
      oldidentity, newidentity: Platform.FileIdentity;
      buf: ARRAY 4096 OF CHAR;
  BEGIN
    (*
    Console.String("Files.Rename old = "); Console.String(old);
    Console.String(", new = "); Console.String(new); Console.Ln;
    *)
    error := Platform.IdentifyByName(old, oldidentity);
    IF error = 0 THEN
      error := Platform.IdentifyByName(new, newidentity);
      IF (error # 0) & ~Platform.SameFile(oldidentity, newidentity) THEN
        Delete(new, error);  (* work around stale nfs handles *)
      END;
      error := Platform.RenameFile(old, new);
      (* Console.String("Platform.Rename error code "); Console.Int(error,1); Console.Ln; *)
      IF ~Platform.DifferentFilesystems(error) THEN
        res := error; RETURN
      ELSE
        (* cross device link, move the file *)
        error := Platform.OldRO(old, fdold);
        IF error # 0 THEN res := 2; RETURN END;
        error := Platform.NewFile(new, fdnew);
        IF error # 0 THEN error := Platform.CloseFile(fdold); res := 3; RETURN END;
        error := Platform.Read(fdold, SYSTEM.ADR(buf), bufsize, n);
        WHILE n > 0 DO
          error := Platform.Write(fdnew, SYSTEM.ADR(buf), n);
          IF error # 0 THEN
            ignore := Platform.CloseFile(fdold);
            ignore := Platform.CloseFile(fdnew);
            Err("cannot move file", NIL, error)
          END;
          error := Platform.Read(fdold, SYSTEM.ADR(buf), bufsize, n);
        END;
        ignore := Platform.CloseFile(fdold);
        ignore := Platform.CloseFile(fdnew);
        IF n = 0 THEN
          error := Platform.DeleteFile(old); res := 0
        ELSE
          Err("cannot move file", NIL, error)
        END;
      END
    ELSE
      res := 2 (* old file not found *)
    END
  END Rename;

  PROCEDURE Register* (f: File);
    VAR errcode: INTEGER; f1: File; file: ARRAY 104 OF CHAR;
  BEGIN
    (*
    Console.String("Files.Register f.registerName = "); Console.String(f.registerName);
    Console.String(", fd = "); Console.Int(f.fd,1); Console.Ln;
    *)
    IF (f.state = create) & (f.registerName # "") THEN f.state := close (* shortcut renaming *) END;
    Close(f);
    IF f.registerName # "" THEN
      Rename(f.workName, f.registerName, errcode);
      (*
      Console.String("Renamed (for register) f.fd = "); Console.Int(f.fd,1);
      Console.String(" from workname ");                Console.String(f.workName);
      Console.String(" to registerName ");              Console.String(f.registerName);
      Console.String(" errorcode = ");                  Console.Int(errcode,1); Console.Ln;
      *)
      IF errcode # 0 THEN COPY(f.registerName, file); HALT(99) END;
      f.workName := f.registerName; f.registerName := ""; f.tempFile := FALSE
    END
  END Register;

  PROCEDURE ChangeDirectory*(path: ARRAY [1] OF CHAR; VAR res: INTEGER);
  BEGIN
    res := Platform.Chdir(path)
  END ChangeDirectory;

  PROCEDURE FlipBytes(VAR src, dest: ARRAY OF BYTE);
    VAR i, j: INTEGER;
  BEGIN
    IF ~Platform.LittleEndian THEN i := LEN(src); j := 0;
      WHILE i > 0 DO DEC(i); dest[j] := src[i]; INC(j) END
    ELSE SYSTEM.MOVE(SYSTEM.ADR(src), SYSTEM.ADR(dest), LEN(src))
    END
  END FlipBytes;

  PROCEDURE ReadBool* (VAR R: Rider; VAR x: BOOLEAN);
  BEGIN ReadByte(R, SYSTEM.VAL(BYTE, x))
  END ReadBool;

  PROCEDURE ReadSInt* (VAR R: Rider; VAR x: SHORTINT);
    VAR b: ARRAY SIZE(SHORTINT) OF CHAR;
  BEGIN ReadBytes(R, b, SIZE(SHORTINT));
    IF SIZE(SHORTINT) = 1 THEN
      x := ORD(b[0])
    ELSE
      x := ORD(b[0]) + ORD(b[1])*256
    END
  END ReadSInt;

  PROCEDURE ReadInt* (VAR R: Rider; VAR x: INTEGER);
    VAR b: ARRAY SIZE(INTEGER) OF CHAR;
  BEGIN ReadBytes(R, b, SIZE(INTEGER));
    IF SIZE(INTEGER) = 2 THEN
      x := ORD(b[0]) + ORD(b[1])*256
    ELSE
      x := ORD(b[0]) + ORD(b[1])*100H + ORD(b[2])*10000H + ORD(b[3])*1000000H
    END
  END ReadInt;

  PROCEDURE ReadLInt* (VAR R: Rider; VAR x: LONGINT);
    VAR b: ARRAY SIZE(LONGINT) OF CHAR; n: INTEGER; s: LONGINT;
  BEGIN ReadBytes(R, b, SIZE(LONGINT));
    IF SIZE(LONGINT) = 4 THEN
      x := ORD(b[0]) + ORD(b[1])*100H + ORD(b[2])*10000H + ORD(b[3])*1000000H
    ELSE
      x := ORD(b[0]); s := 100H;
      FOR n := 1 TO SIZE(LONGINT)-1 DO INC(x, ORD(b[n])*s); s := s*100H END
    END
  END ReadLInt;

  PROCEDURE ReadSet* (VAR R: Rider; VAR x: SET);
    VAR b: ARRAY 4 OF CHAR;
  BEGIN ReadBytes(R, b, 4);
    x := SYSTEM.VAL(SET, ORD(b[0]) + ORD(b[1])*100H + ORD(b[2])*10000H + ORD(b[3])*1000000H)
  END ReadSet;

  PROCEDURE ReadReal* (VAR R: Rider; VAR x: REAL);
    VAR b: ARRAY 4 OF CHAR;
  BEGIN ReadBytes(R, b, 4); FlipBytes(b, x)
  END ReadReal;

  PROCEDURE ReadLReal* (VAR R: Rider; VAR x: LONGREAL);
    VAR b: ARRAY 8 OF CHAR;
  BEGIN ReadBytes(R, b, 8); FlipBytes(b, x)
  END ReadLReal;

  PROCEDURE ReadString* (VAR R: Rider; VAR x: ARRAY OF CHAR);
    VAR i: INTEGER; ch: CHAR;
  BEGIN i := 0;
    REPEAT ReadChar(R, ch); x[i] := ch; INC(i) UNTIL ch = 0X
  END ReadString;

  PROCEDURE ReadLine* (VAR R: Rider; VAR x: ARRAY OF CHAR);
    VAR i: INTEGER; ch: CHAR; b: BOOLEAN;
  BEGIN
    i := 0;
    b := FALSE;
    REPEAT
      ReadChar(R, ch);
      IF ((ch = 0X) OR (ch = 0AX) OR (ch = 0DX)) THEN
        b := TRUE
      ELSE
        x[i] := ch; INC(i)
      END
    UNTIL b
  END ReadLine;

  PROCEDURE ReadNum* (VAR R: Rider; VAR x: LONGINT);
    VAR s: INTEGER; ch: CHAR;
  BEGIN s := 0; x := 0; ReadChar(R, ch);
    WHILE ORD(ch) >= 128 DO INC(x, ASH(LONG(LONG(ORD(ch) - 128)), s) ); INC(s, 7); ReadChar(R, ch) END;
    INC(x, ASH(LONG(LONG(ORD(ch) MOD 64 - ORD(ch) DIV 64 * 64)), s) )
  END ReadNum;

  PROCEDURE WriteBool* (VAR R: Rider; x: BOOLEAN);
  BEGIN WriteByte(R, SYSTEM.VAL(BYTE, x))
  END WriteBool;

  PROCEDURE WriteChar* (VAR R: Rider; x: CHAR);
  BEGIN WriteByte(R, SYSTEM.VAL(BYTE, x))
  END WriteChar;

  PROCEDURE WriteSInt* (VAR R: Rider; x: SHORTINT);
    VAR b: ARRAY SIZE(SHORTINT) OF CHAR;
  BEGIN
    IF SIZE(SHORTINT) = 1 THEN
      b[0] := CHR(x)
    ELSE
      b[0] := CHR(x); b[1] := CHR(x DIV 256)
    END;
    WriteBytes(R, b, SIZE(SHORTINT))
  END WriteSInt;

  PROCEDURE WriteInt* (VAR R: Rider; x: INTEGER);
    VAR b: ARRAY SIZE(INTEGER) OF CHAR;
  BEGIN
    IF SIZE(INTEGER) = 2 THEN
      b[0] := CHR(x); b[1] := CHR(x DIV 256)
    ELSE
      b[0] := CHR(x); b[1] := CHR(x DIV 100H);
      b[2] := CHR(x DIV 10000H); b[3] := CHR(x DIV 1000000H)
    END;
    WriteBytes(R, b, SIZE(INTEGER))
  END WriteInt;

  PROCEDURE WriteLInt* (VAR R: Rider; x: LONGINT);
    VAR b: ARRAY SIZE(LONGINT) OF CHAR; n: INTEGER; s: LONGINT;
  BEGIN
    IF SIZE(LONGINT) = 4 THEN
      b[0] := CHR(x); b[1] := CHR(x DIV 100H);
      b[2] := CHR(x DIV 10000H); b[3] := CHR(x DIV 1000000H)
    ELSE
      b[0] := CHR(x); s := 100H;
      FOR n := 0 TO SIZE(LONGINT)-1 DO b[n] := CHR(x DIV s); s := s*100H END
    END;
    WriteBytes(R, b, SIZE(LONGINT))
  END WriteLInt;

  PROCEDURE WriteSet* (VAR R: Rider; x: SET);
    VAR b: ARRAY 4 OF CHAR; i: INTEGER;
  BEGIN i := SYSTEM.VAL(INTEGER, x);
    b[0] := CHR(i); b[1] := CHR(i DIV 100H); b[2] := CHR(i DIV 10000H); b[3] := CHR(i DIV 1000000H);
    WriteBytes(R, b, 4)
  END WriteSet;

  PROCEDURE WriteReal* (VAR R: Rider; x: REAL);
    VAR b: ARRAY 4 OF CHAR;
  BEGIN FlipBytes(x, b); WriteBytes(R, b, 4)
  END WriteReal;

  PROCEDURE WriteLReal* (VAR R: Rider; x: LONGREAL);
    VAR b: ARRAY 8 OF CHAR;
  BEGIN FlipBytes(x, b); WriteBytes(R, b, 8)
  END WriteLReal;

  PROCEDURE WriteString* (VAR R: Rider; x: ARRAY [1] OF CHAR);
    VAR i: INTEGER;
  BEGIN i := 0;
    WHILE x[i] # 0X DO INC(i) END;
    WriteBytes(R, x, i+1)
  END WriteString;

  PROCEDURE WriteNum* (VAR R: Rider; x: LONGINT);
  BEGIN
    WHILE (x < - 64) OR (x > 63) DO WriteChar(R, CHR(SHORT(x) MOD 128 + 128)); x := x DIV 128 END;
    WriteChar(R, CHR(SHORT(x) MOD 128))
  END WriteNum;

  PROCEDURE GetName*(f: File; VAR name: ARRAY OF CHAR);
  BEGIN
     COPY (f.workName, name)
  END GetName;

  PROCEDURE Finalize(o: SYSTEM.PTR);
    VAR f: File; res: INTEGER;
  BEGIN
    f := SYSTEM.VAL(File, o);
    (*
    Console.String("Files.Finalize f.fd = "); Console.Int(f.fd,1);
    Console.String(", f.registername = "); Console.String(f.registerName);
    Console.String(", f.workName = "); Console.String(f.workName); Console.Ln;
    *)
    IF f.fd # Platform.InvalidHandleValue() THEN
      CloseOSFile(f);
      IF f.tempFile THEN res := Platform.DeleteFile(f.workName) END
    END
  END Finalize;

  PROCEDURE SetSearchPath*(path: ARRAY [1] OF CHAR);
    VAR pathlen: INTEGER;
  BEGIN
    pathlen := Strings.Length(path);
    IF pathlen # 0 THEN
      NEW(SearchPath, pathlen + 1); COPY(path, SearchPath^)
    ELSE
      SearchPath := NIL
    END
  END SetSearchPath;


BEGIN
  tempno := -1;
  Heap.FileCount := 0;
  HOME := "";  Platform.GetEnv("HOME", HOME);
END Files.

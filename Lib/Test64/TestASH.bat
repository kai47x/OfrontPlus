@SET PATH=..\Bin;..\..\Bin64;%PATH%
@SET OBERON=%CD%;%CD%\..\Sym64

@ofront+ -mC -88 ..\Test\TestASH.Mod
@IF errorlevel 1 PAUSE
@CALL gcc64.bat TestASH.c -o TestASH.exe

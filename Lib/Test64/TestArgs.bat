@SET PATH=..\Bin;..\..\Bin64;%PATH%
@SET OBERON=%CD%;%CD%\..\Sym64

@ofront+ -mC -88 ..\Test\TestArgs.Mod
@IF errorlevel 1 PAUSE
@CALL gcc64.bat TestArgs.c -o TestArgs.exe

@IF EXIST TestArgs.exe TestArgs.exe arg1 "ARG 2" "__!@#$_arg3_&%*()-="
@PAUSE

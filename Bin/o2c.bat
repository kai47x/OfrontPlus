@ECHO OFF
CD ..\Obj
SET PATH=..\Bin;%PATH%
SET OBERON=.\;.\..\Lib\Sym;.\..\Sym;.\..\Mod

ofront+ -sC -48 OfrontErrors.Mod OfrontOPM.cmdln.Mod OfrontOPS.Mod OfrontOPT.Mod OfrontOPB.Mod OfrontOPP.Mod OfrontOPC.Mod OfrontOPV.Mod BrowserCmd.Mod -m OCatCmd.Mod -m OfrontCmd.Mod -m
IF errorlevel 1 PAUSE

FOR %%i IN (*.sym) DO MOVE /Y %%i ..\Sym >NUL

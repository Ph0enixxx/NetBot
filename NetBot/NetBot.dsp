# Microsoft Developer Studio Project File - Name="NetBot" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=NetBot - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "NetBot.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "NetBot.mak" CFG="NetBot - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "NetBot - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe
# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "ZLIB_WINAPI" /Fr /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /pdb:none /machine:I386 /nodefaultlib:"MSVCRT.lib" /out:"..\NetBot_Cn.exe"
# Begin Target

# Name "NetBot - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ExClass\AviFile.cpp
# End Source File
# Begin Source File

SOURCE=.\ExControl\CmdEdit.cpp
# End Source File
# Begin Source File

SOURCE=..\Seu_lib\Command.cpp
# End Source File
# Begin Source File

SOURCE=.\DDosDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Debug.cpp
# End Source File
# Begin Source File

SOURCE=.\FileDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FileTranDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ExClass\IniFile.cpp
# End Source File
# Begin Source File

SOURCE=.\NetBot.cpp
# End Source File
# Begin Source File

SOURCE=.\NetBot.rc
# End Source File
# Begin Source File

SOURCE=.\NetBotDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ProcessDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ScreenDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ServerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ExClass\SEU_QQwry.cpp
# End Source File
# Begin Source File

SOURCE=.\ShellDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SortList.cpp
# End Source File
# Begin Source File

SOURCE=.\ExControl\SplashScreenEx.cpp
# End Source File
# Begin Source File

SOURCE=.\ExControl\StatLink.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\ExControl\TrueColorToolBar.cpp
# End Source File
# Begin Source File

SOURCE=.\UpdateDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\VideoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ExControl\XPictureBox.cpp
# End Source File
# Begin Source File

SOURCE=.\XScreenXor.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ExClass\AutoLock.h
# End Source File
# Begin Source File

SOURCE=.\ExClass\AviFile.h
# End Source File
# Begin Source File

SOURCE=.\ExControl\CmdEdit.h
# End Source File
# Begin Source File

SOURCE=..\Seu_lib\Command.h
# End Source File
# Begin Source File

SOURCE=.\DDosDlg.h
# End Source File
# Begin Source File

SOURCE=.\FileDlg.h
# End Source File
# Begin Source File

SOURCE=.\FileTranDlg.h
# End Source File
# Begin Source File

SOURCE=.\ExClass\IniFile.h
# End Source File
# Begin Source File

SOURCE=.\NetBot.h
# End Source File
# Begin Source File

SOURCE=.\NetBotDlg.h
# End Source File
# Begin Source File

SOURCE=.\ProcessDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\ScreenDlg.h
# End Source File
# Begin Source File

SOURCE=.\ServerDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetDlg.h
# End Source File
# Begin Source File

SOURCE=.\ExClass\SEU_QQwry.h
# End Source File
# Begin Source File

SOURCE=.\ShellDlg.h
# End Source File
# Begin Source File

SOURCE=.\SortList.h
# End Source File
# Begin Source File

SOURCE=.\ExControl\SplashScreenEx.h
# End Source File
# Begin Source File

SOURCE=.\start.h
# End Source File
# Begin Source File

SOURCE=.\ExControl\StatLink.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\ExClass\ThreadTemplate.h
# End Source File
# Begin Source File

SOURCE=.\ExControl\TrueColorToolBar.h
# End Source File
# Begin Source File

SOURCE=.\UpdateDlg.h
# End Source File
# Begin Source File

SOURCE=.\VideoDlg.h
# End Source File
# Begin Source File

SOURCE=.\ExControl\XPictureBox.h
# End Source File
# Begin Source File

SOURCE=.\XScreenXor.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\outlook\config.ico
# End Source File
# Begin Source File

SOURCE=.\res\outlook\ddos.ico
# End Source File
# Begin Source File

SOURCE=.\res\DDOSLogo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\outlook\default1.bin
# End Source File
# Begin Source File

SOURCE=.\res\dir.ico
# End Source File
# Begin Source File

SOURCE=.\res\outlook\dos.ico
# End Source File
# Begin Source File

SOURCE=.\res\downfile.ico
# End Source File
# Begin Source File

SOURCE=.\res\outlook\exit.ico
# End Source File
# Begin Source File

SOURCE=.\res\outlook\File.ico
# End Source File
# Begin Source File

SOURCE=.\res\FileToolBar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\FileToolBarDisable.bmp
# End Source File
# Begin Source File

SOURCE=.\res\FileToolBarHot.bmp
# End Source File
# Begin Source File

SOURCE=.\res\FileToolBarNormal.bmp
# End Source File
# Begin Source File

SOURCE=.\res\outlook\FileTran.ico
# End Source File
# Begin Source File

SOURCE=.\res\folders.bmp
# End Source File
# Begin Source File

SOURCE=.\res\outlook\Help.ico
# End Source File
# Begin Source File

SOURCE=.\res\outlook\home.ico
# End Source File
# Begin Source File

SOURCE=.\res\MainToolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\NetBot.ico
# End Source File
# Begin Source File

SOURCE=.\res\NetBot.rc2
# End Source File
# Begin Source File

SOURCE=.\res\online.ico
# End Source File
# Begin Source File

SOURCE=.\res\online2.ico
# End Source File
# Begin Source File

SOURCE=.\res\OnLineToolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\outlook\process.ico
# End Source File
# Begin Source File

SOURCE=.\res\outlook\remote.ico
# End Source File
# Begin Source File

SOURCE=.\res\outlook\screen.ico
# End Source File
# Begin Source File

SOURCE=.\res\outlook\server.ico
# End Source File
# Begin Source File

SOURCE=.\res\outlook\skin1.bin
# End Source File
# Begin Source File

SOURCE=.\res\small.ico
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\outlook\UPDATE.ico
# End Source File
# Begin Source File

SOURCE=.\res\upfile.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\res\xp.xml
# End Source File
# End Target
# End Project

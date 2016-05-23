# Microsoft Developer Studio Project File - Name="svchost" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=svchost - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "svchost.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "svchost.mak" CFG="svchost - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "svchost - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "svchost - Win32 Dll" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "svchost - Win32 Exe Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "svchost - Win32 Dll Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "svchost - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SVCHOST_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\\Seu_lib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "ZLIB_WINAPI" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /version:1.0 /pdb:none /machine:I386 /out:"..\nb.exe"
# SUBTRACT LINK32 /map

!ELSEIF  "$(CFG)" == "svchost - Win32 Dll"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "svchost___Win32_Dll"
# PROP BASE Intermediate_Dir "svchost___Win32_Dll"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "svchostDll"
# PROP Intermediate_Dir "svchostDll"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "..\\Seu_lib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "ZLIB_WINAPI" /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\\Seu_lib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "ZLIB_WINAPI" /D "DLLBUILD" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /version:1.0 /pdb:none /machine:I386 /out:"..\nb.exe"
# SUBTRACT BASE LINK32 /map
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /version:1.0 /dll /pdb:none /machine:I386 /def:"svchost.def" /out:"..\Svc.dll"

!ELSEIF  "$(CFG)" == "svchost - Win32 Exe Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "svchost___Win32_Exe_Debug"
# PROP BASE Intermediate_Dir "svchost___Win32_Exe_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release\Debug"
# PROP Intermediate_Dir "Release\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "..\\Seu_lib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "ZLIB_WINAPI" /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MTd /W3 /GX /Od /I "..\\Seu_lib" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "ZLIB_WINAPI" /D "DE" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /version:1.0 /pdb:none /machine:I386 /out:"..\nb.exe"
# SUBTRACT BASE LINK32 /map
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /version:1.0 /map /debug /machine:I386 /nodefaultlib:"msvcrt.lib" /nodefaultlib:"libcmt.lib" /out:"..\nb.exe"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "svchost - Win32 Dll Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "svchost___Win32_Dll_Debug"
# PROP BASE Intermediate_Dir "svchost___Win32_Dll_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "svchostDll\Debug"
# PROP Intermediate_Dir "svchostDll\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "..\\Seu_lib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "ZLIB_WINAPI" /D "DLLBUILD" /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /Od /I "..\\Seu_lib" /D "WIN32" /D "DE" /D "_WINDOWS" /D "_MBCS" /D "ZLIB_WINAPI" /D "DLLBUILD" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /version:1.0 /dll /pdb:none /machine:I386 /def:"svchost.def" /out:"..\Svc.dll"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /version:1.0 /dll /map /debug /machine:I386 /def:"svchost.def" /out:"..\Svc.dll"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "svchost - Win32 Release"
# Name "svchost - Win32 Dll"
# Name "svchost - Win32 Exe Debug"
# Name "svchost - Win32 Dll Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\Seu_lib\Command.cpp
# End Source File
# Begin Source File

SOURCE=..\Seu_lib\DosShell.cpp
# End Source File
# Begin Source File

SOURCE=..\Seu_lib\FileOperator.cpp
# End Source File
# Begin Source File

SOURCE=..\Seu_lib\Functions.cpp
# End Source File
# Begin Source File

SOURCE=..\Seu_lib\Process.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\svchost.cpp
# End Source File
# Begin Source File

SOURCE=.\svchost.rc
# End Source File
# Begin Source File

SOURCE=..\Seu_lib\SystemInfo.cpp
# ADD CPP /MT
# End Source File
# Begin Source File

SOURCE=..\Seu_lib\VideoCap.cpp
# End Source File
# Begin Source File

SOURCE=..\Seu_lib\XScreenXor.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\Seu_lib\Command.h
# End Source File
# Begin Source File

SOURCE=..\Seu_lib\DosShell.h
# End Source File
# Begin Source File

SOURCE=..\Seu_lib\FileOperator.h
# End Source File
# Begin Source File

SOURCE=..\Seu_lib\Functions.h
# End Source File
# Begin Source File

SOURCE=..\Seu_lib\Process.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\svchost.h
# End Source File
# Begin Source File

SOURCE=..\Seu_lib\SystemInfo.h
# End Source File
# Begin Source File

SOURCE=..\Seu_lib\VideoCap.h
# End Source File
# Begin Source File

SOURCE=..\Seu_lib\XScreenXor.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project

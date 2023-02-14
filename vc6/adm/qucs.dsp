# Microsoft Developer Studio Project File - Name="qucs" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=qucs - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "qucs.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "qucs.mak" CFG="qucs - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "qucs - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "qucs - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "qucs - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "C:\PRADISWORK\src\Qucs\adm\Release"
# PROP Intermediate_Dir "C:\PRADISWORK\src\Qucs\adm\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE F90 /compile_only /include:"Release/" /nologo /warn:nofileopt
# ADD F90 /browser /compile_only /include:"Release/" /nologo /warn:nofileopt
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /Gm /Gi /GX /Zi /O2 /Ob0 /I "../Qucs" /I "$(QTDIR3)/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "UNICODE" /D "QT_DLL" /D "QT_THREAD_SUPPORT" /D "QT_ACCESSIBILITY_SUPPORT" /D "HAVE_CONFIG_H" /D "__MINGW32__" /FR /YX /FD /c
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 "$(QTDIR3)\lib\qt-mt334.lib" /nologo /subsystem:windows /dll /machine:I386 /nodefaultlib:"msvcrt.lib" /out:"C:\DINAMA\Qucs\bin\PRADIS_scheme_parser.dll"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "qucs - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "C:\PRADISWORK\lib\debug-vc6"
# PROP Intermediate_Dir "C:\PRADISWORK\lib\debug-vc6"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE F90 /check:bounds /compile_only /debug:full /include:"Debug/" /nologo /traceback /warn:argument_checking /warn:nofileopt
# ADD F90 /check:bounds /compile_only /debug:full /include:"Debug/" /nologo /traceback /warn:argument_checking /warn:nofileopt
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /I "../Qucs" /I "$(QTDIR3)/include" /D "__MINGW32__" /D "WIN32" /D "_DEBUG" /D "_USRDLL" /D "_WINDOWS" /D "UNICODE" /D "QT_DLL" /D "QT_THREAD_SUPPORT" /D "QT_ACCESSIBILITY_SUPPORT" /D "HAVE_CONFIG_H" /FAs /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# SUBTRACT BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 "$(QTDIR3)\lib\qt-mt334.lib" "opengl32.lib" "glu32.lib" "delayimp.lib" "kernel32.lib" "user32.lib" "gdi32.lib" "comdlg32.lib" "advapi32.lib" "shell32.lib" "ole32.lib" "oleaut32.lib" "uuid.lib" "imm32.lib" "winmm.lib" "wsock32.lib" "winspool.lib" "kernel32.lib" "user32.lib" "gdi32.lib" "comdlg32.lib" "advapi32.lib" "shell32.lib" "ole32.lib" "oleaut32.lib" "uuid.lib" "imm32.lib" "winmm.lib" "wsock32.lib" "winspool.lib" /nologo /subsystem:windows /dll /debug /machine:I386 /nodefaultlib:"msvcrt.lib" /def:".\parser\parser.def" /out:"C:\DINAMA\Qucs\bin\PRADIS_scheme_parser.dll" /pdbtype:sept /libpath:"$(QTDIR)\lib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "qucs - Win32 Release"
# Name "qucs - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat;f90;for;f;fpp"
# Begin Group "qucs"

# PROP Default_Filter ""
# Begin Group "components"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\qucs\components\component.cpp
# End Source File
# Begin Source File

SOURCE=..\qucs\components\component.h
# End Source File
# End Group
# Begin Group "pradis"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\qucs\pradis\PRADISComponent.cpp
# End Source File
# Begin Source File

SOURCE=..\qucs\pradis\PRADISComponent.h
# End Source File
# Begin Source File

SOURCE=..\qucs\pradis\PRADISData.cpp
# End Source File
# Begin Source File

SOURCE=..\qucs\pradis\PRADISData.h
# End Source File
# Begin Source File

SOURCE=..\qucs\pradis\PRADISGround.cpp
# End Source File
# Begin Source File

SOURCE=..\qucs\pradis\PRADISGround.h
# End Source File
# Begin Source File

SOURCE=..\qucs\pradis\PRADISSubcirPort.cpp
# End Source File
# Begin Source File

SOURCE=..\qucs\pradis\PRADISSubcirPort.h
# End Source File
# Begin Source File

SOURCE=..\qucs\pradis\PRADISSubScheme.cpp
# End Source File
# Begin Source File

SOURCE=..\qucs\pradis\PRADISSubScheme.h
# End Source File
# Begin Source File

SOURCE=..\qucs\pradis\PSLSubScheme.cpp
# End Source File
# Begin Source File

SOURCE=..\qucs\pradis\PSLSubScheme.h
# End Source File
# End Group
# Begin Group "diagrams"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\qucs\diagrams\curvediagram.cpp
# End Source File
# Begin Source File

SOURCE=..\qucs\diagrams\curvediagram.h
# End Source File
# Begin Source File

SOURCE=..\qucs\diagrams\diagram.cpp
# End Source File
# Begin Source File

SOURCE=..\qucs\diagrams\diagram.h
# End Source File
# Begin Source File

SOURCE=..\qucs\diagrams\diagrams.h
# End Source File
# Begin Source File

SOURCE=..\qucs\diagrams\graph.cpp
# End Source File
# Begin Source File

SOURCE=..\qucs\diagrams\graph.h
# End Source File
# Begin Source File

SOURCE=..\qucs\diagrams\marker.cpp
# End Source File
# Begin Source File

SOURCE=..\qucs\diagrams\marker.h
# End Source File
# Begin Source File

SOURCE=..\qucs\diagrams\polardiagram.cpp
# End Source File
# Begin Source File

SOURCE=..\qucs\diagrams\polardiagram.h
# End Source File
# Begin Source File

SOURCE=..\qucs\diagrams\psdiagram.cpp
# End Source File
# Begin Source File

SOURCE=..\qucs\diagrams\psdiagram.h
# End Source File
# Begin Source File

SOURCE=..\qucs\diagrams\rect3ddiagram.cpp
# End Source File
# Begin Source File

SOURCE=..\qucs\diagrams\rect3ddiagram.h
# End Source File
# Begin Source File

SOURCE=..\qucs\diagrams\rectdiagram.cpp
# End Source File
# Begin Source File

SOURCE=..\qucs\diagrams\rectdiagram.h
# End Source File
# Begin Source File

SOURCE=..\qucs\diagrams\smithdiagram.cpp
# End Source File
# Begin Source File

SOURCE=..\qucs\diagrams\smithdiagram.h
# End Source File
# Begin Source File

SOURCE=..\qucs\diagrams\tabdiagram.cpp
# End Source File
# Begin Source File

SOURCE=..\qucs\diagrams\tabdiagram.h
# End Source File
# Begin Source File

SOURCE=..\qucs\diagrams\timingdiagram.cpp
# End Source File
# Begin Source File

SOURCE=..\qucs\diagrams\timingdiagram.h
# End Source File
# Begin Source File

SOURCE=..\qucs\diagrams\truthdiagram.cpp
# End Source File
# Begin Source File

SOURCE=..\qucs\diagrams\truthdiagram.h
# End Source File
# End Group
# Begin Group "paintings"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\qucs\paintings\arrow.cpp
# End Source File
# Begin Source File

SOURCE=..\qucs\paintings\arrow.h
# End Source File
# Begin Source File

SOURCE=..\qucs\paintings\ellipse.cpp
# End Source File
# Begin Source File

SOURCE=..\qucs\paintings\ellipse.h
# End Source File
# Begin Source File

SOURCE=..\qucs\paintings\ellipsearc.cpp
# End Source File
# Begin Source File

SOURCE=..\qucs\paintings\ellipsearc.h
# End Source File
# Begin Source File

SOURCE=..\qucs\paintings\graphicline.cpp
# End Source File
# Begin Source File

SOURCE=..\qucs\paintings\graphicline.h
# End Source File
# Begin Source File

SOURCE=..\qucs\paintings\graphictext.cpp
# End Source File
# Begin Source File

SOURCE=..\qucs\paintings\graphictext.h
# End Source File
# Begin Source File

SOURCE=..\qucs\paintings\id_text.cpp
# End Source File
# Begin Source File

SOURCE=..\qucs\paintings\id_text.h
# End Source File
# Begin Source File

SOURCE=..\qucs\paintings\painting.cpp
# End Source File
# Begin Source File

SOURCE=..\qucs\paintings\painting.h
# End Source File
# Begin Source File

SOURCE=..\qucs\paintings\paintings.h
# End Source File
# Begin Source File

SOURCE=..\qucs\paintings\portsymbol.cpp
# End Source File
# Begin Source File

SOURCE=..\qucs\paintings\portsymbol.h
# End Source File
# Begin Source File

SOURCE=..\qucs\paintings\rectangle.cpp
# End Source File
# Begin Source File

SOURCE=..\qucs\paintings\rectangle.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\qucs\element.cpp
# End Source File
# Begin Source File

SOURCE=..\qucs\element.h
# End Source File
# Begin Source File

SOURCE=..\qucs\main.h
# End Source File
# Begin Source File

SOURCE=..\qucs\mainqucs.cpp
# End Source File
# Begin Source File

SOURCE=..\qucs\node.cpp
# End Source File
# Begin Source File

SOURCE=..\qucs\node.h
# End Source File
# Begin Source File

SOURCE=..\qucs\schematic.cpp
# End Source File
# Begin Source File

SOURCE=..\qucs\schematic.h

!IF  "$(CFG)" == "qucs - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - MOCing schematic.h...
InputDir=\PRADISWORK\src\Qucs\qucs
InputPath=..\qucs\schematic.h
InputName=schematic

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"%qtdir3%\bin\moc.exe" "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ELSEIF  "$(CFG)" == "qucs - Win32 Debug"

# PROP Ignore_Default_Tool 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\qucs\schematic_element.cpp
# End Source File
# Begin Source File

SOURCE=..\qucs\schParser.cpp
# End Source File
# Begin Source File

SOURCE=..\qucs\wire.cpp
# End Source File
# Begin Source File

SOURCE=..\qucs\wire.h
# End Source File
# Begin Source File

SOURCE=..\qucs\wirelabel.cpp
# End Source File
# Begin Source File

SOURCE=..\qucs\wirelabel.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\qucs\BaseElement.cpp
# End Source File
# Begin Source File

SOURCE=.\parser\main.cpp
# End Source File
# Begin Source File

SOURCE=.\parser\parser.def

!IF  "$(CFG)" == "qucs - Win32 Release"

!ELSEIF  "$(CFG)" == "qucs - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\parserComponent.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=..\qucs\BaseElement.h
# End Source File
# Begin Source File

SOURCE=..\qucs\config.h
# End Source File
# Begin Source File

SOURCE=.\parserComponent.h
# End Source File
# Begin Source File

SOURCE=..\qucs\schParser.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project

# Microsoft Developer Studio Project File - Name="Jpglib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Jpglib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Jpglib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Jpglib.mak" CFG="Jpglib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Jpglib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Jpglib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Projects/CDL/TestApp", GLAAAAAA"
# PROP Scc_LocalPath "..\..\CDL\testapp"
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Jpglib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\lib"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_DLL" /YX /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\cdl\inc" /I "bogus_headers_for_win32" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_LIB" /D "_MBCS" /YX /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Jpglib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\lib"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_DLL" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\cdl\inc" /I "bogus_headers_for_win32" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_LIB" /D "_MBCS" /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\Lib\Jpglib_d.lib"

!ENDIF 

# Begin Target

# Name "Jpglib - Win32 Release"
# Name "Jpglib - Win32 Debug"
# Begin Source File

SOURCE=.\jcapimin.c
DEP_CPP_JCAPI=\
	".\bogus_headers_for_win32\strings.h"\
	".\jconfig.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	
NODEP_CPP_JCAPI=\
	".\igc_fileapi.h"\
	
# End Source File
# Begin Source File

SOURCE=.\jcapistd.c
DEP_CPP_JCAPIS=\
	".\bogus_headers_for_win32\strings.h"\
	".\jconfig.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	
NODEP_CPP_JCAPIS=\
	".\igc_fileapi.h"\
	
# End Source File
# Begin Source File

SOURCE=.\jccoefct.c
DEP_CPP_JCCOE=\
	".\bogus_headers_for_win32\strings.h"\
	".\jconfig.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	
NODEP_CPP_JCCOE=\
	".\igc_fileapi.h"\
	
# End Source File
# Begin Source File

SOURCE=.\jccolor.c
DEP_CPP_JCCOL=\
	".\bogus_headers_for_win32\strings.h"\
	".\jconfig.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	
NODEP_CPP_JCCOL=\
	".\igc_fileapi.h"\
	
# End Source File
# Begin Source File

SOURCE=.\jcdctmgr.c
DEP_CPP_JCDCT=\
	".\bogus_headers_for_win32\strings.h"\
	".\jconfig.h"\
	".\jdct.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	
NODEP_CPP_JCDCT=\
	".\igc_fileapi.h"\
	
# End Source File
# Begin Source File

SOURCE=.\jchuff.c
DEP_CPP_JCHUF=\
	".\bogus_headers_for_win32\strings.h"\
	".\jchuff.h"\
	".\jconfig.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	
NODEP_CPP_JCHUF=\
	".\igc_fileapi.h"\
	
# End Source File
# Begin Source File

SOURCE=.\jcinit.c
DEP_CPP_JCINI=\
	".\bogus_headers_for_win32\strings.h"\
	".\jconfig.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	
NODEP_CPP_JCINI=\
	".\igc_fileapi.h"\
	
# End Source File
# Begin Source File

SOURCE=.\jcmainct.c
DEP_CPP_JCMAI=\
	".\bogus_headers_for_win32\strings.h"\
	".\jconfig.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	
NODEP_CPP_JCMAI=\
	".\igc_fileapi.h"\
	
# End Source File
# Begin Source File

SOURCE=.\jcmarker.c
DEP_CPP_JCMAR=\
	".\bogus_headers_for_win32\strings.h"\
	".\jconfig.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	
NODEP_CPP_JCMAR=\
	".\igc_fileapi.h"\
	
# End Source File
# Begin Source File

SOURCE=.\jcmaster.c
DEP_CPP_JCMAS=\
	".\bogus_headers_for_win32\strings.h"\
	".\jconfig.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	
NODEP_CPP_JCMAS=\
	".\igc_fileapi.h"\
	
# End Source File
# Begin Source File

SOURCE=.\jcomapi.c
DEP_CPP_JCOMA=\
	".\bogus_headers_for_win32\strings.h"\
	".\jconfig.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	
NODEP_CPP_JCOMA=\
	".\igc_fileapi.h"\
	
# End Source File
# Begin Source File

SOURCE=.\jcparam.c
DEP_CPP_JCPAR=\
	".\bogus_headers_for_win32\strings.h"\
	".\jconfig.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	
NODEP_CPP_JCPAR=\
	".\igc_fileapi.h"\
	
# End Source File
# Begin Source File

SOURCE=.\jcphuff.c
DEP_CPP_JCPHU=\
	".\bogus_headers_for_win32\strings.h"\
	".\jchuff.h"\
	".\jconfig.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	
NODEP_CPP_JCPHU=\
	".\igc_fileapi.h"\
	
# End Source File
# Begin Source File

SOURCE=.\jcprepct.c
DEP_CPP_JCPRE=\
	".\bogus_headers_for_win32\strings.h"\
	".\jconfig.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	
NODEP_CPP_JCPRE=\
	".\igc_fileapi.h"\
	
# End Source File
# Begin Source File

SOURCE=.\jcsample.c
DEP_CPP_JCSAM=\
	".\bogus_headers_for_win32\strings.h"\
	".\jconfig.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	
NODEP_CPP_JCSAM=\
	".\igc_fileapi.h"\
	
# End Source File
# Begin Source File

SOURCE=.\jctrans.c
DEP_CPP_JCTRA=\
	".\bogus_headers_for_win32\strings.h"\
	".\jconfig.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	
NODEP_CPP_JCTRA=\
	".\igc_fileapi.h"\
	
# End Source File
# Begin Source File

SOURCE=.\jdapimin.c
DEP_CPP_JDAPI=\
	".\bogus_headers_for_win32\strings.h"\
	".\jconfig.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	
NODEP_CPP_JDAPI=\
	".\igc_fileapi.h"\
	
# End Source File
# Begin Source File

SOURCE=.\jdapistd.c
DEP_CPP_JDAPIS=\
	".\bogus_headers_for_win32\strings.h"\
	".\jconfig.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	
NODEP_CPP_JDAPIS=\
	".\igc_fileapi.h"\
	
# End Source File
# Begin Source File

SOURCE=.\jdatadst.c
DEP_CPP_JDATA=\
	".\bogus_headers_for_win32\strings.h"\
	".\jconfig.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	
NODEP_CPP_JDATA=\
	".\igc_fileapi.h"\
	
# End Source File
# Begin Source File

SOURCE=.\jdatasrc.c
DEP_CPP_JDATAS=\
	".\bogus_headers_for_win32\strings.h"\
	".\jconfig.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	
NODEP_CPP_JDATAS=\
	".\igc_fileapi.h"\
	
# End Source File
# Begin Source File

SOURCE=.\jdcoefct.c
DEP_CPP_JDCOE=\
	".\bogus_headers_for_win32\strings.h"\
	".\jconfig.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	
NODEP_CPP_JDCOE=\
	".\igc_fileapi.h"\
	
# End Source File
# Begin Source File

SOURCE=.\jdcolor.c
DEP_CPP_JDCOL=\
	".\bogus_headers_for_win32\strings.h"\
	".\jconfig.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	
NODEP_CPP_JDCOL=\
	".\igc_fileapi.h"\
	
# End Source File
# Begin Source File

SOURCE=.\jddctmgr.c
DEP_CPP_JDDCT=\
	".\bogus_headers_for_win32\strings.h"\
	".\jconfig.h"\
	".\jdct.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	
NODEP_CPP_JDDCT=\
	".\igc_fileapi.h"\
	
# End Source File
# Begin Source File

SOURCE=.\jdhuff.c
DEP_CPP_JDHUF=\
	".\bogus_headers_for_win32\strings.h"\
	".\jconfig.h"\
	".\jdhuff.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	
NODEP_CPP_JDHUF=\
	".\igc_fileapi.h"\
	
# End Source File
# Begin Source File

SOURCE=.\jdinput.c
DEP_CPP_JDINP=\
	".\bogus_headers_for_win32\strings.h"\
	".\jconfig.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	
NODEP_CPP_JDINP=\
	".\igc_fileapi.h"\
	
# End Source File
# Begin Source File

SOURCE=.\jdmainct.c
DEP_CPP_JDMAI=\
	".\bogus_headers_for_win32\strings.h"\
	".\jconfig.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	
NODEP_CPP_JDMAI=\
	".\igc_fileapi.h"\
	
# End Source File
# Begin Source File

SOURCE=.\jdmarker.c
DEP_CPP_JDMAR=\
	".\bogus_headers_for_win32\strings.h"\
	".\jconfig.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	
NODEP_CPP_JDMAR=\
	".\igc_fileapi.h"\
	
# End Source File
# Begin Source File

SOURCE=.\jdmaster.c
DEP_CPP_JDMAS=\
	".\bogus_headers_for_win32\strings.h"\
	".\jconfig.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	
NODEP_CPP_JDMAS=\
	".\igc_fileapi.h"\
	
# End Source File
# Begin Source File

SOURCE=.\jdmerge.c
DEP_CPP_JDMER=\
	".\bogus_headers_for_win32\strings.h"\
	".\jconfig.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	
NODEP_CPP_JDMER=\
	".\igc_fileapi.h"\
	
# End Source File
# Begin Source File

SOURCE=.\jdphuff.c
DEP_CPP_JDPHU=\
	".\bogus_headers_for_win32\strings.h"\
	".\jconfig.h"\
	".\jdhuff.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	
NODEP_CPP_JDPHU=\
	".\igc_fileapi.h"\
	
# End Source File
# Begin Source File

SOURCE=.\jdpostct.c
DEP_CPP_JDPOS=\
	".\bogus_headers_for_win32\strings.h"\
	".\jconfig.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	
NODEP_CPP_JDPOS=\
	".\igc_fileapi.h"\
	
# End Source File
# Begin Source File

SOURCE=.\jdsample.c
DEP_CPP_JDSAM=\
	".\bogus_headers_for_win32\strings.h"\
	".\jconfig.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	
NODEP_CPP_JDSAM=\
	".\igc_fileapi.h"\
	
# End Source File
# Begin Source File

SOURCE=.\jdtrans.c
DEP_CPP_JDTRA=\
	".\bogus_headers_for_win32\strings.h"\
	".\jconfig.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	
NODEP_CPP_JDTRA=\
	".\igc_fileapi.h"\
	
# End Source File
# Begin Source File

SOURCE=.\jerror.c
DEP_CPP_JERRO=\
	".\bogus_headers_for_win32\strings.h"\
	".\jconfig.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	".\jversion.h"\
	
NODEP_CPP_JERRO=\
	".\igc_fileapi.h"\
	
# End Source File
# Begin Source File

SOURCE=.\jfdctflt.c
DEP_CPP_JFDCT=\
	".\bogus_headers_for_win32\strings.h"\
	".\jconfig.h"\
	".\jdct.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	
NODEP_CPP_JFDCT=\
	".\igc_fileapi.h"\
	
# End Source File
# Begin Source File

SOURCE=.\jfdctfst.c
DEP_CPP_JFDCTF=\
	".\bogus_headers_for_win32\strings.h"\
	".\jconfig.h"\
	".\jdct.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	
NODEP_CPP_JFDCTF=\
	".\igc_fileapi.h"\
	
# End Source File
# Begin Source File

SOURCE=.\jfdctint.c
DEP_CPP_JFDCTI=\
	".\bogus_headers_for_win32\strings.h"\
	".\jconfig.h"\
	".\jdct.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	
NODEP_CPP_JFDCTI=\
	".\igc_fileapi.h"\
	
# End Source File
# Begin Source File

SOURCE=.\jidctflt.c
DEP_CPP_JIDCT=\
	".\bogus_headers_for_win32\strings.h"\
	".\jconfig.h"\
	".\jdct.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	
NODEP_CPP_JIDCT=\
	".\igc_fileapi.h"\
	
# End Source File
# Begin Source File

SOURCE=.\jidctfst.c
DEP_CPP_JIDCTF=\
	".\bogus_headers_for_win32\strings.h"\
	".\jconfig.h"\
	".\jdct.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	
NODEP_CPP_JIDCTF=\
	".\igc_fileapi.h"\
	
# End Source File
# Begin Source File

SOURCE=.\jidctint.c
DEP_CPP_JIDCTI=\
	".\bogus_headers_for_win32\strings.h"\
	".\jconfig.h"\
	".\jdct.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	
NODEP_CPP_JIDCTI=\
	".\igc_fileapi.h"\
	
# End Source File
# Begin Source File

SOURCE=.\jidctred.c
DEP_CPP_JIDCTR=\
	".\bogus_headers_for_win32\strings.h"\
	".\jconfig.h"\
	".\jdct.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	
NODEP_CPP_JIDCTR=\
	".\igc_fileapi.h"\
	
# End Source File
# Begin Source File

SOURCE=.\jmemansi.c
DEP_CPP_JMEMA=\
	".\bogus_headers_for_win32\Files.h"\
	".\bogus_headers_for_win32\strings.h"\
	".\jconfig.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmemsys.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	
NODEP_CPP_JMEMA=\
	".\igc_fileapi.h"\
	
# End Source File
# Begin Source File

SOURCE=.\jmemmgr.c
DEP_CPP_JMEMM=\
	".\bogus_headers_for_win32\Files.h"\
	".\bogus_headers_for_win32\strings.h"\
	".\jconfig.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmemsys.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	
NODEP_CPP_JMEMM=\
	".\igc_fileapi.h"\
	
# End Source File
# Begin Source File

SOURCE=.\jquant1.c
DEP_CPP_JQUAN=\
	".\bogus_headers_for_win32\strings.h"\
	".\jconfig.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	
NODEP_CPP_JQUAN=\
	".\igc_fileapi.h"\
	
# End Source File
# Begin Source File

SOURCE=.\jquant2.c
DEP_CPP_JQUANT=\
	".\bogus_headers_for_win32\strings.h"\
	".\jconfig.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	
NODEP_CPP_JQUANT=\
	".\igc_fileapi.h"\
	
# End Source File
# Begin Source File

SOURCE=.\jutils.c
DEP_CPP_JUTIL=\
	".\bogus_headers_for_win32\strings.h"\
	".\jconfig.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	
NODEP_CPP_JUTIL=\
	".\igc_fileapi.h"\
	
# End Source File
# Begin Source File

SOURCE=.\rdbmp.c
DEP_CPP_RDBMP=\
	".\bogus_headers_for_win32\strings.h"\
	".\cderror.h"\
	".\cdjpeg.h"\
	".\jconfig.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	
NODEP_CPP_RDBMP=\
	".\igc_fileapi.h"\
	
# End Source File
# Begin Source File

SOURCE=.\rdgif.c
DEP_CPP_RDGIF=\
	".\bogus_headers_for_win32\strings.h"\
	".\cderror.h"\
	".\cdjpeg.h"\
	".\jconfig.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	
NODEP_CPP_RDGIF=\
	".\igc_fileapi.h"\
	
# End Source File
# Begin Source File

SOURCE=.\wrgif.c
DEP_CPP_WRGIF=\
	".\bogus_headers_for_win32\strings.h"\
	".\cderror.h"\
	".\cdjpeg.h"\
	".\jconfig.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	
NODEP_CPP_WRGIF=\
	".\igc_fileapi.h"\
	
# End Source File
# End Target
# End Project

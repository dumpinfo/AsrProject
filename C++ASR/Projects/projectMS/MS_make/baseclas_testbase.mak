# Microsoft Developer Studio Generated NMAKE File, Based on baseclas_testbase.dsp
!IF "$(CFG)" == ""
CFG=baseclas_testbase - Win32 Debug
!MESSAGE No configuration specified. Defaulting to baseclas_testbase - Win32\
 Debug.
!ENDIF 

!IF "$(CFG)" != "baseclas_testbase - Win32 Release" && "$(CFG)" !=\
 "baseclas_testbase - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "baseclas_testbase.mak" CFG="baseclas_testbase - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "baseclas_testbase - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "baseclas_testbase - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "baseclas_testbase - Win32 Release"

OUTDIR=.
INTDIR=.\..\objects
# Begin Custom Macros
OutDir=.
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\baseclas_testbase.exe"

!ELSE 

ALL : "$(OUTDIR)\baseclas_testbase.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\baseclas.obj"
	-@erase "$(INTDIR)\Diagnost.obj"
	-@erase "$(INTDIR)\Testbase.obj"
	-@erase "$(INTDIR)\Textclas.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\baseclas_testbase.exe"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D\
 "_MBCS" /Fp"$(INTDIR)\baseclas_testbase.pch" /YX /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=..\objects/
CPP_SBRS=.
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\baseclas_testbase.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:console /incremental:no /pdb:"$(OUTDIR)\baseclas_testbase.pdb"\
 /machine:I386 /out:"$(OUTDIR)\baseclas_testbase.exe" 
LINK32_OBJS= \
	"$(INTDIR)\baseclas.obj" \
	"$(INTDIR)\Diagnost.obj" \
	"$(INTDIR)\Testbase.obj" \
	"$(INTDIR)\Textclas.obj"

"$(OUTDIR)\baseclas_testbase.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "baseclas_testbase - Win32 Debug"

OUTDIR=.
INTDIR=.\..\objects
# Begin Custom Macros
OutDir=.
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\baseclas_testbase.exe" "$(OUTDIR)\baseclas_testbase.bsc"

!ELSE 

ALL : "$(OUTDIR)\baseclas_testbase.exe" "$(OUTDIR)\baseclas_testbase.bsc"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\baseclas.obj"
	-@erase "$(INTDIR)\baseclas.sbr"
	-@erase "$(INTDIR)\Diagnost.obj"
	-@erase "$(INTDIR)\Diagnost.sbr"
	-@erase "$(INTDIR)\Testbase.obj"
	-@erase "$(INTDIR)\Testbase.sbr"
	-@erase "$(INTDIR)\Textclas.obj"
	-@erase "$(INTDIR)\Textclas.sbr"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\baseclas_testbase.bsc"
	-@erase "$(OUTDIR)\baseclas_testbase.exe"
	-@erase "$(OUTDIR)\baseclas_testbase.pdb"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\baseclas_testbase.pch" /YX\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=..\objects/
CPP_SBRS=..\objects/
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\baseclas_testbase.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\baseclas.sbr" \
	"$(INTDIR)\Diagnost.sbr" \
	"$(INTDIR)\Testbase.sbr" \
	"$(INTDIR)\Textclas.sbr"

"$(OUTDIR)\baseclas_testbase.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:console /incremental:no /pdb:"$(OUTDIR)\baseclas_testbase.pdb"\
 /debug /machine:I386 /out:"$(OUTDIR)\baseclas_testbase.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\baseclas.obj" \
	"$(INTDIR)\Diagnost.obj" \
	"$(INTDIR)\Testbase.obj" \
	"$(INTDIR)\Textclas.obj"

"$(OUTDIR)\baseclas_testbase.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(CFG)" == "baseclas_testbase - Win32 Release" || "$(CFG)" ==\
 "baseclas_testbase - Win32 Debug"
SOURCE=..\..\..\Source\baseclas\baseclas.cpp
DEP_CPP_BASEC=\
	"..\..\..\source\baseclas\baseclas.h"\
	"..\..\..\source\baseclas\baseclas.hpp"\
	"..\..\..\source\baseclas\boolean.h"\
	"..\..\..\source\baseclas\compatib.h"\
	"..\..\..\source\baseclas\defopt.h"\
	"..\..\..\source\baseclas\diagnost.h"\
	

!IF  "$(CFG)" == "baseclas_testbase - Win32 Release"


"$(INTDIR)\baseclas.obj" : $(SOURCE) $(DEP_CPP_BASEC) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "baseclas_testbase - Win32 Debug"


"$(INTDIR)\baseclas.obj"	"$(INTDIR)\baseclas.sbr" : $(SOURCE) $(DEP_CPP_BASEC)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\Source\baseclas\Diagnost.cpp
DEP_CPP_DIAGN=\
	"..\..\..\source\baseclas\boolean.h"\
	"..\..\..\source\baseclas\compatib.h"\
	"..\..\..\source\baseclas\defopt.h"\
	"..\..\..\source\baseclas\diagnost.h"\
	

!IF  "$(CFG)" == "baseclas_testbase - Win32 Release"


"$(INTDIR)\Diagnost.obj" : $(SOURCE) $(DEP_CPP_DIAGN) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "baseclas_testbase - Win32 Debug"


"$(INTDIR)\Diagnost.obj"	"$(INTDIR)\Diagnost.sbr" : $(SOURCE) $(DEP_CPP_DIAGN)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\Source\baseclas\Testbase.cpp
DEP_CPP_TESTB=\
	"..\..\..\source\baseclas\baseclas.h"\
	"..\..\..\source\baseclas\baseclas.hpp"\
	"..\..\..\source\baseclas\boolean.h"\
	"..\..\..\source\baseclas\compatib.h"\
	"..\..\..\source\baseclas\defopt.h"\
	"..\..\..\source\baseclas\diagnost.h"\
	"..\..\..\source\baseclas\textclas.h"\
	

!IF  "$(CFG)" == "baseclas_testbase - Win32 Release"


"$(INTDIR)\Testbase.obj" : $(SOURCE) $(DEP_CPP_TESTB) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "baseclas_testbase - Win32 Debug"


"$(INTDIR)\Testbase.obj"	"$(INTDIR)\Testbase.sbr" : $(SOURCE) $(DEP_CPP_TESTB)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\Source\baseclas\Textclas.cpp
DEP_CPP_TEXTC=\
	"..\..\..\source\baseclas\baseclas.h"\
	"..\..\..\source\baseclas\baseclas.hpp"\
	"..\..\..\source\baseclas\boolean.h"\
	"..\..\..\source\baseclas\compatib.h"\
	"..\..\..\source\baseclas\defopt.h"\
	"..\..\..\source\baseclas\diagnost.h"\
	"..\..\..\source\baseclas\textclas.h"\
	

!IF  "$(CFG)" == "baseclas_testbase - Win32 Release"


"$(INTDIR)\Textclas.obj" : $(SOURCE) $(DEP_CPP_TEXTC) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "baseclas_testbase - Win32 Debug"


"$(INTDIR)\Textclas.obj"	"$(INTDIR)\Textclas.sbr" : $(SOURCE) $(DEP_CPP_TEXTC)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 


!ENDIF 


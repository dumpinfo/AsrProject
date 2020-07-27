# Microsoft Developer Studio Generated NMAKE File, Based on utils.dsp
!IF "$(CFG)" == ""
CFG=utils - Win32 Debug
!MESSAGE No configuration specified. Defaulting to utils - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "utils - Win32 Release" && "$(CFG)" != "utils - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "utils.mak" CFG="utils - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "utils - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "utils - Win32 Debug" (based on "Win32 (x86) Console Application")
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

!IF  "$(CFG)" == "utils - Win32 Release"

OUTDIR=.
INTDIR=.\..\objects
# Begin Custom Macros
OutDir=.
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\utils.exe"

!ELSE 

ALL : "$(OUTDIR)\utils.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\utils.exe"
	-@erase "..\objects\baseclas.obj"
	-@erase "..\objects\Diagnost.obj"
	-@erase "..\objects\multifop.obj"
	-@erase "..\objects\Polimorf.obj"
	-@erase "..\objects\resconf.obj"
	-@erase "..\objects\Textclas.obj"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D\
 "_MBCS" /Fp"$(INTDIR)\utils.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=..\objects/
CPP_SBRS=.
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\utils.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:console /incremental:no /pdb:"$(OUTDIR)\utils.pdb" /machine:I386\
 /out:"$(OUTDIR)\utils.exe" 
LINK32_OBJS= \
	"..\objects\baseclas.obj" \
	"..\objects\Diagnost.obj" \
	"..\objects\multifop.obj" \
	"..\objects\Polimorf.obj" \
	"..\objects\resconf.obj" \
	"..\objects\Textclas.obj"

"$(OUTDIR)\utils.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "utils - Win32 Debug"

OUTDIR=.
INTDIR=.\..\objects
# Begin Custom Macros
OutDir=.
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\utils.exe" "$(OUTDIR)\utils.bsc"

!ELSE 

ALL : "$(OUTDIR)\utils.exe" "$(OUTDIR)\utils.bsc"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\utils.bsc"
	-@erase "$(OUTDIR)\utils.exe"
	-@erase "$(OUTDIR)\utils.pdb"
	-@erase "..\objects\baseclas.obj"
	-@erase "..\objects\baseclas.sbr"
	-@erase "..\objects\Diagnost.obj"
	-@erase "..\objects\Diagnost.sbr"
	-@erase "..\objects\multifop.obj"
	-@erase "..\objects\multifop.sbr"
	-@erase "..\objects\Polimorf.obj"
	-@erase "..\objects\Polimorf.sbr"
	-@erase "..\objects\resconf.obj"
	-@erase "..\objects\resconf.sbr"
	-@erase "..\objects\Textclas.obj"
	-@erase "..\objects\Textclas.sbr"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\utils.pch" /YX /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=..\objects/
CPP_SBRS=..\objects/
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\utils.bsc" 
BSC32_SBRS= \
	"..\objects\baseclas.sbr" \
	"..\objects\Diagnost.sbr" \
	"..\objects\multifop.sbr" \
	"..\objects\Polimorf.sbr" \
	"..\objects\resconf.sbr" \
	"..\objects\Textclas.sbr"

"$(OUTDIR)\utils.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:console /incremental:no /pdb:"$(OUTDIR)\utils.pdb" /debug\
 /machine:I386 /out:"$(OUTDIR)\utils.exe" /pdbtype:sept 
LINK32_OBJS= \
	"..\objects\baseclas.obj" \
	"..\objects\Diagnost.obj" \
	"..\objects\multifop.obj" \
	"..\objects\Polimorf.obj" \
	"..\objects\resconf.obj" \
	"..\objects\Textclas.obj"

"$(OUTDIR)\utils.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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


!IF "$(CFG)" == "utils - Win32 Release" || "$(CFG)" == "utils - Win32 Debug"
SOURCE=..\..\..\Source\baseclas\baseclas.cpp
DEP_CPP_BASEC=\
	"..\..\..\source\baseclas\baseclas.h"\
	"..\..\..\source\baseclas\baseclas.hpp"\
	"..\..\..\source\baseclas\boolean.h"\
	"..\..\..\source\baseclas\compatib.h"\
	"..\..\..\source\baseclas\defopt.h"\
	"..\..\..\source\baseclas\diagnost.h"\
	
INTDIR_SRC=.\..\objects

!IF  "$(CFG)" == "utils - Win32 Release"

CPP_SWITCHES=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D\
 "_MBCS" /Fp"$(INTDIR_SRC)\utils.pch" /YX /Fo"$(INTDIR_SRC)\\"\
 /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\baseclas.obj" : $(SOURCE) $(DEP_CPP_BASEC) "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "utils - Win32 Debug"

CPP_SWITCHES=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /D "_MBCS" /FR"$(INTDIR_SRC)\\" /Fp"$(INTDIR_SRC)\utils.pch" /YX\
 /Fo"$(INTDIR_SRC)\\" /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\baseclas.obj"	"..\objects\baseclas.sbr" : $(SOURCE)\
 $(DEP_CPP_BASEC) "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\..\Source\baseclas\Diagnost.cpp
DEP_CPP_DIAGN=\
	"..\..\..\source\baseclas\boolean.h"\
	"..\..\..\source\baseclas\compatib.h"\
	"..\..\..\source\baseclas\defopt.h"\
	"..\..\..\source\baseclas\diagnost.h"\
	
INTDIR_SRC=.\..\objects

!IF  "$(CFG)" == "utils - Win32 Release"

CPP_SWITCHES=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D\
 "_MBCS" /Fp"$(INTDIR_SRC)\utils.pch" /YX /Fo"$(INTDIR_SRC)\\"\
 /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\Diagnost.obj" : $(SOURCE) $(DEP_CPP_DIAGN) "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "utils - Win32 Debug"

CPP_SWITCHES=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /D "_MBCS" /FR"$(INTDIR_SRC)\\" /Fp"$(INTDIR_SRC)\utils.pch" /YX\
 /Fo"$(INTDIR_SRC)\\" /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\Diagnost.obj"	"..\objects\Diagnost.sbr" : $(SOURCE)\
 $(DEP_CPP_DIAGN) "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\..\Source\utils\multifop.cpp
DEP_CPP_MULTI=\
	"..\..\..\source\baseclas\baseclas.h"\
	"..\..\..\source\baseclas\baseclas.hpp"\
	"..\..\..\source\baseclas\boolean.h"\
	"..\..\..\source\baseclas\compatib.h"\
	"..\..\..\source\baseclas\defopt.h"\
	"..\..\..\source\baseclas\diagnost.h"\
	"..\..\..\source\baseclas\polimorf.h"\
	"..\..\..\source\baseclas\textclas.h"\
	"..\..\..\source\resconf\resconf.h"\
	"..\..\..\source\utils\multifop.h"\
	"..\..\..\source\vetclas\vetclas.h"\
	"..\..\..\source\vetclas\vetclas.hpp"\
	
INTDIR_SRC=.\..\objects

!IF  "$(CFG)" == "utils - Win32 Release"

CPP_SWITCHES=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D\
 "_MBCS" /Fp"$(INTDIR_SRC)\utils.pch" /YX /Fo"$(INTDIR_SRC)\\"\
 /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\multifop.obj" : $(SOURCE) $(DEP_CPP_MULTI) "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "utils - Win32 Debug"

CPP_SWITCHES=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /D "_MBCS" /FR"$(INTDIR_SRC)\\" /Fp"$(INTDIR_SRC)\utils.pch" /YX\
 /Fo"$(INTDIR_SRC)\\" /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\multifop.obj"	"..\objects\multifop.sbr" : $(SOURCE)\
 $(DEP_CPP_MULTI) "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\..\Source\baseclas\Polimorf.cpp
DEP_CPP_POLIM=\
	"..\..\..\source\baseclas\baseclas.h"\
	"..\..\..\source\baseclas\baseclas.hpp"\
	"..\..\..\source\baseclas\boolean.h"\
	"..\..\..\source\baseclas\compatib.h"\
	"..\..\..\source\baseclas\defopt.h"\
	"..\..\..\source\baseclas\diagnost.h"\
	"..\..\..\source\baseclas\polimorf.h"\
	"..\..\..\source\baseclas\textclas.h"\
	
INTDIR_SRC=.\..\objects

!IF  "$(CFG)" == "utils - Win32 Release"

CPP_SWITCHES=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D\
 "_MBCS" /Fp"$(INTDIR_SRC)\utils.pch" /YX /Fo"$(INTDIR_SRC)\\"\
 /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\Polimorf.obj" : $(SOURCE) $(DEP_CPP_POLIM) "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "utils - Win32 Debug"

CPP_SWITCHES=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /D "_MBCS" /FR"$(INTDIR_SRC)\\" /Fp"$(INTDIR_SRC)\utils.pch" /YX\
 /Fo"$(INTDIR_SRC)\\" /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\Polimorf.obj"	"..\objects\Polimorf.sbr" : $(SOURCE)\
 $(DEP_CPP_POLIM) "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\..\Source\resconf\resconf.cpp
DEP_CPP_RESCO=\
	"..\..\..\source\baseclas\baseclas.h"\
	"..\..\..\source\baseclas\baseclas.hpp"\
	"..\..\..\source\baseclas\boolean.h"\
	"..\..\..\source\baseclas\compatib.h"\
	"..\..\..\source\baseclas\defopt.h"\
	"..\..\..\source\baseclas\diagnost.h"\
	"..\..\..\source\baseclas\textclas.h"\
	"..\..\..\source\resconf\resconf.h"\
	"..\..\..\source\vetclas\vetclas.h"\
	"..\..\..\source\vetclas\vetclas.hpp"\
	
INTDIR_SRC=.\..\objects

!IF  "$(CFG)" == "utils - Win32 Release"

CPP_SWITCHES=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D\
 "_MBCS" /Fp"$(INTDIR_SRC)\utils.pch" /YX /Fo"$(INTDIR_SRC)\\"\
 /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\resconf.obj" : $(SOURCE) $(DEP_CPP_RESCO) "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "utils - Win32 Debug"

CPP_SWITCHES=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /D "_MBCS" /FR"$(INTDIR_SRC)\\" /Fp"$(INTDIR_SRC)\utils.pch" /YX\
 /Fo"$(INTDIR_SRC)\\" /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\resconf.obj"	"..\objects\resconf.sbr" : $(SOURCE) $(DEP_CPP_RESCO)\
 "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


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
	
INTDIR_SRC=.\..\objects

!IF  "$(CFG)" == "utils - Win32 Release"

CPP_SWITCHES=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D\
 "_MBCS" /Fp"$(INTDIR_SRC)\utils.pch" /YX /Fo"$(INTDIR_SRC)\\"\
 /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\Textclas.obj" : $(SOURCE) $(DEP_CPP_TEXTC) "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "utils - Win32 Debug"

CPP_SWITCHES=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /D "_MBCS" /FR"$(INTDIR_SRC)\\" /Fp"$(INTDIR_SRC)\utils.pch" /YX\
 /Fo"$(INTDIR_SRC)\\" /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\Textclas.obj"	"..\objects\Textclas.sbr" : $(SOURCE)\
 $(DEP_CPP_TEXTC) "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 


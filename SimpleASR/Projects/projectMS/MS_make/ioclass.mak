# Microsoft Developer Studio Generated NMAKE File, Based on ioclass.dsp
!IF "$(CFG)" == ""
CFG=ioclass - Win32 Debug
!MESSAGE No configuration specified. Defaulting to ioclass - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "ioclass - Win32 Release" && "$(CFG)" !=\
 "ioclass - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ioclass.mak" CFG="ioclass - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ioclass - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "ioclass - Win32 Debug" (based on "Win32 (x86) Console Application")
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

!IF  "$(CFG)" == "ioclass - Win32 Release"

OUTDIR=.
INTDIR=.\..\objects
# Begin Custom Macros
OutDir=.
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\ioclass.exe"

!ELSE 

ALL : "$(OUTDIR)\ioclass.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\ioclass.exe"
	-@erase "..\objects\arraycla.obj"
	-@erase "..\objects\baseclas.obj"
	-@erase "..\objects\diagclas.obj"
	-@erase "..\objects\Diagnost.obj"
	-@erase "..\objects\labelcl.obj"
	-@erase "..\objects\Polimorf.obj"
	-@erase "..\objects\resconf.obj"
	-@erase "..\objects\soundfil.obj"
	-@erase "..\objects\soundlab.obj"
	-@erase "..\objects\testione.obj"
	-@erase "..\objects\Textclas.obj"
	-@erase "..\objects\vetclas.obj"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D\
 "_MBCS" /Fp"$(INTDIR)\ioclass.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c\
 
CPP_OBJS=..\objects/
CPP_SBRS=.
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ioclass.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:console /incremental:no /pdb:"$(OUTDIR)\ioclass.pdb" /machine:I386\
 /out:"$(OUTDIR)\ioclass.exe" 
LINK32_OBJS= \
	"..\objects\arraycla.obj" \
	"..\objects\baseclas.obj" \
	"..\objects\diagclas.obj" \
	"..\objects\Diagnost.obj" \
	"..\objects\labelcl.obj" \
	"..\objects\Polimorf.obj" \
	"..\objects\resconf.obj" \
	"..\objects\soundfil.obj" \
	"..\objects\soundlab.obj" \
	"..\objects\testione.obj" \
	"..\objects\Textclas.obj" \
	"..\objects\vetclas.obj"

"$(OUTDIR)\ioclass.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ioclass - Win32 Debug"

OUTDIR=.
INTDIR=.\..\objects
# Begin Custom Macros
OutDir=.
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\ioclass.exe" "$(OUTDIR)\ioclass.bsc"

!ELSE 

ALL : "$(OUTDIR)\ioclass.exe" "$(OUTDIR)\ioclass.bsc"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\ioclass.bsc"
	-@erase "$(OUTDIR)\ioclass.exe"
	-@erase "$(OUTDIR)\ioclass.pdb"
	-@erase "..\objects\arraycla.obj"
	-@erase "..\objects\arraycla.sbr"
	-@erase "..\objects\baseclas.obj"
	-@erase "..\objects\baseclas.sbr"
	-@erase "..\objects\diagclas.obj"
	-@erase "..\objects\diagclas.sbr"
	-@erase "..\objects\Diagnost.obj"
	-@erase "..\objects\Diagnost.sbr"
	-@erase "..\objects\labelcl.obj"
	-@erase "..\objects\labelcl.sbr"
	-@erase "..\objects\Polimorf.obj"
	-@erase "..\objects\Polimorf.sbr"
	-@erase "..\objects\resconf.obj"
	-@erase "..\objects\resconf.sbr"
	-@erase "..\objects\soundfil.obj"
	-@erase "..\objects\soundfil.sbr"
	-@erase "..\objects\soundlab.obj"
	-@erase "..\objects\soundlab.sbr"
	-@erase "..\objects\testione.obj"
	-@erase "..\objects\testione.sbr"
	-@erase "..\objects\Textclas.obj"
	-@erase "..\objects\Textclas.sbr"
	-@erase "..\objects\vetclas.obj"
	-@erase "..\objects\vetclas.sbr"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\ioclass.pch" /YX /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=..\objects/
CPP_SBRS=..\objects/
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ioclass.bsc" 
BSC32_SBRS= \
	"..\objects\arraycla.sbr" \
	"..\objects\baseclas.sbr" \
	"..\objects\diagclas.sbr" \
	"..\objects\Diagnost.sbr" \
	"..\objects\labelcl.sbr" \
	"..\objects\Polimorf.sbr" \
	"..\objects\resconf.sbr" \
	"..\objects\soundfil.sbr" \
	"..\objects\soundlab.sbr" \
	"..\objects\testione.sbr" \
	"..\objects\Textclas.sbr" \
	"..\objects\vetclas.sbr"

"$(OUTDIR)\ioclass.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:console /incremental:no /pdb:"$(OUTDIR)\ioclass.pdb" /debug\
 /machine:I386 /out:"$(OUTDIR)\ioclass.exe" /pdbtype:sept 
LINK32_OBJS= \
	"..\objects\arraycla.obj" \
	"..\objects\baseclas.obj" \
	"..\objects\diagclas.obj" \
	"..\objects\Diagnost.obj" \
	"..\objects\labelcl.obj" \
	"..\objects\Polimorf.obj" \
	"..\objects\resconf.obj" \
	"..\objects\soundfil.obj" \
	"..\objects\soundlab.obj" \
	"..\objects\testione.obj" \
	"..\objects\Textclas.obj" \
	"..\objects\vetclas.obj"

"$(OUTDIR)\ioclass.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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


!IF "$(CFG)" == "ioclass - Win32 Release" || "$(CFG)" ==\
 "ioclass - Win32 Debug"
SOURCE=..\..\..\Source\vetclas\Arraycla.cpp
DEP_CPP_ARRAY=\
	"..\..\..\source\baseclas\baseclas.h"\
	"..\..\..\source\baseclas\baseclas.hpp"\
	"..\..\..\source\baseclas\boolean.h"\
	"..\..\..\source\baseclas\compatib.h"\
	"..\..\..\source\baseclas\defopt.h"\
	"..\..\..\source\baseclas\diagnost.h"\
	"..\..\..\source\vetclas\arraycla.h"\
	"..\..\..\source\vetclas\arraycla.hpp"\
	"..\..\..\source\vetclas\diagclas.h"\
	"..\..\..\source\vetclas\diagclas.hpp"\
	"..\..\..\source\vetclas\vetclas.h"\
	"..\..\..\source\vetclas\vetclas.hpp"\
	
INTDIR_SRC=.\..\objects

!IF  "$(CFG)" == "ioclass - Win32 Release"

CPP_SWITCHES=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D\
 "_MBCS" /Fp"$(INTDIR_SRC)\ioclass.pch" /YX /Fo"$(INTDIR_SRC)\\"\
 /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\arraycla.obj" : $(SOURCE) $(DEP_CPP_ARRAY) "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ioclass - Win32 Debug"

CPP_SWITCHES=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /D "_MBCS" /FR"$(INTDIR_SRC)\\" /Fp"$(INTDIR_SRC)\ioclass.pch" /YX\
 /Fo"$(INTDIR_SRC)\\" /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\arraycla.obj"	"..\objects\arraycla.sbr" : $(SOURCE)\
 $(DEP_CPP_ARRAY) "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\..\Source\baseclas\baseclas.cpp
DEP_CPP_BASEC=\
	"..\..\..\source\baseclas\baseclas.h"\
	"..\..\..\source\baseclas\baseclas.hpp"\
	"..\..\..\source\baseclas\boolean.h"\
	"..\..\..\source\baseclas\compatib.h"\
	"..\..\..\source\baseclas\defopt.h"\
	"..\..\..\source\baseclas\diagnost.h"\
	
INTDIR_SRC=.\..\objects

!IF  "$(CFG)" == "ioclass - Win32 Release"

CPP_SWITCHES=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D\
 "_MBCS" /Fp"$(INTDIR_SRC)\ioclass.pch" /YX /Fo"$(INTDIR_SRC)\\"\
 /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\baseclas.obj" : $(SOURCE) $(DEP_CPP_BASEC) "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ioclass - Win32 Debug"

CPP_SWITCHES=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /D "_MBCS" /FR"$(INTDIR_SRC)\\" /Fp"$(INTDIR_SRC)\ioclass.pch" /YX\
 /Fo"$(INTDIR_SRC)\\" /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\baseclas.obj"	"..\objects\baseclas.sbr" : $(SOURCE)\
 $(DEP_CPP_BASEC) "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\..\Source\vetclas\Diagclas.cpp
DEP_CPP_DIAGC=\
	"..\..\..\source\baseclas\baseclas.h"\
	"..\..\..\source\baseclas\baseclas.hpp"\
	"..\..\..\source\baseclas\boolean.h"\
	"..\..\..\source\baseclas\compatib.h"\
	"..\..\..\source\baseclas\defopt.h"\
	"..\..\..\source\baseclas\diagnost.h"\
	"..\..\..\source\vetclas\arraycla.h"\
	"..\..\..\source\vetclas\arraycla.hpp"\
	"..\..\..\source\vetclas\diagclas.h"\
	"..\..\..\source\vetclas\diagclas.hpp"\
	"..\..\..\source\vetclas\vetclas.h"\
	"..\..\..\source\vetclas\vetclas.hpp"\
	
INTDIR_SRC=.\..\objects

!IF  "$(CFG)" == "ioclass - Win32 Release"

CPP_SWITCHES=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D\
 "_MBCS" /Fp"$(INTDIR_SRC)\ioclass.pch" /YX /Fo"$(INTDIR_SRC)\\"\
 /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\diagclas.obj" : $(SOURCE) $(DEP_CPP_DIAGC) "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ioclass - Win32 Debug"

CPP_SWITCHES=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /D "_MBCS" /FR"$(INTDIR_SRC)\\" /Fp"$(INTDIR_SRC)\ioclass.pch" /YX\
 /Fo"$(INTDIR_SRC)\\" /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\diagclas.obj"	"..\objects\diagclas.sbr" : $(SOURCE)\
 $(DEP_CPP_DIAGC) "$(INTDIR_SRC)"
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

!IF  "$(CFG)" == "ioclass - Win32 Release"

CPP_SWITCHES=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D\
 "_MBCS" /Fp"$(INTDIR_SRC)\ioclass.pch" /YX /Fo"$(INTDIR_SRC)\\"\
 /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\Diagnost.obj" : $(SOURCE) $(DEP_CPP_DIAGN) "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ioclass - Win32 Debug"

CPP_SWITCHES=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /D "_MBCS" /FR"$(INTDIR_SRC)\\" /Fp"$(INTDIR_SRC)\ioclass.pch" /YX\
 /Fo"$(INTDIR_SRC)\\" /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\Diagnost.obj"	"..\objects\Diagnost.sbr" : $(SOURCE)\
 $(DEP_CPP_DIAGN) "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\..\Source\ioclass\labelcl.cpp
DEP_CPP_LABEL=\
	"..\..\..\source\baseclas\baseclas.h"\
	"..\..\..\source\baseclas\baseclas.hpp"\
	"..\..\..\source\baseclas\boolean.h"\
	"..\..\..\source\baseclas\compatib.h"\
	"..\..\..\source\baseclas\defopt.h"\
	"..\..\..\source\baseclas\diagnost.h"\
	"..\..\..\source\baseclas\polimorf.h"\
	"..\..\..\source\baseclas\textclas.h"\
	"..\..\..\source\ioclass\labelcl.h"\
	"..\..\..\source\resconf\resconf.h"\
	"..\..\..\source\vetclas\vetclas.h"\
	"..\..\..\source\vetclas\vetclas.hpp"\
	
INTDIR_SRC=.\..\objects

!IF  "$(CFG)" == "ioclass - Win32 Release"

CPP_SWITCHES=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D\
 "_MBCS" /Fp"$(INTDIR_SRC)\ioclass.pch" /YX /Fo"$(INTDIR_SRC)\\"\
 /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\labelcl.obj" : $(SOURCE) $(DEP_CPP_LABEL) "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ioclass - Win32 Debug"

CPP_SWITCHES=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /D "_MBCS" /FR"$(INTDIR_SRC)\\" /Fp"$(INTDIR_SRC)\ioclass.pch" /YX\
 /Fo"$(INTDIR_SRC)\\" /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\labelcl.obj"	"..\objects\labelcl.sbr" : $(SOURCE) $(DEP_CPP_LABEL)\
 "$(INTDIR_SRC)"
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

!IF  "$(CFG)" == "ioclass - Win32 Release"

CPP_SWITCHES=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D\
 "_MBCS" /Fp"$(INTDIR_SRC)\ioclass.pch" /YX /Fo"$(INTDIR_SRC)\\"\
 /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\Polimorf.obj" : $(SOURCE) $(DEP_CPP_POLIM) "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ioclass - Win32 Debug"

CPP_SWITCHES=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /D "_MBCS" /FR"$(INTDIR_SRC)\\" /Fp"$(INTDIR_SRC)\ioclass.pch" /YX\
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

!IF  "$(CFG)" == "ioclass - Win32 Release"

CPP_SWITCHES=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D\
 "_MBCS" /Fp"$(INTDIR_SRC)\ioclass.pch" /YX /Fo"$(INTDIR_SRC)\\"\
 /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\resconf.obj" : $(SOURCE) $(DEP_CPP_RESCO) "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ioclass - Win32 Debug"

CPP_SWITCHES=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /D "_MBCS" /FR"$(INTDIR_SRC)\\" /Fp"$(INTDIR_SRC)\ioclass.pch" /YX\
 /Fo"$(INTDIR_SRC)\\" /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\resconf.obj"	"..\objects\resconf.sbr" : $(SOURCE) $(DEP_CPP_RESCO)\
 "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\..\Source\ioclass\Soundfil.cpp
DEP_CPP_SOUND=\
	"..\..\..\source\baseclas\baseclas.h"\
	"..\..\..\source\baseclas\baseclas.hpp"\
	"..\..\..\source\baseclas\boolean.h"\
	"..\..\..\source\baseclas\compatib.h"\
	"..\..\..\source\baseclas\defopt.h"\
	"..\..\..\source\baseclas\diagnost.h"\
	"..\..\..\source\baseclas\polimorf.h"\
	"..\..\..\source\baseclas\textclas.h"\
	"..\..\..\source\ioclass\soundfil.h"\
	"..\..\..\source\resconf\resconf.h"\
	"..\..\..\source\vetclas\vetclas.h"\
	"..\..\..\source\vetclas\vetclas.hpp"\
	
INTDIR_SRC=.\..\objects

!IF  "$(CFG)" == "ioclass - Win32 Release"

CPP_SWITCHES=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D\
 "_MBCS" /Fp"$(INTDIR_SRC)\ioclass.pch" /YX /Fo"$(INTDIR_SRC)\\"\
 /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\soundfil.obj" : $(SOURCE) $(DEP_CPP_SOUND) "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ioclass - Win32 Debug"

CPP_SWITCHES=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /D "_MBCS" /FR"$(INTDIR_SRC)\\" /Fp"$(INTDIR_SRC)\ioclass.pch" /YX\
 /Fo"$(INTDIR_SRC)\\" /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\soundfil.obj"	"..\objects\soundfil.sbr" : $(SOURCE)\
 $(DEP_CPP_SOUND) "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\..\Source\ioclass\Soundlab.cpp
DEP_CPP_SOUNDL=\
	"..\..\..\source\baseclas\baseclas.h"\
	"..\..\..\source\baseclas\baseclas.hpp"\
	"..\..\..\source\baseclas\boolean.h"\
	"..\..\..\source\baseclas\compatib.h"\
	"..\..\..\source\baseclas\defopt.h"\
	"..\..\..\source\baseclas\diagnost.h"\
	"..\..\..\source\baseclas\polimorf.h"\
	"..\..\..\source\baseclas\textclas.h"\
	"..\..\..\source\ioclass\labelcl.h"\
	"..\..\..\source\ioclass\soundfil.h"\
	"..\..\..\source\ioclass\soundlab.h"\
	"..\..\..\source\resconf\resconf.h"\
	"..\..\..\source\vetclas\vetclas.h"\
	"..\..\..\source\vetclas\vetclas.hpp"\
	
INTDIR_SRC=.\..\objects

!IF  "$(CFG)" == "ioclass - Win32 Release"

CPP_SWITCHES=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D\
 "_MBCS" /Fp"$(INTDIR_SRC)\ioclass.pch" /YX /Fo"$(INTDIR_SRC)\\"\
 /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\soundlab.obj" : $(SOURCE) $(DEP_CPP_SOUNDL) "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ioclass - Win32 Debug"

CPP_SWITCHES=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /D "_MBCS" /FR"$(INTDIR_SRC)\\" /Fp"$(INTDIR_SRC)\ioclass.pch" /YX\
 /Fo"$(INTDIR_SRC)\\" /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\soundlab.obj"	"..\objects\soundlab.sbr" : $(SOURCE)\
 $(DEP_CPP_SOUNDL) "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\..\Source\ioclass\testione.cpp
DEP_CPP_TESTI=\
	"..\..\..\source\baseclas\baseclas.h"\
	"..\..\..\source\baseclas\baseclas.hpp"\
	"..\..\..\source\baseclas\boolean.h"\
	"..\..\..\source\baseclas\compatib.h"\
	"..\..\..\source\baseclas\defopt.h"\
	"..\..\..\source\baseclas\diagnost.h"\
	"..\..\..\source\baseclas\polimorf.h"\
	"..\..\..\source\baseclas\textclas.h"\
	"..\..\..\source\ioclass\labelcl.h"\
	"..\..\..\source\ioclass\soundfil.h"\
	"..\..\..\source\ioclass\soundlab.h"\
	"..\..\..\source\resconf\resconf.h"\
	"..\..\..\source\vetclas\vetclas.h"\
	"..\..\..\source\vetclas\vetclas.hpp"\
	
INTDIR_SRC=.\..\objects

!IF  "$(CFG)" == "ioclass - Win32 Release"

CPP_SWITCHES=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D\
 "_MBCS" /Fp"$(INTDIR_SRC)\ioclass.pch" /YX /Fo"$(INTDIR_SRC)\\"\
 /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\testione.obj" : $(SOURCE) $(DEP_CPP_TESTI) "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ioclass - Win32 Debug"

CPP_SWITCHES=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /D "_MBCS" /FR"$(INTDIR_SRC)\\" /Fp"$(INTDIR_SRC)\ioclass.pch" /YX\
 /Fo"$(INTDIR_SRC)\\" /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\testione.obj"	"..\objects\testione.sbr" : $(SOURCE)\
 $(DEP_CPP_TESTI) "$(INTDIR_SRC)"
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

!IF  "$(CFG)" == "ioclass - Win32 Release"

CPP_SWITCHES=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D\
 "_MBCS" /Fp"$(INTDIR_SRC)\ioclass.pch" /YX /Fo"$(INTDIR_SRC)\\"\
 /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\Textclas.obj" : $(SOURCE) $(DEP_CPP_TEXTC) "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ioclass - Win32 Debug"

CPP_SWITCHES=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /D "_MBCS" /FR"$(INTDIR_SRC)\\" /Fp"$(INTDIR_SRC)\ioclass.pch" /YX\
 /Fo"$(INTDIR_SRC)\\" /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\Textclas.obj"	"..\objects\Textclas.sbr" : $(SOURCE)\
 $(DEP_CPP_TEXTC) "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\..\Source\vetclas\Vetclas.cpp
DEP_CPP_VETCL=\
	"..\..\..\source\baseclas\baseclas.h"\
	"..\..\..\source\baseclas\baseclas.hpp"\
	"..\..\..\source\baseclas\boolean.h"\
	"..\..\..\source\baseclas\compatib.h"\
	"..\..\..\source\baseclas\defopt.h"\
	"..\..\..\source\baseclas\diagnost.h"\
	"..\..\..\source\vetclas\vetclas.h"\
	"..\..\..\source\vetclas\vetclas.hpp"\
	
INTDIR_SRC=.\..\objects

!IF  "$(CFG)" == "ioclass - Win32 Release"

CPP_SWITCHES=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D\
 "_MBCS" /Fp"$(INTDIR_SRC)\ioclass.pch" /YX /Fo"$(INTDIR_SRC)\\"\
 /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\vetclas.obj" : $(SOURCE) $(DEP_CPP_VETCL) "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ioclass - Win32 Debug"

CPP_SWITCHES=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /D "_MBCS" /FR"$(INTDIR_SRC)\\" /Fp"$(INTDIR_SRC)\ioclass.pch" /YX\
 /Fo"$(INTDIR_SRC)\\" /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\vetclas.obj"	"..\objects\vetclas.sbr" : $(SOURCE) $(DEP_CPP_VETCL)\
 "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 


# Microsoft Developer Studio Generated NMAKE File, Based on embedded.dsp
!IF "$(CFG)" == ""
CFG=embedded - Win32 Debug
!MESSAGE No configuration specified. Defaulting to embedded - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "embedded - Win32 Release" && "$(CFG)" !=\
 "embedded - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "embedded.mak" CFG="embedded - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "embedded - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "embedded - Win32 Debug" (based on "Win32 (x86) Console Application")
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

!IF  "$(CFG)" == "embedded - Win32 Release"

OUTDIR=.
INTDIR=.\..\objects
# Begin Custom Macros
OutDir=.
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\embedded.exe"

!ELSE 

ALL : "$(OUTDIR)\embedded.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\embedded.exe"
	-@erase "..\objects\arraycla.obj"
	-@erase "..\objects\diagclas.obj"
	-@erase "..\objects\Diagnost.obj"
	-@erase "..\objects\dspproc.obj"
	-@erase "..\objects\emb_b&w.obj"
	-@erase "..\objects\emb_train.obj"
	-@erase "..\objects\feature.obj"
	-@erase "..\objects\labelcl.obj"
	-@erase "..\objects\Polimorf.obj"
	-@erase "..\objects\resconf.obj"
	-@erase "..\objects\soundfil.obj"
	-@erase "..\objects\soundlab.obj"
	-@erase "..\objects\Textclas.obj"
	-@erase "..\objects\tspecbas.obj"
	-@erase "..\objects\vetclas.obj"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D\
 "_MBCS" /Fp"$(INTDIR)\embedded.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD\
 /c 
CPP_OBJS=..\objects/
CPP_SBRS=.
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\embedded.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)\embedded.pdb" /machine:I386 /out:"$(OUTDIR)\embedded.exe" 
LINK32_OBJS= \
	"..\objects\arraycla.obj" \
	"..\objects\diagclas.obj" \
	"..\objects\Diagnost.obj" \
	"..\objects\dspproc.obj" \
	"..\objects\emb_b&w.obj" \
	"..\objects\emb_train.obj" \
	"..\objects\feature.obj" \
	"..\objects\labelcl.obj" \
	"..\objects\Polimorf.obj" \
	"..\objects\resconf.obj" \
	"..\objects\soundfil.obj" \
	"..\objects\soundlab.obj" \
	"..\objects\Textclas.obj" \
	"..\objects\tspecbas.obj" \
	"..\objects\vetclas.obj"

"$(OUTDIR)\embedded.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "embedded - Win32 Debug"

OUTDIR=.
INTDIR=.\..\objects
# Begin Custom Macros
OutDir=.
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\embedded.exe" "$(OUTDIR)\embedded.bsc"

!ELSE 

ALL : "$(OUTDIR)\embedded.exe" "$(OUTDIR)\embedded.bsc"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\embedded.bsc"
	-@erase "$(OUTDIR)\embedded.exe"
	-@erase "$(OUTDIR)\embedded.pdb"
	-@erase "..\objects\arraycla.obj"
	-@erase "..\objects\arraycla.sbr"
	-@erase "..\objects\diagclas.obj"
	-@erase "..\objects\diagclas.sbr"
	-@erase "..\objects\Diagnost.obj"
	-@erase "..\objects\Diagnost.sbr"
	-@erase "..\objects\dspproc.obj"
	-@erase "..\objects\dspproc.sbr"
	-@erase "..\objects\emb_b&w.obj"
	-@erase "..\objects\emb_b&w.sbr"
	-@erase "..\objects\emb_train.obj"
	-@erase "..\objects\emb_train.sbr"
	-@erase "..\objects\feature.obj"
	-@erase "..\objects\feature.sbr"
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
	-@erase "..\objects\Textclas.obj"
	-@erase "..\objects\Textclas.sbr"
	-@erase "..\objects\tspecbas.obj"
	-@erase "..\objects\tspecbas.sbr"
	-@erase "..\objects\vetclas.obj"
	-@erase "..\objects\vetclas.sbr"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\embedded.pch" /YX /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=..\objects/
CPP_SBRS=..\objects/
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\embedded.bsc" 
BSC32_SBRS= \
	"..\objects\arraycla.sbr" \
	"..\objects\diagclas.sbr" \
	"..\objects\Diagnost.sbr" \
	"..\objects\dspproc.sbr" \
	"..\objects\emb_b&w.sbr" \
	"..\objects\emb_train.sbr" \
	"..\objects\feature.sbr" \
	"..\objects\labelcl.sbr" \
	"..\objects\Polimorf.sbr" \
	"..\objects\resconf.sbr" \
	"..\objects\soundfil.sbr" \
	"..\objects\soundlab.sbr" \
	"..\objects\Textclas.sbr" \
	"..\objects\tspecbas.sbr" \
	"..\objects\vetclas.sbr"

"$(OUTDIR)\embedded.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)\embedded.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)\embedded.exe" /pdbtype:sept 
LINK32_OBJS= \
	"..\objects\arraycla.obj" \
	"..\objects\diagclas.obj" \
	"..\objects\Diagnost.obj" \
	"..\objects\dspproc.obj" \
	"..\objects\emb_b&w.obj" \
	"..\objects\emb_train.obj" \
	"..\objects\feature.obj" \
	"..\objects\labelcl.obj" \
	"..\objects\Polimorf.obj" \
	"..\objects\resconf.obj" \
	"..\objects\soundfil.obj" \
	"..\objects\soundlab.obj" \
	"..\objects\Textclas.obj" \
	"..\objects\tspecbas.obj" \
	"..\objects\vetclas.obj"

"$(OUTDIR)\embedded.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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


!IF "$(CFG)" == "embedded - Win32 Release" || "$(CFG)" ==\
 "embedded - Win32 Debug"
SOURCE=..\..\..\Source\vetclas\arraycla.cpp
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

!IF  "$(CFG)" == "embedded - Win32 Release"

CPP_SWITCHES=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D\
 "_MBCS" /Fp"$(INTDIR_SRC)\embedded.pch" /YX /Fo"$(INTDIR_SRC)\\"\
 /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\arraycla.obj" : $(SOURCE) $(DEP_CPP_ARRAY) "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "embedded - Win32 Debug"

CPP_SWITCHES=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /D "_MBCS" /FR"$(INTDIR_SRC)\\" /Fp"$(INTDIR_SRC)\embedded.pch" /YX\
 /Fo"$(INTDIR_SRC)\\" /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\arraycla.obj"	"..\objects\arraycla.sbr" : $(SOURCE)\
 $(DEP_CPP_ARRAY) "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\..\Source\vetclas\diagclas.cpp
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

!IF  "$(CFG)" == "embedded - Win32 Release"

CPP_SWITCHES=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D\
 "_MBCS" /Fp"$(INTDIR_SRC)\embedded.pch" /YX /Fo"$(INTDIR_SRC)\\"\
 /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\diagclas.obj" : $(SOURCE) $(DEP_CPP_DIAGC) "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "embedded - Win32 Debug"

CPP_SWITCHES=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /D "_MBCS" /FR"$(INTDIR_SRC)\\" /Fp"$(INTDIR_SRC)\embedded.pch" /YX\
 /Fo"$(INTDIR_SRC)\\" /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\diagclas.obj"	"..\objects\diagclas.sbr" : $(SOURCE)\
 $(DEP_CPP_DIAGC) "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\..\Source\baseclas\diagnost.cpp
DEP_CPP_DIAGN=\
	"..\..\..\source\baseclas\boolean.h"\
	"..\..\..\source\baseclas\compatib.h"\
	"..\..\..\source\baseclas\defopt.h"\
	"..\..\..\source\baseclas\diagnost.h"\
	
INTDIR_SRC=.\..\objects

!IF  "$(CFG)" == "embedded - Win32 Release"

CPP_SWITCHES=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D\
 "_MBCS" /Fp"$(INTDIR_SRC)\embedded.pch" /YX /Fo"$(INTDIR_SRC)\\"\
 /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\Diagnost.obj" : $(SOURCE) $(DEP_CPP_DIAGN) "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "embedded - Win32 Debug"

CPP_SWITCHES=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /D "_MBCS" /FR"$(INTDIR_SRC)\\" /Fp"$(INTDIR_SRC)\embedded.pch" /YX\
 /Fo"$(INTDIR_SRC)\\" /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\Diagnost.obj"	"..\objects\Diagnost.sbr" : $(SOURCE)\
 $(DEP_CPP_DIAGN) "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\..\Source\features\dspproc.cpp
DEP_CPP_DSPPR=\
	"..\..\..\source\baseclas\baseclas.h"\
	"..\..\..\source\baseclas\baseclas.hpp"\
	"..\..\..\source\baseclas\boolean.h"\
	"..\..\..\source\baseclas\compatib.h"\
	"..\..\..\source\baseclas\defopt.h"\
	"..\..\..\source\baseclas\diagnost.h"\
	"..\..\..\source\baseclas\polimorf.h"\
	"..\..\..\source\baseclas\textclas.h"\
	"..\..\..\source\features\feature.h"\
	"..\..\..\source\resconf\resconf.h"\
	"..\..\..\source\vetclas\vetclas.h"\
	"..\..\..\source\vetclas\vetclas.hpp"\
	
INTDIR_SRC=.\..\objects

!IF  "$(CFG)" == "embedded - Win32 Release"

CPP_SWITCHES=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D\
 "_MBCS" /Fp"$(INTDIR_SRC)\embedded.pch" /YX /Fo"$(INTDIR_SRC)\\"\
 /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\dspproc.obj" : $(SOURCE) $(DEP_CPP_DSPPR) "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "embedded - Win32 Debug"

CPP_SWITCHES=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /D "_MBCS" /FR"$(INTDIR_SRC)\\" /Fp"$(INTDIR_SRC)\embedded.pch" /YX\
 /Fo"$(INTDIR_SRC)\\" /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\dspproc.obj"	"..\objects\dspproc.sbr" : $(SOURCE) $(DEP_CPP_DSPPR)\
 "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE="..\..\..\Source\embedded\emb_b&w.cpp"
DEP_CPP_EMB_B=\
	"..\..\..\source\baseclas\baseclas.h"\
	"..\..\..\source\baseclas\baseclas.hpp"\
	"..\..\..\source\baseclas\boolean.h"\
	"..\..\..\source\baseclas\compatib.h"\
	"..\..\..\source\baseclas\defopt.h"\
	"..\..\..\source\baseclas\diagnost.h"\
	"..\..\..\source\baseclas\polimorf.h"\
	"..\..\..\source\baseclas\textclas.h"\
	"..\..\..\source\embedded\emb_b&w.h"\
	"..\..\..\source\features\feature.h"\
	"..\..\..\source\ioclass\labelcl.h"\
	"..\..\..\source\ioclass\soundfil.h"\
	"..\..\..\source\ioclass\soundlab.h"\
	"..\..\..\source\resconf\resconf.h"\
	"..\..\..\source\tspecmod\tspecbas.h"\
	"..\..\..\source\tspecmod\tspecbas.hpp"\
	"..\..\..\source\vetclas\arraycla.h"\
	"..\..\..\source\vetclas\arraycla.hpp"\
	"..\..\..\source\vetclas\diagclas.h"\
	"..\..\..\source\vetclas\diagclas.hpp"\
	"..\..\..\source\vetclas\vetclas.h"\
	"..\..\..\source\vetclas\vetclas.hpp"\
	
INTDIR_SRC=.\..\objects

!IF  "$(CFG)" == "embedded - Win32 Release"

CPP_SWITCHES=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D\
 "_MBCS" /Fp"$(INTDIR_SRC)\embedded.pch" /YX /Fo"$(INTDIR_SRC)\\"\
 /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\emb_b&w.obj" : $(SOURCE) $(DEP_CPP_EMB_B) "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "embedded - Win32 Debug"

CPP_SWITCHES=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /D "_MBCS" /FR"$(INTDIR_SRC)\\" /Fp"$(INTDIR_SRC)\embedded.pch" /YX\
 /Fo"$(INTDIR_SRC)\\" /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\emb_b&w.obj"	"..\objects\emb_b&w.sbr" : $(SOURCE) $(DEP_CPP_EMB_B)\
 "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\..\Source\embedded\emb_train.cpp
DEP_CPP_EMB_T=\
	"..\..\..\source\baseclas\baseclas.h"\
	"..\..\..\source\baseclas\baseclas.hpp"\
	"..\..\..\source\baseclas\boolean.h"\
	"..\..\..\source\baseclas\compatib.h"\
	"..\..\..\source\baseclas\defopt.h"\
	"..\..\..\source\baseclas\diagnost.h"\
	"..\..\..\source\baseclas\polimorf.h"\
	"..\..\..\source\baseclas\textclas.h"\
	"..\..\..\source\embedded\emb_b&w.h"\
	"..\..\..\source\features\feature.h"\
	"..\..\..\source\ioclass\labelcl.h"\
	"..\..\..\source\ioclass\soundfil.h"\
	"..\..\..\source\ioclass\soundlab.h"\
	"..\..\..\source\resconf\resconf.h"\
	"..\..\..\source\tspecmod\tspecbas.h"\
	"..\..\..\source\tspecmod\tspecbas.hpp"\
	"..\..\..\source\vetclas\arraycla.h"\
	"..\..\..\source\vetclas\arraycla.hpp"\
	"..\..\..\source\vetclas\diagclas.h"\
	"..\..\..\source\vetclas\diagclas.hpp"\
	"..\..\..\source\vetclas\vetclas.h"\
	"..\..\..\source\vetclas\vetclas.hpp"\
	
INTDIR_SRC=.\..\objects

!IF  "$(CFG)" == "embedded - Win32 Release"

CPP_SWITCHES=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D\
 "_MBCS" /Fp"$(INTDIR_SRC)\embedded.pch" /YX /Fo"$(INTDIR_SRC)\\"\
 /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\emb_train.obj" : $(SOURCE) $(DEP_CPP_EMB_T) "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "embedded - Win32 Debug"

CPP_SWITCHES=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /D "_MBCS" /FR"$(INTDIR_SRC)\\" /Fp"$(INTDIR_SRC)\embedded.pch" /YX\
 /Fo"$(INTDIR_SRC)\\" /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\emb_train.obj"	"..\objects\emb_train.sbr" : $(SOURCE)\
 $(DEP_CPP_EMB_T) "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\..\Source\features\feature.cpp
DEP_CPP_FEATU=\
	"..\..\..\source\baseclas\baseclas.h"\
	"..\..\..\source\baseclas\baseclas.hpp"\
	"..\..\..\source\baseclas\boolean.h"\
	"..\..\..\source\baseclas\compatib.h"\
	"..\..\..\source\baseclas\defopt.h"\
	"..\..\..\source\baseclas\diagnost.h"\
	"..\..\..\source\baseclas\polimorf.h"\
	"..\..\..\source\baseclas\textclas.h"\
	"..\..\..\source\features\feature.h"\
	"..\..\..\source\resconf\resconf.h"\
	"..\..\..\source\vetclas\vetclas.h"\
	"..\..\..\source\vetclas\vetclas.hpp"\
	
INTDIR_SRC=.\..\objects

!IF  "$(CFG)" == "embedded - Win32 Release"

CPP_SWITCHES=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D\
 "_MBCS" /Fp"$(INTDIR_SRC)\embedded.pch" /YX /Fo"$(INTDIR_SRC)\\"\
 /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\feature.obj" : $(SOURCE) $(DEP_CPP_FEATU) "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "embedded - Win32 Debug"

CPP_SWITCHES=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /D "_MBCS" /FR"$(INTDIR_SRC)\\" /Fp"$(INTDIR_SRC)\embedded.pch" /YX\
 /Fo"$(INTDIR_SRC)\\" /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\feature.obj"	"..\objects\feature.sbr" : $(SOURCE) $(DEP_CPP_FEATU)\
 "$(INTDIR_SRC)"
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

!IF  "$(CFG)" == "embedded - Win32 Release"

CPP_SWITCHES=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D\
 "_MBCS" /Fp"$(INTDIR_SRC)\embedded.pch" /YX /Fo"$(INTDIR_SRC)\\"\
 /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\labelcl.obj" : $(SOURCE) $(DEP_CPP_LABEL) "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "embedded - Win32 Debug"

CPP_SWITCHES=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /D "_MBCS" /FR"$(INTDIR_SRC)\\" /Fp"$(INTDIR_SRC)\embedded.pch" /YX\
 /Fo"$(INTDIR_SRC)\\" /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\labelcl.obj"	"..\objects\labelcl.sbr" : $(SOURCE) $(DEP_CPP_LABEL)\
 "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\..\Source\baseclas\polimorf.cpp
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

!IF  "$(CFG)" == "embedded - Win32 Release"

CPP_SWITCHES=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D\
 "_MBCS" /Fp"$(INTDIR_SRC)\embedded.pch" /YX /Fo"$(INTDIR_SRC)\\"\
 /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\Polimorf.obj" : $(SOURCE) $(DEP_CPP_POLIM) "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "embedded - Win32 Debug"

CPP_SWITCHES=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /D "_MBCS" /FR"$(INTDIR_SRC)\\" /Fp"$(INTDIR_SRC)\embedded.pch" /YX\
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

!IF  "$(CFG)" == "embedded - Win32 Release"

CPP_SWITCHES=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D\
 "_MBCS" /Fp"$(INTDIR_SRC)\embedded.pch" /YX /Fo"$(INTDIR_SRC)\\"\
 /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\resconf.obj" : $(SOURCE) $(DEP_CPP_RESCO) "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "embedded - Win32 Debug"

CPP_SWITCHES=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /D "_MBCS" /FR"$(INTDIR_SRC)\\" /Fp"$(INTDIR_SRC)\embedded.pch" /YX\
 /Fo"$(INTDIR_SRC)\\" /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\resconf.obj"	"..\objects\resconf.sbr" : $(SOURCE) $(DEP_CPP_RESCO)\
 "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\..\Source\ioclass\soundfil.cpp
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

!IF  "$(CFG)" == "embedded - Win32 Release"

CPP_SWITCHES=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D\
 "_MBCS" /Fp"$(INTDIR_SRC)\embedded.pch" /YX /Fo"$(INTDIR_SRC)\\"\
 /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\soundfil.obj" : $(SOURCE) $(DEP_CPP_SOUND) "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "embedded - Win32 Debug"

CPP_SWITCHES=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /D "_MBCS" /FR"$(INTDIR_SRC)\\" /Fp"$(INTDIR_SRC)\embedded.pch" /YX\
 /Fo"$(INTDIR_SRC)\\" /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\soundfil.obj"	"..\objects\soundfil.sbr" : $(SOURCE)\
 $(DEP_CPP_SOUND) "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\..\Source\ioclass\soundlab.cpp
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

!IF  "$(CFG)" == "embedded - Win32 Release"

CPP_SWITCHES=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D\
 "_MBCS" /Fp"$(INTDIR_SRC)\embedded.pch" /YX /Fo"$(INTDIR_SRC)\\"\
 /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\soundlab.obj" : $(SOURCE) $(DEP_CPP_SOUNDL) "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "embedded - Win32 Debug"

CPP_SWITCHES=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /D "_MBCS" /FR"$(INTDIR_SRC)\\" /Fp"$(INTDIR_SRC)\embedded.pch" /YX\
 /Fo"$(INTDIR_SRC)\\" /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\soundlab.obj"	"..\objects\soundlab.sbr" : $(SOURCE)\
 $(DEP_CPP_SOUNDL) "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\..\Source\baseclas\textclas.cpp
DEP_CPP_TEXTC=\
	"..\..\..\source\baseclas\baseclas.h"\
	"..\..\..\source\baseclas\baseclas.hpp"\
	"..\..\..\source\baseclas\boolean.h"\
	"..\..\..\source\baseclas\compatib.h"\
	"..\..\..\source\baseclas\defopt.h"\
	"..\..\..\source\baseclas\diagnost.h"\
	"..\..\..\source\baseclas\textclas.h"\
	
INTDIR_SRC=.\..\objects

!IF  "$(CFG)" == "embedded - Win32 Release"

CPP_SWITCHES=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D\
 "_MBCS" /Fp"$(INTDIR_SRC)\embedded.pch" /YX /Fo"$(INTDIR_SRC)\\"\
 /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\Textclas.obj" : $(SOURCE) $(DEP_CPP_TEXTC) "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "embedded - Win32 Debug"

CPP_SWITCHES=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /D "_MBCS" /FR"$(INTDIR_SRC)\\" /Fp"$(INTDIR_SRC)\embedded.pch" /YX\
 /Fo"$(INTDIR_SRC)\\" /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\Textclas.obj"	"..\objects\Textclas.sbr" : $(SOURCE)\
 $(DEP_CPP_TEXTC) "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\..\Source\tspecmod\tspecbas.cpp
DEP_CPP_TSPEC=\
	"..\..\..\source\baseclas\baseclas.h"\
	"..\..\..\source\baseclas\baseclas.hpp"\
	"..\..\..\source\baseclas\boolean.h"\
	"..\..\..\source\baseclas\compatib.h"\
	"..\..\..\source\baseclas\defopt.h"\
	"..\..\..\source\baseclas\diagnost.h"\
	"..\..\..\source\baseclas\textclas.h"\
	"..\..\..\source\tspecmod\tspecbas.h"\
	"..\..\..\source\tspecmod\tspecbas.hpp"\
	"..\..\..\source\vetclas\arraycla.h"\
	"..\..\..\source\vetclas\arraycla.hpp"\
	"..\..\..\source\vetclas\diagclas.h"\
	"..\..\..\source\vetclas\diagclas.hpp"\
	"..\..\..\source\vetclas\vetclas.h"\
	"..\..\..\source\vetclas\vetclas.hpp"\
	
INTDIR_SRC=.\..\objects

!IF  "$(CFG)" == "embedded - Win32 Release"

CPP_SWITCHES=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D\
 "_MBCS" /Fp"$(INTDIR_SRC)\embedded.pch" /YX /Fo"$(INTDIR_SRC)\\"\
 /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\tspecbas.obj" : $(SOURCE) $(DEP_CPP_TSPEC) "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "embedded - Win32 Debug"

CPP_SWITCHES=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /D "_MBCS" /FR"$(INTDIR_SRC)\\" /Fp"$(INTDIR_SRC)\embedded.pch" /YX\
 /Fo"$(INTDIR_SRC)\\" /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\tspecbas.obj"	"..\objects\tspecbas.sbr" : $(SOURCE)\
 $(DEP_CPP_TSPEC) "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\..\Source\vetclas\vetclas.cpp
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

!IF  "$(CFG)" == "embedded - Win32 Release"

CPP_SWITCHES=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D\
 "_MBCS" /Fp"$(INTDIR_SRC)\embedded.pch" /YX /Fo"$(INTDIR_SRC)\\"\
 /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\vetclas.obj" : $(SOURCE) $(DEP_CPP_VETCL) "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "embedded - Win32 Debug"

CPP_SWITCHES=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /D "_MBCS" /FR"$(INTDIR_SRC)\\" /Fp"$(INTDIR_SRC)\embedded.pch" /YX\
 /Fo"$(INTDIR_SRC)\\" /Fd"$(INTDIR_SRC)\\" /FD /c 

"..\objects\vetclas.obj"	"..\objects\vetclas.sbr" : $(SOURCE) $(DEP_CPP_VETCL)\
 "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 


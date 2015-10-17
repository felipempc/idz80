# IDZ80 Makefile

#MainDrive
DRV = K:

# wxWidgets
WXWVERSION = 294
WXW ?= $(DRV)\wxw\wx$(WXWVERSION)

# Compiler
CC = $(DRV)\MinGW\bin\g++

# Debug
DEBUG ?= 0

# Resource
RS = windres
RS_FLAGS = --include-dirs=$(WXW)\include

# Date and time
DATE = $(shell date /t)
TIME = $(shell time /t)


ifeq ($(DEBUG), 0)
	# Release
	CC_FLAGS = -O2 -Wredundant-decls -march=pentium-mmx -c -Wall -pipe -mthreads
	DEFS = -D__GNUWIN32__ -D__WXMSW__ -DWXUSINGDLL -DwxUSE_UNICODE
	INCLUDE0_PATH = $(WXW)\lib\mswu
	LIBFILE1 = -lwxbase$(WXWVERSION)u_gcc_custom
	LIBFILE2 = -lwxmsw$(WXWVERSION)u_aui_gcc_custom
	LIBFILE3 = -lwxmsw$(WXWVERSION)u_core_gcc_custom
else
	# DEBUG
	CC_FLAGS = -Wredundant-decls -march=pentium-mmx -c -Wall -pipe -mthreads -g
	DEFS = -D__GNUWIN32__ -D__WXMSW__ -DWXUSINGDLL -DwxUSE_UNICODE -D__WXDEBUG__ -DIDZ80DEBUG -DIDZ80_DECODER -DIDZ80_DASMED
	INCLUDE0_PATH = $(WXW)\lib\mswud
	LIBFILE1 = -lwxbase$(WXWVERSION)ud_gcc_custom
	LIBFILE2 = -lwxmsw$(WXWVERSION)ud_aui_gcc_custom
	LIBFILE3 = -lwxmsw$(WXWVERSION)ud_core_gcc_custom
endif


INCLUDE1_PATH = $(WXW)\include
INCLUDE2_PATH = $(WXW)\contrib\include
INCLUDE = -I$(INCLUDE0_PATH) -I$(INCLUDE1_PATH) -I$(INCLUDE2_PATH)

# Linker
LD_FLAGS = -Wl,--subsystem,windows -mwindows


# libraries
LIBS = $(LIBFILE1) $(LIBFILE2) $(LIBFILE3)

# libraries's path
LIBPATH = -L$(WXW)\lib

# Project's path
ifeq ($(DEBUG), 0)
	OBJDIR = ..\obj\Release
	BINDIR = ..\bin\Release
else
	OBJDIR = ..\obj\Debug
	BINDIR = ..\bin\Debug
endif

SRCDIR = ..\src
RESDIR = ..

EXECFILE = $(BINDIR)\idz80_make.exe


	
	
all: make_dir $(OBJS)
	@echo Linking...
	$(CC) -o $(EXECFILE) $(LD_FLAGS) $(LIBPATH) $(LIBS) $(OBJS) 
	@echo --------------------------------------------------------
	@echo $@ done !
	@echo $(TIME) $(DATE)
	@echo File is $(EXECFILE)


debug: make_dir $(OBJS)
	DEBUG = 1
	@echo Linking (DEBUG mode)...
	$(CC) -o $(EXECFILE) $(LD_FLAGS) $(LIBPATH) $(LIBS) $(OBJS) 
	@echo --------------------------------------------------------
	@echo $@ done !
	@echo $(TIME) $(DATE)
	@echo File is $(EXECFILE)


make_dir:
	@echo Checking dirs...
	@if not exist $(BINDIR) md $(BINDIR)
	@if not exist $(OBJDIR) md $(OBJDIR)


$(OBJDIR)\EditLabelDlg.o: $(SRCDIR)\EditLabelDlg.cpp $(SRCDIR)\EditLabelDlg.h $(SRCDIR)\IDZ80Base.h
	@echo $(SRCDIR)\EditLabelDlg.cpp
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ $(SRCDIR)\EditLabelDlg.cpp $(INCLUDE)

$(OBJDIR)\FileTypeDialog.o: $(SRCDIR)\FileTypeDialog.cpp $(SRCDIR)\FileTypeDialog.h $(SRCDIR)\IDZ80Base.h
	@echo $(SRCDIR)\FileTypeDialog.cpp
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ $(SRCDIR)\FileTypeDialog.cpp $(INCLUDE)

$(OBJDIR)\z80register.o: $(SRCDIR)\z80register.cpp $(SRCDIR)\z80register.h $(SRCDIR)\IDZ80Base.h
	@echo $(SRCDIR)\z80register.cpp
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ $(SRCDIR)\z80register.cpp $(INCLUDE)

$(OBJDIR)\z80registerlist.o: $(SRCDIR)\z80registerlist.cpp $(SRCDIR)\z80registerlist.h $(SRCDIR)\IDZ80Base.h $(OBJDIR)\z80register.o
	@echo $(SRCDIR)\z80registerlist.cpp
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ $(SRCDIR)\z80registerlist.cpp $(INCLUDE)

$(OBJDIR)\MnemonicItem.o: $(SRCDIR)\MnemonicItem.cpp $(SRCDIR)\MnemonicItem.h $(SRCDIR)\IDZ80Base.h
	@echo $(SRCDIR)\MnemonicItem.cpp
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ $(SRCDIR)\MnemonicItem.cpp $(INCLUDE)

	
$(OBJDIR)\segmentmgr.o: $(SRCDIR)\segmentmgr.cpp $(SRCDIR)\segmentmgr.h $(SRCDIR)\IDZ80Base.h
	@echo $(SRCDIR)\segmentmgr.cpp
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ $(SRCDIR)\segmentmgr.cpp $(INCLUDE)

	
$(OBJDIR)\decoder.o: $(SRCDIR)\decoder.cpp $(SRCDIR)\decoder.h $(SRCDIR)\IDZ80debugbase.cpp $(SRCDIR)\IDZ80debugbase.h $(SRCDIR)\IDZ80Base.h
	@echo $(SRCDIR)\decoder.cpp
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ $(SRCDIR)\decoder.cpp $(INCLUDE)

	
$(OBJDIR)\systemlabels.o: $(SRCDIR)\systemlabels.cpp $(SRCDIR)\systemlabels.h $(SRCDIR)\IDZ80Base.h
	@echo $(SRCDIR)\systemlabels.cpp
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ $(SRCDIR)\systemlabels.cpp $(INCLUDE)

$(OBJDIR)\ProcessData.o: $(SRCDIR)\ProcessData.cpp $(SRCDIR)\ProcessData.h $(SRCDIR)\IDZ80Base.h
	@echo $(SRCDIR)\ProcessData.cpp
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ $(SRCDIR)\ProcessData.cpp $(INCLUDE)

$(OBJDIR)\codeview_render.o: $(SRCDIR)\codeview_render.cpp $(SRCDIR)\codeview.h $(SRCDIR)\IDZ80Base.h
	@echo $(SRCDIR)\codeview_render.cpp
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ $(SRCDIR)\codeview_render.cpp $(INCLUDE)

$(OBJDIR)\codeview_evmouse.o: $(SRCDIR)\codeview_evmouse.cpp $(SRCDIR)\codeview.h $(SRCDIR)\IDZ80Base.h
	@echo $(SRCDIR)\codeview_evmouse.cpp
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ $(SRCDIR)\codeview_evmouse.cpp $(INCLUDE)
	
$(OBJDIR)\codeview.o: $(SRCDIR)\codeview.cpp $(OBJDIR)\codeview_evmouse.o $(OBJDIR)\codeview_render.o $(SRCDIR)\codeview.h $(SRCDIR)\IDZ80Base.h
	@echo $(SRCDIR)\codeview.cpp
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ $(OBJDIR)\codeview_evmouse.o $(OBJDIR)\codeview_render.o $(SRCDIR)\codeview.cpp $(INCLUDE)

$(OBJDIR)\codeviewline.o: $(SRCDIR)\codeviewline.cpp $(SRCDIR)\codeviewline.h $(SRCDIR)\IDZ80Base.h
	@echo $(SRCDIR)\codeviewline.cpp
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ $(SRCDIR)\codeviewline.cpp $(INCLUDE)

$(OBJDIR)\d_asm_element.o: $(SRCDIR)\d_asm_element.cpp $(SRCDIR)\d_asm_element.h $(SRCDIR)\IDZ80Base.h
	@echo $(SRCDIR)\d_asm_element.cpp
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ $(SRCDIR)\d_asm_element.cpp $(INCLUDE)

$(OBJDIR)\dasmdata.o: $(SRCDIR)\dasmdata.cpp $(SRCDIR)\dasmdata.h $(SRCDIR)\IDZ80Base.h
	@echo $(SRCDIR)\dasmdata.cpp
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ $(SRCDIR)\dasmdata.cpp $(INCLUDE)

$(OBJDIR)\labelslist.o: $(SRCDIR)\labelslist.cpp $(SRCDIR)\labelslist.h $(SRCDIR)\IDZ80Base.h
	@echo $(SRCDIR)\labelslist.cpp
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ $(SRCDIR)\labelslist.cpp $(INCLUDE)

$(OBJDIR)\mndb.o: $(SRCDIR)\mndb.cpp $(SRCDIR)\mndb.h $(SRCDIR)\IDZ80debugbase.cpp $(SRCDIR)\IDZ80debugbase.h $(SRCDIR)\IDZ80Base.h
	@echo $(SRCDIR)\mndb.cpp
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ $(SRCDIR)\mndb.cpp $(INCLUDE)

$(OBJDIR)\mndb_tools.o: $(SRCDIR)\mndb_tools.cpp $(SRCDIR)\mndb_tools.h $(SRCDIR)\IDZ80Base.h
	@echo $(SRCDIR)\mndb_tools.cpp
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ $(SRCDIR)\mndb_tools.cpp $(INCLUDE)

$(OBJDIR)\projectmanager.o: $(SRCDIR)\projectmanager.cpp $(SRCDIR)\projectmanager.h $(SRCDIR)\IDZ80Base.h
	@echo $(SRCDIR)\projectmanager.cpp
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ $(SRCDIR)\projectmanager.cpp $(INCLUDE)

$(OBJDIR)\rawdata.o: $(SRCDIR)\rawdata.cpp $(SRCDIR)\rawdata.h $(SRCDIR)\IDZ80Base.h
	@echo $(SRCDIR)\rawdata.cpp
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ $(SRCDIR)\rawdata.cpp $(INCLUDE)

$(OBJDIR)\codegenerator.o: $(SRCDIR)\codegenerator.cpp $(SRCDIR)\codegenerator.h $(SRCDIR)\IDZ80Base.h
	@echo $(SRCDIR)\codegenerator.cpp
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ $(SRCDIR)\codegenerator.cpp $(INCLUDE)

$(OBJDIR)\ShowFileInfo.o: $(SRCDIR)\ShowFileInfo.cpp $(SRCDIR)\ShowFileInfo.h $(SRCDIR)\IDZ80Base.h
	@echo $(SRCDIR)\ShowFileInfo.cpp
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ $(SRCDIR)\ShowFileInfo.cpp $(INCLUDE)

$(OBJDIR)\IDZ80.o: $(SRCDIR)\IDZ80.cpp $(SRCDIR)\IDZ80.h
	@echo $(SRCDIR)\IDZ80.cpp
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ $(SRCDIR)\IDZ80.cpp $(INCLUDE)

$(OBJDIR)\IDZ80Base.o: $(SRCDIR)\IDZ80Base.cpp $(SRCDIR)\IDZ80Base.h
	@echo $(SRCDIR)\IDZ80Base.cpp
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ $(SRCDIR)\IDZ80Base.cpp $(INCLUDE)

$(OBJDIR)\IDZ80debugbase.o: $(SRCDIR)\IDZ80debugbase.cpp $(SRCDIR)\IDZ80debugbase.h $(SRCDIR)\IDZ80Base.h
	@echo $(SRCDIR)\IDZ80debugbase.cpp
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ $(SRCDIR)\IDZ80debugbase.cpp $(INCLUDE)
	
$(OBJDIR)\IDZ80App.o: $(SRCDIR)\IDZ80App.cpp $(SRCDIR)\IDZ80App.h
	@echo $(SRCDIR)\IDZ80App.cpp
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ $(SRCDIR)\IDZ80App.cpp $(INCLUDE)

$(OBJDIR)\resource.o: $(RESDIR)\resource.rc
	@echo $(RESDIR)\resource.rc
	$(RS) -o $@ $(RESDIR)\resource.rc -I $(INCLUDE1_PATH)


clean:
	-if exist $(OBJDIR)\*.o del $(OBJDIR)\*.o
	-if exist $(EXECFILE) del $(EXECFILE)



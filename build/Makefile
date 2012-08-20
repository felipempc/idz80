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
	INCLUDE0_PATH = $(WXW)\lib\gcc_dll\mswu
	LIBFILE1 = -lwxbase$(WXWVERSION)u_gcc_custom
	LIBFILE2 = -lwxmsw$(WXWVERSION)u_aui_gcc_custom
	LIBFILE3 = -lwxmsw$(WXWVERSION)u_core_gcc_custom
else
	# DEBUG
	CC_FLAGS = -Wredundant-decls -march=pentium-mmx -c -Wall -pipe -mthreads -g
	DEFS = -D__GNUWIN32__ -D__WXMSW__ -DWXUSINGDLL -DwxUSE_UNICODE -D__WXDEBUG__ -DIDZ80DEBUG
	INCLUDE0_PATH = $(WXW)\lib\gcc_dll\mswud
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
LIBPATH = -L$(WXW)\lib\gcc_dll

# Project's path
ifeq ($(DEBUG), 0)
	OBJDIR = X:\obj\Release
	BINDIR = X:\bin\Release
else
	OBJDIR = X:\obj\Debug
	BINDIR = X:\bin\Debug
endif

SRCDIR = X:\src
RESDIR = X:

EXECFILE = $(BINDIR)\idz80_make.exe


OBJS = $(OBJDIR)\EditLabelDlg.o \
	$(OBJDIR)\FileTypeDialog.o \
	$(OBJDIR)\MnemonicItem.o \
	$(OBJDIR)\ProcessData.o \
	$(OBJDIR)\codeview.o \
	$(OBJDIR)\codeview_evmouse.o \
	$(OBJDIR)\codeview_render.o \
	$(OBJDIR)\codeviewline.o \
	$(OBJDIR)\d_asm_element.o \
	$(OBJDIR)\dasmdata.o \
	$(OBJDIR)\labelslist.o \
	$(OBJDIR)\mndb.o \
	$(OBJDIR)\mndb_tools.o \
	$(OBJDIR)\projectmanager.o \
	$(OBJDIR)\rawdata.o \
	$(OBJDIR)\ShowFileInfo.o \
	$(OBJDIR)\codegenerator.o \
	$(OBJDIR)\IDZ80.o \
	$(OBJDIR)\IDZ80App.o \
	$(OBJDIR)\resource.o

all: make_dir $(OBJS)
	@echo Linking...
	$(CC) -o $(EXECFILE) $(LD_FLAGS) $(LIBPATH) $(LIBS) $(OBJS) 
	@echo --------------------------------------------------------
	@echo $@ done !
	@echo $(TIME) $(DATE)
	@echo File is $(EXECFILE)

make_dir:
	@echo Checking dirs...
	@if not exist $(BINDIR) md $(BINDIR)
	@if not exist $(OBJDIR) md $(OBJDIR)

$(OBJDIR)\EditLabelDlg.o: $(SRCDIR)\EditLabelDlg.cpp $(SRCDIR)\EditLabelDlg.h
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ $(SRCDIR)\EditLabelDlg.cpp $(INCLUDE)

$(OBJDIR)\FileTypeDialog.o: $(SRCDIR)\FileTypeDialog.cpp $(SRCDIR)\FileTypeDialog.h
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ $(SRCDIR)\FileTypeDialog.cpp $(INCLUDE)

$(OBJDIR)\MnemonicItem.o: $(SRCDIR)\MnemonicItem.cpp $(SRCDIR)\MnemonicItem.h
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ $(SRCDIR)\MnemonicItem.cpp $(INCLUDE)

$(OBJDIR)\ProcessData.o: $(SRCDIR)\ProcessData.cpp $(SRCDIR)\ProcessData.h
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ $(SRCDIR)\ProcessData.cpp $(INCLUDE)

$(OBJDIR)\codeview_render.o: $(SRCDIR)\codeview_render.cpp $(SRCDIR)\codeview.h
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ $(SRCDIR)\codeview_render.cpp $(INCLUDE)

$(OBJDIR)\codeview_evmouse.o: $(SRCDIR)\codeview_evmouse.cpp $(SRCDIR)\codeview.h
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ $(SRCDIR)\codeview_evmouse.cpp $(INCLUDE)
	
$(OBJDIR)\codeview.o: $(SRCDIR)\codeview.cpp $(OBJDIR)\codeview_evmouse.o $(OBJDIR)\codeview_render.o $(SRCDIR)\codeview.h
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ $(OBJDIR)\codeview_evmouse.o $(OBJDIR)\codeview_render.o $(SRCDIR)\codeview.cpp $(INCLUDE)

$(OBJDIR)\codeviewline.o: $(SRCDIR)\codeviewline.cpp $(SRCDIR)\codeviewline.h
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ $(SRCDIR)\codeviewline.cpp $(INCLUDE)

$(OBJDIR)\d_asm_element.o: $(SRCDIR)\d_asm_element.cpp $(SRCDIR)\d_asm_element.h
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ $(SRCDIR)\d_asm_element.cpp $(INCLUDE)

$(OBJDIR)\dasmdata.o: $(SRCDIR)\dasmdata.cpp $(SRCDIR)\dasmdata.h
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ $(SRCDIR)\dasmdata.cpp $(INCLUDE)

$(OBJDIR)\labelslist.o: $(SRCDIR)\labelslist.cpp $(SRCDIR)\labelslist.h
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ $(SRCDIR)\labelslist.cpp $(INCLUDE)

$(OBJDIR)\mndb.o: $(SRCDIR)\mndb.cpp $(SRCDIR)\mndb.h
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ $(SRCDIR)\mndb.cpp $(INCLUDE)

$(OBJDIR)\mndb_tools.o: $(SRCDIR)\mndb_tools.cpp $(SRCDIR)\mndb_tools.h
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ $(SRCDIR)\mndb_tools.cpp $(INCLUDE)

$(OBJDIR)\projectmanager.o: $(SRCDIR)\projectmanager.cpp $(SRCDIR)\projectmanager.h
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ $(SRCDIR)\projectmanager.cpp $(INCLUDE)

$(OBJDIR)\rawdata.o: $(SRCDIR)\rawdata.cpp $(SRCDIR)\rawdata.h
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ $(SRCDIR)\rawdata.cpp $(INCLUDE)

$(OBJDIR)\codegenerator.o: $(SRCDIR)\codegenerator.cpp $(SRCDIR)\codegenerator.h
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ $(SRCDIR)\codegenerator.cpp $(INCLUDE)

$(OBJDIR)\ShowFileInfo.o: $(SRCDIR)\ShowFileInfo.cpp $(SRCDIR)\ShowFileInfo.h
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ $(SRCDIR)\ShowFileInfo.cpp $(INCLUDE)

$(OBJDIR)\IDZ80.o: $(SRCDIR)\IDZ80.cpp $(SRCDIR)\IDZ80.h
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ $(SRCDIR)\IDZ80.cpp $(INCLUDE)

$(OBJDIR)\IDZ80App.o: $(SRCDIR)\IDZ80App.cpp $(SRCDIR)\IDZ80App.h
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ $(SRCDIR)\IDZ80App.cpp $(INCLUDE)

$(OBJDIR)\resource.o: $(RESDIR)\resource.rc
	$(RS) -o $@ $(RESDIR)\resource.rc -I $(INCLUDE1_PATH)


clean:
	-if exist $(OBJDIR)\*.o del $(OBJDIR)\*.o
	-if exist $(EXECFILE) del $(EXECFILE)



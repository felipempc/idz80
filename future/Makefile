# IDZ80 Makefile


# wxWidgets
WXW ?= C:\wxWidgets-2.8.11

# Compiler
CC = g++

# Resource
RS = windres
RS_FLAGS = --include-dirs=$(WXW)\include

CC_FLAGS = -O2 -Wredundant-decls -march=pentium-mmx -c -Wall -pipe -mthreads
INCLUDE0_PATH = $(WXW)\lib\gcc_dll\mswu
INCLUDE1_PATH = $(WXW)\include
INCLUDE2_PATH = $(WXW)\contrib\include
INCLUDE = -I$(INCLUDE0_PATH) -I$(INCLUDE1_PATH) -I$(INCLUDE2_PATH)

DEFS = -D__GNUWIN32__ -D__WXMSW__ -DWXUSINGDLL -DwxUSE_UNICODE

# Linker
LD = ld
LD_FLAGS = -s -Wall -pipe -mthreads
#-Wl,--subsystem,windows -mwindows -lkernel32 -luser32 -lgdi32 -lcomdlg32 -lwinspool -lwinmm -lshell32


OBJDIR = nobj


all: $(OBJDIR)\EditLabelDlg.o \
	$(OBJDIR)\FileTypeDialog.o \
	$(OBJDIR)\MnemonicItem.o \
	$(OBJDIR)\ProcessData.o \
	$(OBJDIR)\codeview.o \
	$(OBJDIR)\codeviewline.o \
	$(OBJDIR)\d_asm_element.o \
	$(OBJDIR)\dasmdata.o \
	$(OBJDIR)\labelslist.o \
	$(OBJDIR)\mndb.o \
	$(OBJDIR)\mndb_tools.o \
	$(OBJDIR)\projectmanager.o \
	$(OBJDIR)\rawdata.o \
	$(OBJDIR)\IDZ80.o \
	$(OBJDIR)\IDZ80App.o \
	$(OBJDIR)\resource.o

	$(CC) -o idz80_make.exe $(LD_FLAGS) -L$(WXW)\lib\gcc_dll -lwxmsw28u_gcc_custom $(OBJDIR)\resource.o $(OBJDIR)\EditLabelDlg.o $(OBJDIR)\FileTypeDialog.o $(OBJDIR)\MnemonicItem.o $(OBJDIR)\ProcessData.o $(OBJDIR)\codeview.o $(OBJDIR)\codeviewline.o $(OBJDIR)\d_asm_element.o $(OBJDIR)\dasmdata.o $(OBJDIR)\labelslist.o $(OBJDIR)\mndb.o $(OBJDIR)\mndb_tools.o $(OBJDIR)\projectmanager.o $(OBJDIR)\rawdata.o $(OBJDIR)\IDZ80.o $(OBJDIR)\IDZ80App.o
	@echo $@ done !


$(OBJDIR)\EditLabelDlg.o: src\EditLabelDlg.cpp src\EditLabelDlg.h
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ src\EditLabelDlg.cpp $(INCLUDE)

$(OBJDIR)\resource.o: bin\Release\resource.res
	windres --use-temp-file -i$< -o$@    --define __WXMSW__

$(OBJDIR)\FileTypeDialog.o: src\FileTypeDialog.cpp src\FileTypeDialog.h
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ src\FileTypeDialog.cpp $(INCLUDE)

$(OBJDIR)\MnemonicItem.o: src\MnemonicItem.cpp src\MnemonicItem.h
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ src\MnemonicItem.cpp $(INCLUDE)

$(OBJDIR)\ProcessData.o: src\ProcessData.cpp src\ProcessData.h
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ src\ProcessData.cpp $(INCLUDE)

$(OBJDIR)\codeview.o: src\codeview.cpp src\codeview.h
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ src\codeview.cpp $(INCLUDE)
	
$(OBJDIR)\codeviewline.o: src\codeviewline.cpp src\codeviewline.h
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ src\codeviewline.cpp $(INCLUDE)

$(OBJDIR)\d_asm_element.o: src\d_asm_element.cpp src\d_asm_element.h
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ src\d_asm_element.cpp $(INCLUDE)
	
$(OBJDIR)\dasmdata.o: src\dasmdata.cpp src\dasmdata.h
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ src\dasmdata.cpp $(INCLUDE)

$(OBJDIR)\labelslist.o: src\labelslist.cpp src\labelslist.h
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ src\labelslist.cpp $(INCLUDE)

$(OBJDIR)\mndb.o: src\mndb.cpp src\mndb.h
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ src\mndb.cpp $(INCLUDE)

$(OBJDIR)\mndb_tools.o: src\mndb_tools.cpp src\mndb_tools.h
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ src\mndb_tools.cpp $(INCLUDE)

$(OBJDIR)\projectmanager.o: src\projectmanager.cpp src\projectmanager.h
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ src\projectmanager.cpp $(INCLUDE)

$(OBJDIR)\rawdata.o: src\rawdata.cpp src\rawdata.h
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ src\rawdata.cpp $(INCLUDE)

$(OBJDIR)\IDZ80.o: src\IDZ80.cpp src\IDZ80.h	
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ src\IDZ80.cpp $(INCLUDE)

$(OBJDIR)\IDZ80App.o: src\IDZ80App.cpp src\IDZ80App.h
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ src\IDZ80App.cpp $(INCLUDE)

$(OBJDIR)\resource.o: resource.rc
	$(RS) -o $@ resource.rc


clean:
	del $(OBJDIR)\*.o


### Makefile IDZ80 ###

# Check if it's windows / Verifica se o sistema operacional é windows
ifeq ($(OS),Windows_NT)
        $(info OS var is $(OS))
else
        $(warning OS var is not set!)
endif

# wxWidgets
WXW_INSTALLED_VERSION := $(shell wx-config --version)
WXW_EXPECTED_VERSION := 3.2
WXVERSION_SHORT = 32
WXW_INCLUDE1 := C:/msys64/ucrt64/include/wx-3.2
WXW_INCLUDE2 := C:/msys64/ucrt64/lib/wx/include/msw-unicode-3.2
WXLIBSDIR := C:/msys64/ucrt64/bin

$(info Expected wxwidgets version is $(WXW_EXPECTED_VERSION), installed version is $(WXW_INSTALLED_VERSION).)


# Compiler / Compilador
CC = g++

# Debug
DEBUG ?= 0

# Resource
RS := windres
RS_FLAGS := -I$(WXW_INCLUDE1)
RESOURCE_FILE := resource.rc


# Project's path / Caminhos do projeto
SRCDIR := src
PROJECT_INCLUDE := ./include
RESOURCE_DIR := ./resource

# Date and time / Date e hora
DATE = $(shell date +%yy%mm%dd)
TIME = $(shell date +%Hh%Mm%Ss)

#Backup settings
BACKUPFILE := idz80_$(DATE)_$(TIME)

# Selects compiler's flags, libs and other between release and debug / Seleciona opções do compilador, bibliotecas e outros para release ou debug.
ifeq ($(DEBUG), 0)
#	Release
	CC_FLAGS = -Wredundant-decls -c -Wall -Wextra -pipe -mthreads -std=c++17
	DEFS = -D__WXMSW__ -DwxUSE_UNICODE -DWXUSINGDLL
# -D__GNUWIN32__
	INCLUDE0_PATH := $(WXW_INCLUDE2)
	LIBFILE0 := wxbase32u_gcc_custom
	LIBFILE1 := wxmsw32u_core_gcc_custom
	LIBFILE2 := wxmsw32u_aui_gcc_custom
	LIBFILE3 := wxmsw32u_adv_gcc_custom
	LIBFILE4 := wxbase32u_xml_gcc_custom
else
#	DEBUG - Not updated for MSYS2 yet / Ainda não atualizado para utilizar MSYS2
	CC_FLAGS = -g -Wredundant-decls -c -Wall -Wextra -pedantic -Wsequence-point -pipe -mthreads -std=c++17
	DEFS = -D__WXMSW__ -DwxUSE_UNICODE -DWXUSINGDLL
	INCLUDE0_PATH := $(WXW_INCLUDE2)
	LIBFILE0 := wxbase32u_gcc_custom
	LIBFILE1 := wxmsw32u_core_gcc_custom
	LIBFILE2 := wxmsw32u_aui_gcc_custom
	LIBFILE3 := wxmsw32u_adv_gcc_custom
	LIBFILE4 := wxbase32u_xml_gcc_custom
#	CC_FLAGS = -Wredundant-decls -c -Wall -pipe -mthreads -g -std=c++17 -Wsequence-point
#	DEFS = -D__GNUWIN32__ -D__WXMSW__ -DWXUSINGDLL -DwxUSE_UNICODE -D__WXDEBUG__ -DIDZ80DEBUG -DIDZ80_DECODER -DIDZ80_DASMED
#	INCLUDE0_PATH = $(WXW)/lib/gcc_dll/mswud
#	LIBFILE1 := -lwxbase$(WXVERSION_SHORT)ud_gcc_custom
#	LIBFILE2 := -lwxmsw$(WXVERSION_SHORT)ud_aui_gcc_custom
#	LIBFILE3 := -lwxmsw$(WXVERSION_SHORT)ud_core_gcc_custom
endif


INCLUDE :=-I$(PROJECT_INCLUDE) -I$(WXW_INCLUDE1) -I$(INCLUDE0_PATH)

# ld flags / Opções de linkedição
LDFLAGS =  -Wl,--subsystem,windows -mwindows

# libs / Bibliotecas
LIBS := -L$(WXLIBSDIR) -l$(LIBFILE0) -l$(LIBFILE1) -l$(LIBFILE2) -l$(LIBFILE3) -l$(LIBFILE4)


ifeq ($(DEBUG), 0)
	OBJDIR = obj/Release
	BINDIR = bin/Release
else
	OBJDIR = obj/Debug
	BINDIR = bin/Debug
endif

# wildcard lists source files to the SOURCE_FILES_FULL variable / wildcard é utilizado para listar os arquivos-fonte para a variavel SOURCE_FILES_FULL
SOURCE_FILES_FULL := $(wildcard $(SRCDIR)/*.cpp)
# TEMP - working with few files, for now/ Usando somente poucos arquivos por enquanto
SOURCES_TEMP := src/binfile.cpp src/cartridge_rom_file.cpp src/idz80_app.cpp src/main_dialog.cpp src/main_dialog_setup.cpp src/main_dialog_events.cpp src/logbase.cpp src/logwindow.cpp src/labelslist.cpp src/labelmanager.cpp src/rawdatamanager.cpp src/rawdata.cpp src/newproject_dialog.cpp src/systemlabels.cpp src/mnemonic_access.cpp src/mnemonic_container.cpp src/mnemonic_item_base.cpp src/mnemonic_item.cpp src/mnemonicxmlfile.cpp src/file_settings_dialog.cpp src/disassembled_manager.cpp src/disassembled_container.cpp src/disassembled_item.cpp src/disassembled_item_data.cpp
OBJETOS_FULL := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SOURCES_TEMP))
OBJETOS_FULL += $(OBJDIR)/$(RESOURCE_FILE:.rc=.o)    # Add the compiled resource
#SOURCES_TEMP := $(notdir $(SOURCE_FILES_FULL))     #remove o diretorio antes do arquivo

# Generates a list of object files using the list of source files / gera lista dos arquivos-objeto usando a lista de arquivos-fonte
OBJECTS := $(SOURCES_TEMP:.cpp=.o)

# Executable file name / Nome do arquivo executável
TARGET := idz80_make.exe

.PHONY: all clean print

# Rule for all / Regra para todos
all: $(BINDIR)/$(TARGET)

#Rule to linkeditor / Regra Linkar $(OBJECTS)
$(BINDIR)/$(TARGET): $(OBJETOS_FULL) | $(BINDIR)
	$(CC) $(LDFLAGS) $(LIBS) -o $@ $^

#Rule for compiling / Regra para compilar
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	@echo "Compiling $@ - [$^] ------------------------------------------------------------"
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ -c $^ $(INCLUDE)

# Rule to compile resource files (example for Windows resources)
$(OBJDIR)/%.o: $(RESOURCE_DIR)/%.rc | $(OBJDIR)
	$(RS) $(RS_FLAGS) $< -o $@

# Rule to create object directory if it does not exist / Regra para criar diretorio de objetos, se não existir
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Rule to create binary directory if it does not exist / Regra para criar diretorio do executável, se não existir
$(BINDIR):
	mkdir -p $(BINDIR)

#Rule for cleaning / Regra para limpeza de arquivos objeto e TARGET
clean:
	rm -f $(OBJDIR)/*.o
	rm -f $(BINDIR)/$(TARGET)

backup: clean
	7z a -t7z $(BACKUPFILE) *
	@if [ -f "$(BACKUPFILE).7z" ]; then \
		mv "$(BACKUPFILE).7z" "../Backup/"; \
		echo "$(BACKUPFILE).7z moved to ../Backup/"; \
	else \
		echo "$(BACKUPFILE).7z does not exist, skipping move."; \
	fi


# Rule to show the variables / Regra para mostrar variaveis
print:
	@echo SOURCES_TEMP = $(SOURCES_TEMP)
	@echo ---------
	@echo SOURCE_FILES_FULL = $(SOURCE_FILES_FULL)
	@echo ---------
	@echo OBJECTS = $(OBJECTS)
	@echo ---------
	@echo OBJETOS_FULL = $(OBJETOS_FULL)
	@echo $(DATE)
	@echo $(TIME)
	@echo INCLUDE = $(INCLUDE)
	@echo LIBS = $(LIBS)
	@echo EXECUTÁVEL = $(TARGET)
	@echo BACKUPFILE = $(BACKUPFILE)


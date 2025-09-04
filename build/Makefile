### Makefile IDZ80 ###

# Verifica Sistema Operacional
ifeq ($(OS),Windows_NT)
        $(info OS var is $(OS))
else
        $(warning OS var is not set!)
endif

# Drive virtual pasta Coding
#DRV = K:

# wxWidgets
WXWVERSION = 32
WXVERSION_SHORT = 32
WXW_INCLUDE1 ?= C:/msys64/ucrt64/include/wx-3.2
       #$(DRV)/lib/wxw/wx$(WXWVERSION)
WXW_INCLUDE2 ?= C:/msys64/ucrt64/lib/wx/include/msw-unicode-3.2
PROJECT_INCLUDE ?= ./include
WXLIBSDIR := C:/msys64/ucrt64/bin


# Compilador
CC = g++

# Debug
DEBUG ?= 0

# Resource
RS = windres
RS_FLAGS = --include-dirs=$(WXW_INCLUDE1)

# Date and time
DATE = $(shell date +%y-%m-%d)
TIME = $(shell date +%T)

ifeq ($(DEBUG), 0)
	# Release
	CC_FLAGS = -Wredundant-decls -c -Wall -Wextra -pipe -mthreads -std=c++17
	DEFS = -D__WXMSW__ -DwxUSE_UNICODE -DWXUSINGDLL
	# -D__GNUWIN32__
	INCLUDE0_PATH = $(WXW_INCLUDE2)
	LIBFILE1 = -lwxmsw32u_core_gcc_custom
	LIBFILE2 = -lwxmsw32u_aui_gcc_custom
	LIBFILE3 = -lwxmsw32u_adv_gcc_custom
else
	# DEBUG
	CC_FLAGS = -Wredundant-decls -c -Wall -pipe -mthreads -g -std=c++17 -Wsequence-point
	DEFS = -D__GNUWIN32__ -D__WXMSW__ -DWXUSINGDLL -DwxUSE_UNICODE -D__WXDEBUG__ -DIDZ80DEBUG -DIDZ80_DECODER -DIDZ80_DASMED
	INCLUDE0_PATH = $(WXW)/lib/gcc_dll/mswud
	LIBFILE1 = -lwxbase$(WXVERSION_SHORT)ud_gcc_custom
	LIBFILE2 = -lwxmsw$(WXVERSION_SHORT)ud_aui_gcc_custom
	LIBFILE3 = -lwxmsw$(WXVERSION_SHORT)ud_core_gcc_custom
endif

INCLUDE1_PATH = $(WXW_INCLUDE1)
INCLUDE2_PATH = $(PROJECT_INCLUDE)
#INCLUDE2_PATH = $(WXW)/contrib/include
INCLUDE =-I$(INCLUDE1_PATH) -I$(INCLUDE0_PATH) -I$(INCLUDE2_PATH)

# Opções de linkedição
LDFLAGS =  -Wl,--subsystem,windows -mwindows

# Bibliotecas
LIBS := -LC:/msys64/ucrt64/bin -lwxbase32u_gcc_custom -lwxmsw32u_core_gcc_custom -lwxmsw32u_aui_gcc_custom -lwxmsw32u_adv_gcc_custom
# -L $(WXLIBSDIR) $(LIBFILE1) $(LIBFILE2) $(LIBFILE3)

# libraries's path
LIBPATH = -L$(WXW)/lib

# Project's path
SRCDIR := src

ifeq ($(DEBUG), 0)
	OBJDIR = obj/Release
	BINDIR = bin/Release
else
	OBJDIR = obj/Debug
	BINDIR = bin/Debug
endif

# wildcard é utilizado para listar os arquivos-fonte no diretório atual
FONTES_FULL = $(wildcard $(SRCDIR)/*.cpp)    #lista todos os arquivos-fonte
# Usando somente poucos arquivos para compilar
FONTES := src/binfile.cpp src/cartridge_rom_file.cpp src/idz80_app.cpp src/idz80_base.cpp src/idz80_main_dialog.cpp src/logbase.cpp src/logwindow.cpp src/labelslist.cpp src/labelmanager.cpp src/rawdatamanager.cpp src/rawdata.cpp src/newproject_dialog.cpp src/systemlabels.cpp
OBJETOS_FULL = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(FONTES))
#FONTES := $(notdir $(FONTES_FULL))     #remove o diretorio antes do arquivo



# gera lista dos arquivos-objeto usando a lista de arquivos-fonte
OBJETOS = $(FONTES:.cpp=.o)

# Nome do arquivo executável
EXECUTAVEL = idz80_make.exe

.PHONY: all clean print

#Regra all (Todos)
all: $(BINDIR)/$(EXECUTAVEL)

#Regra Linkar $(OBJETOS)
$(BINDIR)/$(EXECUTAVEL): $(OBJETOS_FULL) | $(BINDIR)
	$(CC) $(LDFLAGS) $(LIBS) -o $@ $^

#Regra Compilar
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	@echo "Compilando $@ - [$^] ------------------------------------------------------------"
	$(CC) $(CC_FLAGS) $(DEFS) -o $@ -c $^ $(INCLUDE)

# Regra para criar diretorio de objetos, se não existir
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Regra para criar diretorio de executavel, se não existir
$(BINDIR):
	mkdir -p $(BINDIR)

clean:
	rm -f $(OBJDIR)/*.o
	rm -f $(BINDIR)/$(EXECUTAVEL)


# Regra mostra variaveis
print:
	@echo $(FONTES)
	@echo ---------
	@echo $(FONTES_FULL)
	@echo ---------
	@echo $(OBJETOS)
	@echo ---------
	@echo $(OBJETOS_FULL)
	@echo $(DATE)
	@echo $(TIME)
	@echo $(INCLUDE)
	@echo $(LIBS)
	@echo $(EXECUTAVEL)

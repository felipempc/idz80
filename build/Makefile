### Makefile IDZ80 ###

# Drive virtual pasta Coding
DRV = K:

# wxWidgets
WXWVERSION = 32
WXVERSION_SHORT = 30
WXW_INCLUDE1 ?= /ucrt64/include/wx-3.2/
       #$(DRV)/lib/wxw/wx$(WXWVERSION)
WXW_INCLUDE2 ?= /ucrt64/lib/wx/include/msw-unicode-3.2
PROJECT_INCLUDE ?= ./include


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
	CC_FLAGS = -O2 -Wredundant-decls -c -Wall -pipe -mthreads -std=c++98
	DEFS = -D__GNUWIN32__ -D__WXMSW__ -DWXUSINGDLL -DwxUSE_UNICODE
	INCLUDE0_PATH = $(WXW_INCLUDE2)
	LIBFILE1 = -llibwx_baseu-3.2.dll
	LIBFILE2 = -llibwx_mswu_aui-3.2.dll
	LIBFILE3 = -llibwx_mswu_core-3.2.dll
else
	# DEBUG
	CC_FLAGS = -Wredundant-decls -c -Wall -pipe -mthreads -g -std=c++98
	DEFS = -D__GNUWIN32__ -D__WXMSW__ -DWXUSINGDLL -DwxUSE_UNICODE -D__WXDEBUG__ -DIDZ80DEBUG -DIDZ80_DECODER -DIDZ80_DASMED
	INCLUDE0_PATH = $(WXW)/lib/gcc_dll/mswud
	LIBFILE1 = -lwxbase$(WXVERSION_SHORT)ud_gcc_custom
	LIBFILE2 = -lwxmsw$(WXVERSION_SHORT)ud_aui_gcc_custom
	LIBFILE3 = -lwxmsw$(WXVERSION_SHORT)ud_core_gcc_custom
endif

INCLUDE1_PATH = $(PROJECT_INCLUDE)
#INCLUDE2_PATH = $(WXW)/contrib/include
INCLUDE = -I$(INCLUDE0_PATH) -I$(INCLUDE1_PATH)

# Opções de linkedição
LDFLAGS =  -Wl,--subsystem,windows -mwindows

# Bibliotecas
LIBS = $(LIBFILE1) $(LIBFILE2) $(LIBFILE3)

# libraries's path
LIBPATH = -L$(WXW)/lib

# Project's path
ifeq ($(DEBUG), 0)
	OBJDIR = ../obj/Release
	OBJDIR_SHORT = obj/Release
	BINDIR = ../bin/Release
else
	OBJDIR = ../obj/Debug
	OBJDIR_SHORT = obj/Debug
	BINDIR = ../bin/Debug
endif

SRCDIR = ../src
RESDIR = ..


# wildcard é utilizado para listar os arquivos-fonte no diretório atual
#FONTES_FULL = 
              #$(wildcard ../src/*.cpp)
FONTES := $(shell ls src/*.cpp)
         #$(notdir $(FONTES_FULL))

# gera lista dos arquivos-objeto usando a lista de arquivos-fonte
OBJETOS = $(FONTES:.cpp=.o)
#OBJETOS = $(subst src,$(OBJDIR_SHORT),$(OBJ_1))

# Nome do arquivo executável
EXECUTAVEL = $(BINDIR)/idz80_make.exe

# Define os diretorios para make
VPATH = $(OBJDIR) $(SRCDIR)

#Regra all (Todos)
all: $(EXECUTAVEL)

# Regra mostra variaveis
print:
	@echo $(FONTES)
	@echo ---------
	@echo $(OBJETOS)
	@echo ---------
	@echo $(DATE)
	@echo $(TIME)
	@echo $(INCLUDE)
	@echo $(LIBS)
	@echo $(EXECUTAVEL)

#Regra Linkar
$(EXECUTAVEL): $(OBJETOS)
	$(CC) -o $@ $^ $(LDFLAGS)

#Regra Compilar
%.o: %.cpp
	@echo "Compilando $@ - [$^] ------------------------------------------------------------"
	$(CC) $(CC_FLAGS) $(DEFS) -o $(OBJDIR)/$@ -c $^ $(INCLUDE)

.PHONY: clean
clean:
	ver
	-if exist $(OBJDIR)/*.o del \"$(OBJDIR)/*.o\""
	-if exist $(EXECUTAVEL) del "$(EXECUTAVEL)"

.PHONY: check
check:
	echo $(FONTES)
	echo -------------------------------------------------------------------------------
	echo $(OBJETOS)
	

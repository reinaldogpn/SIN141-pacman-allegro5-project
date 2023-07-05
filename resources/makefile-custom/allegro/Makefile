## Definições globais I ##

# Indica que ao usar o comando "make", serão executadas as instruções definidas em "build"
.DEFAULT_GOAL := build

# Compilador / linker
CC=g++

# Flags para o compilador
CC_FLAGS=-std=c++11 -c -s -w -O2

# Arquivos .cpp
C_SOURCE=$(wildcard ./source/*.cpp)

# Arquivos .h
H_SOURCE=$(wildcard ./source/*.h)

# Arquivos .o
OBJ=$(subst .cpp,.o,$(subst source,objects,$(C_SOURCE)))

#-------------------------------------------------------------------------------------------------------#
## Condicionais ##

# As instruções abaixo são executadas somente se o SO for Windows
ifeq ($(OS),Windows_NT)

# Nome do projeto
PROJ_NAME=Pacman.exe

# Link .dll
DLL_LINK=https://github.com/reinaldogpn/makefile/raw/main/allegro/allegro_windows/allegro_monolith-5.2.dll

# Flags para o Allegro 5
ALLEGRO_PATH=C:\allegro

ALLEGRO_INCLUDE=-I $(ALLEGRO_PATH)\include

ALLEGRO_LIB=$(ALLEGRO_PATH)\lib\liballegro_monolith.dll.a

$(PROJ_NAME): $(OBJ)
	@ echo =======================================================================================
	@ echo Gerando binarios utilizando o $(CC) ...
	@ $(CC) $^ -o $@ $(ALLEGRO_LIB)
	@ echo =======================================================================================
	@ echo Tudo certo! Arquivo executavel gerado: $@
	@ echo =======================================================================================
	@ echo Este programa requisita o arquivo "allegro_monolith-5.2.dll" para ser executado ...
	@ echo ... este deve estar localizado no mesmo diretorio do arquivo "$(PROJ_NAME)".
	@ echo Caso nao o possua, voce pode fazer o download no link:
	@ echo =======================================================================================
	@ echo $(DLL_LINK)
	@ echo =======================================================================================

./objects/%.o: ./source/%.cpp ./source/%.h
	@$ echo Linkando o arquivo alvo utilizando o $(CC): $<
	@ $(CC) $< $(CC_FLAGS) $(ALLEGRO_INCLUDE) -o $@

./objects/main.o: ./source/main.cpp $(H_SOURCE)
	@ echo Linkando o arquivo main utilizando o $(CC): $<
	@ $(CC) $< $(CC_FLAGS) $(ALLEGRO_INCLUDE) -o $@

./objects/Main.o: ./source/Main.cpp $(H_SOURCE)
	@ echo Linkando o arquivo main utilizando o $(CC): $<
	@ $(CC) $< $(CC_FLAGS) $(ALLEGRO_INCLUDE) -o $@

# Criação de uma pasta para guardar os arquivos .o
objFolder:
	@ if not exist objects mkdir objects

# Execução do programa ao usar "make run"
run:
	@ echo =======================================================================================
	@ echo Executando o programa: $(PROJ_NAME)
	@ echo =======================================================================================
	@ $(PROJ_NAME)

# Faz a limpeza dos arquivos
clean:
	@ if exist $(PROJ_NAME) del /Q $(PROJ_NAME)
	@ if exist objects rmdir /S /Q objects

else

# As instruções abaixo são executadas somente se o SO for Linux.
    UNAME_S := $(shell uname -s)

    ifeq ($(UNAME_S),Linux)
        
        DISTRO_VER := $(shell cat /etc/issue)
		
# O parâmetro "-e" é adicionado ao comando "echo" se o SO for Arch Linux para as cores funcionarem corretamente.
        ifeq ($(DISTRO_VER),Arch Linux \r (\l))
            
             ECHO_FLAG=echo -e
        else

             ECHO_FLAG=echo
        endif

# Nome do projeto
PROJ_NAME=Pacman.out

# Flags para o Allegro5.2
ALLEGRO_FLAGS=-L/lib       \
              -lallegro    \
              `pkg-config            \
              --libs                 \
              allegro-5              \
              allegro_audio-5        \
              allegro_dialog-5       \
              allegro_image-5        \
              allegro_memfile-5      \
              allegro_primitives-5   \
              allegro_acodec-5       \
              allegro_color-5        \
              allegro_font-5         \
              allegro_main-5         \
              allegro_physfs-5       \
              allegro_ttf-5` 

ALLEGRO_INCLUDE=-I/usr/include/allegro5

## INFO: Terminal color codes
#
# Black=		"30"
# Red=			"31"
# Green=		"32"
# Yellow=		"33"
# Blue=			"34"
# Magenta=		"35"
# Cyan=			"36"
# LightGray=		"37"
# Gray=			"90"
# LightRed=		"91"
# LightGreen=		"92"
# LightYellow=	        "93"
# LightBlue=		"94"
# LightMagenta=	        "95"
# LightCyan=		"96"
# White=		"97"

# Color code
CCODE=93

COLOR="\e[0;$(CCODE)m"
COLOR_BOLD="\e[1;$(CCODE)m"
NO_COLOR="\e[0m"

UNDERLINE="\e[4m"
BLINK="\e[5m"
DIM="\e[2m"

$(PROJ_NAME): $(OBJ)
	@ $(ECHO_FLAG) $(COLOR_BOLD)"Gerando binários utilizando o $(CC) ..."$(NO_COLOR)
	@ $(CC) $^ -o $@ $(ALLEGRO_INCLUDE) $(ALLEGRO_FLAGS)
	@ $(ECHO_FLAG) ''
	@ $(ECHO_FLAG) $(COLOR_BOLD)"Criação de binários finalizada! Arquivo executável gerado:"$(NO_COLOR)
	@ $(ECHO_FLAG) $(COLOR_BOLD)"-->" $(UNDERLINE)$@$(NO_COLOR)
	@ $(ECHO_FLAG) ''

./objects/%.o: ./source/%.cpp ./source/%.h
	@ $(ECHO_FLAG) $(COLOR_BOLD)"Linkando o arquivo alvo utilizando o $(CC):" $(COLOR)$<$(NO_COLOR)
	@ $(CC) $< $(CC_FLAGS) -o $@ $(ALLEGRO_INCLUDE) $(ALLEGRO_FLAGS)

./objects/main.o: ./source/main.cpp $(H_SOURCE)
	@ $(ECHO_FLAG) $(COLOR_BOLD)"Linkando o arquivo main utilizando o $(CC):" $(COLOR)$<$(NO_COLOR)
	@ $(CC) $< $(CC_FLAGS) -o $@ $(ALLEGRO_INCLUDE) $(ALLEGRO_FLAGS)

./objects/Main.o: ./source/Main.cpp $(H_SOURCE)
	@ $(ECHO_FLAG) $(COLOR_BOLD)"Linkando o arquivo main utilizando o $(CC):" $(COLOR)$<$(NO_COLOR)
	@ $(CC) $< $(CC_FLAGS) -o $@ $(ALLEGRO_INCLUDE) $(ALLEGRO_FLAGS)

# Criação de uma pasta para guardar os arquivos .o
objFolder:
	@ mkdir -p objects

# Executa o arquivo gerado
run:
	@ $(ECHO_FLAG) $(COLOR_BOLD)"Executando o arquivo gerado:" $(UNDERLINE)$(PROJ_NAME)$(NO_COLOR)
	@ $(ECHO_FLAG) ''
	@ ./$(PROJ_NAME)

clean:
	@ rm -rf ./objects/*.o $(PROJ_NAME) *~
	@ rmdir objects

     endif
     
endif

#-------------------------------------------------------------------------------------------------------#
## Definições globais II ##

# Compilação e linkedição
build: objFolder $(PROJ_NAME)

# Compila e executa o programa
all: build run

# Palavras declaradas como "alvo falso"
.PHONY: all clean
#-------------------------------------------------------------------------------------------------------#

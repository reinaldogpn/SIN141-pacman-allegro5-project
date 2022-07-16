#-------------------------------------------------------------------------------------------------------#
# *** Makefile padrão ***
#
# Após muita pesquisa e noites viradas, aprendi a criar um arquivo makefile padrão que funciona
# para qualquer projeto em C++, sem precisar alterar absolutamente nada no arquivo!!!
#
# E o melhor... funciona para windows e para linux!
#
# Esse makefile organiza o diretório do projeto e realiza a chamada do processo de compilação de forma
# automática e também faz a limpeza do diretório raiz do projeto ao acionar o comando "make clean".
#
# Para que funcione, este arquivo deve estar na pasta raiz do projeto, juntamente com outra pasta
# nomeada "source", onde devem estar contidos todos os arquivos .h e .cpp relacionados ao projeto.
#
# Ao chamar o comando "make" ou "make all" no terminal, será criada de forma automática uma pasta nomeada 
# "objects" na pasta raiz do projeto, onde estarão contidos os arquivos .o gerados. Além disso, será 
# gerado um arquivo executável com o nome definido na variável 'PROJ_NAME'.
#
# Ao chamar o comando "make run" no terminal, o programa é executado.
#-------------------------------------------------------------------------------------------------------#

.DEFAULT_GOAL := all

ifeq ($(OS),Windows_NT)

# Nome do projeto
PROJ_NAME=Pacman.exe

# Arquivos .cpp
C_SOURCE=$(wildcard ./source/*.cpp)

# Arquivos .h
H_SOURCE=$(wildcard ./source/*.h)

# Arquivos .o
OBJ=$(subst .cpp,.o,$(subst source,objects,$(C_SOURCE)))

# Icon
ICON=./doc/info.o

# Link .dll
DLL_LINK=https://github.com/reinaldogpn/makefile/raw/main/allegro_monolith-5.2.dll

# Compilador / linker
CC=g++

# Flags para o compilador
CC_FLAGS=-c         \
         -Wall

# Flags para o Allegro5.2
ALLEGRO_PATH=C:\allegro

ALLEGRO_INCLUDE=-I $(ALLEGRO_PATH)\include

ALLEGRO_LIB=$(ALLEGRO_PATH)\lib\liballegro_monolith.dll.a

# Execução do programa ao usar "make run"
run:
	@ echo =======================================================================================
	@ echo Executando o programa: $(PROJ_NAME)
	@ echo =======================================================================================
	@ $(PROJ_NAME)

# Compilação e linkedição
all: objFolder $(PROJ_NAME)

$(PROJ_NAME): $(OBJ)
	@ echo =======================================================================================
	@ echo Gerando binarios utilizando o $(CC) ...
	@ $(CC) $^ $(ICON) -o $@ $(ALLEGRO_LIB)
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
	@ mkdir objects

clean:
	@ del /Q $(PROJ_NAME)
	@ rmdir /S /Q objects

# Palavras declaradas como "alvo falso"
.PHONY: all clean

else

    UNAME_S := $(shell uname -s)

    ifeq ($(UNAME_S),Linux)
        
        DISTRO_VER := $(shell cat /etc/issue)
            
            ifeq ($(DISTRO_VER),Arch Linux \r (\l))
            
                 OPT_FLAG=-e

            else

                 OPT_FLAG= 

            endif

# Nome do projeto
PROJ_NAME=Pacman.out

# Arquivos .cpp
C_SOURCE=$(wildcard ./source/*.cpp)

# Arquivos .h
H_SOURCE=$(wildcard ./source/*.h)

# Arquivos .o
OBJ=$(subst .cpp,.o,$(subst source,objects,$(C_SOURCE)))

# Compilador / linker
CC=g++

# Flags para o compilador
CC_FLAGS=-c         \
         -Wall

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

## Terminal color codes
Black=			"30"
Red=			"31"
Green=			"32"
Yellow=			"33"
Blue=			"34"
Magenta=		"35"
Cyan=			"36"
LightGray=		"37"
Gray=			"90"
LightRed=		"91"
LightGreen=		"92"
LightYellow=	        "93"
LightBlue=		"94"
LightMagenta=	        "95"
LightCyan=		"96"
White=			"97"

# Color code
CCODE=$(LightYellow)

COLOR="\e[0;$(CCODE)m"
COLOR_BOLD="\e[1;$(CCODE)m"
NO_COLOR="\e[0m"

UNDERLINE="\e[4m"
BLINK="\e[5m"
DIM="\e[2m"

OPT=-e

# Executa o arquivo gerado
run:
	@ echo $(OPT_FLAG) "$(COLOR_BOLD)Executando o arquivo gerado: $(UNDERLINE)$(PROJ_NAME)$(NO_COLOR)"
	@ echo $(OPT_FLAG) ''
	@ ./$(PROJ_NAME)

# Compilação e linkedição
all: objFolder $(PROJ_NAME)

$(PROJ_NAME): $(OBJ)
	@ echo $(OPT_FLAG) $(COLOR_BOLD)"Gerando binários utilizando o $(CC) ..."$(NO_COLOR)
	@ $(CC) $^ -o $@ $(ALLEGRO_INCLUDE) $(ALLEGRO_FLAGS)
	@ echo $(OPT_FLAG) ''
	@ echo $(OPT_FLAG) $(COLOR_BOLD)"Criação de binários finalizada! Arquivo executável gerado:"$(NO_COLOR)
	@ echo $(OPT_FLAG) $(COLOR_BOLD)"-->" $(UNDERLINE)$@$(NO_COLOR) #"$(PWD)/"
	@ echo $(OPT_FLAG) ''

./objects/%.o: ./source/%.cpp ./source/%.h
	@$ echo $(OPT_FLAG) $(COLOR_BOLD)"Linkando o arquivo alvo utilizando o $(CC):" $(COLOR)$<$(NO_COLOR)
	@ $(CC) $< $(CC_FLAGS) -o $@ $(ALLEGRO_INCLUDE) $(ALLEGRO_FLAGS)

./objects/main.o: ./source/main.cpp $(H_SOURCE)
	@ echo $(OPT_FLAG) $(COLOR_BOLD)"Linkando o arquivo main utilizando o $(CC):" $(COLOR)$<$(NO_COLOR)
	@ $(CC) $< $(CC_FLAGS) -o $@ $(ALLEGRO_INCLUDE) $(ALLEGRO_FLAGS)

./objects/Main.o: ./source/Main.cpp $(H_SOURCE)
	@ echo $(OPT_FLAG) $(COLOR_BOLD)"Linkando o arquivo main utilizando o $(CC):" $(COLOR)$<$(NO_COLOR)
	@ $(CC) $< $(CC_FLAGS) -o $@ $(ALLEGRO_INCLUDE) $(ALLEGRO_FLAGS)

# Criação de uma pasta para guardar os arquivos .o
objFolder:
	@ mkdir objects

clean:
	@ rm -rf ./objects/*.o $(PROJ_NAME) *~
	@ rmdir objects

# Palavras declaradas como "alvo falso"
.PHONY: all clean

     endif
endif

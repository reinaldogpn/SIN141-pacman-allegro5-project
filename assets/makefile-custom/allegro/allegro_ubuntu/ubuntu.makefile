#-------------------------------------------------------------------------------------------------------#
# *** Makefile padrão ***
#
# Após muita pesquisa e noites viradas, aprendi a criar um arquivo makefile padrão que funciona
# para qualquer projeto em C++, sem precisar alterar absolutamente nada no arquivo!!!
#
# Esse makefile organiza o diretório do projeto e realiza a chamada do processo de compilação de forma
# automática e também faz a limpeza do diretório raiz do projeto ao acionar o comando "make clean".
#
# Para que funcione, este arquivo deve estar na pasta raiz do projeto, juntamente com outra pasta
# nomeada "source", onde devem estar contidos todos os arquivos .h e .cpp relacionados ao projeto.
#
# Ao chamar o comando "make" no terminal, será criada de forma automática uma pasta nomeada "objects"
# na pasta raiz do projeto, onde estarão contidos os arquivos .o gerados. Além disso, será gerado um
# arquivo executável com o nome definido na variável 'PROJ_NAME'.
#-------------------------------------------------------------------------------------------------------#
.DEFAULT_GOAL := build

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
CC_FLAGS=-std=c++11 -c -s -w -O2

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

# Compilação e linkedição

build: objFolder $(PROJ_NAME)

$(PROJ_NAME): $(OBJ)
	@ echo $(COLOR_BOLD)"Gerando binários utilizando o $(CC) ..."$(NO_COLOR)
	@ $(CC) $^ -o $@ $(ALLEGRO_INCLUDE) $(ALLEGRO_FLAGS)
	@ echo ''
	@ echo $(COLOR_BOLD)"Criação de binários finalizada! Arquivo executável gerado:"$(NO_COLOR)
	@ echo $(COLOR_BOLD)"-->" $(UNDERLINE)$@$(NO_COLOR) #"$(PWD)/"
	@ echo ''

./objects/%.o: ./source/%.cpp ./source/%.h
	@$ echo $(COLOR_BOLD)"Linkando o arquivo alvo utilizando o $(CC):" $(COLOR)$<$(NO_COLOR)
	@ $(CC) $< $(CC_FLAGS) -o $@ $(ALLEGRO_INCLUDE) $(ALLEGRO_FLAGS)

./objects/main.o: ./source/main.cpp $(H_SOURCE)
	@ echo $(COLOR_BOLD)"Linkando o arquivo main utilizando o $(CC):" $(COLOR)$<$(NO_COLOR)
	@ $(CC) $< $(CC_FLAGS) -o $@ $(ALLEGRO_INCLUDE) $(ALLEGRO_FLAGS)

./objects/Main.o: ./source/Main.cpp $(H_SOURCE)
	@ echo $(COLOR_BOLD)"Linkando o arquivo main utilizando o $(CC):" $(COLOR)$<$(NO_COLOR)
	@ $(CC) $< $(CC_FLAGS) -o $@ $(ALLEGRO_INCLUDE) $(ALLEGRO_FLAGS)

# Criação de uma pasta para guardar os arquivos .o
objFolder:
	@ mkdir objects

# Executa o arquivo gerado
run:
	@ echo $(COLOR_BOLD)"Executando o arquivo gerado: "$(UNDERLINE)$(PROJ_NAME)$(NO_COLOR)
	@ echo ''
	@ ./$(PROJ_NAME)

# Compila e executa o programa
all: build run

clean:
	@ rm -rf ./objects/*.o $(PROJ_NAME) *~
	@ rmdir objects

# Palavras declaradas como "alvo falso"
.PHONY: all clean

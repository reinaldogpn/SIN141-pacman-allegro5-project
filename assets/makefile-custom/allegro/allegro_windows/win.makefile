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
PROJ_NAME=Pacman.exe

# Arquivos .cpp
C_SOURCE=$(wildcard ./source/*.cpp)

# Arquivos .h
H_SOURCE=$(wildcard ./source/*.h)

# Arquivos .o
OBJ=$(subst .cpp,.o,$(subst source,objects,$(C_SOURCE)))

# Link .dll
DLL_LINK=https://github.com/reinaldogpn/makefile/raw/main/allegro/allegro_windows/allegro_monolith-5.2.dll

# Compilador / linker
CC=g++

# Flags para o compilador
CC_FLAGS=-std=c++11 -c -s -w -O2

# Flags para o Allegro5.2
ALLEGRO_PATH=C:\allegro

ALLEGRO_INCLUDE=-I $(ALLEGRO_PATH)\include

ALLEGRO_LIB=$(ALLEGRO_PATH)\lib\liballegro_monolith.dll.a

# Compilação e linkedição
build: objFolder $(PROJ_NAME)

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
	@ if not exist objects mkdir objects

# Execução do programa ao usar "make run"
run:
	@ echo =======================================================================================
	@ echo Executando o programa: $(PROJ_NAME)
	@ echo =======================================================================================
	@ $(PROJ_NAME)

# Compila e executa o programa
all: build run

clean:
	@ if exist $(PROJ_NAME) del /Q $(PROJ_NAME)
	@ if exist objects rmdir /S /Q objects

# Palavras declaradas como "alvo falso"
.PHONY: all clean

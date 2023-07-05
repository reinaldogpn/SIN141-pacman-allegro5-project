## Global definitions I

.DEFAULT_GOAL := all

# Project name
PROJ_NAME=Program

#Compiler / linker (g++ / gcc)
CC=g++

# Source extension (.cpp / .c)
S_EXT=.cpp

# Header extension (.hpp / .h)
H_EXT=.h

# Compiler flags
CC_FLAGS=-c -s -w -O2

#-------------------------------------------------------------------------------------------------------#
## Instructions for Windows OS

ifeq ($(OS),Windows_NT)

# Project name with extension
EXE=$(PROJ_NAME).exe

# Creates a folder for keeping .o files
objFolder:
	@ if not exist objects mkdir objects

# Builds (if needed) and runs the program
run: all
	@ echo "Executando o arquivo gerado: $(EXE)"
	@ echo ''
	@ $(EXE)

# Clean up the project folder
clean:
	@ del /Q $(EXE)
	@ rmdir /S /Q objects

else

# Instructions for Linux OS
     UNAME_S := $(shell uname -s)

     ifeq ($(UNAME_S),Linux)

EXE=$(PROJ_NAME).run

objFolder:
	@ mkdir -p objects

run: all
	@ echo "Executando o arquivo gerado: $(EXE)"
	@ echo ''
	@ ./$(EXE)

clean:
	@ rm -rf objects $(EXE) *~

     endif

endif

#-------------------------------------------------------------------------------------------------------#
## Global definitions II

# Source files source (.c / .cpp)
C_SOURCE=$(wildcard ./source/*$(S_EXT))

# Header files source (.h / .hpp)
H_SOURCE=$(wildcard ./source/*$(H_EXT))

# Binary files instructions
OBJ=$(subst $(S_EXT),.o,$(subst source,objects,$(C_SOURCE)))

# Compiling / linking instructions
all: objFolder $(EXE)

$(EXE): $(OBJ)
	@ echo "Gerando binarios utilizando o $(CC) ..."
	@ $(CC) $^ -o $@
	@ echo ''
	@ echo "Criacao de binarios finalizada! Arquivo executavel gerado:"
	@ echo "-->" $@
	@ echo ''

./objects/%.o: ./source/%$(S_EXT) ./source/%$(H_EXT)
	@ echo "Linkando o arquivo alvo utilizando o $(CC):" $<
	@ $(CC) $< $(CC_FLAGS) -o $@

./objects/main.o: ./source/main$(S_EXT) $(H_SOURCE)
	@ echo "Linkando o arquivo main utilizando o $(CC):" $<
	@ $(CC) $< $(CC_FLAGS) -o $@

./objects/Main.o: ./source/Main$(S_EXT) $(H_SOURCE)
	@ echo "Linkando o arquivo main utilizando o $(CC):" $<
	@ $(CC) $< $(CC_FLAGS) -o $@

.PHONY: all clean

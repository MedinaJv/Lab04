##############################################################################
# Makefile para projeto de organização de arquivos e indexação em C++
##############################################################################
# Arquivo principal: main.cpp

# Nome do arquivo principal (contém a função main, sem extensão)
MAIN := main

# Objetos a serem gerados na compilação
OBJECTS := $(MAIN).o Livro.o Arvore.o

# Flags de compilação
FLAGS := -Wall -Wextra -std=c++17 -pedantic-errors

# Habilitar depuração
DEBUG := -g

# Necessário para funções matemáticas (se aplicável)
MATH := -lm

# Compilador
CC := g++

# Ajustando o executável ao sistema operacional
ifeq ($(OS), Windows_NT)
OUTPUTMAIN := $(MAIN).exe
else
OUTPUTMAIN := $(MAIN).out
endif

# Ponto de compilação principal
all: $(OUTPUTMAIN)
	@echo Compilacao completa!

# Gerando o arquivo executável
$(OUTPUTMAIN): $(OBJECTS)
	$(CC) $(FLAGS) $(OBJECTS) -o $(OUTPUTMAIN) $(MATH)

# Gerando o arquivo objeto da função principal
main.o: $(MAIN).cpp Livro.h Arvore.h
	$(CC) $(FLAGS) -c $(MAIN).cpp

# Gerando o arquivo objeto Livro.o
Livro.o: Livro.cpp Livro.h
	$(CC) $(FLAGS) -c Livro.cpp

# Gerando o arquivo objeto Arvore.o
Arvore.o: Arvore.cpp Arvore.h
	$(CC) $(FLAGS) -c Arvore.cpp

# Limpeza dos arquivos objetos e do executável
clean:
	rm -rf $(OBJECTS)
	rm -rf $(OUTPUTMAIN)
	@echo Cleanup completo!

# Executar o programa
run: all
	./$(OUTPUTMAIN)
	@echo Execucao completa!
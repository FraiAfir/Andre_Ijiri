# Makefile para o projeto TED - Trabalho de Estruturas de Dados
# Autor: Andre Ijiri

PROJ_NAME = ted # Nome do executável a ser gerado

ALUNO = Andre_Ijiri # Nome do aluno (sem espaços, para uso em nomes de arquivos)
LIBS  = -lm -lssp   # Bibliotecas necessárias (exemplo: math, stack protector, etc.)
# -lm:   Biblioteca matemática para funções como sqrt, pow, etc.
# -lssp: Biblioteca para proteção contra estouro de pilha (stack protector)

# Arquivos-fonte e objetos
SRCS  = main.c program.c tree.c digraph.c params.c geo.c qry.c svg.c txt.c via.c
OBJETOS = main.o program.o tree.o digraph.o params.o geo.o qry.o svg.o txt.o via.o
# $(SRCS:.c-.o)

# Compilador
CC = gcc

# Flags
CFLAGS  = -g -O0 -std=c99 -fstack-protector-all -Werror=implicit-function-declaration -Wall
LDFLAGS = 

# Regras de compilação
$(PROJ_NAME): $(OBJETOS)
	@echo "Compilando $(PROJ_NAME): ..."
	$(CC) -o $(PROJ_NAME) $(OBJETOS) $(LIBS) $(LDFLAGS)
	@echo "Compilacao finalizada com sucesso."

# Regra para compilar arquivos .c em .o
%.o : %.c
	@echo "Compilando $< ..."
	$(CC) -c $(CFLAGS) $< -o $@

# Dependências dos arquivos .o
program.o: program.h program.c
tree.o:    tree.h tree.c
digraph.o: digraph.h digraph.c
params.o:  params.h params.c
geo.o:     geo.h geo.c
qry.o:     qry.h qry.c
svg.o:     svg.h svg.c
txt.o:	   txt.h txt.c
via.o:     via.h via.c

# Dependências do arquivo main.o
main.o: main.c program.h tree.h digraph.h params.h geo.h qry.h svg.h txt.h via.h

# Regra para limpar os arquivos gerados na compilação
clean:
	@echo "Limpando os arquivos objetos e o executavel..."
	del /Q /F $(OBJETOS) $(PROJ_NAME).exe

# Declara que 'clean' não é um arquivo físico, mas sim um comando
.PHONY: clean
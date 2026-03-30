# Makefile para o projeto TED - Trabalho de Estruturas de Dados
# Autor: Andre Ijiri

PROJ_NAME = ted # Nome do executável a ser gerado

ALUNO = Andre_Ijiri # Nome do aluno (sem espaços, para uso em nomes de arquivos)
LIBS  = -lm -lssp   # Bibliotecas necessárias (exemplo: math, stack protector, etc.)
# -lm:   Biblioteca matemática para funções como sqrt, pow, etc.
# -lssp: Biblioteca para proteção contra estouro de pilha (stack protector)

# Detecção Automática do Sistema Operacional
ifeq ($(OS),Windows_NT)
    # Comandos para Windows (CMD/PowerShell)
    RM   = del /Q /F
    EXEC = .exe
else
    # Comandos para Linux / macOS / WSL
    RM   = rm -f
    EXEC = 
endif
# --------------------------------------------------

# Arquivos-fonte e objetos
SRCS    = main.c program.c tree.c params.c geo.c qry.c svg.c txt.c hashTable.c
OBJETOS = main.o program.o tree.o params.o geo.o qry.o svg.o txt.o hashTable.o
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
program.o:   program.h program.c
tree.o:      tree.h tree.c
params.o:    params.h params.c
geo.o:       geo.h geo.c
qry.o:       qry.h qry.c
svg.o:       svg.h svg.c
txt.o:	     txt.h txt.c
hashTable.o: hashTable.h hashTable.c
# Dependências do arquivo main.o
main.o: main.c program.h tree.h params.h geo.h qry.h svg.h txt.h hashTable.h

# Regra para limpar os arquivos gerados na compilação
clean:
	@echo "Limpando os arquivos objetos e o executavel..."
	$(RM) $(OBJETOS) $(PROJ_NAME)$(EXEC)

# Indica que 'clean' é uma regra que não gera um arquivo, para evitar conflitos com arquivos chamados 'clean'
.PHONY: clean 
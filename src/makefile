PROJ_NAME = ted

ALUNO = Andre_Ijiri
LIBS  = -lm
SRCS  = main.c lista.c digraph.c streap.c

OBJETOS = $(SRCS:.c-.o)

# compilador
CC = gcc

# Flags
CFLAGS= -ggdb -O0 -std=c99 -fstack-protector-all -Werror=implicit-function-
declaration
LDFLAGS=-O0

$(PROJ_NAME): $(OBJETOS)
	@echo "Compilando $(PROJ_NAME): ..."
	$(CC) -o $(PROJ_NAME)$(LDFLAGS) $(OBJETOS) $(LIBS)
	@echo "Compulação finalizada com sucesso."

%.o : %.c
	@echo "Compilando $< ..."
	$(CC) -c $(CFLAGS) $< -o $@

lista.o: lista.h lista.c
digraph.o: digraph.h digraph.c
streap.o: streap.h streap.c

main.o: main.c lista.h digraph.h streap.h
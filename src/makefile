PROJ_NAME = ted

ALUNO = Andre_Ijiri
LIBS  = -lm -lssp

SRCS  = main.c program.c tree.c digraph.c params.c geo.c qry.c svg.c txt.c via.c
OBJETOS = main.o program.o tree.o digraph.o params.o geo.o qry.o svg.o txt.o via.o
# $(SRCS:.c-.o)

# compilador
CC = gcc

# Flags
CFLAGS  = -g -O0 -std=c99 -fstack-protector-all -Werror=implicit-function-declaration -Wall
LDFLAGS = 

$(PROJ_NAME): $(OBJETOS)
	@echo "Compilando $(PROJ_NAME): ..."
	$(CC) -o $(PROJ_NAME) $(OBJETOS) $(LIBS) $(LDFLAGS)
	@echo "Compilacao finalizada com sucesso."

%.o : %.c
	@echo "Compilando $< ..."
	$(CC) -c $(CFLAGS) $< -o $@

program.o: program.h program.c
tree.o:    tree.h tree.c
digraph.o: digraph.h digraph.c
params.o:  params.h params.c
geo.o:     geo.h geo.c
qry.o:     qry.h qry.c
svg.o:     svg.h svg.c
txt.o:	   txt.h txt.c
via.o:     via.h via.c

main.o: main.c program.h tree.h digraph.h params.h geo.h qry.h svg.h txt.h via.h
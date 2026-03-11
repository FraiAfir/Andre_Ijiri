PROJ_NAME = ted

ALUNO = Andre_Ijiri
LIBS  = -lm -lssp

SRCS  = main.c lista.c digraph.c streap.c params.c geo.c qry.c svg.c txt.c via.c
OBJETOS = main.o lista.o digraph.o streap.o params.o geo.o qry.o svg.o txt.o via.o
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

lista.o:   lista.h lista.c
digraph.o: digraph.h digraph.c
streap.o:  streap.h streap.c
params.o:  params.h params.c
geo.o:     geo.h geo.c
qry.o:     qry.h qry.c
svg.o:     svg.h svg.c
txt.o:	   txt.h txt.c
via.o:     via.h via.c

main.o: main.c lista.h digraph.h streap.h params.h geo.h qry.h svg.h txt.h via.h
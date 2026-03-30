# Andre_Ijiri
Repositória para o Projeto 1 da matéria de Estrutura de Dados II

Aluno:     André Felipe Ijiri Ribeiro
Matrícula: 202400560032

# ATU (Arquivo de Teste Unitário)
C:\Users\andre\OneDrive\Documentos\GitHub\Andre_Ijiri
gcc ./test/test_hashTable.c ./src/hashTable.c ./unity/unity.c -I./src -I./unity -o ./test/test_runner
./test/test_runner

# Compilação
C:\Users\andre\OneDrive\Documentos\GitHub\Andre_Ijiri\src
mingw32-make
./ted -f t1.geo -q t2.qry -o ./
mingw32-make clean

# Valgrind
Abrir Powershell
Digitar "wsl"
Digitar "cd OneDrive/Documentos/GitHub/Andre_Ijiri/src"
Compilar com o Makefile
Executar "valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./ted -f teste.geo -q teste.qry -o ./"
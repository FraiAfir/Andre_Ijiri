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
./ted -e ../entradas/ -f t1.geo -pm t3.pm -q consultas/t2.qry -o ../saidas/
mingw32-make clean

# Valgrind
Abrir Powershell
Digitar "wsl"
Digitar "cd OneDrive/Documentos/GitHub/Andre_Ijiri/src"
Compilar com o Makefile: "make"
Executar "valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./ted -e ../entradas/ -f t1.geo -pm t3.pm -q consultas/t2.qry -o ../saidas/"

Resultado esperado:
==450==
==450== HEAP SUMMARY:
==450==     in use at exit: 0 bytes in 0 blocks
==450==   total heap usage: 876 allocs, 876 frees, 185,480 bytes allocated
==450==
==450== All heap blocks were freed -- no leaks are possible
==450==
==450== For lists of detected and suppressed errors, rerun with: -s
==450== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
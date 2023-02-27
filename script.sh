gcc -c lexer.c doublyLinkedList.c hashTable.c parser.c setADT.c stackADT.c parseTree.c
gcc -o a.out lexer.o doublyLinkedList.o hashTable.o parser.o setADT.o stackADT.o parseTree.o
./a.out
rm *.o
rm *a.out

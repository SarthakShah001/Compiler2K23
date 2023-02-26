gcc -c lexer.c doublyLinkedList.c hashTable.c parser.c setADT.c
gcc -o a.out lexer.o doublyLinkedList.o hashTable.o parser.o setADT.o
./a.out

rm *.o
rm *a.out

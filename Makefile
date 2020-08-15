all: main

main: main.c stack.o dlist.o avltree.o hash.o interfaz.o intervalo.o
	gcc  -std=c99 -o main main.c hash.o avltree.o stack.o dlist.o interfaz.o intervalo.o -lm

interfaz.o: manejoAlias/interfaz.c manejoAlias/interfaz.h hash.o avltree.o
	gcc -c -Wall -Wextra -Werror -std=c99 manejoAlias/interfaz.c

hash.o: manejoAlias/hash.c manejoAlias/hash.h
	gcc -c -Wall -Wextra -Werror -std=c99 manejoAlias/hash.c

avltree.o: arbolDeIntervalos/avltree.c arbolDeIntervalos/avltree.h stack.o dlist.o
	gcc -c -Wall -Wextra -Werror -std=c99 arbolDeIntervalos/avltree.c

intervalo.o: arbolDeIntervalos/intervalo.c arbolDeIntervalos/intervalo.h
	gcc -c -c -Wall -Wextra -Werror -std=c99 arbolDeIntervalos/intervalo.c

stack.o: arbolDeIntervalos/lists/stack.c arbolDeIntervalos/lists/stack.h dlist.o
	gcc -c -c -Wall -Wextra -Werror -std=c99 arbolDeIntervalos/lists/stack.c

dlist.o: arbolDeIntervalos/lists/dlist/dlist.c arbolDeIntervalos/lists/dlist/dlist.h
	gcc -c -c -Wall -Wextra -Werror -std=c99 arbolDeIntervalos/lists/dlist/dlist.c

clean:
	rm -f *.o main main.exe

all: main

main: main.c stack.o queue.o dlist.o avltree.o hash.o interfaz.o intervalo.o
	gcc  -std=c99 -o main main.c hash.o avltree.o stack.o queue.o dlist.o interfaz.o intervalo.o -lm

interfaz.o: interfaz.c interfaz.h hash.o avltree.o
	gcc -c -Wall -Wextra -Werror -std=c99 interfaz.c

hash.o: hash.c hash.h
	gcc -c -Wall -Wextra -Werror -std=c99 hash.c

avltree.o: avltree.c avltree.h stack.o queue.o dlist.o
	gcc -c -Wall -Wextra -Werror -std=c99 avltree.c

intervalo.o: intervalo.c intervalo.h
	gcc -c -c -Wall -Wextra -Werror -std=c99 intervalo.c

stack.o: lists/stack.c lists/stack.h dlist.o
	gcc -c -c -Wall -Wextra -Werror -std=c99 lists/stack.c

queue.o: lists/queue.c lists/queue.h dlist.o
	gcc -c -c -Wall -Wextra -Werror -std=c99 lists/queue.c

dlist.o: lists/dlist/dlist.c lists/dlist/dlist.h
	gcc -c -c -Wall -Wextra -Werror -std=c99 lists/dlist/dlist.c

clean:
	rm -f *.o main main.exe

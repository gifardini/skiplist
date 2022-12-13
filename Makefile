all : main

run : main
	./main # / para linux \ para windows #

main : main.o skiplist.o
	gcc -o main main.o skiplist.o
main.o : main.c skiplist.h
	gcc -c main.c
skiplist.o : skiplist.c skiplist.h
	gcc -c skiplist.c
clean:
	rm *.o main
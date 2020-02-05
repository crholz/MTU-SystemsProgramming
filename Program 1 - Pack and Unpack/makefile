all: gen
	gcc -o a.out main.o f.o

gen: f.c main.c
	gcc -c f.c
	gcc -c main.c

clean:
	rm *.o a.out
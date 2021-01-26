all: info change



info: Part1.o

info: Part1Main.o

change: Part2.o

change: Part2Main.o



CFLAGS += -g



info:

	gcc -o info Part1.o Part1Main.o



change:

	gcc -o change Part2.o Part2Main.o



Part1.o: Part1.c Part1.h

	gcc -g -c Part1.c



Part1Main.o: Part1Main.c Part1.h

	gcc -g -c Part1Main.c



Part2.o: Part2.c Part2.h Part1.c Part1.c

	gcc -g -c Part2.c



Part2Main.o: Part2Main.c Part2.h

	gcc -g -c Part2Main.c



clean:

	rm -f *.o info c
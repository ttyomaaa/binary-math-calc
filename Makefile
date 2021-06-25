CFLAGS = -W -Wall -Werror -Wextra -Wunused -Wcast-align -Wpointer-arith -Wwrite-strings -Wcast-align 


a.out: main.o stcks.o tokens.o calc.o
	g++ main.o stcks.o tokens.o calc.o -o a.out
main.o: main.cpp calc.h stcks.h tokens.h
	g++ -c $(CFLAGS) main.cpp
stcks.o: stcks.cpp stcks.h
	g++ -c $(CFLAGS) stcks.cpp
tokens.o: tokens.cpp tokens.h
	g++ -c $(CFLAGS) tokens.cpp
calc.o: calc.cpp calc.h
	g++ -c $(CFLAGS) calc.cpp

clean:
	rm -f main.o stcks.o tokens.o calc.o
	rm -f a.out

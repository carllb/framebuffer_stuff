CC = g++
CFLAGS = -g

main: main.o
	$(CC) -o main main.o $(CFLAGS)

main.o: main.cpp
	$(CC) -c -o main.o main.cpp $(CFLAGS)

clean:
	rm -f main *.o

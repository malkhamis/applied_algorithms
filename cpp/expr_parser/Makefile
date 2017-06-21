CC=g++
CFLAGS= --std=c++14
DEPS= node.h ast.h
OBJ = node.o ast.o main.o

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o
CC=gcc -g

all: librxsolver.so empacotamento_rens mochila_rens

OBJS_SOLVER = rxsolver.o utils.o simplex.o

clean:
	rm -f *.o empacotamento_rens mochila_rens librxsolver.so

empacotamento_rens: empacotamento_rens.o
	$(CC) empacotamento_rens.o -o empacotamento_rens -L. -lrxsolver

mochila_rens: mochila_rens.o
	$(CC) mochila_rens.o -o mochila_rens -L. -lrxsolver

librxsolver.so: $(OBJS_SOLVER) utils.o
	$(CC) -shared $(OBJS_SOLVER) -o librxsolver.so

.c.o: .c
	$(CC) -c -fPIC $<

run: all
	./mochila_rens -d 2

CFLAGS=-g -Wall -Werror -m64
CFLAGS=-g -w -m64
ILOG=/opt/ibm/ILOG/CPLEX_Studio129/
ILOG=/cefet/opt/ibm/ILOG/CPLEX_Studio129/
CC=g++

CPLEX_INC=-DIL_STD -I$(ILOG)/cplex/include -I$(ILOG)/concert/include
CPLEX_LIB=-L$(ILOG)/cplex/lib/x86-64_linux/static_pic -lilocplex -lcplex -L$(ILOG)/concert/lib/x86-64_linux/static_pic -lconcert -lm -lpthread

all: mochila2_cplex

clean:
	rm -f *.o mochila_cplex2

mochila2_cplex: mochila2_cplex.o
	$(CC) mochila2_cplex.o -o mochila2_cplex $(CPLEX_LIB) -ldl

mochila2_cplex.o: mochila2_cplex.cpp
	$(CC) $(CFLAGS) $(CPLEX_INC) -c mochila2_cplex.cpp

run:
	./mochila2_cplex mochila_5_10.txt

CFLAGS=-g -Wall -Werror -m64
ILOG=/cefet/opt/ibm/ILOG/CPLEX_Studio129/
CC=g++

CPLEX_INC=-DIL_STD -I$(ILOG)/cplex/include -I$(ILOG)/concert/include
CPLEX_LIB=-L$(ILOG)/cplex/lib/x86-64_linux/static_pic -lilocplex -lcplex -L$(ILOG)/concert/lib/x86-64_linux/static_pic -lconcert -lm -lpthread

all: empacotamento_cplex

clean:
	rm -f *.o empacotamento_cplex

empacotamento_cplex: empacotamento_cplex.o
	$(CC) empacotamento_cplex.o -o empacotamento_cplex $(CPLEX_LIB) -ldl

empacotamento_cplex.o: empacotamento_cplex.cpp
	$(CC) $(CFLAGS) $(CPLEX_INC) -c empacotamento_cplex.cpp

run: empacotamento_cplex
	./empacotamento_cplex empacotamento_6.txt

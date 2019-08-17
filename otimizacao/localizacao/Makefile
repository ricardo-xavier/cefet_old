CFLAGS=-g -Wall -Werror -m64
ILOG=/cefet/opt/ibm/ILOG/CPLEX_Studio129/
CC=g++

CPLEX_INC=-DIL_STD -I$(ILOG)/cplex/include -I$(ILOG)/concert/include
CPLEX_LIB=-L$(ILOG)/cplex/lib/x86-64_linux/static_pic -lilocplex -lcplex -L$(ILOG)/concert/lib/x86-64_linux/static_pic -lconcert -lm -lpthread

all: localizacao_cplex

clean:
	rm -f *.o localizacao_cplex

localizacao_cplex: localizacao_cplex.o
	$(CC) localizacao_cplex.o -o localizacao_cplex $(CPLEX_LIB) -ldl

localizacao_cplex.o: localizacao_cplex.cpp
	$(CC) $(CFLAGS) $(CPLEX_INC) -c localizacao_cplex.cpp

run: localizacao_cplex
	./localizacao_cplex localizacao_2_4.txt

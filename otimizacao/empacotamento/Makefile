CFLAGS=-g -Wall -Werror -m64
CFLAGS=-g -w -m64
ILOG=/cefet/opt/ibm/ILOG/CPLEX_Studio129/
ILOG=/opt/ibm/ILOG/CPLEX_Studio129/
CC=g++

CPLEX_INC=-DIL_STD -I$(ILOG)/cplex/include -I$(ILOG)/concert/include
CPLEX_LIB=-L$(ILOG)/cplex/lib/x86-64_linux/static_pic -lilocplex -lcplex -L$(ILOG)/concert/lib/x86-64_linux/static_pic -lconcert -lm -lpthread

all: empacotamento_cplex empacotamento_ffd

clean:
	rm -f *.o empacotamento_cplex empacotamento_ffd

empacotamento_cplex: empacotamento_cplex.o dominancia2.o
	$(CC) empacotamento_cplex.o dominancia2.o -o empacotamento_cplex $(CPLEX_LIB) -ldl

empacotamento_cplex.o: empacotamento_cplex.cpp
	$(CC) $(CFLAGS) $(CPLEX_INC) -c empacotamento_cplex.cpp

empacotamento_ffd: empacotamento_ffd.o dominancia2.o
	$(CC) empacotamento_ffd.o dominancia2.o -o empacotamento_ffd

empacotamento_ffd.o: empacotamento_ffd.cpp
	$(CC) $(CFLAGS) -c empacotamento_ffd.cpp

dominancia2.o: utils/dominancia2.cpp
	$(CC) $(CFLAGS) -c utils/dominancia2.cpp

run: empacotamento_cplex empacotamento_ffd
	./empacotamento_ffd empacotamento_6.txt

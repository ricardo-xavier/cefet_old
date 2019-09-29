CC = gcc -g -Wall -Werror

OBJS = logbins.o list2.o instance.o utils.o

all: logbins

clean:
	rm -f logbins $(OBJS)

logbins: $(OBJS)
	$(CC) $(OBJS) -o logbins

.c.o: .c
	$(CC) -c $<

run:
	./logbins carregamento7.txt

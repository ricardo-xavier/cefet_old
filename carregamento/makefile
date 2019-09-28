CC = gcc -g

OBJS = logbins.o list2.o instance.o

all: logbins

clean:
	rm -f logbins $(OBJS)

logbins: $(OBJS)
	$(CC) $(OBJS) -o logbins

.c.o: .c
	$(CC) -c $<

run:
	./logbins

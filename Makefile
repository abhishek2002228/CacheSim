CC = gcc
CFLAGS = -g -Wall -Werror -std=c99 -m64

csim: main.c helper_func.c helper_func.h structs.h cache_op.c cache_op.h
	$(CC) $(CFLAGS) -o csim main.c helper_func.c cache_op.c -lm
clean:
	rm -rf *.o
	rm -f csim

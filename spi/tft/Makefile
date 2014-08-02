CC = gcc
CFLAGS = -Wall -g
lib = ../../lib
bin_name = tft_run.exe

default: pic.o main.c ../../lib/bcm2835.c lcd_lib.c
	$(CC) $(CFLAGS) $^ -I $(lib) -L $(lib) -o $(bin_name)

clean:
	$(RM) $(bin_name) *.o
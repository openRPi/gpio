CC = gcc
CFLAGS = -Wall -g
lib = ../../lib
bin_name = tft_run.exe

default: main.c ../../lib/bcm2835.c lib/lcd_lib.c pic.c
	$(CC) $(CFLAGS) $^ -I $(lib) -L $(lib) -o $(bin_name)

clean:
	$(RM) $(bin_name) *.o
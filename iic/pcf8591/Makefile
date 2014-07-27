CC = gcc
CFLAGS = -Wall -g
lib = ../../lib
bin_name = pcf8591_run.exe
RM = rm

default: pcf8591.c main.c ../../lib/bcm2835.c
	$(CC) $(CFLAGS) $^ -I$(lib) -L $(lib) -o $(bin_name)

clean:
	$(RM) $(bin_name) *.o

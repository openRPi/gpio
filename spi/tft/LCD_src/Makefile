CC = arm-bcm2708-linux-gnueabi-gcc
CFLAGS = -Wall -g
bin_name = LCD.exe

default: bcm2835.o main.o lcd.o gui.o
	$(CC) $(CFLAGS) $^ -o $(bin_name)

main.o: main.c lcd.h
	$(CC) -c $<

bcm2835.o: bcm2835.c bcm2835.h
	$(CC) -c $<
	
lcd.o: lcd.c lcd.h bcm2835.h
	$(CC) -c $<
	
gui.o: gui.c gui.h lcd.h 
	$(CC) -c $<

clean:
	$(RM) $(bin_name) *.o

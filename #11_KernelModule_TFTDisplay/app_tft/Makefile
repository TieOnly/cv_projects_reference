INC0 = -I ./ -I ./tft -I ./tft/fonts -I ./utils -I ./utils/pool 
INC1 = -I ./tetris -I ./tcp

SUBSRC = ./tft/*.c ./utils/*.c ./utils/pool/*.c ./tetris/*.c ./tcp/*.c

LIB = -lm
TARSRC = app.c
TARGET = app
CFLAG = -Wall -Wextra -std=gnu99
CC = gcc

all: builddemo

builddemo:
	$(CC) $(TARSRC) $(SUBSRC) -o $(TARGET) $(CFLAG) $(LIB) $(INC0) $(INC1)

rundemo: $(TARGET)
	./$(TARGET)
clean:
	rm --recursive --force $(TARGET)

sub:
	$(CC) tft.c $(SUBSRC) -o a $(CFLAG) $(LIB) $(INC0) $(INC1)

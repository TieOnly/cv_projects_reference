OBJ := ./obj/

build:
	sdcc uart.c -o ${OBJ}
	packihx ${OBJ}uart.ihx > ${OBJ}uart.hex

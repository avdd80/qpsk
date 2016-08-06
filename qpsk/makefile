C = qpsk.c lut.c common.c

C1D = $(C) main.c

H1D = qpsk.h lut.h common.h main.h

F = -lrt -lm -ldl

all:	qpsk.bin

test.bin:	 $(C1D) $(H1D)
	gcc -o qpsk.bin $(F) $(C1D)

clean:
	rm -f *.bin

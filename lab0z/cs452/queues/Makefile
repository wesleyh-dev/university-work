BIN=/home/ubuntu16/emacsimdir/bin

UTIL=/home/cs580000/share/hoca/util

GCC=$(BIN)/m68k-elf-gcc
GCC_FLAGS=-m68000 -I$(UTIL)/inc/ -I$(UTIL)/inc/m68k-linux-gnu -g -gstabs -c

LD=$(BIN)/m68k-elf-ld
LD_FLAGS=-L$(UTIL)/lib -L$(BIN)/../lib/gcc/m68k-elf/4.8.2/m68000 -Ttext=0x1600

LIBS=-lutil -lc -lsim -lm -lgcc

CRT0=$(UTIL)/crtbegin.o
CRT1=$(UTIL)/crtend.o

all: p1test

all2: p1test2

clean:
	rm -f p1test p1test.o asl.o procq.o

p1test2.o: p1test2.c ../h/types.h ../h/const.h ../h/procq.e ../h/asl.e
	$(GCC) $(GCC_FLAGS) -o p1test2.o p1test2.c

p1test.o: p1test.c ../h/types.h ../h/const.h ../h/procq.e ../h/asl.e
	$(GCC) $(GCC_FLAGS) -o p1test.o p1test.c


asl.o: asl.c ../h/types.h ../h/const.h ../h/asl.h ../h/procq.e ../h/asl.e
	$(GCC) $(GCC_FLAGS) -o asl.o asl.c


procq.o: procq.c ../h/types.h ../h/const.h ../h/procq.e ../h/procq.h ../h/asl.e
	$(GCC) $(GCC_FLAGS) -o procq.o procq.c

p1test2: p1test2.o asl.o procq.o
	$(LD) $(LD_FLAGS) -o p1test2 $(CRT0) p1test2.o asl.o procq.o $(CRT1) $(LIBS)

p1test: p1test.o asl.o procq.o
	$(LD) $(LD_FLAGS) -o p1test $(CRT0) p1test.o asl.o procq.o $(CRT1) $(LIBS)

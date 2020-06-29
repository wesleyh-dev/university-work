BIN=/home/ubuntu16/emacsimdir/bin

UTIL=/home/cs580000/share/hoca/util

GCC=$(BIN)/m68k-elf-gcc
GCC_FLAGS=-m68000 -I$(UTIL)/inc/ -I$(UTIL)/inc/m68k-linux-gnu -g -gstabs -c

LD=$(BIN)/m68k-elf-ld
LD_FLAGS=-L$(UTIL)/lib -L$(BIN)/../lib/gcc/m68k-elf/4.8.2/m68000 -Ttext=0x1600

LIBS=-lutil -lc -lsim -lm -lgcc

CRT0=$(UTIL)/crtbegin.o
CRT1=$(UTIL)/crtend.o


INTDIR = ../interrupts
QUEUEDIR = ../../queues

all: nucleus


clean:
	rm -f nucleus main.o syscall.o trap.o int.o p1.o


nucleus: main.o  syscall.o trap.o p1.o int.o $(QUEUEDIR)/procq.o $(QUEUEDIR)/asl.o
	$(LD) $(LD_FLAGS) -o nucleus $(CRT0) main.o syscall.o trap.o int.o $(QUEUEDIR)/procq.o $(QUEUEDIR)/asl.o p1.o $(CRT1) $(LIBS)


main.o: main.c ../../h/const.h ../../h/types.h ../../h/procq.e ../../h/asl.e
	$(GCC) $(GCC_FLAGS) -o main.o main.c


syscall.o: syscall.c ../../h/const.h ../../h/types.h ../../h/procq.e ../../h/asl.e
	$(GCC) $(GCC_FLAGS) -o syscall.o syscall.c


trap.o: trap.c ../../h/const.h ../../h/types.h ../../h/procq.e ../../h/asl.e ../../h/util.h
	$(GCC) $(GCC_FLAGS) -o trap.o trap.c


int.o: $(INTDIR)/int.c ../../h/const.h ../../h/types.h ../../h/procq.e ../../h/asl.e
	$(GCC) $(GCC_FLAGS) -o int.o $(INTDIR)/int.c


p1.o: p1.c ../../h/const.h ../../h/types.h
	$(GCC) $(GCC_FLAGS) -o p1.o p1.c


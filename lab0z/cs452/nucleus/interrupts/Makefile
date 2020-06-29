BIN=/home/ubuntu16/emacsimdir/bin

UTIL=/home/cs580000/share/hoca/util

GCC=$(BIN)/m68k-elf-gcc
GCC_FLAGS=-m68000 -I$(UTIL)/inc/ -I$(UTIL)/inc/m68k-linux-gnu -g -gstabs -c

LD=$(BIN)/m68k-elf-ld
LD_FLAGS=-L$(UTIL)/lib -L$(BIN)/../lib/gcc/m68k-elf/4.8.2/m68000 -Ttext=0x1600

LIBS=-lutil -lc -lsim -lm -lgcc

CRT0=$(UTIL)/crtbegin.o
CRT1=$(UTIL)/crtend.o

TRAP = ../traps
QUEUE = ../../queues

all: nucleus


clean:
	rm -f nucleus int.o intp1.o


int.o: int.c ../../h/const.h ../../h/types.h ../../h/procq.e ../../h/asl.e ../../h/vpop.h
	$(GCC) $(GCC_FLAGS) -o int.o int.c


intp1.o: intp1.c ../../h/const.h ../../h/types.h ../../h/vpop.h
	$(GCC) $(GCC_FLAGS) -o intp1.o intp1.c


nucleus: $(TRAP)/main.o $(TRAP)/trap.o $(TRAP)/syscall.o int.o $(QUEUE)/procq.o $(QUEUE)/asl.o intp1.o
	$(LD) $(LD_FLAGS) -o nucleus $(CRT0) intp1.o $(TRAP)/main.o $(TRAP)/trap.o $(TRAP)/syscall.o int.o $(QUEUE)/procq.o $(QUEUE)/asl.o $(CRT1) $(LIBS)

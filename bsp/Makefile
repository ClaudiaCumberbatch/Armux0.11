include ../Makefile.header

LDFLAGS	+= -r 

CFLAGS	+= -I../include

CPP	+= -I../include

.c.s:
	@$(CC) $(CFLAGS) \
	-S -o $*.s $<
.s.o:
	@$(AS) -o $*.o $<
.c.o:
	@$(CC) $(CFLAGS) \
	-c -o $*.o $<

OBJS = uart.o bsp_int.o

bsp.o: $(OBJS)
	@$(LD) -r -o bsp.o $(OBJS)
	@sync

clean:
	@rm -f *.o 

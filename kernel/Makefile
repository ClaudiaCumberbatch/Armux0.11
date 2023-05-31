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

OBJS  = vsprintf.o
# sched.o system_call.o traps.o asm.o fork.o panic.o printk.o vsprintf.o sys.o exit.o signal.o mktime.o who.o

kernel.o: $(OBJS)
	@$(LD) $(LDFLAGS) -o kernel.o $(OBJS)
	@sync

clean:
	@rm -f *.o *.a tmp_make
	@for i in *.c;do rm -f `basename $$i .c`.s;done
# @for i in chr_drv blk_drv math; do make clean -C $$i; done

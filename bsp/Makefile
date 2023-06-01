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

clean:
	@rm -f *.o 

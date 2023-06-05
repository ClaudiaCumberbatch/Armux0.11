include ../Makefile.header

bootsect.o: bootsect.S
	@$(AS) -o bootsect.o bootsect.S
	@sync

clean:
	@rm -f *.o
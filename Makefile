include Makefile.header

LDFLAGS	+= -Ttext 0
CFLAGS	+= -Iinclude
CPP	+= -Iinclude

.c.s:
	@$(CC) $(CFLAGS) -S -o $*.s $<
.s.o:
	@$(AS)  -o $*.o $<
.c.o:
	@$(CC) $(CFLAGS) -c -o $*.o $<

Image: tools/system
	@cp -f tools/system system.tmp
	@$(STRIP) system.tmp
	@$(OBJCOPY) -O binary -R .note -R .comment system.tmp tools/kernel
	@tools/build.sh tools/kernel Image

tools/system: boot/bootsect.o init/main.o
	@$(LD) $(LDFLAGS) boot/bootsect.o init/main.o \
	-o tools/system 

kernel/kernel.o:
	@make kernel.o -C kernel

bsp/bsp.o:
	@make bsp.o -C bsp

boot/bootsect.o: boot/bootsect.S
	@make bootsect.o -C boot

OBJS = boot/bootsect.o init/main.o bsp/bsp.o

bootsect: $(OBJS)
	@$(LD) $(LDFLAGS) -o bootsect $(OBJS)
	@$(OBJCOPY) -R .pdr -R .comment -R.note -S -O binary bootsect

start:
	@qemu-system-arm -M virt -m 16M \
	-device loader,file=bootsect,addr=0x00000000 \
	-nographic

debug:
	@qemu-system-arm -M virt -m 16M \
	-device loader,file=bootsect,addr=0x00000000 \
	-S -s \
# -nographic

test:
	@qemu-system-arm -machine xilinx-zynq-a9 -m 16M \
	-device loader,file=bootsect,addr=0x00000000 \
	-nographic

t_debug:
	@qemu-system-arm -machine xilinx-zynq-a9 -m 16M \
	-device loader,file=bootsect,addr=0x00000000 \
	-S -s \
	-nographic

clean:
	@rm -f Image bootsect 
	@rm -f init/*.o
	@(cd boot;make clean)
	@(cd kernel;make clean)
	@(cd bsp;make clean)
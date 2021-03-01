# vim: noexpandtab tabstop=4 shiftwidth=4
CXX     = i386-elf-gcc
CFLAGS  = -std=gnu99 -ffreestanding -O2 -Wall -Wextra -nostdlib -Iinclude
ASM     = nasm

OBJECTS = boot.o gdt.o isr.o idt.o port.o print.o kernel.o

%.o : src/%.c
	$(CXX) $(CFLAGS) -o $@ -c $<

%.o : src/%.s
	$(ASM) -felf32 $< -o $@

os.bin : $(OBJECTS)
	$(CXX) -T linker.ld -o os.bin $(CFLAGS) $(OBJECTS) -lgcc

os.iso : os.bin
	mkdir -p isodir/boot/grub
	mv os.bin isodir/boot/os.bin
	echo "menuentry 'os' {" >> grub.cfg
	echo "    multiboot /boot/os.bin" >> grub.cfg
	echo "}" >> grub.cfg
	mv grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o os.iso isodir

clean :
	rm -rf *.o *.iso *.bin *.cfg isodir/

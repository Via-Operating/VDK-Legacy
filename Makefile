# GNU/Linux Makefile

all:
	mkdir -p build
	nasm -f elf32 libsa/boot.asm -o build/boot.o
	nasm -f elf32 libkern/load_gdt.asm -o build/legit.o
	nasm -f elf32 libkern/load_idt.asm -o build/geg.o
	nasm -f elf32 libkern/exception.asm -o build/ex.o
	nasm -f elf32 libkern/irq.asm -o build/req.o
	gcc -m32 -std=gnu99 -ffreestanding -Wall -Wextra -c libsa/main.c -o build/main.o
	gcc -m32 -std=gnu99 -ffreestanding -Wall -Wextra -c libsa/stdio.c -o build/stdio.o
	gcc -m32 -std=gnu99 -ffreestanding -Wall -Wextra -c libkern/ioport.c -o build/ip.o
	gcc -m32 -std=gnu99 -ffreestanding -Wall -Wextra -c libkern/gdt.c -o build/gdt.o
	gcc -m32 -std=gnu99 -ffreestanding -Wall -Wextra -c libkern/idt.c -o build/idt.o
	gcc -m32 -std=gnu99 -ffreestanding -Wall -Wextra -c libkern/8259_pic.c -o build/pic.o
	gcc -m32 -std=gnu99 -ffreestanding -Wall -Wextra -c libkern/isr.c -o build/isr.o
	ld -m elf_i386 -T linker.ld -nostdlib -o build/krn.bin build/boot.o build/stdio.o build/ip.o build/legit.o build/gdt.o build/geg.o build/ex.o build/req.o build/isr.o build/pic.o build/idt.o build/main.o

clean:
	rm build/main.o build/boot.o build/krn.bin build/stdio.o build/ip.o build/gdt.o build/legit.o
	rm -rf build

iso:
	grub-file --is-x86-multiboot build/krn.bin
	mkdir -p build/isodir/boot/grub/
	cp build/krn.bin build/isodir/boot/
	cp grub.cfg build/isodir/boot/grub/
	grub-mkrescue -o build/via.iso build/isodir/
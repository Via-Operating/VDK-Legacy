# GNU/Linux Makefile

all:
	mkdir -p build
	nasm -f elf32 libsa/boot.asm -o build/boot.o
	gcc -m32 -std=gnu99 -ffreestanding -Wall -Wextra -c libsa/main.c -o build/main.o
	gcc -m32 -std=gnu99 -ffreestanding -Wall -Wextra -c libsa/stdio.c -o build/stdio.o
	ld -m elf_i386 -T linker.ld -nostdlib -o build/krn.bin build/boot.o build/stdio.o build/main.o

clean:
	rm build/main.o build/boot.o build/krn.bin build/stdio.o
	rm -rf build

iso:
	grub-file --is-x86-multiboot build/krn.bin
	mkdir -p build/isodir/boot/grub/
	cp build/krn.bin build/isodir/boot/
	cp grub.cfg build/isodir/boot/grub/
	grub-mkrescue -o build/via.iso build/isodir/
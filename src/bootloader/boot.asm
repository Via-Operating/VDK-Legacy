;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; VIA_LOADER                             ;
;                                        ;
; Written by ViaOS Core Team Member(s):  ;
; Kap Petrov                             ;
;                                        ;
; ViaOS Bootloader                       ;
;                                        ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


org 0x7C00 ; BOOTLOADER Start Address
bits 16 ; 16-BIT Real Mode

%define ENDL 0x0D, 0x0A

;
; FAT12 header
;
jmp short start
nop

bdb_oem:						db 'MSWIN4.1'		; 8 bytes
bdb_bytes_per_sector			dw 512				
bdb_sectors_per_cluster			db 1
bdb_reserved_sectors:			dw 1
bdb_fat_count:					db 2				; 2 fat people
bdb_dir_entries_count:			dw 0E0h
bdb_total_sectors:				dw 2880
bdb_media_descriptor_type:		db 0F0h
bdb_sectors_per_fat:			dw 9
bdb_sectors_per_track:			dw 18
bdb_heads:						dw 2
bdb_hidden_sectors:				dd 0
bdb_large_sector_count:			dd 0				; not as large as bdb_media_descriptor_type lol

; extended boot record (EBR)
ebr_drive_number:				db 0				; 0x00 floppy, 0x80 = hdd, useless
								db 0				; reserved
ebr_signature:					db 29h
ebr_volume_id:					db 12h, 34h, 56h, 78h
ebr_volume_label:				db 'VIA OPERATI'	; 11 bytes, padded with spaces! Via is truly operati(ng).
ebr_system_id:					db 'FAT12   '		; 8 bytes

;
; Code goes here
;

start:

	; setup data segments
	mov ax, 0 ; can't write to ds/es directly
	mov ds, ax
	mov es, ax

	; setup stack
	mov ss, ax
	mov sp, 0x7C00 ; stack

	push es
	push word .after
	retf
.after:

	; read something from floppy
	; BIOS should set DL to drive number

	mov [ebr_drive_number], dl

	; print message
	mov si, msg_loading
	call puts

	; read drive parameters
	push es
	mov ah, 08h
	int 13h
	jc floppy_error
	pop es

	and cl, 0x3F
	xor ch, ch
	mov [bdb_sectors_per_track], cx

	inc dh
	mov [bdb_heads], dh

	mov ax, [bdb_sectors_per_fat]
	mov bl, [bdb_fat_count]
	xor bh, bh
	mul bx
	add ax, [bdb_reserved_sectors]
	push ax

	mov ax, [bdb_sectors_per_fat]
	shl ax, 5
	xor dx, dx
	div word [bdb_bytes_per_sector]

	test dx, dx
	jz .root_dir_after
	inc ax

.root_dir_after:

	; read root directory
	mov cl, al
	pop ax
	mov dl, [ebr_drive_number]
	mov bx, buffer
	call disk_read

	; search for kernel.bin
	xor bx, bx
	mov di, buffer

.search_kernel:
	mov si, file_kernel_bin
	mov cx, 11
	push di
	repe cmpsb
	pop di
	je .found_kernel

	add di, 32
	inc bx
	cmp bx, [bdb_dir_entries_count]
	jl .search_kernel

	; kernel not found
	jmp kernel_not_found_error

.found_kernel:
	
	mov ax, [di + 26]
	mov [kernel_cluster], ax

	mov ax, [bdb_reserved_sectors]
	mov bx, buffer
	mov cl, [bdb_sectors_per_fat]
	mov dl, [ebr_drive_number]
	call disk_read

	mov bx, KERNEL_LOAD_SEGMENT
	mov es, bx
	mov bx, KERNEL_LOAD_OFFSET

.load_kernel_loop:
	mov ax, [kernel_cluster]
	add ax, 31

	mov cl, 1
	mov dl, [ebr_drive_number]
	call disk_read

	add bx, [bdb_bytes_per_sector]

	mov ax, [kernel_cluster]
	mov cx, 3
	mul cx
	mov cx, 2
	div cx

	mov si, buffer
	add si, ax
	mov ax, [ds:si]

	or dx, dx
	jz .even
.odd:
	shr ax, 4
	jmp .next_cluster_after

.even:
	and ax, 0x0FFF

.next_cluster_after:
	cmp ax, 0x0FF8
	jae .read_finish

	mov [kernel_cluster], ax
	jmp .load_kernel_loop

.read_finish:
	mov dl, [ebr_drive_number]

	mov ax, KERNEL_LOAD_SEGMENT
	mov ds, ax
	mov es, ax

	jmp KERNEL_LOAD_SEGMENT:KERNEL_LOAD_OFFSET

	jmp wait_key_and_reboot ; SHOULD NEVER EVER HAPPEN!!!!!!!!

	cli
    hlt

;
; Error handlers
;
floppy_error:
	mov si, msg_read_error
	call puts
	jmp wait_key_and_reboot

wait_key_and_reboot:
	mov ah, 0
	int 16h

	jmp 0FFFFh:0 ; Beginning of BIOS

kernel_not_found_error:
	mov si, msg_kern_not_fnd
	call puts
	jmp wait_key_and_reboot

.halt:
	cli
    hlt

;
; Disk Routines
;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; LBA_TO_CHS                               ;
;                                          ;
; Written by ViaOS Core Team Member(s):    ;
; Kap Petrov                               ;
;                                          ;
; converts an LBA address to a CHS address ;
;                                          ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
lba_to_chs:
	
	push ax
	push dx

	xor dx, dx
	div word [bdb_sectors_per_track]

	inc dx
	mov cx, dx

	xor dx, dx
	div word [bdb_heads]

	mov dh, dl
	mov ch, al
	shl ah, 6
	or cl, ah

	pop ax
	mov dl, al
	pop ax
	ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; DISK_READ                                ;
;                                          ;
; Written by ViaOS Core Team Member(s):    ;
; Kap Petrov                               ;
;                                          ;
; Reads sectors from a disk                ;
;                                          ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
disk_read:

	push ax
	push bx
	push cx
	push dx
	push di

	push cx
	call lba_to_chs
	pop ax
	mov ah, 02h
	mov di, 3

.retry:
	pusha
	stc
	int 13h
	jnc .done

	; read failed
	popa
	call disk_reset

	dec di
	test di, di
	jnz .retry

.fail:
	; all attempts exhausted
	jmp floppy_error
.done:
	popa

	pop di
	pop dx
	pop cx
	pop bx
	pop ax
	ret

disk_reset:
	pusha
	mov ah, 0
	stc
	int 13h
	jc floppy_error
	popa
	ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; PUTS                                   ;
;                                        ;
; Written by ViaOS Core Team Member(s):  ;
; Kap Petrov                             ;
;                                        ;
; ViaOS Bootloader Print String function ;
;                                        ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
puts:
	push si
	push ax

.loop:
	lodsb
	or al, al
	jz .done

	mov ah, 0x0e
	mov bh, 0
	int 0x10

	jmp .loop

.done:
	pop ax
	pop si
	ret


msg_loading: db 'Loading...', ENDL, 0
msg_read_error: db 'Read from disk failed!', ENDL, 0
msg_kern_not_fnd: db 'KERNEL.BIN file not found!', ENDL, 0
file_kernel_bin: db 'KERNEL  BIN'
kernel_cluster: dw 0

KERNEL_LOAD_SEGMENT equ 0x2000
KERNEL_LOAD_OFFSET equ 0

times 510-($-$$) db 0
dw 0AA55h

buffer: 
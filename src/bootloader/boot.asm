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
	jmp main

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

; Bootloader
main:

	; setup data segments
	mov ax, 0 ; can't write to ds/es directly
	mov ds, ax
	mov es, ax

	; setup stack
	mov ss, ax
	mov sp, 0x7C00 ; stack

	; read something from floppy
	; BIOS should set DL to drive number

	mov [ebr_drive_number], dl

	mov ax, 1 ; LBA = 1, second sector
	mov cl, 1 ; 1 sector to read
	mov bx, 0x7E00 ; data should be after bootloader
	call disk_read

	; print message
	mov si, msg_hello
	call puts
	
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

msg_hello: db 'VDK Bootloader 2.1', ENDL, 0
msg_read_error: db 'Sorry, The maximum number of attempts has been reached and we are unable to read the disk. Press any key to reboot.', ENDL, 0

times 510-($-$$) db 0
dw 0AA55h ; H.

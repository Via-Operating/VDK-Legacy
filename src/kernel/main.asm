;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; VIA_LOADER                             ;
;                                        ;
; Written by ViaOS Core Team Member(s):  ;
; Kap Petrov                             ;
;                                        ;
; ViaOS Kernel                       	 ;
;                                        ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


org 0x7C00 ; BOOTLOADER Start Address
bits 16 ; 16-BIT Real Mode

%define ENDL 0x0D, 0x0A

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
	push bx

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
	mov sp, 0x7C00 ; stack my balls

	; print message
	mov si, msg_hello
	call puts
    hlt

; CPU, STOP THE FUCK EXECUTING CODE!!!!!!!!!!!!!!
.halt:
    jmp .halt

msg_hello: db 'VDK Kernel!', ENDL, 0

times 510-($-$$) db 0
dw 0AA55h

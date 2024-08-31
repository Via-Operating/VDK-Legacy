;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Via Dynamic Kernel                     ;
;                                        ;
; Written by ViaOS Core Team Member(s):  ;
; Kap Petrov                             ;
;                                        ;
; ViaOS Kernel                       	 ;
;                                        ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


org 0x0 ; KERNEL Start Address
bits 16 ; 16-BIT Real Mode

%define ENDL 0x0D, 0x0A

start:
	; print message
	mov si, msg_hello
	call puts

.halt:
	cli
    hlt

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; PUTS                                   ;
;                                        ;
; Written by ViaOS Core Team Member(s):  ;
; Kap Petrov                             ;
;                                        ;
; ViaOS Kernel Print String function     ;
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
	pop bx
	pop ax
	pop si
	ret

msg_hello: db '[OK] Hello from Via Dynamic Kernel!', ENDL, 0
;
; kbddos.s
; Julian Squires <tek@wiw.org> / 2001
; $Id$
;
; DOS pmode keyboard functions
;
; Note: COFF name mangling in effect in this file.
;

	section .text

;bool _d_event_kbdinit(void);
;void _d_event_kbdclose(void);
;bool _d_event_kbdishit(byte);

	global __d_event_kbdinit
__d_event_kbdinit:
	push ebp
	mov ebp, esp
	push edi
	push esi
	push ebx

	; get protected mode kbd interrupt vector
	mov ax, 0x0204
	mov bl, 9
	int 0x31
	mov [kbdoldintsel], cx
	mov [kbdoldintoffs], edx

	; lock code
	mov ax, 0x0600
	mov ebx, kbdint
	mov ecx, kbdint
	shr ebx, 16
	mov si, 0
	mov di, kbdcodelocklen
	int 0x31
	jcs .fail

	; lock data
	mov ax, 0x0600
	mov ebx, kbdmap
	mov ecx, kbdmap
	shr ebx, 16
	mov si, 0
	mov di, kbddatalocklen
	int 0x31
	jcs .fail

	; set pmode kbd interrupt
	mov ax, 0x0205
	mov bl, 9
	mov cx, cs
	mov edx, kbdint
	int 0x31
	jcs .fail

	mov al, 1
	jmp .end
.fail:	mov al, 0
.end:	pop ebx
	pop esi
	pop edi
	pop ebp
	ret

	global __d_event_kbdclose
__d_event_kbdclose:
	push ebp
	mov ebp, esp

	; restore the interrupt handler
	mov ax, 0x0205
	mov bl, 9
	mov cx, [kbdoldintsel]
	mov edx, [kbdoldintoffs]
	int 0x31

	; FIXME: we should really unlock the memory used by the
	; interrupt handler here...

	pop ebp
	ret

	global __d_event_kbdishit
__d_event_kbdishit:
	push ebp
	mov ebp, esp

	mov al, [ebp+8]
	js .false		; we don't track values >127 atm

	mov al, [kbdmap+al]
	jmp .end
.false: mov al, 0
.end:	pop ebp
	ret

; the actual interrupt routine
kbdint:
	cli
	in al, 0x60
	cmp al, 0xE0		; extended, which we don't handle yet
	jmp .end
	mov ah, al
	and al, 0x7F
	and ah, 0x80
	jnz .release
.hit:	mov [kbdmap+al], 1
	jmp .end
.release:
	mov [kbdmap+al], 0
.end:	out 0x20, 0x20
	sti
	iret
kbdintend:

	section .data

nscancodes equ 128
kbdcodelocklen equ kbdintend-kbdint
kbddatalocklen equ nscancodes

kbdmap: resb nscancodes
kbdoldintsel: resw
kbdoldintoffs: resd

; EOF kbddos.s

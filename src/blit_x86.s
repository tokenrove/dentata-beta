;
; blit_x86.s ($Id$)
; Julian Squires <tek@wiw.org> / 2001
; 
; x86 optimized blit routines.
; Note that these routines are currently optimized for
; 486-era processors, and don't take pipelining into account.
;


	global _blit80, _blit81, _blit160, _blit161, _blit162, _blit164
	global _blit168, _blit240, _blit241, _blit242, _blit244, _blit248

; eight bit
_blit80:
	push ebp
	mov ebp, esp
	push esi
	push edi
	push ebx

	mov edi, [ebp+8]
	mov ebx, [ebp+36]
	add ebx, edi
	add edi, [ebp+24]
	mov esi, [ebp+16]
	add esi, [ebp+40]

.scan:	mov ecx, [ebp+32]
	rep movsb
	add edi, [ebp+28]
	add esi, [ebp+44]
	cmp edi, ebx
	jl .scan

	pop ebx
	pop edi
	pop esi
	pop ebp
	ret

_blit81:
	ret

;
; sixteen bit
;

; void _blit160(byte *ddat, byte *dalp, byte *sdat, byte *salp, dword doffs,
;               dword dscanoff, dword scanlen, dword endoffs, dword soffs,
;               dword sscanoff)
_blit160:
	push ebp
	mov ebp, esp
	push esi
	push edi
	push eax
	push ebx
	push ecx
	cld

	mov edi, [ebp+8]
	mov esi, [ebp+16]

	mov eax, [ebp+28]
	add [ebp+28], eax
	mov eax, [ebp+44]
	add [ebp+44], eax

	mov ebx, [ebp+36]
	add ebx, ebx
	add ebx, edi

	mov eax, [ebp+24]
	add eax, eax
	add edi, eax

	mov eax, [ebp+40]
	add eax, eax
	add esi, eax

.loop:
	mov ecx, [ebp+32]
	rep movsw
	add edi, [ebp+28]
	add esi, [ebp+44]
	cmp edi, ebx
	jl .loop

	pop ecx
	pop ebx
	pop eax
	pop edi
	pop esi
	pop ebp
	ret

_blit161:
	push ebp
	mov ebp, esp
	push esi
	push edi
	push eax
	push ebx
	push ecx
	push edx

	cld
	mov eax, [ebp+28]
	add [ebp+28], eax
	mov eax, [ebp+44]
	add [ebp+44], eax

	mov edi, [ebp+8]
	mov eax, [ebp+36]
	add eax, eax
	add eax, edi
	mov [ebp+36], eax

	mov eax, [ebp+24]
	add eax, eax
	add edi, eax

	mov esi, [ebp+16]
	mov edx, [ebp+20]
	mov eax, [ebp+40]
	add eax, eax
	add esi, eax

	shr eax, 1
	and eax, 7
	mov ebx, eax
	mov eax, [ebp+40]
	shr eax, 3
	add edx, eax
	mov bh, [edx]

.loop:
	mov ecx, [ebp+32]
.scanlinecopy:
	mov eax, ebx
	movzx ebx, al
	or bl, 8
	bt eax, ebx
	jnc .l1

	movsw
	jmp .l2

.l1:	add edi, 2
	add esi, 2

.l2:	mov bh, ah
	and bl, 7
	inc bl
	cmp bl, 8
	jl .l3

	inc edx
	mov bh, [edx]
	mov bl, 0
.l3:	loop .scanlinecopy

	; add the scan offsets
	add edi, [ebp+28]
	mov eax, [ebp+44]
	add esi, eax
	shr eax, 1
	and eax, 7
	and ebx, 7
	add ebx, eax
	mov eax, [ebp+44]
	shr eax, 4
	add edx, eax
	cmp bl, 8
	jl .l4

	and bl, 7
	inc edx
.l4:
	mov bh, [edx]
	; are we at the end?
	cmp edi, [ebp+36]
	jl .loop

	pop edx
	pop ecx
	pop ebx
	pop eax
	pop edi
	pop esi
	pop ebp
	ret

_blit162:
	push ebp
	mov ebp, esp
	push esi
	push edi
	push eax
	push ebx
	push ecx
	cld

	mov edi, [ebp+8]
	mov esi, [ebp+16]

	mov eax, [ebp+28]
	add [ebp+28], eax
	mov eax, [ebp+44]
	add [ebp+44], eax

	mov ebx, [ebp+36]
	add ebx, ebx
	add ebx, edi

	mov eax, [ebp+24]
	add eax, eax
	add edi, eax

	mov eax, [ebp+40]
	add eax, eax
	add esi, eax

.loop:
	mov ecx, [ebp+32]
	rep movsw
	add edi, [ebp+28]
	add esi, [ebp+44]
	cmp edi, ebx
	jl .loop

	pop ecx
	pop ebx
	pop eax
	pop edi
	pop esi
	pop ebp
	ret

_blit164:
	push ebp
	mov ebp, esp
	push esi
	push edi
	push eax
	push ebx
	push ecx
	cld

	mov edi, [ebp+8]
	mov esi, [ebp+16]

	mov eax, [ebp+28]
	add [ebp+28], eax
	mov eax, [ebp+44]
	add [ebp+44], eax

	mov ebx, [ebp+36]
	add ebx, ebx
	add ebx, edi

	mov eax, [ebp+24]
	add eax, eax
	add edi, eax

	mov eax, [ebp+40]
	add eax, eax
	add esi, eax

.loop:
	mov ecx, [ebp+32]
	rep movsw
	add edi, [ebp+28]
	add esi, [ebp+44]
	cmp edi, ebx
	jl .loop

	pop ecx
	pop ebx
	pop eax
	pop edi
	pop esi
	pop ebp
	ret

_blit168:
	push ebp
	mov ebp, esp
	push esi
	push edi
	push eax
	push ebx
	push ecx
	cld

	mov edi, [ebp+8]
	mov esi, [ebp+16]

	mov eax, [ebp+28]
	add [ebp+28], eax
	mov eax, [ebp+44]
	add [ebp+44], eax

	mov ebx, [ebp+36]
	add ebx, ebx
	add ebx, edi

	mov eax, [ebp+24]
	add eax, eax
	add edi, eax

	mov eax, [ebp+40]
	add eax, eax
	add esi, eax

.loop:
	mov ecx, [ebp+32]
	rep movsw
	add edi, [ebp+28]
	add esi, [ebp+44]
	cmp edi, ebx
	jl .loop

	pop ecx
	pop ebx
	pop eax
	pop edi
	pop esi
	pop ebp
	ret

; twenty-four bit
_blit240:
	push ebp
	mov ebp, esp
	push esi
	push edi
	push eax
	push ebx
	push ecx
	cld

	mov edi, [ebp+8]
	mov esi, [ebp+16]

	mov eax, [ebp+28]
	add [ebp+28], eax
	add [ebp+28], eax
	mov eax, [ebp+44]
	add [ebp+44], eax
	add [ebp+44], eax

	mov ebx, [ebp+36]
	add ebx, [ebp+36]
	add ebx, [ebp+36]
	add ebx, edi

	mov eax, [ebp+24]
	add eax, [ebp+24]
	add eax, [ebp+24]
	add edi, eax

	mov eax, [ebp+40]
	add eax, [ebp+40]
	add eax, [ebp+40]
	add esi, eax

.loop:
	mov ecx, [ebp+32]
	add ecx, [ebp+32]
	add ecx, [ebp+32]
	rep movsb
	add edi, [ebp+28]
	add esi, [ebp+44]
	cmp edi, ebx
	jl .loop

	pop ecx
	pop ebx
	pop eax
	pop edi
	pop esi
	pop ebp
	ret

_blit241:
	ret
_blit242:
	ret
_blit244:
	ret
_blit248:
	ret

; EOF blit_x86.s
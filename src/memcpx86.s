;
; memcpx86.s
; Julian Squires <tek@wiw.org> / 2001
; $Id$
; Memory module, string operations, x86 assembler version.
;

	global d_memory_copy, d_memory_move, d_memory_set

d_memory_copy:
	push ebp
	mov ebp, esp
	push edi
	push esi

	mov edi, [ebp+8]
	mov esi, [ebp+12]
	mov ecx, [ebp+16]
	mov edx, ecx
	and edx, 3
	shr ecx, 2
	rep movsd
	mov ecx, edx
	rep movsb

	pop esi
	pop edi
	pop ebp
	ret

d_memory_move:
	push ebp
	mov ebp, esp
	push edi
	push esi

	mov edi, [ebp+8]
	mov esi, [ebp+12]
	mov ecx, [ebp+16]
	; if the destination is completely before the source,
	; or if the destination's end overlaps the source's beginning,
	;     copy straight
	lea eax, [edi+ecx]
	cmp eax, esi
	jl .l1
	; if the destination's beginning overlaps the source's end,
	;     copy backwards
	lea edi, [edi+ecx]
	lea esi, [esi+ecx]
	std

.l1:	mov edx, ecx
	and edx, 3
	shr ecx, 2
	rep movsd
	mov ecx, edx
	rep movsb

.end:	cld
	pop esi
	pop edi
	pop ebp
	ret

d_memory_set:
	push ebp
	mov ebp, esp
	push edi

	mov edi, [ebp+8]
	mov al, [ebp+12]
	mov ah, al
	shl eax, 16
	mov al, [ebp+12]
	mov ah, al
	mov ecx, [ebp+16]
	mov edx, ecx
	and edx, 3
	shr ecx, 2
	rep stosd
	mov ecx, edx
	rep stosb

	pop edi
	pop ebp
	ret

; EOF memcpx86.s

;
; blit_x86.s ($Id$)
; Julian Squires <tek@wiw.org> / 2001
; 
; x86 optimized blit routines.
; Note that these routines are currently optimized for
; 486-era processors, and don't take pipelining into account.
;
; NOTE! These routines make the assumption that eax, ecx, and edx do
; not need to be preserved.
;


	global _blit80, _blit81, _blit160, _blit161, _blit162, _blit164
	global _blit168, _blit240, _blit241, _blit242, _blit244, _blit248

; eight bit

;;
;; _blit80
;; Uses rep movsb to copy scanlines quickly.
;; A possible additional optimization is to avoid memory access
;; in the scanline offsets and the loading of ECX.
;; 
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
	rep movsb		; copy a scanline wholesale
	add edi, [ebp+28]
	add esi, [ebp+44]
	cmp edi, ebx		; are we at the end?
	jl .scan

	pop ebx
	pop edi
	pop esi
	pop ebp
	ret

;;
;; _blit81
;; Pretty standard, lodsb and test, then either stosb or inc.
;; 
_blit81:
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
.pel:	lodsb			; load a byte from dest
	jz .l1			; skip the store if it's zero
	stosb
	jmp .l2
.l1:	inc edi
.l2:	loop .pel
	add edi, [ebp+28]
	add esi, [ebp+44]
	cmp edi, ebx		; are we at the end?
	jl .scan

	pop ebx
	pop edi
	pop esi
	pop ebp
	ret

;
; sixteen bit
;

;;
;; _blit160
;; Uses rep movsw to copy quickly. (note that we use little endian packing
;; on 16bpp data, so this is fine).
;;
_blit160:
	push ebp
	mov ebp, esp
	push esi
	push edi
	push ebx

	mov edi, [ebp+8]
	mov esi, [ebp+16]

	mov eax, [ebp+28]
	add [ebp+28], eax
	mov eax, [ebp+44]
	add [ebp+44], eax

	mov ebx, [ebp+36]
	lea ebx, [2*ebx+edi]

	mov eax, [ebp+24]
	lea edi, [edi+2*eax]

	mov eax, [ebp+40]
	lea esi, [esi+2*eax]

.loop:
	mov ecx, [ebp+32]
	rep movsw		; wholescale scanline dump
	add edi, [ebp+28]
	add esi, [ebp+44]
	cmp edi, ebx
	jl .loop

	pop ebx
	pop edi
	pop esi
	pop ebp
	ret

;;
;; _blit161
;; Typical bit-test deal.
;; 
_blit161:
	push ebp
	mov ebp, esp
	push esi
	push edi
	push ebx

	mov eax, [ebp+28]
	add [ebp+28], eax
	mov eax, [ebp+44]
	add [ebp+44], eax

	mov edi, [ebp+8]
	mov eax, [ebp+36]
	lea eax, [2*eax+edi]
	mov [ebp+36], eax

	mov eax, [ebp+24]
	lea edi, [edi+2*eax]

	mov esi, [ebp+16]
	mov edx, [ebp+20]
	mov eax, [ebp+40]
	lea esi, [esi+2*eax]

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
	bt eax, ebx		; the test
	jnc .l1

	;; FIXME: would it be faster to read and write
	;; the pixel directly (without movsw), and let it
	;; fall through instead of jmping?
	movsw			; copy a full pixel
	jmp .l2

.l1:	add edi, 2		; don't copy a pixel
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

	pop ebx
	pop edi
	pop esi
	pop ebp
	ret

;;
;; _blit162
;; Based on the generic implementation, uses the _blit_onethird and
;; _blit_twothird tables defined at the end of this file.
;;
_blit162:
	push ebp
	mov ebp, esp
	sub esp, 32
	push edi
	push esi
	push ebx

	mov edi, [ebp+8]	; dest data
	mov ebx, [ebp+36]	; endoffs
	lea ebx, [2*ebx+edi]	; setting up the end offset
	mov [ebp-16], ebx
	mov eax, [ebp+24]	; doffs
	lea edi, [2*eax+edi]

	; mask table
	mov dword [ebp-4], 0xFF3F0F03

	mov edx, [ebp+40]	; soffs
	mov esi, [ebp+16]	; source data
	lea esi, [esi+2*edx]

	mov al, dl
	and al, 3		; low two bits of source offset
	mov [ebp-9], al		; ebp-9 == finetune

	shr edx, 2		; soffs/4
	add [ebp+20], edx	; source alpha offset

	mov edx, [ebp+28]	; dscan offset
	add edx, edx
	mov [ebp-20], edx	; ebp-20 == dscan*2

	mov ecx, [ebp+44]	; sscanoffset
	lea eax, [2*ecx]
	mov [ebp-24], eax	; ebp-24 == sscan*2
	mov [ebp-28], ecx	; ebp-28 == sscan/4
	shr dword [ebp-28], 2
	and cl, 3
	mov byte [ebp-10], cl	; ebp-10 == low 2 bits of sscan

.beginscan:
	mov edx, [ebp+32]	; scanlen
	mov [ebp-8], edx	; ebp-8 == scanlen
	; note:	scanlen can never be zero, so we don't worry about it.

.beginpel:
	movzx edx, byte [ebp-9]	; load finetune
	mov ecx, [ebp+20]	; read alpha
	movzx eax, byte [ecx]
	and al, [ebp-4+edx]	; index into mask table
	lea ecx, [2*edx]
	shr eax, cl		; shift right by finetune*2
	; jump table (eax should never be > 3)
	jmp [.switch+4*eax]
.switch: dd .case0, .case1, .case2, .case3

.case0:	add edi, 2		; transparent, so skip to the next pixel
	add esi, 2
	jmp .endpel

.case1:	lodsw
	bswap eax
	shr eax, 16
	mov edx, eax
	movzx ecx, word [edi]
	bswap ecx
	shr ecx, 16
	jmp .docombine

.case2:	lodsw
	bswap eax
	shr eax, 16
	mov ecx, eax
	movzx edx, word [edi]
	bswap edx
	shr edx, 16
	mov eax, edx

.docombine:	
	mov ebx, _blit_onethird
	and al, 31		; red
	xlatb
	xchg dl, al
	shr ax, 5
	and ax, 63		; green
	xlatb
	shl ax, 5
	or al, dl
	xchg dx, ax
	shr ax, 11		; blue
	xlatb
	shl ax, 11
	or dh, ah

	mov eax, ecx
	mov ebx, _blit_twothird
	and al, 31		; red
	xlatb
	xchg cl, al
	shr ax, 5
	and ax, 63		; green
	xlatb
	shl ax, 5
	or al, cl
	xchg cx, ax
	shr ax, 11		; blue
	xlatb
	shl ax, 11
	or ch, ah

	mov eax, edx
	add eax, ecx
	stosw			; write the word
	jmp .endpel

.case3:	movsw			; simple.

.endpel:
	inc byte [ebp-9]	; increment finetune
	cmp byte [ebp-9], 4	; have we hit four yet?
	jl .l2
	mov byte [ebp-9], 0	; yes, zero out finetune
	inc dword [ebp+20]	; add one to coarse salp address
.l2:	dec dword [ebp-8]	; decrement scanlen
	jz .l3			; out of pixels in this scan?
	jmp .beginpel

.l3:	; end of scanline
	add edi, [ebp-20]	; add dscanoff*2
	add esi, [ebp-24]	; add sscanoff*2

	mov edx, [ebp-28]	; add sscanoff>>2 to salp
	add [ebp+20], edx
	mov cl, [ebp-10]	; add sscanoff%4 to finetune
	add [ebp-9], cl
	cmp byte [ebp-9], 4	; are we over 3?
	jl .l4
	inc dword [ebp+20]	; increment salp
	and byte [ebp-9], 3	; finetune mod 4
.l4:	cmp edi, [ebp-16]	; are we at the end?
	jge .l5
	jmp .beginscan		; do another scanline if not

.l5:	pop ebx
	pop esi
	pop edi
	add esp, 32
	pop ebp
	ret

_blit164:
	ret

_blit168:
	ret

; twenty-four bit

;;
;; _blit240
;; Similar to any of the _blit*0s. Rote rep movsb to copy scanlines.
;; 
_blit240:
	push ebp
	mov ebp, esp
	push esi
	push edi
	push ebx

	mov edi, [ebp+8]
	mov esi, [ebp+16]

	mov eax, [ebp+28]
	mov ebx, [ebp+44]
	lea eax, [3*eax]
	lea ebx, [3*ebx]
	mov [ebp+28], eax
	mov [ebp+44], ebx

	mov ebx, [ebp+36]
	lea ebx, [3*ebx]
	add ebx, edi

	mov eax, [ebp+24]
	lea eax, [3*eax]
	add edi, eax

	mov eax, [ebp+40]
	lea eax, [3*eax]
	add esi, eax

.loop:
	mov ecx, [ebp+32]
	lea ecx, [3*ecx]
	rep movsb
	add edi, [ebp+28]
	add esi, [ebp+44]
	cmp edi, ebx
	jl .loop

	pop ebx
	pop edi
	pop esi
	pop ebp
	ret

;;
;; _blit241
;; Uses the bit test style as seen in the generic C implementation,
;; that is, two variables -- finetune (bl register) and coarse ([ebp-4])
;; 
_blit241:
	push ebp
	mov ebp, esp
	sub esp, 44
	push edi
	push esi
	push ebx

	mov edi, [ebp+8]	; destination address
	mov edx, [ebp+24]	; destination offset
	lea edx, [3*edx]	; multiply dest offset by 3
	add edi, edx

	mov eax, [ebp+36]	; end offset
	lea eax, [3*eax]	; multiply end offset by 3
	add eax, edi
	mov [ebp-8], eax

	mov ecx, [ebp+40]	; source offset
	lea esi, [3*ecx]	; triple
	add esi, [ebp+16]	; add in the address

	mov bl, cl		; take the least three bits of source
	and bl, 7		; bl is the finetune (bit to check inside salp)
	shr ecx, 3
	mov [ebp-4], ecx	; ebp-4 is the offset into alpha
	mov eax, [ebp+20]	; add in the salp address
	add [ebp-4], eax

	mov eax, [ebp+44]	; source scan offset
	mov [ebp-24], eax	; ebp-24 is salp offset per scan
	shr dword [ebp-24], 3
	mov [ebp-20], eax
	and dword [ebp-20], 7	; ebp-20 is salp finetune per scan

	mov eax, [ebp+28]
	lea eax, [3*eax]
	mov [ebp-12], eax	; ebp-12 is dest scan offset in bytes

	mov eax, [ebp+44]
	lea eax, [3*eax]
	mov [ebp-16], eax	; ebp-16 is source scan offset in bytes

.loop:	mov ecx, [ebp+32]	; scanlen

.peltest:
	mov eax, [ebp-4]
	movzx edx, byte [eax]	; load a byte from salp
	movzx eax, bl
	bt edx, eax		; test the current alpha bit
	jnc .transparent
	movsw			; copy 3 bytes
	movsb
	jmp .nextpel

.transparent:
	add edi, 3
	add esi, 3

.nextpel:
	inc bl			; next bit to test
	cmp bl, 8		; have we hit the last bit yet?
	jne .l5
	mov bl, 0
	inc dword [ebp-4]	; go on to the next byte in salp
.l5:	dec ecx
	jnz .peltest

	; end of scanline
	add edi, [ebp-12]	; add scanline offsets
	add esi, [ebp-16]
	mov eax, [ebp-24]
	add [ebp-4], eax
	add bl, [ebp-20]
	cmp bl, 7
	jbe .l7
	inc dword [ebp-4]
	and bl, 7

.l7:	cmp edi, [ebp-8]	; are we at the end yet?
	jb .loop

	pop ebx
	pop esi
	pop edi
	add esp, 44
	pop ebp
	ret

_blit242:
	ret
_blit244:
	ret
_blit248:
	ret

; _blit_onethird: 1/3 from 0 to 255. [autogenerated]
_blit_onethird:
db 0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4,
db 5, 5, 5, 6, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9,
db 10, 10, 10, 11, 11, 11, 12, 12, 12, 13, 13, 13, 14, 14, 14,
db 15, 15, 15, 16, 16, 16, 17, 17, 17, 18, 18, 18, 19, 19, 19,
db 20, 20, 20, 21, 21, 21, 22, 22, 22, 23, 23, 23, 24, 24, 24,
db 25, 25, 25, 26, 26, 26, 27, 27, 27, 28, 28, 28, 29, 29, 29,
db 30, 30, 30, 31, 31, 31, 32, 32, 32, 33, 33, 33, 34, 34, 34,
db 35, 35, 35, 36, 36, 36, 37, 37, 37, 38, 38, 38, 39, 39, 39,
db 40, 40, 40, 41, 41, 41, 42, 42, 42, 43, 43, 43, 44, 44, 44,
db 45, 45, 45, 46, 46, 46, 47, 47, 47, 48, 48, 48, 49, 49, 49,
db 50, 50, 50, 51, 51, 51, 52, 52, 52, 53, 53, 53, 54, 54, 54,
db 55, 55, 55, 56, 56, 56, 57, 57, 57, 58, 58, 58, 59, 59, 59,
db 60, 60, 60, 61, 61, 61, 62, 62, 62, 63, 63, 63, 64, 64, 64,
db 65, 65, 65, 66, 66, 66, 67, 67, 67, 68, 68, 68, 69, 69, 69,
db 70, 70, 70, 71, 71, 71, 72, 72, 72, 73, 73, 73, 74, 74, 74,
db 75, 75, 75, 76, 76, 76, 77, 77, 77, 78, 78, 78, 79, 79, 79,
db 80, 80, 80, 81, 81, 81, 82, 82, 82, 83, 83, 83, 84, 84, 84,
db 85
; end table

; _blit_twothird: 2/3 from 0 to 255. [autogenerated]
_blit_twothird:
db 0, 0, 1, 2, 2, 3, 4, 4, 5, 6, 6, 7, 8, 8, 9,
db 10, 10, 11, 12, 12, 13, 14, 14, 15, 16, 16, 17, 18, 18, 19,
db 20, 20, 21, 22, 22, 23, 24, 24, 25, 26, 26, 27, 28, 28, 29,
db 30, 30, 31, 32, 32, 33, 34, 34, 35, 36, 36, 37, 38, 38, 39,
db 40, 40, 41, 42, 42, 43, 44, 44, 45, 46, 46, 47, 48, 48, 49,
db 50, 50, 51, 52, 52, 53, 54, 54, 55, 56, 56, 57, 58, 58, 59,
db 60, 60, 61, 62, 62, 63, 64, 64, 65, 66, 66, 67, 68, 68, 69,
db 70, 70, 71, 72, 72, 73, 74, 74, 75, 76, 76, 77, 78, 78, 79,
db 80, 80, 81, 82, 82, 83, 84, 84, 85, 86, 86, 87, 88, 88, 89,
db 90, 90, 91, 92, 92, 93, 94, 94, 95, 96, 96, 97, 98, 98, 99,
db 100, 100, 101, 102, 102, 103, 104, 104, 105, 106, 106, 107, 108, 108, 109,
db 110, 110, 111, 112, 112, 113, 114, 114, 115, 116, 116, 117, 118, 118, 119,
db 120, 120, 121, 122, 122, 123, 124, 124, 125, 126, 126, 127, 128, 128, 129,
db 130, 130, 131, 132, 132, 133, 134, 134, 135, 136, 136, 137, 138, 138, 139,
db 140, 140, 141, 142, 142, 143, 144, 144, 145, 146, 146, 147, 148, 148, 149,
db 150, 150, 151, 152, 152, 153, 154, 154, 155, 156, 156, 157, 158, 158, 159,
db 160, 160, 161, 162, 162, 163, 164, 164, 165, 166, 166, 167, 168, 168, 169,
db 170
; end table

; EOF blit_x86.s

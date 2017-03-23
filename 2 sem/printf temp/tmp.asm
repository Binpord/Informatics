	global _main
	extern _ReadFile@20
	extern _ExitProcess@4
	extern _GetStdHandle@4
	extern _WriteFile@20
	
	section .text
_main:
	mov		eax, -45
	xor		edx, edx
	mov		esi, -1
	imul	eax, esi
	
	push	0
	call	_ExitProcess@4
	
;--------------------------------
;	Writes your string on cunsole
;--------------------------------
;	Expected values:
;	push buffer length
;	push buffer address
;	Expects space after stack (sub esp, 4) and ebp as pointer to that free space.
;--------------------------------
;	Destruction list:
;	eax, ebx, ecx (in WriteFile@20), esi
;--------------------------------
write_console:
	pop		ebx								; saving the ret address
	
	; WriteFile( hstdOut, message, length(message), &bytes, 0)
	lea		esi, [ebp - 4]					; saving the position of bytes' space
	add		esp, 4 * 2						; need to correct the position of parameters
	mov		ebp, esp
	mov		eax, [ebp - 4]
	mov		[ebp - 4 * 3], eax				; the length is the 3rd parameter of WriteFile
	mov		eax, [ebp - 4 * 2]
	mov		[ebp - 4 * 4], eax				; the address is the 4th parameter of WriteFile
	push	0
	push	esi
	sub		esp, 4 * 2						; 3rd and 4th parameters are already in stack
	
	; hStdOut = GetstdHandle( STD_OUTPUT_HANDLE )
    push    -11
    call    _GetStdHandle@4    				; handle is in eax now
	
    push   	eax
    call    _WriteFile@20
	
	mov		esp, ebp
	lea		ebp, [esi + 4]
	push	ebx
	ret
	
	section .bss
tmpBuf:
	resb	32
tmpBuf_end:
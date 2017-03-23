	global _main
	extern _ReadFile@20
	extern _ExitProcess@4
	extern _GetStdHandle@4
	extern _WriteFile@20
	
	section .text
_main:
	mov		ebp, esp						; Saving start stack pointer.
    sub     esp, 4							; Freeing space for number of written/read words in functions ReadFile and WriteFile.

	; Writing the introduction message.
	mov		ebx, inMessage
	mov		ecx, (inMessage_end - inMessage)
	call	write_console
	
	; Reading string from console to inBuffer.
	
	push	-10								; Getting handle of the standart input device.
	call	_GetStdHandle@4
	mov		ebx, eax						; Moving this handle from eax to ebx.
	
	;BOOL WINAPI ReadFileEx(
	;	_In_      HANDLE                          hFile,
	;	_Out_opt_ LPVOID                          lpBuffer,
	;	_In_      DWORD                           nNumberOfBytesToRead,
	;	_Inout_   LPOVERLAPPED                    lpOverlapped,
	;	_In_      LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
	;	);
	push	0
	lea		eax, [ebp - 4]
    push    eax
	push	(inBuffer_end - inBuffer)
	push	inBuffer
	push	ebx
	call	_ReadFile@20
	
	; Skiping space in inBuffer.
	mov		ebx, inBuffer					; ebx should contain offset of start and eax - of end of the buffer
	mov		eax, ebx
	add		eax, [ebp - 4]
	sub		eax, 2
	call	skip_space
	je		_bad_input						; cmp was in skip space function for oversize
	; After skip space, cl is first non-space sumbol in inBuffer.
	
	; Reading type of the number in input.
	cmp		cl, 'H'
	je		_Hex
	cmp		cl, 'D'
	je		_Decimal
	cmp		cl, 'B'
	je		_Binary
	
	jmp		_bad_input
	
_Hex:
	; Skiping space after 'H'
	inc		ebx
	call	skip_space
	je		_bad_input						; for oversize
	dec		ebx								; as skip space make one superfluous inc
	
	xor		edx, edx						; here will be our number
	
_Hex_circle:
	cmp		cl, '0'							; Finding out if cl is between 0 - 9 or A - F
	jb		_bad_input
	cmp		cl, '9'
	jb		_Hex_num
	cmp		cl, 'F'
	ja		_bad_input
	cmp		cl, 'A'
	ja		_Hex_letter
	
	jmp _bad_input
	
_Hex_num:									; if between 0 and 9
	sub		cl, '0'
	jmp		_Hex_continue
	
_Hex_letter:								; else, if between A and F
	sub		cl, 'A'
	
_Hex_continue:								; cl is now a number, which ASCII was there before
	shl		edx, 4							; edx *= 16
	add		dl, cl
	inc		ebx
	cmp		ebx, eax
	je		_Print
	mov		cl, [ebx]
	jmp		_Hex_circle
	
_Decimal:
	; Skiping space after 'D'
	inc		ebx
	call	skip_space
	je		_bad_input						; for oversize
	dec		ebx								; as skip space make one superfluous inc
	
	xor		edx, edx						; here will be our number
	
_Decimal_circle:
	cmp		cl, ' '							; finding out if cl is between 0 and 9
	je		_Print
	cmp		cl, '0'
	jb		_bad_input
	cmp		cl, '9'
	ja		_bad_input
	
	sub		cl, '0'							; cl is now a number, which ASCII was there before
	imul	edx, 10
	and		ecx, 000FFh
	add		edx, ecx
	inc		ebx
	cmp		ebx, eax
	je		_Print
	mov		cl, [ebx]
	jmp		_Decimal_circle
	
_Binary:
	inc		ebx
	call	skip_space
	je		_bad_input						; for oversize
	dec		ebx								; as skip space make one superfluous inc
	
	xor		edx, edx						; here will be our number
	
_Binary_circle:
	cmp		cl, ' '							; finding out if cl is between 0 and 1
	je		_Print
	cmp		cl, '0'
	jb		_bad_input
	cmp		cl, '1'
	ja		_bad_input
	
	sub		cl, '0'							; cl is now a number, which ASCII was there before
	shl		edx, 1							; edx *= 2
	add		dl, cl
	inc		ebx
	cmp		ebx, eax
	je		_Print
	mov		cl, [ebx]
	jmp		_Binary_circle
	
_Print:
	push	edx								; save edx
	
	mov		ebx, tmpBuf_end
	mov		byte [ebx], 0Dh					; Move '\n' to the end of our string.
	dec		ebx
	mov		byte [ebx], 0Ah					; Move '\r' before '\n' at the end of our string.
	dec		ebx
	mov		byte [ebx], 'H'
	dec		ebx
	
_Print_hex_circle:
	mov		cl, dl
	and		cl, 0Fh
	cmp		cl, 10
	jb		_num
	sub		cl, 10
	add		cl, 'A'
	jmp		_end_num
	
_num:
	add		cl, '0'
_end_num:

	mov		[ebx], cl
	dec		ebx
	shr		edx, 4
	cmp		edx, 0
	jne		_Print_hex_circle
	
	mov		byte [ebx], ' '
	dec		ebx
	mov		byte [ebx], 'D'
	dec		ebx
	
	pop		edx								; recover edx
	push	edx								; save edx
	
	mov		eax, edx						; div needs the num in edx:eax
	mov		esi, 10
_Print_Decimal_circle:
	xor		edx, edx
	div		esi								; the result is in the eax and the rest is in the edx
	mov		cl, dl
	add		cl, '0'
	
	mov		[ebx], cl
	dec		ebx
	cmp		eax, 0
	jne		_Print_Decimal_circle
	
	mov		byte [ebx], ' '
	dec		ebx
	mov		byte [ebx], 'B'
	dec		ebx
	
	pop		edx								; recover edx
	
_Print_Binary_circle:
	mov		cl, dl
	shr		edx, 1
	and		cl, 0001b
	add		cl, '0'
	
	mov		[ebx], cl
	dec		ebx
	cmp		edx, 0
	jne		_Print_Binary_circle
	
	
	mov		ecx, tmpBuf_end
	sub		ecx, ebx
	call	write_console
	
	mov		edx, [esp]
	jmp		_exit
	
_bad_input:
	mov		ebx, outBadIn
	mov		ecx, (outBadIn_end - outBadIn)
	call	write_console

_exit:
    ; ExitProcess(0)
    push    0
    call    _ExitProcess@4

    ; never here
    hlt
	
;-----------------------------------
;	Skips your spaces (sumbols ' ').
;-----------------------------------
;	Expected values:
;	ebx: buffer address
;	eax: end buffer address
;-----------------------------------
;	Exit values:
;	cl: pointer to first non-space sumb in buffer
;-----------------------------------
;	Destruction list:
;	ebx, ecx
;-----------------------------------
skip_space:
	cmp		ebx, eax						; If ebx is more than end of buffer than stop. This cmp allowes you to je after function to solve the oversizing.
	je		_exit_skip_space
	mov		cl, [ebx]
	inc		ebx
	cmp		cl, ' '
	je		skip_space
_exit_skip_space:
	ret
	
;--------------------------------
;	Writes your string on cunsole
;--------------------------------
;	Expected values:
;	ebx: buffer address
;	ecx: buffer length
;	Expects space after stack (sub esp, 4) and rbp as pointer to that free space.
;--------------------------------
;	Destruction list:
;	eax, esi
;--------------------------------
write_console:
	; hStdOut = GetstdHandle( STD_OUTPUT_HANDLE )
    push    -11
    call    _GetStdHandle@4    

    ; WriteFile( hstdOut, message, length(message), &bytes, 0)
    push    0
    lea		esi, [ebp - 4]
	push	esi
    push    ecx
    push    ebx
    push   	eax
    call    _WriteFile@20
	ret
	
	section .data
inMessage:
    db      'Please, enter input>'
inMessage_end:

outBadIn:
	db		'Sorry, bad input, please try again.', 10
outBadIn_end:

	section .bss
inBuffer:
	resb	128
inBuffer_end:

tmpBuf:
	resb	35
tmpBuf_end:
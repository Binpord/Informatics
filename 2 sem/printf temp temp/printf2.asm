	global _main
	extern _ReadFile@20
	extern _ExitProcess@4
	extern _GetStdHandle@4
	extern _WriteFile@20
	
	section .text
_main:
	mov		ebp, esp						; Saving start stack pointer.
    sub     esp, 4							; Freeing space for number of written/read words in functions ReadFile and WriteFile.

	; printf("string", %d, %x, %b, %c, %s, %s, %c, %x)
	push	3802
	push	3
	push	_and
	push	string							; string for %s
	push	'e'								; char for %c
	push	156								; this will be displayed as octal number (234 for 156)
	push	-10								; this will be displayed as binary number (1010 for 10)
	push	-233							; this will be displayed as hex number (E9 for 233)
	push	-24								; this will be displayed as decimal number (24 for 24)
	push	msg								; "string" in proto of function
	call	_printf
	
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
	
;--------------------------------
;	You do know what it does, don't you?
;--------------------------------
;	Expected values:
;	push all the parameters in reversed way
;	requaiers the write_console function
;--------------------------------
;	Destruction list:
;	eax, ebx, ecx, edx, esi, edi, ebp
;--------------------------------
_printf:
	mov		ebp, esp						; saving stack pointer
	sub		esp, 4 * 3						; i need 3 variables
	
	mov		[ebp - 4 * 2], dword 1			; the amount of used parameters (1 is for main string, that we will be using in circle)
	
	mov		ecx, [ebp + 4]					; ecx is pointer to string
	mov		[ebp - 4 * 3], ecx				; [ebp - 4 * 3] is now pointer to start of printing (changes after printing any part)
	
	; Finding out of the char in bl is 0(the string is ended), or if it is '%' so that we need to find the format of the output and print it.
_Format_circle:
	mov		bl, byte [ecx]					; next char of the string is in the bl
	inc		ecx
	cmp		bl, 0
	je		_printf_exit
	cmp		bl, '%'
	je		_FindFormat
	; If bl is not format or string end or '%' or the format is already processed than we are finding the next '%' or 0 (printing only in FindFormat 
	; and printf exit).
	jmp		_Format_circle
	
	; If bl is '%' than we printing part of the string from [ebx - 4 * 3] to (ecx - 2) as ecx points to the char after % and than processing the format.
	; Amount of used paremeters is stored in [ebx - 4 * 2].
_FindFormat:
	push	ecx								; saving ecx before write_console
	mov		edx, ecx
	sub		edx, [ebp - 4 * 3]				; [ebp - 4 * 3] is pointer to start of printing (changes after printing any part)
	dec		edx
	push	edx
	push	dword [ebp - 4 * 3]
	call	write_console
	pop		ecx
	inc		ecx								; the next print start should point to char after the %'something'
	mov		[ebp - 4 * 3], ecx
	
	; Finding out what is lying after the '%'. Processing only %d, %x, %b, %o, %c, %s. Another allowed combination is %% - % will be printed.
	; Processing strings such as "%", 0: " !invalid format! " will be printed. To all another combinations such as %a the " !invalid format! " 
	; will be printed in their places.
	mov		bl, byte [ecx - 1]				; inc ecx was before
	cmp		bl, 0
	je		_BadFormatExit
	cmp		bl, 'd'
	je		_DecimalFormat
	cmp		bl, 'x'
	je		_HexFormat
	cmp		bl, 'b'
	je		_BinaryFormat
	cmp		bl, 'o'
	je		_OctalFormat
	cmp		bl, 'c'
	je		_CharFormat
	cmp		bl, 's'
	je		_StringFormat
	cmp		bl, '%'
	je		_Percent
	
	call	_BadFormatPrint
	jmp		_Format_circle
	
_DecimalFormat:
	call	_DecimalPrint
	jmp		_Format_circle
	
_HexFormat:
	call	_HexPrint
	jmp		_Format_circle
	
_BinaryFormat:
	call	_BinaryPrint
	jmp		_Format_circle
	
_OctalFormat:
	call	_OctalPrint
	jmp		_Format_circle
	
_CharFormat:
	call	_CharPrint
	jmp		_Format_circle
	
_StringFormat:
	call	_StringPrint
	jmp		_Format_circle
	
_Percent:
	call	_PercentPrint
	jmp		_Format_circle
	
	; Only one thing is not mentioned yet: strings like "%", 0. To process them we need to print " !invalid format! " and ret.
_BadFormatExit:
	push	(bad_format_msg_end - bad_format_msg)
	push	bad_format_msg
	call	write_console
	mov		esp, ebp
	ret
	
	; On exit we need to print last part of the string and than clean stack. To do cleaning we need to pop all used parameters, amount is
	; in [ebp - 4 * 2].
_printf_exit:
	mov		edx, ecx
	sub		edx, [ebp - 4 * 3]				; [ebp - 4 * 3] is pointer to start of printing (changes after printing any part)
	sub		edx, 1
	push	edx
	push	dword [ebp - 4 * 3]
	call	write_console
	
	mov		eax, [ebp - 4 * 2]
	mov		esp, ebp
	pop		ebx								; saving ret address
	imul	eax, 4
	add		esp, eax
	push	ebx
	
	ret
	
;----------------
;	This is auxiliary function as I just understood, that I can have signed input, so it just imul it on (-1) and prints '-'.
;----------------
;	Expected values:
;	Number in eax
;	requaiers the write_console function
;----------------
;	Destruction list:
;	same as write_console, without ecx and eax (as I need them in printf):
;	ebx, ecx (in WriteFile@20), esi
;----------------
_signed_to_unsigned:
	mov		esi, -1							; making eax unsigned by signed mul
	imul	eax, esi
	
	mov		esi, tmpBuf						; making tmp string "-"
	mov		[esi], byte '-'
	
	push	eax								; saving eax and ecx before write_console
	push	ecx
	
	push	1								; printing '-'
	push	tmpBuf
	call	write_console
	
	pop		ecx								; reparing eax and ecx after write_console
	pop		eax
	
	ret
	
;-----------------
;	This is auxiliary function for printing decimal numbers.
;-----------------
;	Expected values:
;	[ebp - 4 * 2] - amount of already used parameters
;	parameter in stack
;----------------
;	Destruction list:
;	eax, edx, esi, edi
;----------------
	; To make the string of decimal number we need to generate it from the end and put there the rest from div 10 plus '0'.
	; Div divisions number from edx:eax and puts result in eax and the rest in edx. Let's use this.
_DecimalPrint:
	mov		edi, tmpBuf_end
	mov		esi, [ebp - 4 * 2]				; amount of used parameters
	mov		eax, [ebp + 4 + esi * 4] 		; as we need to skip ret address and used parameters
	
	cmp		eax, 0							; finding out if parameter is belov 0 and calling _signed_to_unsigned if it is
	jge		_unsigned_decimal				; signed cmp
	
_signed_decimal:
	call	_signed_to_unsigned
	jmp		_unsigned_decimal
_unsigned_decimal:
	
	add		dword [ebp - 4 * 2], 1
	mov		esi, 10
	
_Decimal_circle:
	xor		edx, edx						; need edx to be clean before div
	div		esi								; the result is in the eax and the rest is in the edx
	add		dl, '0'
	mov		[edi], dl
	dec		edi
	cmp		eax, 0							; if the result of div 10 is not 0 than do again
	jne		_Decimal_circle
	
	; if result is 0 than print the number and jump to _Format_circle
	push	ecx								; saving ecx before write_console
	mov		edx, tmpBuf_end
	sub		edx, edi
	add		edi, 1							; as one dec edi was superfluous
	push	edx
	push	edi
	call	write_console
	pop		ecx
	ret
	
;-----------------
;	This is auxiliary function for printing hex numbers.
;-----------------
;	Expected values:
;	[ebp - 4 * 2] - amount of already used parameters
;	parameter in stack
;----------------
;	Destruction list:
;	eax, edx, esi, edi
;----------------
	; To make string of hex number we need to generate it from the end and put there the rest from div 16 (shr 4) (last byte of our number) plus '0'
	; if rest is below than 10 or minus 10 plus 'A' if the result  is above or equal 10.
_HexPrint:
	mov		edi, tmpBuf_end
	mov		esi, [ebp - 4 * 2]				; amount of used parameters
	mov		eax, [ebp + 4 + esi * 4] 		; as we need to skip ret address and used parameters
	
	cmp		eax, 0							; finding out if parameter is belov 0 and calling _signed_to_unsigned if it is
	jge		_unsigned_hex					; signed cmp
	
_signed_hex:
	call	_signed_to_unsigned
	jmp		_unsigned_hex
_unsigned_hex:
	
	add		dword [ebp - 4 * 2], 1
_Hex_circle:
	mov		dl, al
	and		dl, 0Fh							; need only last byte
	cmp		dl, 10
	jb		_num
	sub		dl, 10
	add		dl, 'A'
	jmp		_end_num
	
_num:
	add		dl, '0'
_end_num:

	mov		[edi], dl
	dec		edi
	shr		eax, 4
	cmp		eax, 0							; if result of div 16 is not 0 than do again
	jne		_Hex_circle
	
	push	ecx								; saving ecx before write_console
	mov		edx, tmpBuf_end					; if result is 0 than print the number and jump to _Format_circle
	sub		edx, edi
	add		edi, 1							; as one dec edi was superfluous
	push	edx
	push	edi
	call	write_console
	pop		ecx
	ret
	
;-----------------
;	This is auxiliary function for printing binary numbers.
;-----------------
;	Expected values:
;	[ebp - 4 * 2] - amount of already used parameters
;	parameter in stack
;----------------
;	Destruction list:
;	eax, edx, esi, edi
;----------------
	; To make string of binary number we need to generate it from the end and put there the rest from div 2 (shr 1) (last byte of our number) plus '0'.
_BinaryPrint:
	mov		edi, tmpBuf_end
	mov		esi, [ebp - 4 * 2]				; amount of used parameters
	mov		eax, [ebp + 4 + esi * 4] 		; as we need to skip ret address and used parameters
	
	cmp		eax, 0							; finding out if parameter is belov 0 and calling _signed_to_unsigned if it is
	jge		_unsigned_binary				; signed cmp
	
_signed_binary:
	call	_signed_to_unsigned
	jmp		_unsigned_binary
_unsigned_binary:
	
	add		dword [ebp - 4 * 2], 1
_Binary_circle:
	mov		dl, al
	and		dl, 0001b						; need only last bit
	add		dl, '0'
	
	mov		[edi], dl
	dec		edi
	shr		eax, 1
	cmp		eax, 0							; if result of div 2 is not 0 than do again
	jne		_Binary_circle
	
	push	ecx								; saving ecx before write_console
	mov		edx, tmpBuf_end					; if result is 0 than print the number and jump to _Format_circle
	sub		edx, edi
	add		edi, 1							; as one dec edi was superfluous
	push	edx
	push	edi
	call	write_console
	pop		ecx
	ret

;-----------------
;	This is auxiliary function for printing octal numbers.
;-----------------
;	Expected values:
;	[ebp - 4 * 2] - amount of already used parameters
;	parameter in stack
;----------------
;	Destruction list:
;	eax, edx, esi, edi
;----------------
	; To make string of octal number we need to generate it from the end and put there the rest from div 8 (shr 3) (last byte of our number) plus '0'.
_OctalPrint:
	mov		edi, tmpBuf_end
	mov		esi, [ebp - 4 * 2]				; amount of used parameters
	mov		eax, [ebp + 4 + esi * 4] 		; as we need to skip ret address and used parameters
	
	cmp		eax, 0							; finding out if parameter is belov 0 and calling _signed_to_unsigned if it is
	jge		_unsigned_octal					; signed cmp
	
_signed_octal:
	call	_signed_to_unsigned
	jmp		_unsigned_octal
_unsigned_octal:
	
	add		dword [ebp - 4 * 2], 1
_Octal_circle:
	mov		dl, al
	and		dl, 00000111b					; need only 3 last bites
	add		dl, '0'
	
	mov		[edi], dl
	dec		edi
	shr		eax, 3
	cmp		eax, 0							; if result of div 8 is not 0 than do again
	jne		_Octal_circle
	
	push	ecx								; saving ecx before write_console
	mov		edx, tmpBuf_end					; if result is 0 than print the number and jump to _Format_circle
	sub		edx, edi
	add		edi, 1							; as one dec edi was superfluous
	push	edx
	push	edi
	call	write_console
	pop		ecx
	ret
	
;-----------------
;	This is auxiliary function for printing char.
;-----------------
;	Expected values:
;	[ebp - 4 * 2] - amount of already used parameters
;	parameter in stack
;----------------
;	Destruction list:
;	eax, esi, edi
;----------------
	; To print char we are checking that it is 1 byte (that 0 < number < 256) and than putting it to the tmpBuf and printing it.
_CharPrint:
	mov		esi, [ebp - 4 * 2]				; amount of used parameters
	mov		eax, [ebp + 4 + esi * 4] 		; as we need to skip ret address and used parameters
	add		dword [ebp - 4 * 2], 1
	
	cmp		eax, 0
	jb		_BadFormatPrint
	jb		_Char_Exit
	cmp		eax, 256
	ja		_BadFormatPrint
	ja		_Char_Exit
	
	mov		edi, tmpBuf
	mov		[edi], al
	
	push	ecx								; saving ecx before write_console
	push	1
	push	tmpBuf
	call	write_console
	pop		ecx
	
_Char_Exit:
	ret
	
;-----------------
;	This is auxiliary function for printing string.
;-----------------
;	Expected values:
;	[ebp - 4 * 2] - amount of already used parameters
;	parameter in stack
;----------------
;	Destruction list:
;	eax, edx, esi, edi
;----------------
	; To print string we are counting it's length (before 0 sumbol) and printing it
_StringPrint:
	mov		esi, [ebp - 4 * 2]				; amount of used parameters
	mov		eax, [ebp + 4 + esi * 4] 		; as we need to skip ret address and used parameters
	add		dword [ebp - 4 * 2], 1
	mov		esi, eax
_String_circle:
	mov		dl, [esi]
	inc		esi
	cmp		dl, 0
	jne		_String_circle
	
	push	ecx								; saving ecx before write_console
	sub		esi, 1							; We don't need 0 ASCII, do we?
	sub		esi, eax
	push	esi
	push	eax
	call	write_console
	pop		ecx
	ret

;-----------------
;	This is auxiliary function for printing percent.
;-----------------
;	Expected values:
;	[ebp - 4 * 2] - amount of already used parameters
;	parameter in stack
;----------------
;	Destruction list:
;	edi
;----------------
	; If finding construction %% than printing %
_PercentPrint:
	mov		edi, tmpBuf
	mov		byte [edi], '%'
	
	push	ecx								; saving ecx before write_console
	push	1
	push	tmpBuf
	call	write_console
	pop		ecx
	ret
	
;-----------------
;	This is auxiliary function for printing " !invalid format! ".
;-----------------
;	Expected values:
;	[ebp - 4 * 2] - amount of already used parameters
;	parameter in stack
;----------------
;	Destruction list:
;	nope
;----------------
	; As processing of bad format I'm writing " !invalid format! " at the place, where tha mistake is and jumping to format circle.
_BadFormatPrint:
	push	ecx								; saving ecx before write_console
	push	(bad_format_msg_end - bad_format_msg)
	push	bad_format_msg
	call	write_console
	pop		ecx
	ret
	
	section .data
msg:
    db      "I want you to see this number %d, and this %x, and this one too %b. Another one number %o. And this percent %%. \
	And this letter %c and this string %s. And some invalid format: %alalala. That's all, %s I %c %x", 0
msg_end:

string:
	db		"something like this", 0
string_end:

_and:
	db		"and", 0
_and_end:

bad_format_msg:
	db		" !invalid format! "
bad_format_msg_end:
	
	section .bss
tmpBuf:
	resb	32
tmpBuf_end:
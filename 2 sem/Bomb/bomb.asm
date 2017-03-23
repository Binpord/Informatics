    global _main
    extern _ReadFile@20
    extern _ExitProcess@4
    extern _GetStdHandle@4
    extern _WriteFile@20
	extern _WriteConsole
	extern _ReadConsole
	
    section .text
_main:
    mov      ebp, esp
	
    sub      esp, 4 * 3                            ; creating space for write_console
    push	(_msg_end - _msg)
    push	_msg
    call	_WriteConsole
	
	call	_ReadPassword
	
	call	_CheckPassword
	
	shr		eax, 1
	jc		_good_password
	
	sub		esp, 4 * 3
	push	(_bad_password_msg_end - _bad_password_msg)
	push	_bad_password_msg
	call	_WriteConsole
	
    push	1
    call	_ExitProcess@4
	
_good_password:
	push	_retaddr
	push	_GiveAdmin
	ret
_retaddr:
	
    push	0
    call	_ExitProcess@4
	
;-----------------------
;	This function reads password and writes it in stack. Also it contains some mistakes...
;-----------------------
;	Expected values:
;	no expected values
;-----------------------
;	Destruction list:
;	
;-----------------------
_ReadPassword:
	pop		ebx										; saving ret address
	mov		ebp, esp
	sub		esp, 6									;creating space for password
	
_circle:											; this pointer should have been AFTER next 3 lines!!!
	sub		ebp, 6									; our password will contain only 4 sumbs, but buffer should contain it + 0Dh, 0Ah
	cmp		ebx, 0
	jne		_push_ret								; if circle pointer was on it's place, than I didn't need to do this, 
													; but now I need to check if I already pushed ret address
_after_push:
	sub		esp, 4 * 3
	push	6
	push	ebp
	call	_ReadConsole
	mov		ebx, 0
	
	cmp		eax, 6									; if password do not contain enough sumbs than do another circle
	jne		_not_enough
	
	ret												; else, when password contains 4 sumbs - return to main
	
_not_enough:
	sub		esp, 4 * 3
	push	(_msg_not_enough_end - _msg_not_enough)
	push	_msg_not_enough
	call	_WriteConsole
	mov		ebx, 0
	
	jmp		_circle
	
_push_ret:
	push	ebx
	jmp		_after_push
	
;--------------------
;	This function checks your password (password need to contain at least 1 element).
;--------------------
;	Expected values:
;	password of 4 sumbs in stack
;	special strings for getting the password (real password is "1qt2")
;--------------------
;	Destruction list:
;
;--------------------
_CheckPassword:
	push	ebp
	mov		ebp, esp
	
	sub		esp, 1								; need space for char of generating password
	
	mov		ebx, _passwordBuf					; need for addressing there
	xor		esi, esi							; counting number of new generating char to cmp with _passwordBuf
	
_gen_pw_circle:
	xor		eax, eax							; counting with al (as I need to div count)
	xor		edx, edx							; additional count in dl as I need to know, what byte I am examining
	mov		byte [ebp - 1], 0
	inc		esi
	
_gen_pw_char:
	mov		cl, byte [ebx]						; next char of _passwordBuf is in cl
	add		ebx, 1
	inc		dl
	mov		al, dl
	
	mov		ch, 10
	mul		ch
	add		al, byte 81
	mov		ch, 9
	div		ch									; result is in al, rest in ah, ah is lesser than 8

	push	ebx
	push	ecx
	push	edx
	shr		eax, 8
	and		eax, 000000FFh						; eax = ah now
	sub		eax, 1
	push 	eax									; eax is lesser than 8 => result is in al
	call	_2_in_pop
	pop		edx
	pop		ecx
	pop		ebx
	
	mov		edi, _passwordBufBuf
	add		edi, edx
	sub		edi, 1								; 1 inc dl was superfluous
	sub		esi, 1								; as 1 inc esi was superfluous
	xor		cl, byte [edi + esi * 8]
	add		esi, 1								; to return esi to it's real value
	and		cl, al
	
	add		[ebp - 1], cl						; cl contains only 1 bit
	
	cmp		dl, 8
	jne		_gen_pw_char
	
	mov		dl, [ebp + 4 * 2 + esi - 1]			; need to skip push ebp, ret address and already cmped sumbs, but 1 inc esi was superfluous
	cmp		[ebp - 1], dl
	jne		_bad_password
	
	cmp		esi, 4
	jne		_gen_pw_circle
	
	mov		eax, 1								; here only if password is good
	add		esp, 1
	pop		ebp
	ret
	
_bad_password:
	mov		eax, 0
	add		esp, 1
	pop		ebp
	ret
	
	
;-------------------
;	This func pops and returns 2 in that degree (degree should be not less than 1!!!).
;-------------------
;	Expected values:
;	push degree
;-------------------
;	Destruction list:
;	eax, ebx, ecx, edx
;-------------------
_2_in_pop:
	pop		edx									; saving ret address
	pop		ebx
	mov		eax, 1
	cmp		ebx, 0
	je		_nothing_to_do
	xor		ecx, ecx
	
_2_circle:
	imul	eax, 2
	add		ecx, 1
	cmp		ecx, ebx
	jne		_2_circle
	
	push	edx
	ret
	
_nothing_to_do:
	push	edx
	ret
	
;------------------
;	This function "gives" you admin rights.
;------------------
;	Expected values:
;	nothing
;------------------
;	Destruction list:
;	
;------------------
_GiveAdmin:
	sub		esp, 4 * 3
	push	(_admin_rights_end - _admin_rights)
	push	_admin_rights
	call	_WriteConsole
	
	push	0
    call	_ExitProcess@4								; to ease cracking
	
	section .data
_msg:
	db		"Hello, user. To gain admin rights, please, write password:", 0Dh, 0Ah, ">"
_msg_end:

_msg_not_enough:
	db		"Your password doesn't contain enough sumbols. Please, write new one:", 0Dh, 0Ah, ">"
_msg_not_enough_end:

_passwordBuf:
	db		"bvqiGJFe", "zxqy!BH8", "qzjqpD9o", "KtajhAfk" 
_passwordBuf_end:

_passwordBufBuf:
	db		"cfayskkb", "ihai0a84", "ajwakxel", "kr1zyqav"
_passwordBufBuf_end:

_bad_password_msg:
	db		0Dh, 0Ah, "Sorry, your password is not good. Please, try again later.", 0Dh, 0Ah
_bad_password_msg_end:

_admin_rights:
	db		0Dh, 0Ah, "Admin rights received. Thanks for using this programm."
_admin_rights_end:
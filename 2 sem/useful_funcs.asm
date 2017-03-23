extern	_GetStdHandle@4
extern	_WriteFile@20
extern	_ReadFile@20
global	_WriteConsole
global	_ReadConsole

section	.text

;--------------------------------
;   Writes your string on cunsole
;--------------------------------
;   Expected values:
;   sub esp, 4 * 3 (space for saving number of written sumbols + 2 places for dwords 0 and &bytes)
;   push buffer length
;   push buffer address
;--------------------------------
;   Destruction list:
;   eax, ebx, ecx (in WriteFile), esi
;--------------------------------
_WriteConsole:
	mov		ebx, [esp]							; saving ret address + creating space for last arg in _WriteFile@20
    
    ; WriteFile( hstdOut, message, length(message), &bytes, 0)
    mov     dword [esp + 4 * 4], 0             	; this 0: --^
    lea     esi, [esp + 4 * 5]
    mov     [esp + 4 * 3], esi                 	; num of written sumbs stores after parameters
    
    ; hStdOut = GetstdHandle( STD_OUTPUT_HANDLE )
	push	-11									; get handle of console
	call    _GetStdHandle@4                     ; handle is in eax now
	
    mov		[esp], eax
    
    call    _WriteFile@20
    
	pop		eax									; num of written sumbs is in eax + cleaning stack
    push    ebx
    ret
	
;---------------------------------
;	Reads your string from console
;---------------------------------
;   Expected values:
;   sub esp, 4 * 3 (space for saving number of written sumbols + 2 places for dwords 0 and &bytes)
;   push buffer length
;   push buffer address
;--------------------------------
;   Destruction list:
;   eax, ebx, ecx (in ReadFile), esi
;--------------------------------
_ReadConsole:
	mov		ebx, [esp]							; saving ret address + creating space for last arg in _ReadFile@20
	
	;BOOL WINAPI ReadFileEx(
	;	_In_      HANDLE                          hFile,
	;	_Out_opt_ LPVOID                          lpBuffer,
	;	_In_      DWORD                           nNumberOfBytesToRead,
	;	_Inout_   LPOVERLAPPED                    lpOverlapped,
	;	_In_      LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
	;	);
	mov     dword [esp + 4 * 4], 0             	; lpCompletionRoutine = 0
    lea     esi, [esp + 4 * 5]
    mov     [esp + 4 * 3], esi                 	; num of written sumbs stores after parameters
    
    ; hStdOut = GetstdHandle( STD_OUTPUT_HANDLE )
	push	-10									; get handle of standart input device
	call    _GetStdHandle@4                     ; handle is in eax now
	
    mov		[esp], eax
    
    call    _ReadFile@20
    
	pop		eax									; num of readen sumbs is in eax (including 0Dh, 0Ah) + cleaning stack
	push    ebx
    ret
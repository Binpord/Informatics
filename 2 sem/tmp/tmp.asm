	.model	flat
	.XMM
	PUBLIC	_main
	EXTRN	_printf:PROC
	EXTRN	_scanf:PROC
	EXTRN	_CheckEbx:PROC
	
_TEXT SEGMENT
	_stcbrd$ = -4
	_a$ = -8
_main PROC
	
	comisd	xmm4, xmm5

_main ENDP
_TEXT ENDS
END
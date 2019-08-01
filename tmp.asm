SECTION .data
STR0:	dw	'Hallo :D',0
STR1:	dw	'ello',0
STR2:	dw	'Hello',0
STR3:	dw	'World',0
	
SECTION .bss
n:	resw	1
a:	resd	1
b:	resd	1
	
SECTION .text
	global _start
_start:
	push ebp
	mov ebp,esp

	mov eax,STR0
	mov [n], eax
	mov eax,23
	mov [a], eax
	mov eax,42
	mov [b], eax

	;GET
	sub esp,12

	;CONST
	mov eax,10
	push eax

	;STORE_LOCAL
	pop eax
	mov [ebp+0],eax

	;CONST
	mov eax,STR1
	push eax

	;STORE_LOCAL
	pop eax
	mov [ebp+4],eax

	;LOAD_LOCAL
	mov eax,[ebp+0]
	push eax

	;CONST
	mov eax,10
	push eax

	;EQU

	;JUMP_FALSE

	;CONST
	mov eax,STR2
	push eax

	;STORE_LOCAL
	pop eax
	mov [ebp+4],eax

	;JUMP

	;CONST
	mov eax,STR3
	push eax

	;STORE_LOCAL
	pop eax
	mov [ebp+8],eax

	;UNGET
	add esp,12

	mov esp,ebp
	pop ebp

	mov ebx, 0
	mov eax, 1
	int 80h

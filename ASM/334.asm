global _start

section .bss
text resb 100
buf	 resb 1
section .data

probel db 10

section .text
_start:	
		xor 	esi, esi 	;counter
read:
		mov 	eax, 3		;read
		mov 	ebx, 0 		;stdin
		mov 	ecx, text
		add		ecx, esi
		mov 	edx, 1	;read byte
		int 	80h
		cmp eax, 0
		jb error
		cmp byte[text + esi], '.'
		je readok
		inc esi
		jmp read
		
		
readok:
		xor 	esi, esi
print:	
		mov 	eax, 4		;write
		mov 	ebx, 1		;stdout
		mov		ecx, text 	;adress
		add		ecx, esi	;adress[esi]
		mov 	edx, 1		;write 1 byte
		int 80h
		inc esi
		cmp esi, 100		
		jb print
		
		;print \n
		mov 	eax, 4		
		mov 	ebx, 1		
		mov		ecx, probel
		mov 	edx, 1		
		int 80h
		
exit:	
		
clear_buffer: 
		mov eax, 3
		mov	ebx, 0
		mov ecx, buf
		mov edx, 1
		int 80h
		cmp byte[ecx], 10
		jne clear_buffer
		
		
		mov		eax, 1
		mov 	ebx, 0
		int 	80h


error:
		mov		eax, 1
		mov		ebx, 1
		int 80h

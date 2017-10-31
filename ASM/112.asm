global _start

section .bss
string resb 101
lng    resd 1
sym    resb 1

section .data

eoln db 10
debugs1 db 'rulee-1', 10
debugs2 db 'rulee-2', 10

section .text

_start: 
		xor esi, esi
readb:  
		mov eax, 3
		mov ebx, 0
		mov ecx, string
		add	ecx, esi
		mov edx, 1
		push edx
		push ecx
		push ebx
		push eax
		int 80h
	;   jc  error
		add esp, 16
	    cmp eax, 0
		jb  error
		cmp byte[string + esi], '.'
		je  endread
		inc esi
		jmp readb
endread:
		mov dword[lng], esi
		xor esi, esi
write1:	
		cmp esi, dword[lng]
		je  endwrite1
		mov eax, 4
		mov ebx, 1
		mov ecx, string
		add ecx, esi
		mov edx, 1
		push edx
		push ecx
		push ebx
		push eax
		int 80h
		add esp, 16
		inc esi
		jmp write1
endwrite1:
		mov eax, 4		
		mov ebx, 1		
		mov	ecx, eoln
		mov edx, 1
		push edx
		push ecx
		push ebx
		push eax	
		int 80h
		add esp, 16
		dec dword[lng]
		mov ecx, dword[lng]
		mov al, byte[string + ecx]
		dec ecx
		cmp al, 'A'
		jae nxtcmp
userl2: 
		push dword [lng]
		push dword string
		call rule_2
		add esp, 8
		jmp endofrules
nxtcmp: 
		cmp al, 'Z'
		ja  userl2
lpcmp:  
		cmp al, byte[string + ecx]
		je  userl2
		cmp ecx, 0
		je  userl1
		dec ecx
		jmp lpcmp
userl1: 
		push dword[lng]
		push dword string
		call rule_1
		add esp, 8
endofrules:
		xor esi, esi
		inc dword[lng]
write2:	
		cmp esi, dword[lng]
		je  endwrite2
		mov eax, 4
		mov ebx, 1
		mov ecx, string
		add ecx, esi
		mov edx, 1
		push edx
		push ecx
		push ebx
		push eax
		int 80h
		add esp, 16
		inc esi
		jmp write2
endwrite2:
		mov eax, 4		
		mov ebx, 1		
		mov	ecx, eoln
		mov edx, 1
		push edx
		push ecx
		push ebx
		push eax		
		int 80h
		add esp, 16
clrb:   
		mov eax, 3
		mov ebx, 0
		mov ecx, sym
		mov edx, 1
		;push edx
		;push ecx
		;push ebx
		;push eax
		int 80h
		;add esp, 16
		cmp byte[ecx], 10
		jne clrb
		mov	eax, 1
		mov ebx, 0
		push ebx
		push eax
		int 80h
error:  
		mov	eax, 1
		mov	ebx, 1
		push ebx
		push eax
		int 80h
		
rule_1: 
		push ebp
		mov ebp, esp
		mov eax, 4
		mov ebx, 1
		mov ecx, debugs1
		mov edx, 8
		int 80h
		mov eax, dword[ebp + 8]
		mov ecx, dword[ebp + 12]
		inc ecx
.lp     dec ecx
		cmp byte[eax + ecx], '1'
		jb  .eolp
		cmp byte[eax + ecx], '9'
		ja  .eolp
		add byte[eax + ecx], 48
.eolp   cmp ecx, 0
		jg  .lp
		mov esp, ebp
		pop ebp
		ret
		
rule_2: 
		push ebp
		mov ebp, esp
		mov eax, 4
		mov ebx, 1
		mov ecx, debugs2
		mov edx, 8
		int 80h
		push ebx
		mov ebx, dword[ebp + 8]
		mov eax, dword[ebp + 12]
		cmp eax, 0
		je  .eolp
		dec eax
		xor edx, edx
		mov ecx, 2
		div ecx
		mov ecx, eax
		add edx, eax
		inc ecx
.lp     dec ecx
		inc edx
		xor eax, eax
		mov al, byte[ebx + ecx]
		add al, byte[ebx + edx]
		sub byte[ebx + edx], al
		neg byte[ebx + edx]
		sub al, byte[ebx + edx]
		mov byte[ebx + ecx], al
		cmp ecx, 0
		jg  .lp
.eolp   pop ebx
		mov esp, ebp
		pop ebp
		ret
endoffile:

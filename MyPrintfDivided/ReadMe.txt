Compiling instruction:

nasm -f elf64 -l MyPrintfMain.lst MyPrintfMain.asm && nasm -f elf64 -l MyPrintfFunc.lst MyPrintfFunc.asm && nasm -f elf64 -l MyPrintfProcVar.lst MyPrintfProcVar.asm 
ld -s -o MyPrintf  MyPrintfMain.o MyPrintfFunc.o MyPrintfProcVar.o
./MyPrintf

Functions descriptions:

MyPrintf:

;------------------------------------------------
;Printf, but mine
;------------------------------------------------

;Entry:
;       last pushed value      = pointer on message
;       previous pushed values = values of variables in message
;
;Exit: (none)
;
;Destr: rax, rcx, rdx, rsi, rdi, r8, r9, r11  

;------------------------------------------------

SetUpForSysWrite:

;------------------------------------------------
;Sets up register for syscall write
;------------------------------------------------

;Entry: 
;
;Exit:  rax, rdi for next syscall write 
;                using your rsi and rdx
;                just by typing 'syscall'
;
;Destr: rax, rdi

;------------------------------------------------

--------------------------------------------
;Copies char to writing buffer
;------------------------------------------------

;Entry: cl  = symbol need to be moved
;       rdi = num of writings from buffer
;       r11 = offset inside buffer           
;
;Exit:  rdi = num of writings from buffer
;       r11 = new offser insise buffer
;
;Destr: rax, rbx, rcx, rdx, rdi, r11

;------------------------------------------------

MyPrintfCopySymbolsFromCalculationBuffer:

;------------------------------------------------
;Copies char from calculation buffer to writing buffer
;------------------------------------------------

;Entry: r10 = addres of calculation buffer
;
;Exit:  r10 = addres of first '\0' in calculation buffer
;
;Destr: rax, rbx, rcx, rdx, rdi, r10, r11

;------------------------------------------------

MyPrintfTranslateNumber:

;------------------------------------------------
;Translates number, copies it into calculation buffer
;------------------------------------------------

;Entry: eax = number needed to be transleted
;       cl  = x, when 2^x is base of number system
;       r10 = pointer on calculation buffer
;
;Exit:  r10 = pointer on translated number
;
;Destr: ebx, rdx, r10

;------------------------------------------------

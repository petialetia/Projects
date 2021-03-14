;:================================================
;: My own printf with unique option: 
;;         %b - it prints number in binary format.
;;
;; Use it wisely!!!
;;                              (c)petialetia,2021
;:================================================

; nasm -f elf64 -l MyPrintf.lst MyPrintf.asm && ld -s -o MyPrintf  MyPrintf.o && ./MyPrintf



%macro MyPrintfCopySymbolInWritingBufferFromReg 1

;------------------------------------------------
;Copies char from register to writing buffer
;------------------------------------------------

;Entry: 1 = register nedded to be Copyed
;       rdi = num of writings from buffer
;       r11 = offset inside buffer           
;
;Exit:  rdi = num of writings from buffer
;       r11 = new offser insise buffer
;
;Destr: rax, rbx, rcx, rdx

;------------------------------------------------

        xor rcx, rcx
        mov cl, byte %1
        call MyPrintfCopySymbolInWritingBuffer
        
%endmacro
        
        num_of_args                     equ 13
        buffer_size_power2              equ 9 
        buffer_size                     equ 512                 
        
        calculation_buffer_size         equ 33

section .text

global _start 

_start:     
                        push 15
                        push 31
                        push '!'
                        push 100
                        push 3802
                        push LoveMsg
                        push 15
                        push 31
                        push '!'
                        push 100
                        push 3802
                        push LoveMsg
                        push DedMsg
                        call MyPrintf
                        
                        add rsp, 8 * num_of_args ; "cleaning" stack   
                        
                        push LoveMsg
                        call MyPrintf
                        
                        add rsp, 8
                        
                        push rax
                        push TestMsg
                        call MyPrintf

                        add rsp, 8 * 2
                        
                        push TestMsg1
                        call MyPrintf
                        
                        add rsp, 8
                        
                        push rax
                        push TestMsg2
                        call MyPrintf
                        
                        add rsp, 8*2
                        
                        mov rax, 0x3C      ; exit64 (rdi)
                        xor rdi, rdi
                        syscall

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

                        ;Using registers:
                        ;
                        ;rax - temp values
                        ;rbx - temp values
                        ;rcx - temp values
                        ;rdx - temp values
                        ;rsi - pointer on Writing buffer
                        ;rdi - num of full Writing buffer writings
                        ;r8  - offset inside format string
                        ;r9  - offset of next value in stack
                        ;r10 - pointer on Calculation buffer
                        ;r11 - num of written symbols in writing buffer

                        push rbp
                        mov rbp, rsp
                        
                        push rbx
                        push r10

                        mov r8, [rbp + 8*2]
                        lea r9, [rbp + 8*3]
                        
                        xor rdi, rdi
                        xor r11, r11
                        
                        mov rsi, MyPrintfWritingBuffer
                        mov r10, MyPrintfCalculationBuffer + calculation_buffer_size - 1
                        
MyPrintfLoopBegin:
                        cmp byte [r8], '%'         ; checking for %   
                        je MyPrintfProcessVariable
                        
                        cmp byte [r8], 0           ; checking for end of format string
                        je EndOfMyPrintf   
                                                
                        MyPrintfCopySymbolInWritingBufferFromReg [r8] ; copy symbol
                        
MyPrintfIncrement:  
                        inc r8                     ; moving to the next symbol  
                        jmp MyPrintfLoopBegin
                        

MyPrintfProcessVariable: 
                        inc r8                     ; checking what kind of variable is needed to be writen  
                        
                        cmp byte [r8], 'b'         
                        jb MyPrintfAtypicalParam
                        
                        cmp byte [r8], 'x'
                        ja MyPrintfAtypicalParam
                        
                        xor rcx, rcx
                        
                        mov cl, byte [r8]          ; calculate offset inside switch table
                        sub cl, 'b'
                        shl cl, 3
                        
                        jmp [rcx + MyPrintfTable]  ; jumping to switch table

MyPrintfSymbol:
                        
                        MyPrintfCopySymbolInWritingBufferFromReg [r9] ; copy symbol
                        
                        jmp MyPrintfProcessVariableEnd

MyPrintfString:
                        
                        mov rbx, [r9] ; copy string ptr from stack
                        
MyPrintfStringLoop:
                        
                        cmp byte [rbx], 0          ; copy symbols from string till '\0'
                        je MyPrintfStringExit
                        
                        MyPrintfCopySymbolInWritingBufferFromReg [rbx]
                        
                        inc rbx ; move to next symbol
                        
                        jmp MyPrintfStringLoop
                        
MyPrintfStringExit:                        

                        jmp MyPrintfProcessVariableEnd

MyPrintfInteger:
                        mov eax, dword [r9] ; copy integer from stack
                        
                        mov rcx, 10
                        
MyPrintfIntegerLoop:
                        dec r10             ; r10 is pointer to the end of Calculation buffer  
                                
                        mov byte [r10], "0" 
                         
                        xor rdx, rdx
                        
                        div rcx
                        
                        add byte [r10], dl  ; save digit in Calculation buffer
                        
                        or rax, rax
                        
                        jnz MyPrintfIntegerLoop 
                        
                        call MyPrintfCopySymbolsFromCalculationBuffer                  
                        
                        jmp MyPrintfProcessVariableEnd

MyPrintfOctal:

                        mov cl, 3            ; 2^3 is base of the number system
                        
                        jmp MyPrintfProcessNumber

MyPrintfHexadecimal:

                        mov cl, 4            ; 2^4 is base of the number system

MyPrintfProcessNumber:
                        
                        mov eax, dword [r9]  ; copy integer from stack
                        
                        call MyPrintfTranslateNumber
                        
                        call MyPrintfCopySymbolsFromCalculationBuffer
            
                        jmp MyPrintfProcessVariableEnd

MyPrintfBinary:
                        
                        mov ecx, dword [r9]  ; copy integer from stack
                        
MyPrintfBinaryLoop:                        
                        
                        dec r10              ; r10 is pointer to the end of Calculation buffer
                        
                        mov byte [r10], "0"
                        shr ecx, 1
                        jnb MyPrintfBinaryNoIncrement
                        
                        inc byte [r10]
                        
MyPrintfBinaryNoIncrement:

                        or ecx, ecx
                        jnz MyPrintfBinaryLoop ; translate number till there is ero in ecx
                        
                        call MyPrintfCopySymbolsFromCalculationBuffer
                        
MyPrintfProcessVariableEnd:

                        add r9, 8              ; moving to next arg in stack
                        jmp MyPrintfIncrement
                        
MyPrintfAtypicalParam:

                        dec r8                 ; move to %
                        
                        MyPrintfCopySymbolInWritingBufferFromReg [r8] ; copy %
    
                        inc r8                 ; move to atypucal param
                        
                        cmp byte [r8], '%'
                        je MyPrintfIncrement
                        
                        cmp byte [r8], 0
                        je EndOfMyPrintf
                        
                        MyPrintfCopySymbolInWritingBufferFromReg [r8]
                        
                        jmp MyPrintfIncrement
                        
MyPrintfIncorrectPercent:

                        dec r8                  ; move to %
                        
                        MyPrintfCopySymbolInWritingBufferFromReg [r8] ; copy %
                        
                        inc r8                  ; move to incorrect param
                        
                        MyPrintfCopySymbolInWritingBufferFromReg [r8]  ; copy incorrect param
                        
                        jmp MyPrintfIncrement
                        
EndOfMyPrintf:
                        mov r10, rdi            ; saving num of full writings from Writing buffer
                        
                        call SetUpForSysWrite
                        
                        mov rdx, r11            ; move num of symbols needed to be printed
                        
                        syscall                 ; write64
                        
                        mov rax, r10
                        shl rax, buffer_size_power2
                        add rax, rdx                ; rax is num of printed symbols
                        
                        pop r10
                        pop rbx
                        
                        mov rsp, rbp
                        pop rbp
                        
                        ret

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

                        mov rax, 0x01
                        mov rdi, 1

                        ret
            
MyPrintfCopySymbolInWritingBuffer:

;------------------------------------------------
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
    
                        mov byte [r11 + MyPrintfWritingBuffer], cl ; save symbol in cl to Writing buffer
                                    
                        inc r11                                    ; increase offset inside buffer
                                    
                        cmp r11, buffer_size                       
                        jne MyPrintfCopySymbolInWritingBufferReturn ; checking for buffer overflow
                                
                        mov rbx, rdi                                ; save rdi in rbx
                        mov rdx, r11
                        
                        call SetUpForSysWrite
                                    
                        syscall                     ; sys write64
                                    
                        mov rdi, rbx 
                        inc rdi                     ; increase number of full writes from writing buffer
                        
                        xor r11, r11 ; offset inside buffer after write64
           
MyPrintfCopySymbolInWritingBufferReturn:           
           
                        ret
            
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
            
                        cmp byte [r10], 0 ; checking for '\0'
                        je MyPrintfCopySymbolsFromCalculationBufferExit
                                
                        MyPrintfCopySymbolInWritingBufferFromReg [r10] ; r10 is pointer to the end of Calculation buffer
                                    
                        inc r10
                        
                        jmp MyPrintfCopySymbolsFromCalculationBuffer 
             
MyPrintfCopySymbolsFromCalculationBufferExit:

                        ret                 
            
            
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

                        mov rdx, 1
                        shl rdx, cl
                        dec rdx        ; rdx is needed bit mask
                        
MyPrintfTranslateNumberLoop:

                        dec r10        ; r10 is pointer to the end of Calculation buffer
                        
                        mov ebx, eax   ; copy number
                        and ebx, edx   ; leave only first bits
                        
                        mov bl, byte [ebx + MyPrintfDigitsBuffer] ; copy needed digit
                        
                        mov byte [r10], bl ; save digit
                        
                        shr eax, cl
                        jnz MyPrintfTranslateNumberLoop
                        
                        ret
section .data            
    
MyPrintfTable:

                        dq MyPrintfBinary           ;%b
                        dq MyPrintfSymbol           ;%c
                        dq MyPrintfInteger          ;%d                 
    times 'o' - 'd' - 1 dq MyPrintfIncorrectPercent ; 'e' - ' n'
    
                        dq MyPrintfOctal            ;%o
    times 's' - 'o' - 1 dq MyPrintfIncorrectPercent ; 'p' - 'r'
    
                        dq MyPrintfString           ;%s
    times 'x' - 's' - 1 dq MyPrintfIncorrectPercent ; 't' - 'w'
    
                        dq MyPrintfHexadecimal      ;%x
                        
MyPrintfDigitsBuffer:

    db "0123456789ABCDEF"

MyPrintfWritingBuffer:

    resb buffer_size
        
MyPrintfCalculationBuffer:

    resb calculation_buffer_size             ; for int and '\0'
    
DedMsg:      db "I %s %x%d%%%c%b  %o %~", 0x0a, "0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890!1234!!!abcdefghijklmnopqrstuvwxyz", 0x0a, "I %s %x%d%%%c%b  %o %!", 0x0a, 0

LoveMsg:     db "love", 0

TestMsg:     db " == %d2", 0x0a, 0

TestMsg1:    db "0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890!1234", 0 ;516

TestMsg2:    db " = %d", 0x0a, 0 

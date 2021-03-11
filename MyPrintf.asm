;:================================================
;: 0-Linux-nasm-64.s                   (c)Ded,2012
;:================================================

; nasm -f elf64 -l 1-nasm.lst 1-nasm.s  ;  ld -s -o 1-nasm 1-nasm.o


%macro MyPrintfPullSymbolInWritingBufferFromReg 1

        mov cl, byte [%1]
        call MyPrintfPullSymbolInWritingBuffer
        
%endmacro

section .bss
        
        num_of_args                     equ 13
        buffer_size_in_bytes            equ 9                                          ;2
        buffer_size                     equ 512               ; 2^buffer_size_in_bytes ;4
        max_num_of_writings_from_buffer equ 36028797018963968 ; 2^64 / buffer_size     ;4611686018427387904
        
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
            
            add rsp, 8 * num_of_args        
            
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
;Using registers:
;
;       rax - temp values
;       rcx - temp values
;       rdx - temp values
;       rsi - pointer on Writing bufer
;       rdi - num of full Writing bufer writings
;       r8  - offset inside format string
;       r9  - offset of next value in stack
;       r10 - temp values
;       r11 - num of written symbols in writing buffer
;
;Destr: rsi, rax, rdi, rdx, rcx, r8, r9  

;------------------------------------------------

                        push rbp
                        mov rbp, rsp
                        
                        push rbx

                        mov r8, [rbp + 8*2]
                        lea r9, [rbp + 8*3]
                        
                        xor rdi, rdi
                        xor r11, r11
                        
                        mov rsi, MyPrintfWritingBuffer
                        
MyPrintfLoopBegin:
                        cmp byte [r8], '%'
                        je MyPrintfProcessVariable
                        
                        cmp byte [r8], 0
                        je EndOfMyPrintf   
                                                
                        MyPrintfPullSymbolInWritingBufferFromReg r8
                        
MyPrintfIncrement:  
                        inc r8     
                        jmp MyPrintfLoopBegin
                        

MyPrintfProcessVariable: 
                        inc r8     
                        
                        cmp byte [r8], 'b'
                        jb MyPrintfAtypicalParam
                        
                        cmp byte [r8], 'x'
                        ja MyPrintfAtypicalParam
                        
                        xor rcx, rcx
                        
                        mov cl, byte [r8]
                        sub cl, 'b'
                        shl cl, 3
                        
                        jmp [rcx + MyPrintfTable]

MyPrintfSymbol:
                        
                        MyPrintfPullSymbolInWritingBufferFromReg r9
                        
                        jmp MyPrintfProcessVariableEnd

MyPrintfString:
                        
                        mov r10, [r9]
                        
MyPrintfStringLoop:
                        
                        cmp byte [r10], 0
                        je MyPrintfStringExit
                        
                        MyPrintfPullSymbolInWritingBufferFromReg r10
                        
                        inc r10
                        
                        jmp MyPrintfStringLoop
                        
MyPrintfStringExit:                        

                        jmp MyPrintfProcessVariableEnd

MyPrintfInteger:
                        ;push rdx

                        mov eax, dword [r9]
                        
                        mov rcx, 10
                        
                        mov r10, MyPrintfCalculationBuffer + calculation_buffer_size - 1
                        
MyPrintfIntegerLoop:
                        dec r10        
                                
                        mov byte [r10], "0" 
                         
                        xor rdx, rdx
                        
                        div rcx
                        
                        add byte [r10], dl
                        
                        or rax, rax
                        
                        jnz MyPrintfIntegerLoop
                        
                        call MyPrintfPullSymbolsInWritingBufferFromCalculationBuffer                  
                        
                        jmp MyPrintfProcessVariableEnd

MyPrintfOctal:

                        mov eax, dword [r9]
                        mov r10, MyPrintfCalculationBuffer + calculation_buffer_size - 1
        
                        mov cl, 3
                        
                        call MyPrintfTranslateNumber
                        
                        call MyPrintfPullSymbolsInWritingBufferFromCalculationBuffer                  
                        
                        jmp MyPrintfProcessVariableEnd

MyPrintfHexadecimal:
                        mov eax, dword [r9]
                        mov r10, MyPrintfCalculationBuffer + calculation_buffer_size - 1
                        
                        mov cl, 4
                        
                        call MyPrintfTranslateNumber
                        
                        call MyPrintfPullSymbolsInWritingBufferFromCalculationBuffer
                        

                        jmp MyPrintfProcessVariableEnd

MyPrintfBinary:
                        
                        mov ecx, dword [r9]
                        mov r10, MyPrintfCalculationBuffer + calculation_buffer_size - 1
                        
MyPrintfBinaryLoop:                        
                        
                        dec r10
                        
                        mov byte [r10], "0"
                        shr ecx, 1
                        jnb MyPrintfBinaryNoIncrement
                        
                        inc byte [r10]
                        
MyPrintfBinaryNoIncrement:

                        or ecx, ecx
                        jnz MyPrintfBinaryLoop
                        
                        call MyPrintfPullSymbolsInWritingBufferFromCalculationBuffer
                        
MyPrintfProcessVariableEnd:

                        add r9, 8
                        jmp MyPrintfIncrement
                        
MyPrintfAtypicalParam:

                        dec r8
                        
                        MyPrintfPullSymbolInWritingBufferFromReg r8
                        
                        
                        inc r8
                        
                        cmp byte [r8], '%'
                        je MyPrintfIncrement
                        
                        cmp byte [r8], 0
                        je EndOfMyPrintf
                        
                        MyPrintfPullSymbolInWritingBufferFromReg r8
                        
                        jmp MyPrintfIncrement
                        
MyPrintfIncorrectPercent:

                        dec r8
                        
                        MyPrintfPullSymbolInWritingBufferFromReg r8
                        
                        inc r8
                        
                        MyPrintfPullSymbolInWritingBufferFromReg r8
                        
                        jmp MyPrintfIncrement
                        
EndOfMyPrintf:
                        ;mov rsi, MyPrintfWritingBuffer
                        mov rdi, r10
                        
                        call SetUpForSysWrite
                        
                        mov rdx, r11
                        
                        syscall
                        
                        mov rax, r10
                        shl rax, buffer_size_in_bytes
                        add rax, rdx
                        
                        pop rbx
                        
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
;               just by typing 'syscall'
;
;Destr: rax, rdi

;------------------------------------------------

            mov rax, 0x01
            mov rdi, 1

            ret
            
MyPrintfPullSymbolInWritingBuffer:

;------------------------------------------------
;Pulls char to buffer
;------------------------------------------------

;Entry: rdx = offset inside buffer
;       r9b = symbol need to be moved
;       r10 = num of writings from buffer
;
;Exit:  rdx = new offser insise buffer
;       r10 = num of writings from buffer
;
;Destr: r9, r11, rcx

;------------------------------------------------
    
            mov byte [r11 + MyPrintfWritingBuffer], cl
                        
            inc r11
                        
            cmp r11, buffer_size
            jne MyPrintfPullSymbolInWritingBufferReturn
                    
            mov r10, rdi
            mov rdx, r11
            
            call SetUpForSysWrite
                        
            syscall                     ; sys write64
                        
            mov rdi, r10
            inc rdi
            
            xor r11, r11
           
MyPrintfPullSymbolInWritingBufferReturn:           
           
            ret
            
MyPrintfPullSymbolsInWritingBufferFromCalculationBuffer:

;------------------------------------------------
;Pulls char from calculation buffer to writing buffer
;------------------------------------------------

;Entry: r12 = addres of calculation buffer
;
;Exit:  r12 = addres of first '\0' in calculation buffer
;
;Destr: r9, r11, rcx

;------------------------------------------------
            
            cmp byte [r10], 0
            je MyPrintfPullSymbolsInWritingBufferFromCalculationBufferExit
                    
            MyPrintfPullSymbolInWritingBufferFromReg r10
                        
            inc r10
            
            jmp MyPrintfPullSymbolsInWritingBufferFromCalculationBuffer 
             
MyPrintfPullSymbolsInWritingBufferFromCalculationBufferExit:

            ret
            
MyPrintfTranslateNumber:

;------------------------------------------------
;Translates number, pulls it into calculation buffer
;------------------------------------------------

;Entry: cl = x, when 2^x is base of number system
;
;Exit:  (none)
;
;Destr: r9, r11, r12, r13

;------------------------------------------------

                        mov rdx, 1
                        shl rdx, cl
                        dec rdx
                        
MyPrintfTranslateNumberLoop:

                        dec r10
                        
                        mov ebx, eax
                        and ebx, edx
                        
                        mov bl, byte [ebx + MyPrintfDigitsBuffer]
                        
                        mov byte [r10], bl
                        
                        shr eax, cl
                        jnz MyPrintfTranslateNumberLoop
                        
                        ret

            
section     .data

MyPrintfWritingBuffer:

    resb buffer_size
    
MyPrintfDigitsBuffer:

    db "0123456789ABCDEF"
    
MyPrintfCalculationBuffer:

    resb calculation_buffer_size             ; for int and '\0'


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

DedMsg:      db "I %s %x%d%%%c%b  %o %~", 0x0a, "0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890!1234!!!abcdefghijklmnopqrstuvwxyz", 0x0a, "I %s %x%d%%%c%b  %o %~", 0x0a, 0

LoveMsg:     db "love", 0

;:================================================
;: 0-Linux-nasm-64.s                   (c)Ded,2012
;:================================================

; nasm -f elf64 -l 1-nasm.lst 1-nasm.s  ;  ld -s -o 1-nasm 1-nasm.o


%macro MyPrintfPullSymbolInWritingBufferFromReg 1

        mov r9b, byte [%1]
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
;       r10 == 0 (must be)
;       rdx == 0 (must be)
;
;Exit: (none)
;
;Destr: rsi, rax, rdi, rdx, rcx, r8, r9  

;------------------------------------------------

                        push rbp
                        mov rbp, rsp
                        
                        push r12
                        push r13

                        mov rsi, [rbp + 8*2]
                    
                        mov r8, rbp                 ; r8 offset of current variable inside stack
                        add r8, 3*8

                        call SetUpForSysWrite
                        
                        ;xor r10, r10                ; r10 = num of full buffer writings
                        ;xor rdx, rdx                ; rdx = num of written symbols in buffer
                        
MyPrintfLoopBegin:
                        cmp byte [rsi], '%'
                        je MyPrintfProcessVariable
                        
                        cmp byte [rsi], 0
                        je EndOfMyPrintf   
                                                
                        MyPrintfPullSymbolInWritingBufferFromReg rsi
                        
MyPrintfIncrement:  
                        inc rsi     
                        jmp MyPrintfLoopBegin
                        

MyPrintfProcessVariable: 
                        inc rsi     
                        
                        cmp byte [rsi], 'b'
                        jb MyPrintfAtypicalParam
                        
                        cmp byte [rsi], 'x'
                        ja MyPrintfAtypicalParam
                        
                        xor r9, r9
                        
                        mov r9b, byte [rsi]
                        sub r9, 'b'
                        shl r9, 3
                        
                        jmp [r9 + MyPrintfTable]

MyPrintfSymbol:
                        
                        MyPrintfPullSymbolInWritingBufferFromReg r8
                        
                        jmp MyPrintfProcessVariableEnd

MyPrintfString:
                        
                        mov r12, [r8]
                        
MyPrintfStringLoop:
                        
                        cmp byte [r12], 0
                        je MyPrintfStringExit
                        
                        MyPrintfPullSymbolInWritingBufferFromReg r12
                        
                        inc r12
                        
                        jmp MyPrintfStringLoop
                        
MyPrintfStringExit:                        

                        jmp MyPrintfProcessVariableEnd

MyPrintfInteger:
                        push rdx

                        mov eax, dword [r8]
                        
                        mov rcx, 10
                        
                        mov r12, MyPrintfCalculationBuffer + calculation_buffer_size - 1
                        
MyPrintfIntegerLoop:
                        dec r12        
                                
                        mov byte [r12], "0" 
                         
                        xor rdx, rdx
                        
                        div rcx
                        
                        add byte [r12], dl
                        
                        or rax, rax
                        
                        jnz MyPrintfIntegerLoop
                        
                        pop rdx
                        
                        call SetUpForSysWrite
                        
                        call MyPrintfPullSymbolsInWritingBufferFromCalculationBuffer                  
                        
                        jmp MyPrintfProcessVariableEnd

MyPrintfOctal:

                        mov r9d, dword [r8]
                        mov r12, MyPrintfCalculationBuffer + calculation_buffer_size - 1
        
                        mov cl, 3
                        
                        call MyPrintfTranslateNumber
                        
                        call MyPrintfPullSymbolsInWritingBufferFromCalculationBuffer                  
                        
                        jmp MyPrintfProcessVariableEnd

MyPrintfHexadecimal:
                        mov r9d, dword [r8]
                        mov r12, MyPrintfCalculationBuffer + calculation_buffer_size - 1
                        
                        mov cl, 4
                        
                        call MyPrintfTranslateNumber
                        
                        call MyPrintfPullSymbolsInWritingBufferFromCalculationBuffer
                        

                        jmp MyPrintfProcessVariableEnd

MyPrintfBinary:
                        
                        mov r9d, dword [r8]
                        mov r12, MyPrintfCalculationBuffer + calculation_buffer_size - 1
                        
MyPrintfBinaryLoop:                        
                        
                        dec r12
                        
                        mov byte [r12], "0"
                        shr r9, 1
                        jnb MyPrintfBinaryNoIncrement
                        
                        inc byte [r12]
                        
MyPrintfBinaryNoIncrement:

                        or r9, r9
                        jnz MyPrintfBinaryLoop
                        
                        call MyPrintfPullSymbolsInWritingBufferFromCalculationBuffer
                        
MyPrintfProcessVariableEnd:

                        add r8, 8
                        jmp MyPrintfIncrement
                        
MyPrintfAtypicalParam:

                        dec rsi
                        
                        MyPrintfPullSymbolInWritingBufferFromReg rsi
                        
                        
                        inc rsi
                        
                        cmp byte [rsi], '%'
                        je MyPrintfIncrement
                        
                        cmp byte [rsi], 0
                        je EndOfMyPrintf
                        
                        MyPrintfPullSymbolInWritingBufferFromReg rsi
                        
                        jmp MyPrintfIncrement
                        
MyPrintfIncorrectPercent:

                        dec rsi
                        
                        MyPrintfPullSymbolInWritingBufferFromReg rsi
                        
                        inc rsi
                        
                        MyPrintfPullSymbolInWritingBufferFromReg rsi
                        
                        jmp MyPrintfIncrement
                        
EndOfMyPrintf:
                        mov rsi, MyPrintfWritingBuffer
                        syscall
                        mov rax, r10
                        shl rax, buffer_size_in_bytes
                        add rax, rdx
                        
                        pop r13
                        pop r12
                        
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
    
            mov byte [rdx + MyPrintfWritingBuffer], r9b
                        
            inc rdx
                        
            cmp rdx, buffer_size
            jne MyPrintfPullSymbolInWritingBufferReturn
                    
            mov r9, rsi
            mov rsi, MyPrintfWritingBuffer
                        
            syscall                     ; sys write64
                        
            mov rsi, r9                        
            xor rdx, rdx
            inc r10
            mov rax, 0x01
           
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
            
            cmp byte [r12], 0
            je MyPrintfPullSymbolsInWritingBufferFromCalculationBufferExit
                    
            MyPrintfPullSymbolInWritingBufferFromReg r12
                        
            inc r12
            
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

                        mov r11, 1
                        shl r11, cl
                        dec r11
                        
MyPrintfTranslateNumberLoop:

                        dec r12
                        
                        mov r13d, r9d
                        and r13d, r11d
                        
                        mov r13b, byte [r13d + MyPrintfDigitsBuffer]
                        
                        mov byte [r12], r13b
                        
                        shr r9, cl
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

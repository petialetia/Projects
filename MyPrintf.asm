;:================================================
;: 0-Linux-nasm-64.s                   (c)Ded,2012
;:================================================

; nasm -f elf64 -l 1-nasm.lst 1-nasm.s  ;  ld -s -o 1-nasm 1-nasm.o


%macro MyPrintfPullSymbolInBufferFromReg 1

        mov r9b, byte [%1]
        call MyPrintfPullSymbolInBuffer
        
%endmacro

section .bss
        
        num_of_args                     equ 6
        buffer_size_in_bytes            equ 9                                          ;2
        buffer_size                     equ 512               ; 2^buffer_size_in_bytes ;4
        max_num_of_writings_from_buffer equ 36028797018963968 ; 2^64 / buffer_size     ;4611686018427387904

section .text

global _start            

_start:     
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
                        
                        ;mov rsi

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
                        
                        ;call MyPrintfPullSymbolInBuffer
                        
                        MyPrintfPullSymbolInBufferFromReg rsi
                        
MyPrintfIncrement:  
                        inc rsi     
                        jmp MyPrintfLoopBegin
                        

MyPrintfProcessVariable: 
                        inc rsi     
                        
                        cmp byte [rsi], 'b'
                        jb MyPrintfAtypicalParam
                        
                        cmp byte [rsi], 'x'
                        ja MyPrintfAtypicalParam
                        
                        mov r9b, byte [rsi]
                        sub r9, 'b'
                        shl r9, 3
                        
                        jmp [r9 + MyPrintfTable]

MyPrintfSymbol:
                        
                        MyPrintfPullSymbolInBufferFromReg r8
                        
                        jmp MyPrintfProcessVariableEnd

MyPrintfString:
                        
                        mov r12, [r8]
                        
MyPrintfStringLoop:
                        
                        cmp byte [r12], 0
                        je MyPrintfStringExit
                        
                        MyPrintfPullSymbolInBufferFromReg r12
                        
                        inc r12
                        
                        jmp MyPrintfStringLoop
                        
MyPrintfStringExit:                        

                        jmp MyPrintfProcessVariableEnd

MyPrintfInteger:
                        push rdx

                        mov eax, dword [r8]
                        
                        mov rcx, 10
                        
                        mov r12, MyPrintfCalculationBuffer + 32
                        
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
                        
                        call MyPrintfPullSymbolsInBufferFromCalculationBuffer                  
                        
                        jmp MyPrintfProcessVariableEnd

MyPrintfOctal:

                        mov r9d, dword [r8]
                        mov r12, MyPrintfCalculationBuffer + 32
                        
MyPrintfOctalLoop:
                        
                        dec r12
                        
                        mov byte [r12], "0"
                        
                        mov ecx, r9d
                        and ecx, 111b
                        
                        add byte [r12], cl
                        
                        shr r9, 3
                        jnz MyPrintfOctalLoop
                        
                        call MyPrintfPullSymbolsInBufferFromCalculationBuffer                  
                        
                        jmp MyPrintfProcessVariableEnd

MyPrintfHexadecimal:
                        mov r9d, dword [r8]
                        mov r12, MyPrintfCalculationBuffer + 32
                        
MyPrintfHexadecimalLoop:
                        
                        dec r12
                        
                        mov byte [r12], "0"
                        
                        mov ecx, r9d
                        and ecx, 1111b
                        
                        cmp cl, 9
                        jna MyPrintfHexadecimalDigit
                        
                        add byte [r12], "a" - "9" - 1
                        
MyPrintfHexadecimalDigit:
                        
                        add byte [r12], cl
                        
                        shr r9, 4
                        jnz MyPrintfHexadecimalLoop
                        
                        call MyPrintfPullSymbolsInBufferFromCalculationBuffer
                        

                        jmp MyPrintfProcessVariableEnd

MyPrintfBinary:
                        
                        mov r9d, dword [r8]
                        mov r12, MyPrintfCalculationBuffer + 32
                        
MyPrintfBinaryLoop:                        
                        
                        dec r12
                        
                        mov byte [r12], "0"
                        shr r9, 1
                        jnb MyPrintfBinaryNoIncrement
                        
                        inc byte [r12]
                        
MyPrintfBinaryNoIncrement:

                        or r9, r9
                        jnz MyPrintfBinaryLoop
                        
                        ;cmp byte [r12], 0
                        ;je MyPrintfProcessVariableEnd
                        
                        ;MyPrintfPullSymbolInBufferFromReg r12
                        
                        ;inc r12
                        
                        ;jmp MyPrintfBinaryPrintLoop
                        
                        call MyPrintfPullSymbolsInBufferFromCalculationBuffer
                        
MyPrintfProcessVariableEnd:

                        add r8, 8
                        jmp MyPrintfIncrement
                        
MyPrintfAtypicalParam:

                        dec rsi
                        
                        ;call MyPrintfPullSymbolInBuffer
                        MyPrintfPullSymbolInBufferFromReg rsi
                        
                        
                        inc rsi
                        
                        cmp byte [rsi], '%'
                        je MyPrintfIncrement
                        
                        cmp byte [rsi], 0
                        je EndOfMyPrintf
                        
                        ;call MyPrintfPullSymbolInBuffer
                        
                        MyPrintfPullSymbolInBufferFromReg rsi
                        
                        jmp MyPrintfIncrement
                        
MyPrintfIncorrectPercent:

                        dec rsi
                        
                        MyPrintfPullSymbolInBufferFromReg rsi
                        
                        inc rsi
                        
                        MyPrintfPullSymbolInBufferFromReg rsi
                        
                        jmp MyPrintfIncrement
                        
EndOfMyPrintf:
                        mov rsi, MyPrintfBuffer
                        syscall
                        mov rax, r10
                        shl rax, buffer_size_in_bytes
                        add rax, rdx
                        
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
            
MyPrintfPullSymbolInBuffer:
    
            mov byte [rdx + MyPrintfBuffer], r9b
                        
            inc rdx
                        
            cmp rdx, buffer_size
            jne MyPrintfPullSymbolInBufferReturn
                    
            mov r9, rsi
            mov rsi, MyPrintfBuffer
                        
            syscall                     ; sys write64
                        
            mov rsi, r9                        
            xor rdx, rdx
            inc r10
            mov rax, 0x01
           
MyPrintfPullSymbolInBufferReturn:           
           
            ret
            
MyPrintfPullSymbolsInBufferFromCalculationBuffer:
            
            cmp byte [r12], 0
            je MyPrintfPullSymbolsInBufferFromCalculationBufferExit
                    
            MyPrintfPullSymbolInBufferFromReg r12
                        
            inc r12
            
            jmp MyPrintfPullSymbolsInBufferFromCalculationBuffer 
             
MyPrintfPullSymbolsInBufferFromCalculationBufferExit:

            ret

            
section     .data

MyPrintfBuffer:

    resb buffer_size
    
MyPrintfCalculationBuffer:

    resb 33             ; for int and '\0'


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
    times '~' - 'x' + 1 dq MyPrintfIncorrectPercent ; 'y' - 127
            
;TestMsg:        db "0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890!1234!!!abcdefghijklmnopqrstuvwxyz", 0x0a, 0

;Msg:        db "%d == %b == %o == %x", 0x0a, 0

DedMsg:      db "I %s %x%d%%%c%b", 0x0a, 0

LoveMsg:     db "love", 0

;Petya:      db "Petya", 0

;Molodec:    db "Molodec", 0

;TestMsg:    db "Privet", 0

;01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234
;012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890!

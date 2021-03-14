;:================================================
;: My own printf with unique option: 
;;         %b - it prints number in binary format.
;;
;; Use it wisely!!!
;;                              (c)petialetia,2021
;:================================================

extern MyPrintfAtypicalParam
extern MyPrintfTable

global MyPrintf
global MyPrintfCopySymbolInWritingBuffer
global MyPrintfCopySymbolsFromCalculationBuffer
global MyPrintfIncrement
global EndOfMyPrintf
global MyPrintfIncrement
global MyPrintfDigitsBuffer

%macro MyPrintfCopySymbolInWritingBufferFrom 1

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

        buffer_size_power2              equ 9 
        buffer_size                     equ 512                 
        calculation_buffer_size         equ 33

section .text

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
                                                
                        MyPrintfCopySymbolInWritingBufferFrom [r8] ; copy symbol
                        
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
                                
                        MyPrintfCopySymbolInWritingBufferFrom [r10] ; r10 is pointer to the end of Calculation buffer
                                    
                        inc r10
                        
                        jmp MyPrintfCopySymbolsFromCalculationBuffer 
             
MyPrintfCopySymbolsFromCalculationBufferExit:

                        ret                 
       
section .data            
                        
MyPrintfDigitsBuffer:

    db "0123456789ABCDEF"

MyPrintfWritingBuffer:

    resb buffer_size
        
MyPrintfCalculationBuffer:

    resb calculation_buffer_size             ; for int and '\0'
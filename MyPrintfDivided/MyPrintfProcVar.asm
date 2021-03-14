extern MyPrintfCopySymbolInWritingBuffer
extern MyPrintfCopySymbolsFromCalculationBuffer
extern MyPrintfIncrement
extern EndOfMyPrintf
extern MyPrintfIncrement
extern MyPrintfDigitsBuffer

global MyPrintfTable
global MyPrintfAtypicalParam

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

section .text

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
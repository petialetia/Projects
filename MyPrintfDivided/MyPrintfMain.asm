; nasm -f elf64 -l MyPrintf.lst MyPrintf.asm && ld -s -o MyPrintf  MyPrintf.o && ./MyPrintf
        
extern MyPrintf

global _start 

        num_of_args                     equ 13

section .text

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
    
DedMsg:      db "I %s %x%d%%%c%b  %o %~", 0x0a, "0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890!1234!!!abcdefghijklmnopqrstuvwxyz", 0x0a, "I %s %x%d%%%c%b  %o %!", 0x0a, 0

LoveMsg:     db "love", 0

TestMsg:     db " == %d2", 0x0a, 0

TestMsg1:    db "0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890!1234", 0 ;516

TestMsg2:    db " = %d", 0x0a, 0 

; nasm -f elf64 -l PrintfFromAsm.lst PrintfFromAsm.asm && gcc -no-pie -o PrintfFromAsm PrintfFromAsm.o && ./PrintfFromAsm

global main
extern printf

segment .text

main:

        mov rdi, FormatMsg
        mov rsi, LoveMsg
        mov rdx, 3802
        mov rcx, 100
        mov r8,  '!'

        xor rax, rax
        call printf

        mov rax, 0x3c
        xor rdi, rdi
        syscall

segment .data

FormatMsg: db "I %s %x%d%%%c", 0x0a, 0

LoveMsg:   db "love", 0

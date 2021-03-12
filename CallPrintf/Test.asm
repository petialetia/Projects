; nasm -f elf64 -l Test.lst Test.asm ; gcc -no-pie -o Test Test.o ; ./Test
; Get your "Segmentation fault (core dumped)""

global main
extern printf

segment .text

main:

        mov rdi, FormatMsg
        mov rsi, LoveMsg
        mov rdx, 3802
        mov rcx, 100
        mov r8,  '!'
        mov r9,  31
        push 15
        push LoveMsg
        push 3802
        push 100
        push '!'
        push 31
        push 15

        xor rax, rax
        call printf

        mov rsp, 8*7

        mov rax, 0x3c
        xor rdi, rdi
        syscall

segment .data

FormatMsg: db "I %s %x%d%%%c%b  %o %~", 0x0a, "0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890!1234!!!abcdefghijklmnopqrstuvwxyz", 0x0a, "I %s %x%d%%%c%b  %o %~", 0x0a, 0

LoveMsg:   db "love", 0
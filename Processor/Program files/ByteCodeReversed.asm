in
pop rax
in
pop rbx
in
pop rcx
push rax
push 0.000000
je 25
jmp 105
hlt
push rbx
push 0.000000
je 66
push 1.000000
out
push -1.000000
push rcx
mul
push rbx
div
out
jmp 24
push rcx
push 0.000000
je 81
jmp 93
push 8.000000
out
jmp 24
push 0.000000
out
jmp 24
push rbx
push 2.000000
pow
push rax
push rcx
mul
push 4.000000
mul
sub
pop rdx
push rdx
push 0.000000
jb 93
push rdx
push 0.000000
je 163
jmp 201
push 1.000000
out
push rbx
push -1.000000
mul
push rax
div
push 2.000000
div
out
jmp 24
push 2.000000
out
push rbx
push -1.000000
mul
push rdx
sqrt
add
push rax
div
push 2.000000
div
out
push rbx
push -1.000000
mul
push rdx
sqrt
sub
push rax
div
push 2.000000
div
out
jmp 24

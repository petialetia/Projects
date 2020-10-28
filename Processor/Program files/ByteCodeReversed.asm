in
pop rax
push 1.000000
pop rbx
push 0.000000
pop rcx
call 31
push rbx
out
hlt
push rcx
push 1.000000
add
pop rcx
push rbx
push rcx
mul
pop rbx
push rcx
push rax
jb 60
jmp 62
call 31
ret

in
pop rax
push 1
pop rbx
push 0
pop rcx
call FUNC
push rbx
out
hlt

FUNC:
push rcx
push 1
add
pop rcx
push rbx
push rcx
mul
pop rbx

push rcx
push rax
jb :1
jmp :2
1:
call FUNC
2:
ret
in
pop rax
in
pop rbx
in
pop rcx
push rax
push 0
je :Linear
jmp :FindDiskr

HLT:
hlt

Linear:
push rbx
push 0
je :OnlyC
push 1
out
push -1
push rcx
mul
push rbx
div
out
jmp :HLT


OnlyC:
push rcx
push 0
je :InfinitRoots
jmp :NoRoots

InfinitRoots:
push 8
out
jmp :HLT

NoRoots:
push 0
out
jmp :HLT

FindDiskr:
push rbx
push 2
pow
push rax
push rcx
mul
push 4
mul
sub
pop rdx
push rdx
push 0
jb :NoRoots
push rdx
push 0
je :OneRoot
jmp :TwoRoots

OneRoot:
push 1
out
push rbx
push -1
mul
push rax
div
push 2
div
out
jmp :HLT

TwoRoots:
push 2
out
push rbx
push -1
mul
push rdx
sqrt
add
push rax
div
push 2
div
out

push rbx
push -1
mul
push rdx
sqrt
sub
push rax
div
push 2
div
out
jmp :HLT
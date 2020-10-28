List of commands:

"push <value>" or "push <register>"
Push <value> or value in <register> in stack

"pop <register>"
Pop value, copy it into <register>

"hlt"
Finish work

"in"
Push value, entered by user

"out"
Pop value and print it on the screen

"add"
Pop 2 top values in stack and push summary of them

"sub"
Pop 2 top values in stack and push difference of them

"mul"
Pop 2 top values in stack and push product of them

"div"
Pop 2 top values in stack and push quotient of them

"pow"
Pop 2 top values in stack and push second poped value powered to the first poped value

"sqrt"
Pop top value in stack and push square roor of it

"<label>:"
Create a label with name <label>

"jmp :<label>" or "jmp <offset>"
Jump to the <label> or to the value of <offset>

"ja :<label>" or "jmp <offset>"
Pop 2 top values in stack, jump to the <label> or to the value of <offset> if second poped value above then first poped value

"jae :<label>" or "jmp <offset>"
Pop 2 top values in stack, jump to the <label> or to the value of <offset> if second poped value above then first poped value or equal to it

"jb :<label>" or "jmp <offset>"
Pop 2 top values in stack, jump to the <label> or to the value of <offset> if second poped value below then first poped value

"jbe :<label>" or "jmp <offset>"
Pop 2 top values in stack, jump to the <label> or to the value of <offset> if second poped value below then first poped value or equal to it

"je :<label>" or "jmp <offset>"
Pop 2 top values in stack, jump to the <label> or to the value of <offset> if second poped value equal to first poped value

"jne :<label>" or "jmp <offset>"
Pop 2 top values in stack, jump to the <label> or to the value of <offset> if second poped value not equal to first poped value

"call <label>" or "call <offset>"
Push current offset to the stack of calls, jump to the <label> or to the value of <offset>  

"ret"
Jump to the poped offset in stack of calls
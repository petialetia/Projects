DrawSymbolRectangle:

;------------------------------------------------
;
;Draws rectangle by your coordinates, fillings edges with your symbols
;
;	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
;	Beware: there's no coordinates validation
;		take responsibility for yourself
;	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
;
;------------------------------------------------
;
;Entry: si = coordinate of left upper corner
;	bh = length of rectangle (zero is unexcaptable)
;	bl = width of rectangle (zero is unexceptable)
;	cl = color of edges
;	ch = ASCII code of symbol of edge
;	es = b800h - videoram ptr
;
;Const: screen_length
;
;Exit:  di = coordinate of right low corner
;
;Destr: di, dh
;
;------------------------------------------------

	mov dh, bh
	mov di, si
	call DrawHorizontalLine   ; DrawHorizontalLine (di, dh, cl, ch, es)

	mov dh, bl
	sub dh, 1 * 2             ; one of the corner pixels is already drawn, another will be drawn later
	add di, 2 * screen_length

	call DrawVerticalLine     ; DrawVerticalLine (di, dh, cl, ch, es)

	mov dh, bl
	dec dh                    ; one of the corner pixels is already drawn
	mov di, si
	add di, 2 * screen_length

	call DrawVerticalLine     ; DrawVerticalLine (di, dh, cl, ch, es)

	mov dh, bh
	dec dh                    ; 1 corner pixel is already drawn
	add di, 1 * 2

	call DrawHorizontalLine   ; DrawHorizontalLine (di, dh, cl, ch, es)

	ret

DrawRectangle:

;------------------------------------------------
;
;Draws rectangle by your coordinates (doesn't spoil symbols)
;
;	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
;	Beware: there's no coordinates validation
;		take responsibility for yourself
;	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
;
;------------------------------------------------
;
;Entry: si = coordinate of left upper corner
;	bh = length of rectangle (zero is unexcaptable)
;	bl = width of rectangle (zero is unexceptable)
;	cl = color of edges
;	es = b800h - videoram ptr
;
;Const: screen_length
;
;Exit:  di = coordinate of right low corner
;
;Destr: di, dh
;
;------------------------------------------------

	mov dh, bh
	mov di, si
	call DrawOverHorizontalLine   ; DrawHorizontalLine (di, dh, cl, ch, es)

	mov dh, bl
	sub dh, 1 * 2             ; one of the corner pixels is already drawn, another will be drawn later
	add di, 2 * screen_length

	call DrawOverVerticalLine     ; DrawVerticalLine (di, dh, cl, ch, es)

	mov dh, bl
	dec dh                    ; one of the corner pixels is already drawn
	mov di, si
	add di, 2 * screen_length

	call DrawOverVerticalLine     ; DrawVerticalLine (di, dh, cl, ch, es)

	mov dh, bh
	dec dh                    ; 1 corner pixel is already drawn
	add di, 1 * 2

	call DrawOverHorizontalLine   ; DrawHorizontalLine (di, dh, cl, ch, es)

	ret


DrawHorizontalLine:

;------------------------------------------------
;
;Draws horizontal line by your coordinates
;
;	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
;	Beware: there's no coordinates validation
;		take responsibility for yourself
;	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
;
;------------------------------------------------
;
;Entry: di = coordinate of left end
;	dh = length (zero is unexceptable)
;	cl = color of edge
;	ch = ASCII code of symbol of edge
;	es = b800h - videoram ptr
;
;Exit:  di = coordinate of rigth end
;
;Destr: di, dh
;
;------------------------------------------------

	DrawHorLoop:

		mov byte ptr es:[di], ch   ;print part of the edge
		inc di
		mov byte ptr es:[di], cl   ;print color of the edge
		inc di                     ;moving to next pixel on the right

		dec dh
		je ExitDrawHorLoop

		jmp DrawHorLoop

	ExitDrawHorLoop:

		sub di, 1 * 2              ; coming back on 1 pixel to the left

	 	ret

DrawVerticalLine:

;------------------------------------------------
;
;Draws vertical line by your coordinates
;
;	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
;	Beware: there's no coordinates validation
;		take responsibility for yourself
;	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
;
;------------------------------------------------
;
;Entry: di = coordinate of high end
;	dh = length (zero is unexceptable)
;	cl = color of edge
;	ch = ASCII code of symbol of edge
;	es = b800h - videoram ptr
;
;Exit:  di = coordinate of low end
;
;Destr: di, dh
;
;------------------------------------------------

	DrawVerLoop:

		mov byte ptr es:[di], ch      ; print part of the edge
		inc di
		mov byte ptr es:[di], cl      ; print color of the edge
		add di, 2 * screen_length - 1 ; moving to next pixel down

		dec dh
		je ExitDrawVerLoop

		jmp DrawVerLoop

	ExitDrawVerLoop:

		sub di, 2 * screen_length     ; coming back on 1 pixel to the up

	 	ret

FillRectangle:

;------------------------------------------------
;
;Filles rectangle by your coordinates with symbols
;
;	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
;	Beware: there's no coordinates validation
;		take responsibility for yourself
;	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
;
;------------------------------------------------
;
;Entry: si = coordinate of left upper corner
;	bh = length of rectangle (zero is unexcaptable)
;	bl = width of rectangle (zero is unexceptable)
;	ch = ASCII code of symbol
;       cl = color of edges
;	es = b800h - videoram ptr
;	ah = 00h (it must be so)
;
;Exit:  di = coordinate of right low corner
;
;Destr: di, bl, dh, dl, al
;
;------------------------------------------------

	mov di, si        ; di = coordinate
	mov dh, bh        ; dh = length
	mov dl, bl	  ; dl = width

	mov al, screen_length * 2 + 1 * 2
	shl dh, 1
	sub al, dh        ; al = offset from right corner of one line to left corner of next line
	shr dh, 1

FillRectangleLoop:

	call DrawHorizontalLine

	dec dl
	je ExitFillRectangleLoop

	add di, ax
	mov dh, bh

	jmp FillRectangleLoop

ExitFillRectangleLoop:

	ret

DrawOverRectangle:

;------------------------------------------------
;
;Draws over rectangle by your coordinates (does not spoil symbols)
;
;	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
;	Beware: there's no coordinates validation
;		take responsibility for yourself
;	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
;
;------------------------------------------------
;
;Entry: si = coordinate of left upper corner
;	bh = length of rectangle (zero is unexcaptable)
;	bl = width of rectangle (zero is unexceptable)
;       cl = color of edges
;	es = b800h - videoram ptr
;	ah = 00h (it must be so)
;
;Exit:  di = coordinate of right low corner
;
;Destr: di, bl, dh, dl, ch
;
;------------------------------------------------

	mov di, si        ; di = coordinate
	mov dh, bh        ; dh = length
	mov dl, bl	  ; dl = width

	mov al, screen_length * 2 + 1 * 2
	shl dh, 1
	sub al, dh        ; al = offset from right corner of one line to left corner of next line
	shr dh, 1

DrawOverRectangleLoop:

	call DrawOverHorizontalLine

	dec dl
	je ExitDrawOverRectangleLoop

	add di, ax
	mov dh, bh

	jmp DrawOverRectangleLoop

ExitDrawOverRectangleLoop:

	ret


DrawOverHorizontalLine:

;------------------------------------------------
;
;Draws over horizontal line by your coordinates (doesn't spoils symbols)
;
;	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
;	Beware: there's no coordinates validation
;		take responsibility for yourself
;	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
;
;------------------------------------------------
;
;Entry: di = coordinate of left end
;	dh = length (zero is unexceptable)
;	cl = color of edge
;	es = b800h - videoram ptr
;
;Exit:  di = coordinate of rigth end
;
;Destr: di, dh
;
;------------------------------------------------

		inc di

	DrawOverHorLoop:

		mov byte ptr es:[di], cl   ; print color of the edge
		add di, 1 * 2              ; moving to next pixel on the right

		dec dh
		je ExitDrawOverHorLoop

		jmp DrawOverHorLoop

	ExitDrawOverHorLoop:

		sub di, 1 * 2 + 1          ; coming back on 1 pixel to the left

	 	ret



DrawOverVerticalLine:

;------------------------------------------------
;
;Draws vertical line by your coordinates (doesn't spoils symbols)
;
;	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
;	Beware: there's no coordinates validation
;		take responsibility for yourself
;	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
;
;------------------------------------------------
;
;Entry: di = coordinate of high end
;	dh = length (zero is unexceptable)
;	cl = color of edge
;	es = b800h - videoram ptr
;
;Exit:  di = coordinate of low end
;
;Destr: di, dh
;
;------------------------------------------------

		inc di

	DrawOverVerLoop:

		mov byte ptr es:[di], cl  ; print color of the edge
		add di, 2 * screen_length ; moving to next pixel down

		dec dh
		je ExitDrawOverVerLoop

		jmp DrawOverVerLoop

	ExitDrawOverVerLoop:

		sub di, 2 * screen_length + 1 ; coming back on 1 pixel to the up		

	 	ret

DrawLowerRightCorner:

;------------------------------------------------
;
;Draws right lower corner by your coordinates
;
;	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
;	Beware: there's no coordinates validation
;		take responsibility for yourself
;	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
;
;------------------------------------------------
;
;Entry: di = coordinate of left upper corner
;	bh = length of horizontal ray (zero is unexcaptable)
;	bl = length of vertical ray (zero is unexceptable)
;	cl = color of lines
;	ch = ASCII code of symbol of lines
;	es = b800h - videoram ptr
;
;Exit:  (none)
;
;Destr: si, dh
;
;------------------------------------------------

	mov dh, bl
	mov si, di
	call DrawVerticalLineBackwards

   	mov dh, bh
    	mov si, di

    	sub si, 1 * 2 ; corner pixel already colored
    	dec dh

    	call DrawHorizontalLineBackwards

	ret


DrawHorizontalLineBackwards:

;------------------------------------------------
;
;Draws horizontal line by your coordinates backwards
;
;	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
;	Beware: there's no coordinates validation
;		take responsibility for yourself
;	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
;
;------------------------------------------------
;
;Entry: si = coordinate of high end
;	dh = length (zero is unexceptable)
;	cl = color of edge
;	ch = ASCII code of symbol of edge
;	es = b800h - videoram ptr
;
;Exit:  si = coordinate of left end
;
;Destr: si, dh
;
;------------------------------------------------

    	DrawHorBackLoop:

		mov byte ptr es:[si], ch   ;print part of the edge
		inc si
		mov byte ptr es:[si], cl   ;print color of the edge
		sub si, 1 + 1 * 2          ;moving to next pixel on the left

		dec dh

		je ExitDrawHorBackLoop

		jmp DrawHorBackLoop

	ExitDrawHorBackLoop:

		add si, 1 * 2              ;coming forth on 1 pixel to the right

	 	ret

DrawVerticalLineBackwards:

;------------------------------------------------
;
;Draws vertical line by your coordinates backwards
;
;	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
;	Beware: there's no coordinates validation
;		take responsibility for yourself
;	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
;
;------------------------------------------------
;
;Entry: si = coordinate of high end
;	dh = length (zero is unexceptable)
;	cl = color of edge
;	ch = ASCII code of symbol of edge
;	es = b800h - videoram ptr
;
;Exit:  si = coordinate of upper end
;
;Destr: si, dh
;
;------------------------------------------------

	DrawVerBackLoop:

		mov byte ptr es:[si], ch      ; print part of the edge
		inc si
		mov byte ptr es:[si], cl      ; print color of the edge
		sub si, 1 + 2 * screen_length ; moving to next pixel up

		dec dh
		je ExitDrawVerBackLoop

		jmp DrawVerBackLoop

	ExitDrawVerBackLoop:

		sub si, 2 * screen_length     ; coming back on 1 pixel to the down

	 	ret


WriteDecimalFigure:

;------------------------------------------------
;
;Writes decimal figure by your coordinates
;
;------------------------------------------------
;
;Entry: di = coordinate
;	ch = figure
;	cl = color
;	es = b800h - videoram ptr
;
;Exit:  (none)
;
;Destr: ch
;
;------------------------------------------------

    add ch, '0'
    mov byte ptr es:[di], ch
    inc di
    mov byte ptr es:[di], cl
    dec di

    ret


WriteHexadecimalFigure:

;------------------------------------------------
;
;Writes hexadecimal figure by your coordinates
;
;------------------------------------------------
;
;Entry: di = coordinate
;	ch = figure
;	cl = color
;	es = b800h - videoram ptr
;
;Exit:  (none)
;
;Destr: ch
;
;------------------------------------------------

    add ch, '0'

    cmp ch, ':'            ; ':' is fisrt not figure

    jb WritingOfHexadecimalFigure

    add ch, 7

WritingOfHexadecimalFigure:

    mov byte ptr es:[di], ch
    inc di
    mov byte ptr es:[di], cl
    dec di

    ret


WriteBinaryNumber:

;------------------------------------------------
;
;Writes binary number by your coordinates
;
;------------------------------------------------
;
;Entry: si = coordinate
;  	ax = number
;	es = b800h - videoram ptr
;	cl = color
;
;Exit:  (none)
;
;Destr: bx, ch, di
;
;------------------------------------------------


        mov bx, ax
	mov di, si

WriteBinaryNumberLoopBegin:

	xor ch, ch

        shr bx, 1
        jnb PrintBinaryFigure

        inc ch

PrintBinaryFigure:

        call WriteDecimalFigure

        sub di, 2 * 1

        or bx, bx
        jne WriteBinaryNumberLoopBegin

        ret


WriteOctalNumber:

;------------------------------------------------
;
;Writes octal number by your coordinates
;
;------------------------------------------------
;
;Entry: si = coordinate
;   	ax = number
;	cl = color
;	es = b800h - videoram ptr
;
;Exit:  (none)
;
;Destr: bx, ch, di
;
;------------------------------------------------

        mov bx, ax
	mov di, si

WriteOctalNumberLoopBegin:

        mov ch, bl
        and ch, 111b

        call WriteDecimalFigure

        sub di, 2 * 1

        shr bx, 3
        jne WriteOctalNumberLoopBegin

        ret


WriteHexadecimalNumber:

;------------------------------------------------
;
;Writes hexadecimal number by your coordinates
;
;------------------------------------------------
;
;Entry: si = coordinate
;   	ax = number
;	cl = color
;	es = b800h - videoram ptr
;
;Exit:  (none)
;
;Destr: bx, ch, di
;
;------------------------------------------------

        mov bx, ax
	mov di, si

WriteHexadecimalNumberLoopBegin:

        mov ch, bl
        and ch, 1111b

        call WriteHexadecimalFigure

        sub di, 2 * 1

        shr bx, 4
        jne WriteHexadecimalNumberLoopBegin

        ret

WriteHexadecimalNumberWithZeroes:

;------------------------------------------------
;
;Writes hexadecimal number with leading zeroes by your coordinates
;
;------------------------------------------------
;
;Entry: si = coordinate
;   	ax = number
;	cl = color
;	es = b800h - videoram ptr
;
;Exit:  (none)
;
;Destr: bx, ch, di
;
;------------------------------------------------

	push dx

        mov bx, ax
	mov di, si
	mov dx, 4

WriteHexadecimalNumberWithZeroesLoopBegin:

        mov ch, bl
        and ch, 1111b

        call WriteHexadecimalFigure

        sub di, 2 * 1

        shr bx, 4

	dec dx
        jne WriteHexadecimalNumberWithZeroesLoopBegin

	pop dx

        ret


PrintMessage:

;------------------------------------------------
;
;Writes your message by your coordinates
;
;------------------------------------------------
;
;Entry: si = coordinate
;   	bx = message ptr
;   	cl = color
;	es = b800h - videoram ptr
;
;Exit:  (none)
;
;Destr: di, ch, bx
;
;------------------------------------------------

	mov di, si

PrintMessageLoopBegin:

        mov ch, [bx]
        cmp ch, 0

        je PrintMessageLoopEnd

        mov es:[di], ch
        inc di
        mov es:[di], cl
        inc di

        inc bx

        jmp PrintMessageLoopBegin

PrintMessageLoopEnd:

        ret


SmallDelay:

;------------------------------------------------
;
;Makes small delay during program
;
;------------------------------------------------
;
;Entry: (none)
;
;Exit:  (none)
;
;Destr: (none)
;
;------------------------------------------------

    push ax
    push cx
    push dx


    mov ah, 86h
    mov dx, 8000h
    mov cx, 0001h

    int 15h

    pop dx
    pop cx
    pop ax

    ret

PauseTillNextPressedKey:

;------------------------------------------------
;
;Makes pause in programm till you press any key 
;
;------------------------------------------------
;
;Entry: (none)
;
;Exit:  (none)
;
;Destr: (none)
;
;------------------------------------------------

        push ax

	mov ah, 00h               
	int 16h

	pop ax

	ret

ReadUserNumber:

;------------------------------------------------
;
;Reads number (up to 65535 including) you entered from keyboard,
;	space is sign of end
;
;	!!! Warning: number greater than 65535 will be readen incorrectly
;
;------------------------------------------------
;
;Entry: bh = 0 (must be)
;
;Exit:  ax = readen number
;
;Destr: ax, bx, cx, dx
;
;------------------------------------------------

	push 0
	mov cx, 10

ReadUserNumberLoop:

	mov ah, 01h
	int 21h
	cmp al, ' '
	je ReadUserNumberLoopEnd

	sub al, '0'

	mov bl, al
	pop ax
	mul cx

	add ax, bx
	push ax

	jmp ReadUserNumberLoop

ReadUserNumberLoopEnd:

	pop ax

	ret

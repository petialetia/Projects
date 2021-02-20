.model tiny
.code
org 100h

	x_frame_coordinate               equ 24
	y_frame_coordinate               equ 6

	length_of_frame                  equ 33
	width_of_frame                   equ 7

	frame_color                      equ 55h
	frame_shadow_color               equ 05h
	frame_edge                       equ 178

	x_binary_number_coordinate       equ 54
	y_binary_number_coordinate       equ 7

	x_octal_number_coordinate        equ 54
	y_octal_number_coordinate        equ 9

	x_hexadecimal_number_coordinate  equ 54
	y_hexadecimal_number_coordinate  equ 11

	videoram_ptr                     equ 0b800h

	white_color                      equ 07h
	
	beginning_message_length         equ 37

Start:
	mov ah, 00h               ; clearing screen
	mov al, 03h
	int 10h

   	mov ax, 1003h             ; enabling bold backgrounds (16 bg colors)
	mov bl, 00h
	int 10h

	mov bx, videoram_ptr
	mov es, bx

	mov ch, white_color
	xor si, si
	mov bx, offset BeginningMessage

	call PrintMessage


	mov ah, 03h
	xor bh, bh
	int 10h                              ; Read cursor position

	mov ah, 02h
	add dl, beginning_message_length + 1
	int 10h                              ; Modify cursor position

	xor bh, bh
	call ReadUserNumber

	mov bh, length_of_frame
	mov bl, width_of_frame

	mov si, y_frame_coordinate * 80 * 2 + x_frame_coordinate * 2  ; si = coordinate of left upper corner
	mov ch, frame_edge                                            ; ch = ASCCI code of symbol of edge
	mov cl, frame_color                                           ; cl = color

	call DrawRectangle        ; DrawRectangle (si, bh, bl, cl, ch, es)

	call SmallDelay

	mov cl, frame_shadow_color
	add di, 2*80+1*2

	call DrawLowerRightCorner ; Draws shadow

	call SmallDelay


	add di, 2*80+1*2
	dec ch

	call DrawLowerRightCorner ; Draws shadow

	call SmallDelay


	add di, 2*80+1*2
	dec ch

	call DrawLowerRightCorner ; Draws shadow

	call SmallDelay



	mov ch, white_color
	mov si, 2*80*y_binary_number_coordinate + 2*(x_binary_number_coordinate - length_of_frame + 5)
	mov bx, offset BinaryMessage

	call PrintMessage

	call SmallDelay

	mov si, 2*80*y_binary_number_coordinate + 2*x_binary_number_coordinate

	call WriteBinaryNumber

	call SmallDelay




    	mov ch, white_color
	mov si, 2*80*y_octal_number_coordinate + 2*(x_octal_number_coordinate - length_of_frame + 5)
	mov bx, offset OctalMessage

	call PrintMessage

	call SmallDelay


	mov si, 2*80*y_octal_number_coordinate + 2*x_octal_number_coordinate

	call WriteOctalNumber

	call SmallDelay



    	mov ch, white_color
	mov si, 2*80*y_hexadecimal_number_coordinate + 2*(x_hexadecimal_number_coordinate - length_of_frame + 5)
	mov bx, offset HexadecimalMessage

	call PrintMessage

	call SmallDelay


	mov si, 2*80*y_hexadecimal_number_coordinate + 2*x_hexadecimal_number_coordinate

	call WriteHexadecimalNumber



	mov ah, 00h               ; Pause for you to see the result
	int 16h

	mov ah, 4ch               ; End of program
	int 21h

DrawRectangle:

;------------------------------------------------
;
;Draws rectangle by your coordinates
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
;Exit:  di = coordinate of right low corner
;
;Destr: di, dh
;
;------------------------------------------------

	mov dh, bh
	mov di, si
	call DrawHorizontalLine ; DrawHorizontalLine (di, dh, cl, ch, es)

	mov dh, bl
	sub dh, 2               ; one of the corner pixels is already drawn, another will be drawn later
	add di, 160

	call DrawVerticalLine   ; DrawVerticalLine (di, dh, cl, ch, es)

	mov dh, bl
	dec dh                  ; one of the corner pixels is already drawn
	mov di, si
	add di, 160

	call DrawVerticalLine   ; DrawVerticalLine (di, dh, cl, ch, es)

	mov dh, bh
	dec dh                  ; 1 corner pixel is already drawn
	add di, 2

	call DrawHorizontalLine ; DrawHorizontalLine (di, dh, cl, ch, es)

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

		sub di, 2                  ; coming back on 1 pixel to the left

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

		mov byte ptr es:[di], ch   ; print part of the edge
		inc di
		mov byte ptr es:[di], cl   ; print color of the edge
		add di, 2*80 - 1           ; moving to next pixel down

		dec dh
		je ExitDrawVerLoop

		jmp DrawVerLoop

	ExitDrawVerLoop:

		sub di, 2*80               ; coming back on 1 pixel to the up

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

    	sub si, 1*2 ; corner pixel already colored
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
		sub si, 3                  ;moving to next pixel on the left

		dec dh

		je ExitDrawHorBackLoop

		jmp DrawHorBackLoop

	ExitDrawHorBackLoop:

		add si, 2                  ;coming forth on 1 pixel to the right

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

		mov byte ptr es:[si], ch   ; print part of the edge
		inc si
		mov byte ptr es:[si], cl   ; print color of the edge
		sub si, 161                ; moving to next pixel up

		dec dh
		je ExitDrawVerBackLoop

		jmp DrawVerBackLoop

	ExitDrawVerBackLoop:

		sub si, 160                ; coming back on 1 pixel to the down

	 	ret


WriteDecimalFigure:

;------------------------------------------------
;
;Writes decimal figure by your coordinates
;
;------------------------------------------------
;
;Entry: si = coordinate
;	ch = figure
;	es = b800h - videoram ptr
;
;Exit:  (none)
;
;Destr: ch
;
;------------------------------------------------

    add ch, '0'
    mov byte ptr es:[si], ch
    inc si
    mov byte ptr es:[si], 07h
    dec si

    ret


WriteHexadecimalFigure:

;------------------------------------------------
;
;Writes hexadecimal figure by your coordinates
;
;------------------------------------------------
;
;Entry: si = coordinate
;	ch = figure
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

    mov byte ptr es:[si], ch
    inc si
    mov byte ptr es:[si], 07h
    dec si

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
;
;Exit:  (none)
;
;Destr: bx, ch, si
;
;------------------------------------------------


        mov bx, ax

WriteBinaryNumberLoopBegin:

	xor ch, ch

        shr bx, 1
        jnb PrintBinaryFigure

        inc ch

PrintBinaryFigure:

        call WriteDecimalFigure

        sub si, 2

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
;	es = b800h - videoram ptr
;
;Exit:  (none)
;
;Destr: bx, ch, si
;
;------------------------------------------------

        mov bx, ax

WriteOctalNumberLoopBegin:

        mov ch, bl
        and ch, 111b

        call WriteDecimalFigure

        sub si, 2

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
;	es = b800h - videoram ptr
;
;Exit:  (none)
;
;Destr: bx, ch, si
;
;------------------------------------------------

        mov bx, ax

WriteHexadecimalNumberLoopBegin:

        mov ch, bl
        and ch, 1111b

        call WriteHexadecimalFigure

        sub si, 2

        shr bx, 4
        jne WriteHexadecimalNumberLoopBegin

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
;   	ch = color
;	es = b800h - videoram ptr
;
;Exit:  (none)
;
;Destr: si, cl, bx
;
;------------------------------------------------

PrintMessageLoopBegin:

        mov cl, [bx]
        cmp cl, 0

        je PrintMessageLoopEnd

        mov es:[si], cl
        inc si
        mov es:[si], ch
        inc si

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

ReadUserNumber:

;------------------------------------------------
;
;Reads number (up to 65535 including) you entered from keyboard,
;	space is sign of end
;
;	!!! Warning: number greater than 65535 wil be readen incorrectly
;
;------------------------------------------------
;
;Entry: bh = 0 (must be)
;
;Exit:  ax = readen number
;
;Destr: ax, bx, cx
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

BeginningMessage db 'Print number you want to be converted', 00h

BinaryMessage db 'Binary:', 00h

OctalMessage db 'Octal:', 00h

HexadecimalMessage db 'Hexadecimal:', 00h

end Start

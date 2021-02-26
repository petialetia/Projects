.model tiny
.code
org 100h

CONST_OFFSET_OF_REG_VALUE_STACK macro reg, offset

;--------------------------------------------------------------------------------------------------

;	Allows you to create a const with reg offset in stack

;	Entry: es = 0 (must be)
   
;	Destr: ax, bx, es 

;--------------------------------------------------------------------------------------------------



	offset_of_value_&reg      equ offset

	endm

HANG_ON_INTERRUPT macro num_of_interrupt, place_for_old_ofs, place_for_old_seg, offset_of_new_interrupt

;--------------------------------------------------------------------------------------------------

;	Allows you to replace system interrupt with your own

;	Entry: es = 0 (must be)
   
;	Destr: ax, bx, es 

;--------------------------------------------------------------------------------------------------

	mov bx, (num_of_interrupt + 8) * 4

	cli

	mov ax, es:[bx]
	mov place_for_old_ofs, ax
	mov ax, es:[bx+2]
	mov place_for_old_seg, ax

	mov es:[bx], offset offset_of_new_interrupt
	mov ax, cs
	mov es:[bx+2], ax

	sti

	endm

PRINT_REGISTER_FROM_STACK macro reg 

;--------------------------------------------------------------------------------------------------

;	Prints name of register and it's value, taken from stack

;--------------------------------------------------------------------------------------------------

	mov bx, offset register_&reg
	mov dl, offset_of_value_&reg
	call PrintRegInfoFromStack

	endm


PRINT_REGISTER macro reg 

;--------------------------------------------------------------------------------------------------

;	Prints name of register and it's value

;--------------------------------------------------------------------------------------------------

	mov bx, offset register_&reg
	mov ax, reg
	call PrintRegInfo

	endm

MESSAGE_REGISTER macro reg

;--------------------------------------------------------------------------------------------------

;	Makes string with name of register

;--------------------------------------------------------------------------------------------------

register_&reg                  db '&reg', 00h

	endm
        

	num_of_timer_interrupt    equ 0
	num_of_keyboard_interrupt equ 1
	scan_code_of_first_key    equ 29
	scan_code_of_second_key   equ 33
	true                      equ 1

	screen_length             equ 80

	y_frame_coordinate        equ 1
	x_frame_coordinate        equ 69

	length_of_frame           equ 11
	width_of_frame            equ 16
	
	frame_color               equ 55h

	videoram_ptr              equ 0b800h

	text_color                equ 0ah

	background_color          equ 00h

	reg_name_value_offset     equ 6

	CONST_OFFSET_OF_REG_VALUE_STACK ax, 16

	CONST_OFFSET_OF_REG_VALUE_STACK bx, 14

	CONST_OFFSET_OF_REG_VALUE_STACK cx, 10

	CONST_OFFSET_OF_REG_VALUE_STACK dx, 8

	CONST_OFFSET_OF_REG_VALUE_STACK si, 6

	CONST_OFFSET_OF_REG_VALUE_STACK di, 4

	CONST_OFFSET_OF_REG_VALUE_STACK bp, 2

	CONST_OFFSET_OF_REG_VALUE_STACK ds, 12

	CONST_OFFSET_OF_REG_VALUE_STACK es, 0

Start:
	mov bx, offset end_of_timer_interrupt_ofs
	mov [bx], offset EndOfTimerInterrupt

	mov bx, offset end_of_timer_interrupt_seg
	mov [bx], cs

	xor bx, bx
	mov es, bx

	HANG_ON_INTERRUPT num_of_timer_interrupt,    old_timer_ofs,    old_timer_seg,    my_timer_interrupt

	HANG_ON_INTERRUPT num_of_keyboard_interrupt, old_keyboard_ofs, old_keyboard_seg, my_keyboard_interrupt

	mov ax, 3100h
	mov dx, offset EndOfProgramm
	shr dx, 4
	inc dx
	int 21h

my_timer_interrupt      proc
			push ax bx ds 
			
			mov ax, cs
			mov ds, ax

			mov bx, offset show_register_frame_indicator

			cmp byte ptr [bx], true
			je ContinueTimerInterrupt

			db 0eah

end_of_timer_interrupt_ofs dw 0
end_of_timer_interrupt_seg dw 0


ContinueTimerInterrupt:
			push cx dx si di bp es 

			mov bp, sp
			
			mov si, y_frame_coordinate * screen_length * 2 + x_frame_coordinate * 2

			mov bx, videoram_ptr
			mov es, bx

			mov bh, length_of_frame
			mov bl, width_of_frame

			mov cl, frame_color

			call DrawRectangle

			add si, 2 * screen_length + 1 * 2
			sub bh, 2
			sub bl, 2
			mov cl, background_color
			xor ah, ah

			call DrawOverRectangle

			add si, 2 * screen_length + 1 * 2
			mov cl, text_color

			xor dh, dh

                    	PRINT_REGISTER_FROM_STACK ax
			
			PRINT_REGISTER_FROM_STACK bx

			PRINT_REGISTER_FROM_STACK cx

			PRINT_REGISTER_FROM_STACK dx

			PRINT_REGISTER_FROM_STACK si

			PRINT_REGISTER_FROM_STACK di

			PRINT_REGISTER_FROM_STACK bp

			PRINT_REGISTER sp

			PRINT_REGISTER_FROM_STACK ds

			PRINT_REGISTER_FROM_STACK es

			PRINT_REGISTER ss

			PRINT_REGISTER cs

			pop es bp di si dx cx

EndOfTimerInterrupt:    

			pop ds bx ax
			db 0eah

old_timer_ofs           dw 0
old_timer_seg           dw 0

			endp

;--------------------------------------------------------------------------------------------------

my_keyboard_interrupt   proc
			push ax bx ds

			mov ax, cs
			mov ds, ax
			
			in al, 60h

			mov bx, offset previous_pressed_key
			
			cmp byte ptr [bx], scan_code_of_first_key
			jne EndOfKeyboardInterrupt

			cmp al, scan_code_of_second_key
			jne EndOfKeyboardInterrupt

			mov bx, offset show_register_frame_indicator
			
			xor byte ptr [bx], 1

			mov bx, offset previous_pressed_key

EndOfKeyboardInterrupt: 

			mov byte ptr [bx], al
			pop ds bx ax
			db 0eah

old_keyboard_ofs        dw 0
old_keyboard_seg        dw 0

			endp

			mov ax, 4c00h
			int 21h



show_register_frame_indicator  db 0
previous_pressed_key           db 0


PrintRegInfoFromStack:

;------------------------------------------------
;
;Writes name and value of register, taken from the stack
;
;------------------------------------------------
;
;Entry: si = coordinate
;   	bx = offset of the name of register
;	cl = color
;	dh = 00h (must be)
;	dl = offset of value in stack
;	es = b800h - videoram ptr
;	ss = current ss
;
;Exit:  si = coordinate for next PrintRegInfoFromStack or PrintRegInfo
;
;Destr: ax, bx, ch, di
;
;------------------------------------------------


			call PrintMessage

			add si, 2 * reg_name_value_offset

			mov di, bp
			add di, dx
			mov ax, ss:[di]

			call WriteHexadecimalNumberWithZeroes

			add si, 2 * (screen_length - reg_name_value_offset)

			ret

PrintRegInfo:

;------------------------------------------------
;
;Writes name and value of register
;
;------------------------------------------------
;
;Entry: si = coordinate
;   	bx = offset of the name of register
;	cl = color
;	es = b800h - videoram ptr
;
;Exit:  si = coordinate for next PrintRegInfoFromStack or PrintRegInfo
;
;Destr: ax, bx, ch, di
;
;------------------------------------------------


			call PrintMessage

			add si, 2 * reg_name_value_offset

			call WriteHexadecimalNumberWithZeroes

			add si, 2 * (screen_length - reg_name_value_offset)

			ret

MESSAGE_REGISTER ax

MESSAGE_REGISTER bx

MESSAGE_REGISTER cx

MESSAGE_REGISTER dx

MESSAGE_REGISTER si

MESSAGE_REGISTER di

MESSAGE_REGISTER bp

MESSAGE_REGISTER sp

MESSAGE_REGISTER ds

MESSAGE_REGISTER es

MESSAGE_REGISTER ss

MESSAGE_REGISTER cs


			include MyFunc.asm

EndOfProgramm:

end                     Start
			


















; MIT License - ADA86Games 
; Ege Özkan
; Source code to be included to draw images in Assembly.
; Written for FASM, uses DOS system calls.


; ========================================
; ///////////////////////////////////////|
;                                        |
; CHOOSE ROUTINE                         |
;                                        |
; USED TO MAKE THE USER CHOOSE THE NEXT  |
; DECISION THEY WILL MAKE.               |
;                                        |
; \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|
; ========================================


; CHOOSE
;
; DESCRIPTION
; A Routine to be called when user makes a choice.
; 	Reads input from the user and depending on
;	their choice, jumps to one of these three
;	locations.
;
; PARAMETERS
; @AX: Memory location to the user's first choice.
; @BX: Memory location to the user's second choice.
; @CX: Memory location to the user's third choice.
choose:
        lea     dx, [input] ; Print the input message
        mov     ah, 9 ; The print system call.
        int     21h 
        
        mov     ah, 01h ; Keybard IO
        int     21h ; Read character
        cmp     al, 50 ; Compare if [al] == 2

        mov     dl, lf
        mov     ah, 2
        int     21h ; Add a new line and carriage return.
        
        pop     dx ; Return the
        pop     cx ; Original values
        pop     bx ; of the
        pop     ax ; registers

        jl      short jump_to_one
        jg      short jump_to_three
        jmp     bx

; JUMP_TO_ONE
;
; DESCRIPTION
; Jumps to the memory location at AX.
; 	that should hold user's first
;	choice.
jump_to_one:
        jmp     ax


; JUMP_TO_THREE
;
; DESCRIPTION
; Jumps to the memory location at CX.
; 	that should hold user's second
;	choice.
jump_to_three:
        jmp     cx


; ========================================
; ///////////////////////////////////////|
;                                        |
; PRINT ROUTINE                          |
;                                        |
; USED FOR PRINTING CHARACTERS AND TEXT  |
; ON THE TEXT GRAPHIC MODE.              |
;                                        |
; \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|
; ========================================


; PRINT_CHAR
;
; DESCRIPTION
; Prints a single char to the terminal.
; PARAMETERS
; @AX: Holds the character to print.
print_char:

        push    dx;

        mov     dl, al  ; Get the character to print.
        mov     ah, 02h ; Character print call.
        int     21h;

        pop     dx;
        ret



; PRINT
;
; DESCRIPTION
; Print a $ terminated string, in a typewriter
; 	effect, if the user presses a button,
;	this effect is skipped.
;
; PARAMETERS
; @AX: Holds the address of the $ terminated string.

print:
        push    bx      ; Store bx in stack since it will be used.
        mov     bx, ax  ; ax will be used elsewhere, store the string
	; address into bx.

print_loop: ; Loop through characters until we get $.

        mov     al, [bx] ; Load the next character.
        cmp     al, '$' ; Check if terminal character.
        je      print_ret ; Return if terminal character.
        call    print_char
        call    sleep
        
        mov     al, 0
        add     bx, 1   ; Increment address by a char.

        mov     ah, 01H         ; Check STDIN.
        int     16H             ; Interrupt to check if skip.
        jnz     print_loop_no_sleep ; Stop sleeping while print.
        jmp     print_loop
        


print_loop_no_sleep:    ; Loop through characters without sleeping

        mov     al, [bx]        ; Load the next character.
        cmp     al, '$'         ; Check if terminal character.
        je      print_ret       ; Return if terminal.
        call    print_char
        mov     al, 0
        add     bx, 1           ; Increment address.
        jmp     print_loop_no_sleep

print_ret:

        mov     al, 00h ; Eliminate the input function register.
        mov     ah, 0ch ; Flush the keyboard.
        int     21h     ; Flush the stdin... Hopefully.

        mov     dl, lf  ; Prints a new line.
        mov     ah, 2   ; To the end of the message.
        int     21H

        pop     bx

        ret



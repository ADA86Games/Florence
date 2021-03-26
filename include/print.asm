choose:
        push    ax ; Store ax temporarily since DOS call.
        push    bx
        push    cx
        push    dx




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

jump_to_one:
        jmp     ax

jump_to_three:
        jmp     cx

print_char: ; Prints a single character.
; Argument al: Holds the character to be printed.

        push    dx;

        mov     dl, al  ; Get the character to print.
        mov     ah, 02h ; Character print call.
        int     21h;

        pop     dx;
        ret

sleep: ; Sleeps for some time.
; This is not a set amount of time, depends on CPU,
; terrible impementation, based on stackoverflow
; answer by Peter Cordes on a question about
; Sleep calls and spim.

        push    ax
        mov     ax, 25000

sleep_loop:
        cmp     ax, 0
        je      sleep_exit
        sub     ax, 1   ; Count down until zero.
        jmp     sleep_loop

sleep_exit:

        pop     ax
        ret

print: ; Prints data
; Argument ax: Holds the address to the printed message.
; Terminated with $.

        push    bx      ; Contains address for s
        mov     bx, ax  ; Since ax will be used often, get the address to ax.

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



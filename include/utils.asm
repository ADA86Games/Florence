; MIT License - ADA86Games 
; Ege Özkan
; Source code to be included to draw images in Assembly.
; Written for FASM, uses DOS system calls.


; ========================================
; ///////////////////////////////////////|
;                                        |
; SLEEP ROUTINE                          |
;                                        |
; USED FOR SLEEPING FOR SOME TIME, MAIN  |
; FUNCTION USED IS SLEEP.                |
;                                        |
; \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|
; ========================================


; CHOOSE
;
; DESCRIPTION
; Makes the program execution stop for an amount of time; 	
;	This is not a set amount of time, depends on CPU,
; 	terrible impementation, based on stackoverflow
; 	answer by Peter Cordes on a question about
; 	Sleep calls and spim.
sleep:
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

; ========================================
; ///////////////////////////////////////|
;                                        |
; EXIT ROUTINE                           |
;                                        |
; EXIT FROM THE DOS PROGRAM, HANDING     |
; CONTROL BACK TO THE OS.                |
;                                        |
; \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|
; ========================================

; EXIT
;
; DESCRIPTION
; Exits from the program by printing an
; 	exit message that should always
;	be present in the system.
exit:

        lea     ax, [exit_message]
        call    print
        mov     ah, 4CH
        int     21H



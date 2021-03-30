; MIT License - ADA86Games 
; Ege Özkan
; Source code to make assembly programs compile correctly.
; As well as initialisation routines.
; Written for FASM, uses DOS system calls.

format MZ

stdin equ 0
stdout equ 1
stderr equ 2

cr equ 0dh ; Carriage return
lf equ 0ah ; Line feed
semicolon equ 0x3b ; The semicolon character.

; ========================================
; ///////////////////////////////////////|
;                                        |
; HEADER ROUTINES                        |
;                                        |
; USED TO FACILITATE GAME START AND EXIT |
; SOME GLOBALS MUST BE SET IN PROGRAM.   |
;                                        |
; \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|
; ========================================

; MAIN
;
; DESCRIPTION
; Main function is the enterence function to the
; 	program.
main: ; our main function

        push    cs
        pop     ds

; MAIN_MENU
;
; DESCRIPTION
; Main menu is run first, it is a menu.
main_menu:
        
        lea     ax, [introduction]
        call    print
        lea     ax, [about]
        lea     bx, [start_section]
        lea     cx, [exit]
        jmp     choose

; The about game label.
about:
        
        lea     ax, [about_msg]
        call    print
        jmp     main_menu


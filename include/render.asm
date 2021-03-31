; MIT License - ADA86Games 	
; Ege Özkan
; Source code to be included to draw images in Assembly.
; Written for FASM, uses DOS system calls.

; ========================================
; ///////////////////////////////////////|
;                                        |
; GRAPHICS MODE ROUTINE                  |
;                                        |
; SET GRAPHICS MODES AND MAKE THE SCREEN |
; LOOK CORRECT.                          |
;                                        |
; \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|
; ========================================

; SET_GRAPHICS_MODE
;
; DESCRIPTION
; Set the graphics mode to the given mode.
; PARAMETERS
; @AL: Contains the graphics mode for the DOS.
set_graphics_mode: 
        mov     ah, 00h	; Set graphics mode.
        int     10h    	; Set the display mode to VGA graphics.
	ret

; ========================================
; ///////////////////////////////////////|
;                                        |
; DRAW ROUTINE                           |
;                                        |
; DRAW AN IMAGE TO THE SCREEN PROVIDED   |
; THE CORRECT SCREEN MODE IS SET.        |
;                                        |
; \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|
; ========================================

; DRAW_IMAGE
;
; DESCRIPTION
; Draw an image to the screen.
; PARAMETERS
; @AX: The name of the image to be drawn,
;	formatted as a null terminated
;	string.
draw_image:
        mov     dx, ax
        mov     al, 00h ; Read file.
        mov     ah, 3Dh ; Open file
        int     21h     ; Open the file.

        push    ax
        mov     bx, ax  ; Get the file handle.
        mov     cx, 64000      ; Read 320x200 bytes.
        lea     dx, [screen_buffer] ; Set the screen buffer.
        mov     ah, 3Fh ; Read data.
        int     21h

        pop     bx
        mov     ah, 3Eh ; Close file.
        int     21h
 

write_screen:
        mov     al, [ds:di] ; Get the next pixel
        mov     byte[es:bx], al    ; Write pixel.
        inc     bx              ; Increment ax.
        inc     di
        cmp     bx, 0f9ffH      ; Check if we wrote enough.
        jge     write_screen_ret
        jmp     write_screen

write_screen_ret:
	mov	ah, 01h	; Wait for keyboard IO.
	int	21h	; By reading a character.
	ret 		; Return from the routine.


screen_buffer   rb      64000

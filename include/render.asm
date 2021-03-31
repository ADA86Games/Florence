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
	
	push	ax	; Store ax.
	mov	ah, 0Fh	; Get current video mode.
	int	10h	; Saves current mode to AL.
	mov	cl, al	; Get the display mode to cl.
	pop	ax;
	push	cx;

	mov     dx, ax
        mov     al, 00h ; Read file.
        mov     ah, 3Dh ; Open file
        int     21h     ; Open the file.

        push    ax
        mov     bx, ax  ; Get the file handle.
        mov	cx, 64000	; Read 320x200 bytes.
	lea     dx, [screen_buffer] ; Set the screen buffer.
        mov     ah, 3Fh ; Read data.
        int     21h

        pop     bx
        mov     ah, 3Eh ; Close file.
        int     21h
 
set_screen_buffer:
	mov	al, 13h	; Switch to VGA graphics.
	mov	ah, 00h	; Set grahpics mode.
	int	10h
	mov	ax, 0A000h	; Set the buffer start.
	mov	es, ax
	mov	bx, 0h	; Increment variable.
	lea	di, [screen_buffer]
write_screen:
        mov     al, [ds:di] ; Get the next pixel
        mov     byte[es:bx], al    ; Write pixel.
        inc     bx              ; Increment ax.
        inc     di
        cmp     bx, 0fa00h      ; Check if we wrote enough.
        ja	write_screen_ret
        jmp     write_screen

write_screen_ret:
	mov	ah, 01h	; Wait for keyboard IO.
	int	21h	; By reading a character.
	pop	cx
	mov	al, cl	; Return the display mode.
	call	set_graphics_mode
	ret 		; Return from the routine.


screen_buffer   rb      64000

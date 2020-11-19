; file: skel.asm
; This file is a skeleton that can be used to start assembly programs.

%include "asm_io.inc"

; initialized data is put in the .data segment
segment .data

; uninitialized data is put in the .bss segment
segment .bss

; code is put in the .text segment
segment .text
    global  asm_main
asm_main:
    enter   0,0             ; setup routine
    pusha

    ; add your code here


    popa
    mov     eax, 0          ; return back to C
    leave
    ret



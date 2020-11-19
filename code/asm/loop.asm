;
; Using Linux and gcc:
; nasm -f elf asm_io.asm
; nasm -f elf loop.asm
; gcc -m32 -o loop loop.o driver.c asm_io.o

%include "asm_io.inc"

;
; initialized data is put in the .data segment
;
segment .data
;
; These labels refer to strings used for output
;
outmsg db  "Sum is ", 0

;
; uninitialized data is put in the .bss segment
;
segment .bss
;
; These labels refer to double words used to store the inputs
;
;
; code is put in the .text segment
;
segment .text
    global  asm_main
asm_main:
    enter   0,0                 ; setup routine
    pusha

    ; for
    mov     ecx, 10
    mov     ebx, 0
loop_start:
    add     ebx, ecx 
    loop    loop_start

    mov     eax, outmsg
    call    print_string        ; print out third message
    mov     eax, ebx
    call    print_int
    call    print_nl            ; print new-line

    ; while
    mov     ecx, 10
    mov     ebx, 0
while:
    cmp     ecx, 0
    jz      end_while
    add     ebx, ecx
    dec     ecx
    jmp     while
end_while:
    mov     eax, outmsg
    call    print_string        ; print out third message
    mov     eax, ebx
    call    print_int
    call    print_nl            ; print new-line

    ; do
    mov     ecx, 10
    mov     ebx, 0
do:
    add     ebx, ecx
    dec     ecx
    jnz     do
    mov     eax, outmsg
    call    print_string        ; print out third message
    mov     eax, ebx
    call    print_int
    call    print_nl            ; print new-line


    popa
    mov     eax, 0              ; return back to C
    leave              
    ret


; 子程序: CALL, RET, ebp, esp, 参数, 局部变量

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
    push    dword 2
    push    dword 5
    call    sub_program 
    add     esp, 8

    popa
    mov     eax, 0          ; return back to C
    leave              
    ret

sub_program:
    push    ebp
    mov     ebp, esp
    sub     esp, 4

    mov     dword[ebp-4], 10
    mov     eax, 0
    add     eax, [ebp+12]
    add     eax, [ebp+8]
    mul     dword[ebp-4]

    mov     esp, ebp
    pop     ebp
    ret


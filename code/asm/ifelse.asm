;
; Using Linux and gcc:
; nasm -f elf asm_io.asm
; nasm -f elf ifelse.asm
; gcc -m32 -o ifelse ifelse.o driver.c asm_io.o

%include "asm_io.inc"

;
; initialized data is put in the .data segment
;
segment .data
;
; These labels refer to strings used for output
;
prompt  db    "Enter a number: ", 0 ; don't forget nul terminator
outmsg1 db    "<8", 0
outmsg2 db    ">=8", 0

;
; uninitialized data is put in the .bss segment
;
segment .bss
;
; These labels refer to double words used to store the inputs
;
input1    resd 1


;
; code is put in the .text segment
;
segment .text
    global  asm_main
asm_main:
    enter   0,0                 ; setup routine
    pusha

    mov     eax, prompt         ; print out prompt
    call    print_string

    call    read_int            ; read integer
    mov     [input1], eax       ; store into input1

    mov     eax, [input1]       ; eax = dword at input1
    cmp     eax, 8
    dump_regs   1               ; dump out register values
    dump_mem    2, outmsg1, 1   ; dump out memory
    jae     then_block
    ; EAX < 8
    mov     eax, outmsg1
    call    print_string
    call    print_nl
    jmp     end_block
; EAX >= 8
then_block:
    mov     eax, outmsg2
    call    print_string        ; print out third message
    call    print_nl            ; print new-line

end_block:
    popa
    mov     eax, 0              ; return back to C
    leave              
    ret


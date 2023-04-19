global health_hook        ; Tells NASM that this can be used outside of this file
%define MY_AMMO 99 ; Preprocessor directive that makes the code easier to read, similar to a C/C++ #define


section .text         ; Tells NASM that the following section is code

; Need to increment stack offests by 0x10 (16) to account for out stack alignment

health_hook:
    push rbp        ; Stack alignment stuff, don't worry if you don't know what that means
    mov rbp, rsp

; Stolen Bytes

    minss xmm1,xmm0
    movaps xmm9,xmm1

; End Stolen Bytes

    cmp dword [rbx+0x25C], 0x1
    jne skip 

    movss [rbx+0x000001B0],xmm9

skip:

    leave    ; Short for mov rsp, rbp + pop rbp, returns the stack registers to how they were pre-alignment
    ret


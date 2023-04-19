global mineral_hook         ; Tells NASM that this can be used outside of this file
%define MY_AMMO 99 ; Preprocessor directive that makes the code easier to read, similar to a C/C++ #define


section .text         ; Tells NASM that the following section is code


mineral_hook:
    push rbp        ; Stack alignment stuff, don't worry if you don't know what that means
    mov rbp, rsp

    mov eax, __?float32?__(200.0)
    mov [rcx+0x60],eax
      

    ; Stolen bytes
    movss [rsp+0x38],xmm1 ; Need to increment stack offests by 0x10 (16) to account for out stack alignment
    movss [rcx+0x68],xmm0


    leave    ; Short for mov rsp, rbp + pop rbp, returns the stack registers to how they were pre-alignment
    ret
global objective_hook         ; Tells NASM that this can be used outside of this file
%define MY_AMMO 99 ; Preprocessor directive that makes the code easier to read, similar to a C/C++ #define

; Need to increment stack offests by 0x10 (16) to account for out stack alignment

section .text         ; Tells NASM that the following section is code


objective_hook:
    push rbp        ; Stack alignment stuff, don't worry if you don't know what that means
    mov rbp, rsp

; StolenBytes
    minss xmm2,[rcx+0x00000188]
; EndStolenBytes
      

    mov dword [rcx+0x0000018C], __?float32?__(500.0)

    leave    ; Short for mov rsp, rbp + pop rbp, returns the stack registers to how they were pre-alignment
    ret

    

[BITS 64]

DEFAULT REL

EXTERN hbin_main

GLOBAL Start
GLOBAL StRipStart
GLOBAL StRipEnd
GLOBAL GetInStruct
GLOBAL ___chkstk_ms

[SECTION .text$STUB]
    
    Start:
        push  rsi
        mov   rsi, rsp
        and   rsp, 0FFFFFFFFFFFFFFF0h
        sub   rsp, 020h
        call  hbin_main
        mov   rsp, rsi
        pop   rsi
        ret

    StRipStart:
        call StRipPtrStart
        ret

    StRipPtrStart:
        mov	rax, [rsp]
        sub rax, 0x1b 
        ;;sub rax, 0x24
        ret           

    GetInStruct:
        call StRipStart ;; Get start of agent       
        sub rax, 8      ;; Go backwards 8 bytes
        ret             ;; Return a ptr to a ptr to the input struct

    ___chkstk_ms:
        ret

[SECTION .text$E]

    StRipEnd:
        call StRetPtrEnd
        ret

    StRetPtrEnd:
        mov rax, [rsp]
        add	rax, 0xb  
        ret           

[SECTION .text$P]
   
    SymHannibalEnd:
        db 'H', 'A', 'N', 'N', 'I', 'B', 'A', 'L', '-', 'E', 'N', 'D'
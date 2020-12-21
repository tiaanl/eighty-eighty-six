; sum of odd numbers in the array.

     mov dx, bx
     ;mov [ds:si], dx

    ;shr byte [bp], cl

    ;inc cx
    ;inc bx

    ;mov cs, ax
    ;jmp 0xACDC:0x5578
    ;mov [cs:0x5578], ax
    ;repe stosb

    ;mov ax, [cs:0x000f]


;main:
;    mov si, data
;    mov cl, [si]
;    inc si
;    mov ch, 0x00
;    mov al, 0x00
;loop:
;    mov bl, [si]
;    test bl, 0x01
;    jz next
;    add al, bl
;next:
;    inc si
;    loop loop
;
;finish:
;    hlt
;
;data:
;    db 0x04
;    db 0x15
;    db 0x28
;    db 0x07
;    db 0x08

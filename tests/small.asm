; sum of odd numbers in the array.

    ;inc cx
    ;inc bx

    add word [bp+0x0102], 0x0242

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

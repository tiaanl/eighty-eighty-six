main:
    mov si, data
    mov cl, BYTE [si]
    inc si
    mov ch, 0x00
    mov al, 0x00

loop:
    mov bl, BYTE [si]
    test bl, 0x01
    je loop
    add al, bl
    inc si

finish:
    hlt

data:
    db 0x00

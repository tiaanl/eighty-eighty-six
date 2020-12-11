;main:
;    mov si, 0x01f4
;    mov cl, BYTE [si]
;    inc si
;    mov ch, 0x00
;    mov al, 0x00
;loop:
;    mov bl, BYTE [si]
;    test bl, 0x01
;    je loop
;    add al, bl
;    inc si
;finish:
;    hlt
;
;    add al, bl
;    add al, BYTE [di]

;add al, dl
;add bl, cl
;add cl, bl
;add dl, al

;add [di], al
;add [di-48], al
;add [di-257], al
;add al, bl
;add [0xFCBC], al
;
;add al, [di]
;add bl, [di-48]
;add cl, [di-257]
;add bl, al
;add dl, [0xFCBC]
;
;add ax, dx
;add bx, cx
;add cx, bx
;add dx, ax
;
;add [di], ax
;add [di-48], bx
;add [di-257], cx
;add ax, dx
;add [0xFCBC], dx
;
;add ax, [di]
;add bx, [di-48]
;add cx, [di-257]
;add dx, ax
;add dx, [0xFCBC]

;add al, 1
;add dl, 1
;add cl, 2
;add bl, 3
;add bx, 0x1007

mov al, 0xb0
mov cl, 0xb1
mov dl, 0xb2
mov bl, 0xb3
mov ah, 0xb4
mov ch, 0xb5
mov dh, 0xb6
mov bh, 0xb7
;mov ax, 0x01f4
;mov bx, 0x01f4
;mov cx, 0x01f4
;mov dx, 0x01f4
;mov di, 0x01f4
;mov si, 0x01f4
;mov bp, 0x01f4
;mov sp, 0x01f4

;mov [di], al
;mov [di], cl

;mov [di], ax
;mov [di], cx

;mov bl, [di]
;mov cx, [di]

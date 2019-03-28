	include boot\bcpp31\startup.inc
	include boot\bcpp31\bcpp31.inc
	include system_a.cfg



boot_text SEGMENT PARA PUBLIC 'BOOT'
		assume cs:boot_text
		PUBLIC	_boot_dtime
_boot_dtime	label	word
		dw	0000h	
		
		PUBLIC	_boot_down_flag
_boot_down_flag	label	byte
		db	00h
				
		PUBLIC	_dtemp_valid
_dtemp_valid	label	byte
		db	00h
	
		PUBLIC	_dtemp_pos
_dtemp_pos	label	word
		dw	0000h	
		
		PUBLIC	_dtemp_len
_dtemp_len	label	word
		dw	0000h			
	
		PUBLIC	_boot_dtemp
_boot_dtemp	label	byte
		db	00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h	
		db	00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
		db	00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
		db	00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
		db	00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
		db	00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
		db	00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
		db	00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
		db	00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
		db	00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
		db	00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
		db	00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
		db	00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
		db	00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
		db	00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
		db	00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
		db	00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
		db	00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
boot_text ends


DGROUP	group	_DATA,_BSS
	assume	cs:BOOT_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends


;_bootdown :	word
;_bootdown	label byte
;	db	0

;_b_down_f label	byte
;extrn	_b_down_f : byte
extrn	tos : word
extrn	_loader : far
extrn	__startup : dword

boot_text SEGMENT PARA PUBLIC 'BOOT'
		assume cs:boot_text


BITBLK_CONTROL_REG          equ 0fe80h
BITBLK_STATUS_REG           equ (BITBLK_CONTROL_REG + 1)
BITBLK_SOURCE_WIDTH_L       equ (BITBLK_CONTROL_REG + 2)  
BITBLK_SOURCE_WIDTH_H       equ (BITBLK_CONTROL_REG + 3)  
BITBLK_SA_L                 equ (BITBLK_CONTROL_REG + 4)  
BITBLK_SA_M                 equ (BITBLK_CONTROL_REG + 5)  
BITBLK_SA_H                 equ (BITBLK_CONTROL_REG + 6)  
BITBLK_DESTINATION_WIDTH_L  equ (BITBLK_CONTROL_REG + 8)  
BITBLK_DESTINATION_WIDTH_H  equ (BITBLK_CONTROL_REG + 9)  
BITBLK_DA_L                 equ (BITBLK_CONTROL_REG + 12)  
BITBLK_DA_M                 equ (BITBLK_CONTROL_REG + 13)  
BITBLK_DA_H                 equ (BITBLK_CONTROL_REG + 14)  
BITBLK_X_SIZE_L             equ (BITBLK_CONTROL_REG + 16)  
BITBLK_X_SIZE_H             equ (BITBLK_CONTROL_REG + 17)  
BITBLK_Y_SIZE_L             equ (BITBLK_CONTROL_REG + 18)  
BITBLK_Y_SIZE_H             equ (BITBLK_CONTROL_REG + 19)
   ;    
   ;    void fmem_insb_dma(unsigned long fmem,unsigned char *buffer,int count)
   ;    
    public  _fmem_insb_dma
_fmem_insb_dma  proc    far
    push bp
    mov bp, sp
    push ds
    push es
    pushf

    mov	dx,BITBLK_STATUS_REG    ;while((inportb(BITBLK_STATUS_REG)&0x01)!=0x01);	// 等待块传送完成
fmem_insb_dma_loop3:
    in  al, dx
    mov ah, 0
    and ax, 1
    cmp ax, 1
    jne short fmem_insb_dma_loop3

    cli                         ;可在读状态后关中断

    mov	dx,BITBLK_SA_H
    mov al, [bp + 8]
    out	dx,al
    dec dx                      ; mov dx, BITBLK_SA_M
    mov al, [bp + 7]
    out dx,al
    dec dx                      ; mov dx, BITBLK_SA_L
    mov al, [bp + 6]
    out dx,al

    push di
    les di, [bp + 10]

    mov ax, es
    mov cl, 12
    shr ax, cl
    or ax, 0fff0h
    mov	dx,BITBLK_DA_H
    out	dx,al
 
    mov ax, es
    mov cl, 4
    shl ax, cl
    mov cx, di
    add ax, cx
    mov dx,BITBLK_DA_L
    out	dx,al

    mov	dx,BITBLK_DA_M
    mov	al, ah
    out	dx,al

    mov cx, [bp + 14]
    add cx, 16	;弥补bitblock缺陷
    shr cx, 1
    mov	dx,BITBLK_SOURCE_WIDTH_L
    mov	ax,0800h
    out	dx,ax

    mov	dx,BITBLK_DESTINATION_WIDTH_L
    mov	ax,0800h
    out	dx,ax
    
    mov	dx,BITBLK_X_SIZE_L
    mov	ax,cx
    out	dx,ax

    mov	dx,BITBLK_Y_SIZE_L
    mov	ax,1
    out	dx,ax

    mov	dx,BITBLK_CONTROL_REG
    mov	al,051h
    out	dx,al

    sti

    mov	dx,BITBLK_STATUS_REG

fmem_insb_dma_loop4:
    in  al, dx
    mov ah, 0
    and ax, 1
    cmp ax, 1
    jne short fmem_insb_dma_loop4

    pop di

    popf
    pop es
    pop ds
    pop bp
    ret
_fmem_insb_dma  endp

   ;    
   ;    void fmem_fmem_dma(unsigned long fmem,unsigned long fmem,int count)
   ;    
    public  _fmem_fmem_dma
_fmem_fmem_dma  proc    far
    push bp
    mov bp, sp
    push ds
    push es
    pushf

    mov	dx,BITBLK_STATUS_REG    ;while((inportb(BITBLK_STATUS_REG)&0x01)!=0x01);	// 等待块传送完成
fmem_fmem_dma_loop3:
    in  al, dx
    mov ah, 0
    and ax, 1
    cmp ax, 1
    jne short fmem_fmem_dma_loop3

    cli                         ;可在读状态后关中断

    mov	dx,BITBLK_SA_H
    mov al, [bp + 8]
    out	dx,al
    dec dx                      ; mov dx, BITBLK_SA_M
    mov al, [bp + 7]
    out dx,al
    dec dx                      ; mov dx, BITBLK_SA_L
    mov al, [bp + 6]
    out dx,al

    mov	dx,BITBLK_DA_H
    mov al, [bp + 12]
    out	dx,al
    dec dx                      ; mov dx, BITBLK_DA_M
    mov al, [bp + 11]
    out dx,al
    dec dx                      ; mov dx, BITBLK_DA_L
    mov al, [bp + 10]
    out dx,al

    mov cx, [bp + 14]
;    add cx, 16	;弥补bitblock缺陷
    shr cx, 1
    mov	dx,BITBLK_SOURCE_WIDTH_L
    mov	ax,0800h
    out	dx,ax

    mov	dx,BITBLK_DESTINATION_WIDTH_L
    mov	ax,0800h
    out	dx,ax
    
    mov	dx,BITBLK_X_SIZE_L
    mov	ax,cx
    out	dx,ax

    mov	dx,BITBLK_Y_SIZE_L
    mov	ax,1
    out	dx,ax

    mov	dx,BITBLK_CONTROL_REG
    mov	al,051h
    out	dx,al

    sti

    mov	dx,BITBLK_STATUS_REG

fmem_fmem_dma_loop4:
    in  al, dx
    mov ah, 0
    and ax, 1
    cmp ax, 1
    jne short fmem_fmem_dma_loop4


    popf
    pop es
    pop ds
    pop bp
    ret
_fmem_fmem_dma  endp

BYTE_RD_LOW_ADD		equ 0fee6h
BYTE_RD_HIGH_ADD	equ 0fee7h
BYTE_RD_MOST_ADD	equ 0fee8h
BYTE_RD_DATA		equ 0fee9h

BYTE_WR_LOW_ADD		equ 0fee2h
BYTE_WR_HIGH_ADD	equ 0fee3h
BYTE_WR_MOST_ADD	equ 0fee4h
BYTE_WR_DATA		equ 0fee5h

   ;    
   ;    void fmem_insb(unsigned long fmem,unsigned char *buffer,int count)
   ;    
    public  _fmem_insb
_fmem_insb  proc    far
    push bp
    mov bp, sp
    pushf

    mov dx, BYTE_RD_MOST_ADD
    mov al, [bp + 8]
    out dx, al
    dec dx                      ; mov dx, BYTE_RD_HIGH_ADD
    mov al, [bp + 7]
    out dx, al
    dec dx                      ; mov dx, BYTE_RD_LOW_ADD
    mov al, [bp + 6]
    out dx, al

    push di
    mov dx, BYTE_RD_DATA
    les di, [bp + 10]
    mov cx, [bp + 14]
    shr cx, 1
    cld
    ;repz ins
_fmem_insb_loop:
    in  al, dx
    xchg al, ah
    in  al, dx
    xchg al, ah
;    stosb
    stosw
    loop _fmem_insb_loop
    pop di

    popf
    pop bp
    ret

_fmem_insb  endp

   ;    
   ;    void fmem_fmem(unsigned long fmem,unsigned long fmem1,int count)
   ;    
    public  _fmem_fmem
_fmem_fmem  proc    far
    push bp
    mov bp, sp
    pushf

    mov dx, BYTE_RD_MOST_ADD
    mov al, [bp + 8]
    out dx, al
    dec dx                      ; mov dx, BYTE_RD_HIGH_ADD
    mov al, [bp + 7]
    out dx, al
    dec dx                      ; mov dx, BYTE_RD_LOW_ADD
    mov al, [bp + 6]
    out dx, al

;    push di
;    mov dx, BYTE_RD_DATA
;    les di, [bp + 10]

    mov dx, BYTE_WR_MOST_ADD
    mov al, [bp + 12]
    out dx, al
    dec dx                      ; mov dx, BYTE_WR_HIGH_ADD
    mov al, [bp + 11]
    out dx, al
    dec dx                      ; mov dx, BYTE_WR_LOW_ADD
    mov al, [bp + 10]
    out dx, al

    mov cx, [bp + 14]
    cld

_fmem_fmem_loop:
    mov dx, BYTE_RD_DATA
    in  al, dx
    mov dx, BYTE_WR_DATA
    out dx, al
    loop _fmem_fmem_loop
;    pop di

    popf
    pop bp
    ret

_fmem_fmem  endp

		public	boot
boot:
		cli

		mov dx, 0ff84h
		mov al, 02ch
		out dx, al
		
		mov dx, 0ff85h
		mov ax, 0ah
		out dx, ax
		
		mov dx, 0ff82h
		mov al, 040h
		out dx, al
		
;		mov dx, 0ff81h
;		mov al, 0aah
;		out dx, al
		
if 1;test sdram ready	
		mov 	dx, 0fee0h
test1:
		in		al, dx
		
		test		al, 4

		jz		test1
endif		

		call	far ptr _loader	
		jmp	Far Ptr __startup	
	

		Ret
boot_text	Ends
		end

		PUBLIC	_BS_LOAD
_BS_LOAD           PROC      FAR
                   PUSH      AX
                   PUSH      BX
                   PUSH      CX
                   PUSH      DX
                   PUSH      SI
                   PUSH      DI
;enable bit stream pre_scan
		mov 	dx, 0feeah
		mov	al, 040h
		out	dx, al

		mov 	dx, 0fee4h
		mov	al, 017h
		out	dx, al
		dec	dx
		mov	al, 0
		out	dx, al
		dec	dx
		out	dx, al
		
    mov		si, 0
    mov		ax, 0c000h
    mov		ds, ax

		mov 	dx, 0fee5h
;		mov	cx, 047d6h
		mov	cx, 05c71h
;		mov	cx, 09d6h
		
test2:
		lodsb
		out dx, al
		
		loop	test2
;bit stream length
		mov 	dx, 0fe0ah
		mov	ax, 05c71h
;		mov	ax, 09d6h
		out dx, ax
;disable bit stream pre_scan
		mov 	dx, 0feeah
		mov	al, 000h
		out	dx, al
                   POP       DI
                   POP       SI
                   POP       DX
                   POP       CX
                   POP       BX
                   POP       AX
                   RET
_BS_LOAD		endp                   


		
regdata SEGMENT BYTE PUBLIC 'CODE'
regdata ends

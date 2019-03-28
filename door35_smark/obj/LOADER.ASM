	ifndef	??version
?debug	macro
	endm
publicdll macro	name
	public	name
	endm
$comm	macro	name,dist,size,count
	comm	dist name:BYTE:count*size
	endm
	else
$comm	macro	name,dist,size,count
	comm	dist name[size]:BYTE:count
	endm
	endif
	?debug	V 300h
	?debug	S "boot\loader.c"
	?debug	C E9F05893410D626F6F745C6C6F616465722E63
	?debug	C E917ADAA2A1E2E2E5C2E2E5C746F6F6C735C626333315C696E636C+
	?debug	C 7564655C646F732E68
	?debug	C E94059CA18202E2E5C2E2E5C746F6F6C735C626333315C696E636C+
	?debug	C 7564655C5F646566732E68
LOADER_TEXT	segment byte public 'CODE'
LOADER_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:LOADER_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
_DATA	segment word public 'DATA'
_boot_GpioWatchDogStarted	label	byte
	db	1
_DATA	ends
LOADER_TEXT	segment byte public 'CODE'
	?debug	C E8010D626F6F745C6C6F616465722E63F0589341
   ;	
   ;	void loader(void)
   ;	
	?debug	L 69
	assume	cs:LOADER_TEXT
_loader	proc	far
	?debug	B
	push	bp
	mov	bp,sp
	sub	sp,14
	?debug	B
   ;	
   ;	{
   ;	
   ;	  INT8U boot_send_comm[10];
   ;	  INT8U sendlength, crc, i;
   ;	
   ;	  Boot_InitComm(BOOT_UART0_TX_STATUS, 57600, 3, 0xc7, 1);         // 0xc7
   ;	
	?debug	L 75
	mov	al,1
	push	ax
	mov	al,199
	push	ax
	mov	al,3
	push	ax
	xor	ax,ax
	mov	dx,-7936
	push	ax
	push	dx
	mov	ax,-128
	push	ax
	call	far ptr _Boot_InitComm
	add	sp,12
   ;	
   ;	
   ;	  sendlength = 6;//263;
   ;	
	?debug	L 77
	mov	byte ptr [bp-11],6
   ;	
   ;	  boot_send_comm[0] = 0xAA;
   ;	
	?debug	L 78
	mov	byte ptr [bp-10],170
   ;	
   ;	  boot_send_comm[1] = (sendlength - 3) / 256;
   ;	
	?debug	L 79
	mov	al,byte ptr [bp-11]
	mov	ah,0
	add	ax,-3
	mov	bx,256
	cwd	
	idiv	bx
	mov	byte ptr [bp-9],al
   ;	
   ;	  boot_send_comm[2] = (sendlength - 3) % 256;
   ;	
	?debug	L 80
	mov	al,byte ptr [bp-11]
	mov	ah,0
	add	ax,-3
	mov	bx,256
	cwd	
	idiv	bx
	mov	byte ptr [bp-8],dl
   ;	
   ;	  boot_send_comm[3] = BOOT_DOWNLOADFILE;
   ;	
	?debug	L 81
	mov	byte ptr [bp-7],224
   ;	
   ;	  boot_send_comm[4] = BOOT_STARTDOWN;             //命令类型 请求下载;
   ;	
	?debug	L 82
	mov	byte ptr [bp-6],3
   ;	
   ;	  crc = 0;
   ;	
	?debug	L 83
	mov	byte ptr [bp-12],0
   ;	
   ;	  for(i=1; i<(sendlength - 1); i++)
   ;	
	?debug	L 84
	mov	byte ptr [bp-13],1
	jmp	short @1@114
@1@58:
   ;	
   ;	    crc += boot_send_comm[i];
   ;	
	?debug	L 85
	mov	al,byte ptr [bp-13]
	mov	ah,0
	lea	dx,word ptr [bp-10]
	add	ax,dx
	mov	bx,ax
	mov	al,byte ptr ss:[bx]
	add	byte ptr [bp-12],al
	?debug	L 84
	inc	byte ptr [bp-13]
@1@114:
	mov	al,byte ptr [bp-13]
	mov	ah,0
	mov	dl,byte ptr [bp-11]
	mov	dh,0
	dec	dx
	cmp	ax,dx
	jl	short @1@58
   ;	
   ;	  boot_send_comm[sendlength - 1] = crc;
   ;	
	?debug	L 86
	mov	al,byte ptr [bp-11]
	mov	ah,0
	lea	dx,word ptr [bp-11]
	add	ax,dx
	mov	dl,byte ptr [bp-12]
	mov	bx,ax
	mov	byte ptr ss:[bx],dl
   ;	
   ;	
   ;	  outportb(BOOT_UART0_TX_DATA, 0x66);
   ;	
	?debug	L 88
	mov	dx,-127
	mov	al,102
	out	dx,al
   ;	
   ;	  outportb(BOOT_UART0_TX_DATA, 0x77);
   ;	
	?debug	L 89
	mov	dx,-127
	mov	al,119
	out	dx,al
   ;	
   ;	  boot_down_flag = 0;
   ;	
	?debug	L 90
	mov	byte ptr DGROUP:_boot_down_flag,0
   ;	
   ;	  for(i=0; i<sendlength; i++)
   ;	
	?debug	L 91
	mov	byte ptr [bp-13],0
	jmp	short @1@282
@1@170:
	jmp	short @1@198
@1@198:
   ;	
   ;	   {
   ;	    while((inportb(BOOT_UART0_TX_STATUS)&0x1)!=0x1);	// 等待发送保持器为空
   ;	
	?debug	L 93
	mov	dx,-128
	in	al,dx
	mov	ah,0
	and	ax,1
	cmp	ax,1
	jne	short @1@198
   ;	
   ;	    outportb(BOOT_UART0_TX_DATA, boot_send_comm[i]);				// 发送字符
   ;	
	?debug	L 94
	mov	al,byte ptr [bp-13]
	mov	ah,0
	lea	dx,word ptr [bp-10]
	add	ax,dx
	mov	bx,ax
	mov	al,byte ptr ss:[bx]
	mov	dx,-127
	out	dx,al
	?debug	L 91
	inc	byte ptr [bp-13]
@1@282:
	mov	al,byte ptr [bp-13]
	cmp	al,byte ptr [bp-11]
	jb	short @1@170
   ;	
   ;	   }
   ;	  boot_dtime = 0;
   ;	
	?debug	L 96
	mov	word ptr DGROUP:_boot_dtime,0
@1@338:
   ;	
   ;	  while(1)
   ;	   {
   ;	    Boot_comm_upgrade();
   ;	
	?debug	L 99
	call	far ptr _Boot_comm_upgrade
   ;	
   ;	    if(boot_down_flag == 0)
   ;	
	?debug	L 100
	cmp	byte ptr DGROUP:_boot_down_flag,0
	jne	short @1@394
   ;	
   ;	      boot_dtime ++;
   ;	
	?debug	L 101
	inc	word ptr DGROUP:_boot_dtime
@1@394:
   ;	
   ;	    if(boot_dtime >= 10000)
   ;	
	?debug	L 102
	cmp	word ptr DGROUP:_boot_dtime,10000
	jb	short @1@450
   ;	
   ;	      break;
   ;	
	?debug	L 103
	jmp	short @1@478
@1@450:
	jmp	short @1@338
@1@478:
   ;	
   ;	   }
   ;	}
   ;	
	?debug	L 105
	mov	sp,bp
	pop	bp
	ret	
	?debug	C E318000A001A08
	?debug	C E601690802F3FF00036372630802F4FF000A7365+
	?debug	C 6E646C656E6774680802F5FF000E626F6F745F73+
	?debug	C 656E645F636F6D6D1802F6FF00
	?debug	E
	?debug	E
_loader	endp
   ;	
   ;	void Boot_InitComm(INT16U port, INT32U baud, INT8U datafrm, INT8U fifolevel, INT8U freq)
   ;	
	?debug	L 107
	assume	cs:LOADER_TEXT
_Boot_InitComm	proc	far
	?debug	B
	push	bp
	mov	bp,sp
	sub	sp,2
	?debug	C E60466726571080A100000096669666F6C657665+
	?debug	C 6C080A0E0000076461746166726D080A0C000004+
	?debug	C 626175640C0A08000004706F72740A0A060000
	?debug	B
   ;	
   ;	{
   ;		INT16U gen;
   ;	    /* set baudrate */
   ;	
   ;	        gen = BOOT_CPU_MAIN_FREQ/baud/freq;   //20120222 改晶振
   ;	
	?debug	L 112
	mov	al,byte ptr [bp+16]
	mov	ah,0
	cwd	
	push	dx
	push	ax
	push	word ptr [bp+10]
	push	word ptr [bp+8]
	mov	ax,750
	xor	dx,dx
	push	ax
	push	dx
	call	far ptr F_LUDIV@
	push	dx
	push	ax
	call	far ptr F_LUDIV@
	mov	word ptr [bp-2],ax
   ;	
   ;	
   ;	   outportb(port+4, (gen&0xff));
   ;	
	?debug	L 114
	mov	dx,word ptr [bp+6]
	add	dx,4
	mov	al,byte ptr [bp-2]
	and	al,255
	out	dx,al
   ;	
   ;	   outportb(port+5, (gen>>8));
   ;	
	?debug	L 115
	mov	ax,word ptr [bp-2]
	mov	cl,8
	shr	ax,cl
	mov	dx,word ptr [bp+6]
	add	dx,5
	out	dx,al
   ;	
   ;	   outportb(port+2, fifolevel);
   ;	
	?debug	L 116
	mov	dx,word ptr [bp+6]
	add	dx,2
	mov	al,byte ptr [bp+14]
	out	dx,al
   ;	
   ;	
   ;	  dtemp_valid = 0;
   ;	
	?debug	L 118
	mov	byte ptr DGROUP:_dtemp_valid,0
   ;	
   ;	  dtemp_pos = 0;
   ;	
	?debug	L 119
	mov	word ptr DGROUP:_dtemp_pos,0
   ;	
   ;	
   ;	}
   ;	
	?debug	L 121
	mov	sp,bp
	pop	bp
	ret	
	?debug	C E60367656E0A02FEFF0004706F72740A0A060000+
	?debug	C 04626175640C0A080000076461746166726D080A+
	?debug	C 0C0000096669666F6C6576656C080A0E00000466+
	?debug	C 726571080A100000
	?debug	E
	?debug	E
_Boot_InitComm	endp
   ;	
   ;	void interrupt Boot_IdleInt()
   ;	
	?debug	L 123
	assume	cs:LOADER_TEXT
_Boot_IdleInt	proc	far
	?debug	B
	push	ax
	push	bx
	push	cx
	push	dx
	push	es
	push	ds
	push	si
	push	di
	push	bp
	mov	bp,DGROUP
	mov	ds,bp
	mov	bp,sp
	?debug	B
   ;	
   ;	{
   ;	  outportb(BOOT_UART0_TX_DATA, 16);				// 发送字符
   ;	
	?debug	L 125
	mov	dx,-127
	mov	al,16
	out	dx,al
   ;	
   ;	}
   ;	
	?debug	L 126
	pop	bp
	pop	di
	pop	si
	pop	ds
	pop	es
	pop	dx
	pop	cx
	pop	bx
	pop	ax
	iret	
	?debug	E
	?debug	E
_Boot_IdleInt	endp
   ;	
   ;	void Boot_comm_upgrade(void)    //20110608 xu 初始升级
   ;	
	?debug	L 128
	assume	cs:LOADER_TEXT
_Boot_comm_upgrade	proc	far
	?debug	B
	push	bp
	mov	bp,sp
	sub	sp,10
	?debug	B
	jmp	@4@758
@4@58:
   ;	
   ;	{
   ;	  INT8U recv_byte, crc;
   ;	  INT16U CurrPackage;
   ;	  INT16U i;
   ;	  INT32U padd;
   ;	
   ;	 	while((inportb(BOOT_UART0_RX_CTRL)&0x10)==0x10)
   ;	         {
   ;	          boot_ClearWatchDog();
   ;	
	?debug	L 137
	call	far ptr _boot_ClearWatchDog
   ;	
   ;	          recv_byte = inportb(BOOT_UART0_RX_DATA);
   ;	
	?debug	L 138
	mov	dx,-125
	in	al,dx
	mov	byte ptr [bp-1],al
   ;	
   ;	          if(dtemp_valid == 0)
   ;	
	?debug	L 139
	cmp	byte ptr DGROUP:_dtemp_valid,0
	jne	short @4@170
   ;	
   ;	           {
   ;	             if(recv_byte == 0xAA)      //包开始
   ;	
	?debug	L 141
	cmp	byte ptr [bp-1],170
	jne	short @4@142
   ;	
   ;	              {
   ;	               dtemp_valid = 1;
   ;	
	?debug	L 143
	mov	byte ptr DGROUP:_dtemp_valid,1
   ;	
   ;	               dtemp_pos = 0;
   ;	
	?debug	L 144
	mov	word ptr DGROUP:_dtemp_pos,0
   ;	
   ;	               boot_dtemp[dtemp_pos] = recv_byte;
   ;	
	?debug	L 145
	mov	bx,word ptr DGROUP:_dtemp_pos
	mov	al,byte ptr [bp-1]
	mov	byte ptr DGROUP:_boot_dtemp[bx],al
   ;	
   ;	               dtemp_pos ++;
   ;	
	?debug	L 146
	inc	word ptr DGROUP:_dtemp_pos
@4@142:
   ;	
   ;	              }
   ;	           }
   ;	
	?debug	L 148
	jmp	@4@758
@4@170:
   ;	
   ;	          else
   ;	           {
   ;	            boot_dtemp[dtemp_pos] = recv_byte;
   ;	
	?debug	L 151
	mov	bx,word ptr DGROUP:_dtemp_pos
	mov	al,byte ptr [bp-1]
	mov	byte ptr DGROUP:_boot_dtemp[bx],al
   ;	
   ;	            dtemp_pos ++;
   ;	
	?debug	L 152
	inc	word ptr DGROUP:_dtemp_pos
   ;	
   ;	            if(dtemp_pos == 3)
   ;	
	?debug	L 153
	cmp	word ptr DGROUP:_dtemp_pos,3
	jne	short @4@226
   ;	
   ;	             {
   ;	              dtemp_len = boot_dtemp[1];
   ;	
	?debug	L 155
	mov	al,byte ptr DGROUP:_boot_dtemp+1
	mov	ah,0
	mov	word ptr DGROUP:_dtemp_len,ax
   ;	
   ;	              dtemp_len <<= 8;//*256
   ;	
	?debug	L 156
	mov	cl,8
	shl	word ptr DGROUP:_dtemp_len,cl
   ;	
   ;	              dtemp_len += boot_dtemp[2];
   ;	
	?debug	L 157
	mov	al,byte ptr DGROUP:_boot_dtemp+2
	mov	ah,0
	add	word ptr DGROUP:_dtemp_len,ax
@4@226:
   ;	
   ;	             }
   ;	            if(dtemp_pos == (dtemp_len + 3))   //接收到一个包
   ;	
	?debug	L 159
	mov	ax,word ptr DGROUP:_dtemp_len
	add	ax,3
	cmp	ax,word ptr DGROUP:_dtemp_pos
	je	@@0
	jmp	@4@758
@@0:
   ;	
   ;	             {
   ;	               crc = 0;
   ;	
	?debug	L 161
	mov	byte ptr [bp-2],0
   ;	
   ;	               for(i=1; i<(dtemp_pos - 1); i++)
   ;	
	?debug	L 162
	mov	word ptr [bp-6],1
	jmp	short @4@338
@4@282:
   ;	
   ;	                 crc += boot_dtemp[i];
   ;	
	?debug	L 163
	mov	bx,word ptr [bp-6]
	mov	al,byte ptr DGROUP:_boot_dtemp[bx]
	add	byte ptr [bp-2],al
	?debug	L 162
	inc	word ptr [bp-6]
@4@338:
	mov	ax,word ptr DGROUP:_dtemp_pos
	dec	ax
	cmp	ax,word ptr [bp-6]
	ja	short @4@282
   ;	
   ;	               if(boot_dtemp[dtemp_pos - 1] == crc)
   ;	
	?debug	L 164
	mov	bx,word ptr DGROUP:_dtemp_pos
	dec	bx
	mov	al,byte ptr DGROUP:_boot_dtemp[bx]
	cmp	al,byte ptr [bp-2]
	je	@@1
	jmp	@4@730
@@1:
   ;	
   ;	                {
   ;	                 boot_dtime = 0;
   ;	
	?debug	L 166
	mov	word ptr DGROUP:_boot_dtime,0
   ;	
   ;	                 boot_down_flag = 1;
   ;	
	?debug	L 167
	mov	byte ptr DGROUP:_boot_down_flag,1
   ;	
   ;	
   ;	                 //写 flash
   ;	                 CurrPackage = boot_dtemp[5];
   ;	
	?debug	L 170
	mov	al,byte ptr DGROUP:_boot_dtemp+5
	mov	ah,0
	mov	word ptr [bp-4],ax
   ;	
   ;	                 CurrPackage <<= 8;//*256
   ;	
	?debug	L 171
	mov	cl,8
	shl	word ptr [bp-4],cl
   ;	
   ;	                 CurrPackage += boot_dtemp[6];
   ;	
	?debug	L 172
	mov	al,byte ptr DGROUP:_boot_dtemp+6
	mov	ah,0
	add	word ptr [bp-4],ax
   ;	
   ;	                 padd = (INT32U)CurrPackage * 256;
   ;	
	?debug	L 173
	mov	ax,word ptr [bp-4]
	xor	dx,dx
	mov	cl,8
	call	far ptr F_LXLSH@
	mov	word ptr [bp-8],dx
	mov	word ptr [bp-10],ax
   ;	
   ;	                 if (!(CurrPackage%16))       //4096
   ;	
	?debug	L 174
	test	word ptr [bp-4],15
	jne	short @4@450
   ;	
   ;	                  {
   ;	                   boot_w25x32writeenable();
   ;	
	?debug	L 176
	call	far ptr _boot_w25x32writeenable
   ;	
   ;	                   boot_sectererase25x32(padd);
   ;	
	?debug	L 177
	push	word ptr [bp-8]
	push	word ptr [bp-10]
	call	far ptr _boot_sectererase25x32
	pop	cx
	pop	cx
   ;	
   ;	                   boot_read25x32status();
   ;	
	?debug	L 178
	call	far ptr _boot_read25x32status
@4@450:
   ;	
   ;	                   //boot_delay(10);
   ;	                  }
   ;	                 boot_w25x32writeenable();
   ;	
	?debug	L 181
	call	far ptr _boot_w25x32writeenable
   ;	
   ;	                 boot_programpage25x32(padd, boot_dtemp + 7, 256);
   ;	
	?debug	L 182
	mov	ax,256
	push	ax
	push	ds
	mov	ax,offset DGROUP:_boot_dtemp+7
	push	ax
	push	word ptr [bp-8]
	push	word ptr [bp-10]
	call	far ptr _boot_programpage25x32
	add	sp,10
   ;	
   ;	                 boot_w25x32writedisable();
   ;	
	?debug	L 183
	call	far ptr _boot_w25x32writedisable
   ;	
   ;	                 //boot_delay(10);
   ;	
   ;	                 boot_dtemp[4] = BOOT_REPLY;
   ;	
	?debug	L 186
	mov	byte ptr DGROUP:_boot_dtemp+4,2
   ;	
   ;	                 crc = 0;
   ;	
	?debug	L 187
	mov	byte ptr [bp-2],0
   ;	
   ;	                 for(i=1; i<(dtemp_pos - 1); i++)
   ;	
	?debug	L 188
	mov	word ptr [bp-6],1
	jmp	short @4@534
@4@478:
   ;	
   ;	                   crc += boot_dtemp[i];
   ;	
	?debug	L 189
	mov	bx,word ptr [bp-6]
	mov	al,byte ptr DGROUP:_boot_dtemp[bx]
	add	byte ptr [bp-2],al
	?debug	L 188
	inc	word ptr [bp-6]
@4@534:
	mov	ax,word ptr DGROUP:_dtemp_pos
	dec	ax
	cmp	ax,word ptr [bp-6]
	ja	short @4@478
   ;	
   ;	                 boot_dtemp[dtemp_pos - 1] = crc;
   ;	
	?debug	L 190
	mov	bx,word ptr DGROUP:_dtemp_pos
	dec	bx
	mov	al,byte ptr [bp-2]
	mov	byte ptr DGROUP:_boot_dtemp[bx],al
   ;	
   ;	                 for(i=0; i<dtemp_pos; i++)
   ;	
	?debug	L 191
	mov	word ptr [bp-6],0
	jmp	short @4@702
@4@590:
	jmp	short @4@618
@4@618:
   ;	
   ;	                  {
   ;	                   while((inportb(BOOT_UART0_TX_STATUS)&0x1)!=0x1);	// 等待发送保持器为空
   ;	
	?debug	L 193
	mov	dx,-128
	in	al,dx
	mov	ah,0
	and	ax,1
	cmp	ax,1
	jne	short @4@618
   ;	
   ;	                   outportb(BOOT_UART0_TX_DATA, boot_dtemp[i]);				// 发送字符
   ;	
	?debug	L 194
	mov	bx,word ptr [bp-6]
	mov	al,byte ptr DGROUP:_boot_dtemp[bx]
	mov	dx,-127
	out	dx,al
	?debug	L 191
	inc	word ptr [bp-6]
@4@702:
	mov	ax,word ptr [bp-6]
	cmp	ax,word ptr DGROUP:_dtemp_pos
	jb	short @4@590
@4@730:
   ;	
   ;	                  }
   ;	                }
   ;	               dtemp_valid = 0;
   ;	
	?debug	L 197
	mov	byte ptr DGROUP:_dtemp_valid,0
   ;	
   ;	               dtemp_pos = 0;
   ;	
	?debug	L 198
	mov	word ptr DGROUP:_dtemp_pos,0
@4@758:
	?debug	L 135
	mov	dx,-126
	in	al,dx
	mov	ah,0
	and	ax,16
	cmp	ax,16
	jne	@@2
	jmp	@4@58
@@2:
   ;	
   ;	             }
   ;	           }
   ;		 }
   ;	}
   ;	
	?debug	L 202
	mov	sp,bp
	pop	bp
	ret	
	?debug	C E604706164640C02F6FF0001690A02FAFF000B43+
	?debug	C 7572725061636B6167650A02FCFF000363726308+
	?debug	C 02FEFF0009726563765F627974650802FFFF00
	?debug	E
	?debug	E
_Boot_comm_upgrade	endp
   ;	
   ;	void boot_w25x32writeenable(void)
   ;	
	?debug	L 219
	assume	cs:LOADER_TEXT
_boot_w25x32writeenable	proc	far
	?debug	B
	push	bp
	mov	bp,sp
	?debug	B
   ;	
   ;	{
   ;	 		disable();
   ;	
	?debug	L 221
	db	250
   ;	
   ;	    outportb(BOOT_BITBLK_CONTROL_REG, 0x20);    //sel spi controller
   ;	
	?debug	L 222
	mov	dx,-384
	mov	al,32
	out	dx,al
   ;	
   ;	    outportb(BOOT_SPCR, 0x50);                  //int disable, spi enable, ss=0, mstr =1
   ;	
	?debug	L 223
	mov	dx,-80
	mov	al,80
	out	dx,al
   ;	
   ;	
   ;	    outportb(BOOT_SPDR, 0x06);                  //write enable command
   ;	
	?debug	L 225
	mov	dx,-78
	mov	al,6
	out	dx,al
   ;	
   ;	    inportb(BOOT_SPDR);
   ;	
	?debug	L 226
	mov	dx,-78
	in	al,dx
   ;	
   ;	
   ;	    outportb(BOOT_SPCR, inportb(BOOT_SPCR)|0x70);      //ss=1
   ;	
	?debug	L 228
	mov	dx,-80
	in	al,dx
	or	al,112
	mov	dx,-80
	out	dx,al
   ;	
   ;	    outportb(BOOT_BITBLK_CONTROL_REG, 0x00);      //sel spi flash
   ;	
	?debug	L 229
	mov	dx,-384
	mov	al,0
	out	dx,al
   ;	
   ;			enable();
   ;	
	?debug	L 230
	db	251
   ;	
   ;	}
   ;	
	?debug	L 231
	pop	bp
	ret	
	?debug	E
	?debug	E
_boot_w25x32writeenable	endp
   ;	
   ;	void boot_w25x32writedisable(void)
   ;	
	?debug	L 233
	assume	cs:LOADER_TEXT
_boot_w25x32writedisable	proc	far
	?debug	B
	push	bp
	mov	bp,sp
	?debug	B
   ;	
   ;	{
   ;			disable();
   ;	
	?debug	L 235
	db	250
   ;	
   ;	    outportb(BOOT_BITBLK_CONTROL_REG, 0x20);    //sel spi controller
   ;	
	?debug	L 236
	mov	dx,-384
	mov	al,32
	out	dx,al
   ;	
   ;	    outportb(BOOT_SPCR, 0x50);                  //int enable, spi enable, ss=0, mstr =1
   ;	
	?debug	L 237
	mov	dx,-80
	mov	al,80
	out	dx,al
   ;	
   ;	
   ;	    outportb(BOOT_SPDR, 0x04);                  //write disable command
   ;	
	?debug	L 239
	mov	dx,-78
	mov	al,4
	out	dx,al
   ;	
   ;	    inportb(BOOT_SPDR);
   ;	
	?debug	L 240
	mov	dx,-78
	in	al,dx
   ;	
   ;	
   ;	    outportb(BOOT_SPCR, inportb(BOOT_SPCR)|0x70);      //ss=1
   ;	
	?debug	L 242
	mov	dx,-80
	in	al,dx
	or	al,112
	mov	dx,-80
	out	dx,al
   ;	
   ;	    outportb(BOOT_BITBLK_CONTROL_REG, 0x00);      //sel spi flash
   ;	
	?debug	L 243
	mov	dx,-384
	mov	al,0
	out	dx,al
   ;	
   ;			enable();
   ;	
	?debug	L 244
	db	251
   ;	
   ;	}
   ;	
	?debug	L 245
	pop	bp
	ret	
	?debug	E
	?debug	E
_boot_w25x32writedisable	endp
   ;	
   ;	void boot_read25x32status(void)
   ;	
	?debug	L 247
	assume	cs:LOADER_TEXT
_boot_read25x32status	proc	far
	?debug	B
	push	bp
	mov	bp,sp
	?debug	B
   ;	
   ;	{
   ;			disable();
   ;	
	?debug	L 249
	db	250
   ;	
   ;	    outportb(BOOT_BITBLK_CONTROL_REG, 0x20);    //sel spi controller
   ;	
	?debug	L 250
	mov	dx,-384
	mov	al,32
	out	dx,al
   ;	
   ;	    outportb(BOOT_SPCR, 0x50);                  //int disable, spi enable, ss=0, mstr =1
   ;	
	?debug	L 251
	mov	dx,-80
	mov	al,80
	out	dx,al
   ;	
   ;	
   ;	    outportb(BOOT_SPDR, 0x05);                  //read status register command
   ;	
	?debug	L 253
	mov	dx,-78
	mov	al,5
	out	dx,al
   ;	
   ;	    inportb(BOOT_SPDR);
   ;	
	?debug	L 254
	mov	dx,-78
	in	al,dx
	jmp	short @7@58
@7@58:
   ;	
   ;	
   ;	    outportb(BOOT_SPDR, 0x00);
   ;	    while(inportb(BOOT_SPDR)&0x01==0x01) {       // read status register
   ;	           outportb(BOOT_SPDR, 0x00);
   ;	
	?debug	L 258
	mov	dx,-78
	mov	al,0
	out	dx,al
	?debug	L 257
	mov	dx,-78
	in	al,dx
	mov	ah,0
	test	ax,1
	jne	short @7@58
   ;	
   ;	      }
   ;	
   ;	    outportb(BOOT_SPCR, inportb(BOOT_SPCR)|0x70);     //ss=1
   ;	
	?debug	L 261
	mov	dx,-80
	in	al,dx
	or	al,112
	mov	dx,-80
	out	dx,al
   ;	
   ;	    outportb(BOOT_BITBLK_CONTROL_REG, 0x00);     //sel spi flash
   ;	
	?debug	L 262
	mov	dx,-384
	mov	al,0
	out	dx,al
   ;	
   ;			enable();
   ;	
	?debug	L 263
	db	251
   ;	
   ;	}
   ;	
	?debug	L 264
	pop	bp
	ret	
	?debug	E
	?debug	E
_boot_read25x32status	endp
   ;	
   ;	void boot_programpage25x32(INT32U paddr, INT8U *data, INT16U len)
   ;	
	?debug	L 266
	assume	cs:LOADER_TEXT
_boot_programpage25x32	proc	far
	?debug	B
	push	bp
	mov	bp,sp
	sub	sp,2
	?debug	C E319000400160800
	?debug	C E6036C656E0A0A0E00000464617461190A0A0000+
	?debug	C 0570616464720C0A060000
	?debug	B
   ;	
   ;	{
   ;	    INT16U i;
   ;	
   ;	    //xtm_printf("inportb(SPSR)&0x40 = 0x%X\n", (INT16U)(inportb(SPSR)&0x40));
   ;	    boot_wait_till_ready();
   ;	
	?debug	L 271
	call	far ptr _boot_wait_till_ready
   ;	
   ;	
   ;	    disable();
   ;	
	?debug	L 273
	db	250
   ;	
   ;	    outportb(BOOT_BITBLK_CONTROL_REG, 0x20);    //sel spi controller
   ;	
	?debug	L 274
	mov	dx,-384
	mov	al,32
	out	dx,al
   ;	
   ;	    outportb(BOOT_SPCR, 0x50);                  //int disable, spi enable, ss=0, mstr =1
   ;	
	?debug	L 275
	mov	dx,-80
	mov	al,80
	out	dx,al
   ;	
   ;	    outportb(BOOT_SPDR, 0x02);                  //program page 256 bytes data command
   ;	
	?debug	L 276
	mov	dx,-78
	mov	al,2
	out	dx,al
   ;	
   ;	    inportb(BOOT_SPDR);
   ;	
	?debug	L 277
	mov	dx,-78
	in	al,dx
   ;	
   ;	    outportb(BOOT_SPDR, paddr>>16);             //write A23-A16 Address
   ;	
	?debug	L 278
	mov	ax,word ptr [bp+8]
	mov	dx,-78
	out	dx,al
   ;	
   ;	    inportb(BOOT_SPDR);
   ;	
	?debug	L 279
	mov	dx,-78
	in	al,dx
   ;	
   ;	    outportb(BOOT_SPDR, paddr>>8);              //write A15-A8 Address
   ;	
	?debug	L 280
	mov	dx,word ptr [bp+8]
	mov	ax,word ptr [bp+6]
	mov	cl,8
	call	far ptr F_LXURSH@
	mov	dx,-78
	out	dx,al
   ;	
   ;	    inportb(BOOT_SPDR);
   ;	
	?debug	L 281
	mov	dx,-78
	in	al,dx
   ;	
   ;	    outportb(BOOT_SPDR, paddr);                 //write A7-A0 address
   ;	
	?debug	L 282
	mov	dx,-78
	mov	al,byte ptr [bp+6]
	out	dx,al
   ;	
   ;	    inportb(BOOT_SPDR);
   ;	
	?debug	L 283
	mov	dx,-78
	in	al,dx
   ;	
   ;	    
   ;	    for(i=0;i<len;i++){ 
   ;	
	?debug	L 285
	mov	word ptr [bp-2],0
	jmp	short @8@114
@8@58:
   ;	
   ;	//    		i++;
   ;	//    		i--;
   ;	        outportb(BOOT_SPDR, *data++);                 //write dummy data
   ;	
	?debug	L 288
	les	bx,dword ptr [bp+10]
	inc	word ptr [bp+10]
	mov	al,byte ptr es:[bx]
	mov	dx,-78
	out	dx,al
   ;	
   ;	        inportb(BOOT_SPDR);                     //read chip data  
   ;	
	?debug	L 289
	mov	dx,-78
	in	al,dx
	?debug	L 285
	inc	word ptr [bp-2]
@8@114:
	mov	ax,word ptr [bp-2]
	cmp	ax,word ptr [bp+14]
	jb	short @8@58
   ;	
   ;	     }
   ;	    outportb(BOOT_SPCR, inportb(BOOT_SPCR)|0x70);      //ss=1
   ;	
	?debug	L 291
	mov	dx,-80
	in	al,dx
	or	al,112
	mov	dx,-80
	out	dx,al
   ;	
   ;	    outportb(BOOT_BITBLK_CONTROL_REG, 0x00);      //sel spi flash	
   ;	
	?debug	L 292
	mov	dx,-384
	mov	al,0
	out	dx,al
   ;	
   ;			enable();
   ;	
	?debug	L 293
	db	251
   ;	
   ;	    boot_wait_till_ready();
   ;	
	?debug	L 294
	call	far ptr _boot_wait_till_ready
   ;	
   ;	}
   ;	
	?debug	L 295
	mov	sp,bp
	pop	bp
	ret	
	?debug	C E601690A02FEFF000570616464720C0A06000004+
	?debug	C 64617461190A0A0000036C656E0A0A0E0000
	?debug	E
	?debug	E
_boot_programpage25x32	endp
   ;	
   ;	void boot_sectererase25x32(INT32U paddr)   // erase 4k
   ;	
	?debug	L 298
	assume	cs:LOADER_TEXT
_boot_sectererase25x32	proc	far
	?debug	B
	push	bp
	mov	bp,sp
	?debug	C E60570616464720C0A060000
	?debug	B
   ;	
   ;	{
   ;	    disable();
   ;	
	?debug	L 300
	db	250
   ;	
   ;	    outportb(BOOT_BITBLK_CONTROL_REG, 0x20);    //sel spi controller
   ;	
	?debug	L 301
	mov	dx,-384
	mov	al,32
	out	dx,al
   ;	
   ;	    outportb(BOOT_SPCR, 0x50);                  //int enable, spi enable, ss=0, mstr =1 
   ;	
	?debug	L 302
	mov	dx,-80
	mov	al,80
	out	dx,al
   ;	
   ;	    outportb(BOOT_SPDR, 0x20);                  //secter erase command
   ;	
	?debug	L 303
	mov	dx,-78
	mov	al,32
	out	dx,al
   ;	
   ;	    inportb(BOOT_SPDR);
   ;	
	?debug	L 304
	mov	dx,-78
	in	al,dx
   ;	
   ;	    outportb(BOOT_SPDR, paddr>>16);             //write A23-A16 Address
   ;	
	?debug	L 305
	mov	ax,word ptr [bp+8]
	mov	dx,-78
	out	dx,al
   ;	
   ;	    inportb(BOOT_SPDR);
   ;	
	?debug	L 306
	mov	dx,-78
	in	al,dx
   ;	
   ;	    outportb(BOOT_SPDR, paddr>>8);              //write A15-A8 Address
   ;	
	?debug	L 307
	mov	dx,word ptr [bp+8]
	mov	ax,word ptr [bp+6]
	mov	cl,8
	call	far ptr F_LXURSH@
	mov	dx,-78
	out	dx,al
   ;	
   ;	    inportb(BOOT_SPDR);
   ;	
	?debug	L 308
	mov	dx,-78
	in	al,dx
   ;	
   ;	    outportb(BOOT_SPDR, paddr);                 //write A7-A0 address
   ;	
	?debug	L 309
	mov	dx,-78
	mov	al,byte ptr [bp+6]
	out	dx,al
   ;	
   ;	    inportb(BOOT_SPDR);
   ;	
	?debug	L 310
	mov	dx,-78
	in	al,dx
   ;	
   ;	
   ;	    outportb(BOOT_SPCR, inportb(BOOT_SPCR)|0x70);      //ss=1
   ;	
	?debug	L 312
	mov	dx,-80
	in	al,dx
	or	al,112
	mov	dx,-80
	out	dx,al
   ;	
   ;	    outportb(BOOT_BITBLK_CONTROL_REG, 0x00);      //sel spi flash	
   ;	
	?debug	L 313
	mov	dx,-384
	mov	al,0
	out	dx,al
   ;	
   ;			enable();
   ;	
	?debug	L 314
	db	251
   ;	
   ;	    boot_wait_till_ready();            
   ;	
	?debug	L 315
	call	far ptr _boot_wait_till_ready
   ;	
   ;	}
   ;	
	?debug	L 316
	pop	bp
	ret	
	?debug	C E60570616464720C0A060000
	?debug	E
	?debug	E
_boot_sectererase25x32	endp
   ;	
   ;	void boot_wait_till_ready(void)
   ;	
	?debug	L 318
	assume	cs:LOADER_TEXT
_boot_wait_till_ready	proc	far
	?debug	B
	push	bp
	mov	bp,sp
	sub	sp,4
	?debug	B
   ;	
   ;	{
   ;	    INT16U i;
   ;	    INT8U tmp;
   ;		      	disable();
   ;	
	?debug	L 322
	db	250
   ;	
   ;	    outportb(BOOT_BITBLK_CONTROL_REG, 0x20);    //sel spi controller
   ;	
	?debug	L 323
	mov	dx,-384
	mov	al,32
	out	dx,al
   ;	
   ;	    outportb(BOOT_SPCR, 0x50);                  //int disable, spi enable, ss=0, mstr =1
   ;	
	?debug	L 324
	mov	dx,-80
	mov	al,80
	out	dx,al
   ;	
   ;	
   ;	    outportb(BOOT_SPDR, 0x05);                  //read status register command
   ;	
	?debug	L 326
	mov	dx,-78
	mov	al,5
	out	dx,al
   ;	
   ;	    inportb(BOOT_SPDR);
   ;	
	?debug	L 327
	mov	dx,-78
	in	al,dx
   ;	
   ;	    tmp = 0x01;
   ;	
	?debug	L 328
	mov	byte ptr [bp-3],1
   ;	
   ;	
   ;	    i = 0;
   ;	
	?debug	L 330
	mov	word ptr [bp-2],0
	jmp	short @10@114
@10@58:
   ;	
   ;	    while((tmp & 0x01) == 0x01)                         //write status register
   ;	     {
   ;	      outportb(BOOT_SPDR, 0x00);                  //write dummy Address
   ;	
	?debug	L 333
	mov	dx,-78
	mov	al,0
	out	dx,al
   ;	
   ;	      tmp = inportb(BOOT_SPDR);
   ;	
	?debug	L 334
	mov	dx,-78
	in	al,dx
	mov	byte ptr [bp-3],al
   ;	
   ;	      i++;
   ;	
	?debug	L 335
	inc	word ptr [bp-2]
   ;	
   ;	      if(i > 30000)
   ;	
	?debug	L 336
	cmp	word ptr [bp-2],30000
	jbe	short @10@114
   ;	
   ;	       {
   ;	        break;
   ;	
	?debug	L 338
	jmp	short @10@142
@10@114:
	?debug	L 331
	mov	al,byte ptr [bp-3]
	mov	ah,0
	and	ax,1
	cmp	ax,1
	je	short @10@58
@10@142:
   ;	
   ;	       }
   ;	     }
   ;	
   ;	    outportb(BOOT_SPCR, inportb(BOOT_SPCR)|0x70);      //ss=1
   ;	
	?debug	L 342
	mov	dx,-80
	in	al,dx
	or	al,112
	mov	dx,-80
	out	dx,al
   ;	
   ;	    outportb(BOOT_BITBLK_CONTROL_REG, 0x00);      //sel spi flash
   ;	
	?debug	L 343
	mov	dx,-384
	mov	al,0
	out	dx,al
   ;	
   ;		       	enable();
   ;	
	?debug	L 344
	db	251
   ;	
   ;	
   ;	}
   ;	
	?debug	L 346
	mov	sp,bp
	pop	bp
	ret	
	?debug	C E603746D700802FDFF0001690A02FEFF00
	?debug	E
	?debug	E
_boot_wait_till_ready	endp
   ;	
   ;	void boot_ClearWatchDog(void)
   ;	
	?debug	L 348
	assume	cs:LOADER_TEXT
_boot_ClearWatchDog	proc	far
	?debug	B
	push	bp
	mov	bp,sp
	sub	sp,2
	?debug	B
   ;	
   ;	{
   ;	  INT8U tmp_data;
   ;	  if(boot_GpioWatchDogStarted)  //在GPIO检测线程中启动WatchDog
   ;	
	?debug	L 351
	cmp	byte ptr DGROUP:_boot_GpioWatchDogStarted,0
	je	short @11@170
   ;	
   ;	   {
   ;	    outportb(BOOT_GPIOA_OEN_DATA, inportb(BOOT_GPIOA_OEN_DATA) & 0xfb);       //gpioa2为输出
   ;	
	?debug	L 353
	mov	dx,721
	in	al,dx
	and	al,251
	mov	dx,721
	out	dx,al
   ;	
   ;	    tmp_data = inportb(BOOT_GPIOA_OUT_DATA);
   ;	
	?debug	L 354
	mov	dx,720
	in	al,dx
	mov	byte ptr [bp-1],al
   ;	
   ;	    if(tmp_data & 0x04)
   ;	
	?debug	L 355
	test	byte ptr [bp-1],4
	je	short @11@114
   ;	
   ;	      outportb(BOOT_GPIOA_OUT_DATA, tmp_data & 0xfb);
   ;	
	?debug	L 356
	mov	al,byte ptr [bp-1]
	and	al,251
	jmp	short @11@142
@11@114:
   ;	
   ;	    else
   ;	      outportb(BOOT_GPIOA_OUT_DATA, tmp_data | 0x04);
   ;	
	?debug	L 358
	mov	al,byte ptr [bp-1]
	or	al,4
@11@142:
	mov	dx,720
	out	dx,al
@11@170:
   ;	
   ;	   }
   ;	}
   ;	
	?debug	L 360
	mov	sp,bp
	pop	bp
	ret	
	?debug	C E608746D705F646174610802FFFF00
	?debug	E
	?debug	E
_boot_ClearWatchDog	endp
	?debug	C E9
	?debug	C FA00000000
LOADER_TEXT	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
LOADER_TEXT	segment byte public 'CODE'
LOADER_TEXT	ends
	extrn	F_LXURSH@:far
	extrn	F_LXLSH@:far
	extrn	F_LUDIV@:far
	public	_loader
	public	_boot_ClearWatchDog
	public	_boot_GpioWatchDogStarted
	public	_boot_wait_till_ready
	public	_boot_programpage25x32
	public	_boot_sectererase25x32
	public	_boot_read25x32status
	public	_boot_w25x32writedisable
	public	_boot_w25x32writeenable
	public	_Boot_comm_upgrade
	public	_Boot_IdleInt
	public	_Boot_InitComm
	extrn	_boot_dtemp:byte
	extrn	_dtemp_len:word
	extrn	_dtemp_pos:word
	extrn	_dtemp_valid:byte
	extrn	_boot_down_flag:byte
	extrn	_boot_dtime:word
_s@	equ	s@
	?debug	C EA010C
	?debug	C E31A00000023040500
	?debug	C EB09465F4C5855525348401A00
	?debug	C EB08465F4C584C5348401A00
	?debug	C EB08465F4C55444956401A00
	?debug	C E31B00000023010400
	?debug	C EC075F6C6F616465721B1800
	?debug	C E31C00000023010400
	?debug	C EC135F626F6F745F436C6561725761746368446F+
	?debug	C 671C1800
	?debug	C EC195F626F6F745F4770696F5761746368446F67+
	?debug	C 53746172746564080000
	?debug	C E31D00000023010400
	?debug	C EC155F626F6F745F776169745F74696C6C5F7265+
	?debug	C 6164791D1800
	?debug	C E31E00000023010400
	?debug	C EC165F626F6F745F70726F6772616D7061676532+
	?debug	C 357833321E1800
	?debug	C E31F00000023010400
	?debug	C EC165F626F6F745F736563746572657261736532+
	?debug	C 357833321F1800
	?debug	C E32000000023010400
	?debug	C EC155F626F6F745F726561643235783332737461+
	?debug	C 747573201800
	?debug	C E32100000023010400
	?debug	C EC185F626F6F745F773235783332777269746564+
	?debug	C 697361626C65211800
	?debug	C E32200000023010400
	?debug	C EC175F626F6F745F773235783332777269746565+
	?debug	C 6E61626C65221800
	?debug	C E32300000023010400
	?debug	C EC125F426F6F745F636F6D6D5F75706772616465+
	?debug	C 231800
	?debug	C E32400000023010700
	?debug	C EC0D5F426F6F745F49646C65496E74249800
	?debug	C E32500000023010400
	?debug	C EC0E5F426F6F745F496E6974436F6D6D251800
	?debug	C E3260020011A08
	?debug	C EB0B5F626F6F745F6474656D702600
	?debug	C EB0A5F6474656D705F6C656E0A00
	?debug	C EB0A5F6474656D705F706F730A00
	?debug	C EB0C5F6474656D705F76616C69640800
	?debug	C EB0F5F626F6F745F646F776E5F666C61670800
	?debug	C EB0B5F626F6F745F6474696D650A00
	?debug	C E606494E543332550C060006494E543136550A06+
	?debug	C 0005494E543855080600
	end

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
	?debug	C E9F759E2400D626F6F745C6C6F616465722E63
	?debug	C E94677BE4017617263682F696E636C7564655C696E636C75646573+
	?debug	C 2E68
	?debug	C E94059CA18212E2E5C2E2E5C746F6F6C735C626333315C696E636C+
	?debug	C 7564655C7374646465662E68
	?debug	C E94059CA18202E2E5C2E2E5C746F6F6C735C626333315C696E636C+
	?debug	C 7564655C5F646566732E68
	?debug	C E94059CA18202E2E5C2E2E5C746F6F6C735C626333315C696E636C+
	?debug	C 7564655C5F6E756C6C2E68
	?debug	C E94059CA18212E2E5C2E2E5C746F6F6C735C626333315C696E636C+
	?debug	C 7564655C7369676E616C2E68
	?debug	C E94059CA181F2E2E5C2E2E5C746F6F6C735C626333315C696E636C+
	?debug	C 7564655C6D6174682E68
	?debug	C E94059CA18202E2E5C2E2E5C746F6F6C735C626333315C696E636C+
	?debug	C 7564655C666C6F61742E68
	?debug	C E94059CA18202E2E5C2E2E5C746F6F6C735C626333315C696E636C+
	?debug	C 7564655C737464696F2E68
	?debug	C E94059CA18212E2E5C2E2E5C746F6F6C735C626333315C696E636C+
	?debug	C 7564655C5F6E66696C652E68
	?debug	C E94059CA18212E2E5C2E2E5C746F6F6C735C626333315C696E636C+
	?debug	C 7564655C737472696E672E68
	?debug	C E94059CA181E2E2E5C2E2E5C746F6F6C735C626333315C696E636C+
	?debug	C 7564655C6D656D2E68
	?debug	C E94059CA18202E2E5C2E2E5C746F6F6C735C626333315C696E636C+
	?debug	C 7564655C63747970652E68
	?debug	C E94059CA18212E2E5C2E2E5C746F6F6C735C626333315C696E636C+
	?debug	C 7564655C7374646C69622E68
	?debug	C E94059CA18202E2E5C2E2E5C746F6F6C735C626333315C696E636C+
	?debug	C 7564655C636F6E696F2E68
	?debug	C E917ADAA2A1E2E2E5C2E2E5C746F6F6C735C626333315C696E636C+
	?debug	C 7564655C646F732E68
	?debug	C E94059CA18212E2E5C2E2E5C746F6F6C735C626333315C696E636C+
	?debug	C 7564655C7365746A6D702E68
	?debug	C E96069792016626F6F742F6263707033315C74797065646566732E+
	?debug	C 68
	?debug	C E94059CA18232E2E5C2E2E5C746F6F6C735C626333315C696E636C+
	?debug	C 7564655C67726170686963732E68
	?debug	C E94059CA18212E2E5C2E2E5C746F6F6C735C626333315C696E636C+
	?debug	C 7564655C737472696E672E68
	?debug	C E94059CA18202E2E5C2E2E5C746F6F6C735C626333315C696E636C+
	?debug	C 7564655C616C6C6F632E68
	?debug	C E96B869E3C0F6B65726E656C5C6F735F6370752E68
	?debug	C E9667CD530116B65726E656C5C6F735F6366675F722E68
	?debug	C E9B77BD44015617263682F696E636C7564655C6D5F696E69742E68
	?debug	C E9B57EBE4012617263682F696E636C7564655C6370752E68
	?debug	C E9084EDA4014617263682F696E636C7564655C785F6370752E68
LOADER_TEXT	segment byte public 'BOOT'
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
LOADER_TEXT	segment byte public 'BOOT'
	?debug	C E8010D626F6F745C6C6F616465722E63F759E240
   ;	
   ;	void loader(void)
   ;	
	?debug	L 24
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
   ;	  INT8U send_comm[10];
   ;	  INT8U sendlength, crc, i;
   ;	
   ;	  Boot_InitComm(UART0_TX_STATUS, 115200, 3, 0xc7, 1);
   ;	
	?debug	L 30
	mov	al,1
	push	ax
	mov	al,199
	push	ax
	mov	al,3
	push	ax
	mov	ax,1
	mov	dx,-15872
	push	ax
	push	dx
	mov	ax,-128
	push	ax
	call	far ptr _Boot_InitComm
	add	sp,12
	jmp	short @1@58
@1@58:
   ;	
   ;	
   ;	  while((inportb(UART0_TX_STATUS)&0x1)!=0x1);	// 等待发送保持器为空
   ;	
	?debug	L 32
	mov	dx,-128
	in	al,dx
	mov	ah,0
	and	ax,1
	cmp	ax,1
	jne	short @1@58
   ;	
   ;	  outportb(UART0_TX_DATA, 0x66);
   ;	
	?debug	L 33
	mov	dx,-127
	mov	al,102
	out	dx,al
	jmp	short @1@114
@1@114:
   ;	
   ;	  while((inportb(UART0_TX_STATUS)&0x1)!=0x1);	// 等待发送保持器为空
   ;	
	?debug	L 34
	mov	dx,-128
	in	al,dx
	mov	ah,0
	and	ax,1
	cmp	ax,1
	jne	short @1@114
   ;	
   ;	  outportb(UART0_TX_DATA, 0x77);  
   ;	
	?debug	L 35
	mov	dx,-127
	mov	al,119
	out	dx,al
   ;	
   ;	  sendlength = 6;//263;
   ;	
	?debug	L 36
	mov	byte ptr [bp-11],6
   ;	
   ;	  send_comm[0] = 0xAA;
   ;	
	?debug	L 37
	mov	byte ptr [bp-10],170
   ;	
   ;	  send_comm[1] = (sendlength - 3) / 256;
   ;	
	?debug	L 38
	mov	al,byte ptr [bp-11]
	mov	ah,0
	add	ax,-3
	mov	bx,256
	cwd	
	idiv	bx
	mov	byte ptr [bp-9],al
   ;	
   ;	  send_comm[2] = (sendlength - 3) % 256;
   ;	
	?debug	L 39
	mov	al,byte ptr [bp-11]
	mov	ah,0
	add	ax,-3
	mov	bx,256
	cwd	
	idiv	bx
	mov	byte ptr [bp-8],dl
   ;	
   ;	  send_comm[3] = BOOT_DOWNLOADFILE;
   ;	
	?debug	L 40
	mov	byte ptr [bp-7],224
   ;	
   ;	  send_comm[4] = BOOT_STARTDOWN;             //命令类型 请求下载;
   ;	
	?debug	L 41
	mov	byte ptr [bp-6],3
   ;	
   ;	  crc = 0;
   ;	
	?debug	L 42
	mov	byte ptr [bp-12],0
   ;	
   ;	  for(i=1; i<(sendlength - 1); i++)
   ;	
	?debug	L 43
	mov	byte ptr [bp-13],1
	jmp	short @1@226
@1@170:
   ;	
   ;	    crc += send_comm[i];
   ;	
	?debug	L 44
	mov	al,byte ptr [bp-13]
	mov	ah,0
	lea	dx,word ptr [bp-10]
	add	ax,dx
	mov	bx,ax
	mov	al,byte ptr ss:[bx]
	add	byte ptr [bp-12],al
	?debug	L 43
	inc	byte ptr [bp-13]
@1@226:
	mov	al,byte ptr [bp-13]
	mov	ah,0
	mov	dl,byte ptr [bp-11]
	mov	dh,0
	dec	dx
	cmp	ax,dx
	jl	short @1@170
   ;	
   ;	  send_comm[sendlength - 1] = crc;
   ;	
	?debug	L 45
	mov	al,byte ptr [bp-11]
	mov	ah,0
	lea	dx,word ptr [bp-11]
	add	ax,dx
	mov	dl,byte ptr [bp-12]
	mov	bx,ax
	mov	byte ptr ss:[bx],dl
   ;	
   ;	
   ;	  outportb(UART0_TX_DATA, 0x66);
   ;	
	?debug	L 47
	mov	dx,-127
	mov	al,102
	out	dx,al
   ;	
   ;	  outportb(UART0_TX_DATA, 0x77);
   ;	
	?debug	L 48
	mov	dx,-127
	mov	al,119
	out	dx,al
   ;	
   ;	  boot_down_flag = 0;
   ;	
	?debug	L 49
	mov	byte ptr DGROUP:_boot_down_flag,0
   ;	
   ;	  for(i=0; i<sendlength; i++)
   ;	
	?debug	L 50
	mov	byte ptr [bp-13],0
	jmp	short @1@394
@1@282:
	jmp	short @1@310
@1@310:
   ;	
   ;	   {
   ;	    while((inportb(UART0_TX_STATUS)&0x1)!=0x1);	// 等待发送保持器为空
   ;	
	?debug	L 52
	mov	dx,-128
	in	al,dx
	mov	ah,0
	and	ax,1
	cmp	ax,1
	jne	short @1@310
   ;	
   ;	    outportb(UART0_TX_DATA, send_comm[i]);				// 发送字符
   ;	
	?debug	L 53
	mov	al,byte ptr [bp-13]
	mov	ah,0
	lea	dx,word ptr [bp-10]
	add	ax,dx
	mov	bx,ax
	mov	al,byte ptr ss:[bx]
	mov	dx,-127
	out	dx,al
	?debug	L 50
	inc	byte ptr [bp-13]
@1@394:
	mov	al,byte ptr [bp-13]
	cmp	al,byte ptr [bp-11]
	jb	short @1@282
@1@422:
   ;	
   ;	   }
   ;	
   ;	       // outportb(UART0_TX_DATA, 0x55);
   ;		//InitComm(UART1_TX_STATUS, 115200, 3, 0xc7, 1);
   ;	       // xtm_printf("loader\n");
   ;	  // delay(1000);
   ;	   while(1)
   ;	    {
   ;	     Boot_comm_upgrade();
   ;	
	?debug	L 62
	call	far ptr _Boot_comm_upgrade
   ;	
   ;	    // delay(1);
   ;	     if(boot_down_flag == 0)
   ;	
	?debug	L 64
	cmp	byte ptr DGROUP:_boot_down_flag,0
	jne	short @1@478
   ;	
   ;	       boot_dtime ++;
   ;	
	?debug	L 65
	inc	word ptr DGROUP:_boot_dtime
@1@478:
   ;	
   ;	     if(boot_dtime >= 15000)
   ;	
	?debug	L 66
	cmp	word ptr DGROUP:_boot_dtime,15000
	jb	short @1@534
   ;	
   ;	       break;
   ;	
	?debug	L 67
	jmp	short @1@562
@1@534:
	jmp	short @1@422
@1@562:
   ;	
   ;	    }
   ;	 /*  boot_dtime = 0;
   ;	   SetPTC2(1);
   ;	   setvect(12, Boot_TimeInt);
   ;	
   ;	   while(boot_dtime < 1000);   */
   ;	  //
   ;	/*   while(boot_down_flag == 1)
   ;	    {
   ;	    // outportb(UART0_TX_DATA, boot_dtime);
   ;	    // Boot_Com1Int();
   ;	     DelayMs(20);
   ;	     if(boot_down_flag == 0)
   ;	       boot_dtime ++;
   ;	     if(boot_dtime >= 1500)
   ;	       break;
   ;	    }         */
   ;	
   ;	    //Boot_comm_upgrade();
   ;	}
   ;	
	?debug	L 87
	mov	sp,bp
	pop	bp
	ret	
	?debug	C E318000A001A08
	?debug	C E601690802F3FF00036372630802F4FF000A7365+
	?debug	C 6E646C656E6774680802F5FF000973656E645F63+
	?debug	C 6F6D6D1802F6FF00
	?debug	E
	?debug	E
_loader	endp
   ;	
   ;	void Boot_InitComm(INT16U port, INT32U baud, INT8U datafrm, INT8U fifolevel, INT8U freq)
   ;	
	?debug	L 89
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
   ;	        INT16U i;
   ;	    /* set baudrate */
   ;	
   ;	    //	gen = 50000000UL/baud/freq;
   ;	        gen = CPU_MAIN_FREQ/baud/freq;   //20120222 改晶振
   ;	
	?debug	L 96
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
	?debug	L 98
	mov	dx,word ptr [bp+6]
	add	dx,4
	mov	al,byte ptr [bp-2]
	and	al,255
	out	dx,al
   ;	
   ;	   outportb(port+5, (gen>>8));
   ;	
	?debug	L 99
	mov	ax,word ptr [bp-2]
	mov	cl,8
	shr	ax,cl
	mov	dx,word ptr [bp+6]
	add	dx,5
	out	dx,al
   ;	
   ;	   outportb(port+2, fifolevel);
   ;	
	?debug	L 100
	mov	dx,word ptr [bp+6]
	add	dx,2
	mov	al,byte ptr [bp+14]
	out	dx,al
   ;	
   ;	
   ;	  dtemp_valid = 0;
   ;	
	?debug	L 102
	mov	byte ptr DGROUP:_dtemp_valid,0
   ;	
   ;	  dtemp_pos = 0;
   ;	
	?debug	L 103
	mov	word ptr DGROUP:_dtemp_pos,0
   ;	
   ;	 /* setvect(13, Boot_Com1Int);
   ;	
   ;	  outportb(INT_MASK, 0x0);//高四个中断使能，低四个中断关闭 1：屏蔽 0:使能
   ;	  outportb(INT_EDGE, 0x00);//都是上升沿触发
   ;	
   ;	  outportb(INT_VECTOR, 0x08);//中断号为8-15
   ;	  outportb(UART0_RX_CTRL, 0xc0);//串口0接收中断使能
   ;	  enable();      */
   ;	}
   ;	
	?debug	L 112
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
   ;	void interrupt Boot_TimeInt(void)
   ;	
	?debug	L 114
	assume	cs:LOADER_TEXT
_Boot_TimeInt	proc	far
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
   ;	  outportb(PTC2_CTRL0,0x41);
   ;	
	?debug	L 116
	mov	dx,-58
	mov	al,65
	out	dx,al
   ;	
   ;	  boot_dtime ++;
   ;	
	?debug	L 117
	inc	word ptr DGROUP:_boot_dtime
   ;	
   ;	  outportb(PTC2_CTRL0,0x21);
   ;	
	?debug	L 118
	mov	dx,-58
	mov	al,33
	out	dx,al
   ;	
   ;	}
   ;	
	?debug	L 119
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
_Boot_TimeInt	endp
   ;	
   ;	void interrupt Boot_Com1Int(void)
   ;	
	?debug	L 121
	assume	cs:LOADER_TEXT
_Boot_Com1Int	proc	far
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
	sub	sp,4
	?debug	B
   ;	
   ;	{
   ;	  char buff[4];
   ;	  INT8U recv_byte, crc;
   ;	  INT16U i;
   ;	      	asm sti
   ;	
	?debug	L 126
 	sti	
	jmp	@4@590
@4@86:
   ;	
   ;		
   ;	 	while((inportb(UART0_RX_CTRL)&0x10)==0x10)
   ;	         {
   ;	          recv_byte = inportb(UART0_RX_DATA);
   ;	
	?debug	L 130
	mov	dx,-125
	in	al,dx
	mov	byte ptr [bp-1],al
   ;	
   ;	          if(dtemp_valid == 0)
   ;	
	?debug	L 131
	cmp	byte ptr DGROUP:_dtemp_valid,0
	jne	short @4@198
   ;	
   ;	           {
   ;	             if(recv_byte == 0xAA)      //包开始
   ;	
	?debug	L 133
	cmp	byte ptr [bp-1],170
	jne	short @4@170
   ;	
   ;	              {
   ;	               dtemp_valid = 1;
   ;	
	?debug	L 135
	mov	byte ptr DGROUP:_dtemp_valid,1
   ;	
   ;	               dtemp_pos = 0;
   ;	
	?debug	L 136
	mov	word ptr DGROUP:_dtemp_pos,0
   ;	
   ;	               boot_dtemp[dtemp_pos] = recv_byte;
   ;	
	?debug	L 137
	mov	bx,word ptr DGROUP:_dtemp_pos
	mov	al,byte ptr [bp-1]
	mov	byte ptr DGROUP:_boot_dtemp[bx],al
   ;	
   ;	               dtemp_pos ++;
   ;	
	?debug	L 138
	inc	word ptr DGROUP:_dtemp_pos
@4@170:
   ;	
   ;	              }
   ;	           }
   ;	
	?debug	L 140
	jmp	@4@590
@4@198:
   ;	
   ;	          else
   ;	           {
   ;	            boot_dtemp[dtemp_pos] = recv_byte;
   ;	
	?debug	L 143
	mov	bx,word ptr DGROUP:_dtemp_pos
	mov	al,byte ptr [bp-1]
	mov	byte ptr DGROUP:_boot_dtemp[bx],al
   ;	
   ;	            dtemp_pos ++;
   ;	
	?debug	L 144
	inc	word ptr DGROUP:_dtemp_pos
   ;	
   ;	            if(dtemp_pos == 3)
   ;	
	?debug	L 145
	cmp	word ptr DGROUP:_dtemp_pos,3
	jne	short @4@254
   ;	
   ;	             {
   ;	              dtemp_len = boot_dtemp[1];
   ;	
	?debug	L 147
	mov	al,byte ptr DGROUP:_boot_dtemp+1
	mov	ah,0
	mov	word ptr DGROUP:_dtemp_len,ax
   ;	
   ;	              dtemp_len <<= 8;//*256
   ;	
	?debug	L 148
	mov	cl,8
	shl	word ptr DGROUP:_dtemp_len,cl
   ;	
   ;	              dtemp_len += boot_dtemp[2];
   ;	
	?debug	L 149
	mov	al,byte ptr DGROUP:_boot_dtemp+2
	mov	ah,0
	add	word ptr DGROUP:_dtemp_len,ax
@4@254:
   ;	
   ;	             }
   ;	            if(dtemp_pos == (dtemp_len + 3))   //接收到一个包
   ;	
	?debug	L 151
	mov	ax,word ptr DGROUP:_dtemp_len
	add	ax,3
	cmp	ax,word ptr DGROUP:_dtemp_pos
	je	@@0
	jmp	@4@590
@@0:
   ;	
   ;	             {
   ;	               boot_dtime = 0;
   ;	
	?debug	L 153
	mov	word ptr DGROUP:_boot_dtime,0
   ;	
   ;	               boot_down_flag = 1;
   ;	
	?debug	L 154
	mov	byte ptr DGROUP:_boot_down_flag,1
   ;	
   ;	               boot_dtemp[4] = BOOT_REPLY;
   ;	
	?debug	L 155
	mov	byte ptr DGROUP:_boot_dtemp+4,2
   ;	
   ;	               crc = 0;
   ;	
	?debug	L 156
	mov	byte ptr [bp-2],0
   ;	
   ;	               for(i=1; i<(dtemp_pos - 1); i++)
   ;	
	?debug	L 157
	mov	word ptr [bp-4],1
	jmp	short @4@366
@4@310:
   ;	
   ;	                 crc += boot_dtemp[i];
   ;	
	?debug	L 158
	mov	bx,word ptr [bp-4]
	mov	al,byte ptr DGROUP:_boot_dtemp[bx]
	add	byte ptr [bp-2],al
	?debug	L 157
	inc	word ptr [bp-4]
@4@366:
	mov	ax,word ptr DGROUP:_dtemp_pos
	dec	ax
	cmp	ax,word ptr [bp-4]
	ja	short @4@310
   ;	
   ;	               boot_dtemp[dtemp_pos - 1] = crc;
   ;	
	?debug	L 159
	mov	bx,word ptr DGROUP:_dtemp_pos
	dec	bx
	mov	al,byte ptr [bp-2]
	mov	byte ptr DGROUP:_boot_dtemp[bx],al
   ;	
   ;	               for(i=0; i<dtemp_pos; i++)
   ;	
	?debug	L 160
	mov	word ptr [bp-4],0
	jmp	short @4@534
@4@422:
	jmp	short @4@450
@4@450:
   ;	
   ;	                {
   ;	                 while((inportb(UART0_TX_STATUS)&0x1)!=0x1);	// 等待发送保持器为空
   ;	
	?debug	L 162
	mov	dx,-128
	in	al,dx
	mov	ah,0
	and	ax,1
	cmp	ax,1
	jne	short @4@450
   ;	
   ;	                 outportb(UART0_TX_DATA, boot_dtemp[i]);				// 发送字符
   ;	
	?debug	L 163
	mov	bx,word ptr [bp-4]
	mov	al,byte ptr DGROUP:_boot_dtemp[bx]
	mov	dx,-127
	out	dx,al
	?debug	L 160
	inc	word ptr [bp-4]
@4@534:
	mov	ax,word ptr [bp-4]
	cmp	ax,word ptr DGROUP:_dtemp_pos
	jb	short @4@422
   ;	
   ;	                }
   ;	               dtemp_valid = 0;
   ;	
	?debug	L 165
	mov	byte ptr DGROUP:_dtemp_valid,0
   ;	
   ;	               dtemp_pos = 0;
   ;	
	?debug	L 166
	mov	word ptr DGROUP:_dtemp_pos,0
@4@590:
	?debug	L 128
	mov	dx,-126
	in	al,dx
	mov	ah,0
	and	ax,16
	cmp	ax,16
	jne	@@1
	jmp	@4@86
@@1:
   ;	
   ;	             }
   ;	           }
   ;		 }
   ;	}
   ;	
	?debug	L 170
	mov	sp,bp
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
	?debug	C E601690A02FCFF00036372630802FEFF00097265+
	?debug	C 63765F627974650802FFFF00
	?debug	E
	?debug	E
_Boot_Com1Int	endp
   ;	
   ;	void Boot_comm_upgrade(void)    //20110608 xu 初始升级
   ;	
	?debug	L 172
	assume	cs:LOADER_TEXT
_Boot_comm_upgrade	proc	far
	?debug	B
	push	bp
	mov	bp,sp
	sub	sp,4
	?debug	B
	jmp	@5@562
@5@58:
   ;	
   ;	{
   ;	  char buff[4];
   ;	  INT8U recv_byte, crc;
   ;	  INT16U i;
   ;		
   ;	 	while((inportb(UART0_RX_CTRL)&0x10)==0x10)
   ;	         {
   ;	          recv_byte = inportb(UART0_RX_DATA);
   ;	
	?debug	L 180
	mov	dx,-125
	in	al,dx
	mov	byte ptr [bp-1],al
   ;	
   ;	          if(dtemp_valid == 0)
   ;	
	?debug	L 181
	cmp	byte ptr DGROUP:_dtemp_valid,0
	jne	short @5@170
   ;	
   ;	           {
   ;	             if(recv_byte == 0xAA)      //包开始
   ;	
	?debug	L 183
	cmp	byte ptr [bp-1],170
	jne	short @5@142
   ;	
   ;	              {
   ;	               dtemp_valid = 1;
   ;	
	?debug	L 185
	mov	byte ptr DGROUP:_dtemp_valid,1
   ;	
   ;	               dtemp_pos = 0;
   ;	
	?debug	L 186
	mov	word ptr DGROUP:_dtemp_pos,0
   ;	
   ;	               boot_dtemp[dtemp_pos] = recv_byte;
   ;	
	?debug	L 187
	mov	bx,word ptr DGROUP:_dtemp_pos
	mov	al,byte ptr [bp-1]
	mov	byte ptr DGROUP:_boot_dtemp[bx],al
   ;	
   ;	               dtemp_pos ++;
   ;	
	?debug	L 188
	inc	word ptr DGROUP:_dtemp_pos
@5@142:
   ;	
   ;	              }
   ;	           }
   ;	
	?debug	L 190
	jmp	@5@562
@5@170:
   ;	
   ;	          else
   ;	           {
   ;	            boot_dtemp[dtemp_pos] = recv_byte;
   ;	
	?debug	L 193
	mov	bx,word ptr DGROUP:_dtemp_pos
	mov	al,byte ptr [bp-1]
	mov	byte ptr DGROUP:_boot_dtemp[bx],al
   ;	
   ;	            dtemp_pos ++;
   ;	
	?debug	L 194
	inc	word ptr DGROUP:_dtemp_pos
   ;	
   ;	            if(dtemp_pos == 3)
   ;	
	?debug	L 195
	cmp	word ptr DGROUP:_dtemp_pos,3
	jne	short @5@226
   ;	
   ;	             {
   ;	              dtemp_len = boot_dtemp[1];
   ;	
	?debug	L 197
	mov	al,byte ptr DGROUP:_boot_dtemp+1
	mov	ah,0
	mov	word ptr DGROUP:_dtemp_len,ax
   ;	
   ;	              dtemp_len <<= 8;//*256
   ;	
	?debug	L 198
	mov	cl,8
	shl	word ptr DGROUP:_dtemp_len,cl
   ;	
   ;	              dtemp_len += boot_dtemp[2];
   ;	
	?debug	L 199
	mov	al,byte ptr DGROUP:_boot_dtemp+2
	mov	ah,0
	add	word ptr DGROUP:_dtemp_len,ax
@5@226:
   ;	
   ;	             }
   ;	            if(dtemp_pos == (dtemp_len + 3))   //接收到一个包
   ;	
	?debug	L 201
	mov	ax,word ptr DGROUP:_dtemp_len
	add	ax,3
	cmp	ax,word ptr DGROUP:_dtemp_pos
	je	@@2
	jmp	@5@562
@@2:
   ;	
   ;	             {
   ;	               boot_dtime = 0;
   ;	
	?debug	L 203
	mov	word ptr DGROUP:_boot_dtime,0
   ;	
   ;	               boot_down_flag = 1;
   ;	
	?debug	L 204
	mov	byte ptr DGROUP:_boot_down_flag,1
   ;	
   ;	               boot_dtemp[4] = BOOT_REPLY;
   ;	
	?debug	L 205
	mov	byte ptr DGROUP:_boot_dtemp+4,2
   ;	
   ;	               crc = 0;
   ;	
	?debug	L 206
	mov	byte ptr [bp-2],0
   ;	
   ;	               for(i=1; i<(dtemp_pos - 1); i++)
   ;	
	?debug	L 207
	mov	word ptr [bp-4],1
	jmp	short @5@338
@5@282:
   ;	
   ;	                 crc += boot_dtemp[i];
   ;	
	?debug	L 208
	mov	bx,word ptr [bp-4]
	mov	al,byte ptr DGROUP:_boot_dtemp[bx]
	add	byte ptr [bp-2],al
	?debug	L 207
	inc	word ptr [bp-4]
@5@338:
	mov	ax,word ptr DGROUP:_dtemp_pos
	dec	ax
	cmp	ax,word ptr [bp-4]
	ja	short @5@282
   ;	
   ;	               boot_dtemp[dtemp_pos - 1] = crc;
   ;	
	?debug	L 209
	mov	bx,word ptr DGROUP:_dtemp_pos
	dec	bx
	mov	al,byte ptr [bp-2]
	mov	byte ptr DGROUP:_boot_dtemp[bx],al
   ;	
   ;	               for(i=0; i<dtemp_pos; i++)
   ;	
	?debug	L 210
	mov	word ptr [bp-4],0
	jmp	short @5@506
@5@394:
	jmp	short @5@422
@5@422:
   ;	
   ;	                {
   ;	                 while((inportb(UART0_TX_STATUS)&0x1)!=0x1);	// 等待发送保持器为空
   ;	
	?debug	L 212
	mov	dx,-128
	in	al,dx
	mov	ah,0
	and	ax,1
	cmp	ax,1
	jne	short @5@422
   ;	
   ;	                 outportb(UART0_TX_DATA, boot_dtemp[i]);				// 发送字符
   ;	
	?debug	L 213
	mov	bx,word ptr [bp-4]
	mov	al,byte ptr DGROUP:_boot_dtemp[bx]
	mov	dx,-127
	out	dx,al
	?debug	L 210
	inc	word ptr [bp-4]
@5@506:
	mov	ax,word ptr [bp-4]
	cmp	ax,word ptr DGROUP:_dtemp_pos
	jb	short @5@394
   ;	
   ;	                }
   ;	               dtemp_valid = 0;
   ;	
	?debug	L 215
	mov	byte ptr DGROUP:_dtemp_valid,0
   ;	
   ;	               dtemp_pos = 0;
   ;	
	?debug	L 216
	mov	word ptr DGROUP:_dtemp_pos,0
@5@562:
	?debug	L 178
	mov	dx,-126
	in	al,dx
	mov	ah,0
	and	ax,16
	cmp	ax,16
	jne	@@3
	jmp	@5@58
@@3:
   ;	
   ;	             }
   ;	           }
   ;		 }
   ;	#if 0
   ;	    	INT32U padd;
   ;		INT16U i, j;
   ;		INT8U k;
   ;		while (boot_comm.RP!=boot_comm.WP){
   ;	     	switch (boot_comm.DP){
   ;	     		case 0:
   ;	     			if (boot_comm.Rbuf[boot_comm.RP]==0xAA){
   ;	     				boot_comm.DP=1;
   ;	     				boot_comm.Dbuf[0]=0xAA;
   ;	     				boot_comm.TP=boot_comm.RP+1;
   ;	     				if (boot_comm.TP>=COMBUFFLEN)
   ;	     					boot_comm.TP=0;
   ;	     			}
   ;	     			break;
   ;	     		case 1:
   ;	     			if (boot_comm.Rbuf[boot_comm.RP]==0x55){
   ;	     				boot_comm.DP=2;
   ;	     				boot_comm.Dbuf[1]=0x55;
   ;	     			}else{
   ;	     				boot_comm.DP=0;
   ;	     				boot_comm.RP=boot_comm.TP;
   ;	     			}
   ;	     			break;
   ;	     		case 2:
   ;	     			if (boot_comm.Rbuf[boot_comm.RP]<4){
   ;	     				boot_comm.DP=3;
   ;	     				boot_comm.Dbuf[2]=boot_comm.Rbuf[boot_comm.RP];
   ;	     			}else{
   ;	     				boot_comm.DP=0;
   ;	     				boot_comm.RP=boot_comm.TP;
   ;	     			}
   ;	     			break;
   ;	     		default:
   ;	     			boot_comm.Dbuf[boot_comm.DP]=boot_comm.Rbuf[boot_comm.RP];
   ;	     			boot_comm.DP++;
   ;	     			if (boot_comm.DP>288){
   ;	     				boot_comm.DP=0;
   ;	     				boot_comm.RP=boot_comm.TP;
   ;	     			}
   ;	     			break;
   ;	     	}
   ;				boot_comm.RP++;
   ;				if(boot_comm.RP>=COMBUFFLEN)
   ;	  	  	boot_comm.RP=0;
   ;	  	  if (boot_comm.DP>4){
   ;	  	  	i=boot_comm.Dbuf[2]*256+boot_comm.Dbuf[3];
   ;	  	  	if (i==(boot_comm.DP-5)){
   ;	  	  		k=0;
   ;	  	  		for (j=0;j<boot_comm.DP-1;j++){
   ;	  	  			k+=boot_comm.Dbuf[j];
   ;						}
   ;	  	  		if (k==boot_comm.Dbuf[boot_comm.DP-1]){
   ;	  	  			switch(boot_comm.Dbuf[4]){
   ;	  	  				case 0xb0://answer
   ;	  	  					boot_dtime=100;
   ;	  	  					boot_dtemp[0]=0xaa;
   ;	  	  					boot_dtemp[1]=0x55;
   ;	  	  					boot_dtemp[2]=0x1;
   ;	  	  					boot_dtemp[3]=0xb0;
   ;	  	  					boot_dtemp[4]=0;
   ;	  	  					for (i=0;i<4;i++)
   ;	  	  						boot_dtemp[4]+=boot_dtemp[i];
   ;	  	  					for (i=0;i<5;i++){
   ;										while((inportb(UART0_TX_STATUS)&0x1)!=0x1);	// 等待发送保持器为空
   ;										outportb(UART0_TX_DATA, boot_dtemp[i]);				// 发送字符
   ;	  	  					}
   ;	  	  					break;
   ;	  	  				case 0xa0://write flash
   ;	  	  					boot_dtime=100;
   ;	  	  					for (j=0;j<i-3;j++)
   ;	  	  						boot_dtemp[j]=boot_comm.Dbuf[j+7];
   ;	  	  					while(j<256){
   ;	  	  						boot_dtemp[j]=0x0;
   ;	  	  						j++;
   ;	  	  					}
   ;	  	  					j=boot_comm.Dbuf[5];
   ;	  	  					j<<=8;//*256
   ;	  	  					j+=boot_comm.Dbuf[6];
   ;	
   ;	  	  			  /*		padd=j;
   ;	  	  					padd<<=8;
   ;	  	  					if (!(j%16)){
   ;	                                                        w25x32writeenable();
   ;	  	  						sectererase25x32(padd);
   ;	  	  						read25x32status();
   ;	  	  					}
   ;	                                                w25x32writeenable();
   ;	  	  					programpage25x32(padd,boot_dtemp,256);
   ;	  	  					w25x32writedisable();     */
   ;	                                                
   ;	  	  					boot_dtemp[0]=0xaa;
   ;	  	  					boot_dtemp[1]=0x55;
   ;	  	  					boot_dtemp[2]=0x3;
   ;	  	  					boot_dtemp[3]=0xa0;
   ;	  	  					boot_dtemp[4]=boot_comm.Dbuf[5];
   ;	  	  					boot_dtemp[5]=boot_comm.Dbuf[6];
   ;	  	  					boot_dtemp[6]=0;
   ;	  	  					for (i=0;i<6;i++)
   ;	  	  						boot_dtemp[6]+=boot_dtemp[i];
   ;	  	  					for (i=0;i<7;i++){
   ;										while((inportb(UART0_TX_STATUS)&0x1)!=0x1);	// 等待发送保持器为空
   ;										outportb(UART0_TX_DATA, boot_dtemp[i]);				// 发送字符
   ;	  	  					}
   ;	  	  					break;
   ;	     					default:
   ;	     						break;
   ;	  	  			}
   ;	  	  		}else
   ;	     				boot_comm.RP=boot_comm.TP;
   ;	     			boot_comm.DP=0;
   ;	  	  	}
   ;	  	  }
   ;	        }
   ;	#endif        
   ;	}
   ;	
	?debug	L 335
	mov	sp,bp
	pop	bp
	ret	
	?debug	C E601690A02FCFF00036372630802FEFF00097265+
	?debug	C 63765F627974650802FFFF00
	?debug	E
	?debug	E
_Boot_comm_upgrade	endp
LOADER_TEXT	ends
_BSS	segment word public 'BSS'
_boot_dtemp	label	byte
	db	512 dup (?)
_dtemp_len	label	word
	db	2 dup (?)
_dtemp_pos	label	word
	db	2 dup (?)
_dtemp_valid	label	byte
	db	1 dup (?)
_boot_dtime	label	word
	db	2 dup (?)
	?debug	C E9
	?debug	C FA00000000
_BSS	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
LOADER_TEXT	segment byte public 'BOOT'
LOADER_TEXT	ends
	extrn	F_LUDIV@:far
	public	_loader
	public	_Boot_comm_upgrade
	public	_Boot_TimeInt
	public	_Boot_Com1Int
	public	_Boot_InitComm
	public	_boot_dtemp
	public	_dtemp_len
	public	_dtemp_pos
	public	_dtemp_valid
	extrn	_boot_down_flag:byte
	public	_boot_dtime
_s@	equ	s@
	?debug	C EA010C
	?debug	C E31900000023040500
	?debug	C EB08465F4C55444956401900
	?debug	C E31A00000023010400
	?debug	C EC075F6C6F616465721A1800
	?debug	C E31B00000023010400
	?debug	C EC125F426F6F745F636F6D6D5F75706772616465+
	?debug	C 1B1800
	?debug	C E31C00000023010700
	?debug	C EC0D5F426F6F745F54696D65496E741C9800
	?debug	C E31D00000023010700
	?debug	C EC0D5F426F6F745F436F6D31496E741D9800
	?debug	C E31E00000023010400
	?debug	C EC0E5F426F6F745F496E6974436F6D6D1E1800
	?debug	C E31F0000021A08
	?debug	C EC0B5F626F6F745F6474656D701F0000
	?debug	C EC0A5F6474656D705F6C656E0A0000
	?debug	C EC0A5F6474656D705F706F730A0000
	?debug	C EC0C5F6474656D705F76616C6964080000
	?debug	C EB0F5F626F6F745F646F776E5F666C61670800
	?debug	C EC0B5F626F6F745F6474696D650A0000
	?debug	C E6084F535F464C4147530A0600094F535F435055+
	?debug	C 5F53520A0600064F535F53544B0A060004465036+
	?debug	C 340F060004465033320E060006494E5433325306+
	?debug	C 060006494E543332550C060006494E5431365304+
	?debug	C 06000475696E740A060006494E543136550A0600+
	?debug	C 05494E5438530206000575636861720806000549+
	?debug	C 4E54385508060007424F4F4C45414E0806000544+
	?debug	C 574F52440C060004574F52440A06000442595445+
	?debug	C 08060007464C4F4154383010060007464C4F4154+
	?debug	C 36340F060007464C4F415433320E06000655494E+
	?debug	C 5433320C060005494E5433320606000655494E54+
	?debug	C 31360A060005494E5431360406000555494E5438+
	?debug	C 08060004494E5438020600074C444F55424C4510+
	?debug	C 060006444F55424C450F060005464C4F41540E06+
	?debug	C 0005554C4F4E470C0600044C4F4E470606000455+
	?debug	C 494E540A060003494E5404060005554348415208+
	?debug	C 06000443484152020600034249540A0600066670+
	?debug	C 6F735F740606000C7369675F61746F6D69635F74+
	?debug	C 0406000777636861725F740206000673697A655F+
	?debug	C 740A060009707472646966665F74060600
	end

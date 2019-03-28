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
	?debug	S "lib_src\ov7725.c"
	?debug	C E999486F42106C69625F7372635C6F76373732352E63
	?debug	C E97459E34017617263682F696E636C7564655C696E636C75646573+
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
	?debug	C E94059CA18202E2E5C2E2E5C746F6F6C735C626333315C696E636C+
	?debug	C 7564655C616C6C6F632E68
	?debug	C E96B869E3C0F6B65726E656C5C6F735F6370752E68
	?debug	C E9667CD530116B65726E656C5C6F735F6366675F722E68
	?debug	C E96D529C4215617263682F696E636C7564655C6D5F696E69742E68
	?debug	C E9A24B504212617263682F696E636C7564655C6370752E68
	?debug	C E95A6DDB4214617263682F696E636C7564655C785F6370752E68
	?debug	C E9D4796C4117617263682F696E636C7564655C785F7072696E7466+
	?debug	C 2E68
OV7725_TEXT	segment byte public 'CODE'
OV7725_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:OV7725_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
OV7725_TEXT	segment byte public 'CODE'
	?debug	C E801106C69625F7372635C6F76373732352E6399+
	?debug	C 486F42
   ;	
   ;	unsigned int init_camera(INT8U FrameRate)
   ;	
	?debug	L 40
	assume	cs:OV7725_TEXT
_init_camera	proc	far
	?debug	B
	push	bp
	mov	bp,sp
	?debug	C E6094672616D6552617465080A060000
	?debug	B
   ;	
   ;	{
   ;	   INT8U val;
   ;	    WriteDeviceAddress=0x42;
   ;	
	?debug	L 43
	mov	byte ptr DGROUP:_WriteDeviceAddress,66
   ;	
   ;	    ReadDeviceAddress=0x43;
   ;	
	?debug	L 44
	mov	byte ptr DGROUP:_ReadDeviceAddress,67
   ;	
   ;	
   ;	    nop();
   ;	
	?debug	L 46
 	nop	
   ;	
   ;	    nop();
   ;	
	?debug	L 47
 	nop	
   ;	
   ;	     if((Read7725(0x0a)==0x77)&&(Read7725(0x0b)==0x21)){
   ;	
	?debug	L 48
	mov	al,10
	push	ax
	call	far ptr _Read7725
	pop	cx
	cmp	al,119
	jne	short @1@254
	mov	al,11
	push	ax
	call	far ptr _Read7725
	pop	cx
	cmp	al,33
	jne	short @1@254
   ;	
   ;	        init7725(FrameRate);
   ;	
	?debug	L 49
	mov	al,byte ptr [bp+6]
	push	ax
	call	far ptr _init7725
	pop	cx
   ;	
   ;	        return(0x7725);
   ;	
	?debug	L 50
	mov	ax,30501
@1@198:
	jmp	short @1@282
   ;	
   ;	     }
   ;	
	?debug	L 51
	jmp	short @1@282
@1@254:
   ;	
   ;	     else{
   ;	//        init7113();
   ;	//        init7725(FrameRate);
   ;	        return(0x7113);
   ;	
	?debug	L 55
	mov	ax,28947
	jmp	short @1@198
@1@282:
   ;	
   ;	     }
   ;		
   ;	}
   ;	
	?debug	L 58
	pop	bp
	ret	
	?debug	C E6094672616D6552617465080A060000
	?debug	E
	?debug	E
_init_camera	endp
   ;	
   ;	INT8U Read7725(INT8U RomAddress)
   ;	
	?debug	L 60
	assume	cs:OV7725_TEXT
_Read7725	proc	far
	?debug	B
	push	bp
	mov	bp,sp
	?debug	C E60A526F6D41646472657373080A060000
	?debug	B
   ;	
   ;	{
   ;	
   ;	    //Local.GpioWatchDogStarted = 0;  //在GPIO检测线程中启动WatchDog
   ;	    /* Start() */
   ;	    asm{
   ;	        mov dx, OV7725_PORT
   ;	
	?debug	L 66
	mov	 dx, 02d0H
   ;	
   ;	        in al, dx
   ;	
	?debug	L 67
	in	 al, dx
   ;	
   ;	        or al, OV7725_SDA_H
   ;	
	?debug	L 68
	or	 al, 008H
   ;	
   ;	        out dx, al
   ;	
	?debug	L 69
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 71
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 72
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 73
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 74
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 75
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 76
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 77
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 78
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 79
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 80
	nop	
   ;	
   ;	      #endif
   ;	        or al, OV7725_SCL_H
   ;	
	?debug	L 82
	or	 al, 004H
   ;	
   ;	        out dx, al
   ;	
	?debug	L 83
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 85
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 86
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 87
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 88
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 89
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 90
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 91
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 92
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 93
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 94
	nop	
   ;	
   ;	      #endif
   ;	        and al, OV7725_SDA_L
   ;	
	?debug	L 96
	and	 al, 0f7H
   ;	
   ;	        out dx, al
   ;	
	?debug	L 97
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 99
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 100
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 101
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 102
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 103
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 104
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 105
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 106
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 107
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 108
	nop	
   ;	
   ;	      #endif
   ;	        and al, OV7725_SCL_L
   ;	
	?debug	L 110
	and	 al, 0fbH
   ;	
   ;	        out dx, al
   ;	
	?debug	L 111
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 113
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 114
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 115
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 116
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 117
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 118
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 119
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 120
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 121
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 122
	nop	
   ;	
   ;	      #endif
   ;	    }
   ;	    /* Write8Bit(WriteDeviceAddress); */
   ;	    asm {
   ;	        mov bl, WriteDeviceAddress
   ;	
	?debug	L 127
	mov	 bl, DGROUP:_WriteDeviceAddress
   ;	
   ;	        mov cx, 8
   ;	
	?debug	L 128
	mov	 cx, 8
@2@1486:
   ;	
   ;	    }
   ;	  write8_0:
   ;	    asm{
   ;	        and al, OV7725_SDA_L
   ;	
	?debug	L 132
	and	 al, 0f7H
   ;	
   ;	        test bl, 80h //0x80
   ;	
	?debug	L 133
	test	 bl, 80h 
   ;	
   ;	        jz  pdsda002
   ;	
	?debug	L 134
	je	short @2@1598
   ;	
   ;	        or al, OV7725_SDA_H
   ;	
	?debug	L 135
	or	 al, 008H
@2@1598:
   ;	
   ;	    }
   ;	  pdsda002:
   ;	    asm{
   ;	    	  out dx, al
   ;	
	?debug	L 139
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 141
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 142
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 143
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 144
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 145
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 146
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 147
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 148
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 149
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 150
	nop	
   ;	
   ;	      #endif
   ;	    	  nop
   ;	
	?debug	L 152
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 153
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 154
	nop	
   ;	
   ;	        or al, OV7725_SCL_H
   ;	
	?debug	L 155
	or	 al, 004H
   ;	
   ;	        out dx, al
   ;	
	?debug	L 156
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 158
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 159
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 160
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 161
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 162
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 163
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 164
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 165
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 166
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 167
	nop	
   ;	
   ;	      #endif
   ;	        nop
   ;	
	?debug	L 169
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 170
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 171
	nop	
   ;	
   ;	        and al, OV7725_SCL_L
   ;	
	?debug	L 172
	and	 al, 0fbH
   ;	
   ;	        out dx, al
   ;	
	?debug	L 173
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 175
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 176
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 177
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 178
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 179
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 180
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 181
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 182
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 183
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 184
	nop	
   ;	
   ;	      #endif
   ;	        shl bl, 1
   ;	
	?debug	L 186
	shl	 bl, 1
   ;	
   ;	        loop write8_0
   ;	
	?debug	L 187
	loop	short @2@1486
   ;	
   ;	
   ;	
   ;	    }
   ;	    /* TestAck(); */
   ;	    asm{
   ;	    	  nop
   ;	
	?debug	L 193
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 194
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 195
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 196
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 197
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 198
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 199
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 200
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 201
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 202
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 203
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 204
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 205
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 206
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 207
	nop	
   ;	
   ;	        or al, OV7725_SCL_H
   ;	
	?debug	L 208
	or	 al, 004H
   ;	
   ;	        out dx, al
   ;	
	?debug	L 209
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 211
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 212
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 213
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 214
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 215
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 216
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 217
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 218
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 219
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 220
	nop	
   ;	
   ;	      #endif
   ;	        nop
   ;	
	?debug	L 222
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 223
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 224
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 225
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 226
	nop	
   ;	
   ;	         and  al, OV7725_SCL_L
   ;	
	?debug	L 227
	and	  al, 0fbH
   ;	
   ;	        out dx, al
   ;	
	?debug	L 228
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 230
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 231
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 232
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 233
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 234
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 235
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 236
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 237
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 238
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 239
	nop	
   ;	
   ;	      #endif
   ;	    	  nop
   ;	
	?debug	L 241
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 242
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 243
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 244
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 245
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 246
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 247
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 248
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 249
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 250
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 251
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 252
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 253
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 254
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 255
	nop	
   ;	
   ;	    }
   ;	
   ;	    /* Write8Bit(RomAddress); */
   ;	    asm {
   ;	        mov bl, RomAddress
   ;	
	?debug	L 260
	mov	 bl, [bp+6]
   ;	
   ;	        mov cx, 8
   ;	
	?debug	L 261
	mov	 cx, 8
@2@4510:
   ;	
   ;	    }
   ;	  write8_41:
   ;	    asm{
   ;	        and al, OV7725_SDA_L
   ;	
	?debug	L 265
	and	 al, 0f7H
   ;	
   ;	        test bl, 80h //0x80
   ;	
	?debug	L 266
	test	 bl, 80h 
   ;	
   ;	        jz  pdsda012
   ;	
	?debug	L 267
	je	short @2@4622
   ;	
   ;	        or al, OV7725_SDA_H
   ;	
	?debug	L 268
	or	 al, 008H
@2@4622:
   ;	
   ;	    }
   ;	  pdsda012:
   ;	    asm{
   ;	        out dx, al
   ;	
	?debug	L 272
	out	 dx, al
   ;	
   ;	        nop
   ;	
	?debug	L 273
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 274
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 275
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 276
	nop	
   ;	
   ;	        or al, OV7725_SCL_H
   ;	
	?debug	L 277
	or	 al, 004H
   ;	
   ;	        out dx, al
   ;	
	?debug	L 278
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 280
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 281
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 282
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 283
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 284
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 285
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 286
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 287
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 288
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 289
	nop	
   ;	
   ;	      #endif
   ;	        nop
   ;	
	?debug	L 291
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 292
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 293
	nop	
   ;	
   ;	        and al, OV7725_SCL_L
   ;	
	?debug	L 294
	and	 al, 0fbH
   ;	
   ;	        out dx, al
   ;	
	?debug	L 295
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 297
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 298
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 299
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 300
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 301
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 302
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 303
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 304
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 305
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 306
	nop	
   ;	
   ;	      #endif
   ;	        shl bl, 1
   ;	
	?debug	L 308
	shl	 bl, 1
   ;	
   ;	        loop write8_41
   ;	
	?debug	L 309
	loop	short @2@4510
   ;	
   ;	
   ;	
   ;	    }
   ;	    /* TestAck(); */
   ;	    asm{
   ;	    	  nop
   ;	
	?debug	L 315
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 316
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 317
	nop	
   ;	
   ;	     	  nop
   ;	
	?debug	L 318
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 319
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 320
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 321
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 322
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 323
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 324
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 325
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 326
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 327
	nop	
   ;	
   ;	        or al, OV7725_SCL_H
   ;	
	?debug	L 328
	or	 al, 004H
   ;	
   ;	        out dx, al
   ;	
	?debug	L 329
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 331
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 332
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 333
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 334
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 335
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 336
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 337
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 338
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 339
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 340
	nop	
   ;	
   ;	      #endif
   ;	        nop
   ;	
	?debug	L 342
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 343
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 344
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 345
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 346
	nop	
   ;	
   ;	        and  al, OV7725_SCL_L
   ;	
	?debug	L 347
	and	  al, 0fbH
   ;	
   ;	        out dx, al
   ;	
	?debug	L 348
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 350
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 351
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 352
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 353
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 354
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 355
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 356
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 357
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 358
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 359
	nop	
   ;	
   ;	      #endif
   ;	    	  nop
   ;	
	?debug	L 361
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 362
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 363
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 364
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 365
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 366
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 367
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 368
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 369
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 370
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 371
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 372
	nop	
   ;	
   ;	    }
   ;	    /* Stop(); */
   ;	    asm{
   ;	        and al, OV7725_SCL_L
   ;	
	?debug	L 376
	and	 al, 0fbH
   ;	
   ;	        out dx, al
   ;	
	?debug	L 377
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 379
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 380
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 381
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 382
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 383
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 384
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 385
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 386
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 387
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 388
	nop	
   ;	
   ;	      #endif
   ;	        and al, OV7725_SDA_L
   ;	
	?debug	L 390
	and	 al, 0f7H
   ;	
   ;	        out dx, al
   ;	
	?debug	L 391
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 393
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 394
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 395
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 396
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 397
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 398
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 399
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 400
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 401
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 402
	nop	
   ;	
   ;	      #endif
   ;	        or  al, OV7725_SCL_H
   ;	
	?debug	L 404
	or	  al, 004H
   ;	
   ;	        out dx, al
   ;	
	?debug	L 405
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 407
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 408
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 409
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 410
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 411
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 412
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 413
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 414
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 415
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 416
	nop	
   ;	
   ;	      #endif
   ;	        nop
   ;	
	?debug	L 418
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 419
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 420
	nop	
   ;	
   ;	        or  al, OV7725_SDA_H
   ;	
	?debug	L 421
	or	  al, 008H
   ;	
   ;	        out dx, al
   ;	
	?debug	L 422
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 424
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 425
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 426
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 427
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 428
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 429
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 430
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 431
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 432
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 433
	nop	
   ;	
   ;	      #endif
   ;	        nop
   ;	
	?debug	L 435
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 436
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 437
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 438
	nop	
   ;	
   ;	
   ;	    }
   ;	    /* Start(); */
   ;	    asm{
   ;	        /* SDA = 1 */
   ;	        or al, OV7725_SDA_H
   ;	
	?debug	L 444
	or	 al, 008H
   ;	
   ;	        out dx, al
   ;	
	?debug	L 445
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 447
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 448
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 449
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 450
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 451
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 452
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 453
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 454
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 455
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 456
	nop	
   ;	
   ;	      #endif
   ;	        /* SCL = 1 */
   ;	        or al, OV7725_SCL_H
   ;	
	?debug	L 459
	or	 al, 004H
   ;	
   ;	        out dx, al
   ;	
	?debug	L 460
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 462
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 463
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 464
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 465
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 466
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 467
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 468
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 469
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 470
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 471
	nop	
   ;	
   ;	      #endif
   ;	        /* SDA = 0 */
   ;	        and al, OV7725_SDA_L
   ;	
	?debug	L 474
	and	 al, 0f7H
   ;	
   ;	        out dx, al
   ;	
	?debug	L 475
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 477
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 478
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 479
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 480
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 481
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 482
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 483
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 484
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 485
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 486
	nop	
   ;	
   ;	      #endif
   ;	        /* SCL = 0 */
   ;	        and al, OV7725_SCL_L
   ;	
	?debug	L 489
	and	 al, 0fbH
   ;	
   ;	        out dx, al
   ;	
	?debug	L 490
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 492
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 493
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 494
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 495
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 496
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 497
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 498
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 499
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 500
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 501
	nop	
   ;	
   ;	      #endif
   ;	    }
   ;	    /* Write8Bit(ReadDviceAddress); */
   ;	    asm {
   ;	        mov bl, ReadDeviceAddress
   ;	
	?debug	L 506
	mov	 bl, DGROUP:_ReadDeviceAddress
   ;	
   ;	        mov cx, 8
   ;	
	?debug	L 507
	mov	 cx, 8
@2@10026:
   ;	
   ;	    }
   ;	  write8_21:
   ;	    asm{
   ;	        and al, OV7725_SDA_L
   ;	
	?debug	L 511
	and	 al, 0f7H
   ;	
   ;	        test bl, 80h //0x80
   ;	
	?debug	L 512
	test	 bl, 80h 
   ;	
   ;	        jz  pdsda022
   ;	
	?debug	L 513
	je	short @2@10138
   ;	
   ;	        or al, OV7725_SDA_H
   ;	
	?debug	L 514
	or	 al, 008H
@2@10138:
   ;	
   ;	    }
   ;	  pdsda022:
   ;	    asm{
   ;	        out dx, al
   ;	
	?debug	L 518
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 520
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 521
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 522
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 523
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 524
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 525
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 526
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 527
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 528
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 529
	nop	
   ;	
   ;	      #endif
   ;	        nop
   ;	
	?debug	L 531
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 532
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 533
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 534
	nop	
   ;	
   ;	        or al, OV7725_SCL_H
   ;	
	?debug	L 535
	or	 al, 004H
   ;	
   ;	        out dx, al
   ;	
	?debug	L 536
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 538
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 539
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 540
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 541
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 542
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 543
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 544
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 545
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 546
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 547
	nop	
   ;	
   ;	      #endif
   ;	        nop
   ;	
	?debug	L 549
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 550
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 551
	nop	
   ;	
   ;	        and al, OV7725_SCL_L
   ;	
	?debug	L 552
	and	 al, 0fbH
   ;	
   ;	        out dx, al
   ;	
	?debug	L 553
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 555
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 556
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 557
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 558
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 559
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 560
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 561
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 562
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 563
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 564
	nop	
   ;	
   ;	      #endif
   ;	        nop
   ;	
	?debug	L 566
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 567
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 568
	nop	
   ;	
   ;	        shl bl, 1
   ;	
	?debug	L 569
	shl	 bl, 1
   ;	
   ;	        loop write8_21
   ;	
	?debug	L 570
	loop	short @2@10026
   ;	
   ;	
   ;	    }
   ;	
   ;	    /* TestAck(); */
   ;	    asm{
   ;	    	  nop
   ;	
	?debug	L 576
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 577
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 578
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 579
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 580
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 581
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 582
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 583
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 584
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 585
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 586
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 587
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 588
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 589
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 590
	nop	
   ;	
   ;	        or al, OV7725_SCL_H
   ;	
	?debug	L 591
	or	 al, 004H
   ;	
   ;	        out dx, al
   ;	
	?debug	L 592
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 594
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 595
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 596
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 597
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 598
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 599
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 600
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 601
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 602
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 603
	nop	
   ;	
   ;	      #endif
   ;	        nop
   ;	
	?debug	L 605
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 606
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 607
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 608
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 609
	nop	
   ;	
   ;	        and  al, OV7725_SCL_L
   ;	
	?debug	L 610
	and	  al, 0fbH
   ;	
   ;	        out dx, al
   ;	
	?debug	L 611
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 613
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 614
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 615
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 616
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 617
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 618
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 619
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 620
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 621
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 622
	nop	
   ;	
   ;	      #endif
   ;	    	  nop
   ;	
	?debug	L 624
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 625
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 626
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 627
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 628
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 629
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 630
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 631
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 632
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 633
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 634
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 635
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 636
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 637
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 638
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 639
	nop	
   ;	
   ;	    }
   ;	    asm{
   ;	        mov cx, 8
   ;	
	?debug	L 642
	mov	 cx, 8
   ;	
   ;	        mov bh, al
   ;	
	?debug	L 643
	mov	 bh, al
   ;	
   ;	
   ;	        mov dx,OV7725_PORT_OEN
   ;	
	?debug	L 645
	mov	 dx,02d1H
   ;	
   ;	      //#if 0       //20120406 xu
   ;	        in al, dx
   ;	
	?debug	L 647
	in	 al, dx
   ;	
   ;	        or al, OV7725_SDA_H //80h //8
   ;	
	?debug	L 648
	or	 al, 008H 
   ;	
   ;	      //#else
   ;	      //  mov al,80h
   ;	      //#endif
   ;	        out dx,al
   ;	
	?debug	L 652
	out	 dx,al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 654
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 655
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 656
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 657
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 658
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 659
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 660
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 661
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 662
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 663
	nop	
   ;	
   ;	      #endif
   ;	        mov al,bh
   ;	
	?debug	L 665
	mov	 al,bh
   ;	
   ;	        mov bl, 0
   ;	
	?debug	L 666
	mov	 bl, 0
   ;	
   ;	        dec dx
   ;	
	?debug	L 667
	dec	 dx
@2@13666:
   ;	
   ;	    }
   ;	  read8:
   ;	    asm{
   ;	        or al, OV7725_SCL_H
   ;	
	?debug	L 671
	or	 al, 004H
   ;	
   ;	        out dx, al
   ;	
	?debug	L 672
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 674
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 675
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 676
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 677
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 678
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 679
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 680
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 681
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 682
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 683
	nop	
   ;	
   ;	      #endif
   ;	        mov bh, al
   ;	
	?debug	L 685
	mov	 bh, al
   ;	
   ;	        nop
   ;	
	?debug	L 686
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 687
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 688
	nop	
   ;	
   ;	        shl bl, 1
   ;	
	?debug	L 689
	shl	 bl, 1
   ;	
   ;	        inc dx
   ;	
	?debug	L 690
	inc	 dx
   ;	
   ;	        inc dx
   ;	
	?debug	L 691
	inc	 dx
   ;	
   ;	        in  al, dx
   ;	
	?debug	L 692
	in	  al, dx
   ;	
   ;	        dec dx
   ;	
	?debug	L 693
	dec	 dx
   ;	
   ;	        dec dx
   ;	
	?debug	L 694
	dec	 dx
   ;	
   ;	
   ;	        test al, OV7725_SDA_H
   ;	
	?debug	L 696
	test	 al, 008H
   ;	
   ;	        mov al,bh
   ;	
	?debug	L 697
	mov	 al,bh
   ;	
   ;	        jz read81
   ;	
	?debug	L 698
	je	short @2@14394
   ;	
   ;	        or bl, 1
   ;	
	?debug	L 699
	or	 bl, 1
@2@14394:
   ;	
   ;	    }
   ;	  read81:
   ;	    asm{
   ;	
   ;	        and al, OV7725_SCL_L
   ;	
	?debug	L 704
	and	 al, 0fbH
   ;	
   ;	        out dx, al
   ;	
	?debug	L 705
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 707
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 708
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 709
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 710
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 711
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 712
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 713
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 714
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 715
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 716
	nop	
   ;	
   ;	      #endif
   ;	        nop
   ;	
	?debug	L 718
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 719
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 720
	nop	
   ;	
   ;	        loop read8
   ;	
	?debug	L 721
	loop	short @2@13666
   ;	
   ;	
   ;	    }
   ;	    /* NoAck(); */
   ;	    asm{
   ;	          mov dx, OV7725_PORT_OEN
   ;	
	?debug	L 726
	mov	 dx, 02d1H
   ;	
   ;	      //#if 0       //20120406 xu
   ;	        in al, dx
   ;	
	?debug	L 728
	in	 al, dx
   ;	
   ;	        and al, OV7725_SDA_L //7fh //0f7h
   ;	
	?debug	L 729
	and	 al, 0f7H 
   ;	
   ;	      //#else
   ;	      //  mov al, 0
   ;	      //#endif
   ;	    	  out dx, al
   ;	
	?debug	L 733
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 735
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 736
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 737
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 738
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 739
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 740
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 741
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 742
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 743
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 744
	nop	
   ;	
   ;	      #endif
   ;	    	  mov al, bh
   ;	
	?debug	L 746
	mov	 al, bh
   ;	
   ;	    	  dec dx
   ;	
	?debug	L 747
	dec	 dx
   ;	
   ;	        or  al, OV7725_SDA_H
   ;	
	?debug	L 748
	or	  al, 008H
   ;	
   ;	        out dx, al
   ;	
	?debug	L 749
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 751
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 752
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 753
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 754
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 755
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 756
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 757
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 758
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 759
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 760
	nop	
   ;	
   ;	      #endif
   ;	    	  nop
   ;	
	?debug	L 762
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 763
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 764
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 765
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 766
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 767
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 768
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 769
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 770
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 771
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 772
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 773
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 774
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 775
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 776
	nop	
   ;	
   ;	        or  al, OV7725_SCL_H
   ;	
	?debug	L 777
	or	  al, 004H
   ;	
   ;	        out dx, al
   ;	
	?debug	L 778
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 780
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 781
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 782
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 783
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 784
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 785
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 786
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 787
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 788
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 789
	nop	
   ;	
   ;	      #endif
   ;	        nop
   ;	
	?debug	L 791
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 792
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 793
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 794
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 795
	nop	
   ;	
   ;	         and al, OV7725_SCL_L
   ;	
	?debug	L 796
	and	 al, 0fbH
   ;	
   ;	        out dx, al
   ;	
	?debug	L 797
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 799
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 800
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 801
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 802
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 803
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 804
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 805
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 806
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 807
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 808
	nop	
   ;	
   ;	      #endif
   ;	    	  nop
   ;	
	?debug	L 810
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 811
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 812
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 813
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 814
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 815
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 816
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 817
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 818
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 819
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 820
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 821
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 822
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 823
	nop	
   ;	
   ;	    	  nop
   ;	
	?debug	L 824
	nop	
   ;	
   ;	    }
   ;	    /* Stop(); */
   ;	    asm{
   ;	        and al, OV7725_SCL_L
   ;	
	?debug	L 828
	and	 al, 0fbH
   ;	
   ;	        out dx, al
   ;	
	?debug	L 829
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 831
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 832
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 833
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 834
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 835
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 836
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 837
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 838
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 839
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 840
	nop	
   ;	
   ;	      #endif
   ;	        and al, OV7725_SDA_L
   ;	
	?debug	L 842
	and	 al, 0f7H
   ;	
   ;	        out dx, al
   ;	
	?debug	L 843
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 845
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 846
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 847
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 848
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 849
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 850
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 851
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 852
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 853
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 854
	nop	
   ;	
   ;	      #endif
   ;	        or  al, OV7725_SCL_H
   ;	
	?debug	L 856
	or	  al, 004H
   ;	
   ;	        out dx, al
   ;	
	?debug	L 857
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 859
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 860
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 861
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 862
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 863
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 864
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 865
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 866
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 867
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 868
	nop	
   ;	
   ;	      #endif
   ;	        or  al, OV7725_SDA_H
   ;	
	?debug	L 870
	or	  al, 008H
   ;	
   ;	        out dx, al
   ;	
	?debug	L 871
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 873
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 874
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 875
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 876
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 877
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 878
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 879
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 880
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 881
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 882
	nop	
   ;	
   ;	      #endif
   ;	    }
   ;	    //Local.GpioWatchDogStarted = 1;  //在GPIO检测线程中启动WatchDog
   ;	    return _BL;
   ;	
	?debug	L 886
	mov	al,bl
	jmp	short @2@18650
@2@18650:
   ;	
   ;	}
   ;	
	?debug	L 887
	pop	bp
	ret	
	?debug	C E60A526F6D41646472657373080A060000
	?debug	E
	?debug	E
_Read7725	endp
   ;	
   ;	void Write7725(INT8U RomAddress,INT8U Wdata)
   ;	
	?debug	L 890
	assume	cs:OV7725_TEXT
_Write7725	proc	far
	?debug	B
	push	bp
	mov	bp,sp
	?debug	C E6055764617461080A0800000A526F6D41646472+
	?debug	C 657373080A060000
	?debug	B
   ;	
   ;	{
   ;	    //Local.GpioWatchDogStarted = 0;  //在GPIO检测线程中启动WatchDog
   ;	    /* Start() */
   ;	
   ;	    asm{
   ;	        mov dx, OV7725_PORT
   ;	
	?debug	L 896
	mov	 dx, 02d0H
   ;	
   ;	        in al, dx
   ;	
	?debug	L 897
	in	 al, dx
   ;	
   ;	        or al, OV7725_SDA_H
   ;	
	?debug	L 898
	or	 al, 008H
   ;	
   ;	        out dx, al
   ;	
	?debug	L 899
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 901
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 902
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 903
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 904
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 905
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 906
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 907
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 908
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 909
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 910
	nop	
   ;	
   ;	      #endif
   ;	        or al, OV7725_SCL_H
   ;	
	?debug	L 912
	or	 al, 004H
   ;	
   ;	        out dx, al
   ;	
	?debug	L 913
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 915
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 916
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 917
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 918
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 919
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 920
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 921
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 922
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 923
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 924
	nop	
   ;	
   ;	      #endif
   ;	        and al, OV7725_SDA_L
   ;	
	?debug	L 926
	and	 al, 0f7H
   ;	
   ;	        out dx, al
   ;	
	?debug	L 927
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 929
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 930
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 931
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 932
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 933
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 934
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 935
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 936
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 937
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 938
	nop	
   ;	
   ;	      #endif
   ;	        and al, OV7725_SCL_L
   ;	
	?debug	L 940
	and	 al, 0fbH
   ;	
   ;	        out dx, al
   ;	
	?debug	L 941
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 943
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 944
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 945
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 946
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 947
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 948
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 949
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 950
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 951
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 952
	nop	
   ;	
   ;	      #endif
   ;	    }
   ;	    /* Write8Bit(WriteDeviceAddress); */
   ;	    asm {
   ;	        mov bl, WriteDeviceAddress
   ;	
	?debug	L 957
	mov	 bl, DGROUP:_WriteDeviceAddress
   ;	
   ;	        mov cx, 8
   ;	
	?debug	L 958
	mov	 cx, 8
@3@1486:
   ;	
   ;	    }
   ;	  write8_10:
   ;	    asm{
   ;	        and al, OV7725_SDA_L
   ;	
	?debug	L 962
	and	 al, 0f7H
   ;	
   ;	        test bl, 0x80
   ;	
	?debug	L 963
	test	 bl, 080H
   ;	
   ;	        jz  pdsda001
   ;	
	?debug	L 964
	je	short @3@1598
   ;	
   ;	        or al, OV7725_SDA_H
   ;	
	?debug	L 965
	or	 al, 008H
@3@1598:
   ;	
   ;	    }
   ;	  pdsda001:
   ;	    asm{
   ;	    	  out dx, al
   ;	
	?debug	L 969
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 971
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 972
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 973
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 974
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 975
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 976
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 977
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 978
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 979
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 980
	nop	
   ;	
   ;	      #endif
   ;	        or al, OV7725_SCL_H
   ;	
	?debug	L 982
	or	 al, 004H
   ;	
   ;	        out dx, al
   ;	
	?debug	L 983
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 985
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 986
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 987
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 988
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 989
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 990
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 991
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 992
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 993
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 994
	nop	
   ;	
   ;	      #endif
   ;	        and al, OV7725_SCL_L
   ;	
	?debug	L 996
	and	 al, 0fbH
   ;	
   ;	        out dx, al
   ;	
	?debug	L 997
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 999
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1000
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1001
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1002
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1003
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1004
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1005
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1006
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1007
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1008
	nop	
   ;	
   ;	      #endif
   ;	        shl bl, 1
   ;	
	?debug	L 1010
	shl	 bl, 1
   ;	
   ;	        loop write8_10
   ;	
	?debug	L 1011
	loop	short @3@1486
   ;	
   ;	      //  or al, OV7725_SDA_H
   ;	      // out dx, al
   ;	
   ;	
   ;	    }
   ;	    /* TestAck(); */
   ;	    asm{
   ;	        and  al, OV7725_SDA_L
   ;	
	?debug	L 1019
	and	  al, 0f7H
   ;	
   ;	        out dx, al
   ;	
	?debug	L 1020
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 1022
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1023
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1024
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1025
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1026
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1027
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1028
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1029
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1030
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1031
	nop	
   ;	
   ;	      #endif
   ;	        or al, OV7725_SCL_H
   ;	
	?debug	L 1033
	or	 al, 004H
   ;	
   ;	        out dx, al
   ;	
	?debug	L 1034
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 1036
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1037
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1038
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1039
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1040
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1041
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1042
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1043
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1044
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1045
	nop	
   ;	
   ;	      #endif
   ;	        and  al, OV7725_SCL_L
   ;	
	?debug	L 1047
	and	  al, 0fbH
   ;	
   ;	        out dx, al
   ;	
	?debug	L 1048
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 1050
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1051
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1052
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1053
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1054
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1055
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1056
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1057
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1058
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1059
	nop	
   ;	
   ;	      #endif
   ;	      //  nop
   ;	    }
   ;	
   ;	    /* Write8Bit(RomAddress); */
   ;	    asm {
   ;	        mov bl, RomAddress
   ;	
	?debug	L 1066
	mov	 bl, [bp+6]
   ;	
   ;	        mov cx, 8
   ;	
	?debug	L 1067
	mov	 cx, 8
@3@3698:
   ;	
   ;	    }
   ;	  write8_31:
   ;	    asm{
   ;	        and al, OV7725_SDA_L
   ;	
	?debug	L 1071
	and	 al, 0f7H
   ;	
   ;	        test bl, 0x80
   ;	
	?debug	L 1072
	test	 bl, 080H
   ;	
   ;	        jz  pdsda014
   ;	
	?debug	L 1073
	je	short @3@3810
   ;	
   ;	        or al, OV7725_SDA_H
   ;	
	?debug	L 1074
	or	 al, 008H
@3@3810:
   ;	
   ;	    }
   ;	  pdsda014:
   ;	    asm{
   ;	        out dx, al
   ;	
	?debug	L 1078
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 1080
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1081
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1082
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1083
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1084
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1085
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1086
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1087
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1088
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1089
	nop	
   ;	
   ;	      #endif
   ;	        or al, OV7725_SCL_H
   ;	
	?debug	L 1091
	or	 al, 004H
   ;	
   ;	        out dx, al
   ;	
	?debug	L 1092
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 1094
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1095
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1096
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1097
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1098
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1099
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1100
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1101
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1102
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1103
	nop	
   ;	
   ;	      #endif
   ;	        and al, OV7725_SCL_L
   ;	
	?debug	L 1105
	and	 al, 0fbH
   ;	
   ;	        out dx, al
   ;	
	?debug	L 1106
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 1108
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1109
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1110
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1111
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1112
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1113
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1114
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1115
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1116
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1117
	nop	
   ;	
   ;	      #endif
   ;	        shl bl, 1
   ;	
	?debug	L 1119
	shl	 bl, 1
   ;	
   ;	        loop write8_31
   ;	
	?debug	L 1120
	loop	short @3@3698
   ;	
   ;	     //   or al, OV7725_SDA_H
   ;	     //   out dx, al
   ;	
   ;	
   ;	    }
   ;	    /* TestAck(); */
   ;	    asm{
   ;	        and  al, OV7725_SDA_L
   ;	
	?debug	L 1128
	and	  al, 0f7H
   ;	
   ;	        out dx, al
   ;	
	?debug	L 1129
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 1131
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1132
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1133
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1134
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1135
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1136
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1137
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1138
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1139
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1140
	nop	
   ;	
   ;	      #endif
   ;	        or al, OV7725_SCL_H
   ;	
	?debug	L 1142
	or	 al, 004H
   ;	
   ;	        out dx, al
   ;	
	?debug	L 1143
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 1145
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1146
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1147
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1148
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1149
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1150
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1151
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1152
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1153
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1154
	nop	
   ;	
   ;	      #endif
   ;	        and  al, OV7725_SCL_L
   ;	
	?debug	L 1156
	and	  al, 0fbH
   ;	
   ;	        out dx, al
   ;	
	?debug	L 1157
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 1159
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1160
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1161
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1162
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1163
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1164
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1165
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1166
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1167
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1168
	nop	
   ;	
   ;	      #endif
   ;	        nop
   ;	
	?debug	L 1170
	nop	
   ;	
   ;	    }
   ;	
   ;	    asm {
   ;	        mov bl, Wdata
   ;	
	?debug	L 1174
	mov	 bl, [bp+8]
   ;	
   ;	        mov cx, 8
   ;	
	?debug	L 1175
	mov	 cx, 8
@3@5938:
   ;	
   ;	    }
   ;	  write83:
   ;	    asm{
   ;	        and al, OV7725_SDA_L
   ;	
	?debug	L 1179
	and	 al, 0f7H
   ;	
   ;	        test bl, 0x80
   ;	
	?debug	L 1180
	test	 bl, 080H
   ;	
   ;	        jz  pdsda20
   ;	
	?debug	L 1181
	je	short @3@6050
   ;	
   ;	        or al, OV7725_SDA_H
   ;	
	?debug	L 1182
	or	 al, 008H
@3@6050:
   ;	
   ;	    }
   ;	  pdsda20:
   ;	    asm{
   ;	        out dx, al
   ;	
	?debug	L 1186
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 1188
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1189
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1190
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1191
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1192
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1193
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1194
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1195
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1196
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1197
	nop	
   ;	
   ;	      #endif
   ;	        or al, OV7725_SCL_H
   ;	
	?debug	L 1199
	or	 al, 004H
   ;	
   ;	        out dx, al
   ;	
	?debug	L 1200
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 1202
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1203
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1204
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1205
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1206
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1207
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1208
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1209
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1210
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1211
	nop	
   ;	
   ;	      #endif
   ;	        and al, OV7725_SCL_L
   ;	
	?debug	L 1213
	and	 al, 0fbH
   ;	
   ;	        out dx, al
   ;	
	?debug	L 1214
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 1216
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1217
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1218
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1219
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1220
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1221
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1222
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1223
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1224
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1225
	nop	
   ;	
   ;	      #endif
   ;	        shl bl, 1
   ;	
	?debug	L 1227
	shl	 bl, 1
   ;	
   ;	        loop write83
   ;	
	?debug	L 1228
	loop	short @3@5938
   ;	
   ;	       // or al, OV7725_SDA_H
   ;	       // out dx, al
   ;	
   ;	    }
   ;	    /* TestAck(); */
   ;	    asm{
   ;	        and  al, OV7725_SDA_L
   ;	
	?debug	L 1235
	and	  al, 0f7H
   ;	
   ;	        out dx, al
   ;	
	?debug	L 1236
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 1238
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1239
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1240
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1241
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1242
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1243
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1244
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1245
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1246
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1247
	nop	
   ;	
   ;	      #endif
   ;	        or al, OV7725_SCL_H
   ;	
	?debug	L 1249
	or	 al, 004H
   ;	
   ;	        out dx, al
   ;	
	?debug	L 1250
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 1252
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1253
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1254
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1255
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1256
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1257
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1258
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1259
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1260
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1261
	nop	
   ;	
   ;	      #endif
   ;	        and  al, OV7725_SCL_L
   ;	
	?debug	L 1263
	and	  al, 0fbH
   ;	
   ;	        out dx, al
   ;	
	?debug	L 1264
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 1266
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1267
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1268
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1269
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1270
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1271
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1272
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1273
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1274
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1275
	nop	
   ;	
   ;	      #endif
   ;	        nop
   ;	
	?debug	L 1277
	nop	
   ;	
   ;	    }
   ;	
   ;	    /* Stop(); */
   ;	    asm{
   ;	        and al, OV7725_SCL_L
   ;	
	?debug	L 1282
	and	 al, 0fbH
   ;	
   ;	        out dx, al
   ;	
	?debug	L 1283
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 1285
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1286
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1287
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1288
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1289
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1290
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1291
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1292
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1293
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1294
	nop	
   ;	
   ;	      #endif
   ;	        and al, OV7725_SDA_L
   ;	
	?debug	L 1296
	and	 al, 0f7H
   ;	
   ;	        out dx, al
   ;	
	?debug	L 1297
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 1299
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1300
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1301
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1302
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1303
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1304
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1305
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1306
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1307
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1308
	nop	
   ;	
   ;	      #endif
   ;	        or  al, OV7725_SCL_H
   ;	
	?debug	L 1310
	or	  al, 004H
   ;	
   ;	        out dx, al
   ;	
	?debug	L 1311
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 1313
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1314
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1315
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1316
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1317
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1318
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1319
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1320
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1321
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1322
	nop	
   ;	
   ;	      #endif
   ;	        or  al, OV7725_SDA_H
   ;	
	?debug	L 1324
	or	  al, 008H
   ;	
   ;	        out dx, al
   ;	
	?debug	L 1325
	out	 dx, al
   ;	
   ;	      #ifdef _MULTI_NOP
   ;	        nop
   ;	
	?debug	L 1327
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1328
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1329
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1330
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1331
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1332
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1333
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1334
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1335
	nop	
   ;	
   ;	        nop
   ;	
	?debug	L 1336
	nop	
   ;	
   ;	      #endif
   ;	    }
   ;	   //Local.GpioWatchDogStarted = 1;  //在GPIO检测线程中启动WatchDog 
   ;	}
   ;	
	?debug	L 1340
	pop	bp
	ret	
	?debug	C E60A526F6D41646472657373080A060000055764+
	?debug	C 617461080A080000
	?debug	E
	?debug	E
_Write7725	endp
   ;	
   ;	void init7725(INT8U FrameRate)
   ;	
	?debug	L 1343
	assume	cs:OV7725_TEXT
_init7725	proc	far
	?debug	B
	push	bp
	mov	bp,sp
	sub	sp,2
	?debug	C E6094672616D6552617465080A060000
	?debug	B
   ;	
   ;	{
   ;	  INT8U val;
   ;	#if 0
   ;	  INT8U val;
   ;	    WriteDeviceAddress=0x42;
   ;	    ReadDeviceAddress=0x43;
   ;	
   ;	    Write7725(0x12,0x80);
   ;	//    xtm_printf("0x12 = 0x%X\n", Read7725(0x12));
   ;	    DelayMs(20);
   ;	   #ifdef _MULTI_WRITE_7725
   ;	    while(Read7725(0x11) != 0x01)
   ;	   #endif
   ;	      Write7725(0x11,0x01);
   ;	   #ifdef _MULTI_WRITE_7725
   ;	    while(Read7725(0x12) != 0x20)
   ;	   #endif
   ;	      Write7725(0x12,0x20);
   ;	   #ifdef _MULTI_WRITE_7725
   ;	    while(Read7725(0x0c) != 0x00)
   ;	   #endif
   ;	      Write7725(0x0c,0x00);
   ;	   #ifdef _MULTI_WRITE_7725
   ;	    while(Read7725(0x69) != 0x5c)
   ;	   #endif
   ;	      Write7725(0x69,0x5c);
   ;	   #ifdef _MULTI_WRITE_7725
   ;	    while(Read7725(0x13) != 0x8b)
   ;	   #endif
   ;	      Write7725(0x13,0x8b);
   ;	 //    Write7725(0x13,0xab);
   ;	   #ifdef _MULTI_WRITE_7725
   ;	    while(Read7725(0xa6) != 0x06)
   ;	   #endif
   ;	      Write7725(0xa6,0x06);   //d3
   ;	   #ifdef _MULTI_WRITE_7725
   ;	    while(Read7725(0x9b) != 0x18)
   ;	   #endif
   ;	      Write7725(0x9b,0x18);   //brightness +3
   ;	   #ifdef _MULTI_WRITE_7725
   ;	    while(Read7725(0xab) != 0x06)
   ;	   #endif
   ;	      Write7725(0xab,0x06);   //
   ;	   #ifdef _MULTI_WRITE_7725
   ;	    while(Read7725(0x9c) != 0x40)
   ;	   #endif
   ;	      Write7725(0x9c,0x40);   //Contrast
   ;	   #ifdef _MULTI_WRITE_7725
   ;	    while(Read7725(0x60) != 0x80)
   ;	   #endif
   ;	      Write7725(0x60,0x80);
   ;	   #ifdef _MULTI_WRITE_7725
   ;	    while(Read7725(0x61) != 0x80)
   ;	   #endif
   ;	      Write7725(0x61,0x80);
   ;	   #ifdef _MULTI_WRITE_7725
   ;	    while(Read7725(0xa7) != 0x60)
   ;	   #endif
   ;	      Write7725(0xa7,0x60);   //  U
   ;	   #ifdef _MULTI_WRITE_7725
   ;	    while(Read7725(0xa8) != 0x60)
   ;	   #endif
   ;	      Write7725(0xa8,0x60);   //  V
   ;	   #ifdef _MULTI_WRITE_7725
   ;	    while(Read7725(0xa9) != 0x80)
   ;	   #endif
   ;	      Write7725(0xa9,0x80);   // coshue
   ;	   #ifdef _MULTI_WRITE_7725
   ;	    while(Read7725(0xaa) != 0x80)
   ;	   #endif
   ;	      Write7725(0xaa,0x80);   // sinhue
   ;	
   ;	
   ;	#else
   ;	INT8U i;
   ;	    Write7725(0x12,0x80);
   ;	
	?debug	L 1419
	mov	al,128
	push	ax
	mov	al,18
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	//    xtm_printf("0x12 = 0x%X\n", Read7725(0x12));
   ;	    DelayMs(20);
   ;	
	?debug	L 1421
	mov	ax,20
	push	ax
	call	far ptr _DelayMs
	pop	cx
   ;	
   ;	Write7725(0x00, 0x02);
   ;	
	?debug	L 1422
	mov	al,2
	push	ax
	mov	al,0
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x01, 0x7e);
   ;	
	?debug	L 1423
	mov	al,126
	push	ax
	mov	al,1
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x02, 0x65);
   ;	
	?debug	L 1424
	mov	al,101
	push	ax
	mov	al,2
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x03, 0x40);
   ;	
	?debug	L 1425
	mov	al,64
	push	ax
	mov	al,3
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x04, 0x00);
   ;	
	?debug	L 1426
	mov	al,0
	push	ax
	mov	al,4
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x05, 0x79);
   ;	
	?debug	L 1427
	mov	al,121
	push	ax
	mov	al,5
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x06, 0x78);
   ;	
	?debug	L 1428
	mov	al,120
	push	ax
	mov	al,6
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x07, 0x78);
   ;	
	?debug	L 1429
	mov	al,120
	push	ax
	mov	al,7
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x08, 0x01);
   ;	
	?debug	L 1430
	mov	al,1
	push	ax
	mov	al,8
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x09, 0x00);
   ;	
	?debug	L 1431
	mov	al,0
	push	ax
	mov	al,9
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x0a, 0x77);
   ;	
	?debug	L 1432
	mov	al,119
	push	ax
	mov	al,10
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x0b, 0x21);
   ;	
	?debug	L 1433
	mov	al,33
	push	ax
	mov	al,11
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x0c, 0xc0);
   ;	
	?debug	L 1434
	mov	al,192
	push	ax
	mov	al,12
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	
   ;	switch(FrameRate)
   ;	
	?debug	L 1436
	mov	al,byte ptr [bp+6]
	mov	ah,0
	or	ax,ax
	je	short @4@198
	cmp	ax,1
	je	short @4@198
	cmp	ax,2
	je	short @4@254
	jmp	short @4@282
@4@198:
   ;	
   ;	 {
   ;	  case 0:
   ;	  case 1:
   ;	         Write7725(0x0d, 0x41);       //xu 20120418
   ;	
	?debug	L 1440
	mov	al,65
@4@226:
	push	ax
	mov	al,13
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	         break;
   ;	
	?debug	L 1441
	jmp	short @4@282
@4@254:
   ;	
   ;	  case 2:
   ;	         Write7725(0x0d, 0x81);     ///////////////////////////////
   ;	
	?debug	L 1443
	mov	al,129
	jmp	short @4@226
@4@282:
   ;	
   ;	         break;
   ;	 }
   ;	Write7725(0x0e, 0x79);
   ;	
	?debug	L 1446
	mov	al,121
	push	ax
	mov	al,14
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x0f, 0xa5);
   ;	
	?debug	L 1447
	mov	al,165
	push	ax
	mov	al,15
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x10, 0x7d);
   ;	
	?debug	L 1448
	mov	al,125
	push	ax
	mov	al,16
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	
   ;	switch(FrameRate)
   ;	
	?debug	L 1450
	mov	al,byte ptr [bp+6]
	mov	ah,0
	or	ax,ax
	je	short @4@450
	cmp	ax,1
	je	short @4@450
	cmp	ax,2
	je	short @4@506
	jmp	short @4@534
@4@450:
   ;	
   ;	 {
   ;	  case 0:
   ;	  case 1:
   ;	         Write7725(0x11, 0x01);     //xu 20120418
   ;	
	?debug	L 1454
	mov	al,1
@4@478:
	push	ax
	mov	al,17
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	         break;
   ;	
	?debug	L 1455
	jmp	short @4@534
@4@506:
   ;	
   ;	  case 2:
   ;	         Write7725(0x11, 0x03);   ///////////////////////////////
   ;	
	?debug	L 1457
	mov	al,3
	jmp	short @4@478
@4@534:
   ;	
   ;	         break;
   ;	 }
   ;	
   ;	Write7725(0x12, 0x20);
   ;	
	?debug	L 1461
	mov	al,32
	push	ax
	mov	al,18
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x13, 0xff);
   ;	
	?debug	L 1462
	mov	al,255
	push	ax
	mov	al,19
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x14, 0x10);
   ;	
	?debug	L 1463
	mov	al,16
	push	ax
	mov	al,20
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x15, 0x01);
   ;	
	?debug	L 1464
	mov	al,1
	push	ax
	mov	al,21
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x16, 0x00);
   ;	
	?debug	L 1465
	mov	al,0
	push	ax
	mov	al,22
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x17, 0x23);//0x26);   555  20121231
   ;	
	?debug	L 1466
	mov	al,35
	push	ax
	mov	al,23
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x18, 0xa0);
   ;	
	?debug	L 1467
	mov	al,160
	push	ax
	mov	al,24
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x19, 0x07);
   ;	
	?debug	L 1468
	mov	al,7
	push	ax
	mov	al,25
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x1a, 0xf0);
   ;	
	?debug	L 1469
	mov	al,240
	push	ax
	mov	al,26
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x1b, 0x40);
   ;	
	?debug	L 1470
	mov	al,64
	push	ax
	mov	al,27
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x1c, 0x7f);
   ;	
	?debug	L 1471
	mov	al,127
	push	ax
	mov	al,28
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x1d, 0xa2);
   ;	
	?debug	L 1472
	mov	al,162
	push	ax
	mov	al,29
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x1e, 0x00);
   ;	
	?debug	L 1473
	mov	al,0
	push	ax
	mov	al,30
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x1f, 0x00);
   ;	
	?debug	L 1474
	mov	al,0
	push	ax
	mov	al,31
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x20, 0x10);
   ;	
	?debug	L 1475
	mov	al,16
	push	ax
	mov	al,32
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x21, 0x00);
   ;	
	?debug	L 1476
	mov	al,0
	push	ax
	mov	al,33
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	//Write7725(0x22, 0x7f);
   ;	//Write7725(0x23, 0x07);
   ;	switch(FrameRate)      // 0x22    0xaa  -- 30   0x99  --  25      0x73  -- 20
   ;	
	?debug	L 1479
	mov	al,byte ptr [bp+6]
	mov	ah,0
	or	ax,ax
	je	short @4@702
	cmp	ax,1
	je	short @4@758
	cmp	ax,2
	je	short @4@786
	jmp	short @4@814
@4@702:
   ;	
   ;	 {                                      // 0x23    0x02  -- 30   0x03  --- 25      0x04  -- 20
   ;	  case 0:
   ;	         Write7725(0x22, 0xaa);  //xu 20120418
   ;	
	?debug	L 1482
	mov	al,170
	push	ax
	mov	al,34
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	         Write7725(0x23, 0x02);  //xu 20120418
   ;	
	?debug	L 1483
	mov	al,2
@4@730:
	push	ax
	mov	al,35
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	         break;
   ;	
	?debug	L 1484
	jmp	short @4@814
@4@758:
   ;	
   ;	  case 1:
   ;	         Write7725(0x22, 0x99);  //xu 20120418
   ;	
	?debug	L 1486
	mov	al,153
	push	ax
	mov	al,34
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	         Write7725(0x23, 0x03);  //xu 20120418
   ;	
	?debug	L 1487
	mov	al,3
	jmp	short @4@730
@4@786:
   ;	
   ;	         break;
   ;	  case 2:
   ;	         Write7725(0x22, 0x73);  //xu 20120418
   ;	
	?debug	L 1490
	mov	al,115
	push	ax
	mov	al,34
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	         Write7725(0x23, 0x04);  //xu 20120418
   ;	
	?debug	L 1491
	mov	al,4
	jmp	short @4@730
@4@814:
   ;	
   ;	         break;
   ;	 }
   ;	
   ;	Write7725(0x24, 0x40);
   ;	
	?debug	L 1495
	mov	al,64
	push	ax
	mov	al,36
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x25, 0x30);
   ;	
	?debug	L 1496
	mov	al,48
	push	ax
	mov	al,37
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x26, 0xa1);
   ;	
	?debug	L 1497
	mov	al,161
	push	ax
	mov	al,38
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x27, 0x00);
   ;	
	?debug	L 1498
	mov	al,0
	push	ax
	mov	al,39
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x28, 0x00);
   ;	
	?debug	L 1499
	mov	al,0
	push	ax
	mov	al,40
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x29, 0xa0);
   ;	
	?debug	L 1500
	mov	al,160
	push	ax
	mov	al,41
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x2a, 0x02);
   ;	
	?debug	L 1501
	mov	al,2
	push	ax
	mov	al,42
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x2b, 0x00);
   ;	
	?debug	L 1502
	mov	al,0
	push	ax
	mov	al,43
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x2c, 0xf0);
   ;	
	?debug	L 1503
	mov	al,240
	push	ax
	mov	al,44
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x2d, 0x00);
   ;	
	?debug	L 1504
	mov	al,0
	push	ax
	mov	al,45
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x2e, 0x00);
   ;	
	?debug	L 1505
	mov	al,0
	push	ax
	mov	al,46
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x2f, 0x37);
   ;	
	?debug	L 1506
	mov	al,55
	push	ax
	mov	al,47
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x30, 0x80);
   ;	
	?debug	L 1507
	mov	al,128
	push	ax
	mov	al,48
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x31, 0x60);
   ;	
	?debug	L 1508
	mov	al,96
	push	ax
	mov	al,49
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x32, 0x00);//0x02);  555  20121231
   ;	
	?debug	L 1509
	mov	al,0
	push	ax
	mov	al,50
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	//Write7725(0x33, 0x00);
   ;	switch(FrameRate)      // 0x00 -- 30    0x66  -- 25    0x40  -- 20
   ;	
	?debug	L 1511
	mov	al,byte ptr [bp+6]
	mov	ah,0
	or	ax,ax
	je	short @4@982
	cmp	ax,1
	je	short @4@1038
	cmp	ax,2
	je	short @4@1066
	jmp	short @4@1094
@4@982:
   ;	
   ;	 {
   ;	  case 0:
   ;	         Write7725(0x33, 0x00);  //xu 20120418       0x66  -- 25    0x00  30
   ;	
	?debug	L 1514
	mov	al,0
@4@1010:
	push	ax
	mov	al,51
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	         break;
   ;	
	?debug	L 1515
	jmp	short @4@1094
@4@1038:
   ;	
   ;	  case 1:
   ;	         Write7725(0x33, 0x66);  //xu 20120418       0x66  -- 25    0x00  30
   ;	
	?debug	L 1517
	mov	al,102
	jmp	short @4@1010
@4@1066:
   ;	
   ;	         break;
   ;	  case 2:
   ;	         Write7725(0x33, 0x40);                             ///////////////////////////////
   ;	
	?debug	L 1520
	mov	al,64
	jmp	short @4@1010
@4@1094:
   ;	
   ;	         break;
   ;	 }
   ;	
   ;	Write7725(0x34, 0x00);
   ;	
	?debug	L 1524
	mov	al,0
	push	ax
	mov	al,52
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x35, 0x7c);
   ;	
	?debug	L 1525
	mov	al,124
	push	ax
	mov	al,53
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x36, 0x7e);
   ;	
	?debug	L 1526
	mov	al,126
	push	ax
	mov	al,54
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x37, 0x7f);
   ;	
	?debug	L 1527
	mov	al,127
	push	ax
	mov	al,55
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x38, 0x7f);
   ;	
	?debug	L 1528
	mov	al,127
	push	ax
	mov	al,56
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x39, 0x94);
   ;	
	?debug	L 1529
	mov	al,148
	push	ax
	mov	al,57
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x3a, 0xa4);
   ;	
	?debug	L 1530
	mov	al,164
	push	ax
	mov	al,58
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x3b, 0xa8);
   ;	
	?debug	L 1531
	mov	al,168
	push	ax
	mov	al,59
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x3c, 0xaa);
   ;	
	?debug	L 1532
	mov	al,170
	push	ax
	mov	al,60
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x3d, 0x03);
   ;	
	?debug	L 1533
	mov	al,3
	push	ax
	mov	al,61
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x3e, 0xe2);
   ;	
	?debug	L 1534
	mov	al,226
	push	ax
	mov	al,62
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x3f, 0x1f);
   ;	
	?debug	L 1535
	mov	al,31
	push	ax
	mov	al,63
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x40, 0xe8);
   ;	
	?debug	L 1536
	mov	al,232
	push	ax
	mov	al,64
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x41, 0x00);
   ;	
	?debug	L 1537
	mov	al,0
	push	ax
	mov	al,65
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x42, 0x7f);
   ;	
	?debug	L 1538
	mov	al,127
	push	ax
	mov	al,66
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x42, 0x80);
   ;	
	?debug	L 1539
	mov	al,128
	push	ax
	mov	al,66
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x44, 0x80);
   ;	
	?debug	L 1540
	mov	al,128
	push	ax
	mov	al,68
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x45, 0x80);
   ;	
	?debug	L 1541
	mov	al,128
	push	ax
	mov	al,69
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x46, 0x00);
   ;	
	?debug	L 1542
	mov	al,0
	push	ax
	mov	al,70
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x47, 0x00);
   ;	
	?debug	L 1543
	mov	al,0
	push	ax
	mov	al,71
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x48, 0x00);
   ;	
	?debug	L 1544
	mov	al,0
	push	ax
	mov	al,72
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x49, 0x50);
   ;	
	?debug	L 1545
	mov	al,80
	push	ax
	mov	al,73
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x4a, 0x30);
   ;	
	?debug	L 1546
	mov	al,48
	push	ax
	mov	al,74
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x4b, 0x50);
   ;	
	?debug	L 1547
	mov	al,80
	push	ax
	mov	al,75
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x4c, 0x50);
   ;	
	?debug	L 1548
	mov	al,80
	push	ax
	mov	al,76
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x4d, 0x00);
   ;	
	?debug	L 1549
	mov	al,0
	push	ax
	mov	al,77
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x4e, 0xef);
   ;	
	?debug	L 1550
	mov	al,239
	push	ax
	mov	al,78
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x4f, 0x10);
   ;	
	?debug	L 1551
	mov	al,16
	push	ax
	mov	al,79
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x50, 0x60);
   ;	
	?debug	L 1552
	mov	al,96
	push	ax
	mov	al,80
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x51, 0x00);
   ;	
	?debug	L 1553
	mov	al,0
	push	ax
	mov	al,81
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x52, 0x00);
   ;	
	?debug	L 1554
	mov	al,0
	push	ax
	mov	al,82
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x53, 0x24);
   ;	
	?debug	L 1555
	mov	al,36
	push	ax
	mov	al,83
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x54, 0x7a);
   ;	
	?debug	L 1556
	mov	al,122
	push	ax
	mov	al,84
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x55, 0xfc);
   ;	
	?debug	L 1557
	mov	al,252
	push	ax
	mov	al,85
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x56, 0xfc);
   ;	
	?debug	L 1558
	mov	al,252
	push	ax
	mov	al,86
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x57, 0xfc);
   ;	
	?debug	L 1559
	mov	al,252
	push	ax
	mov	al,87
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x58, 0xfc);
   ;	
	?debug	L 1560
	mov	al,252
	push	ax
	mov	al,88
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x59, 0xfc);
   ;	
	?debug	L 1561
	mov	al,252
	push	ax
	mov	al,89
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x5a, 0xfc);
   ;	
	?debug	L 1562
	mov	al,252
	push	ax
	mov	al,90
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x5b, 0xfc);
   ;	
	?debug	L 1563
	mov	al,252
	push	ax
	mov	al,91
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x5c, 0xfc);
   ;	
	?debug	L 1564
	mov	al,252
	push	ax
	mov	al,92
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x5d, 0xfc);
   ;	
	?debug	L 1565
	mov	al,252
	push	ax
	mov	al,93
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x5e, 0xfc);
   ;	
	?debug	L 1566
	mov	al,252
	push	ax
	mov	al,94
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x5f, 0xfc);
   ;	
	?debug	L 1567
	mov	al,252
	push	ax
	mov	al,95
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x60, 0x00);
   ;	
	?debug	L 1568
	mov	al,0
	push	ax
	mov	al,96
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x61, 0x05);
   ;	
	?debug	L 1569
	mov	al,5
	push	ax
	mov	al,97
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x62, 0xff);
   ;	
	?debug	L 1570
	mov	al,255
	push	ax
	mov	al,98
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x63, 0xe0);
   ;	
	?debug	L 1571
	mov	al,224
	push	ax
	mov	al,99
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x64, 0xff);
   ;	
	?debug	L 1572
	mov	al,255
	push	ax
	mov	al,100
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x65, 0x20);
   ;	
	?debug	L 1573
	mov	al,32
	push	ax
	mov	al,101
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x66, 0x00);
   ;	
	?debug	L 1574
	mov	al,0
	push	ax
	mov	al,102
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x67, 0x00);
   ;	
	?debug	L 1575
	mov	al,0
	push	ax
	mov	al,103
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x68, 0x00);
   ;	
	?debug	L 1576
	mov	al,0
	push	ax
	mov	al,104
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x69, 0x5c);
   ;	
	?debug	L 1577
	mov	al,92
	push	ax
	mov	al,105
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x6a, 0x11);
   ;	
	?debug	L 1578
	mov	al,17
	push	ax
	mov	al,106
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x6b, 0xaa);
   ;	
	?debug	L 1579
	mov	al,170
	push	ax
	mov	al,107
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x6c, 0x01);
   ;	
	?debug	L 1580
	mov	al,1
	push	ax
	mov	al,108
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x6d, 0x50);
   ;	
	?debug	L 1581
	mov	al,80
	push	ax
	mov	al,109
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x6e, 0x80);
   ;	
	?debug	L 1582
	mov	al,128
	push	ax
	mov	al,110
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x6f, 0x80);
   ;	
	?debug	L 1583
	mov	al,128
	push	ax
	mov	al,111
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x70, 0x0f);
   ;	
	?debug	L 1584
	mov	al,15
	push	ax
	mov	al,112
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x71, 0x00);
   ;	
	?debug	L 1585
	mov	al,0
	push	ax
	mov	al,113
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x72, 0x00);
   ;	
	?debug	L 1586
	mov	al,0
	push	ax
	mov	al,114
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x73, 0x0f);
   ;	
	?debug	L 1587
	mov	al,15
	push	ax
	mov	al,115
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x74, 0x0f);
   ;	
	?debug	L 1588
	mov	al,15
	push	ax
	mov	al,116
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x75, 0xff);
   ;	
	?debug	L 1589
	mov	al,255
	push	ax
	mov	al,117
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x76, 0x00);
   ;	
	?debug	L 1590
	mov	al,0
	push	ax
	mov	al,118
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x77, 0x10);
   ;	
	?debug	L 1591
	mov	al,16
	push	ax
	mov	al,119
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x78, 0x10);
   ;	
	?debug	L 1592
	mov	al,16
	push	ax
	mov	al,120
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x79, 0x70);
   ;	
	?debug	L 1593
	mov	al,112
	push	ax
	mov	al,121
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x7a, 0x70);
   ;	
	?debug	L 1594
	mov	al,112
	push	ax
	mov	al,122
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x7b, 0xf0);
   ;	
	?debug	L 1595
	mov	al,240
	push	ax
	mov	al,123
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x7c, 0xf0);
   ;	
	?debug	L 1596
	mov	al,240
	push	ax
	mov	al,124
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x7d, 0xf0);
   ;	
	?debug	L 1597
	mov	al,240
	push	ax
	mov	al,125
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x7e, 0x11);
   ;	
	?debug	L 1598
	mov	al,17
	push	ax
	mov	al,126
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x7f, 0x21);
   ;	
	?debug	L 1599
	mov	al,33
	push	ax
	mov	al,127
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x80, 0x3f);
   ;	
	?debug	L 1600
	mov	al,63
	push	ax
	mov	al,128
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x81, 0x65);
   ;	
	?debug	L 1601
	mov	al,101
	push	ax
	mov	al,129
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x82, 0x74);
   ;	
	?debug	L 1602
	mov	al,116
	push	ax
	mov	al,130
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x83, 0x81);
   ;	
	?debug	L 1603
	mov	al,129
	push	ax
	mov	al,131
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x84, 0x8f);
   ;	
	?debug	L 1604
	mov	al,143
	push	ax
	mov	al,132
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x85, 0x9b);
   ;	
	?debug	L 1605
	mov	al,155
	push	ax
	mov	al,133
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x86, 0xa5);
   ;	
	?debug	L 1606
	mov	al,165
	push	ax
	mov	al,134
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x87, 0xab);
   ;	
	?debug	L 1607
	mov	al,171
	push	ax
	mov	al,135
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x88, 0xb6);
   ;	
	?debug	L 1608
	mov	al,182
	push	ax
	mov	al,136
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x89, 0xbf);
   ;	
	?debug	L 1609
	mov	al,191
	push	ax
	mov	al,137
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x8a, 0xd0);
   ;	
	?debug	L 1610
	mov	al,208
	push	ax
	mov	al,138
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x8b, 0xda);
   ;	
	?debug	L 1611
	mov	al,218
	push	ax
	mov	al,139
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x8c, 0xec);
   ;	
	?debug	L 1612
	mov	al,236
	push	ax
	mov	al,140
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x8d, 0x1a);
   ;	
	?debug	L 1613
	mov	al,26
	push	ax
	mov	al,141
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x8e, 0x03);
   ;	
	?debug	L 1614
	mov	al,3
	push	ax
	mov	al,142
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x8f, 0x04);
   ;	
	?debug	L 1615
	mov	al,4
	push	ax
	mov	al,143
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x90, 0x05);
   ;	
	?debug	L 1616
	mov	al,5
	push	ax
	mov	al,144
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x91, 0x01);
   ;	
	?debug	L 1617
	mov	al,1
	push	ax
	mov	al,145
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x92, 0x05);
   ;	
	?debug	L 1618
	mov	al,5
	push	ax
	mov	al,146
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x93, 0x00);
   ;	
	?debug	L 1619
	mov	al,0
	push	ax
	mov	al,147
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x94, 0xd4);
   ;	
	?debug	L 1620
	mov	al,212
	push	ax
	mov	al,148
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x95, 0xed);
   ;	
	?debug	L 1621
	mov	al,237
	push	ax
	mov	al,149
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x96, 0x17);
   ;	
	?debug	L 1622
	mov	al,23
	push	ax
	mov	al,150
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x97, 0x24);
   ;	
	?debug	L 1623
	mov	al,36
	push	ax
	mov	al,151
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x98, 0x76);
   ;	
	?debug	L 1624
	mov	al,118
	push	ax
	mov	al,152
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x99, 0x98);
   ;	
	?debug	L 1625
	mov	al,152
	push	ax
	mov	al,153
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x9a, 0x1a);
   ;	
	?debug	L 1626
	mov	al,26
	push	ax
	mov	al,154
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x9b, 0x08);
   ;	
	?debug	L 1627
	mov	al,8
	push	ax
	mov	al,155
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x9c, 0x20);
   ;	
	?debug	L 1628
	mov	al,32
	push	ax
	mov	al,156
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x9d, 0x00);
   ;	
	?debug	L 1629
	mov	al,0
	push	ax
	mov	al,157
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x9e, 0x81);
   ;	
	?debug	L 1630
	mov	al,129
	push	ax
	mov	al,158
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0x9f, 0xfa);
   ;	
	?debug	L 1631
	mov	al,250
	push	ax
	mov	al,159
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	
   ;	Write7725(0xa0, 0x02);
   ;	
	?debug	L 1633
	mov	al,2
	push	ax
	mov	al,160
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0xa1, 0x50);
   ;	
	?debug	L 1634
	mov	al,80
	push	ax
	mov	al,161
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0xa2, 0x40);
   ;	
	?debug	L 1635
	mov	al,64
	push	ax
	mov	al,162
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0xa3, 0x06);
   ;	
	?debug	L 1636
	mov	al,6
	push	ax
	mov	al,163
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0xa4, 0x00);
   ;	
	?debug	L 1637
	mov	al,0
	push	ax
	mov	al,164
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0xa5, 0x00);
   ;	
	?debug	L 1638
	mov	al,0
	push	ax
	mov	al,165
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0xa6, 0x04);
   ;	
	?debug	L 1639
	mov	al,4
	push	ax
	mov	al,166
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0xa7, 0x40);
   ;	
	?debug	L 1640
	mov	al,64
	push	ax
	mov	al,167
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0xa8, 0x40);
   ;	
	?debug	L 1641
	mov	al,64
	push	ax
	mov	al,168
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0xa9, 0x80);
   ;	
	?debug	L 1642
	mov	al,128
	push	ax
	mov	al,169
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0xaa, 0x00);
   ;	
	?debug	L 1643
	mov	al,0
	push	ax
	mov	al,170
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	
   ;	Write7725(0xab, 0x06);
   ;	
	?debug	L 1645
	mov	al,6
	push	ax
	mov	al,171
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	Write7725(0xac, 0xff);
   ;	
	?debug	L 1646
	mov	al,255
	push	ax
	mov	al,172
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	for(i = 0xad; i <= 0xf3; i++)
   ;	
	?debug	L 1647
	mov	byte ptr [bp-1],173
	jmp	short @4@1178
@4@1122:
   ;	
   ;	  Write7725(i, 0x65);
   ;	
	?debug	L 1648
	mov	al,101
	push	ax
	mov	al,byte ptr [bp-1]
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
	?debug	L 1647
	inc	byte ptr [bp-1]
@4@1178:
	cmp	byte ptr [bp-1],243
	jbe	short @4@1122
   ;	
   ;	#endif
   ;	
   ;	}
   ;	
	?debug	L 1651
	mov	sp,bp
	pop	bp
	ret	
	?debug	C E601690802FFFF00094672616D6552617465080A+
	?debug	C 060000
	?debug	E
	?debug	E
_init7725	endp
OV7725_TEXT	ends
_DATA	segment word public 'DATA'
	db	0
	db	1
	db	2
	db	3
	db	4
	db	9
	db	10
	db	11
	db	12
	db	13
	db	14
	db	15
	db	16
	db	17
	db	18
	db	19
	db	2
	db	126
	db	101
	db	64
	db	0
	db	0
	db	119
	db	33
	db	192
	db	65
	db	121
	db	165
	db	125
	db	1
	db	32
	db	255
_DATA	ends
OV7725_TEXT	segment byte public 'CODE'
   ;	
   ;	INT8U test7725(void)
   ;	
	?debug	L 1653
	assume	cs:OV7725_TEXT
_test7725	proc	far
	?debug	B
	push	bp
	mov	bp,sp
	sub	sp,36
	?debug	B
   ;	
   ;	{
   ;	  INT8U i, val, ReadOK;
   ;	  INT8U reg[16] = {0x00, 0x01, 0x02, 0x03, 0x04,  0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13};
   ;	
	?debug	L 1656
	lea	ax,word ptr [bp-20]
	push	ss
	push	ax
	mov	ax,offset DGROUP:d@w+0
	push	ds
	push	ax
	mov	cx,16
	call	far ptr F_SCOPY@
   ;	
   ;	  INT8U value[16] = {0x02, 0x7e, 0x65, 0x40, 0x00, 0x00, 0x77, 0x21, 0xc0, 0x41, 0x79, 0xa5, 0x7d, 0x01, 0x20, 0xff};
   ;	
	?debug	L 1657
	lea	ax,word ptr [bp-36]
	push	ss
	push	ax
	mov	ax,offset DGROUP:d@w+16
	push	ds
	push	ax
	mov	cx,16
	call	far ptr F_SCOPY@
   ;	
   ;	  ReadOK = 1;
   ;	
	?debug	L 1658
	mov	byte ptr [bp-3],1
   ;	
   ;	  for(i=0; i<16; i++)
   ;	
	?debug	L 1659
	mov	byte ptr [bp-1],0
	jmp	@5@170
@5@58:
   ;	
   ;	   if(ReadOK)
   ;	
	?debug	L 1660
	cmp	byte ptr [bp-3],0
	jne	@@0
	jmp	@5@142
@@0:
   ;	
   ;	    {
   ;	     Write7725(reg[i], value[i]);
   ;	
	?debug	L 1662
	mov	al,byte ptr [bp-1]
	mov	ah,0
	lea	dx,word ptr [bp-36]
	add	ax,dx
	mov	bx,ax
	mov	al,byte ptr ss:[bx]
	push	ax
	mov	al,byte ptr [bp-1]
	mov	ah,0
	lea	dx,word ptr [bp-20]
	add	ax,dx
	mov	bx,ax
	mov	al,byte ptr ss:[bx]
	push	ax
	push	cs
	call	near ptr _Write7725
	pop	cx
	pop	cx
   ;	
   ;	     val = Read7725(reg[i]);
   ;	
	?debug	L 1663
	mov	al,byte ptr [bp-1]
	mov	ah,0
	lea	dx,word ptr [bp-20]
	add	ax,dx
	mov	bx,ax
	mov	al,byte ptr ss:[bx]
	push	ax
	push	cs
	call	near ptr _Read7725
	pop	cx
	mov	byte ptr [bp-2],al
   ;	
   ;	     xtm_printf("reg = 0x%X, val = 0x%X, value[i] = 0x%X\n", reg[i], val, value[i]);
   ;	
	?debug	L 1664
	mov	al,byte ptr [bp-1]
	mov	ah,0
	lea	dx,word ptr [bp-36]
	add	ax,dx
	mov	bx,ax
	mov	al,byte ptr ss:[bx]
	mov	ah,0
	push	ax
	mov	al,byte ptr [bp-2]
	mov	ah,0
	push	ax
	mov	al,byte ptr [bp-1]
	mov	ah,0
	lea	dx,word ptr [bp-20]
	add	ax,dx
	mov	bx,ax
	mov	al,byte ptr ss:[bx]
	mov	ah,0
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@
	push	ax
	call	far ptr _xtm_printf
	add	sp,10
   ;	
   ;	     //if((i == 5)||(i == 6))
   ;	    //   continue;
   ;	     if(val != value[i])
   ;	
	?debug	L 1667
	mov	al,byte ptr [bp-1]
	mov	ah,0
	lea	dx,word ptr [bp-36]
	add	ax,dx
	mov	bx,ax
	mov	al,byte ptr ss:[bx]
	cmp	al,byte ptr [bp-2]
	je	short @5@142
   ;	
   ;	       ReadOK = 0;
   ;	
	?debug	L 1668
	mov	byte ptr [bp-3],0
@5@142:
	?debug	L 1659
	inc	byte ptr [bp-1]
@5@170:
	cmp	byte ptr [bp-1],16
	jae	@@1
	jmp	@5@58
@@1:
   ;	
   ;	    }
   ;	
   ;	  xtm_printf("ReadOK = %d\n", ReadOK);
   ;	
	?debug	L 1671
	mov	al,byte ptr [bp-3]
	mov	ah,0
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+41
	push	ax
	call	far ptr _xtm_printf
	add	sp,6
   ;	
   ;	  return ReadOK;
   ;	
	?debug	L 1672
	mov	al,byte ptr [bp-3]
	jmp	short @5@226
@5@226:
   ;	
   ;	}
   ;	
	?debug	L 1673
	mov	sp,bp
	pop	bp
	ret	
	?debug	C E3180010001A08
	?debug	C E3190010001A08
	?debug	C E60576616C75651802DCFF00037265671902ECFF+
	?debug	C 0006526561644F4B0802FDFF000376616C0802FE+
	?debug	C FF0001690802FFFF00
	?debug	E
	?debug	E
_test7725	endp
OV7725_TEXT	ends
_BSS	segment word public 'BSS'
_ReadDeviceAddress	label	byte
	db	1 dup (?)
_WriteDeviceAddress	label	byte
	db	1 dup (?)
	?debug	C E9
	?debug	C FA00000000
_BSS	ends
_DATA	segment word public 'DATA'
s@	label	byte
	db	'reg = 0x%X, val = 0x%X, value[i] = 0x%X'
	db	10
	db	0
	db	'ReadOK = %d'
	db	10
	db	0
_DATA	ends
OV7725_TEXT	segment byte public 'CODE'
OV7725_TEXT	ends
	extrn	F_SCOPY@:far
	public	_test7725
	public	_init_camera
	public	_Read7725
	public	_Write7725
	public	_init7725
	public	_ReadDeviceAddress
	public	_WriteDeviceAddress
	extrn	_xtm_printf:far
	extrn	_DelayMs:far
_s@	equ	s@
	?debug	C EA010C
	?debug	C E31A00000023040500
	?debug	C EB08465F53434F5059401A00
	?debug	C E31B00000023080400
	?debug	C EC095F74657374373732351B1800
	?debug	C E31C000000230A0400
	?debug	C EC0C5F696E69745F63616D6572611C1800
	?debug	C E31D00000023080400
	?debug	C EC095F52656164373732351D1800
	?debug	C E31E00000023010400
	?debug	C EC0A5F5772697465373732351E1800
	?debug	C E31F00000023010400
	?debug	C EC095F696E6974373732351F1800
	?debug	C EC125F5265616444657669636541646472657373+
	?debug	C 080000
	?debug	C EC135F5772697465446576696365416464726573+
	?debug	C 73080000
	?debug	C E32000000023040401
	?debug	C EB0B5F78746D5F7072696E74662000
	?debug	C E32100000023010400
	?debug	C EB085F44656C61794D732100
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
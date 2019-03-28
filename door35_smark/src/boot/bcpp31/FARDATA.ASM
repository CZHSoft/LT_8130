	page	60, 132
	name	fardata
	title	Borland C++ FAR_DATA class copy/decompression initializer

;
; Paradigm LOCATE FAR_DATA support for BC++ 3.1, Version 5.11
; Copyright (C) 1995 Paradigm Systems.  All rights reserved.
;
; *********************************************************************
; Permission to modify and distribute object files based on this
; source code is granted to licensed users of Paradigm LOCATE.
;
; Under no circumstances is this source code to be re-distributed
; without the express permission of Paradigm Systems.
; *********************************************************************
;
; Function		
; ========
; This module is used to initialize the class FAR_DATA from a copy stored
; in EPROM.  You have the choice of a direct copy, or a decompressed copy
; (if the class ROMFARDATA has been compressed by Paradigm LOCATE).
;
; Make sure that this fardata module is linked immediately after
; the startup module

	INCLUDE	boot\bcpp31\startup.inc		; Macros and other assembly langauge defintions
	INCLUDE	boot\bcpp31\bcpp31.inc		; Include definitions

	subttl	Segment ordering/alignment section
	page
;
; Segments declared/referenced from this module.  Segment _TEXT is used to
; the code from this module, segment _INIT_ contains the initializer table
; entry that guarantees the initialization of the class FAR_DATA.
;
DefSeg	_TEXT,    word,  public,  CODE,          <>
DefSeg	_INIT_,   word,  public,  INITDATA,      IGROUP

;
; These are the four segments that form the FAR_DATA class.  FAR_DATA is followed
; by ENDFAR_DATA, which is needed to calculate the size of class FAR_DATA.
; ROMFARDATA holds the duplicated (and possibly compressed) contents of class
; FAR_DATA and ENDROMFARDATA is used to mark the of class ROMFARDATA and prevent
; an alias condition from occurring.
;
DefSeg	_bfd,     para,  public,  FAR_DATA,      <>	; Start FAR_DATA class
DefSeg	_efd,     para,  public,  ENDFAR_DATA,   <>	; End FAR_DATA class
DefSeg	_brfd,    para,  public,  ROMFARDATA,    <>	; Start of FAR_DATA copy
DefSeg	_erfd,    para,  public,  ENDROMFARDATA, <>	; End of FAR_DATA copy

;
; These are the four segments we defined above to support the FAR_DATA class.
; Here we open each segment and define a label that we can take the address of
; for the purposes of finding the length of class FAR_DATA, and the starting
; addresses of the classes FAR_DATA and ROMFARDATA.
;

_bfd	segment
bfdata	label	byte			; Mark the start of class FAR_DATA
_bfd	ends
_efd	segment
efdata	label	byte
	db	16 dup (?)		; Force the next segment to a new paragraph
_efd	ends

_brfd	segment
brfdata	label	byte			; Mark the start of class ROMFARDATA
_brfd	ends
_erfd	segment
	db	16 dup (?)		; Force the next segment to a new paragraph
_erfd	ends

	page
;
; Functions defined external to this module.
;
IFDEF	COMPRESSED
ExtProc	_DecompressClass		; Paradigm LOCATE class decompression helper
	SHOW	<Including Paradigm LOCATE class decompression support>
ELSE
ExtLbl	F_PSBP@, far			; BC++ RTL huge pointer helper
ENDIF	; COMPRESSED

	page
_TEXT	segment

;
; This is the code which either copies the class FAR_DATA from its position
; in EPROM, or calls the Paradigm LOCATE class decompression helper function
; which does the same work on a compressed class.
;
	assume	cs:_TEXT
CopyFD	proc	near
IFDEF	COMPRESSED
;
; Set up the addresses and perform the decompression
;
	mov	ax, _bfd		; Push the destination address
	push	ax
	mov	ax, offset bfdata
	push	ax

	mov	ax, _brfd		; Push the source address
	push	ax
	mov	ax, offset brfdata
	push	ax

	call	_DecompressClass	; Decompress the class
	add	sp, 8			; Clean up the stack and return
	ret
ELSE
	push	ds			; Save the important registers
	push	si
	push	di

	mov	ax, offset efdata	; Compute the size of the class
	mov	dx, _efd
	mov	bx, offset bfdata
	mov	cx, _bfd
	call	F_PSBP@			; DX:AX is the length in bytes

	mov	si, _brfd		; Source address of ROMFARDATA
	mov	ds, si
	mov	si, offset brfdata

	mov	di, _bfd		; Destination address of FAR_DATA
	mov	es, di
	mov	di, offset bfdata

	or	dx, dx			; DX is the # of 64KB blocks
	jz	$$2
	
$$1:
	mov	cx, 8000h		; Copy a 64KB block of FAR_DATA
	rep	movsw

	mov	cx, es			; Bump the destination segment
	add	cx, 1000h
	mov	es, cx

	mov	cx, ds			; Bump the source segment
	add	cx, 1000h
	mov	ds, cx

	dec	dx			; Decrement the block count
	jnz	$$1			; Continue till all 64KB copied

$$2:
	mov	cx, ax			; Copy the remainder of the data
	jcxz	$$3
	shr	cx, 1
	rep	movsw
	adc	cx, cx
	rep	movsb			; Copy the odd byte, if necessary
$$3:
	pop	di			; Pop the saved registers and exit
	pop	si
	pop	ds
	ret
ENDIF	; COMPRESSED

CopyFD	endp

_TEXT	ends

	page
;
; FAR_DATA copy/decompression initializer.  This entry gets placed in the
; table of startup code initializers to automatically copy or decompress
; the FAR_DATA class whenever it is linked in.
;
_INIT_	segment
	InitRec	<MM_NEAR, PRI_FARDATA, offset CopyFD, seg CopyFD >
_INIT_	ends

	end

	.xlist
;
; Assembly language definitions for Borland/Tubro C++, Version 5.11
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
; This include file helps set the memory model variables and defines
; data structures used by Borland/Tubro C++ assembly language modules.
;

IFDEF	??Version			; Check for TASM
NOWARN	RES				; Turn off reserved words warnings
ENDIF	; ??Version

;
; This macro is used to display comments during assembly
;
SHOW	macro	msg
IFDEF	??Version			; Check if TASM is doing the work
NOWARN	PDC				; Disable pass 1 warnings for TASM
ENDIF	; ??Version
IF1
	%out	msg
ENDIF	; IF1
IFDEF	??Version
WARN	PDC				; Turn warnings back on
ENDIF	; ??Version
	endm

;
; Determine the memory module begin used and define values which can
; be tested later.  This section appears after the default segment
; declarations to avoid any problems with changing the default alignment
; characteristics.
;
IFDEF		__s__
	@CodeSize = 0
	@DataSize = 0
	pptr	equ	near ptr
	SHOW	<Assembling for the small memory model>
ELSEIFDEF	__m__
	@CodeSize = 1
	@DataSize = 0
	pptr	equ	far ptr
	SHOW	<Assembling for the medium memory model>
ELSEIFDEF	__c__
	@CodeSize = 0
	@DataSize = 1
	pptr	equ	near ptr
	SHOW	<Assembling for the compact memory model>
ELSEIFDEF	__l__
	@CodeSize = 1
	@DataSize = 1
	pptr	equ	far ptr
	SHOW	<Assembling for the large memory model>
ELSEIFDEF	__h__
	@CodeSize = 2
	@DataSize = 2
	pptr	equ	far ptr
	SHOW	<Assembling for the huge memory model>
ELSE	
	@CodeSize = 0
	@DataSize = 0
	pptr	equ	near ptr
	SHOW	<Assembler error: Missing memory model macro definition>
;	.ERR
ENDIF	; IFDEF


;
; Initializer/terminator structure format.  This is used by the
; startup code to automatically initialize run-time library packages.
;

InitRec		struc
ctype		db	?		; 0=near, 1=far, 0ffh=not used
pri		db	?		; 0=highest, 0ffh=lowest
foff		dw	?		; Function offset
fseg		dw	?		; Function segment
InitRec		ends

	.list

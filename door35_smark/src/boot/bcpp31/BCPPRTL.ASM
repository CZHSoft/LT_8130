	page	60, 132
	name	bcpprtl
	title	Borland C++ 3.1 Run-Time Library Support Package

;
; Paradigm LOCATE Run-Time Library Support for BC++ 3.1, Version 5.11
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
; This module is responsible for the installation and initialization
; of the Borland C++ run-time library support functions.  This
; module must be linked in anytime the floating point, stream I/O,
; dynamic memory management, or other run-time library functions
; needing definitions contained herein.
;
	
	INCLUDE	boot\bcpp31\startup.inc		; Macros and other assembly langauge defintions
	INCLUDE	boot\bcpp31\bcpp31.inc		; Include definitions

IFDEF	PDREMOTE
	SHOW	<Building Paradigm PDREMOTE-compatible library support>
ENDIF	; PDREMOTE

;
; External defintions
;
ExtProc	_startup			; Startup code entry point for errors
ExtLbl	EDATA, word			; Required for stack overflow checking

	subttl	Segment ordering/alignment section
	page
;
; Segments declared/referenced from this module
;
DefSeg	_TEXT,       word,  public,  CODE,       <>	; Default code
DefSeg	_DATA,       word,  public,  DATA,       DGROUP
DefSeg	_BSS,        word,  public,  BSS,        DGROUP
DefSeg	_INIT_,      word,  public,  INITDATA,   IGROUP	; Initializers

	page
_TEXT	segment
	assume	cs:_TEXT, ds:DGROUP

BegProc	_IOERROR, near			; Error code translation
	push	bp
	mov	bp, sp			; Prepare the stack frame
	mov	ax, [bp+4]		; After IP
	mov	_doserrno, ax
	pop	bp
	ret	2			; Pascal - must cleanup the stack
EndProc	_IOERROR

BegProc	_cleanup			; Call #pragma exit routines
	ret				; Never called by Paradigm startup
EndProc	_cleanup

BegProc	_checknull			; Check for null pointers
	ret				; Never called by Paradigm startup
EndProc	_checknull

BegProc	_restorezero			; Restore hooked interrupt vectors
	ret				; Never called by Paradigm startup
EndProc	_restorezero

;
; _terminate is used to service all error exits from an application
;
BegProc	_terminate			; Exit program with error code
IFNDEF	PDREMOTE			; ROMable code branch
	cli				; Disabling interrupts will force a
	hlt				; wait here till reset or NMI
	jmp	_startup		; ** Application dependent ***
ELSE
	mov	bp, sp			; Prepare the stack frame
IF	@CodeSize EQ MM_NEAR
	mov	ax, [bp+2]		; After IP
ELSE
	mov	ax, [bp+4]		; After CS:IP
ENDIF	; MM_NEAR
	xor	bx, bx			; Address the interrupt vector table
	mov	es, bx

	pushf				; Push the PSW
	push	bx			; IP
	push	bx			; CS
	jmp	dptr es:[000ch]		; Call the PDREMOTE break handler
ENDIF	; PDREMOTE
EndProc	_terminate

BegProc	abort				; RTL abort()
	mov	ax, PDE_ABORT		; Abort error code
	push	ax
	call	pptr _terminate		; Call the exit routine (no return)
EndProc	abort

;
; Pure virtual function support
;
BegProc	_fpure_error_, far
BegProc	_npure_error_, near
	mov	ax, PDE_PUREERR		; Return the pure virtual function code
	push	ax
	call	pptr _terminate		; Call the exit routine (no return)
EndProc	_npure_error_
EndProc	_fpure_error_

;
; Stack overflow checking support
;
public	N_OVERFLOW@, F_OVERFLOW@, OVERFLOW@
N_OVERFLOW@	label	near
F_OVERFLOW@	label	far
OVERFLOW@	proc
	mov	ax, PDE_STKOVL		; Return the stack overflow code
	push	ax
	call	pptr _terminate		; Call the exit routine (no return)
OVERFLOW@	endp

public	DGROUP@
DGROUP@	dw	DGROUP			; Used by RTL code
_TEXT	ends

_DATA	segment
GlobalW	_version, 0a02h			; Emulated DOS version number
GlobalW	__brklvl, DGROUP:EDATA		; Break level (needed for stack checking)
_DATA	ends

_BSS	segment				; Uninitialized data
GlobalW	errno				; RTL error numbers
GlobalW	_doserrno
_BSS	ends

	end

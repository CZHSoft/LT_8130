	page	60, 132
	name	bcppflt
	title	Borland C++ 3.1 Floating Point Support Package

;
; Paradigm LOCATE Floating Point Support for BC++ 3.1, Version 5.11
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
; of the 80x87/emulator floating point functions for Borland C++.
;
; Math coprocessor users should remove the module FP87INIT.OBJ from the
; library FP87.LIB while emulator users should remove the module FPINIT.OBJ
; from the library EMU.LIB.  The Paradigm LOCATE installation script will
; perform this automatically for you.
;

	locals	@@
	INCLUDE	boot\bcpp31\startup.inc		; Macros and other assembly langauge defintions
	INCLUDE	boot\bcpp31\bcpp31.inc		; Borland C++ helper macros
	INCLUDE	boot\bcpp31\bcppflt.inc		; Borland C++ floating point helpers


IFDEF	FLOAT				; Check that FLOAT has been defined
IF	FLOAT EQ FP87
	noemul				; Generate in-line 80x87 instructions
_Emu_	=	0			; no emulation, '87 required
_Fpu_	=	3			; 1 for 8087, 3 for 80C187 support
	SHOW	<Building Borland C++ 80x87 floating point support>
ELSE
	emul				; Generate emulator instructions
_Emu_	=	1			; Assume the math coprocessor emulation
_Fpu_	=	0			; Emulator type
	SHOW	<Building Borland C++ emulator floating point support>
ENDIF	; FLOAT EQ FP87
ELSE
	SHOW	<Illegal floating point option selected - check your code>
	.err
ENDIF	; FLOAT

;
; Segment declarations
;
DefSeg	_TEXT,       word,  public,  CODE,       <>	; Default code
DefSeg	_DATA,       word,  public,  DATA,       DGROUP
DefSeg	_BSS,        word,  public,  BSS,        DGROUP
DefSeg	_INIT_,      word,  public,  INITDATA,   IGROUP	; Initializers
DefSeg	E87_PROG     para,  public,  CODE,       <>
if	_Emu_
DefSeg	EMU_PROG,    para,  public,  CODE,       <>	; Emulator code segment
endif	; _Emu_

;
; Data references and definitions
;
_DATA	segment
GlobalW	_8087, -1
_DATA	ends

;
; External code and data, possibly not in DGROUP
;
ExtProc	_turboCvt, abs			; Force link of REALCVT for printf
ExtProc	_default87, word		; Default 80x87 control word
ExtProc	_emu, byte			; Floating point data structures
ExtProc	_fperror			; Floating point exception handler


FP_INT		= 10h			; Interrupt vector for exceptions
FpuPriority	= 16d			; Floating point initializer priority

	page
;
; FIXUP constants for floating arithmetic.  These determine whether in-line
; or emulator calls are generated.
;

emInt		= 34h			; Allocated to 8087 by Microsoft
shortcutInt	= 3Eh			; Used by Borland for shortcuts

public	FIWRQQ, FIDRQQ, FIARQQ, FICRQQ, FIERQQ
public	FISRQQ, FJARQQ, FJCRQQ, FJSRQQ

if	_Emu_
FIWRQQ		= 024fbh		; Convert NOP-WAIT to MOV AX, AX
FIDRQQ		= 05C32h		; wait, esc
FIARQQ		= 0FE32h		; wait, DS:
FICRQQ		= 00E32h		; wait, CS:
FIERQQ		= 01632h		; wait, ES:
FISRQQ		= 00632h		; wait, SS:
FJARQQ		= 04000h		; Esc nn -> DS:nn
FJCRQQ		= 0C000h		; Esc nn -> CS:nn
FJSRQQ		= 08000h		; Esc nn -> ES:nn
else
FIWRQQ		= 00000h		; nop, wait
FIDRQQ		= 00000h		; wait, esc
FIARQQ		= 00000h		; wait, DS:
FICRQQ		= 00000h		; wait, CS:
FIERQQ		= 00000h		; wait, ES:
FISRQQ		= 00000h		; wait, SS:
FJARQQ		= 00000h		; Esc nn -> DS:nn
FJCRQQ		= 00000h		; Esc nn -> CS:nn
FJSRQQ		= 00000h		; Esc nn -> ES:nn
endif	;_Emu_

	page
if	_Emu_
;
; This is the segment containing the full emulation code
;
EMU_PROG	segment
ExtLbl	e086_Entry, far
ExtLbl	e086_Shortcut, far
EMU_PROG	ends
endif	; _Emu_

;
; This is the segment containing the code to work with the 8087 chip
;
E87_PROG	segment
ExtLbl	e087_Entry, far
ExtLbl	e087_Shortcut, far
ExtLbl	e087_Trap, far
E87_PROG	ends

	page
_TEXT	segment
	assume	cs:_TEXT

;
; Floating point exceptions are routed to this function for resolution.
; This vector must be captured but the previous contents need to be
; remembered.  The field priorVector is used if the floating point exception
; interrupt is triggered but the coprocessor did not cause it.
;

fp_int_handler	proc	far
	push	ax
	mov	ax, seg __emu		; Segment holding the data structures
	call	e087_Trap		; Preserves all but ax, flags

;
; Returns only if error could not be handled with AX containing the offset
; to the error structure
;
	jnc	@@err			; Check if a floating point exception

;
; Floating point exception record is in SS:AX
;
	push	bx
	xchg	bx, ax			; SS:AX contains the trap record

;
; Set DS to DGROUP while the exception handler sorts things out
;
	mov	ax, seg _8087
	mov	ds, ax
	call	__fperror

;
; Restore the system state and exit
;
	pop	bx
	pop	ax
	iret

;
; If desired, user should add code to chain to the original contents of
; the interrupt vector used for floating point exceptions (if shared)
; otherwise declare an error since we got here without a floating point
; exception.

@@err:
	pop	ax			; Default is to ignore the error
	iret
fp_int_handler	endp

	page
;
; emu1st -- Emulator installation routine
;
; This function is called by the startup code as a initializer to install
; the interrupt vectors and initialize the floating point data structures.
; It is always near and must be placed in the segment _TEXT.
;
; The function _fpreset() installs the interrupt vectors and re-initializes
; the floating point package.
;

	assume	cs:_TEXT, ds:DGROUP, ss:seg __emu
emu1st	proc	near
;
; Set end of FPU stack area
;
	lea	ax, __emu + (size fpu)
	mov	ss:__emu.ws_lastSP, ax

;
; Set the start of FPU stack, leaving space to trap emulated underflows
;
	add	ax, MinFpStack - size_emu_temp
	mov	ss:__emu.ws_initSP, ax
	xor	ax, ax
	mov	ss:__emu.ws_LDT, ax
	mov	ss:__emu.ws_protected, al

;
; Set the coprocessor type being used
;
	mov	bx, _Fpu_
	mov	_8087, bx
	mov	ss:__emu.ws_8087, bl

;
; Fall thru to _fpreset() to set the initial conditions for the floating
; point package
;
emu1st	endp

	page
temp1	=	[bp-8]
temp2	=	[bp-4]

public	___fpreset
___fpreset	proc	near
	push	bp
	mov	bp, sp
	sub	sp, 8			; Space for the locals
	push	ds			; Save DS to restore later

;
; Install the appropriate set of floating point vectors
;
if	_Emu_
	mov	temp1.W0, offset e086_Entry
	mov	temp1.W1, seg e086_Entry
	mov	temp2.W0, offset e086_Shortcut
	mov	temp2.W1, seg e086_Shortcut
else
	mov	temp1.W0, offset e087_Entry
	mov	temp1.W1, seg e087_Entry
	mov	temp2.W0, offset e087_Shortcut
	mov	temp2.W1, seg e087_Shortcut
endif	; _Emu_

;
; Take over the interrupt vectors reserved for the use of the emulator and
; the 80x87 coprocessors.
;
	mov	ax, 2500h + emInt
	mov	cx, 10
	lds	dx, temp1.DD0
@@capture:
	int	21h
	inc	ax
	loop	@@capture

;
; The shortcut vector must also be installed
;
	mov	ax, 2500h + shortcutInt
	lds	dx, temp2.DD0
	int	21h

;
; Install FP exception handler into the specified vector
;
	mov	ax, 2500h + FP_INT
	push	cs
	pop	ds
	mov	dx, offset fp_int_handler
	int	21h

;
; Restore DS to its original contents
;
	pop	ds

;
; Now emulator or 80x87 support is installed so we an go ahead and
; initialize it
;
	finit				; Initialize the coprocessor
	mov	ss:__emu.ecount1, 0	; Used by the trap handler
	mov	ss:__emu.ecount2, 0

;
; The previous FINIT instruction will disable all exception interrupts
; Enable those which will be on by default
;
	mov	ax, _default87

;
; 80C187 is fully IEEE, need not trap denormal exceptions
;
	cmp	_8087, 3
	jl	@@ctrl
	or	ax, CW_DENORMAL
	mov	_default87, ax
@@ctrl:
	mov	temp1.W0, ax
	fldcw	temp1.W0

;
; Set the shadow control up
;
	mov	ax, temp1.W0
	and	ss:__emu.ws_control, ax

;
; Cleanup and exit - floating point is installed
;
	mov	sp, bp
	pop	bp
	ret
___fpreset	endp


;
; Function:	_fpuint
;
; Returns the interrupt vector being used for exception handling.
;

BegProc	_fpuint, far
        mov     ax, FP_INT		; Return our FP exception handler
        ret
EndProc	_fpuint

_TEXT	ends

	page
;
; Floating point startup code initializer.  This entry gets placed in the
; table of startup code initializers to automatically install the BC++
; floating point package whenever it is linked in.
;
_INIT_	segment
	InitRec	<MM_NEAR, PRI_FPU, offset emu1st, 0 >
_INIT_	ends

	end

	.xlist
;
; Paradigm LOCATE common assembly language definitions/macros
; Copyright (C) 1996 Paradigm Systems.  All rights reserved.
;
; *********************************************************************
; Permission to modify and distribute object files based on this
; source code is granted to licensed users of Paradigm LOCATE.
;
; Under no circumstances is this source code to be re-distributed
; without the express permission of Paradigm Systems.
; *********************************************************************
;
; Many of the macros define C-style functions and data declarations
; by appending a leading underscore to public symbols.  To fixup these
; symbols, a new symbol is defined which replaces all occurrences of
; the original.  This permits the C declaration to be used except when
; a name clash occurs.
;
; Should a name clash occur, such as in exit() and _exit() (since _exit
; is the alias created for the symbol exit), the use of manual delcarations
; must be used.
;

;
; Pointer shorthand for assembly language
;
bptr	equ	byte ptr		; Data references
wptr	equ	word ptr
dptr	equ	dword ptr
qptr	equ	qword ptr

nptr	equ	near ptr		; Function references
fptr	equ	far ptr

;
; Define a macro for defining segments.  The paramters in order are
;    - Segment name
;    - Alignment
;    - Combine type
;    - Class name
;    - Group name
;
; All macro arguments are mandatory except for a group name.
;
DefSeg	macro	sname, salign, scomb, sclass, sgroup
.errb	<sname>
.errb	<salign>
.errb	<scomb>
.errb	<sclass>
sname	segment	salign	scomb	'&sclass'
sname	ends
ifnb	<sgroup>
sgroup	group	sname
endif
endm

;
; Some simple macros for defining global (public) data where
;    B - byte	W - word
;    D - dword	Q - qword
; and 'ginit' is an optional initializer.
;
GlobalB	macro	gname, ginit
.errb	<gname>
ifnb	<ginit>
_&gname	db	ginit
else
_&gname	db	?
endif
public	_&gname
gname	equ	_&gname
endm	; GlobalB

GlobalW	macro	gname, ginit
.errb	<gname>
ifnb	<ginit>
_&gname	dw	ginit
else
_&gname	dw	?
endif
public	_&gname
gname	equ	_&gname
endm	; GlobalW

GlobalD	macro	gname, ginit
.errb	<gname>
ifnb	<ginit>
_&gname	dd	ginit
else
_&gname	dd	?
endif
public	_&gname
gname	equ	_&gname
endm	; GlobalD

GlobalQ	macro	gname, ginit
.errb	<gname>
ifnb	<ginit>
_&gname	dq	ginit
else
_&gname	dq	?
endif
public	_&gname
gname	equ	_&gname
endm	; GlobalQ

;
; Some simple macros for defining local (non-public) data where
;    B - byte	W - word
;    D - dword	Q - qword
; and 'linit' is an optional initializer.
;
LocalB	macro	lname, linit
.errb	<lname>
ifnb	<linit>
lname	db	linit
else
lname	db	?
endif
endm	; LocalB

LocalW	macro	lname, linit
.errb	<lname>
ifnb	<linit>
lname	dw	linit
else
lname	dw	?
endif
endm	; LocalW

LocalD	macro	lname, linit
.errb	<lname>
ifnb	<linit>
lname	dd	linit
else
lname	dd	?
endif
endm	; LocalD

LocalQ	macro	lname, linit
.errb	<lname>
ifnb	<linit>
lname	dq	linit
else
lname	dq	?
endif
endm	; LocalQ

;
; A pair of macros for defining public functions and marking the end.
; use BegProc to define a function with an optional forced memory model
; or use the default current memory model.  The macro EndProc is then
; used to close the function.
;
BegProc	macro	pname, pmodel
.errb	<pname>
ifb	<pmodel>
if	@CodeSize EQ 0
_&pname	proc	near
else
_&pname	proc	far
endif
else
_&pname	proc	pmodel
endif
public	_&pname
pname	equ	_&pname
endm	; BegProc

EndProc	macro	pname
.errb	<pname>
_&pname	endp
endm	; EndProc

;
; Some simple macros for defining labels where
;    B - byte	W - word
;    D - dword	Q - qword
; The optional parameter 'lpub' can be used to make the label public.
;
LabelB	macro	lname, lpub
.errb	<lname>
ifnb	<lpub>
public	_&lname
_&lname	label	byte
lname	equ	_&lname
else
lname	label	byte
endif
endm	; LabelB
		
LabelW	macro	lname, lpub
.errb	<lname>
ifnb	<lpub>
_&lname	label	word
public	_&lname
lname	equ	_&lname
else
lname	label	word
endif
endm	; LabelW

LabelD	macro	lname, lpub
.errb	<lname>
ifnb	<lpub>
public	_&lname
_&lname	label	dword
lname	equ	_&lname
else
lname	label	dword
endif	; ifnb
endm	; LabelD

LabelQ	macro	lname, lpub
.errb	<lname>
ifnb	<lpub>
public	_&lname
_&lname	label	qword
lname	equ	_&lname
else
lname	label	qword
endif
endm	; LabelQ

;
; A macro to determine the near/far-ness of an external function
; based on the defined memory model.  Also a macro for accessing external
; labels.
;
ExtProc	macro	ename, emodel
.errb	<ename>
ifb	<emodel>
if	@CodeSize EQ 0
extrn	_&ename : near
else
extrn	_&ename : far
endif
else
extrn	_&ename : emodel
endif
ename	equ	_&ename
endm	; ExtProc

ExtLbl	macro	ename, etype
.errb	<ename>
ifb	<etype>
extrn	&ename : byte
else
extrn	&ename : etype
endif
endm	; ExtLbl

;
; Some useful constants for checking code/data memory models and
; the floating point options.
;
MM_NEAR		equ	0		; Small code/data model definition
MM_FAR		equ	1		; Large code/data model definition
MM_HUGE		equ	2		; Huge code/data model definition

FPDFT		equ	0		; Default floating point option
FPALT		equ	1		; Alternate math
FPEMU		equ	2		; Emulator math
FP87		equ	3		; 80x87 in-line code

;
; Paradigm LOCATE initializer priorities
;
PRI_FARDATA	equ	0		; FAR_DATA initialization
PRI_INT21H	equ	1		; Int 21h installation
PRI_DMM		equ	2		; Dynamic memory management initialization
PRI_FPU		equ	2		; Floating point initialization
PRI_STDIO	equ	2		; Stream I/O initialization

;
; PDREMOTE exit codes
;
PDE_PUREERR	equ	242		; Pure virtual function error
PDE_DMMERR	equ	243		; Heap initialization error
PDE_FPEXCP	equ	244		; Floating point exception
PDE_STKOVL	equ	245		; Stack overflow
PDE_ABORT	equ	246		; abort() called
PDE_INTO		equ	247		; INTO exception
PDE_CHKIND	equ	248		; CHKIND exception
PDE_IOTRAP	equ	249		; I/O trap exception
PDE_ILLOP	equ	250		; Illegal opcode exception
PDE_ESCOP	equ	251		; Escape opcode exception
PDE_ZERODIV	equ	252		; Zero divide exception

	.list

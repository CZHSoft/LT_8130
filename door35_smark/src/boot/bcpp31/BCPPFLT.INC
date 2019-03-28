	.xlist

;
; Paradigm LOCATE Floating Point Support, Version 5.11
; For Borland C++ 2.0/3.x and Turbo C++ 1.0/3.0
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
; These defintions help initialize and access the registers and fields
; of the 80x87 numeric coprocessor.
;

; 8087/80187 status word bit fields
SW_INVALID	= 00001h		; Invalid operation
SW_DENORMAL	= 00002h		; Denormalized operand
SW_ZERODIVIDE	= 00004h		; Zero divide
SW_OVERFLOW	= 00008h		; Overflow
SW_UNDERFLOW	= 00010h		; Underflow
SW_INEXACT	= 00020h		; Precision (inexact result)

; 8087/80187 control word bit fields
CW_INVALID	= 00001h		; invalid
CW_DENORMAL	= 00002h		; denormal
CW_ZERODIVIDE	= 00004h		; zero divide
CW_OVERFLOW	= 00008h		; overflow
CW_UNDERFLOW	= 00010h		; underflow
CW_INEXACT	= 00020h		; inexact (precision)


; Some useful structures used to access memory in various formats
_fourWords	struc
W0	dw	?
W1	dw	?
W2	dw	?
W3	dw	?
_fourWords	ends

_twoDwords	struc
DD0	dd	?
DD1	dd	?
_twoDwords	ends


; The 8087/80187 FSTENV structure format
fp_env	struc
	ndp_control	dw	?
	ndp_status	dw	?
	ndp_tags	dw	?
	ndp_ip		dw	?
	ndp_ip_opcode	dw	?
	ndp_dp		dw	?
	ndp_dptop	dw	?
fp_env	ends


;
; These are the data structures used by the Borland/Turbo C++ math coprocessor
; support package, as defined in the include file EMUVARS.ASI.
;
; NOTE: These definitions and their layout must not be changed.
;       =======================================================
;

fpu	struc
ws_status	dw	?
ws_control	dw	?
ws_adjust	dw	?
ws_8087		db	?
ws_protected	db	?
ws_type		dw	?
ws_LDT		dw	?
ws_TOS		dw	?
ws_lastSP	dw	?
ws_initSP	dw	?
fp_state	fp_env	<>
ecount1		dw	?	
ecount2		dw	?	
ws_status2	dw	?	
fpx1		dt	?	
fpsword		dw	?	
ws_BPsafe	dw	?
ws_stamp	dd	?
ws_version	dw	?
ws_stackStyle	db	?
ws_unsigned	db	?
ws_saveVector	dd	?
ws_nmiVector	dd	?
fpu	ends

; Size of emu_temp
size_emu_temp	=	12

; The minimum size for the emulator stack.
MinFpStack	=	(17 * size_emu_temp)

	.list

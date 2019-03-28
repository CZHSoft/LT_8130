/*
//		Paradigm LOCATE MS-DOS Emulation Support,	Version 5.11
//		For Borland C++ and Turbo C++
//		Copyright (C) 1995 Paradigm Systems.  All rights reserved.
//
//		*********************************************************************
//		Permission to modify and distribute object files based on this
//		source code is granted to licensed users of Paradigm LOCATE.
//
//		Under no circumstances is this source code to be re-distributed
//		without the express permission of Paradigm Systems.
//		*********************************************************************
//
//		Function		
//		========
//		This file contains the code for serving some common MS-DOS requests
//		from the Borland/Turbo C++ run-time library.
//
//		NOTE: This module requires that BCPPRTL.ASM (or TCPPRTL.ASM	if TC++)
//		be part of the application.
*/

#if	defined(PDREMOTE)
#include	<stdlib.h>						/* exit() for PDREMOTE error handling */
#endif
#include	<dos.h>							/* enable(), disable(), MK_FP() */

#include	"typedefs.h"					/* General purpose type declarations */
#include	"dosemu.h"						/* Int 21h function/data prototypes */


UINT		wInt21Handlers ;				/* Count of installed handlers */


/*
//		Function:	Int21Handler	
//
//		This interrupt handler services the basic set of Int 21h calls from the
//		application.  Other modules, such as stream I/O support and dynamic memory
//		management routines will hook the Int 21h vector and filter out those
//		Int 21h services belonging to them.
//
//		This is the root Int 21h handler and it must be installed first so that
//		other Int 21h handlers can capture the vector for their own support.
*/

#pragma	option	-O-b					/* Disable dead-code optimizations */
#pragma	argsused							/* Ignored the unused parameters */
static
void	interrupt	Int21Handler(
				UINT	bp,	UINT	di,	UINT	si,
				UINT	ds,	UINT	es,
				UINT	dx,	UINT	cx,	UINT	bx,	UINT	ax,
				UINT	ip,	UINT	cs,	UINT	psw
			)
{
	UINT	far	*wp ;						/* General purpose 16-bit pointer */


	/* Clear the carry to indicate success */
	psw &= 0xfffe ;

	/* The contents of the AH register is used to determine the service */
	switch (highbyte(ax))   {
		case DOS_SETVECTOR:
			wp = (UINT far *) MK_FP(0, lowbyte(ax) << 2) ;
			disable() ;
			*wp++ = dx ;
			*wp = ds ;
			break ;
		
		case DOS_VERSION:
			/* Emulate MS-DOS 2.10 */
			ax = 0x0a02 ;
			break ;

		case DOS_GETVECTOR:
			wp = (UINT far *) MK_FP(0, lowbyte(ax) << 2) ;
			disable() ;
			bx = *wp++ ;
			es = *wp ;
			break ;

		default:
#if	defined(PDREMOTE)
			/* Return the DOS subfunction code as the exit code */
			exit(highbyte(ax)) ;
#endif
			/* All other functions are not supported */
			psw |= 1 ;
			break ;
	}
}


/*
//		Function:	InitInt21h
//
//		This function is responsible for initializing the Paradigm Systems
//		MS-DOS Int 21h emulation package by inserting the root Int 21h
//		interrupt into the interrupt vector table.
*/

static
void	far	InitInt21h(void)
{
	/* Install the interrupt handlers */
	setvect(0x21, Int21Handler) ;

	/* Bump the interrupt 21h handler count */
	wInt21Handlers++ ;
}
#pragma	startup	InitInt21h		1

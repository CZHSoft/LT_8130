/*
//		Paradigm LOCATE MS-DOS Stream I/O Support, Version 5.11
//		For Borland C++ 3.0 (or newer) and Turbo C++ 3.0
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
//		This module contains the device-independent initializations for using
//		Borland/Turbo C++ streams in an embedded system. This interface uses
//		the console device interface defined in the file CONSOLE.C.
//
//		NOTE: This module requires that DOSEMU.C and BCPPRTL.ASM (or TCPPRTL.ASM
//		if TC++) be part of the application.
*/

#pragma inline 							/* Uses inline assembly language */

#include	<fcntl.h>						/* file control definitions */
#include	<io.h>							/* setmode(), fileno() */
#include	<stdio.h>						/* setbuf() */
#include	<dos.h>							/* enable(), disable(), MK_FP() */

#include	"typedefs.h"					/* General purpose type declarations */
#include	"dosemu.h"						/* Int 21h function/data prototypes */


static	ISRP	pOldInt21 ;				/* Hooked Int 21h vector */


/*
//		Function:	Int21SioHandler	
//
//		This interrupt handler services any Int 21h calls for stream I/O
//		functions.  All other functions are passed on to the original
//		Int21 interrupt handler.
*/

#pragma	option	-O-b					/* Disable dead-code optimizations */
#pragma	argsused							/* Ignored the unused parameters */
static
void	interrupt	Int21SioHandler(
				UINT	bp,	UINT	di,	UINT	si,
				UINT	ds,	UINT	es,
				UINT	dx,	UINT	cx,	UINT	bx,	UINT	ax,
				UINT	ip,	UINT	cs,	UINT	psw
			)
{
	UINT		i ;							/* General purpose counter */
	CHAR		far *cp ;					/* General purpose character pointer */
	int		c ;							/* Temporary character storage */


	/* Re-enable interrupts during processing */
	enable() ;

	/* The contents of the AH register is used to determine the service */
	switch (highbyte(ax))   {
		case DOS_READ:
			cp = (CHAR far *) MK_FP(ds, dx) ;
			for (i = 0; i < cx; )   {
				c = _getch() ;
				/* Check for and process backspace characters */
				if (c == '\b')	{
					if (i > 0)	{
						_putch(c) ;
						_putch(' ') ;
						_putch(c) ;
						cp-- ;
						i-- ;
					}
					continue ;
				}
				/* Check for the end of the line */
				if (c == '\r')   {
					*cp++ = _putch('\r') ;
					*cp = _putch('\n') ;
					i += 2 ;
					break ;
				}
				/* Put the character in the buffer */
				_putch(*cp++ = c) ;
				i++ ;
			}

			/* Clear the carry to indicate success */
			psw &= 0xfffe ;

			/* Return the number of characters read in AX */
			ax = i ;
			break ;

		case DOS_WRITE:
			cp = (CHAR far *) MK_FP(ds, dx) ;
			for (i = 0; i < cx; i++)   {
				/* Check for and handle a backspace character */
				if (*cp == '\b')   {
					_putch(*cp) ;
					_putch(' ') ;
				}
				_putch(*cp++) ;
			}

			/* Clear the carry to indicate success */
			psw &= 0xfffe ;

			/* Return the number of characters written in AX */
			ax = i ;
			break ;

		case DOS_IOCTL:
			if (lowbyte(ax) == 0)   {
				dx = 0x80d3 ;
			}
			else   {
				psw |= 1 ;					/* Signal an error */
				ax = 6 ;						/* Invalid or not open handle */
			}
			break ;

		default:		 						/* All other functions */
			_chain_intr(pOldInt21) ;
			break ;
	}
}


/*
//		Function:	InitStreams
//
//		This function is responsible for initializing the console device
//		used by the stream I/O package.
*/

static
void		InitStreams(void)
{
	static char keybuf[BUFSIZ] ;		/* line input buffer */

	/* Install the stream extension Int 21h handler */
	pOldInt21 = getvect(0x21) ;
	setvect(0x21, Int21SioHandler) ;

	/* Bump the interrupt 21h handler count */
	wInt21Handlers++ ;

	/* Call the console device initialization routine */
	InitConsole() ;

	/* Assign buffered line input and unbuffered output stream */
	setbuf(stdin, keybuf) ;
	setbuf(stdout, NULL) ;

	/* Set streams in text mode */
	setmode(fileno(stdin), O_TEXT);
	setmode(fileno(stdout), O_TEXT);
}
#pragma	startup	InitStreams		2


/*
//		We need a little bit of assembly language magic to make sure
//		everything is linked in to support formatted input and output.
*/

#pragma	option	-w-
asm	_CVTSEG	segment	word	public	'DATA'
asm	__RealCvtVector	label		word
asm	public	__RealCvtVector
asm	_CVTSEG	ends
asm	_SCNSEG	segment	word	public	'DATA'
asm	__ScanTodVector	label		word
asm	public	__ScanTodVector
asm	_SCNSEG	ends
asm	DGROUP	group	_CVTSEG, _SCNSEG
#pragma	option	-w.

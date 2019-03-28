/*
//		Paradigm LOCATE Generic Console I/O Driver, Version 5.11
//		For Borland C++, Turbo C++ and Microsoft C/C++
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
//		This module contains a generic console device driver for the
//		Paradigm stream I/O interface containing the following functions.
//
//			void		InitConsole(void) ;
//			int		_getch(void) ;
//			int		_putch(int) ;
//
//		This version is specially customized to simulate the presence of a
//		serial port by reading/writing static buffers and it must be customized
//		for the target system before use.
//
//		NOTE: This module requires that DOSEMU.C and BCPPRTL.ASM (or TCPPRTL.ASM
//		if TC++) be part of the application.
*/

#include	"typedefs.h"					/* General purpose type declarations */
#include	"dosemu.h"					/* Int 21h function/data prototypes */


/*
//		Function:  _getch
//
//		This function returns the next character received from the console.
//		As implemented, it returns characters from a string and must be
//		modified to reflect the hardware in your target system.
*/

int	_getch(void)
{
	static	char	buf[] = "16\r512\r1024\r2048\r" ;
	static	int	i ;

	if (buf[i] == '\0')
		i = 0 ;

	return	buf[i++] ;
}


/*
//		Function:  _putch
//
//		This function must be written to copy the supplied character to
//		to the console device.
//
//		As implemented, it returns characters from a string and must be
//		modified to reflect the hardware in your target system.
*/

char		outbuf[256] ;					/* Output buffer */

int	_putch(
			int		ch						/* Character to output */
		)
{
	static	int	i = 0 ;				/* Output buffer offset */

	outbuf[i++] = ch ;
	if (i == sizeof(outbuf) || ch == '\n')
		i = 0 ;

	return	ch ;
}


/*
//		Function:	InitConsole
//
//		This function is responsible for initializing the console device
//		used by the stream I/O package.
//
//		This is the place to perform any initialization of UARTs, displays,
//		or other console hardware that may be required by your system.
*/

void		InitConsole(void)
{
	/* Add your console initialization code here */
}

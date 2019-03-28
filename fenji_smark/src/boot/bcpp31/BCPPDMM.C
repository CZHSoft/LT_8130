/*
//		Paradigm LOCATE Dynamic Memory Management Support, Version 5.11
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
//		This module contains a complete implementation of the heap management
//		interface for the Borland/Turbo C++ compiler.  As with DOS targets,
//		the near heap is supported for the small and medium memory models with
//		a far heap supported in all memory models.
//
//		The correct definition of HEAPEND is VERY IMPORTANT since as
//		implemented in this module, Paradigm LOCATE is not aware of the heap
//		and cannot perform any overlap or other checking.  The advantage of
//		this approach is that the far heap is optimally sized to use all
//		memory from the heap base to the paragraph address defined by the
//		HEAPEND definition.
//
//		NOTE: This module requires that DOSEMU.C and BCPPRTL.ASM (or TCPPRTL.ASM
//		if TC++) be part of the application.
*/

#pragma inline 							/* Uses inline assembly language */

#include	<dos.h>							/* enable(), disable(), MK_FP() */
#include	"arch\include\typedefs.h"					/* General purpose type declarations */
#include	"arch\include\dosemu.h"						/* Int 21h function/data prototypes */

/*
//		Define the paragrah address of the end of the far heap.  When HEAPEND
//		is 0x0000, no far heap is defined.  Note that this is the paragraph
//		address, not a memory address.  Some common settings for HEAPEND are
//		shown below:
//
//				System RAM Size		HEAPEND value
//				===============		=============
//					32K		0x0800
//					64K		0x1000
//					128K		0x2000
//					256K		0x4000
//					512K		0x8000
//
//		If you prefer a fixed size far heap, it can be accomplished by
//		declaring a character array of the appropriate size and initializing
//		the far heap variables to point to the array.  Far heaps larger than
//		64K can be accomodated by declaring the array huge.
//
//		NOTE: Paradigm DEBUG/RT - PDREMOTE users should set HEAPEND to the
//		end of the usable RAM for application data, not including the
//		"simulated ROM" area where application code is loaded.  Other users
//		may simply set HEAPEND to the physical end of system RAM.
*/

#if	!defined(HEAPEND)
#define	HEAPEND		0x4000			/* Paragraph address of the far heap */
#endif	/* !defined(HEAPEND) */


/*
//		The following are checks on the correctness of the heap definitions.
//		A check is made that a far heap is setup in the compact/large memory
//		models (no near heap is supported for these models).
*/

#if		(HEAPEND == 0) && (defined(__COMPACT__) || defined(__LARGE__) || defined(__HUGE__))
#error	Must define a far heap in the compact/large/huge memory model
#endif


#if	defined(__SMALL__) || defined(__MEDIUM__)
/*
//		Near heap management variables.  __brklvl is defined in BCPPRTL.ASM (or TCPPRTL.ASM)
//		since it is also used by the stack overflow checking logic.
*/

			UINT		__heaptop ;			/* Top of the heap */
			UINT		__heapbase ;		/* Base of the heap */
extern	UINT		__brklvl ;			/* Current break level */
#endif	/* defined(__SMALL__) || defined(__MEDIUM__)*/

#if	(HEAPEND != 0)						/* Define when a far heap is desired */
/*
//		Far heap management variables
*/

ULONG		_brklvl ;						/* Current break level */
ULONG		_heaptop ;						/* Top of the far heap */
ULONG		_heapbase ;						/* Base of the far heap */
UINT		_psp ;							/* PSP needed to resolve externals */

static	ISRP	pOldInt21 ;				/* Original Int 21h vector */


/*
//		Function:	Int21DmmHandler	
//
//		This interrupt handler services any Int 21h calls for dynamic
//		memory management functions.  All other functions are passed on
//		to the original Int21 interrupt handler which was saved to allow
//		the use of the _chain_intr() function.
//
//		This support is only required when a far heap is implemented.
*/

#pragma	option	-O-b					/* Disable dead-code optimizations */
#pragma	option	-a-					/* Forced packed structures */
#pragma	argsused							/* Ignored the unused parameters */
static
void	interrupt	Int21DmmHandler(
				UINT	bp,	UINT	di,	UINT	si,
				UINT	ds,	UINT	es,
				UINT	dx,	UINT	cx,	UINT	bx,	UINT	ax,
				UINT	ip,	UINT	cs,	UINT	psw
			)
{
	MCBP		ma ;							/* Pointer to the current MCB */
	MCBP		nma ;							/* Pointer to the next MCB */
	MCBP		tmp ;							/* A working pointer to an MCB */
	UINT		mcb ;							/* Application segment identifier */


	/* Re-enable interrupts during processing */
	enable() ;

	/* The contents of the AH register is used to determine the service */
	switch (highbyte(ax))   {
		case DOS_REALLOC:
			/* Make a pointer to the MCB */
			ma = (MCBP) MK_FP(es - 1, 0) ;

			/* Verify we are pointing to an MCB */
			if (ma->id != 'M' && ma->id != 'Z')   {
				/* Bad block identifier - return an error code */
				ax = 9 ;
				psw |= 1 ;
				return ;
			}

			/* Now either shrink or expand it */
			if (bx > ma->size)   {
 				/* Expand - is the next block is free/sufficiently large */
				nma = (MCBP) MK_FP(es + ma->size, 0) ;
				tmp = (MCBP) MK_FP(es + bx, 0) ;
				if (nma->owner != 0 || (ma->size + nma->size) < bx)   {
					/* Return the size block that can be allocated */
					ax = 8 ;
					bx = ma->size + ((nma->owner == 0) ? nma->size : 0) ;
					psw |= 1 ;
					return ;
				}

				/* Steal the space from the second MCB */
				mcb = bx - ma->size ;
				ma->size = bx ;

				/* Initialize the new smaller MCB */
				tmp->id = nma->id ;
				tmp->owner = nma->owner ;
				tmp->size = nma->size - mcb ;
			}
			else if (bx < ma->size)   {
				/* Release - create a new MCB holding the unallocated memory */
				tmp = (MCBP) MK_FP(es + ma->size, 0) ;
				nma = (MCBP) MK_FP(es + bx, 0) ;
				nma->size = ma->size - bx - 1 ;
				nma->owner = 0 ;
				nma->id = ma->id ;

				/* Shrink the size to the specified number */
				ma->size = bx ;

				/* Combine adjacent blocks if possible */
				if (ma->id != 'Z' && tmp->owner == 0)   {
					nma->size += tmp->size + 1 ;
					nma->id = tmp->id ;
				}

				/* Insert into the chain */
				if (ma->id == 'Z')
					ma->id = 'M' ;
			}

			/* Clear the carry to indicate success */
			psw &= 0xfffe ;
			break ;

		default:
			/* All other functions are passed to the old handler for service */
			_chain_intr(pOldInt21) ;
			break ;
	}
}
#endif	/* (HEAPEND != 0) */


/*
//		Function: InitDMM
//	
//		This initialization function is responsible for initializing the
//		near and far heaps for dynamic memory management.  Which functions
//		are installed and initialized are determined by the memory model used
//		and the value of HEAPEND.
*/

//static
void	far	InitDMM(void)
{
#if	(HEAPEND != 0)						/* Define when a far heap is desired */
	MCBP		ma ;							/* Pointer to the current MCB */
	UINT		wHeapBase ;					/* Paragraph address of heap base */
	UINT		wHeapEnd ;					/* Paragraph address of heap end */
	extern	void	_terminate(int) ;	/* Used for error checking by PDREMOTE */


	/* Save the original Int 21h handler and install ours */
	pOldInt21 = getvect(0x21) ;
	setvect(0x21, Int21DmmHandler) ;

	/* Bump the interrupt 21h handler count */
	wInt21Handlers++ ;

	/* Set up the far heap addressing */
	asm	mov	ax, _FARHEAP
	wHeapBase = _AX ;
	ma = (MCBP) MK_FP(wHeapBase++, 0) ;

	/* Use the far heap end frame */
	wHeapEnd = HEAPEND ;

#if	defined(PDREMOTE)
	/* Check the heap limits for obvious errors */
	if (wHeapBase > wHeapEnd)   {
		/* Call the PDREMOTE exit routine (no return) */
		_terminate(PDE_DMMERR) ;
	}
#endif	/* defined(PDREMOTE) */

	/* Initialize the far heap variables */
	_heapbase = _brklvl = (ULONG) MK_FP(wHeapBase, 0x0000) ;
	_heaptop = (ULONG) MK_FP(wHeapEnd, 0x0000) ;
	_psp = wHeapBase ;

	/* Initialize the far heap to be a single block */
	ma->id = 'Z' ;
	ma->owner = 0 ;
	ma->size = wHeapEnd - wHeapBase ;
#endif	/* (HEAPEND != 0) */

#if	defined(__SMALL__) || defined(__MEDIUM__)
	/* Initialize the near heap variables */
	__heaptop = __heapbase = __brklvl ;
#endif	/* defined(__SMALL__) || defined(__MEDIUM__) */
}
#pragma	startup	InitDMM			2


/*
//		We need a little bit of assembly language magic to make sure
//		everything is linked in to support the far heap.  In this case,
//		a small segment is defined to hold the first MCB in the far heap,
//		with the heap end determined by the value assigned to HEAPEND.
*/
#if	(HEAPEND != 0)				/* Define when a far heap is desired */
#pragma	option	-w-
asm	_FARHEAP	segment	para	public	'FARHEAP'
asm	db		16 dup (?)
asm	_FARHEAP	ends
#pragma	option	-w.
#endif	/* (HEAPEND != 0)	*/

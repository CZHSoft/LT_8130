/*
//		Paradigm LOCATE General purpose definition, Version 5.11
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
//		This file contains the general purpose definitions common to
//		all Paradigm applications.  By defining synonyms for the physical
//		data types to be manipulated, portability between memory models
//		and machines is maximized.
//
//		Note that this file follows the system include files and before
//		any application include files.
//
*/

#if	!defined(_TYPEDEFS)
#define	_TYPEDEFS

/*
// Borland/Turbo C++ and Microsoft C/C++ compiler compatibility macros
*/
#if	defined(__TURBOC__)
#define	NEAR				near
#define	FAR				far
#define	INTERRUPT		interrupt

typedef	unsigned 		BIT ;					/* Borland/Turbo C++ bit fields */
#if	defined(__cplusplus)
typedef	void INTERRUPT (*ISRP)(...) ;		/* Interrupt table vector entries */
#else
typedef	void INTERRUPT (*ISRP)() ;
#endif

#else

#if	defined(_MSC_VER) && (_MSC_VER >= 700)
#define	NEAR				__near
#define	FAR				__far
#define	INTERRUPT		__cdecl __interrupt __far
#else
#define	NEAR				_near
#define	FAR				_far
#define	INTERRUPT		_cdecl _interrupt _far
#endif	/* defined(_MSC_VER) && (_MSC_VER >= 700) */

typedef	unsigned char	BIT ;					/* Microsoft C/C++ bit fields */
#if	defined(__cplusplus)
typedef	void (INTERRUPT *ISRP)(...) ;		/* Interrupt table vector entries */
#else
typedef	void (INTERRUPT *ISRP)() ;
#endif

#endif	/* defined(__TUBROC__) */

/*
// Compiler-dependent, general purpose types
*/
typedef	char				CHAR ;
typedef	unsigned char	UCHAR ;
typedef	int				INT ;
typedef	unsigned int	UINT ;
typedef	long				LONG ;
typedef	unsigned long	ULONG ;

typedef	UCHAR FAR *		FPTR ;

typedef	float				FLOAT ;
typedef	double			DOUBLE ;
typedef	long double		LDOUBLE ;

/*
// Compiler-independent, fixed size types
*/
typedef	char						INT8 ;		/* Signed 8-bit integer */
typedef	unsigned char			UINT8 ;		/* Unsigned 8-bit integer */
typedef	short int				INT16 ;		/* Signed 16-bit integer */
typedef	unsigned short int	UINT16 ;		/* Unsigned 16-bit integer	*/
typedef	long int					INT32 ;		/* Signed 32-bit integer */
typedef	unsigned long int		UINT32 ;		/* Unsigned 32-bit integer */

typedef	float						FLOAT32 ;	/* 32-bit IEEE single precision */
typedef	double					FLOAT64 ;	/* 64-bit IEEE double precision */
typedef	long double				FLOAT80 ;	/* 80-bit IEEE max precision */

typedef	void FAR *				PTR ;			/* Pointer to any data type */
typedef	UINT8 FAR *				PTR8 ;		/* Pointer to 8-bit data */
typedef	UINT16 FAR *			PTR16 ;		/* Pointer to 16-bit data */
typedef	UINT32 FAR *			PTR32 ;		/* Pointer to 32-bit data */

typedef	unsigned char			BYTE ;		/* 8-bit data */
typedef	unsigned short int	WORD ;		/* 16-bit data */
typedef	unsigned long int		DWORD ;		/* 32-bit data */

typedef	BYTE FAR *				BYTE_PTR ;	/* Pointer to 8-bit data */
typedef	WORD FAR *				WORD_PTR ;	/* Pointer to 16-bit data */
typedef	DWORD FAR *				DWORD_PTR;	/* Pointer to 32-bit data */

/*
//	Common used type definitions and macros
*/
typedef	enum   {
		PDE_PUREERR = 242,				/* Pure virtual function error */
		PDE_DMMERR = 243,					/* Heap initialization error */
		PDE_FPEXCP = 244,					/* Floating point exception */
		PDE_STKOVL = 245,					/* Stack overflow */
		PDE_ABORT = 246,					/* abort() called */
		PDE_INTO = 247,					/* INTO exception */
		PDE_CHKIND = 248,					/* CHKIND exception */
		PDE_IOTRAP = 249,					/* I/O trap exception */
		PDE_ILLOP = 250,					/* Illegal opcode exception */
		PDE_ESCOP = 251,					/* Escape opcode exception */
		PDE_ZERODIV = 252					/* Zero divide exception */
	} PDEXIT ;								/* PDREMOTE exit codes */

typedef	enum   {
				FALSE = 0,
				TRUE
			} BOOL ;							/* Simple enumeration for booleans */

typedef	union	  {
		struct	{
			UINT16	lower16 ;			/* Lower 16-bits access mode */
			UINT16	upper16 ;			/* Upper 16-bits access mode */
		} s ;									/* Union definition */
		UINT32		whole32 ;			/* 32-bit access mode */
	} VALUE32, *VALUE32P ;				/* 32-bit address manipulation */

typedef	union	  {
		struct	{
			UINT16	lw ;					/* Lower 16-bits access mode */
			UINT16	uw ;					/* Upper 16-bits access mode */
		} s ;									/* Union definition */
		UINT32		ul ;					/* 32-bit access mode */
	} ULS, *ULSP ;							/* 32-bit address manipulation */

/*
//	lowbyte(x)				Extract the low order byte of UINT 'x'
//	highbyte(x)				Extract the high order byte of UINT 'x'
//	dim(x)					Computes the dimension of an array 'x'
//	setvect(inum, addr)	Install interrupt handler 'addr' in vector 'inum'
//	getvect(inum)			Get the contents of interrupt vector 'inum'
*/

#define	lowbyte(word)			((word) & 0xff)
#define	highbyte(word)			lowbyte((word) >> 8)
#define	dim(x)					(sizeof(x) / sizeof(x[0]))
#define	setvect(inum, addr)	*((ISRP FAR *) ((inum) * 4)) = ((ISRP) addr)
#define	getvect(inum) 			(ISRP) (*((ISRP FAR *) ((inum) * 4)))

#if	!defined(min)
#define max(a,b)	(((a) > (b)) ? (a) : (b))
#define min(a,b)	(((a) < (b)) ? (a) : (b))
#endif

#if	!defined(MK_FP)
#define	MK_FP(seg,ofs)	((void FAR *) (((UINT32)(seg) << 16) | (UINT16)(ofs)))
#endif

#endif	/* !defined(_TYPEDEFS) */

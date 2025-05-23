/**
 *	\file dp_types.h
 *	\brief contains the data types supported by Drivers in both 32 bit and 64 bit 
 *
 *	This file contains the data types supported by Drivers in both 32 bit and 64 bit 
 *	\author Shanmugam S
 *	\date 04:01PM on June 24, 2016
 *
 *	\revision
 *	- Initial version
 *
 *	\copyright Copyright (C) 2016 Data Patterns (India) Ltd. \n
 *	All Rights Reserved.\n
 *	Address:	Plot # H9, Fourth Main Road, SIPCOT IT Park, Siruseri, \n
 *				Off Rajiv Gandhi Salai (OMR), Pudupakkam P.O., \n
 *				Chennai-603103 | India\n
 *				Website : http:// www.datapatternsindia.com/\n
 *				Phone: 91-44-4741-4000\n
 *				FAX: 91-44-4741-4444 \n
 *
	////////////////////////////REVISION LOG ENTRY//////////////////////////////////
	Date		Version Number	     Reason for Revision	Revised by 
	----		--------------		 -------------------	----------
									 

	 
*/

#ifndef _DP_TYPES_H_
#define _DP_TYPES_H_

typedef unsigned char         	U8BIT;
typedef unsigned short        	U16BIT;
typedef unsigned int          	U32BIT;

typedef char           		  	S8BIT;
typedef short                 	S16BIT;
typedef int                   	S32BIT;

typedef unsigned char *        	PU8BIT;
typedef unsigned short *       	PU16BIT;
typedef unsigned int *         	PU32BIT;

typedef char *         		  	PS8BIT;
typedef short *                	PS16BIT;
typedef int *                   PS32BIT;

typedef float					FSINGLE;
typedef double					FDOUBLE;

typedef float *					PFSINGLE;
typedef double *				PFDOUBLE;

typedef void * 					DP_DRV_HANDLE;
typedef void * 					HANDLE;
typedef void * 					PVPTR;
typedef unsigned int 		DPPOINTER32;
/*Arch = x86 and MS VC++ version <= 6.0 then long long is not supported*/
#if (defined(_M_IX86) && defined(_X86_)) && (_MSC_VER <= 1200)

typedef __int64					S64BIT;
typedef unsigned __int64		U64BIT;

typedef __int64 *				PS64BIT;
typedef unsigned __int64*		PU64BIT;

#define STDCALL __stdcall


#else

typedef long long 	 			S64BIT;
typedef unsigned long long 		U64BIT;

typedef long long * 			PS64BIT;
typedef unsigned long long *	PU64BIT;
typedef char char_t;

#if !(defined(_MSC_VER)) && !(defined(__MINGW32__) || defined(__MINGW64__)) // If complier is not msvc and MinGW
#define STDCALL
#else //If MSVC version is > 6.0 
#define STDCALL __stdcall
#endif
#endif /*#if (defined(_M_IX86) || defined(_X86_)) && (_MSC_VER <= 1200)*/

#if defined(_MSC_VER)
#define PRAGMA(X) 		__pragma(X)
#define PRAGMA_PACK_PUSH(n)	PRAGMA(pack(push,n))
#define PRAGMA_PACK_POP()	PRAGMA(pack(pop))
#define PACK_ATTRIBUTE
#elif  defined(__GNUC__)
#define PRAGMA_PACK_PUSH(n)
#define PRAGMA_PACK_POP()
#define PACK_ATTRIBUTE 		__attribute__((packed,aligned(1)))
#else

  #error "Unsupported compiler"

#endif
#endif /*#ifndef _DP_TYPES_H_*/

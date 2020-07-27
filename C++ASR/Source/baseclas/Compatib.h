//  *********************************************************************
//  *                   This file is part of RES 6.0.                   *
//  *    RES 6.0 is an original software distributed within the book    *
//  *                                                                   *
//  *      |-----------------------------------------------------|      *
//  *      | "Speech Recognition: Theory and C++ Implementation" |      *
//  *      |               John Wiley & Sons, Ltd                |      *
//  *      |    by Claudio Becchetti and Lucio Prina  Ricotti    |      *
//  *      |-----------------------------------------------------|      *
//  *                                                                   *
//  *       See copyright.txt file for further info. on copyright       *
//  *********************************************************************

//  _____________________________________________________________________
//  |-------------------------------------------------------------------|
//  |                                                                   |
//  |   FILE:   compatibility.h                                         |
//  |   FUNCTIONALITY: multiplatform define are here                    |
//  |   PROGRAM: required to  all codes                                 |
//  |   COMMENTS:header                                                 |
//  |   CONTRIBUTIONS: Claudio			                                |
//  |   ACTUAL REVISION: 6.0                                            |
//  |   DATA ACTUAL REVISION: 19/11/98                                  |
//  |   FIRST VERSION:   1.0                                            |
//  |   DATA FIRST VERSION: 1/11/94                                     |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________

//*******************    VERSION HISTORY  *******************************
// <<< version 4.0 >>>
//24/4/98 added unix compatibility added TWO_BYTE_SHORT

//16/9/97 read code revision by claudio

//check option __CONSOLE__ for other compilers
// <<< version 2.0 >>>
// 1) OK MS 5.0 compiler
//2 added date sistem version
//17/5/98 added unix compatibility

#ifndef __COMPATIBILITY_H_
#define __COMPATIBILITY_H_


#include "../baseclas/defopt.h"


#define _SYSTEM_VERSION_INFO_  "** Ver. " ## _DEFOPT_VERSION_
#define COMPATIBILITY_VERSION "Ver. 6.0 of 19/11/98 " ##__FILE__ ## " c." ## __DATE__

//short int is assumed to be two bytes
typedef short int TWO_BYTE_SHORT;

#ifndef __cplusplus
	#error Must use C++ for these routines
#endif



#ifdef _Windows
	#include <windows.h>

	#ifdef _WINDOWS_MESSAGES_ON_MESSAGE_BOX_
		#define MESSAGE_THOROUGH_MSG_BOX
	#endif

#endif

#if defined(_Windows) || defined(__MSDOS__) || defined (_WIN32)
#define  _DOS_WINDOWS_WORLD_
#endif
#if defined(__GNUG__)
#undef  _DOS_WINDOWS_WORLD_
#endif

// this is the default
#define __32BIT_CODE__


//MCS consider that operator[](int) is different from operator(t_index)

//therefore both must be defined / decklared

//for other compilers (i.e. GNU) this provokes ambigous operator
#ifdef _MSC_VER

#define _OPERATOR_SQUARE_BRACKET_TO_BE_DECLARED_ALSO_FOR_INT

#endif



#ifdef __BORLANDC__
	#if (__BCPLUSPLUS__==0x0310 || __BCPLUSPLUS__ ==	0x340)
		#undef __32BIT_CODE__

		#ifndef __MSDOS__   //windows
			#undef __CHECK_MEMORY_ALLOC
		#else
			#undef MESSAGE_THOROUGH_MSG_BOX
		#endif

	#endif
#endif

#ifndef __32BIT_CODE__
	#define _HUGEPTR_ huge
#else
	#define _HUGEPTR_
#endif

//check memory alloc is specific of borland
#if defined (__CHECK_MEMORY_ALLOC) && (!defined(__MSDOS__) || !defined(__BORLANDC__))
	#undef __CHECK_MEMORY_ALLOC
#endif

#if (__BCPLUSPLUS__ ==	0x340) && !defined (__32BIT_CODE__)
	#pragma message ("this compiler does not accept object T of ImpObjectList/ImpaBaseList os size not multiple of 16, or sizeof(T)=8,4,2,1 if the list is greater then 64K verify all T")
#endif

#if !(__BCPLUSPLUS__ ==	0x340) && !defined (__32BIT_CODE__)
	#pragma message ("this compiler may not accept object T of ImpObjectList/ImpaBaseList os size not multiple of 16, or sizeof(T)=8,4,2,1 if the list is greater then 64K verify all T")
#endif


#endif

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
//  |	FILE:	boolean.h	                                            |
//  |   FUNCTIONALITY: Boolean definitions and max min                  |
//  |   PROGRAM: required to  all codes                                 |
//  |	COMMENTS:                                                       |
//  |	CONTRIBUTIONS: Claudio    Antonio					            |
//  |	ACTUAL REVISION: 6.0                                            |
//  |	DATA ACTUAL REVISION: 19/11/98                                  |
//  |	FIRST VERSION:   1.0                                            |
//  |	DATA FIRST VERSION: 1/11/94                                     |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________

// *******************    VERSION HISTORY  *******************************
//<<versione 4.0 >>
//16/9/97 read code revision by claudio
// <<< version 2.0 >>>
//1) OK  min and max are defined in stdlib

#ifndef _Boolean_type

	#define _Boolean_type

	#define AND &&
	#define OR ||
	#define NOT !

	#ifdef BOOL
		#define Boolean BOOL
	#else
		  typedef int Boolean;
	#endif

	#ifndef FALSE
		#define FALSE 0
		#define TRUE 1
	#endif

#endif

#if !defined( __MINMAX_DEFINED) 
#define __MINMAX_DEFINED
template <class T> T max(T x, T y)
	{
	return (x > y) ? x : y;
	};

template <class T> T min(T x, T y)
	{
	return (x < y) ? x : y;
	};

#endif





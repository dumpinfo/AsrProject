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
//  |   FILE:   defopt.h                                                |
//  |   FUNCTIONALITY: all options for the compilar in define are here  |
//  |   PROGRAM: required to  all codes                                 |
//  |   COMMENTS:header                                                 |
//  |   CONTRIBUTIONS: Claudio			                                |
//  |   ACTUAL REVISION: 6.0                                            |
//  |   DATA ACTUAL REVISION: 19/11/98                                   |
//  |   FIRST VERSION:   4.0                                            |
//  |   DATA FIRST VERSION: 19/11/94                                    |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________

//*******************    VERSION HISTORY  *******************************

// <<< version 4.0 >>>

//25/4/98 added Unix compatibility
////16/9/97 read code revision by claudio
//tested under borland
// 1) added version info
// 2) 15/9/97 revised by claudio
// 3) added options for vectors
//*******************    VERSION HISTORY  *******************************

#ifndef __DEF_OPT_H_
#define __DEF_OPT_H_

#define _DEFOPT_VERSION_  "6.0 of 19/11/98 File:" __FILE__##" " ## __DATE__


// this option is required by strange compilers/linkers
#define INSTANTIATE_DUMMY_CLASSES_FOR_STRANGE_COMPILERS

// ******************* define of diagnostic.h *******************
#define NO_CHECK
//these options should be automaticly set in compatib.h
//this options allows message box in windows
//#define _WINDOWS_MESSAGES_ON_MESSAGE_BOX_

//this options eliminate messages on screen
/*
#define _NO_MESSAGES_ON_SCREEN
*/
#define DEFAULT_DIAGNOSTIC_FILE "msg_dgn.dgn"


// ***************   define of baseclas.h *******************
// these options should be automaticly set in compatib.h
//flat memory and 32 bit pointer and 32 bit int expected
/*#define __32BIT_CODE__
#define __CHECK_MEMORY_ALLOC
*/
#define _DEFAULT_DATA_CHUNK 4L
// ***************   define of textclas.h *******************

const unsigned long MAX_LEN_STRING=64000UL;

// ***************   define of vetclas.h ********************

const double REAL_ZERO=0.00000000000000001;
 
// ************** define of diagclas ************************

const double PRECISION=0.00000001;

// ************** define of soundfile ***********************

#define WAVE_EXTENCTION ".wav"

// ************** define of resconfig ***********************

const unsigned long MAX_LEN_INPUT_LINE=800;

// ************* define of lessico **************************

const unsigned long __MAX_LEN_LINE=100;

const unsigned long MAX_R=5;

// ************ define of recognition module ****************

const double LOGSMALL=-0.5E+10;

#define MINLOGEXP -log(1.0E+10)

// ************ define of tspecbas **************************

const unsigned long OUTPUT_SIZE=20;

const double LOGZERO=-1.0E+10;

#include<math.h>
#if !defined(PI)
	const double PI=3.14159265358979323846;
	
#endif

const long MINEXPARG=-700;

const double MINLOGARG=0.001;

const double MINMIX=0.001;

const unsigned long MINMODELPROB=10;

#endif

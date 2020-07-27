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
//  |   FILE:   diagnostic.h                                            |
//  |   FUNCTIONALITY: diag. message routines for all files             |
//  |   PROGRAM: required to  all codes                                 |
//  |   COMMENTS:                                                       |
//  |   CONTRIBUTIONS: Claudio			                                |
//  |   ACTUAL REVISION: 6.0                                            |
//  |   DATA ACTUAL REVISION: 19/11/98                                  |
//  |   FIRST VERSION:   2.0                                            |
//  |   DATA FIRST VERSION: 23/9/96                                     |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________


// *******************    VERSION HISTORY  *******************************
//17/5/98 added unix
//15/9/97 read code revision by claudio

// *******************   USER INFORMATION   ******************************

/*
Functionality:

1) Check:
		for pre/post condition to verify critical conditions which occur
		frequently: ( may be switched off )

2) Assert:
		for pre/post condition to verify critical conditions which occur
		frequently: ( may be switched off )

3) StatusMessage
		to report message on log screen

4) WarningMessage
		to report malfunctioning on screen and log file
		( doesn't suspend program, may be catched in debuggers)

5) ErrorMessage
		to report fatal errors on screen and log file
		(stops the program with a messagebox if enabled,
		 may be catched in debuggers,
		 after stop program is aborted)

for the last two messages file , line and time of occurrance is reported
on diag file the name of the diag file is modifiable run time

the beginning and at the end of the program is reported on
diag file

Syntax

Check(condizion AND ...OR ...NOT ...,"remarks"<<numbers<<data<<..);
Assert(condition);
merr<<"remarks"<<numbers<<data<<..    ;
mstat<<"remarks"<<numbers<<data<<..   ;
mwarn<<"remarks"<<numbers<<data<<..   ;

Note !!!!
if you want to catch warnings and abort in debugger set the breakpoint
on the indicated lines on diagnost.cpp routine TDiagnosticMessages(ostream & p)
*/

// *******************   IMPLEMENTATION INFORMATION   *******************************
/*
compilation switches
#define _NO_MESSAGES_ON_SCREEN //no dump on screen
#define NO_CHECK //eliminates check controls
//#define MESSAGE_THOROUGH_MSG_BOX display fatal errors on winboxes
all output is sent to cerr cout are used only to send results of real time
recognize and segment programms

*/

#ifndef __DIAGNOSTIC_H_
#define __DIAGNOSTIC_H_

/***************compilation switches are in defopt.h ********/

//#define DEFAULT_DIAGNOSTIC_FILE "msg_dgn.dgn"

#include "../baseclas/boolean.h"
//must be set over the define
#include "../baseclas/compatib.h"

#include <iostream.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#ifdef _DOS_WINDOWS_WORLD_
	#include <strstrea.h>
#else
	#include <strstream.h>
#endif




// ********************** Check Assert ***********

	#ifdef NO_CHECK
		#define Check(p,q) ((void)0)

	#else
		#define Check(p,q) \
			if(!(p)) 	\
				merr<<"Check Aborted: "<<#p<<": "<<q

	#endif

	#ifdef NO_CHECK
		#define Assert(p) ((void)0)

	#else
		#define Assert(p) \
			if(!(p)) 	\
				merr<<"Assert Aborted: "<<#p

	#endif

// **********************define di merr mwarn e mstat***********

#define merr  __mmsg=__m_temp<<"ERR.: "<< __FILE__<<" Line:"<< __LINE__<<": "
#define mwarn __mmsg=__m_temp<<"Warn.: "<< __FILE__<<" Line:"<< __LINE__<<": "
#define mstat __mmsg=__m_temp<<" "



class TDiagnosticMessages
	{

	public:
	//must be static because first constructor is always invoked
	//and reset these values
	static char diag_file[120];

	//main routine invoked to show messages
	//on this function Breakpoint must be set in order to stop
	void operator=(ostream & p);

	TDiagnosticMessages() ;
	~TDiagnosticMessages() ;

	void Display_Message_On_Screen(char * text);
	void Display_Message_On_Screen_And_Wait(char * text);
	void Dump_Message_On_Log_File(char * text) ;

	};


extern class TDiagnosticMessages __mmsg;
extern class ostrstream __m_temp;

Boolean Test_Diag_Routines();


#endif

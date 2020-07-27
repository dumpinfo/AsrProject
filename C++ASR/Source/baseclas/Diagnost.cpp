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
//  |   FILE:   diagnostic.cpp                                          |
//  |   FUNCTIONALITY: code for diagnostic routines                     |
//  |   PROGRAM: required to  all codes                                 |
//  |   COMMENTS:                                                       |
//  |   CONTRIBUTIONS: Claudio			                                |
//  |   ACTUAL REVISION: 6.0                                            |
//  |   DATA ACTUAL REVISION: 19/11/98                                  |
//  |   FIRST VERSION:   1.0                                            |
//  |   DATA FIRST VERSION: 1/11/94                                     |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________

#define DIAG_CLASS_VERSION "6.0 of 19/11/98 " ## "File:" ## __FILE__ " "## __DATE__

// *******************    VERSION HISTORY  *******************************
////16/9/97 read code revision by claudio

// *******************          Code       *******************************

#include "../baseclas/diagnost.h"
#include <string.h>

//this is to avoid the no instantiation code bug on some strange compilers
#ifdef INSTANTIATE_DUMMY_CLASSES_FOR_STRANGE_COMPILERS
ostrstream dummy_stupid_class;
#endif

char TDiagnosticMessages::diag_file[120];

TDiagnosticMessages::TDiagnosticMessages()
		{
		#ifdef NO_CHECK
			char *temp="**  Program Starts: disabled Check/Assert   **";
		#else
			char *temp="**  Program Starts: ENABLED Check/Assert   **";
		#endif

		strncpy(diag_file,DEFAULT_DIAGNOSTIC_FILE,120);
		Dump_Message_On_Log_File(temp);
		Display_Message_On_Screen(temp);
		Dump_Message_On_Log_File(_SYSTEM_VERSION_INFO_);
		Display_Message_On_Screen(_SYSTEM_VERSION_INFO_);

		return;
		};

TDiagnosticMessages::~TDiagnosticMessages()
		{

		Display_Message_On_Screen("--  Program Ends --");
		Dump_Message_On_Log_File("--  Program Ends --");
		return;
		}

void TDiagnosticMessages::operator=(ostream & p)
		{
		ostrstream *temp;
		int temp_char;
		temp =(ostrstream *)&p;
		temp->operator<<('\0');
		temp_char=(int) temp->str()[0];

		//mastat message
		if( temp_char!='E' AND  temp_char!='W')
			 Display_Message_On_Screen(temp->str());

		//mawarn message
		if(temp_char=='W')
			{

//****************
			//put a breakpoint over here to catch warnings

			Display_Message_On_Screen(temp->str());
			Dump_Message_On_Log_File(temp->str());
			}

		 //maerr message
		 if( temp_char=='E')
			{
			Boolean must_stop;
			Display_Message_On_Screen_And_Wait(temp->str());
			Dump_Message_On_Log_File(temp->str());

//****************
			//put a breakpoint over here to catch abort
			must_stop=TRUE;

			if (must_stop)
				exit(0); 
			}


		(temp->rdbuf())->freeze(0);
		 temp->seekp(0);
		 return;
		 }



void TDiagnosticMessages::Dump_Message_On_Log_File(char * text)
	{

	FILE * file;
	time_t t;
	char *tptr;

	file=fopen(diag_file,"a+");
	if (file==NULL)
		{
		Display_Message_On_Screen("Unable to open Diagnostic File");
		return;
		}
	time(&t);
	tptr=  ctime(&t);
	tptr[strlen(tptr)-1]='\0';
	fprintf(file,"%s:%s\n",tptr,text);
	fclose(file);
	return;
	}


void TDiagnosticMessages::Display_Message_On_Screen_And_Wait(char * text)
	{
	#ifndef _NO_MESSAGES_ON_SCREEN
		#ifdef MESSAGE_THOROUGH_MSG_BOX
		MessageBeep(MB_ICONEXCLAMATION);
		MessageBox(NULL,text,"Fatal error",MB_OK|MB_ICONEXCLAMATION);
			#else
			char temp[120];
			Display_Message_On_Screen(text);
			Display_Message_On_Screen("push a key and after return to exit");
			cin>>temp;
			#endif
	#endif
	return;
	}




void TDiagnosticMessages::Display_Message_On_Screen(char * text)
	{
	#ifndef _NO_MESSAGES_ON_SCREEN
	cerr<<text<<endl ;
	#endif
	}

Boolean Test_Diag_Routines()
	{
	unsigned long i=24;

	mwarn<<"Compatibilty file Version: "<<COMPATIBILITY_VERSION;
	mwarn<<"def opt      file Version: "<<_DEFOPT_VERSION_ ;
	mwarn<<"Diagnostic Sistem Version: "<<DIAG_CLASS_VERSION;

	mwarn<<"Testing Diagnostic System " ;
	mstat<<"show "<<20<<"messages";
	for(i=1;i<=20;i++)
		mstat<<"Status Message Number: "<<i<<" other "<<(20-i)<<" are expected";

	mwarn<<"warning message of numbers:"<<i<<23;
	Check(i>20,"check test"<<i);
	Assert(i>20);
	//merr<<"end of program";
	mwarn<<"Diagnostic System OK" ;
	return TRUE;
	}


class ostrstream __m_temp;
class TDiagnosticMessages __mmsg;
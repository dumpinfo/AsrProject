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
//  |   FILE:   Iresconfi.h                                             |
//  |   FUNCTIONALITY: class ConfigFile definition                      |
//  |   PROGRAM: required to  codes which opens ires config file        |
//  |   COMMENTS:                                                       |
//  |   CONTRIBUTIONS: Fabrizio   Claudio                               |
//  |   ACTUAL REVISION: 6.0									        |
//  |   DATA ACTUAL REVISION: 19/11/98                                  |
//  |   FIRST VERSION:   1.0                                            |
//  |   DATA FIRST VERSION: 20/11/95                                    |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________


// *******************    VERSION HISTORY  *******************************
// <<< version 4.0 >>>
//9/2/98 eliminated const char *
//4/02/98 added option String in option file and options in opt file
//may stay in a different winsection then in ini files
//25/9/97 read code revision by claudio
//25/9/97 fixed bugs on filin no passed by reference,
//25/9/97 deleted all non print chars from search buffer
//07/1/97 comments included by claudio

// *******************   USER INFORMATION   ******************************


// *******************   IMPLEMENTATION INFORMATION   *******************************
// the default options are not used by the engines as the engines have their 
// own defaults

#ifndef _CONFIG_H_
#define _CONFIG_H_


#include <fstream.h>
#include <limits.h>
#include <float.h>

#include "../baseclas/textclas.h"
#include "../baseclas/boolean.h"
#include "../baseclas/defopt.h"
#include "../baseclas/diagnost.h"
#include "../vetclas/vetclas.h"
#include "../vetclas/vetclas.hpp"

// Class to read options from config file (=*.ini)
// requires *.opt system file to check the coherence of the options

class ConfigFile
	{
	ifstream file_ini, file_opt;
	Boolean file_open;
	
	//returns the (string) option related to the option win_option_type
	//section: win_section in the file:file
	void Get_Option(ifstream & file, const String &  win_section, const String & win_option_type,
						String &option);

	// returns the left and right side substring of option
	//the centre string is str
	//if str not included in option left=option; right is empty
	void Take_Left_And_Right(const String &  option, const String & str, String &left, String & right);

	public:

	ConfigFile();
		
	~ConfigFile();

	//open *.ini and *.opt file default is res.ini 
	//option file is always res.opt
	void Open_File(const String & nome_file);

	//return the string in win_option_type
	//the string must be among those reported in the res.opt file
	//win_section refers to *.ini file while win_opt_section refers to
	//res.opt file if not specified in this file the win_section is used
	void Get_String_Opt(const String &  win_section, const String &  win_option_type,
			 String & option,const String &  win_opt_section="");


	//return the unsigned in win_option_type
	//the unsigned must be in the range specified in res.opt file
	t_index Get_Unsigned_Opt (const String &  win_section,
		const String &  win_option_type,const String &  win_opt_section="");

	//return the real in win_option_type
	//the real must be in the range specified in res.opt file
	t_real Get_Real_Opt (const String &  win_section,
			const String &  win_option_type,const String &  win_opt_section="");

	//return a boolean
	Boolean Get_Boolean_Opt (const String &  win_section, const String &  win_option_type);

		  // Return a vector of integers. Range check is performed
	// syntax like 1,2-4 are allowed in config file
	// returned list includes all the numbers 1,2,3,4
	void Get_Range_Opt(const String &  win_section,
			const String &  win_option_type, VetULong & range,
			const String &  win_opt_section="");
	};

Boolean Test_Config_Class();
#endif

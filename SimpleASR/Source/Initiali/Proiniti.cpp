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
//  |   FILE:   Proiniti.cpp                                            |
//  |   FUNCTIONALITY: main function for start initialization           |
//  |   PROGRAM:                                                        |
//  |   COMMENTS:                                                       |
//  |   CONTRIBUTIONS: Antonio & Fabrizio 	Domenico					|
//  |   ACTUAL REVISION: 6.0                                            |
//  |   DATA ACTUAL REVISION: 19/11/98					                |
//  |   FIRST VERSION:   1.0                                            |
//  |   DATA FIRST VERSION: 18/6/95                                     |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________


#include "../initiali/initiali.h"
#include "../initiali/iniopt.h"
#include "../ioclass/soundlab.h"


void main(int argc, char* argv[])
	{	
	Initiali symbol_models_initialization;
	DbaseVoc dbase;

	IniOptions ini_options; // class for options of inizialization procedure
 	 	
 	String config_file;
	
	// ****************************************************************
	// 			
	//			OPEN CONFIG FILE, CONFIGURE INITIALIZATION
	//
	// ****************************************************************

	if ( argc!=2)
		{
		mstat<<"No configfile name specified, using: res.ini";
		config_file="res.ini";
		}
	else config_file=argv[1];
	
	//	***************************************************************
	//
	//			SET DBASE 			CONFIGURE DBASE
	//
	//			from configuration file "config_file"
	//
	// ****************************************************************
	
	// get dbase configuration options
	dbase.Configure(config_file, TRUE);
	
	ini_options.Set_Options(config_file, dbase.Get_Num_Of_Symbols());
	
	//String 	list_of_sound_fnames;
	//list_of_sound_fnames = ini_options.ListOfSoundFNames;
		
	symbol_models_initialization.Configure(ini_options, config_file, 
													dbase.Get_Num_Of_Symbols());

	symbol_models_initialization.Symb_Model_Calculation(ini_options.InitializedModelsFName,
						  ini_options.models_file_input, dbase, ini_options.full_covariance,
						  ini_options.load_one_mixture, ini_options.unif_sect, 
						  ini_options.model_type);

	mstat<<"ok!";
	return;
	};

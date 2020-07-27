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
//  |   FILE:   main_lessico.cpp                                        |
//  |   FUNCTIONALITY: main program of lessico module                   |
//  |   PROGRAM:                                                        |
//  |   COMMENTS:                                                       |
//  |   CONTRIBUTIONS: Domenico		                                    |
//  |   ACTUAL REVISION: 4.0                                            |
//  |   DATA ACTUAL REVISION: 19/11/98                                  |
//  |   FIRST VERSION:   2.0                                            |
//  |   DATA FIRST VERSION: 23/1/96                                     |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________


// *******************    VERSION HISTORY  *******************************
// <<< version 4.0 >>>


#define LESSICO_VERSION "V.6.0 of 19/11/98 " ## __FILE__ " c." ## __DATE__


#include "../lessico/lessico.h"
#include "../lessico/lexopt.h"

int main (int argc, char * argv[])
	{
    String conf_file_name;
    LexiconOptions lexic_options;

	if (argc!=2)
		{
		mwarn << "No configure file name specified, using \"res.ini\""<<endl;
		conf_file_name= "res.ini";
		}
	else conf_file_name=argv[1];

	lexic_options.Set_Options(conf_file_name);
	LexiconTimit lexicon_timit;    
	LexiconAtis2 lexicon_atis2;
	LexiconAtis3 lexicon_atis3;
	Lexicon * lexic_database[]={&lexicon_timit, &lexicon_atis2, &lexicon_atis3};
    
	if (lexic_options.trainingLM)
		lexic_database[lexic_options.database]->Build_Language_Model(lexic_options);
	if (lexic_options.perplexity_comput)
		lexic_database[lexic_options.database]->Compute_Perplexity (lexic_options);
    if (lexic_options.trainphongr)
		lexic_database[lexic_options.database]->Train_Phon_Grammar (lexic_options);
    if (lexic_options.phon_perplexity)
		lexic_database[lexic_options.database]->Compute_Phon_Perplexity (lexic_options);

	mstat << endl << "Test is ok";
	return 1;
	}
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
//  |   FILE:  recopt.cpp                                               | 
//  |   FUNCTIONALITY:                                                  |
//  |   PROGRAM:                                                        |
//  |   COMMENTS:                                                       |
//  |   CONTRIBUTIONS: Domenico								            |
//  |   ACTUAL REVISION: 6.0							                |
//  |   DATA ACTUAL REVISION: 19/11/97                                  |
//  |   FIRST VERSION:   2.0                                            |
//  |   DATA FIRST VERSION: 20/2/96                                     |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________

#define RECOPT_VERSION "V.6.0 of 19/11/98 " ## __FILE__ " c." ## __DATE__

#include "recopt.h"

void RecogOptions::Config_File_Name(String & conf_fname)const
	{
	conf_fname = config_file_name;
	return ;
	};


void RecogOptions::Set_Options(const String & file_name)
	{
    ConfigFile config;
	String string_option;
         
	//open config file
	config_file_name=file_name;
	config.Open_File(config_file_name);

    //store algorithm type 
	config.Get_String_Opt("Recognition", "AlgorithmType", string_option);
	if (string_option=="windowsearch")
		{
		algorithm=WindowSearch;
		window_width=config.Get_Unsigned_Opt("Recognition", "WindowWidth");
		}
	else //if (string_option=="BeamSearch")
		{
		algorithm=BeamSearch;
		beam_initial=config.Get_Real_Opt("Recognition", "BeamCoefficientInitial");
		beam_internal=config.Get_Real_Opt("Recognition", "BeamCoefficientInternal");
		}

    //maximum volume of hypothesis tree
    tree_volume=config.Get_Unsigned_Opt("Recognition", "TreeVolume"); 

    //names of output files
	config.Get_String_Opt("Recognition", "FileOfSolutions", file_of_solutions);
    config.Get_String_Opt("Recognition", "FileOfResults", file_of_results);

    //file of acoustic model
    config.Get_String_Opt("Recognition", "FileOfAcousticModels", file_of_acoustic_models);

    //static grammar level
    config.Get_String_Opt("Recognition", "StaticGrammar", string_option);
    if (string_option=="phongrammar")
		grammar=PhonGrammar;
	else //if (string_options=="WordGrammar")
		grammar=WordGrammar; 

    penalty=config.Get_Real_Opt ("Recognition", "GrammarProbPenalty");
    penalty_sil=config.Get_Real_Opt ("Recognition", "SilenceProbPenalty");
	bigr_th=config.Get_Real_Opt ("Recognition", "BigrTh");
	skip_th=config.Get_Real_Opt ("Recognition", "SkipTh");
	desc_gr_const=config.Get_Real_Opt ("Recognition", "DescGrConst");

    //grammar type
    config.Get_String_Opt("Recognition", "GrammarType", string_option);
    if (string_option=="nogrammar")
		grammar_type=NoGrammar;
	else // if (string_option=="Bigram")
		grammar_type=BiGram;

	//file of phonemes grammar
    config.Get_String_Opt("Recognition", "FileOfBigramPhonemeGrammar", file_of_phonemes_bigram);

    //file of word bigram
	config.Get_String_Opt("Recognition", "FileOfBigramWordGrammar", file_of_bigram);

	//file of vocabulary
	config.Get_String_Opt("Recognition", "FileOfVocabulary", file_of_vocabulary);
 
    config.Get_String_Opt("Recognition", "WordClass", string_option);
	if (string_option=="noclassif")
		word_class=NoClassif;
	else //if(string_option=="YesClassif")
		{
		word_class=YesClassif;

		config.Get_String_Opt("Recognition", "FileOfClasses", file_of_classes);
		config.Get_String_Opt("Recognition", "FileOfClassGrammar", file_of_bigram_classes);
		}

    return;

	}
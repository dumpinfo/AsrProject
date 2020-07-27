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
//  |   FILE:   lexopt.cpp                                              |
//  |   FUNCTIONALITY: lessico options                                  |
//  |   PROGRAM:                                                        |
//  |   COMMENTS:                                                       |
//  |   CONTRIBUTIONS: Domenico		                                    |
//  |   ACTUAL REVISION: 6.0                                            |
//  |   DATA ACTUAL REVISION: 19/11/98                                  |
//  |   FIRST VERSION:   2.0                                            |
//  |   DATA FIRST VERSION: 23/1/96                                     |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________


// *******************    VERSION HISTORY  *******************************
// <<< version 4.0 >>>

#include "../lessico/lexopt.h"

inline void LexiconOptions::Config_File_Name (String & file_name)const
	{
	file_name=config_file_name;
	return;
	}
    
void LexiconOptions::Set_Options (const String & file_name)
    {
	ConfigFile config;
	String string_option, string_opt;
         
	//open config file
	config_file_name=file_name;
	config.Open_File(config_file_name);

    if (config.Get_Boolean_Opt("Lexicon", "TestRecog"))
		{
		config.Get_String_Opt("Lexicon", "TestRecogFileList", file_recog);
		return;
		}

	//about label type
	config.Get_String_Opt("DBaseOptions", "LabelFiletype", string_option);
	if (string_option=="ntimitreducedlabel")
	   label_type=NTimitReduced;
	else if (string_option=="ntimitlabel")
			label_type=NTimit;
	else if (string_option=="atislabel")
			label_type=AtisLabels;
	else if (string_option=="ntimit39label")
	        label_type=NTimit39;
	else if (string_option=="atisreducedlabel")
			label_type=AtisReducedLabels;

    //about database
    config.Get_String_Opt("Lexicon", "Database",string_option);
	if (string_option=="timit")
		database = timit;
	else if (string_option=="atis2") 
			{ 
	        database = atis2;
			config.Get_String_Opt("Lexicon", "AtisPhonFile", atis_phon_file);
			}
	else //if (string_option=="atis3")
		{
		database = atis3; 
		config.Get_String_Opt("Lexicon", "AtisPhonFile", atis_phon_file);
		}

	//about operation(s) requested
    if (trainingLM=config.Get_Boolean_Opt("Lexicon", "TrainingLM"))
		config.Get_String_Opt("Lexicon", "FileTrainList", file_train_list);
	if (trainphongr=config.Get_Boolean_Opt("Lexicon", "TrainPhonGr"))
		    config.Get_String_Opt("Lexicon", "FileTrainList", file_train_list);
	if (perplexity_comput=config.Get_Boolean_Opt("Lexicon", "Perplexity"))
		{
		config.Get_String_Opt("Lexicon", "FilePerplexList", file_perplex_list);
		config.Get_String_Opt("Lexicon", "FilePerplex", file_perplex);
		}
    if (phon_perplexity=config.Get_Boolean_Opt("Lexicon", "PhonPerlexity"))
		{
		config.Get_String_Opt("Lexicon", "FilePerplexList", file_perplex_list);
		config.Get_String_Opt("Lexicon", "FilePerplex", file_perplex);
		}

    //about grammar type
	config.Get_String_Opt("Lexicon", "GrammarType", string_option);
	if (string_option=="unigram")
		{
        grammar = Unigram;
		config.Get_String_Opt("Lexicon", "FileUnigram", file_unigram);
		}
	else{
		config.Get_String_Opt ("Lexicon", "Smoothing", string_opt);
		if (string_opt=="linear")
			smooth_techn=Linear;
		else if (string_opt=="nonlinear")
				smooth_techn=NonLinear;
		else if (string_opt=="turing")
			    smooth_techn=Turing;
		else //if (string_opt=="nosmoothing")
				smooth_techn=NoSmoothing;

		if (string_option=="bigram")
			{
			grammar = Bigram;
			config.Get_String_Opt("Lexicon", "FileBigram", file_bigram);
			}
		}	                   
	//about classification
	config.Get_String_Opt ("Lexicon", "ClassType", string_option);
    if (string_option=="noclass")
		class_type=NoClass;
	else if (string_option=="hand")
			{
			class_type=HandClass;
		    config.Get_String_Opt("Lexicon", "FileHandClass", file_hand_classes);            
			}
	else //if (string_option=="autom")
		{
		class_type=AutomClass;
		config.Get_String_Opt("Lexicon", "FileAutomClass", file_autom_classes);
		max_num_classes=config.Get_Unsigned_Opt("Lexicon", "MaxNumClas");
		}	
		 
	//about file of the vocabulary
	config.Get_String_Opt("Lexicon", "FileOfVocabulary", file_vocabulary);

   	return;
	}
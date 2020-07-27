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
//  |   FILE:   lexopt.h                                                |
//  |   FUNCTIONALITY: lessico options                                  |
//  |   PROGRAM:                                                        |
//  |   COMMENTS:                                                       |
//  |   CONTRIBUTIONS: Domenico			                                |
//  |   ACTUAL REVISION: 6.0                                            |
//  |   DATA ACTUAL REVISION: 19/11/98                                  |
//  |   FIRST VERSION:   2.0                                            |
//  |   DATA FIRST VERSION: 23/1/96                                     |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________


// *******************    VERSION HISTORY  *******************************
// <<< version 4.0 >>>

#ifndef __LEXOPT_
#define __LEXOPT_

#include "../resconf/resconf.h"
#include "../baseclas/textclas.h"
#include "../vetclas/vetclas.h"

enum DatabaseName {timit, atis2, atis3};
enum GrammarType {Unigram, Bigram};
enum SmoothingTechn {NonLinear,Linear,Turing, NoSmoothing};
enum ClassType {HandClass, AutomClass, NoClass};
enum LabelType {NTimitReduced, NTimit, AtisLabels, NTimit39, AtisReducedLabels};

class LexiconOptions
	{
     
		String config_file_name;

	public:
     
		//database name over which perform language model training 
		DatabaseName database;

		//type of label
		LabelType label_type;

		//if requested language model training 
		Boolean trainingLM;

		//if requested perplexity computation
		Boolean perplexity_comput;

		//if requested phonemes grammar training
		Boolean trainphongr;

		//if requested perplexity phonemes grammar perplexity computation
		Boolean phon_perplexity;

		//different approssimation of sentence probability 
		GrammarType grammar;

		//word classification type
		ClassType class_type;

		//file of the list of files name which perform language model training
		String file_train_list;

		//type of interpolation scheme for computing conditional probability
		SmoothingTechn smooth_techn;

		//file of the list of files name which perform perplexity computation
		String file_perplex_list;

		//file of the list of files name which perform recognition
		String file_recog;

		//file whith hand made word classes
		String file_hand_classes;

		//output file whith automatic word classes
		String file_autom_classes;

		//max number of automatic classes
		t_index max_num_classes;

		//file with phonetic transcription of Atis
		String atis_phon_file;

		//file with vocabulary and phonetic transcription of each word
		String file_vocabulary;

		//file with logprobability of unigram 
		String file_unigram;

		//file with matrix of bigram transition
		String file_bigram;

		//file with the result of perplexity computation
		String file_perplex;

		//public function for loading options from config file
		void Set_Options (const String & file_name);

		void Config_File_Name (String & file_name)const;

	};

#endif

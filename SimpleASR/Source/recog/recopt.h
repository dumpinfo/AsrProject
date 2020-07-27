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
//  |   FILE:   recopt.h                                                |
//  |   FUNCTIONALITY: options of recognizer                            |
//  |   PROGRAM:                                                        |
//  |   COMMENTS:                                                       |
//  |   CONTRIBUTIONS: Domenico		                                    |
//  |   ACTUAL REVISION: 6.0                                            |
//  |   DATA ACTUAL REVISION: 19/11/98                                  |
//  |   FIRST VERSION:   2.0                                            |
//  |   DATA FIRST VERSION: 20/2/96                                     |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________


#ifndef _RECOG_OPTIONS
#define _RECOG_OPTIONS

#include "../resconf/resconf.h"
#include "../baseclas/baseclas.h"
#include "../baseclas/textclas.h"
#include "../lessico/lexopt.h"

enum AlgorithmType {BeamSearch, WindowSearch};
enum StaticGrammarLevel {PhonGrammar, WordGrammar};
enum StaticGrammarType {NoGrammar, BiGram};
enum WordClass {NoClassif, YesClassif};

class RecogOptions
	{

		String config_file_name;

	public:

		//algorithm for limiting the hypothesis tree
		AlgorithmType algorithm;
	
		//max hyponode number 
		t_index tree_volume;

		// window search option
		t_index window_width;

		// beam search option
		t_real beam_initial;
		t_real beam_internal;

		t_real penalty;
		t_real penalty_sil;
		t_real desc_gr_const;

		t_real bigr_th;
		t_real skip_th;

		//output files
		String file_of_solutions;     
		String file_of_results;     
                           
		//file with the acoustic model of phonemes         
		String file_of_acoustic_models;

		//type of static grammar
		StaticGrammarLevel grammar;
		StaticGrammarType grammar_type;

		//file of vocabulary
		String file_of_vocabulary;
     
		String file_of_phonemes_bigram;
		String file_of_bigram;

		WordClass word_class;

		String file_of_classes;
		String file_of_bigram_classes;

		void Config_File_Name(String & conf_fname)const;

		// public function for loading option from configuration file
		void Set_Options(const String & file_name);
	};

#endif

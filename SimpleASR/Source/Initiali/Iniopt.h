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
//  |   FILE:   iniopt.h                                                |
//  |   FUNCTIONALITY: class for read options                           |
//  |   PROGRAM:                                                        |
//  |   COMMENTS:                                                       |
//  |   CONTRIBUTIONS: Antonio  & Fabrizio 	Domenico                    |
//  |   ACTUAL REVISION: 6.0                                            |
//  |   DATA ACTUAL REVISION: 19/11/98						            |
//  |   FIRST VERSION:   1.0                                            |
//  |   DATA FIRST VERSION: 24/4/96                                     |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________



#ifndef ini_opt
#define ini_opt

#include "../baseclas/baseclas.h"
#include "../ioclass/soundlab.h"
#include "../resconf/resconf.h"


enum Range{All, Partial, Category};
enum ModelType {Model_0, Model_1, Model_2, Model_3};

class IniOptions
	{

	
public:

	String InitializedModelsFName;
	Boolean load_one_mixture;
	Boolean unif_sect;
	String models_file_input;
	String FileOfSymbolModels;
	Range SymbolRange; 		// perhaps not needed: if all phonemes specified
							// set phonemes list at proper value
	VetULong SymbolList;
	
	Boolean full_covariance;

    ModelType model_type;
		
	void Set_Options(const String& config_fname, t_index num_of_symbols);

	};

#endif

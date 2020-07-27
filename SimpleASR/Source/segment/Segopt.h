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
//  |   FILE:   segopt.h   derived from segopt.cpp in recognition       |
//  |   FUNCTIONALITY: options for segmentation                         |
//  |   PROGRAM:                                                        |
//  |   COMMENTS:                                                       |
//  |   CONTRIBUTIONS: Fabrizio		                                    |
//  |   ACTUAL REVISION: 6.0                                            |
//  |   DATA ACTUAL REVISION: 19/11/98                                  |
//  |   FIRST VERSION:   4.0                                            |
//  |   DATA FIRST VERSION: 12/9/97                                     |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________

#ifndef _segment_options
#define _segment_options

#include "../resconf/resconf.h"
#include "../baseclas/baseclas.h"
#include "../baseclas/textclas.h"

#include "../ioclass/soundfil.h"

enum AlgorithmType{BeamSearch, WindowSearch};
enum StaticGrammarType{WordGrammar, PhonGrammar, SmallVocabularies, NoGrammar};
enum OutputStyle {RES, ARES};

class SegmentOptions
	{ 
		String config_file_name;
	public:
 
		AlgorithmType decoding_algo;
		OutputStyle out_style;
    
		t_index WindowWidth;
		t_index	TreeVolume;

		// beam search option
		t_real BeamCoefficient;
		String FileOfAcousticModels;
	
		Boolean sil_opt;

		void SetOptions(const String& filename);

	};

#endif

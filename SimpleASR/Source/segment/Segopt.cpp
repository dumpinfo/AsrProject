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
//  |   FILE:   segopt.cpp derived from segopt.cpp in recognition       |
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

#include "segopt.h"

void SegmentOptions::SetOptions(const String& filename)
	{
    ConfigFile conf;
	String buffer;
		
	config_file_name=filename;
	conf.Open_File(filename);
	conf.Get_String_Opt("Segment","OutputStyle",buffer);
	
	if (buffer=="res")
		out_style=RES;
	else //if (buffer=="ares")
		 out_style=ARES;
	
	// definition of the dinamic Viterbi structure
 	conf.Get_String_Opt("Segment","AlgorithmType",buffer);
	if (buffer=="windowsearch")
		{
		decoding_algo=WindowSearch;
		// read TreeVolume value
		TreeVolume=conf.Get_Unsigned_Opt("Segment","TreeVolume");
		// read WindowWidth value
		WindowWidth=conf.Get_Unsigned_Opt("Segment","WindowWidth");
		}
	else{ 	
		if (buffer=="beamsearch")
			{
			decoding_algo=BeamSearch;
			// read WindowWidth value
			BeamCoefficient=conf.Get_Real_Opt("Segment","BeamCoefficient");
			}
		else merr<<"Alghoritm type not implemented"; 	
		}

	conf.Get_String_Opt("Segment","AlgorithmType",buffer);
	conf.Get_String_Opt("Segment","FileOfAcousticModels",FileOfAcousticModels);
	sil_opt=conf.Get_Boolean_Opt("Segment","SilenceOpt");
	
	return;	
	}

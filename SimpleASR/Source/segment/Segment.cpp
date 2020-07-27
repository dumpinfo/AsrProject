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
//  |   FILE:   segment.cpp  derived by recog.cpp                       |
//  |   FUNCTIONALITY: main for segmentation                            |
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
 // Set to silence is wrong since the first tree construction pass
 // it is constrained to reach the entering hypoes of each state of
 // each grammar level
 // Check if it chooses the best or the worst hypoes

#include "../segment/hypolist.h"
#include "../segment/hypolist.hpp"
#include "segopt.h"

int main(int argc, char* argv[])
	{
	SpectralHmm hmm;		  // grammar static structure
	HypoTree tree;			  // Viterbi dinamic structure
	DbaseVoc dbase;
	
	SegmentOptions options;
    
	VetULong utter_label_list;
	String act_dataf_name;	  	// file of acoustic initialized parameters
	String sound_fname;
	String simbol;
	Boolean new_sentence;

	String config_file;
	t_index number=0;
    t_index sample_rate;
	
	GenericStaticHmm::HypoList ris;	  // partial and complete decoded

	// *************************************************************
	//															   *
	//				KEEP CONFIGURATION FILE:	*.INI			   *
	//															   *
	// *************************************************************

	if ( argc!=2)
		{
		mwarn<<"No configfile name specified, using: res.ini";
		config_file="res.ini";
		}
	else config_file=argv[1];
		
	// *******************************************************
	//
	//					CONFIGURE FROM CONFIGURATION FILE
	// 					
	// 				RECOGNITION OPTIONS, DBASE, FEATURES
	//
	// *******************************************************

	options.SetOptions(config_file);
	dbase.Configure(config_file, TRUE);
	hmm.Initialise_Structures(config_file, options, dbase);
	dbase.Get_Utter_FName(sound_fname);
	dbase.Get_Label_List_Of_Actual_Utterance(utter_label_list);
    sample_rate=dbase.Smp_Rate();
	hmm.Build_Grammar_Over_Phonemes(utter_label_list, options.sil_opt, dbase);

    if (options.decoding_algo==BeamSearch)
		{
		while (tree.Viterbi_Beam(ris, options.BeamCoefficient, new_sentence, hmm, dbase))
			{
	        if (options.out_style==RES)
				hmm.Dump_Utterance_Segmentation_Res(ris, sound_fname);
			else if (options.out_style==ARES)
					hmm.Dump_Utterance_Segmentation_Ares(ris, sound_fname, sample_rate);

			mstat<<"Segmented sentence number: "<<number;
			number++;
		
			dbase.Get_Utter_FName(sound_fname);
			dbase.Get_Label_List_Of_Actual_Utterance(utter_label_list);
			hmm.Build_Grammar_Over_Phonemes(utter_label_list, options.sil_opt, dbase);
			ris.Reset();
			}
		}
		else{
			while (tree.Viterbi(ris, options.WindowWidth, options.TreeVolume, new_sentence, hmm, dbase))
				{ 
 				if (options.out_style==RES)
					hmm.Dump_Utterance_Segmentation_Res(ris, sound_fname);
				else if (options.out_style==ARES)
						hmm.Dump_Utterance_Segmentation_Ares(ris, sound_fname, sample_rate);

				mstat<<"Segmented sentence number: "<<number;
				number++;
		
				dbase.Get_Utter_FName(sound_fname);
				dbase.Get_Label_List_Of_Actual_Utterance(utter_label_list);
	
				hmm.Build_Grammar_Over_Phonemes(utter_label_list, options.sil_opt, dbase);
				ris.Reset();
				}
			}
		if (options.out_style==RES)
		hmm.Dump_Utterance_Segmentation_Res(ris, sound_fname);
		else if (options.out_style==ARES)
    			hmm.Dump_Utterance_Segmentation_Ares(ris, sound_fname, sample_rate);

		return 1;
		}

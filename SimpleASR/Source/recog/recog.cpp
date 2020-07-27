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
//  |   FILE:   recogniz.cpp                                            |
//  |   FUNCTIONALITY:  main of recognize module                        |
//  |   PROGRAM:                                                        |
//  |   COMMENTS:                                                       |
//  |   CONTRIBUTIONS: Domenico		                                    |
//  |   ACTUAL REVISION: 6.0                                            |
//  |   DATA ACTUAL REVISION: 19/11/98                                  |
//  |   FIRST VERSION:   2.0                                            |
//  |   DATA FIRST VERSION: 22/4/96                                     |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________

#define RECOGNIZE_VERSION "V.6.0 of 19/11/98 " ## __FILE__ " c." ## __DATE__

#include "../recog/hypolist.h"
#include "../recog/hypolist.hpp"
#include "../recog/recopt.h"
#include "../ioclass/soundlab.h"
#include "../features/feature.h"
#include "../lessico/lessico.h"
#include "../lessico/lexopt.h"



int main(int argc, char* argv[])
	{
	SpectralHmm hmm;	// static structure
 	HypoTree tree;		// dinamic structure  

	RecogOptions recog_options;
	
    GenericStaticHmm::HypoList ris;

	DbaseVoc dbase;

	String config_file_name;
	String temp;
	String recog_f_name;
	String symbol;

	t_index i;
    t_index sil_index;
	t_index ris_dim;

	ofstream recog_file;
	Boolean new_sentence;

    if ( argc!=2)
		{
		mwarn<<"No config file name specified, using \"res.ini\""<<endl;  
		config_file_name="res.ini";
		}
	else{
		config_file_name=argv[1];	
		}
														 
	recog_options.Set_Options(config_file_name);
	dbase.Configure(config_file_name, TRUE);
 	hmm.Configure(recog_options, dbase, sil_index, config_file_name);

	//open file of solutions
	recog_f_name = recog_options.file_of_results; 
	recog_file.open(recog_f_name);

    mwarn <<"Start of recognition"<<endl;

    switch (recog_options.algorithm)
		{
	   
		case 0: // if (recog_options.algorithm==BeamSearch)
				while (tree.Viterbi_Beam (ris, recog_options.beam_initial,
			           recog_options.beam_internal, 
			           recog_options.tree_volume, new_sentence, hmm, 
					   sil_index, dbase))
					{ 
					ris_dim=ris.Dim();
					for (i=0; i<ris_dim; i++)
						{
						if (ris[i].new_unit)
							{	  
							if (recog_options.grammar == PhonGrammar)
								dbase.Translate_Symbol(symbol, ris[i].state[1]);

							if (recog_options.grammar == WordGrammar)
								symbol = hmm.top_level_symbols[ris[i].state[2]];
			           
							recog_file<<symbol<<" ";
							}
						} //end of for (i=0; i<ris.Dim(); i++)

					if (new_sentence) 
					recog_file<<endl;

					ris.Reset();

		            } //end of while

				break;
			  
		case 1:	// if (recog_options.algorithm==WindowSearch)
			    while (tree.Viterbi_Window(ris, recog_options.window_width, 
			      recog_options.tree_volume, new_sentence, hmm, sil_index, dbase))
					{
					ris_dim=ris.Dim();
					for (i=0; i<ris_dim; i++)
						{
						if (recog_options.grammar == PhonGrammar)
							dbase.Translate_Symbol(symbol, ris[i].state[1]);

						if (recog_options.grammar == WordGrammar)
							symbol = hmm.top_level_symbols[ris[i].state[2]];
			           
						recog_file<<symbol<<" ";
						} //end of for (i=0; i<ris.Dim(); i++)

					if (new_sentence) 
						recog_file<<endl;

					ris.Reset();
		         
					} //end of while

			    break;
			  
		};//end of switch

    for (i=0; i<ris.Dim(); i++)
		{		
		if (ris[i].new_unit)
			{
			if (recog_options.grammar == PhonGrammar)
				dbase.Translate_Symbol(symbol, ris[i].state[1]);
			if (recog_options.grammar == WordGrammar)
				symbol=hmm.top_level_symbols[ris[i].state[2]];
			recog_file<<symbol<<" ";
			}	
		}

    ris.Reset();
	recog_file.close();	

    mwarn <<"End of recognition"<<endl;
		
    if (recog_options.grammar == PhonGrammar)
		dbase.Dump_File_Of_Sentence_List_Transcriptions(recog_options.file_of_solutions);
    else //if (recog_options.grammar == WordGrammar)
		{
		LexiconAtis lexic;
		LexiconOptions lexicon_options;

		lexicon_options.Set_Options(config_file_name);
		lexic.Write_Solution_File (recog_options.file_of_solutions, lexicon_options.file_recog);
		}

    mstat<<"Test is ok!";
	return 1;
	}

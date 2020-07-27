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
//  |   FILE:   Emb_Train.cpp                                           |
//  |   FUNCTIONALITY: Main function for Embedded Training				|
//  |   PROGRAM:														|
//  |   COMMENTS:                                                       |
//  |   CONTRIBUTIONS: Fabrizio											|
//  |   ACTUAL REVISION: 6.0                                            |
//  |   DATA ACTUAL REVISION: 19/11/98                                  |
//  |   FIRST VERSION:          4.0                                     |
//  |   DATA FIRST VERSION:		19/12/97	                            |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________


#include "../embedded/emb_b_w.h"
#include "../ioclass/soundlab.h"


int main(int argc, char* argv[])
	{
	ModelsSimultaneousTraining model_embtrain;
	String config_file;

	// ****************************************************************
	//
	//			OPEN CONFIG FILE, CONFIGURE TRAINING PROCEDURE
	//
	// ****************************************************************

	if ( argc!=2)
		{
		mstat<<"No configfile name specified, using: ires.ini";
		config_file="res.ini";
		}
	else{
		config_file=argv[1];
		}

	model_embtrain.Configure(config_file);

	model_embtrain.Symbol_Models_Optimisation();

	model_embtrain.Store_HMM_Models();
	
	return 1;

	}
